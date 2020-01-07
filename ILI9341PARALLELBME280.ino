#include <SPFD5408_Adafruit_GFX.h>    
#include <SPFD5408_Adafruit_TFTLCD.h> 
#include <SPFD5408_TouchScreen.h>  
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <SPI.h>

Adafruit_BME280 bme;

#define bme_ADDRESS 0x76

// *** Define Touchscreen Pin
#define YP A1
#define XM A2
#define YM 5
#define XP 6

// *** Define Value of Touchscreen input
#define TS_MINX 125
#define TS_MINY 85
#define TS_MAXX 965
#define TS_MAXY 905
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// *** Define Pin of LCD used
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
//#define LCD_RESET A4

// *** Define Name of Color
#define BLACK   0x0000
#define WHITE   0xFFFF
#define RED     0xF800
#define GREEN   0x07E0
#define BLUE    0x001F
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define GREY    0x2108

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, 1);

#define MINPRESSURE 10
#define MAXPRESSURE 1000

// Meter colour schemes
#define RED2RED 0
#define GREEN2GREEN 1
#define BLUE2BLUE 2
#define BLUE2RED 3
#define GREEN2RED 4
#define RED2GREEN 5

uint32_t runTime = -99999;       // time for next update
int reading = 0; // Value to be displayed
int d = 0; // Variable used for the sinewave test waveform
boolean alert = 0;
int8_t ramp = 1;
int tesmod = 0;

float ah; //absolute humidity = water vapor density in g/m*3
float rh; // relative humidity in %
float t; //temperature in celsius
float td; //dew point in celsius
float tf; //temperature in fahrenheit
float tdf; // dew point temperature in fahrenheit

double dewPoint(double celsius, double humidity){
double RATIO = 373.15 / (273.15 + celsius);
double RHS = -7.90298 * (RATIO - 1);
RHS += 5.02808 * log10(RATIO);
RHS += -1.3816e-7 * (pow(10, (11.344 * (1 - 1 / RATIO ))) - 1) ;
RHS += 8.1328e-3 * (pow(10, (-3.49149 * (RATIO - 1))) - 1) ;
RHS += log10(1013.246);
double VP = pow(10, RHS - 3) * humidity;
double T = log(VP / 0.61078); // temp var
return (241.88 * T) / (17.558 - T);
}

  void setup() 
  {
  Serial.begin(9600);
  Serial.println("Reading sensors..."); 
  randomSeed(analogRead(5));
  tft.reset(); 
  bme.begin(0x76);
  tft.begin(0x9325);  //Or replace it with 0x9325, 0x9228, 0x9230
  tft.setRotation(1); // 
  tft.fillScreen(GREY); 
  tft.setCursor (240, 57);
  tft.setTextSize (2);
  tft.setTextColor (WHITE, GREY);
  tft.print ("TEMP C");
  tft.setCursor (250, 5);
  tft.setTextSize (2);
  tft.setTextColor (WHITE,GREY);
  tft.print ("HUM");
  
  //Design Interface (lines)
  tft.fillRect(0, 197, 217, 4, BLUE);
  tft.fillRect(217, 98, 320, 4, BLUE);
  tft.fillRect(217, 148, 320, 4, BLUE);
  tft.fillRect(217, 0, 4, 240, BLUE);
  tft.fillRect(221, 50, 320, 4, BLUE);
  tft.fillRect(221, 197, 320, 4, BLUE);
}

  void loop() {





  float h = bme.readHumidity();
  float t = bme.readTemperature()-2;
  float p = bme.readPressure();
  

  rh = bme.readHumidity();
  
  ah = (6.112 * pow(2.71828, ((17.67 * t) / (243.5 + t))) * rh * 2.1674) / (273.15 + t);

  
  {
  
  tft.setCursor(234, 78);
  tft.setTextSize (2);
  tft.setTextColor ( RED , GREY);
  tft.print (t,2);

  
  
  tft.setCursor(229, 105);
  tft.setTextSize (2);
  tft.setTextColor ( BLUE , GREY);
  tft.print ("Abs.Hum");
  tesmod=1;
    
  tft.setCursor(230, 128);
  tft.setTextSize (2);
  tft.setTextColor ( YELLOW , GREY);
  tft.print (ah);
  tesmod=1;
  
  tft.setCursor(278, 128);
  tft.setTextSize (2);
  tft.setTextColor ( YELLOW , GREY);
  tft.print ("G");

  tft.setCursor(292, 130);
  tft.setTextSize (1);
  tft.setTextColor ( YELLOW , GREY);
  tft.print ("/m3");

    tft.setCursor (225,160);
    tft.setTextSize (1);
    tft.setTextColor (WHITE,GREY);
    tft.print ("Portable");

    tft.setCursor (245,173);
    tft.setTextSize (1);
    tft.setTextColor (WHITE,GREY);
    tft.print ("Weather");

    tft.setCursor (270,185);
    tft.setTextSize (1);
    tft.setTextColor (WHITE,GREY);
    tft.print ("Station");

    tft.setCursor (245,205);
    tft.setTextSize (1);
    tft.setTextColor (GREEN,GREY);
    tft.print ("Dew point");

    tft.setCursor (242,222);
    tft.setTextSize (2);
    tft.setTextColor (GREEN,GREY);
    tft.print (dewPoint(t, h));

    tft.setCursor (290,222);
    tft.setTextSize (2);
    tft.setTextColor (GREEN,GREY);
    tft.print ("C");

    
    
    tft.setCursor (5,214);
    tft.setTextSize (2);
    tft.setTextColor (WHITE,GREY);
    tft.print ("Press:");

    tft.setTextSize (2);
    tft.setTextColor (WHITE,GREY);
    tft.print (bme.readPressure() / 98.5);

    tft.setTextSize (2);
    tft.setTextColor (WHITE,GREY);
    tft.print (" hpa");

    
  
    }
    
  if (millis() - runTime >= 500) { // Execute every 500ms
    runTime = millis();
    if (tesmod == 0) {
      reading = 40;
    }
    if (tesmod == 1) {
      reading = t ;
    }
    int xpos = 0, ypos = 5, gap = 4, radius = 52;
    // Draw a large meter
    xpos = 320 / 2 - 160, ypos = 0, gap = 100, radius = 100;
    ringMeter(reading, 1, 99, xpos, ypos, radius, "Celsius", GREEN2RED); 
    if (h > 0) { //Humidity %
      tft.setCursor (226, 25); //157,208
      tft.setTextSize (3);
      tft.setTextColor (BLUE, GREY);
      tft.print (h, 1); tft.print ('%');
      tesmod = 1;
    }
  }
}

