#include "game.hpp"

Pair<int8_t, int8_t> Game::s_ballVelocity = {0, 0};
Pair<uint8_t, uint8_t> Game::s_ball = {Display::WIDTH / 2, Display::HEIGHT / 2};
Pair<uint8_t, uint8_t> Game::s_score = {0, 0};

void Game::tick(Pair<uint8_t, uint8_t> const t_leftPaddle, Pair<uint8_t, uint8_t> const t_rightPaddle)
{
    Pair<uint8_t, uint8_t> nextPos = {s_ball.left + s_ballVelocity.left, s_ball.right + s_ballVelocity.right};
    // Serial.print("NEXT POS: ");
    // Serial.print(nextPos.left);
    // Serial.print(", ");
    // Serial.println(nextPos.right);
    if (nextPos.right <= Config::PADDLE_PADDING_UPPER 
        || nextPos.right >= Display::HEIGHT - Config::PADDLE_PADDING_LOWER + 1)
    {
        // Serial.print("VERTICAL COLLISION: ");
        // Serial.print(nextPos.right);
        // Serial.print(" AFTER: ");
        nextPos.right -= s_ballVelocity.right;
        s_ballVelocity.right = -s_ballVelocity.right;
        nextPos.right += s_ballVelocity.right;
        // Serial.println(nextPos.right);
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

    // Serial.print("Left Paddle: ");
    // Serial.print(t_leftPaddle.left);
    // Serial.print(", ");
    // Serial.println(t_leftPaddle->right);


    if ((nextPos.left == t_leftPaddle.left && t_leftPaddle.right + Config::PADDLE_HEIGHT >= nextPos.right && t_leftPaddle.right <= nextPos.right)
        || (nextPos.left == t_rightPaddle.left && t_rightPaddle.right + Config::PADDLE_HEIGHT >= nextPos.right && t_rightPaddle.right <= nextPos.right))
    {
        // Serial.print("PADDLE COLLISION");
        // Serial.print(nextPos.left);
        // Serial.print(" AFTER: ");
        nextPos.left -= s_ballVelocity.left;
        s_ballVelocity.left = -s_ballVelocity.left;
        nextPos.left += s_ballVelocity.left;
        // Serial.println(nextPos.left);
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
    // Serial.print("Score: Left: ");
    // Serial.print(s_score.left);
    // Serial.print(" Right: ");
    // Serial.println(s_score.right);
}
