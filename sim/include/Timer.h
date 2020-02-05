/**
 * Copyright (c) 2020 FRC Team 3260
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
            _prevTimestamp(startTimestamp), _running(false),
            _countdownFrom(countdownFrom), _countdownTime(countdownFrom), _prevSignal(0) {};

    void start(double startTimestamp) { _running = true; _prevTimestamp = startTimestamp; }

    void stop() { _running = false; }

    void reset() { _running = false; _countdownTime = _countdownFrom; }

    void update(double currTimestamp)
    {
        if (_running)
        {
            double elapsedTime = currTimestamp - _prevTimestamp;
            _countdownTime -= elapsedTime;
            if (_countdownTime <= 0)
            {
                _countdownTime = 0;
                _running = false;
            }
            _prevTimestamp = currTimestamp;
        }
    }

    void processCommand(int currSignal, double timestamp)
    {
        if (currSignal == 0 && _prevSignal == 1)
        {
            if (_running)
            {
                stop();
            }
            else
            {
                start(timestamp);
            }
        }
        _prevSignal = currSignal;
    }

    bool isRunning() { return _running; }

    double getValue() { return _countdownTime; }

private:
    int _prevSignal;
    double _prevTimestamp; // Time at which timer was last updated
    bool _running; // True for counting down, false to stop counting down
    double _countdownTime;
    double _countdownFrom;
};


#endif //ROBOT_SIM_TIMER_H
