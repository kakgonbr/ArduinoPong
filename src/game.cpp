#include "game.hpp"

using PairF = Pair<float, float>;

PairF Game::s_ballVelocity = {0, 0};
PairF Game::s_ball = {Display::WIDTH / 2, Display::HEIGHT / 2};
Pair<uint8_t, uint8_t> Game::s_score = {0, 0};
int8_t Game::s_player1Name[Config::PLAYER_NAME_BUFFER];
int8_t Game::s_player2Name[Config::PLAYER_NAME_BUFFER];
bool Game::ended = 0;

void Game::handlePaddleCollision(const Pair<uint8_t, uint8_t> &paddle)
{
    float paddleCenterY = paddle.right + (Config::PADDLE_HEIGHT / 2.0f);

    float relativeIntersectY = (s_ball.right - paddleCenterY) / (Config::PADDLE_HEIGHT / 2.0f); // factor in ball
    if (relativeIntersectY < -1.0f)
        relativeIntersectY = -1.0f;
    if (relativeIntersectY > 1.0f)
        relativeIntersectY = 1.0f;

    float bounceAngle = relativeIntersectY * Config::MAX_BOUNCE_ANGLE;

    // to make it 1:1, gotta compute the speed
    float speed = sqrt(s_ballVelocity.left * s_ballVelocity.left + s_ballVelocity.right * s_ballVelocity.right);
    if (speed < 0.0001f)
        speed = 1.0f; // avoid division by zero

    // math, dont ask me how this works, just google how to get the angle of a vector
    float currentAngle = atan2(s_ballVelocity.right, fabs(s_ballVelocity.left));

    float combinedAngle = .5f * bounceAngle + .5f * currentAngle; // half of current angle, half of computed bounce angle

    // flip the ball velocity and give it a horizontal speed of speed * cos (DUHHHHHHHHHHH)
    s_ballVelocity.left = -copysign(speed * cos(combinedAngle), s_ballVelocity.left);
    // paddle collition, no need to flip vertical velocity
    s_ballVelocity.right = speed * sin(combinedAngle);
}
void Game::tick(const Pair<uint8_t, uint8_t> &t_leftPaddle, const Pair<uint8_t, uint8_t> &t_rightPaddle)
{
    PairF nextPos = {s_ball.left + s_ballVelocity.left, s_ball.right + s_ballVelocity.right};

    if (nextPos.right <= Config::PADDLE_PADDING_UPPER || nextPos.right >= Display::HEIGHT - Config::PADDLE_PADDING_LOWER + 1)
    {
        nextPos.right -= s_ballVelocity.right;
        s_ballVelocity.right = -s_ballVelocity.right;
        nextPos.right += s_ballVelocity.right;
    }

    // Check for scoring: ball passes left or right boundaries
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

    // since we switched to floating point, the collision is a littleeeeeeee wonky without checking if the x is in a range.
    if (nextPos.left <= t_leftPaddle.left + 1 && nextPos.left >= t_leftPaddle.left - 1 && t_leftPaddle.right + Config::PADDLE_HEIGHT >= nextPos.right && t_leftPaddle.right <= nextPos.right)
    {
        // undo adding left of velocity
        nextPos.left -= s_ballVelocity.left;
        handlePaddleCollision(t_leftPaddle);
        // add it back
        nextPos.left += s_ballVelocity.left;
    }
    else if (nextPos.left <= t_rightPaddle.left + 1 && nextPos.left >= t_rightPaddle.left - 1 && t_rightPaddle.right + Config::PADDLE_HEIGHT >= nextPos.right && t_rightPaddle.right <= nextPos.right)
    {
        nextPos.left -= s_ballVelocity.left;
        handlePaddleCollision(t_rightPaddle);
        nextPos.left += s_ballVelocity.left;
    }

    // Update the ball position
    s_ball = nextPos;
}

Pair<uint8_t, uint8_t> &Game::getScore()
{
    return s_score;
}

PairF &Game::getBall()
{
    return s_ball;
}

void Game::throwBall(uint8_t t_direction)
{
    int8_t xModifier = 1; // this was uint8_t??????? how did it work even?
    int8_t yModifier = 1; // top right
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
    s_ball = {Display::WIDTH / 2.f, Display::HEIGHT / 2.f};
    s_ballVelocity = {static_cast<float>(Config::BALL_RATE.left * xModifier),
                      static_cast<float>(Config::BALL_RATE.right * yModifier)};
}

void Game::preGame()
{
    s_score = {0, 0};

    const char *message = "\nEnter Player 1 Name:\n";
    memcpy(Bluetooth::s_buffer, message, 23);
    Bluetooth::writeFromBuffer();

    while (!Bluetooth::readAvailable())
    {
        Bluetooth::readToBuffer();
    }

    memcpy(s_player1Name, Bluetooth::s_buffer, Config::PLAYER_NAME_BUFFER);
    Bluetooth::resetBuffer();

    memcpy(Bluetooth::s_buffer, "Enter Player 2 Name:\n", 22);
    Bluetooth::writeFromBuffer();

    while (!Bluetooth::readAvailable())
    {
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
        const char *message = "\nWinner:\n";
        memcpy(Bluetooth::s_buffer, message, 10);
        Bluetooth::writeFromBuffer();
        memcpy(Bluetooth::s_buffer, s_score.left > s_score.right ? s_player1Name : s_player2Name, Config::PLAYER_NAME_BUFFER);
        Bluetooth::writeFromBuffer();
        ended = 1;
    }
}
