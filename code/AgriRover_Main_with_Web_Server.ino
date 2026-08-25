/************************************************************
 * AGRIROVER - REAR XIAO ESP32-S3 SENSE
 *
 * AI WEED DETECTION + 8 SELECTIVE SPRAYERS
 *
 * Flow:
 * Camera
 *    ↓
 * AI Detection
 *    ↓
 * Weed Bounding Box
 *    ↓
 * Weed Center X
 *    ↓
 * Spray Zone 1-8
 *    ↓
 * MCP23017
 *    ↓
 * 8 MOSFET / RELAY DRIVERS
 *    ↓
 * Targeted Weedicide Spraying
 ************************************************************/

#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_MCP23X17.h>
#include "esp_camera.h"

// ============================================================
// WIFI
// ============================================================

const char* ssid =
  "YOUR_WIFI_NAME";

const char* password =
  "YOUR_WIFI_PASSWORD";

// ============================================================
// FRONT XIAO IP
//
// CHANGE THIS TO THE IP SHOWN BY THE FRONT XIAO
// ============================================================

const char* FRONT_SERVER =
  "http://192.168.1.100/weed";

// ============================================================
// MCP23017
// ============================================================

Adafruit_MCP23X17 mcp;

// ============================================================
// CAMERA PINS
// XIAO ESP32-S3 Sense
// ============================================================

#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM     10
#define SIOD_GPIO_NUM     40
#define SIOC_GPIO_NUM     39

#define Y9_GPIO_NUM       48
#define Y8_GPIO_NUM       11
#define Y7_GPIO_NUM       12
#define Y6_GPIO_NUM       14
#define Y5_GPIO_NUM       16
#define Y4_GPIO_NUM       18
#define Y3_GPIO_NUM       17
#define Y2_GPIO_NUM       15

#define VSYNC_GPIO_NUM    38
#define HREF_GPIO_NUM     47
#define PCLK_GPIO_NUM     13

// ============================================================
// MCP23017 SPRAYER OUTPUTS
// ============================================================

const int sprayerPins[8] = {

  0,  // Sprayer 1
  1,  // Sprayer 2
  2,  // Sprayer 3
  3,  // Sprayer 4
  4,  // Sprayer 5
  5,  // Sprayer 6
  6,  // Sprayer 7
  7   // Sprayer 8
};

// ============================================================
// AI SETTINGS
// ============================================================

const int FRAME_WIDTH = 320;

// Minimum AI confidence
const float MIN_CONFIDENCE = 0.70;

// Spray time in milliseconds
const int SPRAY_DURATION = 250;

// ============================================================
// DETECTION RESULT
// ============================================================

struct WeedDetection {

  bool found;

  int x;
  int y;

  int width;
  int height;

  float confidence;
};

// ============================================================
// CAMERA SETUP
// ============================================================

void setupCamera() {

  camera_config_t config;

  config.ledc_channel =
    LEDC_CHANNEL_0;

  config.ledc_timer =
    LEDC_TIMER_0;

  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;

  config.pin_xclk =
    XCLK_GPIO_NUM;

  config.pin_pclk =
    PCLK_GPIO_NUM;

  config.pin_vsync =
    VSYNC_GPIO_NUM;

  config.pin_href =
    HREF_GPIO_NUM;

  config.pin_sccb_sda =
    SIOD_GPIO_NUM;

  config.pin_sccb_scl =
    SIOC_GPIO_NUM;

  config.pin_pwdn =
    PWDN_GPIO_NUM;

  config.pin_reset =
    RESET_GPIO_NUM;

  config.xclk_freq_hz =
    20000000;

  config.pixel_format =
    PIXFORMAT_RGB565;

  config.frame_size =
    FRAMESIZE_QVGA;

  config.jpeg_quality = 12;

  config.fb_count = 2;

  config.grab_mode =
    CAMERA_GRAB_LATEST;

  esp_err_t err =
    esp_camera_init(&config);

  if (err != ESP_OK) {

    Serial.printf(
      "Camera Error: 0x%x\n",
      err
    );

    return;
  }

  Serial.println(
    "Rear AI camera ready"
  );
}

// ============================================================
// INITIALIZE 8 SPRAYERS
// ============================================================

void setupSprayers() {

  for (
    int i = 0;
    i < 8;
    i++
  ) {

    mcp.pinMode(
      sprayerPins[i],
      OUTPUT
    );

    mcp.digitalWrite(
      sprayerPins[i],
      LOW
    );
  }
}

// ============================================================
// TURN ALL SPRAYERS OFF
// ============================================================

void allSprayersOFF() {

  for (
    int i = 0;
    i < 8;
    i++
  ) {

    mcp.digitalWrite(
      sprayerPins[i],
      LOW
    );
  }
}

