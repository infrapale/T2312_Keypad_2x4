#include "Arduino.h"
//#include "AVR_Watchdog.h"
#include <EEPROM.h>
#include <TaHa.h> 
#include "BtnPinOnOff.h"

#define ROW1  3
#define ROW2  4
#define ROW3  5
#define ROW4  6
#define COL1  A1
#define COL2  A0



TaHa TaHa_10ms;

BtnPinOnOff  Pin1;
BtnPinOnOff  BtnPin[8];


void setup() 
{
  delay(2000);
  Serial.begin(9600); 
  Serial.println("btn_pin_on_off_test.ino  2023");

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
}

void loop() {
    // put your main code here, to run repeatedly:
    TaHa_10ms.run();

    for( uint8_t i= 0; i<8; i++)
    {
        char c = BtnPin[i].Read();
        if (c != 0x00) 
        {
            Serial.print("<KB");
            Serial.print(char(c & 0b01111111));
            Serial.print(":");  
            if ((c & 0b10000000) == 0) 
                Serial.print("1");
            else 
                Serial.print("0");
            Serial.println(">");
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