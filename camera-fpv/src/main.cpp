// ============================================================
// main.cpp — FPV Camera · Freenove ESP32-S3-WROOM CAM (OV2640)
// Connects to the rover AP with static IP 192.168.4.2 and serves
// MJPEG at http://192.168.4.2:81/stream
// ============================================================
#include <Arduino.h>
#include <WiFi.h>
#include "esp_camera.h"
#include "esp_http_server.h"

// ---- Rover network: credentials from secrets.ini ----
#ifndef SECRET_AP_SSID
  #define SECRET_AP_SSID "Rover-4WD"
#endif
#ifndef SECRET_AP_PASS
  #define SECRET_AP_PASS "cambiame123"
#endif
#define WIFI_SSID  SECRET_AP_SSID
#define WIFI_PASS  SECRET_AP_PASS
IPAddress localIP(192, 168, 4, 2);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

// ---- Camera pins: Freenove ESP32-S3-WROOM CAM ----
#define PWDN_GPIO  -1
#define RESET_GPIO -1
#define XCLK_GPIO  15
#define SIOD_GPIO  4
#define SIOC_GPIO  5
#define Y9_GPIO    16
#define Y8_GPIO    17
#define Y7_GPIO    18
#define Y6_GPIO    12
#define Y5_GPIO    10
#define Y4_GPIO    8
#define Y3_GPIO    9
#define Y2_GPIO    11
#define VSYNC_GPIO 6
#define HREF_GPIO  7
#define PCLK_GPIO  13

httpd_handle_t streamServer = nullptr;

// ------------------------------------------------------------
// MJPEG handler: multipart/x-mixed-replace
// ------------------------------------------------------------
static esp_err_t streamHandler(httpd_req_t* req) {
  static const char* BOUNDARY = "123456789000000000000987654321";
  char hdr[64];

  httpd_resp_set_type(req, "multipart/x-mixed-replace;boundary=123456789000000000000987654321");
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");

  while (true) {
    camera_fb_t* fb = esp_camera_fb_get();
    if (!fb) { vTaskDelay(pdMS_TO_TICKS(10)); continue; }

    size_t hlen = snprintf(hdr, sizeof(hdr),
      "\r\n--%s\r\nContent-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n",
      BOUNDARY, fb->len);

    esp_err_t res = httpd_resp_send_chunk(req, hdr, hlen);
    if (res == ESP_OK) res = httpd_resp_send_chunk(req, (const char*)fb->buf, fb->len);
    esp_camera_fb_return(fb);

    if (res != ESP_OK) break;   // client disconnected
  }
  return ESP_OK;
}

void startStreamServer() {
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = 81;
  config.ctrl_port = 3281;

  httpd_uri_t uri = {
    .uri = "/stream", .method = HTTP_GET,
    .handler = streamHandler, .user_ctx = nullptr
  };
  if (httpd_start(&streamServer, &config) == ESP_OK)
    httpd_register_uri_handler(streamServer, &uri);
}

// ------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  Serial.println("\n[CAM] Starting...");

  camera_config_t cfg = {};
  cfg.ledc_channel = LEDC_CHANNEL_0;
  cfg.ledc_timer   = LEDC_TIMER_0;
  cfg.pin_d0 = Y2_GPIO;  cfg.pin_d1 = Y3_GPIO;
  cfg.pin_d2 = Y4_GPIO;  cfg.pin_d3 = Y5_GPIO;
  cfg.pin_d4 = Y6_GPIO;  cfg.pin_d5 = Y7_GPIO;
  cfg.pin_d6 = Y8_GPIO;  cfg.pin_d7 = Y9_GPIO;
  cfg.pin_xclk  = XCLK_GPIO;
  cfg.pin_pclk  = PCLK_GPIO;
  cfg.pin_vsync = VSYNC_GPIO;
  cfg.pin_href  = HREF_GPIO;
  cfg.pin_sccb_sda = SIOD_GPIO;
  cfg.pin_sccb_scl = SIOC_GPIO;
  cfg.pin_pwdn  = PWDN_GPIO;
  cfg.pin_reset = RESET_GPIO;
  cfg.xclk_freq_hz = 20000000;
  cfg.pixel_format = PIXFORMAT_JPEG;
  cfg.frame_size   = FRAMESIZE_VGA;     // 640x480: FPV balance
  cfg.jpeg_quality = 12;                // 0-63 (lower = better quality)
  cfg.fb_count     = 2;                 // double buffer with PSRAM
  cfg.fb_location  = CAMERA_FB_IN_PSRAM;
  cfg.grab_mode    = CAMERA_GRAB_LATEST;

  if (esp_camera_init(&cfg) != ESP_OK) {
    Serial.println("[CAM] ERROR: camera init failed");
    while (true) delay(1000);
  }

  // Sensor tuning for outdoors
  sensor_t* s = esp_camera_sensor_get();
  s->set_brightness(s, 0);
  s->set_saturation(s, 1);
  s->set_whitebal(s, 1);
  s->set_exposure_ctrl(s, 1);
  s->set_gain_ctrl(s, 1);

  // WiFi client of the rover AP with static IP
  WiFi.mode(WIFI_STA);
  WiFi.config(localIP, gateway, subnet);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  WiFi.setSleep(false);   // lower stream latency

  Serial.print("[CAM] Connecting to rover");
  while (WiFi.status() != WL_CONNECTED) { delay(400); Serial.print("."); }
  Serial.printf("\n[CAM] Connected: http://%s:81/stream\n",
                WiFi.localIP().toString().c_str());

  startStreamServer();
}

void loop() {
  // Automatic reconnection if the rover reboots
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.reconnect();
    delay(2000);
  }
  delay(500);
}
