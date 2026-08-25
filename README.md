# AgriRover---Low-Cost-Modular-Multipurpose-Agricultural-Robot-for-Small-and-Marginal-Farmers
# 🌱 AgriRover

## Low-Cost Modular Multipurpose Agricultural Robot for Small and Marginal Farmers

AgriRover is a **low-cost modular agricultural robotics platform** designed to assist small and marginal farmers through intelligent field monitoring, AI-based weed detection, and precision weedicide spraying.

The system combines **XIAO ESP32-S3 Sense**, computer vision, IoT monitoring, environmental sensors, tank-level monitoring, and an **8-zone precision spraying system**.

Unlike conventional spraying systems that apply chemicals uniformly across an entire field, AgriRover identifies weed locations using an onboard AI camera and activates only the corresponding spray zone.

---

## 🚜 Project Overview

AgriRover is designed around two XIAO ESP32-S3 Sense controllers.

### Front XIAO ESP32-S3 Sense — Main Controller

The front controller manages:

* Front camera
* Temperature monitoring
* Humidity monitoring
* Atmospheric pressure
* Seed-level monitoring
* Weedicide-level monitoring
* Web server and dashboard
* Sprayer control
* Communication with the AI controller
* Safety and emergency-stop logic

### Rear XIAO ESP32-S3 Sense — AI Weed Detection

The rear controller is mounted on the agricultural attachment and manages:

* Rear-facing camera
* AI-based weed detection
* Weed localization
* Spray-zone identification
* Communication with the main controller

The rear camera's field of view is divided into **8 spray zones**. When a weed is detected, its position is converted into a corresponding spray zone.

---

# 🎯 Main Objective

The primary objective of AgriRover is to develop an affordable agricultural robot capable of:

1. Monitoring important environmental parameters.
2. Monitoring seed and weedicide levels.
3. Capturing live field images.
4. Detecting weeds using AI.
5. Determining the location of detected weeds.
6. Selecting the appropriate spray zone.
7. Activating only the required sprayer.
8. Providing real-time information through a web dashboard.
9. Reducing unnecessary chemical application.
10. Providing a modular platform for future autonomous agricultural operations.

---

# ✨ Features

* 🤖 AI-based weed detection
* 🌿 Precision weedicide spraying
* 🎯 8 independent spray zones
* 📷 Front camera monitoring
* 📷 Rear AI camera
* 🌡️ Temperature monitoring
* 💧 Humidity monitoring
* 🌬️ Atmospheric pressure monitoring
* 🌾 Seed-level monitoring
* 🧪 Weedicide-level monitoring
* 🌐 Wi-Fi web dashboard
* 📊 Real-time sensor data
* 🚨 Emergency spray shutdown
* 🔌 Modular hardware architecture
* 📡 Wireless communication between controllers
* ⚙️ Expandable agricultural platform
* 🚜 Designed for small and marginal farming applications

---

# 🧠 System Architecture

```text
                         AGRIROVER
                            │
             ┌──────────────┴──────────────┐
             │                             │
             ▼                             ▼
    ┌─────────────────┐           ┌─────────────────┐
    │ FRONT XIAO      │           │ REAR XIAO       │
    │ ESP32-S3 Sense  │           │ ESP32-S3 Sense  │
    │ MAIN CONTROLLER │           │ AI CONTROLLER   │
    └────────┬────────┘           └────────┬────────┘
             │                             │
       ┌─────┼──────────┐                  │
       │     │          │                  │
       ▼     ▼          ▼                  ▼
    BME280  LEVEL     CAMERA           AI CAMERA
            SENSORS                       │
       │                                  │
       ▼                                  ▼
 ENVIRONMENT                         WEED DETECTION
 MONITORING                               │
       │                                  ▼
       │                             ZONE DETECTION
       │                                  │
       └──────────────┐             ┌─────┘
                      ▼             ▼
                 WEB DASHBOARD
                      │
                      ▼
              SPRAYER CONTROLLER
                      │
          ┌───────────┼───────────┐
          ▼           ▼           ▼
         S1          S2          S3 ... S8
          │           │           │
          ▼           ▼           ▼
       NOZZLE 1    NOZZLE 2    NOZZLE 3 ... NOZZLE 8
```

---

# 🔄 Complete Working

The complete AgriRover operation follows this pipeline:

