/*
  ==============================================================================

    OneShotTimerCallback.h
    Created: 29 Nov 2023 3:13:50pm
    Author:  NeZvers

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class OneShotTimeCallback : public Timer {
public:
    OneShotTimeCallback(int ms, std::function<void()> fn) {
        callback = fn;
        startTimer(ms);
    }
    ~OneShotTimeCallback() {
        stopTimer();
    }
    void timerCallback() override {
        std::function<void()> f = callback;
        delete this;
        f();
    }
private:
    std::function<void()> callback;
};
