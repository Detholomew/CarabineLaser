void setup() {
  Serial.begin(9600);
  pinMode(13,OUTPUT);
}

void loop() {
  
  if (Serial.available())
  {
      if(char ch = Serial.read())
        Serial.print(ch);
  }

  

}