```text
                    START
                      │
                      ▼
             Initialize Hardware
                      │
                      ▼
                Connect Wi-Fi
                      │
                      ▼
              Initialize Sensors
                      │
                      ▼
               Initialize Camera
                      │
                      ▼
              Start Web Server
                      │
                      ▼
              Capture Field Image
                      │
                      ▼
                 AI Inference
                      │
                      ▼
               Weed Detected?
                 /          \
               NO            YES
               │              │
               │              ▼
               │        Get Bounding Box
               │              │
               │              ▼
               │        Calculate X Position
               │              │
               │              ▼
               │        Determine Spray Zone
               │              │
               │              ▼
               │        Send Zone to Main XIAO
               │              │
               │              ▼
               │       Check Safety Conditions
               │              │
               │        ┌─────┴─────┐
               │        │           │
               │      SAFE        UNSAFE
               │        │           │
               │        ▼           ▼
               │     Spray       Do Not Spray
               │      Zone
               │        │
               │        ▼
               │  Calibrated Spray Pulse
               │        │
               └────────┴─────────────┐
                                      ▼
                              Continue Monitoring
```

---

# 🔩 Hardware Components

## Main Controller

| Component              | Quantity | Function                             |
| ---------------------- | -------: | ------------------------------------ |
| XIAO ESP32-S3 Sense    |        1 | Main controller + front camera       |
| BME280                 |        1 | Temperature, humidity and pressure   |
| Seed-level sensor      |        1 | Seed quantity monitoring             |
| Weedicide-level sensor |        1 | Weedicide quantity monitoring        |
| MCP23017               |        1 | Additional GPIO for 8 spray channels |
| Wi-Fi                  | Built-in | Web dashboard and communication      |

## AI Controller

| Component           | Quantity | Function                        |
| ------------------- | -------: | ------------------------------- |
| XIAO ESP32-S3 Sense |        1 | AI controller                   |
| Camera              | Built-in | Weed detection                  |
| AI model            |        1 | Weed detection and localization |

## Spraying System

| Component                 |    Quantity | Function                    |
| ------------------------- | ----------: | --------------------------- |
| Weedicide tank            |           1 | Chemical storage            |
| Pump                      |           1 | Pressurizes spraying system |
| Solenoid valves / drivers |           8 | Individual spray control    |
| Spray nozzles             |           8 | Targeted spraying           |
| MOSFET/relay driver       |  8 channels | Drives valves               |
| Tubing                    | As required | Chemical delivery           |
| Flow sensor               |    Optional | Spray-flow monitoring       |
| Emergency stop            |           1 | Hardware safety             |

## Rover Platform

| Component        | Function                       |
| ---------------- | ------------------------------ |
| DC geared motors | Rover movement                 |
| Motor driver     | Motor control                  |
| Battery          | Power supply                   |
| Wheels           | Mobility                       |
| Chassis          | Mechanical platform            |
| Wheel encoders   | Speed and distance measurement |

---

# 🔌 Electrical Connections

## XIAO ESP32-S3 Sense + BME280

```text
XIAO ESP32-S3        BME280
──────────────────────────────
3.3V          ────── VCC
GND           ────── GND
D4 / GPIO5    ────── SDA
D5 / GPIO6    ────── SCL
```

The BME280 communicates with the XIAO through I²C.

---

# 🌾 Seed-Level Sensor

Example analog configuration:

```text
XIAO ESP32-S3        Seed Level Sensor
──────────────────────────────────────
3.3V          ────── VCC
GND           ────── GND
Analog GPIO   ────── OUT
```

The raw sensor reading is calibrated between empty and full:

```text
Empty → 0%
Half  → 50%
Full  → 100%
```

The actual calibration values depend on the selected level sensor and hopper geometry.

---

# 🧪 Weedicide-Level Sensor

```text
XIAO ESP32-S3        Weedicide Sensor
─────────────────────────────────────
3.3V          ────── VCC
GND           ────── GND
Analog GPIO   ────── OUT
```

The web dashboard displays the estimated remaining weedicide:

```text
WEEDICIDE

██████████████░░░░░░

72%
```

For the final prototype, use a level sensor that is chemically compatible with the selected agricultural chemical.

---

# 🚿 Eight-Zone Sprayer

The XIAO GPIO pins should not directly drive pumps or high-current solenoid valves.

The recommended architecture is:

