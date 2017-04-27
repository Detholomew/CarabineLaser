#include "Arduino.h"
#include "string.h" 
#include <LiquidCrystal.h>  /*on importe la bibliothèque pour le LCD*/

LiquidCrystal lcd(12, 11, 10, 9, 6, 5); /* LiquidCrystal nomDeVariable(RS, E, d4, d5, d6, d7); */
void WelcomeUser();

int volatile Tirs, pinGachette, pinRecharge, User, pinUser;
boolean volatile ATire;
int Res_Tir;




void setup() 
{ 
  lcd.begin(16, 2);
  Serial.begin(9600);
  pinGachette=7; /* Interrupteurs Gachette et Recharge sur ports 7 et 6 */
  pinRecharge=2;
  pinUser=3;
  Tirs=0;       
  ATire=false;
  User=1;
  Res_Tir=0;
  pinMode(8,OUTPUT);  /* Trigger du laser en mode sortie */
  digitalWrite(8, LOW); /* trigger à 0 */
  attachInterrupt(digitalPinToInterrupt(pinGachette),RoutineDeTir,FALLING); /* Gachette, recharge et changement d'utilisateur déclanchés à l'appui dse bouttons */
  attachInterrupt(digitalPinToInterrupt(pinRecharge),RoutineRecharge,FALLING);
  attachInterrupt(digitalPinToInterrupt(pinUser),ChangeUser,FALLING);
  Serial.println(User);
  WelcomeUser();
}


void loop() 
{
  while(ATire!=true && Tirs<6){ }; /* On attend un tir */
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 0);
  
  if(Tirs<5) /* Si on a encore des munitions, on tire avec le laser (trigger port 8) */
  {
    Tirs++;
    digitalWrite(8,HIGH);
    delay(100);  /* on laisse allumé 100ms. */
    digitalWrite(8,LOW); /* On éteint le laser pour éviter de rester appuyé et tricher, faut pas déconner non plus.*/
    lcd.print(User);  /* On affiche le num d'utilisateur et le numéro du tir effectué */
    lcd.setCursor(1, 0);
    lcd.print(") ");
    lcd.setCursor(3, 0);
    lcd.print("Tir ");
    lcd.setCursor(9, 0);
    lcd.print(Tirs);
    if(Serial.available() > 0) /* On récupère la valeur du tir et on affiche le résultat */
    {
      Res_Tir=Serial.read(); 
    }
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    if(Res_Tir>0)
    {
      lcd.print("Touche a");
      lcd.setCursor(0, 9);
      lcd.print(Res_Tir);
      lcd.setCursor(12, 0);    
      lcd.print("%");
    }
    else
    {
      lcd.print("Echec!");    
    }
    Res_Tir=0;
  }
  else
  {
    lcd.print(User);  /* On affiche le nombre de tirs effectués, histoire de pas devoir compter dans sa tête. */
    lcd.setCursor(1, 0);
    lcd.print(") ");
    lcd.setCursor(3, 0);
    lcd.print("Rechargez");
    lcd.setCursor(0,1);
    lcd.print("                ");
  }
  
  delay(300); /* On attend un peu pour laisser le temps à la cible de faire l'acquisition */
  ATire=false; /* On se remet en attente d'un tir. */
  Serial.println(Tirs);
}






void RoutineRecharge() /* Routine appelée à l'appui du bouton de recharge */
{
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 0);
  if(Tirs==5)   /* Si on a bien effectué nos 5 tirs, on réinitialise à 0 tirs pour recommencer */
  {
    lcd.print("Recharge faite");
    Tirs=0;
  }
  else        /*Sinon on attend que les 5 tirs soient réalisés */
  {
    lcd.print("balles restantes");
  }
  lcd.setCursor(0,1);
  lcd.print("Tir possible !");
}






void RoutineDeTir() /*Routine appelée à l'appui de la "gachette" */
{
  ATire=true; /* On signale qu'on a tiré pour déclancher la suite du programme */
}





void ChangeUser() /* A l'appui du boutton on change d'utilisateur (9 maximum) et on envoie le nouvel utilisateur à la cible et à l'écran */
{
  if(User<9)
  {
    User++;  
  }
  else
  {
    User=1;
  }
  Serial.println(User);
  WelcomeUser();
  Tirs=0;
}





void WelcomeUser()
{
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,0);
  lcd.print("Welcome user ");
  lcd.setCursor(13, 0);
  lcd.print(User);
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("Tir possible!");
  delay(300);
}
