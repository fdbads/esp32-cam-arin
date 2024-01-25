int i = 0;
void setup() {
    // put your setup code here, to run once:
    //Serial.begin(115200);
    pinMode(LED_BUILTIN,OUTPUT);
    pinMode(12,INPUT_PULLUP);
    pinMode(0,OUTPUT);
    digitalWrite(0,LOW);

    digitalWrite(LED_BUILTIN,HIGH);delay(500);
    digitalWrite(LED_BUILTIN,LOW);delay(500);
    digitalWrite(LED_BUILTIN,HIGH);delay(500);
    digitalWrite(LED_BUILTIN,LOW);delay(500);
    
    digitalWrite(LED_BUILTIN,LOW);
    
    
}

void loop() {
    if (digitalRead(12) == LOW){
      digitalWrite(LED_BUILTIN,HIGH);
      digitalWrite(0,HIGH);
    }
    delay(100);
}