```text
                    XIAO
                      │
                     I²C
                      │
                      ▼
                 MCP23017
                      │
       ┌──────────────┼──────────────┐
       │              │              │
       ▼              ▼              ▼
      CH1            CH2            CH3
       │              │              │
       ▼              ▼              ▼
    MOSFET          MOSFET         MOSFET
       │              │              │
       ▼              ▼              ▼
   NOZZLE 1       NOZZLE 2       NOZZLE 3

       ...

      CH8
       │
       ▼
    MOSFET
       │
       ▼
   NOZZLE 8
```

Each channel corresponds to one spray zone.

```text
S1 → Zone 1
S2 → Zone 2
S3 → Zone 3
S4 → Zone 4
S5 → Zone 5
S6 → Zone 6
S7 → Zone 7
S8 → Zone 8
```

Use properly rated MOSFET drivers, protection components, wiring, fusing and a separate power path for pumps/valves.

---

# 📷 Camera System

AgriRover uses two camera systems.

## Front Camera

The front XIAO camera is used for:

* Field monitoring
* Rover-view monitoring
* Live camera display
* Future obstacle detection
* Future navigation features

## Rear AI Camera

The rear camera is used for:

* Weed detection
* Weed localization
* Spray-zone selection
* AI inference

The XIAO ESP32-S3 Sense integrates an onboard camera and PSRAM, making it suitable for embedded computer-vision applications.

Official documentation:

https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/

---

# 🧠 AI Weed Detection

The rear XIAO runs a lightweight object-detection model.

A suitable example model for development is:

```text
YOLOv8n
```

Another practical deployment option is an Edge Impulse object-detection model optimized for the XIAO ESP32-S3 Sense.

## Example Classes

A simple model:

```text
Class 0 → Weed
```

A more advanced model:

```text
Class 0 → Crop
Class 1 → Weed
Class 2 → Soil
```

The model should output a bounding box and confidence score.

Example:

```text
Object      : Weed
Confidence  : 93.4%
X           : 426
Y           : 238
Width       : 67
Height      : 84
```

The X-coordinate is then used to select the appropriate spray zone.

---

# 🎯 Eight-Zone Weed Detection

The rear camera view is divided into eight vertical zones.

```text
640 PIXEL WIDE CAMERA IMAGE

┌────┬────┬────┬────┬────┬────┬────┬────┐
│ Z1 │ Z2 │ Z3 │ Z4 │ Z5 │ Z6 │ Z7 │ Z8 │
└────┴────┴────┴────┴────┴────┴────┴────┘

80 pixels per zone
```

For a 640-pixel-wide image:

```text
Zone Width = 640 / 8

Zone Width = 80 pixels
```

Example:

```text
Weed X coordinate = 430

430 / 80 = 5.37

Spray Zone = 6
```

Therefore:

```text
AI Camera
    │
    ▼
Weed detected
    │
    ▼
X = 430
    │
    ▼
Zone 6
    │
    ▼
Main XIAO
    │
    ▼
Sprayer 6
```

---

# 📡 Communication Between XIAO Controllers

The two XIAO ESP32-S3 boards communicate over Wi-Fi.

The rear AI controller detects the weed and sends its zone to the front controller.

Example request:

```http
GET /weed?zone=6
```

The main controller receives:

```text
zone = 6
```

and checks:

```text
Spraying enabled?
        ↓
Emergency stop inactive?
        ↓
Weedicide level sufficient?
        ↓
AI confidence sufficient?
        ↓
Valid zone?
        ↓
YES
        ↓
Activate Sprayer 6
```

---

# ⏱️ Camera-to-Nozzle Timing

The camera and spray bar are physically separated.

For example:

```text
Rear Camera
     │
     │
     │ 60 cm
     │
     ▼
Spray Bar
```

If the rover moves at:

```text
0.5 m/s
```

then:

```text
Time = Distance / Speed

Time = 0.60 / 0.50

Time = 1.2 seconds
```

The system can therefore perform:

```text
Detect Weed
     ↓
Remember Zone
     ↓
Estimate Travel Time
     ↓
Wait for Weed to Reach Spray Bar
     ↓
Activate Correct Nozzle
     ↓
Apply Calibrated Spray Pulse
     ↓
Turn Nozzle OFF
```

A wheel encoder is recommended for accurate implementation of this timing system.

---

# 🌐 Web Dashboard

The front XIAO hosts a local web server.

After connecting to Wi-Fi, the Serial Monitor displays:

```text
AgriRover IP: 192.168.1.100
```

