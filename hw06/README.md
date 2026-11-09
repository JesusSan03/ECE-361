# ECE 361 Homework 6
This homework uses sensors, switches, and LEDs. When a switch is pressed, its LED turns on or off.

## How to Run
Open the hw06 folder in VS Code and run these commands in the terminal:

```
make
./replay scenarios/basic.txt
make test
make clean
```

- `make` builds the program.
- `./replay scenarios/basic.txt` runs an example scenario.
- `make test` checks if everything works correctly.
- `make clean` removes the files made when building the program.

The tests also compare two scenarios with their expected results. The program uses UTC so the time is the same on every computer.

## Files
- `sensor.h` and `sensor.c`: Read the temperature and humidity from the sensor and round them to one decimal place.
- `panel.h` and `panel.c`: Check when switches are pressed or released and control the LEDs.
- `scenario.h` and `scenario.c`: Read scenario files with up to 1000 lines of data.
- `main.c`: Runs the scenario one second at a time and prints the results.
- `tests/test_hw06.c`: Tests the different parts of the program.
- `scenarios/` and `expected/`: Hold example scenarios and their correct results.
- `regress.sh`: Compares the program's results with the expected results.
- `iom361_r4.h` and `iom361_r4.c`: Files used to simulate the hardware. Both need to be replaced with the original files from the project folder before turning in the homework.

## Scenario Format
The first line starts with `start` followed by a Unix timestamp. The other lines contain the tick, temperature, humidity, and switch values.

For example:

```
start 1791273600
0 21.5 40.0 0x0
2 21.5 40.0 0x1
3 22.0 42.0 0x0
```

The switch values use hexadecimal numbers. Each tick represents one second, and the ticks must go in increasing order. If a value does not change, the program keeps using the previous value. If the first tick is not zero, the program uses the starting sensor and switch values until the first record.

## How the Program Works
The sensor reads the temperature and humidity from the hardware registers. It only saves the new reading if both values are read successfully.

The panel checks if a switch was pressed or released. When a switch is pressed, the matching LED changes from on to off or off to on. The program also makes sure changing one LED does not accidentally change the others.

The scenario code reads the file and checks for errors. If there is a problem, it returns the line number where the error happened without changing the saved scenario data.

The LED functions return -1 if the LED number is not between 0 and 31. Other hardware errors are returned using the iom361 error codes.

## Testing
The tests check the sensor readings, switch presses and releases, LED changes, and scenario files. They also check that the program handles incorrect inputs.

The regression tests run two scenarios and compare the output with the expected results. If the results match, the tests pass.

## GitHub
The hw06 folder goes inside my ECE-361 GitHub repository. Before submitting, I need to replace the two iom361 files with the original ones from the project folder.

After that, I can run `make test` again to make sure everything still works. The project folder itself does not need to be changed.