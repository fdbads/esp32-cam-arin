unsigned char Rx_byte = 0x00;
const unsigned char Tx_byte = 0x00;


void setup() {
    
    Serial.begin(115200);

}

void loop() {
    //delay(1000);
    
    if (Serial.available()){
        
        Rx_byte = Serial.read();
        delay(50);
        Serial.write(Rx_byte);
        

    }

}