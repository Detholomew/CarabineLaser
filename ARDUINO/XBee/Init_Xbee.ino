/*
  XBEE #2 SENDER
*/

//#include <SoftwareSerial.h>
//SoftwareSerial Xbee(5,6); // RX / TX

const int button = 2; // Boutton pressoir pour envoyer le message 
const int led = 12; // LED pour vérifier le bon fonctionnement du XBee
int buttonState; 
int lastButtonState = LOW;

void setup() {
  Serial.begin(9600); 
  pinMode (led, OUTPUT);
  pinMode (button, INPUT);
  Serial.println("Début initialisation XBee");
 // setDestination();  // On initialise le XBee
  Serial.println("INITIALISATION TERMINEE");
  blink(led, 3); // Si tout va bien la LED clignotte 3 fois
}

void loop() {
    
  buttonState = digitalRead(button); // on lit l'état du Boutton
  if (buttonState != lastButtonState) { // Si changement d'état du boutton
  // on envoie l'état du bouton par le XBee
  //Xbee.println(buttonState, DEC);
  Serial.println(buttonState, DEC);
  lastButtonState = buttonState; // on enregistre l'état du boutton
  blink(led, 2); //et la LED sur notre carte clignote 2 fois
  }
  delay(50);
}

//configuration Xbee 
/*void setDestination() {
  Serial.print("+++");
  char thisByte = 0;
  while (thisByte != '\r') { // On attend la réponse du XBee (bon branchement)
    if (Xbee.available() > 0) {
      thisByte = Xbee.read(); 
    }
  }
  XbeeSerial.print("ATRE\r");
  Xbee.print("ATDL0\r"); // On envoie au XBee 0
  Xbee.print("ATMY1\r"); // On est le XBee 1
  Xbee.print("ATID1111\r");
  Xbee.print("ATCN\r");
}*/

// Blink, fais de clignoter N fois le port Num_Pin
void blink(int Num_Pin, int Nfois) {
  Serial.print("Je clignote ");
  Serial.print( Nfois);
  Serial.print(" fois\n");
  for (int blinks=0; blinks< Nfois; blinks++) {
    digitalWrite(Num_Pin, HIGH);
    delay(200);
    digitalWrite(Num_Pin, LOW);
    delay(200);  
  }
}
