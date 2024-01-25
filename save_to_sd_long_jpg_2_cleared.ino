//Notlar dokümanında her notun açıklamasını bulabilirsiniz.
//NOT 1 
#include "esp_camera.h"
#include "Arduino.h"
#include "soc/soc.h"           // Disable brownout problems
#include "soc/rtc_cntl_reg.h"  // Disable brownout problems
#include "driver/rtc_io.h"

#include <stdbool.h>

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


//NOT 2
#define burst_size 1
#define pic_batch_count 500

unsigned int pictureNumber = 0;

unsigned long time_capture = 0;
unsigned long time_capture_temp = 0;

unsigned long time_write = 0;
unsigned long time_write_temp = 0;

unsigned long init_time;

unsigned char error_flag = 0;

unsigned int burst_index = 0;
unsigned int temp2;


camera_config_t config;
sensor_t * s;

//NOT 3
void mem_monitor(){

  Serial.printf("---------------------------\n");
  Serial.printf("-8-bit Addressable\n");
  Serial.printf("%d / ",heap_caps_get_free_size(MALLOC_CAP_8BIT));
  Serial.printf("%d | ",heap_caps_get_total_size(MALLOC_CAP_8BIT));
  Serial.printf("%d\n",heap_caps_get_largest_free_block(MALLOC_CAP_8BIT));

  Serial.printf("\n-Internal\n");
  Serial.printf("%d / ",heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
  Serial.printf("%d | ",heap_caps_get_total_size(MALLOC_CAP_INTERNAL));
  Serial.printf("%d\n",heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL));
  
  Serial.printf("\n-SPI\n");
  Serial.printf("%d / ",heap_caps_get_free_size(MALLOC_CAP_SPIRAM ));
  Serial.printf("%d | ",heap_caps_get_total_size(MALLOC_CAP_SPIRAM ));
  Serial.printf("%d\n",heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM ));
  
  


}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  
  
  Serial.begin(115200);
  Serial.printf("---------------------------\n");
  Serial.println("Booted");
  
  init_time = millis();

  //NOT 4
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
  config.jpeg_quality = 4;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.frame_size = FRAMESIZE_UXGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
  config.fb_count = burst_size;
  
  // Init Camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    //digitalWrite(4, HIGH);delay(40);digitalWrite(4, LOW);
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }


  s = esp_camera_sensor_get();
  
  //s->set_brightness(s, -2);     // -2 to 2
  s->set_contrast(s, 1);       // -2 to 2
  //s->set_saturation(s, 0);     // -2 to 2
  ////s->set_special_effect(s, 1); // 0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
  s->set_whitebal(s, 1);       // 0 = disable , 1 = enable
  s->set_awb_gain(s, 1);       // 0 = disable , 1 = enable
  s->set_wb_mode(s, 0);        // 0 to 4 - if awb_gain enabled (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
  s->set_exposure_ctrl(s, 1);  // 0 = disable , 1 = enable
  // keep commented s->set_aec2(s, 1);           // 0 = disable , 1 = enable
  //s->set_ae_level(s, 0);       // -2 to 2
  //s->set_aec_value(s, 300);    // 0 to 1200
  s->set_gain_ctrl(s, 1);      // 0 = disable , 1 = enable
  //s->set_agc_gain(s, 0);       // 0 to 30
  //s->set_gainceiling(s, (gainceiling_t)0);  // 0 to 6
  s->set_bpc(s, 1);            // 0 = disable , 1 = enable
  s->set_wpc(s, 1);            // 0 = disable , 1 = enable
  //s->set_raw_gma(s, 1);        // 0 = disable , 1 = enable
  //s->set_lenc(s, 1);           // 0 = disable , 1 = enable
  //s->set_hmirror(s, 0);        // 0 = disable , 1 = enable
  //s->set_vflip(s, 0);          // 0 = disable , 1 = enable
  //s->set_dcw(s, 1);            // 0 = disable , 1 = enable
  //s->set_colorbar(s, 1);       // 0 = disable , 1 = enable
  
  /*
  for (int i=0;i<10;i++){
    
    camera_fb_t * fb_0 = NULL;
    
    fb_0 = esp_camera_fb_get();
    if(!fb_0) Serial.printf("Camera init capture %d failed\n",i); return;
  
    esp_camera_fb_return(fb_0);
    delay(100);
  }
  */

  //NOT 5
  if(!SD_MMC.begin()){
    Serial.println("SD card mount failed");
    delay(2000);
    esp_deep_sleep_start();
    return;
  }
  else Serial.println("SD card mounted");

  uint8_t cardType = SD_MMC.cardType();
  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    delay(2000);
    esp_deep_sleep_start();
    return;
  }
  else Serial.printf("SD card found with type: %d\n",cardType);

  /* Dummy run for green init problem*/
  camera_fb_t * fb_0 = NULL;
  //camera_fb_t * fb_1 = NULL;
  //camera_fb_t * fb_2 = NULL;
  //camera_fb_t * fb_3 = NULL;


  //NOT 6
  fb_0 = esp_camera_fb_get();
  if(fb_0 != NULL){Serial.printf("| d0 cap ");} else {Serial.printf("| d0 fail ");}
  delay(100);
  esp_camera_fb_return(fb_0);
  delay(100);
  
  fb_0 = esp_camera_fb_get();
  if(fb_0 != NULL){Serial.printf("| d1 cap ");} else {Serial.printf("| d1 fail ");}
  delay(100);
  esp_camera_fb_return(fb_0);
  delay(100);
  
  fb_0 = esp_camera_fb_get();
  if(fb_0 != NULL){Serial.printf("| d2 cap ");} else {Serial.printf("| d2 fail ");}
  delay(100);
  esp_camera_fb_return(fb_0);
  delay(100);
  
  fb_0 = esp_camera_fb_get();
  if(fb_0 != NULL){Serial.printf("| d3 cap ");} else {Serial.printf("| d3 fail ");}
  delay(100);
  esp_camera_fb_return(fb_0);
  delay(100);
  
  /*
  fb_1 = esp_camera_fb_get();
  if(fb_1 != NULL){Serial.printf("| d1 cap ");} else {Serial.printf("| d1 fail ");}
  delay(100);
  fb_2 = esp_camera_fb_get();
  if(fb_2 != NULL){Serial.printf("| d2 cap ");} else {Serial.printf("| d2 fail ");}
  delay(100);
  fb_3 = esp_camera_fb_get();
  if(fb_3 != NULL){Serial.printf("| d3 cap\n");} else {Serial.printf("| d3 fail\n");}
  delay(100);
  
  
  esp_camera_fb_return(fb_1);
  esp_camera_fb_return(fb_2);
  esp_camera_fb_return(fb_3);
  */
  
}

