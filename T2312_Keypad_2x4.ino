#include "Arduino.h"
#include "avr_watchdog.h"
#include <EEPROM.h>
#include <TaHa.h> 
#include "BtnPinOnOff.h"

#define ROW1  3
#define ROW2  4
#define ROW3  5
#define ROW4  6
#define COL1  A1
#define COL2  A0

#define EEPROM_ADDR_KEYPAD_INDEX  0
#define WD_TIMEOUT_SECONDS        30


TaHa TaHa_10ms;
TaHa TaHa_watchdog_timeout;

BtnPinOnOff  Pin1;
BtnPinOnOff  BtnPin[8];
AVR_Watchdog watchdog(4);
uint8_t   unit_index = 1;
uint16_t  wd_timeout_counter;

void setup() 
{
  delay(2000);
  uint8_t u8 = EEPROM.read(EEPROM_ADDR_KEYPAD_INDEX);
  if ((u8>0) && (u8<=4)) unit_index = u8;
  Serial.begin(9600); 
  Serial.println("T2312_Keypad_2x4.ino");
  Serial.print("Compiled:     "); Serial.print(__DATE__); Serial.print(" "); Serial.println(__TIME__);
  Serial.print("Keypad Index: "); Serial.println(unit_index);
  watchdog.set_timeout(4);
  BtnPin[0].Init(ROW1,'1');
  BtnPin[1].Init(ROW2,'2');
  BtnPin[2].Init(ROW3,'3');
  BtnPin[3].Init(ROW4,'4');
  BtnPin[4].Init(ROW1,'5');
  BtnPin[5].Init(ROW2,'6');
  BtnPin[6].Init(ROW3,'7');
  BtnPin[7].Init(ROW4,'8');


  pinMode(COL1,OUTPUT);
  pinMode(COL2,OUTPUT);
  TaHa_10ms.set_interval(10, RUN_RECURRING, run_10ms); 
  TaHa_watchdog_timeout.set_interval(1000, RUN_RECURRING, run_wd_timeout_task); 

}

void loop() {
    // put your main code here, to run repeatedly:
    TaHa_10ms.run();
    TaHa_watchdog_timeout.run();

    for( uint8_t i= 0; i<8; i++)
    {
        char c = BtnPin[i].Read();
        if (c != 0x00) 
        {
            Serial.print("<KP");
            Serial.print(unit_index);
            Serial.print(":");  
            Serial.print(char(c & 0b01111111));
            Serial.print("=");  
            if ((c & 0b10000000) == 0) 
                Serial.print("1");
            else 
                Serial.print("0");
            Serial.println(">");
            wd_timeout_counter = 0;
            
        }
    }
}


void run_10ms()
{
    digitalWrite(COL1,LOW);
    digitalWrite(COL2,HIGH);

    for( uint8_t i= 0; i<4; i++)
    {
        BtnPin[i].Scan();

    }
    digitalWrite(COL1,HIGH);
    digitalWrite(COL2,LOW);

    for( uint8_t i= 4; i<8; i++)
    {
        BtnPin[i].Scan();

    }
}

void run_wd_timeout_task(void)
{
    if (wd_timeout_counter++ < WD_TIMEOUT_SECONDS)
    {
        watchdog.clear();
    }
    else
    {
      Serial.println("Watchdog restart..");
    }
}
