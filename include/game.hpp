#ifndef GAME_HEADER
#define GAME_HEADER
#include "config.hpp"
#include "pair.hpp"
#include "display.hpp"
#include "bluetooth.hpp"

class Game {
    using PairF = Pair<float, float>;

    private:    static PairF s_ball;
    private:    static PairF s_ballVelocity;
    private:    static Pair<uint8_t, uint8_t> s_score;
    private:    static int8_t s_player1Name[Config::PLAYER_NAME_BUFFER];
    private:    static int8_t s_player2Name[Config::PLAYER_NAME_BUFFER];
    public:     static bool ended;
    
    // private:    static Pair<uint16_t, uint16_t> s_leftPaddle;
    // private:    static Pair<uint16_t, uint16_t> s_rightPaddle;
    // private:    static constexpr 

    // public:     explicit Game();

    public:     static void handlePaddleCollision(const Pair<uint8_t, uint8_t>& paddle);
    public:     static void tick(const Pair<uint8_t, uint8_t>& t_leftPaddle, const Pair<uint8_t, uint8_t>& t_rightPaddle);
    public:     static Pair<uint8_t, uint8_t>& getScore();
    public:     static PairF& getBall();
    public:     static void throwBall(uint8_t t_direction);
    public:     static void preGame();
    // public:     static void setPaddles(Pair<uint16_t, uint16_t> const t_leftPaddle, Pair<uint16_t, uint16_t> const t_rightPaddle);
    
    private:    static void updateScore(boolean t_left);
};

#endif