/* Stunts
 */
 
#include <PololuLedStrip.h>

// Create an ledStrip object and specify the pin it will use.
PololuLedStrip<10> ledStrip;

// Create a buffer for holding the colors (3 bytes per color).
#define LED_COUNT 252
#define STUNT 3
#define STUNTLEN 2
rgb_color colors[LED_COUNT];
rgb_color stunt[STUNT];
uint16_t stuntloc[STUNT];
int8_t stuntdir[STUNT];

void setup()
{
    pinMode(13, OUTPUT);
    randomSeed(analogRead(0));
    startOneStunt(0);
}

// Converts a color from HSV to RGB.
// h is hue, as a number between 0 and 360.
// s is the saturation, as a number between 0 and 255.
// v is the value, as a number between 0 and 255.
rgb_color hsvToRgb(uint16_t h, uint8_t s, uint8_t v)
{
    uint8_t f = (h % 60) * 255 / 60;
    uint8_t p = v * (255 - s) / 255;
    uint8_t q = v * (255 - f * s / 255) / 255;
    uint8_t t = v * (255 - (255 - f) * s / 255) / 255;
    uint8_t r = 0, g = 0, b = 0;
    switch((h / 60) % 6){
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        case 5: r = v; g = p; b = q; break;
    }
    return (rgb_color){r, g, b};
}

void startOneStunt(uint16_t i){
  stunt[i]=hsvToRgb(random(360),255,255);
  if(random(2)==1) { 
    stuntdir[i]=1; 
    stuntloc[i] = 1;
  } else { 
    stuntdir[i]=-1; 
    stuntloc[i] = LED_COUNT-1;
  }
}

void loop()
{
  for(uint16_t i = 0; i < LED_COUNT; i++)
  {
    colors[i] = (rgb_color){0,0,0};
  }
  for(uint16_t i=0; i<STUNT; i++)
  {
    if(stuntloc[i]==0 && random(0,100)==i)
       {
          startOneStunt(i);
       }
    else if(stuntloc[i]!=0)
       {
          stuntloc[i]=max(0,stuntloc[i]+stuntdir[i])%LED_COUNT;
          for(uint16_t j=max(stuntloc[i]-STUNTLEN,0); j<stuntloc[i]; j++)
          {
              colors[j]=stunt[i];
          }
       }
  }
  // Write the colors to the LED strip.
  ledStrip.write(colors, LED_COUNT);  
  
  delay(12);
}
