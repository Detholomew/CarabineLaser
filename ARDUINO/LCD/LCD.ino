#include <LiquidCrystal.h>  /*on importe la bibliothèque*/
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
/* ----- LiquidCrystal nomDeVariable(RS, E, d4, d5, d6, d7); ----- */

void setup() 
{
/* ----- Set up the LCD's number of columns and rows: ----- */
lcd.begin(16, 2);
Serial.begin(9600);
/* ----- Print a message to the LCD ----- */
lcd.print("hello, world!");
}


void loop() 
{
  /* ----- set the cursor to column 0, line 1 ----- */
  /* ----- (note: line 1 is the second row, since counting begins with 0): ----- */
  lcd.setCursor(0, 1);
  /* ----- print the number of seconds since reset: ----- */
  lcd.print(millis() / 1000);
}