// ============================================================
// CALCULATE WHICH SPRAYER
//
// Camera width = 320
//
// Zone 1 = 0-39
// Zone 2 = 40-79
// ...
// Zone 8 = 280-319
// ============================================================

int getSprayerZone(
  int weedCenterX
) {

  int zoneWidth =
    FRAME_WIDTH / 8;

  int zone =
    weedCenterX /
    zoneWidth;

  zone =
    constrain(
      zone,
      0,
      7
    );

  return zone + 1;
}

// ============================================================
// ACTIVATE ONE SPRAYER
// ============================================================

void sprayWeed(
  int zone
) {

  if (
    zone < 1 ||
    zone > 8
  ) {
    return;
  }

  allSprayersOFF();

  int pin =
    sprayerPins[
      zone - 1
    ];

  Serial.print(
    "SPRAYING ZONE: "
  );

  Serial.println(zone);

  mcp.digitalWrite(
    pin,
    HIGH
  );

  delay(SPRAY_DURATION);

  mcp.digitalWrite(
    pin,
    LOW
  );
}

// ============================================================
// SEND DATA TO FRONT XIAO
// ============================================================

void sendToFront(
  bool detected,
  int zone,
  int centerX,
  float confidence
) {

  if (
    WiFi.status() !=
    WL_CONNECTED
  ) {
    return;
  }

  HTTPClient http;

  String url =
    String(FRONT_SERVER) +
    "?detected=" +
    String(detected ? 1 : 0) +
    "&sprayer=" +
    String(zone) +
    "&x=" +
    String(centerX) +
    "&confidence=" +
    String(confidence * 100, 1);

  http.begin(url);

  int response =
    http.GET();

  Serial.print(
    "Front response: "
  );

  Serial.println(response);

  http.end();
}

// ============================================================
// AI WEED DETECTION
//
// IMPORTANT:
//
// Replace this function with your actual
// Edge Impulse / TensorFlow Lite / ESP-DL model.
//
// It should return:
// found
// x, y
// width, height
// confidence
// ============================================================

WeedDetection runWeedAI(
  camera_fb_t *fb
) {

  WeedDetection result;

  result.found = false;

  result.x = 0;
  result.y = 0;

  result.width = 0;
  result.height = 0;

  result.confidence = 0;

  /*
  ==========================================================

  YOUR AI MODEL GOES HERE

  Example conceptual output:

  if (
    detected_label == "weed" &&
    confidence > MIN_CONFIDENCE
  ) {

    result.found = true;

    result.x = boundingBoxX;
    result.y = boundingBoxY;

    result.width = boundingBoxWidth;
    result.height = boundingBoxHeight;

    result.confidence = confidence;
  }

  ==========================================================
  */

  return result;
}

// ============================================================
// PROCESS ONE CAMERA FRAME
// ============================================================

void detectAndSpray() {

  camera_fb_t *fb =
    esp_camera_fb_get();

  if (!fb) {

    Serial.println(
      "Camera capture failed"
    );

    return;
  }

  // Run AI model

  WeedDetection weed =
    runWeedAI(fb);

  if (
    weed.found &&
    weed.confidence >=
    MIN_CONFIDENCE
  ) {

    // Calculate bounding-box center

    int centerX =
      weed.x +
      (weed.width / 2);

    // Select one of 8 sprayers

    int zone =
      getSprayerZone(
        centerX
      );

    Serial.println(
      "WEED DETECTED"
    );

    Serial.print(
      "Confidence: "
    );

    Serial.println(
      weed.confidence
    );

    Serial.print(
      "Center X: "
    );

    Serial.println(
      centerX
    );

    Serial.print(
      "Zone: "
    );

    Serial.println(
      zone
    );

    // Send status first

    sendToFront(
      true,
      zone,
      centerX,
      weed.confidence
    );

    // Spray only selected zone

    sprayWeed(zone);

  }

  else {

    // Report no weed

    sendToFront(
      false,
      0,
      0,
      0
    );
  }

  esp_camera_fb_return(fb);
}

// ============================================================
// SETUP
// ============================================================

void setup() {

  Serial.begin(115200);

  // I2C for MCP23017

  Wire.begin();

  // MCP23017 default address 0x20

  if (!mcp.begin_I2C(0x20)) {

    Serial.println(
      "MCP23017 not found!"
    );

    while (true) {
      delay(1000);
    }
  }

  setupSprayers();

  setupCamera();

  // WiFi

  WiFi.begin(
    ssid,
    password
  );

  Serial.print(
    "Connecting WiFi"
  );

  while (
    WiFi.status() !=
    WL_CONNECTED
  ) {

    delay(500);

    Serial.print(".");
  }

  Serial.println();

  Serial.println(
    "Rear Weed AI System Ready"
  );

  Serial.println(
    WiFi.localIP()
  );
}

// ============================================================
// LOOP
// ============================================================

void loop() {

  detectAndSpray();

  // Small delay between AI frames

  delay(100);
}