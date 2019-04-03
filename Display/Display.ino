//5V-> VCC; A5 -> SLC; A4 -> SDA; https://www.quinapalus.com/hd44780udg.html

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);
 byte sym[8] = {0x0,0xa,0x1f,0x1f,0xe,0x4,0x0};
 
void setup()
{
 lcd.begin (16,2);
 lcd.createChar(0, sym);
}
 
void loop()
{
  lcd.setBacklight(HIGH);
  lcd.setCursor(0, 0);
  lcd.print("TAIS TE AMO");
  //lcd.scrollDisplayLeft();
  lcd.setCursor(0,1);
  lcd.print((char)0);
  //lcd.print("MUITOOOOOO");
  delay(1000);
  //lcd.setBacklight(LOW);
  delay(1000);
}
