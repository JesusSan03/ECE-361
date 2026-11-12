# AI Usage - ECE 361 Project

Using Gemini I asked for the following help

**1. Fixing compiler warnings**
I asked why I was getting compiler warnings when using gcc with `-Wall` and `-Wextra`. It explained that some warnings can happen because of unused variables, incorrect types, or missing function declarations. I used the explanations to help fix my code.

**2. Fixing the START button**
I asked why holding the START switch could cause the program to start a cycle more than once. It explained the difference between checking if a switch is on and checking when it changes from off to on. This helped me understand rising-edge detection.

**3. Understanding the circular buffer**
I asked how the circular buffer stores only the 16 newest readings. I was confused about how the head index moves when the buffer becomes full. It explained how the index wraps around and how a new reading replaces the oldest one.

**4. Fixing timing problems**
I asked why my irrigation controller was sometimes switching states one tick too early or too late. It explained how using the wrong starting timestamp or updating the timer in the wrong order could cause this. It also helped me understand how to check whether a zone had been active for exactly 120 ticks.

**5. Debugging scenario replay**
I asked why my scenario tests were not always matching the expected output. It explained that the scenario file only changes the sensor and switch values at certain ticks, and the previous values should stay the same between those ticks. This helped me understand how the replay system should work.

**6. Fixing a suggestion that was wrong**
I asked for help with the rain interruption logic. One possible mistake was that its suggested logic continued the watering timer from where it stopped after the rain cleared. However, the assignment says the interrupted zone must start over. I pointed this out and changed the logic so the zone timer resets when watering starts again.
