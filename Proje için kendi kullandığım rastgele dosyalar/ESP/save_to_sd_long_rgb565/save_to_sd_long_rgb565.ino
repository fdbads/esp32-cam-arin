/* Data stream successfu */

#include "esp_camera.h"
#include "Arduino.h"
#include "FS.h"                // SD Card ESP32
#include "SD_MMC.h"            // SD Card ESP32
#include "soc/soc.h"           // Disable brownour problems
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include "driver/rtc_io.h"
//#include <EEPROM.h>            // read and write from flash memory

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

unsigned int pictureNumber = 0;

unsigned long time_capture = 0;
unsigned long time_capture_temp = 0;

unsigned long time_write = 0;
unsigned long time_write_temp = 0;


void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  
  //rtc_gpio_hold_en(GPIO_NUM_4);
  
  Serial.begin(115200);
  
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
  config.pixel_format = PIXFORMAT_RGB565; 
  
  config.frame_size = FRAMESIZE_UXGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
  config.fb_count = 1;
  
  // Init Camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    //digitalWrite(4, HIGH);delay(40);digitalWrite(4, LOW);
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  /*
  for (int i=0;i<10;i++){
    
    camera_fb_t * fb = NULL;
    
    fb = esp_camera_fb_get();
    if(!fb) Serial.printf("Camera init capture %d failed\n",i); return;
  
    esp_camera_fb_return(fb);
    delay(200);
  }
  */
  if(!SD_MMC.begin()){
    Serial.println("SD card mount failed");
    return;
  }
  else Serial.println("SD card mounted");

  uint8_t cardType = SD_MMC.cardType();
  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }
  else Serial.printf("SD card found with type: %d",cardType);



}

void loop() {
  
  
  Serial.println("---");
  camera_fb_t * fb = NULL;
  
  // Take Picture with Camera
  time_capture_temp = millis();
  fb = esp_camera_fb_get(); 
  time_capture = millis() - time_capture_temp;

  if(fb == NULL) {
    Serial.println("Camera capture failed");
  }
  else{

    // Path where new picture will be saved in SD Card
    
    String path = "/rgb565_" + String(pictureNumber) +".txt";

    fs::FS &fs = SD_MMC; 
    Serial.printf("Picture file name: %s\n", path.c_str());
    
    File file = fs.open(path.c_str(), FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file in writing mode");
    } 
    else {
        
        time_write_temp = millis();
        file.write(fb->buf, fb->len); // payload (image), payload length
        time_write = millis() - time_write_temp;
        
        Serial.printf("Saved file to path: %s\n", path.c_str());
        //EEPROM.write(0, pictureNumber);
        //EEPROM.commit();

        file.close();

        Serial.printf("Saved photo  %d\n",pictureNumber);
        Serial.printf("Capture time:  %d\n",time_capture);
        Serial.printf("Write time:  %d\n",time_write);
        pictureNumber += 1;
        
        if (pictureNumber >= 40){
            Serial.println("Going to sleep now");
            
            delay(2000);
            esp_deep_sleep_start();
        }

    }
    
    
  }

  esp_camera_fb_return(fb); 

  Serial.println("+++");
  delay(100);

}
