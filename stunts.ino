/* LedStripRainbow: Example Arduino sketch that shows
 * how to make a moving rainbow pattern on an
 * Addressable RGB LED Strip from Pololu.
 *
 * To use this, you will need to plug an Addressable RGB LED
 * strip from Pololu into pin 12.  After uploading the sketch,
 * you should see a moving rainbow.
 */
 
#include <PololuLedStrip.h>

// Create an ledStrip object and specify the pin it will use.
PololuLedStrip<12> ledStrip;

// Create a buffer for holding the colors (3 bytes per color).
#define LED_COUNT 60
#define STUNT 3
rgb_color colors[LED_COUNT];
rgb_color stunt[STUNT];
uint16_t stuntloc[STUNT];
int8_t stuntdir[STUNT];

void setup()
{
    pinMode(13, OUTPUT);
    randomSeed(analogRead(0));
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

void loop()
{
  for(uint16_t i=0; i<STUNT; i++)
  {
    if(stuntloc[i]==0 && random(0,100)==i)
       {
          if (i==0) { digitalWrite(13, HIGH); }   // turn the LED on (HIGH is the voltage level)
          stunt[i]=hsvToRgb(random(256),255,255);
          stuntdir[i]=random(0,1)*2-1;
          stuntloc[i]+=1;
       }
     else if(stuntloc[i]!=0)
       {
          stuntloc[i]=(stuntloc[i]+1)%LED_COUNT;
       }
  }
  // Update the colors.
  uint16_t time = millis() >> 2;
  for(uint16_t i = 0; i < LED_COUNT; i++)
  {
    byte x = (time >> 2) - (i << 3);
    colors[i] = hsvToRgb((uint32_t)x * 359 / 256, 255, 255);
  }
  
  // Write the colors to the LED strip.
  ledStrip.write(colors, LED_COUNT);  
  
  delay(10);
}
