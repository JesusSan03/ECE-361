# ECE 361 Project 6 - Irrigation Controller

## About My Project
This project is a watering system that controls three garden zones. It uses switches and sensors to decide when to water and when to stop.

Each zone waters for 120 seconds and waits 60 seconds before moving to the next zone. If it rains or humidity gets too high, the system stops watering until the weather gets better.

## What Works So Far

- The START switch begins the watering cycle.
- The system controls all three zones.
- Rain and high humidity stop watering.
- The sensor reads temperature and humidity.
- The program keeps track of time and saves readings.
- The circular buffer stores the newest 16 readings.
- The program can run different test scenarios.

## How to Run

Open the terminal inside the project folder.

Build the program:

```
make
```

Run the tests:

```
make test
```

Run a normal watering scenario:

```
./irrigation scenarios/normal.txt
```

Run a rain scenario:

```
./irrigation scenarios/rain.txt
```

Check for memory problems:

```
make test-sanitize
```

Remove built files:

```
make clean
```

## Main Files

- sensor.c: Reads the sensors.
- fsm.c: Controls the watering states.
- store.c: Saves the readings.
- aux.c: Keeps the newest 16 readings.
- main.c: Runs the program.
- report.c: Will be finished in Week 9.

The iom361 files are the hardware simulator provided by the professor. I kept those files unchanged.

## Testing

The current version has 13 checks, including unit tests and scenario tests. They passed using GCC on Linux.

## What Is Left

In Week 8, I will change the store into a binary search tree. In Week 9, I will finish the humidity report and add the remaining tests.

This is my Week 7 version, so the whole project is not finished yet.