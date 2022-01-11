#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 2, 3, 4, 6);

const int pulsePin = 8; 

unsigned long pulseHigh; 
unsigned long pulseLow; 
unsigned long pulseTotal; 
float frequency; 

void setup()
{
    Serial.begin(9600);
    pinMode(pulsePin,INPUT);
    lcd.begin(16, 2);
lcd.setCursor(0,0);
    lcd.print("Frequency is    ");
    lcd.setCursor(0,1);
    lcd.print("                ");
    delay(100);
}
void loop()
{
    pulseHigh = pulseIn(pulsePin,HIGH);
    pulseLow = pulseIn(pulsePin,LOW);
    
    pulseTotal = pulseHigh + pulseLow; 
    frequency=1000000/pulseTotal; 
    Serial.println(frequency);
    lcd.setCursor(0,1);
    lcd.print(frequency);
    lcd.print(" Hz");
    delay(500);
}
