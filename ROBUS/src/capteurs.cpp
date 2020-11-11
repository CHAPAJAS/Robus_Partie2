/*****************************************************************************/
/* Inclusions -------------------------------------------------------------- */
#include "capteurs.h"
#include <Wire.h>
#include <string.h>


/*****************************************************************************/
/* Définitions ------------------------------------------------------------- */
#define MARGE 3

#define PIN_RED    8
#define PIN_BLUE   9
#define PIN_YELLOW 10


/*****************************************************************************/
/* Variables --------------------------------------------------------------- */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_154MS, TCS34725_GAIN_1X);


/*****************************************************************************/
/* Déclarations de fonctions ----------------------------------------------- */
void AffichageCouleur(int couleur);
bool DetecterRouge(struct RGB couleur);
bool DetecterBleu(struct RGB couleur);
bool DetecterJaune(struct RGB couleur);
bool DetecterVert(struct RGB couleur);

// void imprimerRGB();


/*****************************************************************************/
/* Définitions de fonctions ------------------------------------------------ */
/* Example code for the Adafruit TCS34725 breakout library */

/* Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 3.3V DC
   Connect GROUND to common ground */

/* Initialise with default values (int time = 2.4ms, gain = 1x) */
// Adafruit_TCS34725 tcs = Adafruit_TCS34725();

/* Initialise with specific int time and gain values */
// Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

void capteurCouleur_Init()
{
    pinMode(PIN_RED, OUTPUT);
    pinMode(PIN_BLUE, OUTPUT);
    pinMode(PIN_YELLOW, OUTPUT);
    digitalWrite(PIN_RED, HIGH);    // La LED est éteinte à l'état HIGH
    digitalWrite(PIN_BLUE, HIGH);
    digitalWrite(PIN_YELLOW, HIGH);
}

int RoutineCouleur()
{
    struct RGB  couleur;
    static char nomCouleur[100] = "";

    saisirRGB(&tcs, &couleur);
    detecterCouleur(couleur, nomCouleur);
    print("%s\n", nomCouleur);

    if(strcmp(nomCouleur, "Rouge") == 0)
    {
        AffichageCouleur(ROUGE);
        return ROUGE;
    }
    else if(strcmp(nomCouleur, "Jaune") == 0)
    {
        AffichageCouleur(JAUNE);
        return JAUNE;
    }
    else if(strcmp(nomCouleur, "Bleu") == 0)
    {
        AffichageCouleur(BLEU);
        return BLEU;
    }
    else
    {
        return COULEUR_INCONNUE;
    }
    
}

void saisirRGB(Adafruit_TCS34725* tcs, struct RGB* rawCouleur)
{
    struct RGB couleurTemp;
    uint16_t   r, g, b, c;    // colorTemp, lux;

    tcs->getRawData(&r, &g, &b, &c);
    // colorTemp = tcs.calculateColorTemperature(r, g, b);
    // lux = tcs.calculateLux(r, g, b);

    // Conversion de raw Data en code HEX et affichage en DEC
    uint32_t sum    = c;
    couleurTemp.red = r;
    couleurTemp.red /= sum;

    couleurTemp.green = g;
    couleurTemp.green /= sum;

    couleurTemp.blue = b;
    couleurTemp.blue /= sum;

    couleurTemp.red *= 256;
    couleurTemp.green *= 256;
    couleurTemp.blue *= 256;

    // Serial.print("\t");
    // Serial.print((int)couleurTemp.red, DEC);
    // Serial.print((int)couleurTemp.green, DEC);
    // Serial.print((int)couleurTemp.blue, DEC);
    // Serial.print("\n");

    rawCouleur->red   = couleurTemp.red;
    rawCouleur->green = couleurTemp.green;
    rawCouleur->blue  = couleurTemp.blue;
}

void detecterCouleur(struct RGB couleur, char* couleurDetecte)
{
    if(DetecterRouge(couleur) == true)
    {

        strcpy(couleurDetecte, "Rouge");
    }
    else if(DetecterBleu(couleur) == true)
    {

        strcpy(couleurDetecte, "Bleu");
    }
    else if(DetecterJaune(couleur) == true)
    {

        strcpy(couleurDetecte, "Jaune");
    }
    else
    {
        strcpy(couleurDetecte, "Not found!");
    }

    // Serial.println(couleurDetecte);
}
int detecterCouleur(struct RGB couleur)
{
    if(DetecterRouge(couleur) == true)
    {
        return ROUGE;
    }
    else if(DetecterBleu(couleur) == true)
    {
        return BLEU;
    }
    else if(DetecterJaune(couleur) == true)
    {
        return JAUNE;
    }
    else if(DetecterVert(couleur) == true)
    {
        return VERT;
    }
    else
    {
        return -1;
    }
}

bool DetecterRouge(struct RGB couleur)
{
    // Rouge  928069
    if((couleur.red <= 92 + MARGE && couleur.red >= 92 - MARGE)
       && (couleur.green >= 80 - MARGE && couleur.green <= 80 + MARGE)
       && (couleur.blue >= 69 - MARGE && couleur.blue <= 69 + MARGE))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool DetecterBleu(struct RGB couleur)
{
    // Blue  699576
    if((couleur.red <= 69 + MARGE && couleur.red >= 69 - MARGE)
       && (couleur.green >= 95 - MARGE && couleur.green <= 95 + MARGE)
       && (couleur.blue >= 76 - MARGE && couleur.blue <= 76 + MARGE))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool DetecterJaune(struct RGB couleur)
{
    // Jaune  919553
    if((couleur.red <= 91 + MARGE && couleur.red >= 91 - MARGE)
       && (couleur.green >= 95 - MARGE && couleur.green <= 95 + MARGE)
       && (couleur.blue >= 53 - MARGE && couleur.blue <= 53 + MARGE))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool DetecterVert(struct RGB couleur)
{
    // Vert à déterminer
    return false;
}

/**
 * @brief   Allume la led correspondante à la couleur détectée
 */
void AffichageCouleur(int couleur)
{
    switch(couleur)
    {
        case ROUGE:
            digitalWrite(PIN_RED, LOW);
            return;

        case BLEU:
            digitalWrite(PIN_BLUE, LOW);
            return;

        case JAUNE:
            digitalWrite(PIN_YELLOW, LOW);
            return;

        default:
            // BIIIP();

            break;
    }
}

/*struct RGB color_converter(int hexValue)
{
 struct RGB rgbColor;
 rgbColor.r = ((hexValue >> 16) & 0xFF) / 255.0; // Extract the RR byte
 rgbColor.g = ((hexValue >> 8) & 0xFF) / 255.0; // Extract the GG byte
 rgbColor.b = ((hexValue) & 0xFF) / 255.0; // Extract the BB byte
 return (rgbColor);
}*/