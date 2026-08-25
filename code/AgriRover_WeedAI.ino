/*
=========================================================
 AEROROVER - GROUND ROVER
 File: AeroRover_Rover.ino

 Features:
 - Temperature
 - Humidity
 - Pressure
 - Moisture
 - Heat Detection
 - Gas Reading
 - GPS Live Position
 - MPU6050 Tilt
 - Distance Detection
 - Battery Monitoring
 - Rover Motor Control
 - Web Dashboard
 - AI Detection Placeholder
=========================================================
*/

#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_VL53L0X.h>
#include <TinyGPSPlus.h>

// =====================================================
// WIFI
// =====================================================

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

WebServer server(80);

// =====================================================
// I2C PINS
// Change if required
// =====================================================

#define I2C_SDA 1
#define I2C_SCL 2

// =====================================================
// SENSOR PINS
// =====================================================

#define MOISTURE_PIN 4
#define GAS_PIN 5
#define BATTERY_PIN 6

// =====================================================
// MOTOR DRIVER PINS
// =====================================================

#define LEFT_IN1 7
#define LEFT_IN2 8

#define RIGHT_IN1 9
#define RIGHT_IN2 10

// =====================================================
// GPS
// =====================================================

#define GPS_RX 17
#define GPS_TX 18

HardwareSerial GPSSerial(1);
TinyGPSPlus gps;

// =====================================================
// SENSOR OBJECTS
// =====================================================

Adafruit_BME280 bme;
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
Adafruit_MPU6050 mpu;
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

// =====================================================
// DEVICE INFORMATION
// =====================================================

String deviceID = "ROVER_01";
String movement = "STOP";

// =====================================================
// AI VARIABLES
// =====================================================

String aiObject = "Scanning";
String aiThreat = "NORMAL";
int aiConfidence = 0;

// =====================================================
// SENSOR VALUES
// =====================================================

float temperature = 0;
float humidity = 0;
float pressure = 0;
float heat = 0;

int moisture = 0;
int gasValue = 0;

float distanceCM = 0;

float latitude = 0;
float longitude = 0;

float tiltX = 0;
float tiltY = 0;
float accelerationX = 0;
float accelerationY = 0;
float accelerationZ = 0;

float batteryVoltage = 0;
int batteryPercent = 0;

// =====================================================
// HTML PAGE
// =====================================================

const char ROVER_PAGE[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html>

<head>

<meta name="viewport"
content="width=device-width, initial-scale=1">

<title>AeroRover Ground Rover</title>

<style>

body{
margin:0;
padding:20px;
font-family:Arial,sans-serif;
background:#111827;
color:white;
}

h1{
color:#38bdf8;
font-size:32px;
}

h2{
color:#e5e7eb;
}

.grid{
display:grid;
grid-template-columns:
repeat(auto-fit,minmax(210px,1fr));
gap:15px;
}

.card{
background:#1f2937;
border:1px solid #374151;
border-radius:12px;
padding:16px;
}

.label{
color:#9ca3af;
font-size:15px;
}

.value{
font-size:25px;
font-weight:bold;
color:#22c55e;
margin-top:8px;
}

.ai{
color:#facc15;
}

button{
padding:14px 20px;
margin:5px;
font-size:16px;
border:none;
border-radius:8px;
background:#0284c7;
color:white;
cursor:pointer;
}

.stop{
background:#dc2626;
}

</style>

</head>

<body>

<h1>🚙 AeroRover Ground Rover</h1>

<p>
Live Environmental Monitoring | GPS | AI | Emergency Inspection
</p>

<div class="grid">

<div class="card">
<div class="label">Temperature</div>
<div class="value" id="temp">-- °C</div>
</div>

<div class="card">
<div class="label">Humidity</div>
<div class="value" id="humidity">-- %</div>
</div>

<div class="card">
<div class="label">Pressure</div>
<div class="value" id="pressure">-- hPa</div>
</div>

<div class="card">
<div class="label">Moisture</div>
<div class="value" id="moisture">-- %</div>
</div>

<div class="card">
<div class="label">Heat / Object Temperature</div>
<div class="value" id="heat">-- °C</div>
</div>

<div class="card">
<div class="label">Gas Level</div>
<div class="value" id="gas">--</div>
</div>

<div class="card">
<div class="label">Obstacle Distance</div>
<div class="value" id="distance">-- cm</div>
</div>

<div class="card">
<div class="label">Battery</div>
<div class="value" id="battery">-- %</div>
</div>

<div class="card">
<div class="label">GPS Position</div>
<div class="value" id="gps">Waiting...</div>
</div>

<div class="card">
<div class="label">Tilt X / Y</div>
<div class="value" id="tilt">-- °</div>
</div>

<div class="card">
<div class="label">Rover Status</div>
<div class="value" id="movement">STOP</div>
</div>

<div class="card">
<div class="label">AI Detection</div>
<div class="value ai" id="ai">Scanning</div>
</div>

</div>

<br>

<h2>Rover Control</h2>

<center>

<button onclick="control('forward')">
⬆ Forward
</button>

<br>

<button onclick="control('left')">
⬅ Left
</button>

<button class="stop"
onclick="control('stop')">
■ STOP
</button>

<button onclick="control('right')">
Right ➡
</button>

<br>

<button onclick="control('backward')">
⬇ Reverse
</button>

</center>

<script>

function control(command){

fetch("/control?cmd="+command);

}

function updateData(){

fetch("/telemetry")

.then(response => response.json())

.then(data => {

document.getElementById("temp").innerHTML =
data.temperature + " °C";

document.getElementById("humidity").innerHTML =
data.humidity + " %";

document.getElementById("pressure").innerHTML =
data.pressure + " hPa";

document.getElementById("moisture").innerHTML =
data.moisture + " %";

document.getElementById("heat").innerHTML =
data.heat + " °C";

document.getElementById("gas").innerHTML =
data.gas;

document.getElementById("distance").innerHTML =
data.distance + " cm";

document.getElementById("battery").innerHTML =
data.battery + " %";

document.getElementById("gps").innerHTML =
data.latitude.toFixed(6) +
"<br>" +
data.longitude.toFixed(6);

document.getElementById("tilt").innerHTML =
data.tiltX + "° / " +
data.tiltY + "°";

document.getElementById("movement").innerHTML =
data.movement;

document.getElementById("ai").innerHTML =
data.aiObject +
"<br>" +
data.aiConfidence + "%";

});

}

setInterval(updateData,1000);

updateData();

</script>

</body>
</html>

)rawliteral";

