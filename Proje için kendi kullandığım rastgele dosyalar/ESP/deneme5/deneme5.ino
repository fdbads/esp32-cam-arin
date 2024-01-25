#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

#define imHeight 24
#define imWidth 32
#define im_arr_len imHeight*imWidth*3

unsigned char *im_arr;

unsigned char rx = 0;

void mem_monitor(){

  Serial.println("-8-bit Addressable");
  Serial.println(heap_caps_get_total_size(MALLOC_CAP_8BIT));
  Serial.println(heap_caps_get_free_size(MALLOC_CAP_8BIT));
  Serial.println(heap_caps_get_largest_free_block(MALLOC_CAP_8BIT));
  Serial.println("-Internal");
  Serial.println(heap_caps_get_total_size(MALLOC_CAP_INTERNAL));
  Serial.println(heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
  Serial.println(heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL));
  Serial.println("-SPI");
  Serial.println(heap_caps_get_total_size(MALLOC_CAP_SPIRAM ));
  Serial.println(heap_caps_get_free_size(MALLOC_CAP_SPIRAM ));
  Serial.println(heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM ));
  
}

void setup() {

  //camera flash once
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);delay(200);
  digitalWrite(4, HIGH);delay(30);digitalWrite(4, LOW);
  
  
  SerialBT.begin("ESP32 Bluetooth");
  Serial.begin(115200);
  
  im_arr = (unsigned char *)malloc(im_arr_len * sizeof(unsigned char));
  // Initialize the array elements
  for (int i = 0; i < im_arr_len; i++) {
      im_arr[i] = 1; // You can use any initialization value here
  }

  
  // go through whole array
  for (int i = 0; i < im_arr_len; i++) {
      im_arr[i] = 0;
      
  }

  
  

}

void loop() {
  Serial.print("\n\n-START OF LOOP-\n");  
  //mem_monitor();
  if(SerialBT.available())rx = SerialBT.read();
  
  
  SerialBT.print("@#@"); //Packet start
  

  // go through whole array
  for (int i = 0; i < im_arr_len; i++) {
      if      (i%3 == 0) im_arr[i] = rx; // R channel
      else if (i%3 == 1) im_arr[i] = rx; // G channel
      else if (i%3 == 2) im_arr[i] = rx; // B channel
  }
  
  // go through whole array
  printf("\n");
  for (int i = 0; i < im_arr_len; i++) {
      SerialBT.write(im_arr[i]);

      /*
      if (i%3 == 0){
        Serial.print("\n");
        Serial.print(i);
      }
      Serial.print(" ");
      Serial.println(im_arr[i]);
      */
  }
  
  
  SerialBT.print("#@#"); //Packet end
  SerialBT.flush();
  Serial.flush();
  //delay();

}
