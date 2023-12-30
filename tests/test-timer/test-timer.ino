// Copyright (c) 2022 Niklas Bettgen

#include <AUnitVerbose.h>
#include "./src/timer.h"


class ManipulableTimer : public Timer {
 public:
    using   Timer::Timer;
    void    setFixedCurrentTime(const ms current_time) { m_current_time_fixed = current_time; }
    ms      getCurrentTime() const override { return m_current_time_fixed; }
 private:
    ms      m_current_time_fixed = 0;
};


test(TimingAndState) {
    auto timer = Timer();

    assertEqual(timer.getDuration(), 0u);
    assertFalse(timer.check());
    assertFalse(timer.isRunning());
    assertFalse(timer.isSet());
    assertEqual(timer.getElapsedTime(), 0u);
    assertNear(timer.getElapsedTimeRel(), 0.0, 0.01);

    timer.setDuration(8000);
    assertEqual(timer.getDuration(), 8000u);
    assertFalse(timer.check());
    assertFalse(timer.isRunning());
    assertTrue(timer.isSet());
    assertEqual(timer.getElapsedTime(), 0u);
    assertNear(timer.getElapsedTimeRel(), 0.0, 0.1);

    assertTrue(timer.start());
    delay(4000);
    assertFalse(timer.start());
    assertEqual(timer.getDuration(), 8000u);
    assertFalse(timer.check());
    assertTrue(timer.isRunning());
    assertTrue(timer.isSet());
    assertNear(timer.getElapsedTime(), 4000u, 10u);
    assertNear(timer.getElapsedTimeRel(), 0.5, 0.01);

    delay(6000);
    assertFalse(timer.start());
    assertEqual(timer.getDuration(), 8000u);
    assertTrue(timer.check());
    assertTrue(timer.isRunning());
    assertTrue(timer.isSet());
    assertNear(timer.getElapsedTime(), 10000u, 10u);
    assertNear(timer.getElapsedTimeRel(), 1.0, 0.01);

    timer.reset();
    assertEqual(timer.getDuration(), 8000u);
    assertFalse(timer.check());
    assertFalse(timer.isRunning());
    assertTrue(timer.isSet());
    assertNear(timer.getElapsedTime(), 0u, 10u);
    assertNear(timer.getElapsedTimeRel(), 0.0, 0.01);

    assertTrue(timer.start(3000));
    delay(2000);
    assertEqual(timer.getDuration(), 3000u);
    assertFalse(timer.check());
    assertTrue(timer.isRunning());
    assertTrue(timer.isSet());
    assertNear(timer.getElapsedTime(), 2000u, 10u);
    assertNear(timer.getElapsedTimeRel(), 0.667, 0.01);

    assertTrue(timer.restart());
    delay(2000);
    assertEqual(timer.getDuration(), 3000u);
    assertFalse(timer.check());
    assertTrue(timer.isRunning());
    assertTrue(timer.isSet());
    assertNear(timer.getElapsedTime(), 2000u, 10u);
    assertNear(timer.getElapsedTimeRel(), 0.667, 0.01);
    assertFalse(timer.checkAndRestart());

    delay(2000);
    assertEqual(timer.getDuration(), 3000u);
    assertTrue(timer.check());
    assertTrue(timer.isRunning());
    assertTrue(timer.isSet());
    assertNear(timer.getElapsedTime(), 4000u, 10u);
    assertNear(timer.getElapsedTimeRel(), 1.0, 0.01);
    assertTrue(timer.checkAndRestart());

    assertEqual(timer.getDuration(), 3000u);
    assertFalse(timer.check());
    assertTrue(timer.isRunning());
    assertTrue(timer.isSet());
    assertNear(timer.getElapsedTime(), 0u, 10u);
    assertNear(timer.getElapsedTimeRel(), 0.0, 0.01);

    delay(3100);
    assertEqual(timer.getDuration(), 3000u);
    assertTrue(timer.check());
    assertTrue(timer.isRunning());
    assertTrue(timer.isSet());
    assertNear(timer.getElapsedTime(), 3100u, 10u);
    assertNear(timer.getElapsedTimeRel(), 1.0, 0.01);
}