Open:

```text
http://192.168.1.100
```

from a device connected to the same network.

The dashboard provides:

```text
┌────────────────────────────────────────────┐
│              🌱 AGRIROVER                 │
├────────────────────────────────────────────┤
│                                            │
│ Temperature       28.4 °C                  │
│ Humidity          71 %                     │
│ Pressure          1008 hPa                 │
│                                            │
│ Seeds             76 %                     │
│ Weedicide         63 %                     │
│                                            │
│ Detected Weed     Zone 6                   │
│                                            │
│ Sprayer 1         OFF                      │
│ Sprayer 2         OFF                      │
│ Sprayer 3         OFF                      │
│ Sprayer 4         OFF                      │
│ Sprayer 5         OFF                      │
│ Sprayer 6         ON                       │
│ Sprayer 7         OFF                      │
│ Sprayer 8         OFF                      │
│                                            │
│ [ ENABLE ] [ DISABLE ] [ EMERGENCY STOP ] │
│                                            │
└────────────────────────────────────────────┘
```

---

# 📊 Dashboard Data API

The main controller provides a JSON endpoint:

```text
/data
```

Example response:

```json
{
  "temperature": 28.4,
  "humidity": 71.2,
  "pressure": 1008.4,
  "seedLevel": 76,
  "weedicideLevel": 63,
  "detectedZone": 6,
  "sprayingEnabled": true,
  "emergencyStop": false
}
```

The weed detection controller can send:

```text
/weed?zone=6
```

Manual sprayer testing can use:

```text
/sprayer?zone=1
/sprayer?zone=2
/sprayer?zone=3
/sprayer?zone=4
/sprayer?zone=5
/sprayer?zone=6
/sprayer?zone=7
/sprayer?zone=8
```

Emergency stop:

```text
/stop
```

Enable spraying:

```text
/enable
```

Disable spraying:

```text
/disable
```

---

# 📁 Repository Structure

```text
AgriRover/
│
├── README.md
│
├── firmware/
│   │
│   ├── main-controller/
│   │   └── AgriRover_Main.ino
│   │
│   └── weed-detection/
│       └── AgriRover_WeedAI.ino
│
├── ai/
│   ├── dataset/
│   ├── training/
│   ├── model/
│   └── README.md
│
├── web/
│   ├── dashboard.html
│   ├── style.css
│   └── script.js
│
├── hardware/
│   ├── wiring/
│   ├── schematics/
│   └── pcb/
│
├── mechanical/
│   ├── chassis/
│   ├── spray-attachment/
│   └── cad/
│
├── images/
│   ├── rover.jpg
│   ├── front-module.jpg
│   ├── rear-module.jpg
│   ├── wiring.jpg
│   └── dashboard.jpg
│
├── docs/
│   ├── architecture.md
│   ├── wiring.md
│   └── ai-model.md
│
└── LICENSE
```

---

# 💻 Software Requirements

## Arduino IDE

Install Arduino IDE and the ESP32 board package.

Select the appropriate XIAO ESP32-S3 board configuration.

Recommended libraries:

```text
WiFi
WebServer
Wire
Adafruit BME280 Library
Adafruit Unified Sensor
Adafruit MCP23017
esp_camera
```

For AI deployment, use the framework associated with the selected model, such as:

```text
Edge Impulse SDK
```

or an optimized TensorFlow Lite Micro deployment.

---

# 🚀 Installation

## 1. Clone the Repository

```bash
git clone https://github.com/YOUR_USERNAME/AgriRover.git

cd AgriRover
```

## 2. Configure the Main Controller

Open:

```text
firmware/main-controller/AgriRover_Main.ino
```

Set your Wi-Fi credentials:

```cpp
const char* WIFI_SSID = "YOUR_WIFI";
const char* WIFI_PASSWORD = "YOUR_PASSWORD";
```

## 3. Configure the AI Controller

Open:

```text
firmware/weed-detection/AgriRover_WeedAI.ino
```

Set the IP address of the main XIAO:

```cpp
const char* MAIN_ROVER_IP = "192.168.1.100";
```

Replace the example IP with the IP printed by the main XIAO.

## 4. Upload Firmware

Upload:

```text
Main Controller
        ↓
Front XIAO ESP32-S3 Sense
```

and:

```text
Weed Detection
        ↓
Rear XIAO ESP32-S3 Sense
```

## 5. Open Serial Monitor

