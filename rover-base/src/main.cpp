// ============================================================
// main.cpp — Rover 4WD · Phase 1
// Core 0: WiFi AP + web server (joysticks, telemetry)
// Core 1: motors, encoders, IMU, LEDs, buzzer, battery
// ============================================================
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "config.h"
#include "motors.h"
#include "encoders.h"
#include "imu.h"
#include "leds.h"
#include "buzzer.h"
#include "battery.h"
#include "webui.h"

WebServer server(80);

// Current joystick command (shared between cores)
volatile int8_t cmdX = 0, cmdY = 0;
volatile uint32_t lastCmdMs = 0;
volatile bool findRequest = false;

// ------------------------------------------------------------
// HTTP handlers (core 0)
// ------------------------------------------------------------
void handleRoot() {
  String html = FPSTR(WEB_UI);
  html.replace("%CAM_URL%", CAMERA_STREAM_URL);
  html.replace("%CAM_ON%", CAMERA_ENABLED ? "1" : "0");
  server.send(200, "text/html", html);
}

void handleCmd() {
  cmdX = constrain(server.arg("x").toInt(), -100, 100);
  cmdY = constrain(server.arg("y").toInt(), -100, 100);
  lastCmdMs = millis();
  server.send(200, "text/plain", "ok");
}

void handleLights() {
  Leds::headlightsOn = !Leds::headlightsOn;
  server.send(200, "text/plain", Leds::headlightsOn ? "1" : "0");
}

void handleFind() {
  findRequest = true;
  server.send(200, "text/plain", "ok");
}

// Gear selector: /gear?v=0 (low) | 1 (normal) | 2 (high)
void handleGear() {
  int g = server.arg("v").toInt();
  Motors::maxSpeedPct = (0 == g) ? GEAR_LOW_PCT
                      : (2 == g) ? GEAR_HIGH_PCT
                      : GEAR_NORMAL_PCT;
  server.send(200, "text/plain", String(Motors::maxSpeedPct));
}

void handleStatus() {
  const char* st = "OK";
  switch (IMU::state) {
    case IMU::ROLLED_OVER: st = "ROLLED"; break;
    case IMU::LIFTED:      st = "LIFTED"; break;
    case IMU::TILT_WARN:   st = "TILT";   break;
    default: break;
  }
  char buf[220];
  snprintf(buf, sizeof(buf),
    "{\"v\":%.2f,\"pct\":%u,\"low\":%s,\"crit\":%s,"
    "\"spd\":%.3f,\"rpm\":%.1f,\"dist\":%.0f,"
    "\"pitch\":%.1f,\"roll\":%.1f,\"state\":\"%s\"}",
    Battery::voltage, Battery::percent,
    Battery::low ? "true" : "false", Battery::critical ? "true" : "false",
    Encoders::speedMS(), Encoders::avgRPM(), Encoders::totalDistanceMM,
    IMU::pitch, IMU::roll, st);
  server.send(200, "application/json", buf);
}

// ------------------------------------------------------------
// Control task (core 1) — real time, never blocks
// ------------------------------------------------------------
void controlTask(void* pv) {
  uint32_t lastImu = 0, lastBat = 0, lastLed = 0;

  for (;;) {
    uint32_t now = millis();

    // --- Failsafe: no recent command -> stop ---
    bool timedOut = (now - lastCmdMs) > CMD_TIMEOUT_MS;

    // --- IMU safety: rolled over or lifted -> motors OFF ---
    bool unsafe = (IMU::state == IMU::ROLLED_OVER || IMU::state == IMU::LIFTED);

    if (unsafe || timedOut) Motors::stop();
    else                    Motors::driveJoystick(cmdX, cmdY);

    // --- Encoders (every ENCODER_CALC_MS internally) ---
    Encoders::update();

    // --- IMU ---
    if (now - lastImu >= IMU_LOOP_MS) { lastImu = now; IMU::update(); }

    // --- Battery ---
    if (now - lastBat >= TELEMETRY_MS) {
      lastBat = now;
      Battery::update();
      Buzzer::lowBattery = Battery::low;
    }

    // --- LEDs ---
    if (now - lastLed >= 50) { lastLed = now; Leds::update(); }

    // --- Buzzer (continuous alarms) ---
    Buzzer::update();

    // --- "Find me" request from the web UI ---
    if (findRequest) {
      findRequest = false;
      Buzzer::playFind();
      Leds::flashFind();
    }

    vTaskDelay(pdMS_TO_TICKS(CONTROL_LOOP_MS));
  }
}

// ------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  Serial.println("\n[Rover] Starting Phase 1...");

  Motors::begin();
  Encoders::begin();
  IMU::begin();
  Leds::begin();
  Buzzer::begin();
  Battery::begin();

  Serial.printf("[Rover] IMU %s\n", IMU::present ? "OK" : "NOT DETECTED");

  // WiFi AP
  WiFi.mode(WIFI_AP);
  WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASS, WIFI_AP_CHANNEL);
  Serial.printf("[Rover] AP '%s' -> http://%s\n",
                WIFI_AP_SSID, WiFi.softAPIP().toString().c_str());

  // Web routes
  server.on("/",       handleRoot);
  server.on("/cmd",    handleCmd);
  server.on("/lights", handleLights);
  server.on("/find",   handleFind);
  server.on("/gear",   handleGear);
  server.on("/status", handleStatus);
  server.begin();

  // Control task on core 1 (APP_CPU)
  xTaskCreatePinnedToCore(controlTask, "control", 8192, nullptr, 2, nullptr, 1);

  Buzzer::playBoot();
  Serial.println("[Rover] Ready.");
}

// loop() runs on core 1 by default in Arduino-ESP32, but the server is
// lightweight; we serve it here and the control task runs separately
// with higher priority.
void loop() {
  server.handleClient();
  delay(2);
}