test(BufferOverflow) {
    assertEqual(Timer::MaxValue + 1, 0u);

    // In case of a timer overflow, the actual µC time will be lost. This is
    // inevitable and thus acceptable as long as calculating the time difference
    // works with an overflow in between.
    auto timer = ManipulableTimer(6900);
    timer.setFixedCurrentTime(Timer::MaxValue - 3000);
    timer.start();

    // Seconds before overflow
    timer.setFixedCurrentTime(Timer::MaxValue - 3000);
    assertEqual(timer.getDuration(), 6900u);
    assertFalse(timer.check());
    assertTrue(timer.isRunning());
    assertTrue(timer.isSet());
    assertEqual(timer.getElapsedTime(), 0u);
    assertNear(timer.getElapsedTimeRel(), 0.0, 0.01);

    // Moment of overflow and afterwards
    timer.setFixedCurrentTime(Timer::MaxValue);
    assertEqual(timer.getDuration(), 6900u);
    assertFalse(timer.check());
    assertTrue(timer.isRunning());
    assertTrue(timer.isSet());
    assertEqual(timer.getElapsedTime(), 3000u);
    assertNear(timer.getElapsedTimeRel(), 0.43, 0.01);

    timer.setFixedCurrentTime(Timer::MaxValue + 1);
    assertEqual(timer.getDuration(), 6900u);
    assertFalse(timer.check());
    assertTrue(timer.isRunning());
    assertTrue(timer.isSet());
    assertEqual(timer.getElapsedTime(), 3001u);
    assertNear(timer.getElapsedTimeRel(), 0.43, 0.01);

    timer.setFixedCurrentTime(Timer::MaxValue + 2);
    assertEqual(timer.getDuration(), 6900u);
    assertFalse(timer.check());
    assertTrue(timer.isRunning());
    assertTrue(timer.isSet());
    assertEqual(timer.getElapsedTime(), 3002u);
    assertNear(timer.getElapsedTimeRel(), 0.43, 0.01);

    timer.setFixedCurrentTime(Timer::MaxValue + 3000);
    assertEqual(timer.getDuration(), 6900u);
    assertFalse(timer.check());
    assertTrue(timer.isRunning());
    assertTrue(timer.isSet());
    assertEqual(timer.getElapsedTime(), 6000u);
    assertNear(timer.getElapsedTimeRel(), 0.87, 0.01);

    timer.setFixedCurrentTime(Timer::MaxValue + 4000);
    assertEqual(timer.getDuration(), 6900u);
    assertTrue(timer.check());
    assertTrue(timer.isRunning());
    assertTrue(timer.isSet());
    assertEqual(timer.getElapsedTime(), 7000u);
    assertNear(timer.getElapsedTimeRel(), 1.0, 0.01);

    // 'Underflow'
    timer.setFixedCurrentTime(Timer::MaxValue - 4000);
    assertEqual(timer.getDuration(), 6900u);
    assertTrue(timer.check());
    assertTrue(timer.isRunning());
    assertTrue(timer.isSet());
    assertEqual(timer.getElapsedTime(), Timer::MaxValue - 999u);
    assertNear(timer.getElapsedTimeRel(), 1.0, 0.01);

    timer.setDuration(Timer::MaxValue);
    assertEqual(timer.getDuration(), Timer::MaxValue);
    assertFalse(timer.check());
    assertTrue(timer.isRunning());
    assertTrue(timer.isSet());
    assertEqual(timer.getElapsedTime(), Timer::MaxValue - 999u);
    assertNear(timer.getElapsedTimeRel(), 1.0, 0.01);
}

test(CastingAndComparison) {
    auto timer1 = Timer();
    assertEqual((uint32_t)timer1, (uint32_t)0);
    assertFalse(timer1);

    auto timer2 = Timer(69);
    assertEqual((uint32_t)timer2, (uint32_t)69);
    assertTrue(timer2);

    auto timer3 = Timer(42);
    timer3.setDuration(69);
    assertTrue(timer1 != timer2);
    assertTrue(timer2 == timer3);
}


void setup() { aunit::TestRunner::setTimeout(20); }
void loop() { aunit::TestRunner::run(); }
