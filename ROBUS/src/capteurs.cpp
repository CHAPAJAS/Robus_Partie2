#include "capteurs.h"
#include "Adafruit_TCS34725.h"
#include <Wire.h>
#include <string.h>

#define MARGE 3


/*-----Initialiser les fonctions-------------*/
// void imprimerRGB();


/* Example code for the Adafruit TCS34725 breakout library */

/* Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 3.3V DC
   Connect GROUND to common ground */

/* Initialise with default values (int time = 2.4ms, gain = 1x) */
// Adafruit_TCS34725 tcs = Adafruit_TCS34725();

/* Initialise with specific int time and gain values */
// Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

void capteurCouleur_Init(Adafruit_TCS34725 tcs)
{

    //Serial.begin(9600);

    // Sert à détercter un capteur.
    if(tcs.begin())
    {
        Serial.println("Found sensor");
    }
    else
    {
        Serial.println("No TCS34725 found ... check your connections");
        while(1)
            ;
    }

    // Now we're ready to get readings!
}

void saisirRGB(Adafruit_TCS34725 tcs, struct RGB* rawCouleur)
{        //

    struct RGB couleurTemp;
    uint16_t   r, g, b, c;        // colorTemp, lux;

    tcs.getRawData(&r, &g, &b, &c);
    // colorTemp = tcs.calculateColorTemperature(r, g, b);
    // lux = tcs.calculateLux(r, g, b);

    // Conversion de raw Data en code HEX et affichage en DEC
    uint32_t sum = c;
    couleurTemp.red = r;
    couleurTemp.red /= sum;

    couleurTemp.green = g;
    couleurTemp.green /= sum;

    couleurTemp.blue = b;
    couleurTemp.blue /= sum;

    couleurTemp.red *= 256;
    couleurTemp.green *= 256;
    couleurTemp.blue *= 256;

    Serial.print("\t");
    Serial.print((int)couleurTemp.red, DEC);
    Serial.print((int)couleurTemp.green, DEC);
    Serial.print((int)couleurTemp.blue, DEC);
    Serial.print("\n");

    rawCouleur->red   = couleurTemp.red;
    rawCouleur->green = couleurTemp.green;
    rawCouleur->blue  = couleurTemp.blue;
}

void detecterCouleur(struct RGB couleur, char* couleurDetecte)
{

    if((couleur.red <= 92 + MARGE && couleur.red >= 92 - MARGE) && (couleur.green >= 80 - MARGE && couleur.green <= 80 + MARGE)
       && (couleur.blue >= 69 - MARGE && couleur.blue <= 69 + MARGE))
    {
      // Rouge  928069
        strcpy(couleurDetecte, ROUGE);
    }else if((couleur.red <= 69 + MARGE && couleur.red >= 69 - MARGE) && (couleur.green >= 95 - MARGE 
       && couleur.green <= 95 + MARGE) && (couleur.blue >= 76 - MARGE && couleur.blue <= 76 + MARGE))
    {
      // Blue  699576
        strcpy(couleurDetecte, BLUE);
    }else if((couleur.red <= 91 + MARGE && couleur.red >= 91 - MARGE) && (couleur.green >= 95 - MARGE && couleur.green <= 95 + MARGE)
         && (couleur.blue >= 53 - MARGE && couleur.blue <= 53 + MARGE))
    {
      // Jaune  919553
        strcpy(couleurDetecte, JAUNE);
    }else
    {
        strcpy(couleurDetecte, "Not found!");
    }

    //Serial.println(couleurDetecte);

}

/*struct RGB color_converter(int hexValue)
{
 struct RGB rgbColor;
 rgbColor.r = ((hexValue >> 16) & 0xFF) / 255.0; // Extract the RR byte
 rgbColor.g = ((hexValue >> 8) & 0xFF) / 255.0; // Extract the GG byte
 rgbColor.b = ((hexValue) & 0xFF) / 255.0; // Extract the BB byte
 return (rgbColor);
}*/
