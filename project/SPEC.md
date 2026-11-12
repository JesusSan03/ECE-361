# ECE 361 Project 6 - SPEC

## Project Description
My project is an irrigation controller that waters three zones. It follows a set of steps and stops watering when it rains or humidity gets too high.

## States

The program uses four states:

- IDLE: Waiting for the START button.
- ZONE_ACTIVE: Watering one zone.
- SOAK: Waiting for the water to soak into the soil.
- RAIN_HOLD: Waiting for rain or high humidity to stop.

## State Diagram

```
IDLE
  |
  | START
  v
ZONE_ACTIVE
  |
  | 120 seconds
  v
SOAK
  |
  | 60 seconds
  v
ZONE_ACTIVE (next zone)
  |
  | After zone 3 finishes
  v
IDLE

ZONE_ACTIVE or SOAK
  |
  | Rain starts
  v
RAIN_HOLD
  |
  | Rain stops
  v
ZONE_ACTIVE
```

## Events and Transitions

The program uses events to change states.

| Current State | Event | Next State | Action |
|---|---|---|---|
| IDLE | START | ZONE_ACTIVE | Start zone 1 |
| ZONE_ACTIVE | WATER_DONE | SOAK | Stop watering and soak |
| ZONE_ACTIVE | LAST_ZONE_DONE | IDLE | Finish the cycle |
| SOAK | SOAK_DONE | ZONE_ACTIVE | Start next zone |
| ZONE_ACTIVE | RAIN_ON | RAIN_HOLD | Close valves |
| SOAK | RAIN_ON | RAIN_HOLD | Stop soaking |
| RAIN_HOLD | RAIN_OFF | ZONE_ACTIVE | Restart the zone |
| Any state | NONE or other event | Same state | Keep current state |

The START event only happens when the switch changes from off to on.

## Extra Rules I Chose

- Humidity at 90% or higher counts as rain.
- Pressing START during a cycle does nothing.
- The system will not start if it is raining.
- Rain stops watering immediately.
- After rain, the interrupted zone starts over.
- If rain happens during SOAK, the next zone starts after the rain.
- If rain happens again, the zone starts over again.
- Rain takes priority over the timer.
- The state changes when the timer reaches its required time.

## Sensor Readings

The sensor reads temperature and humidity. The values are converted from the raw registers and rounded to one decimal place.

The program saves the readings with UTC timestamps.

## Record Storage

Each record stores the sensor reading, state, and status word.

Right now I use a sorted array to store the records by timestamp. Duplicate timestamps are not allowed.

In Week 8, I will change this into a binary search tree. Since the timestamps arrive in order, a normal tree could become a long chain. I will need to deal with that problem when I make the tree.

## Circular Buffer

The circular buffer stores the newest 16 readings.

It uses an array, a head position, and a count. When the buffer becomes full, a new reading replaces the oldest one.

Adding and getting readings takes O(1) time.

A growing array could save every reading, but it would keep using more memory. Adding to a growing array usually takes O(1) time, but resizing can take O(n). My circular buffer uses a fixed amount of memory.

## Humidity Report

This part will be finished in Week 9.

The report will find humidity changes greater than 8 percentage points between two readings.

The first reading does not count because there is nothing before it.

The report will print the tick, reading, and humidity change. If nothing unusual happens, it will say there are no anomalies.

The report will use the store functions to access records.

## Status Word

The status word is a 32-bit number that stores information using different bits.

- Bits 0-1: Current state
- Bits 2-4: Valve LEDs
- Bits 5-6: Current zone
- Bits 7-31: Unused and set to zero

The state starts at 0 for IDLE. The zone is 0 when the controller is idle.

## RGB LED Colors

- IDLE: Blue
- ZONE_ACTIVE: Green
- SOAK: Yellow
- RAIN_HOLD: Red

## Time and the Year 2038

The program uses time_t to keep track of time in UTC.

If time_t used a signed 32-bit number, it would run out of space in January 2038. This could cause the time to become negative and mess up the program's timers.

Using 64-bit time_t avoids this problem.

## My Thoughts About the States

I think the four states work well because they are easy to understand. Each state tells the program what it should be doing. The only tricky part is RAIN_HOLD because the program needs to remember which zone was interrupted. I use a separate zone number instead of making more states.

## What Is Left

In Week 8, I need to add the binary search tree and decide how to handle timestamps arriving in order.

In Week 9, I need to finish the humidity report and the remaining tests.

After that, I will check everything again before submitting the final project.