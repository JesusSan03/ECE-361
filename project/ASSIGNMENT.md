# ECE 361 Project 6 - Irrigation Controller

## Project Overview

For this project, I am making a watering system that controls three garden zones. It uses sensors and switches to decide when to water. The program runs on my computer using the iom361 hardware simulator.

## How It Works

- Press START to begin watering.
- Each zone gets watered for 120 seconds.
- The system waits 60 seconds between zones.
- If it rains or humidity reaches 90%, watering stops.
- After the rain ends, the interrupted zone starts over.
- After all three zones finish, the system goes back to IDLE.

## Main Parts

- sensor: Reads temperature and humidity.
- fsm: Controls the watering states.
- store: Saves readings using timestamps.
- aux: Keeps the newest 16 readings.
- report: Finds large changes in humidity.
- main: Runs the whole program.

## Project Requirements

The program needs to use C, memory-mapped registers, a state machine, a binary search tree, and a circular buffer.

It also needs to save readings, print timestamps, run scenario files, and pass tests.

## Testing

I need to test normal watering, rain interruptions, sensor thresholds, switch changes, buffer wrap-around, humidity reports, and midnight.

The program should compile without warnings and pass all its tests.

## Project Plan

Week 7: Finish the circular buffer and earlier project parts.

Week 8: Add the binary search tree.

Week 9: Finish the report and remaining tests.

Week 10: Fix any problems and finish the project.

The project is due December 6, 2026.