Set:

```text
115200 baud
```

Expected output:

```text
==========================
AgriRover Main Controller
==========================

Connecting to WiFi....
AgriRover IP: 192.168.1.100
Web server started
```

## 6. Open Dashboard

Open the displayed IP address in a browser:

```text
http://192.168.1.100
```

---

# 🧠 AI Model Development

The recommended AI development pipeline is:

```text
Collect Field Images
        ↓
Label Weeds
        ↓
Create Dataset
        ↓
Train Object Detection Model
        ↓
Validate Model
        ↓
Optimize Model
        ↓
Deploy to Rear XIAO
        ↓
Test in Field
        ↓
Calibrate Spray Zones
```

For a prototype, Edge Impulse can be used to simplify dataset management, model training and embedded deployment.

https://www.edgeimpulse.com/

---

# 📸 Dataset Collection

The AI model should be trained using images captured from the actual camera mounted on AgriRover whenever possible.

The dataset should contain different:

* Lighting conditions
* Sunlight levels
* Soil colors
* Crop growth stages
* Weed sizes
* Weed densities
* Camera heights
* Camera angles
* Shadows
* Weather conditions
* Field conditions

Example dataset:

```text
ai/
│
├── dataset/
│   ├── train/
│   ├── validation/
│   └── test/
│
├── model/
│   └── weed_detection/
│
└── training/
    └── training_notes.md
```

---

# 📊 Example AI Output

```text
----------------------------------
AgriRover AI Detection
----------------------------------

Object       : Weed
Confidence   : 93.4%

Bounding Box
X            : 426
Y            : 238
Width        : 67
Height       : 84

Camera Width : 640

Spray Zone   : 6

Status       : TARGET DETECTED
----------------------------------
```

---

# 🎯 Precision Spraying Concept

Traditional agricultural spraying:

```text
Entire Field
────────────────────────────
████████████████████████████
████████████████████████████
████████████████████████████
████████████████████████████

Chemical applied broadly
```

AgriRover:

```text
Field
────────────────────────────
       🌿

                 🌿

────────────────────────────

       ↓
AI Weed Detection

       ↓

      Z3              Z7
       │               │
       ▼               ▼
      💧              💧

Targeted spray zones
```

The actual chemical reduction must be measured experimentally and should not be assumed without field testing.

---

# 🔬 Precision Spraying Algorithm

```text
1. Capture image
2. Run AI inference
3. Detect weed
4. Check confidence
5. Calculate weed center
6. Convert X-coordinate to spray zone
7. Determine rover speed
8. Calculate camera-to-nozzle delay
9. Verify weedicide level
10. Verify safety conditions
11. Activate corresponding nozzle
12. Apply calibrated spray pulse
13. Turn nozzle OFF
14. Record event
15. Continue scanning
```

---

# 🛡️ Safety System

AgriRover should use multiple levels of protection.

Software safety checks include:

```text
✓ Spraying enabled
✓ Emergency stop inactive
✓ Weedicide level sufficient
✓ Valid spray zone
✓ AI confidence above threshold
✓ Valid communication
✓ No detected hardware fault
```

Example:

```cpp
if (
    sprayingEnabled &&
    !emergencyStop &&
    weedicideLevel > MIN_WEEDICIDE_LEVEL &&
    confidence >= MIN_CONFIDENCE
) {
    sprayZone(zone);
}
```

A physical emergency-stop circuit should also be implemented independently of the software.

During initial testing, use water or another safe test fluid instead of agricultural chemicals.

---

# 🧪 Calibration

Before field operation, calibrate the following systems.

## Tank Level

```text
Empty → 0%
Half  → 50%
Full  → 100%
```

## AI Model

Tune:

```text
Confidence threshold
Detection size
Camera height
Camera angle
Lighting conditions
```

## Spraying System

Measure:

```text
Pump flow rate
Nozzle flow rate
Spray width
Valve response time
Spray pressure
Camera-to-nozzle distance
```

## Rover

Measure:

```text
Wheel circumference
Encoder pulses/revolution
Rover speed
Camera-to-nozzle distance
```

---

# 📈 Development Roadmap

## Phase 1 — Basic Rover

* [ ] Chassis
* [ ] Motors
* [ ] Motor driver
* [ ] Battery
* [ ] Manual movement

## Phase 2 — IoT Monitoring

