# Project 6 - Irrigation Controller

## Device Description

My project is an irrigation controller that waters three different areas in a garden. It waters one area at a time and waits a little before watering the next one. If it starts raining, the system will stop watering until the rain is gone.

## States

IDLE --> The system is doing nothing and waiting to start.

ZONE_ACTIVE --> The system is watering one of the three areas.

SOAK --> The system waits for the water to soak into the ground before moving to the next area.

RAIN_HOLD --> The system stops watering because it is raining or the humidity is too high.

## Events

START_PRESSED --> Someone presses the start switch to begin watering.

RAIN_DETECTED --> The rain switch is on or the humidity is 90% or higher.

RAIN_CLEARED --> The rain has stopped and the humidity is below 90%.

WATERING_COMPLETE --> One area has been watered for 120 seconds.

SOAK_COMPLETE --> The system has waited 60 seconds and can move to the next area.

CYCLE_COMPLETE --> All three areas have been watered and the system is done.

## Hardware

The system has 2 switches and 3 LEDs.

Switch 0 --> Starts the watering process.

Switch 1 --> Tells the system when it is raining.

LED 0 --> Shows when area 1 is being watered.

LED 1 --> Shows when area 2 is being watered.

LED 2 --> Shows when area 3 is being watered.

## Future Work

Later on, I will add the code that controls how the system moves between states. I will also add the sensor readings, a way to save data, a circular buffer, and a report that checks for big changes in humidity.
