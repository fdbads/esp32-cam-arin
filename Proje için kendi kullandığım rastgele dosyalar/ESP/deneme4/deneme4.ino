#define CAMERA_MODEL_AI_THINKER
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

void setup() {

  //camera flash once
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);delay(200);
  digitalWrite(4, HIGH);delay(40);digitalWrite(4, LOW);
  
  
  SerialBT.begin("ESP32 Bluetooth");
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
  config.pixel_format = PIXFORMAT_JPEG;
  config.fb_count = 1;
  config.frame_size = FRAMESIZE_QVGA;
  config.fb_location = CAMERA_FB_IN_PSRAM;

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x \n", err);
    return;
  }
  
}

void loop() {
  Serial.print("-START OF LOOP-\n");
  camera_fb_t * fb = NULL;
  
  Serial.println("8-bit Addressable");
  Serial.println(heap_caps_get_total_size(MALLOC_CAP_8BIT));
  Serial.println(heap_caps_get_free_size(MALLOC_CAP_8BIT));
  Serial.println(heap_caps_get_largest_free_block(MALLOC_CAP_8BIT));
  Serial.println("Internal");
  Serial.println(heap_caps_get_total_size(MALLOC_CAP_INTERNAL));
  Serial.println(heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
  Serial.println(heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL));
  Serial.println("SPI");
  Serial.println(heap_caps_get_total_size(MALLOC_CAP_SPIRAM ));
  Serial.println(heap_caps_get_free_size(MALLOC_CAP_SPIRAM ));
  Serial.println(heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM ));

  Serial.println("sizeof camera_fb_t");
  Serial.println(sizeof(camera_fb_t));
  

  
  
  
  
  
  
  fb = esp_camera_fb_get();

  
  if (!fb) Serial.print("-Error capturing image from camera-\n");
  if (fb==NULL) Serial.print("-FB NULL-\n");
  
  
  /*
  // allocate memory to store rgb data in psram
  void *ptrVal = NULL;
  uint32_t ARRAY_LENGTH = fb->width * fb->height * 3; // number of pixels in the jpg image x 3
  
  if (heap_caps_get_free_size( MALLOC_CAP_SPIRAM) <  ARRAY_LENGTH) Serial.printf(" -Error not enough free psram to store rgb data- ");      // check free memory in psram
  ptrVal = heap_caps_malloc(ARRAY_LENGTH, MALLOC_CAP_SPIRAM);  
  uint8_t *rgb = (uint8_t *)ptrVal;

  heap_caps_free(ptrVal);
  */
  esp_camera_fb_return(fb);
  
  SerialBT.print("@#@"); //Packet start
  SerialBT.write(0xff);
  
  SerialBT.print("#@#"); //Packet end
  SerialBT.flush();
  delay(1000);

}
