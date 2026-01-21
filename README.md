# smart-iot-kettle-for-beginers
this is a user friendly easy to understand low cost easy to  make project this could be used as a school project or in general for geeks to fiddel around  this project --has 2 sensors and 2 output modules and uses blynk iot app as a major control system 

This project converts a regular electric kettle into a smart, IoT-enabled appliance using an ESP32 microcontroller. The system adds remote monitoring, automated control, safety features, and local display while retaining the original kettle hardware.

PROJECT OVERVIEW:-

The ESP32 acts as the central controller for the system. It reads sensor data, makes control decisions, drives output devices, and communicates with the Blynk IoT cloud platform. The kettle can be controlled manually or automatically based on time or temperature, with an added rain/moisture safety override

KEY FEATURES:-
Remote control via Blynk IoT mobile app
Temperature-based automatic shutoff
Timer-based operation
Rain/moisture safety override
Real-time OLED display
Priority-based control logic for safe operation

-------------------------------------------------------------------REQUIRED COMPONENTS---------------------------------------------------------
Hardware Components
ESP32 development board
Electric kettle (standard AC kettle)
Single-channel relay module
DHT11 temperature sensor
Rain/moisture sensor (digital output)
OLED display (128×64, I2C, SSD1306)
Jumper wires
Breadboard or PCB
External 5V supply for relay (recommended)

Software Requirements
Arduino IDE
ESP32 board package installed in Arduino IDE
Blynk IoT accou

REQUIRED LIBRARIES:-
Blynk
DHTesp
Adafruit SSD1306
Adafruit GFX

--------------------------------------------------------------------HOW THE SYSTEM WORKStest----------------------------------------------------

The ESP32 connects to Wi-Fi and the Blynk IoT cloud.
The DHT11 sensor measures temperature at regular intervals.
The rain sensor continuously monitors for moisture or water presence.
Based on sensor inputs and user commands, the ESP32 decides whether the relay should be ON or OFF.
The relay switches the kettle’s power safely.
The OLED display shows temperature, relay status, timer countdown, and threshold information.
Sensor data and controls are synchronized with the Blynk IoT app.
Control Logic Priority
The system uses a strict priority order to ensure safety and predictable behavior:
Rain sensor override (highest priority)
Temperature threshold control
Timer-based control
Manual ON/OFF control
If rain is detected, the relay is immediately turned OFF regardless of other settings. Once the sensor is dry, control is returned to the previously active mode.

------------------------------------------------------------------WIRING CONNECTIONS------------------------------------------------------------

ESP32 Pin Connections

 RELAY MODULE:-
IN → GPIO 19,
VCC → External 5V,
GND → Common ground,

DHT11 SENSOR:-
DATA → GPIO 18,
VCC → 3.3V,
GND → GND,

RAIN SENSOR (digital mode):-
DO → GPIO 5,
VCC → 3.3V,
GND → GND,

OLED DISPLAY (I2C):-
SDA → GPIO 21,
SCL/SCK → GPIO 22,
VCC → 3.3V,
GND → GND,

RELAY LOAD WIRING:-
For the kettle:

Live wire → COM terminal of relay
NO terminal → kettle live input
Neutral wire → directly to kettle
This ensures the kettle turns ON only when the relay is activated.
Blynk IoT Setup
Create a new template in Blynk IoT.

------------------------------------------------------------BLYNK TEMPLATE SETUP-----------------------------------------------------------------

ADD WIDGETS:-
Button (V0) for manual ON/OFF
Slider (V2) for timer control (minutes)
Slider (V3) for temperature threshold
Gauge or display widget (V1) for temperature
Copy the Template ID, Template Name, and Auth Token into the code.
Upload the code to the ESP32.
OLED Display Information
The OLED shows:
Current temperature
Relay ON/OFF status
Remaining timer countdown
Temperature threshold status and notifications
All information is displayed on a single screen for clarity.

----------------------------------------------------------------USER SAFTEY NOTE---------------------------------------------------------------------

Always use a relay module rated for the kettle’s voltage and current.
Do not power high-voltage loads directly from the ESP32.
Keep AC wiring isolated from low-voltage electronics.
Test with low-power loads before connecting the kettle.

CONCLUSION:-
This project demonstrates how traditional appliances can be enhanced using embedded systems and IoT technologies. The design is modular, safe, and extensible, making it suitable for further upgrades such as additional sensors, mobile notifications, or data logging.
