#ifndef GAME_HEADER
#define GAME_HEADER
#include "config.hpp"
#include "pair.hpp"
#include "display.hpp"

class Game {
    private:    static Pair<uint16_t, uint16_t> s_ball;
    private:    static Pair<int16_t, int16_t> s_ballVelocity;
    private:    static Pair<uint8_t, uint8_t> s_score;
    // private:    static Pair<uint16_t, uint16_t> s_leftPaddle;
    // private:    static Pair<uint16_t, uint16_t> s_rightPaddle;
    // private:    static constexpr 

    // public:     explicit Game();

    public:     static void tick(Pair<uint16_t, uint16_t>* const t_leftPaddle, Pair<uint16_t, uint16_t>* const t_rightPaddle);
    public:     static Pair<uint8_t, uint8_t>& getScore();
    public:     static Pair<uint16_t, uint16_t>& getBall();
    public:     static void throwBall(uint8_t t_direction);
    // public:     static void setPaddles(Pair<uint16_t, uint16_t> const t_leftPaddle, Pair<uint16_t, uint16_t> const t_rightPaddle);
    
    private:    static void updateScore(boolean t_left);
};

#endif