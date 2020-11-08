/******************************************************************************/
/* Inclusions --------------------------------------------------------------- */
#include "LibCHAPAJAS.h"
#include "deplacement.h"
#define PIN          0
#define SONAR        1
#define DELAY_VIRAGE 1000
#define CAPTEUR      1        // 1 ou 2
/******************************************************************************/
/* Constantes --------------------------------------------------------------- */
#define ROBUS 'A'

#if(ROBUS == 'A')
#define ENCODEUR_GAUCHE_360 (long)8169
#define ENCODEUR_DROIT_360  (long)7667

#define ANGULOD 0.98
#define ANGULOG 0.955

#define SPD         1.0625        // 1.089
#define SPECIAL_SPD 1.075
#define LONGULO     1.05

#elif(ROBUS == 'B')
#define ENCODEUR_GAUCHE_360 (long)7700
#define ENCODEUR_DROIT_360  (long)7840

// Diminuer ces valeurs pour réduire l'angle parcouru
#define ANGULOD             0.961        // Cette valeur multiplie l'angle droite à parcourir.
#define ANGULOG             0.955        // Cette valeur multiplie l'angle gauche à parcourir.

// Diminuer cette valeur pour aller plus à droite.
#define SPD                 0.972        // Cette valeur multiplie le moteur de droite.
#define SPECIAL_SPD         0.993

// Augmenter cette valeur pour aller plus loin.
#define LONGULO             1.03        // Cette valeur multiplie la distance à parcourir.
#else
#error(Constante `ROBUS` doit être définie à `'A'` ou `'B'`)
#endif

//*******************************************************
// Definitions fonctions :

// capteur 1 : petit collant
// capteur 2 : gros collant
uint16_t LECTURE_DISTANCE();
float    LECTURE_DISTANCE_CM1();
float    LECTURE_DISTANCE_CM2();
float    DISTANCECM(uint16_t capteur);
float    distanceSonar();
float    trouverBalleCap1();
float    trouverBalleCap2();
float    trouverBalleSonar();

void Virage_Gauche(int angle);
void Virage_Droit(int angle);
float Virage_Gauche_Lecture(int angle);


void setup()
{
    // put your setup code here, to run once:
    BoardInit();
    //Deplacement_Init();
}

void loop()
{
    float distance = 0;


    Serial.println(LECTURE_DISTANCE_CM1());
    delay(500);
    Serial.println(LECTURE_DISTANCE_CM1());
    delay(500);
    Serial.println(LECTURE_DISTANCE_CM1());
    delay(500);
    
    
    //Virage_Gauche(45);
    
    //Capteur sonor
    trouverBalleSonar();
    Virage_Droit(90);
    //Rajouter truc des bumpers
    MOTOR_SetSpeed(1,0.75);
    MOTOR_SetSpeed(0,0.75);
    while(!ROBUS_IsBumper(2))//2 pour bumper avant
    {}
    MOTOR_SetSpeed(1,0);
    MOTOR_SetSpeed(0,0);


    //capteur balle
    /*distance = Virage_Gauche_Lecture(90);
    Serial.print("Distance balle : ");
    Serial.println(distance);

    MOTOR_SetSpeed(1,0.2);
    MOTOR_SetSpeed(0,0.2);
    delay(500);
    MOTOR_SetSpeed(1,0);
    MOTOR_SetSpeed(0,0);*/

    while(1)
    {
    }
}

float distanceSonar()        // retourne la valeur en cm
{
    return (SONAR_GetRange(SONAR));
}



uint16_t LECTURE_DISTANCE()
{
    return (ROBUS_ReadIR(PIN));
}


float LECTURE_DISTANCE_CM1()
{
    return (15959 * (pow((ROBUS_ReadIR(PIN)), -1.194)));
}
float LECTURE_DISTANCE_CM2()
{
    return (12276 * (pow((ROBUS_ReadIR(PIN)), -1.17)));
}
float trouverBalleCap1()
{

    int   cpt           = 0;
    int   cptpertetmps  = 0;
    float distanceBalle = 0;
    float distancetemp  = LECTURE_DISTANCE_CM1();
    // rajouter le tournant
    while(cpt < 3)
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

    // arreter la rotation
    delay(500);
    distanceBalle = LECTURE_DISTANCE_CM1();
    // noter l'angle/position du robot
    return distanceBalle;
}
float trouverBalleCap2()
{

    int   cpt           = 0;
    int   cptpertetmps  = 0;
    float distanceBalle = 0;
    float distancetemp  = LECTURE_DISTANCE_CM2();
    // rajouter le tournant
    while(cpt < 3)
    {
        if(distancetemp < 100)
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

    // arreter la rotation
    delay(250);
    distanceBalle = LECTURE_DISTANCE_CM2();
    // noter l'angle/position du robot
    return distanceBalle;
}
float trouverBalleSonar()
{

    int   cpt           = 0;
    int   cptpertetmps  = 0;
    float distanceBalle = 0;
    float distancetemp  = distanceSonar();
    // rajouter le tournant
    while(cpt < 3)
    {
        MOTOR_SetSpeed(1,0.6);
        MOTOR_SetSpeed(0,0.6);

        if(distancetemp < 100)
        {
            cpt++;
        }
        cptpertetmps++;
        delay(100);
        distancetemp = distanceSonar();
        if(cptpertetmps > 15)
        {
            distanceBalle = 0;
            break;
        }
    }
    MOTOR_SetSpeed(1,0);
    MOTOR_SetSpeed(0,0);
    // arreter la rotation
    delay(250);
    distanceBalle = distanceSonar();
    // noter l'angle/position du robot
    return distanceBalle;
}




float Virage_Gauche_Lecture(int angle)
{
    ENCODER_ReadReset(0);
    ENCODER_ReadReset(1);
    int   cpt           = 0;
    int   cptpertetmps  = 0;
    float distanceBalle = 0;
    float distancetemp  = LECTURE_DISTANCE_CM1();

    long valeurEncodeurDroit = ENCODER_Read(1);
    Serial.print("Gauche start\n");
    if(CAPTEUR == 1)
    {
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
    else if(CAPTEUR == 2)
    {
       
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
    }
    // arreter la rotation

    // noter l'angle/position du robot

    
    return distanceBalle;
}
void Virage_Gauche(int angle)
{
    ENCODER_ReadReset(0);
    ENCODER_ReadReset(1);

    long valeurEncodeurDroit = ENCODER_Read(1);

    while(valeurEncodeurDroit <= ENCODEUR_DROIT_360 / (360 / (angle * ANGULOG)))
    {
        MOTOR_SetSpeed(0, -0.3);
        MOTOR_SetSpeed(1, 0.3);
        valeurEncodeurDroit = ENCODER_Read(1);
    }

    MOTOR_SetSpeed(0, 0);
    MOTOR_SetSpeed(1, 0);
}
void Virage_Droit(int angle)
{
    ENCODER_ReadReset(0);
    ENCODER_ReadReset(1);

    long valeurEncodeurGauche = ENCODER_Read(0);

    while(valeurEncodeurGauche <= ENCODEUR_GAUCHE_360 / (360 / (angle * ANGULOD)))
    {
        MOTOR_SetSpeed(0, 0.3);
        MOTOR_SetSpeed(1, -0.3);
        valeurEncodeurGauche = ENCODER_Read(0);
    }

    MOTOR_SetSpeed(0, 0);
    MOTOR_SetSpeed(1, 0);
}