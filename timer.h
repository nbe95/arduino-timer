// Copyright (c) 2022 Niklas Bettgen

#include "Arduino.h"

#ifndef NBE95_ARDUINO_TIMER_H_
#define NBE95_ARDUINO_TIMER_H_


// Timer class for easy time handling and scheduling
class Timer {
 public:
    // Static constants and type definitions
    typedef uint32_t ms;
    static constexpr ms MaxValue = 0xFFFFFFFF;
    static constexpr ms InvalidDuration = 0;

    // Constructor for a timer with or without specific duration
    explicit Timer(const ms duration = InvalidDuration) {
        setDuration(duration);
    }

    // Set the timer duration
    void setDuration(const ms duration) {
        if (duration != m_duration) {
            m_duration = duration;
        }
    }

    // Reset the timer
    void reset() {
        m_start_time = 0;
        m_running = false;
    }

    // Start the timer if not done yet
    bool start(const ms duration = InvalidDuration) {
        if (duration != InvalidDuration) {
            setDuration(duration);
        }

        if (isRunning()) {
            return false;
        }

        m_start_time = getCurrentTime();
        m_running = true;
        return true;
    }

    // Reset and, if provided, restart the timer with a new duration
    bool restart(const ms duration = InvalidDuration) {
        reset();
        return start(duration);
    }

    // Get the configured timer duration
    ms getDuration() const {
        return m_duration;
    }

    // Check whether the specified duration is already expired
    bool check() const {
        return isRunning() && getElapsedTime() > getDuration();
    }

    // Perform a check and immediately restarts the timer - useful for cyclic tasks
    bool checkAndRestart() {
        if (check()) {
            restart();
            return true;
        }
        return false;
    }

    // Check if the timer is currently running
    bool isRunning() const {
        return m_running;
    }

    // Check if a valid duration has been set
    bool isSet() const {
        return getDuration() != InvalidDuration;
    }

    // Return the time when the timer was started
    ms getStartTime() const {
        return isRunning() ? m_start_time : 0;
    }

    // Return the amount of elapsed milliseconds
    ms getElapsedTime() const {
        return isRunning() ? getCurrentTime() - getStartTime() : 0;
    }

    // Return the amount of relative elapsed time as a float value between 0 and 1
    float getElapsedTimeRel() const {
        if (!getDuration()) {
            return 0;
        }
        float p = (float)getElapsedTime() / getDuration();
        return max((float)0, min(p, (float)1));   //NOLINT
    }

    // Internal function to retrieve the current µC time
    ms getCurrentTime() const { return millis(); }

    // Comparison operators
    bool operator==(const Timer& other) const { return getDuration() == other.getDuration(); }
    bool operator!=(const Timer& other) const { return getDuration() != other.getDuration(); }

    // Conversion operators
    explicit operator bool() const  { return isSet(); }
    operator uint32_t() const       { return (uint32_t)m_duration; }

 protected:
    bool        m_running = false;              // Flag indicating that the timer is running
    ms          m_start_time = 0;               // Start time captured when the timer was started
    ms          m_duration = InvalidDuration;   // Duration of the timer
};


#endif  // NBE95_ARDUINO_TIMER_H_
