/******************************************************************************/
/* Inclusions --------------------------------------------------------------- */
#include "LibCHAPAJAS.h"
#include "deplacement.h"
#include "analog.h"
int readPin = A4;
int readPin2 = A5;
int V = 0;
int V2 = 0;
void  analogsetup() {
 pinMode(readPin, INPUT);
 pinMode(readPin2, INPUT);
  Serial.begin(9600);
}

void analogloop() {
  V = analogRead(readPin);
  V2 = analogRead(readPin2);
  //soustraction 5kHz - bruit ambiant
  int V3 = V - V2;
  // Conversion de 0 à 1023 en (0-5V)
  float V4 = V3 * (5.0/1023.0);
  // impression de la valeur
  Serial.println(V4);
}
/*if(V2>=3.0){


}*/