// =====================================================
// MOTOR FUNCTIONS
// =====================================================

void stopRover(){

digitalWrite(LEFT_IN1, LOW);
digitalWrite(LEFT_IN2, LOW);

digitalWrite(RIGHT_IN1, LOW);
digitalWrite(RIGHT_IN2, LOW);

movement = "STOP";

}

void moveForward(){

digitalWrite(LEFT_IN1, HIGH);
digitalWrite(LEFT_IN2, LOW);

digitalWrite(RIGHT_IN1, HIGH);
digitalWrite(RIGHT_IN2, LOW);

movement = "FORWARD";

}

void moveBackward(){

digitalWrite(LEFT_IN1, LOW);
digitalWrite(LEFT_IN2, HIGH);

digitalWrite(RIGHT_IN1, LOW);
digitalWrite(RIGHT_IN2, HIGH);

movement = "REVERSE";

}

void moveLeft(){

digitalWrite(LEFT_IN1, LOW);
digitalWrite(LEFT_IN2, HIGH);

digitalWrite(RIGHT_IN1, HIGH);
digitalWrite(RIGHT_IN2, LOW);

movement = "LEFT";

}

void moveRight(){

digitalWrite(LEFT_IN1, HIGH);
digitalWrite(LEFT_IN2, LOW);

digitalWrite(RIGHT_IN1, LOW);
digitalWrite(RIGHT_IN2, HIGH);

movement = "RIGHT";

}

// =====================================================
// READ GPS
// =====================================================

void updateGPS(){

while(GPSSerial.available()){

gps.encode(GPSSerial.read());

}

if(gps.location.isValid()){

latitude = gps.location.lat();
longitude = gps.location.lng();

}

}

// =====================================================
// READ SENSORS
// =====================================================

void updateSensors(){

temperature =
bme.readTemperature();

humidity =
bme.readHumidity();

pressure =
bme.readPressure() / 100.0F;

heat =
mlx.readObjectTempC();

int moistureRaw =
analogRead(MOISTURE_PIN);

moisture =
map(
moistureRaw,
0,
4095,
100,
0
);

moisture =
constrain(
moisture,
0,
100
);

gasValue =
analogRead(GAS_PIN);

// -----------------------------
// MPU6050
// -----------------------------

sensors_event_t a;
sensors_event_t g;
sensors_event_t tempEvent;

mpu.getEvent(
&a,
&g,
&tempEvent
);

accelerationX = a.acceleration.x;
accelerationY = a.acceleration.y;
accelerationZ = a.acceleration.z;

tiltX =
atan2(
accelerationY,
accelerationZ
) * 180 / PI;

tiltY =
atan2(
-accelerationX,
sqrt(
accelerationY * accelerationY +
accelerationZ * accelerationZ
)
) * 180 / PI;

// -----------------------------
// VL53L0X
// -----------------------------

VL53L0X_RangingMeasurementData_t measure;

lox.rangingTest(
&measure,
false
);

if(measure.RangeStatus != 4){

distanceCM =
measure.RangeMilliMeter / 10.0;

}

// -----------------------------
// BATTERY
// -----------------------------

int batteryRaw =
analogRead(BATTERY_PIN);

// IMPORTANT:
// Calibrate according to your voltage divider.

batteryVoltage =
(
batteryRaw / 4095.0
) * 3.3 * 2.0;

batteryPercent =
map(
(int)(batteryVoltage * 100),
330,
420,
0,
100
);

batteryPercent =
constrain(
batteryPercent,
0,
100
);

}