void loop() {
  
  Serial.printf("---------------------------\n");
  
  //NOT 7
  temp2 = (int)(((burst_index)/50));
  Serial.printf("burst property: no change %d | burst_index: %d\n",temp2,burst_index);
  //s->set_agc_gain(s, temp2);
  
  


  /* Frame buffer pointers*/
  
  /*
  camera_fb_t * fb_1 = NULL;
  camera_fb_t * fb_2 = NULL;
  camera_fb_t * fb_3 = NULL;
  camera_fb_t * fb_4 = NULL;
  camera_fb_t * fb_5 = NULL;
  camera_fb_t * fb_6 = NULL;
  camera_fb_t * fb_7 = NULL;
  */
  
  
  // Take Picture with Camera
  time_capture_temp = millis();
  
  fb_0 = esp_camera_fb_get();
  Serial.printf("| p0 cap ");
  /*
  fb_1 = esp_camera_fb_get();
  Serial.printf("| p1 cap ");
  fb_2 = esp_camera_fb_get();
  Serial.printf("| p2 cap ");
  fb_3 = esp_camera_fb_get();
  Serial.printf("| p3 cap\n");
  fb_4 = esp_camera_fb_get();
  Serial.printf("| p4 cap ");
  fb_5 = esp_camera_fb_get();
  Serial.printf("| p5 cap ");
  fb_6 = esp_camera_fb_get();
  Serial.printf("| p6 cap\n");
  fb_7 = esp_camera_fb_get();
  Serial.printf("pic 7 captured\n");
  */
  
  
  time_capture = millis() - time_capture_temp;

  if(fb_0 == NULL){// || fb_1 == NULL || fb_2 == NULL || fb_3 == NULL || 
     //fb_4 == NULL || fb_5 == NULL || fb_6 == NULL || fb_7 == NULL) {
    if(fb_0 == NULL) Serial.println("c0 failed");
    //if(fb_1 == NULL) Serial.println("Camera capture 1 failed");
    //if(fb_2 == NULL) Serial.println("Camera capture 2 failed");
    //if(fb_3 == NULL) Serial.println("Camera capture 3 failed");
    //if(fb_4 == NULL) Serial.println("Camera capture 4 failed");
    //if(fb_5 == NULL) Serial.println("Camera capture 5 failed");
    //if(fb_6 == NULL) Serial.println("Camera capture 6 failed");
    //if(fb_7 == NULL) Serial.println("Camera capture 7 failed");
    
  }
  else{

    fs::FS &fs = SD_MMC;
    // Path where new picture will be saved in SD Card
    
    time_write_temp = millis();

    String path_0 = "/jpg_" + String(pictureNumber) +".jpg";
    File file_0 = fs.open(path_0.c_str(), FILE_WRITE);
    if(!file_0) {
      Serial.println("Failed to open file 0 in writing mode");
      error_flag = 1;
    }
    else{
      file_0.write(fb_0->buf, fb_0->len);
      Serial.printf("Saved file to path: %s\n", path_0.c_str());
      file_0.close();
    }
    /*
    String path_1 = "/jpg_" + String(pictureNumber+1) +".jpg";
    File file_1 = fs.open(path_1.c_str(), FILE_WRITE);
    if(!file_1) {
      Serial.println("Failed to open file 1 in writing mode");
      error_flag = 1;
    }
    else{
      file_1.write(fb_1->buf, fb_1->len);
      Serial.printf("Saved file to path: %s\n", path_1.c_str());
      file_1.close();
    }
    String path_2 = "/jpg_" + String(pictureNumber+2) +".jpg";
    File file_2 = fs.open(path_2.c_str(), FILE_WRITE);
    if(!file_2) {
      Serial.println("Failed to open file 2 in writing mode");
      error_flag = 1;
    }
    else{
      file_2.write(fb_2->buf, fb_2->len);
      Serial.printf("Saved file to path: %s\n", path_2.c_str());
      file_2.close();
    }
    String path_3 = "/jpg_" + String(pictureNumber+3) +".jpg";
    File file_3 = fs.open(path_3.c_str(), FILE_WRITE);
    if(!file_3) {
      Serial.println("Failed to open file 3 in writing mode");
      error_flag = 1;
    }
    else{
      file_3.write(fb_3->buf, fb_3->len);
      Serial.printf("Saved file to path: %s\n", path_3.c_str());
      file_3.close();
    }
    String path_4 = "/jpg_" + String(pictureNumber+4) +".jpg";
    File file_4 = fs.open(path_4.c_str(), FILE_WRITE);
    if(!file_4) {
      Serial.println("Failed to open file 4 in writing mode");
      error_flag = 1;
    }
    else{
      file_4.write(fb_4->buf, fb_4->len);
      Serial.printf("Saved file to path: %s\n", path_4.c_str());
      file_4.close();
    }
    String path_5 = "/jpg_" + String(pictureNumber+5) +".jpg";
    File file_5 = fs.open(path_5.c_str(), FILE_WRITE);
    if(!file_5) {
      Serial.println("Failed to open file 5 in writing mode");
      error_flag = 1;
    }
    else{
      file_5.write(fb_5->buf, fb_5->len);
      Serial.printf("Saved file to path: %s\n", path_5.c_str());
      file_5.close();
    }
    String path_6 = "/jpg_" + String(pictureNumber+6) +".jpg";
    File file_6 = fs.open(path_6.c_str(), FILE_WRITE);
    if(!file_6) {
      Serial.println("Failed to open file 6 in writing mode");
      error_flag = 1;
    }
    else{
      file_6.write(fb_6->buf, fb_6->len);
      Serial.printf("Saved file to path: %s\n", path_6.c_str());
      file_6.close();
    }
    String path_7 = "/jpg_" + String(pictureNumber+7) +".jpg";
    File file_7 = fs.open(path_7.c_str(), FILE_WRITE);
    if(!file_7) {
      Serial.println("Failed to open file 7 in writing mode");
      error_flag = 1;
    }
    else{
      file_7.write(fb_7->buf, fb_7->len);
      Serial.printf("Saved file to path: %s\n", path_7.c_str());
      file_7.close();
    }
    */
    time_write = millis() - time_write_temp;

    if (error_flag == 1){
      Serial.printf("Error flag raised\n");
      delay(2000);
      esp_deep_sleep_start();
      return;
    }
    else {
      
      Serial.printf("-----\n");
      Serial.printf("|Capture time:  %d\n",time_capture);
      Serial.printf("|FPS: %f\n",1000/(float)(time_capture+time_write));
      Serial.printf("|Write time:  %d\n",time_write);
      Serial.printf("|Total runtime:  %d\n",(millis()-init_time));
      Serial.printf("|Average pic/min: %f\n",((float)60000/(millis()-init_time)));
      Serial.printf("-----\n");
      //Serial.printf("Picture number:  %d\n",pictureNumber);
      
      pictureNumber += burst_size;
      
      if (burst_index+1 >= pic_batch_count){
          Serial.println("\nGoing to sleep now");
          delay(2000);
          esp_deep_sleep_start();
      }
      
    }
    
    
  }
  //mem_monitor();

  esp_camera_fb_return(fb_0);
  /*
  esp_camera_fb_return(fb_1); 
  esp_camera_fb_return(fb_2); 
  esp_camera_fb_return(fb_3); 
  esp_camera_fb_return(fb_4);
  esp_camera_fb_return(fb_5); 
  esp_camera_fb_return(fb_6); 
  esp_camera_fb_return(fb_7); 
  */
  
  burst_index++;

  //Serial.println("+++");
  //delay(500);

}
