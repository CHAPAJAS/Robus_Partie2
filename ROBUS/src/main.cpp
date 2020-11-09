/******************************************************************************/
/* Inclusions --------------------------------------------------------------- */
#include "LibCHAPAJAS.h"
#include "deplacement.h"
#include "analog.h"

/******************************************************************************/
/* main --------------------------------------------------------------------- */
void setup() {
  // put your setup code here, to run once:
  Deplacement_Init();
  BoardInit();
  analogsetup();
}

void loop() {
  // put your main code here, to run repeatedly:
  // SERVO_Enable(0);
  // SERVO_SetAngle(0,0);
  // delay(3000);
  //  SERVO_SetAngle(0,175);
  // delay(3000);
  // SERVO_Disable(0);
  analogloop();
  delay(50);
  //while (true)
  {
    
  }
  
}