* [ ] XIAO ESP32-S3
* [ ] BME280
* [ ] Seed-level sensor
* [ ] Weedicide-level sensor
* [ ] Web dashboard

## Phase 3 — Computer Vision

* [ ] Rear XIAO
* [ ] Rear camera
* [ ] Weed dataset
* [ ] AI model
* [ ] Weed detection

## Phase 4 — Precision Spraying

* [ ] MCP23017
* [ ] 8-channel driver
* [ ] 8 spray zones
* [ ] Pump
* [ ] Nozzles
* [ ] Spray calibration

## Phase 5 — Autonomous Agriculture

* [ ] Wheel encoders
* [ ] Autonomous navigation
* [ ] Row following
* [ ] GPS/RTK GPS
* [ ] Obstacle detection
* [ ] Field mapping
* [ ] Weed-density mapping

## Phase 6 — Advanced Agriculture

* [ ] Soil moisture monitoring
* [ ] Soil nutrient monitoring
* [ ] Crop-health detection
* [ ] Variable-rate spraying
* [ ] Cloud telemetry
* [ ] Mobile application
* [ ] Remote monitoring
* [ ] Autonomous return-to-start
* [ ] Solar charging

---

# 🔮 Future Scope

AgriRover can be extended into a complete autonomous agricultural platform.

Potential future capabilities include:

### Autonomous Navigation

```text
GPS / RTK GPS
      +
Wheel Encoders
      +
IMU
      ↓
Autonomous Field Navigation
```

### Row Following

```text
Camera
   ↓
Crop Row Detection
   ↓
Steering Correction
   ↓
Automatic Row Following
```

### Weed Mapping

```text
GPS Position
     +
AI Weed Detection
     ↓
Weed Location Database
     ↓
Field Weed Map
```

### Variable-Rate Spraying

```text
Weed Density
     ↓
AI Analysis
     ↓
Spray Requirement
     ↓
Variable Application Rate
```

---

# 📷 Recommended Project Documentation

The repository should contain photographs and diagrams showing:

1. Complete AgriRover
2. Front XIAO module
3. Rear AI XIAO module
4. BME280 sensor
5. Seed hopper
6. Weedicide tank
7. Eight-nozzle spray attachment
8. Electronics enclosure
9. Wiring
10. Web dashboard
11. AI weed detection
12. Spray-zone demonstration
13. Complete working prototype

Example AI visualization:

```text
┌─────────────────────────────────┐
│                                 │
│       ┌──────────────┐          │
│       │     WEED     │          │
│       │     93%      │          │
│       └──────────────┘          │
│                                 │
│              ZONE 5             │
│                                 │
└─────────────────────────────────┘
```

---

# 🏆 Project Impact

AgriRover aims to demonstrate how **embedded AI, IoT, computer vision and robotics** can be combined to build affordable agricultural technology.

The core concept is:

```text
                    SEE
                     │
                     ▼
              Capture Field
                     │
                     ▼
                  THINK
                     │
                     ▼
              Detect Weed
                     │
                     ▼
                 LOCATE
                     │
                     ▼
             Determine Zone
                     │
                     ▼
                   ACT
                     │
                     ▼
             Targeted Spraying
                     │
                     ▼
                MONITOR
                     │
                     ▼
             Update Dashboard
```

The goal is to move agricultural spraying from broad application toward **data-driven, targeted application**.

---

# 📌 Project Highlights

```text
Platform        : AgriRover
Controller      : XIAO ESP32-S3 Sense
AI Controller   : XIAO ESP32-S3 Sense
Environment     : BME280
AI              : Weed Object Detection
Vision          : Embedded Camera
Sprayers        : 8 Independent Zones
Communication   : Wi-Fi
Dashboard       : ESP32 Web Server
GPIO Expansion  : MCP23017
Development     : Arduino IDE
Language        : C/C++
Application     : Precision Agriculture
```

---

# 🧰 Technologies Used

* C/C++
* Arduino
* ESP32
* XIAO ESP32-S3 Sense
* Embedded Systems
* IoT
* Computer Vision
* Edge AI
* Machine Learning
* Robotics
* Web Technologies
* Precision Agriculture

---

# ⭐ GitHub Repository Tagline

> **AgriRover — An affordable AI-powered agricultural rover for real-time field monitoring and precision weedicide spraying.**

---

## 🌱 AgriRover

**See → Think → Locate → Spray → Monitor**

A modular approach toward affordable and intelligent precision agriculture.
