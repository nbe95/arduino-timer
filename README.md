# Arduino Timer Module

This is a very basic OOP timer implementation for easy low-level handling of
schedules or repetitive tasks.

Simply create a `Timer` object and think of it as a stopwatch. You can set a
duration, start, stop and check it at any time. Each timer object uses a
`uint32_t` value as returned by `millis()` to internally store the duration and
time. Given this limitation, durations of up to almost 50 days (4,294,967,295
ms) can be handled, which should be sufficient for most task running on embedded
hardware.

> Pro tip: The method `checkAndRestart()` comes in very handy when creating a
repetitive task. Used in an if-clause, it checks whether it's time to perform
the task or not, and if so, it automatically restarts the timer for the next
run.

Since this timer module automatically handles timestamp subtraction/overflow, I
find it very useful and often use it in my privsate projects when it comes down
to timing several tasks on microcontrollers etc.

## Features

- `set/GetDuration`
- `start`
- `restart`
- `reset`
- `check`
- `checkAndRestart`
- `isRunning`
- `isSet`
- `getStartTime`
- `getElapsedTime`
- `getElapsedTimeRel`

## Usage

Example:

```sh
git clone https://github.com/nbe95/arduino-timer.git ./lib/timer

```

Afterwards, add `#include "lib/timer/src/timer.h"` to your sketch and you're
ready to go.
