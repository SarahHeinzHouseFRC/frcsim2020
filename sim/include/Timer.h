/**
 * Copyright (c) 2019 FRC Team 3260
 */

#ifndef ROBOT_SIM_TIMER_H
#define ROBOT_SIM_TIMER_H


class Timer
{
public:
    /**
     * Constructor
     * @param startTimestamp Time at initialization (epoch sec)
     * @param countdownFrom Time to countdown from (sec)
     */
    Timer(double startTimestamp, double countdownFrom) :
            _startTimestamp(startTimestamp), _running(false),
            _countdownFrom(countdownFrom), _countdownTime(countdownFrom) {};

    void start() { _running = true; }

    void stop() { _running = false; }

    void reset() { _running = false; _countdownTime = _countdownFrom; }

    void update(double currTimestamp)
    {
        if (_running)
        {
            double elapsedTime = currTimestamp - _startTimestamp;
            _countdownTime = _countdownFrom - elapsedTime;
            if (_countdownTime <= 0)
            {
                _countdownTime = 0;
                _running = false;
            }
        }
    }

    double getValue() { return _countdownTime; }

private:
    double _startTimestamp;
    bool _running; // True for counting down, false to stop counting down
    double _countdownTime;
    double _countdownFrom;
};


#endif //ROBOT_SIM_TIMER_H
