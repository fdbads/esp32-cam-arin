/* 
 *  Using ESP32 Camera Module
 *  
 *  *  
 * Modified by Ahmad Shamshiri for Robojax Robojax.com
 * on Sep 14, 2019 in Ajax, Ontario, Canada
 Watch the video instruction for this sketch: 
 https://youtu.be/5XCb3t8J4Kg
 
 
You can get the wiring diagram from my Arduino Course at Udemy.com
Learn Arduino step by step with all library, codes, wiring diagram all in one place
visit my course now http://robojax.com/L/?id=62

If you found this tutorial helpful, please support me so I can continue creating 
content like this. You can support me on Patreon http://robojax.com/L/?id=63
or make donation using PayPal http://robojax.com/L/?id=64

Original code by BARRAGAN <http://barraganstudio.com>
* 
 * Code is available at http://robojax.com/learn/arduino

 * This code is "AS IS" without warranty or liability. Free to be used as long as you keep this note intact.* 
 * This code has been download from Robojax.com
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>. 

*/
//
// WARNING!!! Make sure that you have either selected ESP32 Wrover Module,
//            or another board which has PSRAM enabled
//

// Select camera model
//#define CAMERA_MODEL_WROVER_KIT
//#define CAMERA_MODEL_ESP_EYE
//#define CAMERA_MODEL_M5STACK_PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE
#define CAMERA_MODEL_AI_THINKER

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

#include "BluetoothSerial.h"
BluetoothSerial SerialBT;
#include "esp_camera.h"

unsigned int resultsToShow = 10;                 // how much data to display
String tReply = "";  // reply to send to pc

const int im_scale = 10;
unsigned long long fb_len;

uint8_t ARR[10] = {1,2,3,4,5,6,7,8,9,10};
uint8_t *pARR = ARR;

//unsigned char main_loop_ct = 0;

void setup() {
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);delay(200);
  digitalWrite(4, HIGH);delay(40);digitalWrite(4, LOW);
  
  SerialBT.begin("ESP32 Bluetooth");
  //Serial.begin(9600);
  
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
  config.fb_count = 1;
  config.frame_size = FRAMESIZE_QVGA;

  //esp_camera_init(&config);
  
  //init with high specs to pre-allocate larger buffers
  /*
  if(psramFound()){
    config.frame_size = FRAMESIZE_QVGA;
    //config.jpeg_quality = 10;
    config.fb_count = 1;
    
  } else {
    config.frame_size = FRAMESIZE_QVGA;
    //config.jpeg_quality = 12;
    config.fb_count = 1;
    
    while (1 == 1)digitalWrite(4, HIGH);delay(100);digitalWrite(4, LOW);delay(500);
    
    
  }
  */
#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    SerialBT.printf("Camera init failed with error 0x%x", err);
    return;
  }

  //sensor_t * s = esp_camera_sensor_get();
  //initial sensors are flipped vertically and colors are a bit saturated
  
  /*if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);//flip it back
    s->set_brightness(s, 1);//up the blightness just a bit
    s->set_saturation(s, -2);//lower the saturation
  }
  //drop down frame size for higher initial frame rate
  s->set_framesize(s, FRAMESIZE_QVGA);
  */

//#if defined(CAMERA_MODEL_M5STACK_WIDE)
//  s->set_vflip(s, 1);
 // s->set_hmirror(s, 1);
//#endif

}

void loop() {
  // put your main code here, to run repeatedly:
  
  //digitalWrite(4, HIGH);delay(40);digitalWrite(4, LOW);
  //Serial.print("@");
  //Serial.printf(main_loop_ct);
  //main_loop_ct++;
  SerialBT.print("@#@"); //Packet start 
  /*
  SerialBT.print("free_mem");
  SerialBT.print(heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
  SerialBT.print("###");
  SerialBT.print(heap_caps_get_free_size(MALLOC_CAP_8BIT));
  SerialBT.print("###");
  */
  
  // capture live image (jpg)
    
    camera_fb_t * fb = NULL;
    //digitalWrite(4, HIGH);delay(100);
    fb = esp_camera_fb_get();
    //digitalWrite(4, LOW);
    
    //if (!fb) tReply+=" -Error capturing image from camera- ";  
    //tReply+="(Image resolution=" + String(fb->width) + "x" + String(fb->height) + ")";         // display image resolution        

  /* 
  // allocate memory to store rgb data in psram
    //if (!psramFound()) tReply+=" -Error no psram found- ";
    void *ptrVal = NULL;
    uint32_t ARRAY_LENGTH = fb->width * fb->height * 3;               // number of pixels in the jpg image x 3
    //SerialBT.print(ARRAY_LENGTH);
    //SerialBT.print("###");
    
    //if (heap_caps_get_free_size( MALLOC_CAP_SPIRAM) <  ARRAY_LENGTH) tReply+=" -Error not enough free psram to store rgb data- ";      // check free memory in psram
    ptrVal = heap_caps_malloc(ARRAY_LENGTH, MALLOC_CAP_SPIRAM);  
    uint8_t *rgb = (uint8_t *)ptrVal;
    //SerialBT.print(heap_caps_get_free_size( MALLOC_CAP_SPIRAM));
    //SerialBT.print("###");
  
  // convert jpg to rgb (store in an array 'rgb')
    bool jpeg_converted = fmt2rgb888(fb->buf, fb->len, PIXFORMAT_JPEG, rgb);    
    if (!jpeg_converted) tReply+=" -Error converting image to RGB- ";
 
  
  // display some of the resulting data
      for (uint32_t i = 0; i < (int)ARRAY_LENGTH/im_scale; i++) {
        // // x and y coordinate of the pixel
        //   uint16_t x = i % fb->width;
        //   uint16_t y = floor(i / fb->width);

        
        if (i%3 == 0) tReply+="B";
        else if (i%3 == 1) tReply+="G";
        else if (i%3 == 2) tReply+="R";
        tReply+= String(rgb[i]) + ",";
        
        tReply+= (char)rgb[i*im_scale];
        
      }
  
       
  // free up memory
    
    heap_caps_free(ptrVal);
  
  */
  
  //SerialBT.write(*(fb->buf)); //sends 0xFF
  /*
  fb_len = (unsigned long long)(fb->len);
  SerialBT.write(((fb_len)>>(8*0))& 0xFF);
  SerialBT.write(((fb_len)>>(8*1))& 0xFF);
  SerialBT.write(((fb_len)>>(8*2))& 0xFF);
  SerialBT.write(((fb_len)>>(8*3))& 0xFF);
  SerialBT.write(((fb_len)>>(8*4))& 0xFF);
  SerialBT.write(((fb_len)>>(8*5))& 0xFF);
  SerialBT.write(((fb_len)>>(8*6))& 0xFF);
  SerialBT.write(((fb_len)>>(8*7))& 0xFF);  
  */
  
  if (!fb) Serial.print(" -Error capturing image from camera- ");

  //SerialBT.write(pARR,10);
  
  SerialBT.write(*(fb->buf));
  
  //SerialBT.print((fb->len));
  
  // invalid conversion from 'uint8_t' {aka 'unsigned char'} to 'const uint8_t*' {aka 
  // 'const unsigned char*'} [-fpermissive]
  
  //"pointer olan bir şete pointer olmayan bir şety vermeye çalışıyorsun"
  
  
  //tReply = "LIVE IMAGE AS RGB DATA: ";
  tReply = "";
  
  esp_camera_fb_return(fb);

  SerialBT.print("#@#"); //Packet end
  SerialBT.flush();
  delay(5000);
}
