//#include <avr/sleep.h>

int volatile Tirs, pinGachette, pinRecharge;
boolean volatile Cote, Centre, ATire;

/* ----- Attention ceci est un test ---- 
int wakePin = 2;                 // pin used for waking up
int sleepStatus = 0;             // variable to store a request for sleep
int count = 0;                   // counter
----- Fin du test ----- */


void setup() 
{ 
  Serial.begin(9600);
  
/* ----- Attention ceci est un test ---- 
  pinMode(wakePin, INPUT);
 ----- Fin du test ----- */
  

  pinGachette=7; //Interrupteurs Gachette et Recharge sur ports 7 et 6
  pinRecharge=1;
  Tirs=0;       
  Cote=false;
  Centre=false;
  ATire=false;
  pinMode(8,OUTPUT);  //Trigger du laser en mode sortie
  digitalWrite(8, LOW); //trigger à 0
  attachInterrupt(digitalPinToInterrupt(pinGachette),RoutineDeTir,FALLING); //Gachette et recharge déclanchées à l'appui dse bouttons
  attachInterrupt(digitalPinToInterrupt(pinRecharge),RoutineRecharge,FALLING);

/* ----- Attention ceci est un test ----   
  attachInterrupt(0, sleepNow, LOW); // use interrupt 0 (pin 2) and run function
                                      // wakeUpNow when pin 2 gets LOW
 ----- Fin du test ----- */

}

void loop() 
{
  while(ATire!=true && Tirs!=5){}; //On attend un tir
  delay(75);  //on laisse allumé 75ms
  digitalWrite(8,LOW); //On éteint le laser pour éviter de rester appuyé et tricher, faut pas déconner non plus.
  ATire=false;
}

void RoutineRecharge() //Routine appelée à l'appui du bouton de recharge
{
  if(Tirs==5)   //Si on a bien effectué nos 5 tirs, on réinitialise à 0 tirs pour recommencer
  {
    Serial.println("Recharge effectuee");
   Tirs=0;
  }
  else        //Sinon on attend que les 5 tirs soient réalisés
  {
    Serial.println("Vous n'avez pas effectue tous vos tirs...");
  }
}

void RoutineDeTir() //Routine appelée à l'appui de la "gachette"
{
  if(Tirs<5) //Si on a encore des munitions on comptabilise un tir et on tire avec le laser (trigger port 8)
  {
    Tirs++;
  digitalWrite(8,HIGH);
  Serial.print("Vous venez de tirer ");  //On affiche le nombre de tirs effectués, histoire de pas devoir compter dans sa tête.
  Serial.print(Tirs);
  Serial.println(" fois "); 
  }
  else
  {
    Serial.println("Vous devez recharger"); // Si on a tiré 5 fois on attend une recharge pour pouvoir continuer
  }
  ATire=true; // On signale qu'on a tiré pour déclancher la suite du programme
}


/* ----- Attention ceci est un test ---- 

void sleepNow()         // here we put the arduino to sleep
{    
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here
  sleep_enable();          // enables the sleep bit in the mcucr register
                             // so sleep is possible. just a safety pin

  attachInterrupt(0,wakeUpNow, LOW); // use interrupt 0 (pin 2) and run function
                                       // wakeUpNow when pin 2 gets LOW
 
  sleep_mode();            // here the device is actually put to sleep!!
                             // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
 
  sleep_disable();         // first thing after waking from sleep:
                             // disable sleep...
  detachInterrupt(0);      // disables interrupt 0 on pin 2 so the
                             // wakeUpNow code will not be executed
                             // during normal running time.
}

void wakeUpNow()
{
}

 ----- Fin du test ----- */

