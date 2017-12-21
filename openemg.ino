
#include "TM1637.h"
#define CLK 8;
#define DIO 7
TM1637 tm1637(CLK,DIO);

#define BUZ 12
#define SONDA A5

void setup()
{
  tm1637.init();
  tm1637.set(BRIGHT_TYPICAL);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;

  Serial.begin(9600);
  
  pinMode(BUZ, OUTPUT);
  pinMode(SONDA, INPUT);
}

int16_t pomiary[10];
int16_t suma, np1, np2;

void loop()
{
  int8_t NumTab[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};//0~9,A,b,C,d,E,F
  int8_t ListDisp[4];
  unsigned char i = 0;
  unsigned char count = 0;
  tone(BUZ, 1000);
  delay(150);
  noTone(BUZ);
  while(1)
  {
    for (int i = 0; i < 10; i++) {
      pomiary[i] = analogRead(SONDA);
      Serial.println(pomiary[i]);
      delay(100);
    }

    np1 = 0;
    np2 = 0;
    for (int i = 0; i < 10; i++) {
      if (pomiary[i] > np1) {
        np2 = np1;
        np1 = pomiary[i];
      } else if (pomiary[i] > np2)
        np2 = pomiary[i];
    }
    
    suma = -np1 - np2;
    np1 = 1024;
    np2 = 1024;
    for (int i = 0; i < 10; i++) {
      if (pomiary[i] < np1) {
        np2 = np1;
        np1 = pomiary[i];
      } else if (pomiary[i] < np2)
        np2 = pomiary[i];
      suma += pomiary[i];
    }
    suma -= np1 + np2;
    
    Serial.print("== ");
    Serial.print(suma);
    
    tm1637.display(0, (suma / 1000) % 10);
    tm1637.display(1, (suma / 100) % 10); 
    tm1637.display(2, (suma / 10) % 10);
    tm1637.display(3, suma % 10);
  }
}
