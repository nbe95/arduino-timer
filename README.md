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
find it very useful and often use it in my private projects when it comes down
to timing several tasks on microcontrollers etc.

## Features

- `set/getDuration`
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

## Example

```cpp
#include "src/lib/timer/src/timer.h"

// Create a timer with or without a specific duration...
Timer my_timer;
Timer another_timer(3000);

void setup() {
    // ...or provide a duration when starting the timer...
    my_timer.start(1000);
    // ...or set/change it later anytime you want
    my_timer.setDuration(2000);

    // Check if `my_timer` is set and running
    if (my_timer) { // Short for `my_timer.isSet()`
        Serial.println("`my_timer` is set, i.e. has got a duration.")
    }
    if (my_timer.isRunning()) {
        Serial.println("... and has been started!")
    }
}

void loop() {
    // Easy check for repetitive tasks
    if (my_timer.checkAndRestart()) {
        Serial.println("Yay, 2 seconds have passed!");

        // After this instruction, `another_timer` will be stopped and won't
        // have a duration any more
        another_timer.reset();
    }

    // Check if `another_timer` is set
    if (!another_timer) {   // Short for !another_timer.isSet()
        another_timer.start(500);
        Serial.println("Setting `another_timer` to 1/2s...");
    }

    // Just check once without resetting
    if (another_timer.check()) {
        Serial.println("Congratulations, 500ms have elapsed! This should only be printed once.");
    }
}
```
