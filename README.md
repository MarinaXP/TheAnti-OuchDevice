# The Anti-Ouch Device - Haptic Echolocation System

Ever wish you had a sixth sense? This system gives you a new way to sense the world around you, acting as a navigation aid that uses vibrations to help you "feel" objects. It's all inspired by the way bats use echolocation.

![Teasing screen](https://github.com/MarinaXP/https://github.com/MarinaXP/The_Anti-Ouch_Device/blob/20e5a0d961f0e79bd2adb1fd8e0170e3fe029860/presentation_screen.png?raw=true)

### How It Works

Super easy. This device uses a simple HC-SR04 ultrasonic sensor to measure the distance to obstacles. It then translates that distance into a pattern of vibrations using a vibrator motor. The closer an object gets, the faster and more intense the vibrations become. It's a remarkably intuitive system—you feel the danger before you see it.

### Hardware Requirements

You'll need a few common components to build this project:

* **Arduino** (Uno, Nano, or Pro Mini)
* **HC-SR04 Ultrasonic Sensor**
* **Vibrator Motor Module**
* **Breadboard** and **jumper wires**

### Pin Configuration

Putting everything together is straightforward. Follow this wiring guide to connect the components to your Arduino:

**HC-SR04:**
VCC with 5V
GND with GND
Trig with Pin 2
Echo with Pin 3

**Vibrator Motor Module:**
VCC with 5V
GND withGND
Signal with Pin 4


### Operation

The system operates on a 30ms measurement cycle. Depending on the distance, you'll get a different vibration pattern:

* **Close Objects (~5cm):** Rapid vibrations (a pulse every 20ms for 80ms)
* **Medium Objects (~60cm):** Noticeable vibrations (a pulse every 500ms for 30ms)
* **Far Objects (~150cm):** Slow vibrations (a pulse every 1000ms for 15ms)

The system clamps distances closer than 5cm or farther than 150cm to these limits, so you'll always get a clear feedback pattern.

---

### Installation

1.  Assemble the hardware according to the pin configuration above.
2.  Upload the Arduino sketch to your board.
3.  Open the Serial Monitor (at 9600 baud) to view real-time distance data.
4.  The system will confirm it's ready with three short vibration pulses.

---

### Configuration

You can easily modify key parameters directly in the code to suit your needs:

```cpp
const int MIN_DISTANCE = 5;     // Minimum sensing distance (cm)
const int MAX_DISTANCE = 150;   // Maximum sensing distance (cm)
const int MEASUREMENT_INTERVAL = 30; // How often the system takes a measurement (ms)