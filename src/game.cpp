#include "game.hpp"

Pair<int8_t, int8_t> Game::s_ballVelocity = {0, 0};
Pair<uint8_t, uint8_t> Game::s_ball = {Display::WIDTH / 2, Display::HEIGHT / 2};
Pair<uint8_t, uint8_t> Game::s_score = {0, 0};
int8_t Game::s_player1Name[Config::PLAYER_NAME_BUFFER];
int8_t Game::s_player2Name[Config::PLAYER_NAME_BUFFER];
bool Game::ended = 0;

void Game::tick(Pair<uint8_t, uint8_t> const t_leftPaddle, Pair<uint8_t, uint8_t> const t_rightPaddle)
{
    Pair<uint8_t, uint8_t> nextPos = {s_ball.left + s_ballVelocity.left, s_ball.right + s_ballVelocity.right};
    if (nextPos.right <= Config::PADDLE_PADDING_UPPER 
        || nextPos.right >= Display::HEIGHT - Config::PADDLE_PADDING_LOWER + 1)
    {
        nextPos.right -= s_ballVelocity.right;
        s_ballVelocity.right = -s_ballVelocity.right;
        nextPos.right += s_ballVelocity.right;
    }

    if (nextPos.left <= 0)
    {
        updateScore(false);
        throwBall(static_cast<uint8_t>(random(4)));
        return;
    } 
    else if (nextPos.left >= Display::WIDTH - 1)
    {
        updateScore(true);
        throwBall(static_cast<uint8_t>(random(4)));
        return;
    }


    if ((nextPos.left == t_leftPaddle.left && t_leftPaddle.right + Config::PADDLE_HEIGHT >= nextPos.right && t_leftPaddle.right <= nextPos.right)
        || (nextPos.left == t_rightPaddle.left && t_rightPaddle.right + Config::PADDLE_HEIGHT >= nextPos.right && t_rightPaddle.right <= nextPos.right))
    {
        nextPos.left -= s_ballVelocity.left;
        s_ballVelocity.left = -s_ballVelocity.left;
        nextPos.left += s_ballVelocity.left;
    }

    s_ball = nextPos;
}

Pair<uint8_t, uint8_t> &Game::getScore()
{
    return s_score;
}

Pair<uint8_t, uint8_t> &Game::getBall()
{
    return s_ball;
}

void Game::throwBall(uint8_t t_direction)
{
    uint8_t xModifier = 1;
    uint8_t yModifier = 1; // top right
    switch (t_direction % 4)
    {
        case 1:
            yModifier = -1; // bottom right
        break;
        case 2:
            xModifier = -1;
            yModifier = -1; // bottom left
        break;
        case 3:
            xModifier = -1; // top left
        break;
    }
    s_ball = {Display::WIDTH / 2, Display::HEIGHT / 2};
    s_ballVelocity = {static_cast<int8_t>(Config::BALL_RATE.left * xModifier),
                        static_cast<int8_t>(Config::BALL_RATE.right * yModifier)};
}

void Game::preGame()
{
    s_score = {0, 0};

    const char* message = "\nEnter Player 1 Name:\n";
    memcpy(Bluetooth::s_buffer, message, 23);
    Bluetooth::writeFromBuffer();

    while (!Bluetooth::readAvailable()) {
        Bluetooth::readToBuffer();
    }

    memcpy(s_player1Name, Bluetooth::s_buffer, Config::PLAYER_NAME_BUFFER);
    Bluetooth::resetBuffer();
    
    memcpy(Bluetooth::s_buffer, "Enter Player 2 Name:\n", 22);
    Bluetooth::writeFromBuffer();
    
    while (!Bluetooth::readAvailable()) {
        Bluetooth::readToBuffer();
    }
    
    memcpy(s_player2Name, Bluetooth::s_buffer, Config::PLAYER_NAME_BUFFER);
    Bluetooth::resetBuffer();

    message = "\nPlayer 1:\n";
    memcpy(Bluetooth::s_buffer, message, 12);
    Bluetooth::writeFromBuffer();
    memcpy(Bluetooth::s_buffer, s_player1Name, Config::PLAYER_NAME_BUFFER);
    Bluetooth::writeFromBuffer();
    message = "\nPlayer 2:\n";
    memcpy(Bluetooth::s_buffer, message, 12);
    Bluetooth::writeFromBuffer();
    memcpy(Bluetooth::s_buffer, s_player2Name, Config::PLAYER_NAME_BUFFER);
    Bluetooth::writeFromBuffer();
}

// void Game::setPaddles(Pair<uint8_t, uint8_t> const t_leftPaddle, Pair<uint8_t, uint8_t> const t_rightPaddle)
// {
//     s_leftPaddle = t_leftPaddle;
//     s_rightPaddle = t_rightPaddle;
// }

void Game::updateScore(boolean t_left)
{
    // Serial.print(t_left);
    s_score.left += static_cast<uint8_t>(t_left);
    s_score.right += static_cast<uint8_t>(!t_left);

    if (s_score.left >= 3 || s_score.right >= 3)
    {
        const char* message = "\nWinner:\n";
        memcpy(Bluetooth::s_buffer, message, 10);
        Bluetooth::writeFromBuffer();
        memcpy(Bluetooth::s_buffer, s_score.left > s_score.right ? s_player1Name : s_player2Name, Config::PLAYER_NAME_BUFFER);
        Bluetooth::writeFromBuffer();
        ended = 1;
    }
}
