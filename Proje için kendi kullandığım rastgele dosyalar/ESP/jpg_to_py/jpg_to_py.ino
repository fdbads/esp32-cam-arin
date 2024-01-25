/* Data stream successfu */

#include "esp_camera.h"
#include "Arduino.h"
#include "FS.h"                // SD Card ESP32
#include "SD_MMC.h"            // SD Card ESP32
#include "soc/soc.h"           // Disable brownour problems
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include "driver/rtc_io.h"
#include <EEPROM.h>            // read and write from flash memory

#include <stdbool.h>


// define the number of bytes you want to access
#define EEPROM_SIZE 1

// Pin definition for CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

unsigned char rx = 0;

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  
  pinMode(4, OUTPUT);
  //digitalWrite(4, HIGH);delay(40);digitalWrite(4, LOW);
  digitalWrite(4, LOW);
  
  Serial.begin(115200);
  //Serial.setDebugOutput(true);
  //Serial.println();
  while (1){
    if(Serial.available()){
      if (Serial.read() == 'x') {
        break;
      }
    }
    delay(200);
  }
  
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG; 
  
  //if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
    config.jpeg_quality = 4;
    config.fb_count = 1;
  /*} else {
    config.frame_size = FRAMESIZE_XGA;
    config.jpeg_quality = 5;
    config.fb_count = 1;
  }*/
  
  // Init Camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    //digitalWrite(4, HIGH);delay(40);digitalWrite(4, LOW);
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  //digitalWrite(4, HIGH);delay(40);digitalWrite(4, LOW);delay(40);
  //digitalWrite(4, HIGH);delay(40);digitalWrite(4, LOW);delay(40);
  for (int i=0;i<10;i++){
    camera_fb_t * fb = NULL;
    fb = esp_camera_fb_get();
    esp_camera_fb_return(fb);
    delay(100);
  }
  //digitalWrite(4, HIGH);delay(40);digitalWrite(4, LOW);delay(40);
  //digitalWrite(4, HIGH);delay(40);digitalWrite(4, LOW);delay(40);
    
  camera_fb_t * fb = NULL;
  
  // Take Picture with Camera
  //digitalWrite(4, HIGH);delay(100);
  fb = esp_camera_fb_get(); 
  //digitalWrite(4, LOW);delay(40);
  //digitalWrite(4, HIGH);delay(40);digitalWrite(4, LOW);delay(40);
  //digitalWrite(4, HIGH);delay(40);digitalWrite(4, LOW);delay(40);
  
  if(!fb) {
    Serial.println("Camera capture failed");
    return;
  }
  //file.write(fb->buf, fb->len); // payload (image), payload length

  Serial.print("@#@");
  for (int i = 0; i < fb->len; i++)
  {
     Serial.write(fb->buf[i]);
  }
  Serial.print("#@#");
  
  esp_camera_fb_return(fb); 
  
  // Turns off the ESP32-CAM white on-board LED (flash) connected to GPIO 4
  digitalWrite(4, LOW);
  rtc_gpio_hold_en(GPIO_NUM_4);
  
  delay(2000);
  Serial.println("Going to sleep now");
  delay(2000);
  esp_deep_sleep_start();
  
}

void loop() {
  
}
