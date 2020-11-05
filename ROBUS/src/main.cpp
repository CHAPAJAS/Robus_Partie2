/******************************************************************************/
/* Inclusions --------------------------------------------------------------- */
#include "LibCHAPAJAS.h"
#include "deplacement.h"
#define PIN 0
#define SONAR 1

//capteur 1 : petit collant
//capteur 2 : gros collant
uint16_t LECTURE_DISTANCE();
float LECTURE_DISTANCE_CM1();
float LECTURE_DISTANCE_CM2();
float DISTANCECM (uint16_t capteur);
float distanceSonar();
float trouverBalle();
void setup() {
  // put your setup code here, to run once:
  BoardInit();
  Deplacement_Init();
}

void loop() {
  
  Serial.println(distanceSonar());
  delay(500);
  Serial.println(distanceSonar());
  delay(500);
  Serial.println(distanceSonar());
  delay(500);
  // put your main code here, to run repeatedly:
 /* float distance=0;
  Serial.print("Distance 1 cm : ");
  Serial.println(LECTURE_DISTANCE_CM1());
  delay(1000);
  Serial.print("Distance 2 cm : ");
  Serial.println(LECTURE_DISTANCE_CM1());
  delay(1000);
  Serial.print("Distance 3 cm : ");
  Serial.println(LECTURE_DISTANCE_CM1());
  delay(1000);
  distance  = trouverBalle();
  Serial.print("Distance balle : ");
  Serial.println(distance);*/

  while(1)
  {}
}

float distanceSonar()
{
  return (SONAR_GetRange(SONAR));
}



uint16_t LECTURE_DISTANCE()
{
  return (ROBUS_ReadIR(PIN));
}


float LECTURE_DISTANCE_CM1()
{
  return(15959*(pow((ROBUS_ReadIR(PIN)),-1.194)));
}
float LECTURE_DISTANCE_CM2()
{
  return(12276*(pow((ROBUS_ReadIR(PIN)),-1.17)));
}
float trouverBalleCap1()
{
  
  int cpt = 0;
  int cptpertetmps = 0;
float distanceBalle = 0;
  float distancetemp =  LECTURE_DISTANCE_CM1();
  //rajouter le tournant
  while (cpt<3)
  {
    if(distancetemp < 60)
    {
      cpt++;
    }
    cptpertetmps++;
    delay(100);
    distancetemp = LECTURE_DISTANCE_CM1();
    if(cptpertetmps > 15)
    {
      distanceBalle = 0;
      break;
    }
  }

  //arreter la rotation 
  delay(500);
  distanceBalle= LECTURE_DISTANCE_CM1();
  //noter l'angle/position du robot
  return distanceBalle;
}
float trouverBalleCap2()
{
  
  int cpt = 0;
  int cptpertetmps = 0;
float distanceBalle = 0;
  float distancetemp =  LECTURE_DISTANCE_CM2();
  //rajouter le tournant
  while (cpt<3)
  {
    if(distancetemp < 60)
    {
      cpt++;
    }
    cptpertetmps++;
    delay(250);
    distancetemp = LECTURE_DISTANCE_CM2();
    if(cptpertetmps > 15)
    {
      distanceBalle = 0;
      break;
    }
  }

  //arreter la rotation 
  delay(500);
  distanceBalle= LECTURE_DISTANCE_CM2();
  //noter l'angle/position du robot
  return distanceBalle;
}