// =====================================================
// AI PLACEHOLDER
// =====================================================

void runRoverAI(){

/*

CAMERA FRAME
      ↓
EDGE IMPULSE / TFLITE MODEL
      ↓
Object Detection

Possible classes:

Person
Victim
Fire
Smoke
Obstacle
Debris
Damaged Structure

Replace this function with
your trained AI model.

*/

if(heat > 60){

aiObject = "Possible Heat Source";
aiConfidence = 85;
aiThreat = "HIGH";

}else if(distanceCM < 30){

aiObject = "Obstacle";
aiConfidence = 90;
aiThreat = "MEDIUM";

}else{

aiObject = "Environment Clear";
aiConfidence = 95;
aiThreat = "NORMAL";

}

}

// =====================================================
// WEB HANDLERS
// =====================================================

void handleRoot(){

server.send(
200,
"text/html",
ROVER_PAGE
);

}

void handleControl(){

String cmd =
server.arg("cmd");

if(cmd == "forward"){

moveForward();

}else if(cmd == "backward"){

moveBackward();

}else if(cmd == "left"){

moveLeft();

}else if(cmd == "right"){

moveRight();

}else{

stopRover();

}

server.send(
200,
"text/plain",
"Command: " + movement
);

}

void handleTelemetry(){

updateGPS();
updateSensors();
runRoverAI();

String json = "{";

json += "\"device\":\"ROVER_01\",";
json += "\"temperature\":" + String(temperature,1) + ",";
json += "\"humidity\":" + String(humidity,1) + ",";
json += "\"pressure\":" + String(pressure,1) + ",";
json += "\"moisture\":" + String(moisture) + ",";
json += "\"heat\":" + String(heat,1) + ",";
json += "\"gas\":" + String(gasValue) + ",";
json += "\"distance\":" + String(distanceCM,1) + ",";
json += "\"battery\":" + String(batteryPercent) + ",";
json += "\"latitude\":" + String(latitude,6) + ",";
json += "\"longitude\":" + String(longitude,6) + ",";
json += "\"tiltX\":" + String(tiltX,1) + ",";
json += "\"tiltY\":" + String(tiltY,1) + ",";
json += "\"movement\":\"" + movement + "\",";
json += "\"aiObject\":\"" + aiObject + "\",";
json += "\"aiConfidence\":" + String(aiConfidence);

json += "}";

server.send(
200,
"application/json",
json
);

}

// =====================================================
// SETUP
// =====================================================

void setup(){

Serial.begin(115200);

Wire.begin(
I2C_SDA,
I2C_SCL
);

// Motor Pins

pinMode(LEFT_IN1, OUTPUT);
pinMode(LEFT_IN2, OUTPUT);

pinMode(RIGHT_IN1, OUTPUT);
pinMode(RIGHT_IN2, OUTPUT);

stopRover();

// Analog Pins

pinMode(MOISTURE_PIN, INPUT);
pinMode(GAS_PIN, INPUT);
pinMode(BATTERY_PIN, INPUT);

// GPS

GPSSerial.begin(
9600,
SERIAL_8N1,
GPS_RX,
GPS_TX
);

// BME280

if(!bme.begin(0x76)){

Serial.println(
"BME280 not found!"
);

}

// MLX90614

if(!mlx.begin()){

Serial.println(
"MLX90614 not found!"
);

}

// MPU6050

if(!mpu.begin()){

Serial.println(
"MPU6050 not found!"
);

}

// VL53L0X

if(!lox.begin()){

Serial.println(
"VL53L0X not found!"
);

}

// WiFi

WiFi.begin(
ssid,
password
);

Serial.print(
"Connecting to WiFi"
);

while(
WiFi.status() != WL_CONNECTED
){

delay(500);

Serial.print(".");

}

Serial.println();

Serial.print(
"Rover IP: "
);

Serial.println(
WiFi.localIP()
);

// Web Server

server.on(
"/",
handleRoot
);

server.on(
"/telemetry",
handleTelemetry
);

server.on(
"/control",
handleControl
);

server.begin();

Serial.println(
"Rover Server Started"
);

}

// =====================================================
// LOOP
// =====================================================

void loop(){

server.handleClient();

updateGPS();

}