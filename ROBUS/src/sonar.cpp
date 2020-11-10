#include "sonar.h"
#include "LibCHAPAJAS.h"


#define PIN          0
#define SONAR        1
#define ENCODEUR_DROIT_360  (long)7667
#define ANGULOG 0.955



float distanceSonar()        // retourne la valeur en cm
{
    return (SONAR_GetRange(SONAR));
}


float LECTURE_DISTANCE_CM2()
{
    return (12276 * (pow((ROBUS_ReadIR(PIN)), -1.17)));
}

float Virage_Gauche_Lecture(int angle)
{
    ENCODER_ReadReset(0);
    ENCODER_ReadReset(1);
    int   cpt           = 0;
    int   cptpertetmps  = 0;
    float distanceBalle = 0;
    float distancetemp  = LECTURE_DISTANCE_CM2();

    long valeurEncodeurDroit = ENCODER_Read(1);
  
       
        while(valeurEncodeurDroit <= ENCODEUR_DROIT_360 / (360 / (angle * ANGULOG)) && cpt < 2)
        {
            MOTOR_SetSpeed(0, -0.3);
        MOTOR_SetSpeed(1, 0.3);
            valeurEncodeurDroit = ENCODER_Read(0);

            if(distancetemp < 60)
            {
                cpt++;
            }
            cptpertetmps++;
            delay(100);
            distancetemp = LECTURE_DISTANCE_CM2();
            if(cptpertetmps > 15)
            {
                distanceBalle = 0;
                break;
            }
        }
        MOTOR_SetSpeed(0, 0);
        MOTOR_SetSpeed(1, 0);
        delay(250);
        if( LECTURE_DISTANCE_CM2()<60)
        {
            distanceBalle = LECTURE_DISTANCE_CM2();
        }
        else
        {
            distanceBalle = 0;
        }
    
    return distanceBalle;
     /* Serial.print("Gauche start\n");
    
        Serial.print("Gauche pre while\n");
        while(valeurEncodeurDroit <= ENCODEUR_DROIT_360*1 / (360 / (angle * ANGULOG)) && cpt < 2)
        {
            Serial.print("Gauche post while\n");
            MOTOR_SetSpeed(0, -0.15);
            MOTOR_SetSpeed(1, 0.15);
            distancetemp = LECTURE_DISTANCE_CM1();
            if(distancetemp < 60)
            {
                cpt++;
            }
            
            valeurEncodeurDroit = ENCODER_Read(1);
        }
        MOTOR_SetSpeed(0, 0);
        MOTOR_SetSpeed(1, 0);
        delay(100);
        
        if(LECTURE_DISTANCE_CM1()<60)
        {
            distanceBalle = LECTURE_DISTANCE_CM1();
        }
        else
        {
            distanceBalle = 0;
        }
    }
    */
}