int ringMeter(int value, int vmin, int vmax, int x, int y, int r, char *units, byte scheme)
{
  
  x += r; y += r;   
  int w = r / 3;    
  int angle = 150;  
  int v = map(value, vmin, vmax, -angle, angle); 
  byte seg = 3; 
  byte inc = 6; 
  
  int colour = GREY;
  
  for (int i = -angle + inc / 2; i < angle - inc / 2; i += inc) {
    
    float sx = cos((i - 90) * 0.0174532925);
    float sy = sin((i - 90) * 0.0174532925);
    uint16_t x0 = sx * (r - w) + x;
    uint16_t y0 = sy * (r - w) + y;
    uint16_t x1 = sx * r + x;
    uint16_t y1 = sy * r + y;
    // Calculate pair of coordinates for segment end
    float sx2 = cos((i + seg - 90) * 0.0174532925);
    float sy2 = sin((i + seg - 90) * 0.0174532925);
    int x2 = sx2 * (r - w) + x;
    int y2 = sy2 * (r - w) + y;
    int x3 = sx2 * r + x;
    int y3 = sy2 * r + y;
    if (i < v) { // Fill in coloured segments with 2 triangles
      switch (scheme) {
        case 0: colour = RED; break; // Fixed colour
        case 1: colour = GREEN; break; // Fixed colour
        case 2: colour = BLUE; break; // Fixed colour
        case 3: colour = rainbow(map(i, -angle, angle, 0, 127)); break; // Full spectrum blue to red
        case 4: colour = rainbow(map(i, -angle, angle, 70, 127)); break; // Green to red (high temperature etc)
        case 5: colour = rainbow(map(i, -angle, angle, 127, 63)); break; // Red to green (low battery etc)
        default: colour = BLUE; break; // Fixed colour
      }
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, colour);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, colour);
      //text_colour = colour; // Save the last colour drawn
    }
    else // Fill in blank segments
    {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, GREY);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, GREY);
    }
  }
  // Convert value to a string
  char buf[10];
  byte len = 0;  if (value > 999) len = 5;
  dtostrf(value, 1, len, buf);
    //buf[len] = 1; 
  //buf[len] = 2; // Add blanking space and terminator, helps to centre text too!
  // Set the text colour to default
  if (value > 7) {
    tft.setTextColor(colour, GREY);
    tft.setCursor(x - 30, y - 40); tft.setTextSize(6);
    tft.print(buf);
  }
  if (value < 10) {
    tft.setTextColor(BLUE, GREY);
    tft.setCursor(x - 30, y - 40); tft.setTextSize(6);
    tft.print(buf);
  }
  tft.setTextColor(GREEN, GREY);
  tft.setCursor(x - 39, y + 75); tft.setTextSize(2);
  tft.print(units); // Units display
  // Calculate and return right hand side x coordinate
  return x + r;
}

unsigned int rainbow(byte value)
{
  // Value is expected to be in range 0-127
  // The value is converted to a spectrum colour from 0 = blue through to 127 = red
  byte red = 0; // Red is the top 5 bits of a 16 bit colour value
  byte green = 0;// Green is the middle 6 bits
  byte blue = 0; // Blue is the bottom 5 bits
  byte quadrant = value / 32;
  if (quadrant == 0) {
    blue = 31;
    green = 2 * (value % 32);
    red = 0;
  }
  if (quadrant == 1) {
    blue = 31 - (value % 32);
    green = 63;
    red = 0;
  }
  if (quadrant == 2) {
    blue = 0;
    green = 63;
    red = value % 32;
  }
  if (quadrant == 3) {
    blue = 0;
    green = 63 - 2 * (value % 32);
    red = 31;
  }
  return (red << 11) + (green << 5) + blue;
}
// #########################################################################
// Return a value in range -1 to +1 for a given phase angle in degrees
// #########################################################################
float sineWave(int phase) {
  return sin(phase * 0.0174532925);
}

