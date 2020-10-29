/******************************************************************************/
/* Inclusions --------------------------------------------------------------- */
#include "LibCHAPAJAS.h"
#include "deplacement.h"


uint16_t Distance=0;
uint16_t LECTURE_DISTANCE(int PIN);
float DISTANCECM (uint16_t capteur);
void setup() {
  // put your setup code here, to run once:
  BoardInit();
  Deplacement_Init();
}

void loop() {
  // put your main code here, to run repeatedly:
  Distance = LECTURE_DISTANCE(0);
  Serial.print("Distance 1 : ");
  Serial.println(Distance);
  Serial.print("cm 1 : ");
  Serial.println(DISTANCECM(Distance));
  delay(1000);
  Distance = LECTURE_DISTANCE(0);
  Serial.print("Distance 2 : ");
  Serial.println(Distance);
  Serial.print("cm 2 : ");
  Serial.println(DISTANCECM(Distance));
  //print("Distance 2 : %u\n",LECTURE_DISTANCE(0) );
  delay(1000);
  Distance = LECTURE_DISTANCE(0);
  Serial.print("Distance 3 : ");
  Serial.println(Distance);
  Serial.print("cm 3 : ");
  Serial.println(DISTANCECM(Distance));
  //print("Distance 3 : %u\n",LECTURE_DISTANCE(0) );
  delay(2000);
  while(1)
  {}
}
uint16_t LECTURE_DISTANCE(int PIN)
{
  return (ROBUS_ReadIR(PIN));
}

float DISTANCECM (uint16_t capteur)
{
  return(15959*(pow(capteur,-1.194)));
}