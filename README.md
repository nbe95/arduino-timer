# Arduino Timer Module

This is a basic OOP timer implementation for easy low-level handling of
schedules or repetitive tasks on embedded hardware. [Scroll down](#example-code)
for a meaningful example.

## Why?

The code is designed to provide a **simple and intuitive interface** and
therefore help you develop quickly and without pain. To use it, simply create a
Timer object and think of it as a **stopwatch**: You can start, stop, set a
desired total duration or check the elapsed time at any time.

Each timer internally uses `uint32_t` values as returned by `millis()` to store
the duration and starting time. Given this technical limitation, any duration of
**up to almost 50 days** (4,294,967,295 ms) can be handled, which should be
sufficient for most task running on embedded hardware.

> Pro tip: The method `checkAndRestart()` comes in very handy when creating a
repetitive task. Used as a one-liner with an if-clause, it checks whether it's
time to perform a specific task or not, and if so, it automatically restarts the
timer for the next run. Nice. :tada:

The timer module is **non-blocking**. It has a very simple structure and since
only discrete time events are captured, there's **no need** for a periodically
called `tick()` method or something similar. Plus, timer objects automatically
handle **timestamp subtraction/overflow** on their own. I thus find it very
handy and often use it in my private projects when it comes down to timing
several tasks on a microcontroller etc.

## Features

All Timer objects created a compile time or runtime are independent of each
other; you can create as much of them as you need (or your RAM can hold).

Thinking of a simple stopwatch, each timer object provides a fairly intuitive
interface.

- `Timer` (constructor): Creates a Timer object. If a duration is provided as
  argument, it will be immediately applied. (However, the timer will not be
  started yet.)
- `set/getDuration`: Sets or gets the desired duration as `uint32_t` in ms for
  this timer.
- `start`: The most important function: Starts the timer. Has no effect on a
  timer that's already running. If a duration is provided as argument, it will
  be set accordingly. Returns `true` if the timer was successfully started,
  otherwise `false`.
- `restart`: Acts exactly like the `start` method, but (re)starts the timer even
  if it was already running.
- `reset`: Stops the timer, keeping the configured duration.
- `check`: Returns `true` if the timer is running and the elapsed time since
  start exceeds the specified duration, otherwise `false`.
- `checkAndRestart`: Acts exactly like the `check` method. If the result was
  `true`, the timer will be immediately restarted. Perfect for cyclic tasks.
- `isRunning`: Returns `true` if a duration was set and the timer was started,
  otherwise `false`.
- `isSet`: Returns `true` if a duration was set.
- `getStartTime`: Returns the time value as `uint32_t` captured when the timer
  was started in ms.
- `getElapsedTime`: Returns the total elapsed time difference since the timer
  was started as `uint32_t` in ms.
- `getElapsedTimeRel`: Returns the relative elapsed time difference since the
  timer was started as `float` in a range from 0 to 1 (0% to 100%).

When compared, two timer objects are considered equal if both their individual
durations are equal.

A Timer object that is casted to a `bool` will evaluate to `true` if a duration
was set, otherwise `false`. This can be useful e.g. for a quick check in a
single if-clause.

## Quick start

Add both the timer cpp and header file to your source code, e.g. by cloning this
repository as a [git
submodule](https://git-scm.com/book/en/v2/Git-Tools-Submodules) under
`src/lib/` of your main sketch. Use the default git workflow to pull/upgrade
etc.

```sh
git clone https://github.com/nbe95/arduino-timer.git ./src/lib/timer/

```

> :information_source: In the Arduino universe, it is required that the code
resides under the `src` directory!

Then, simply include the `timer.h` in your sketch and you're ready to go.
Take a quick look at the following code to see how everything works.

### Example code

```cpp
#include "src/lib/timer/src/timer.h"

// Create a timer with or without a specific duration...
Timer my_timer;
Timer another_timer(3000);

void setup() {
    // ...or provide a duration when starting the timer...
    my_timer.start(1000);
    // ...or set/change it anytime you want
    my_timer.setDuration(2000);

    // Check if my_timer is set and running
    if (my_timer) { // Short for my_timer.isSet()
        Serial.println("my_timer is set, i.e. has got a duration.")
    }
    if (my_timer.isRunning()) {
        Serial.println("... and has been started!")
    }
}

void loop() {
    // Easy check for repetitive tasks
    if (my_timer.checkAndRestart()) {
        Serial.println("Yay, 2 seconds have passed!");

        // After this instruction, another_timer will be stopped and won't
        // have a duration any more
        another_timer.reset();
    }

    // Check if another_timer is set
    if (!another_timer) {   // Short for !another_timer.isSet()
        another_timer.start(500);
        Serial.println("Setting another_timer to 1/2s...");

        // Get elapsed time
        Serial.print("So far, ");
        Serial.print(my_timer.getElapsedTime());
        Serial.print(" ms have elapsed, which is ");
        Serial.print(my_timer.getElapsedTimeRel() * 100);
        Serial.println(" percent of the timer.");
    }

    // Just check once without resetting
    if (another_timer.check()) {
        Serial.print("Congratulations, another 500ms have elapsed! ");
        Serial.println("This message should be printed only once.");
    }
}
```
