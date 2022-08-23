

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>


  // For the breakout board, you can use any 2 or 3 pins.
  // These pins will also work for the 1.8" TFT shield.
  #define TFT_CS        A7
  #define TFT_RST        7 // Or set to -1 and connect to Arduino RESET pin
  #define TFT_DC         A6

// For 1.44" and 1.8" TFT with ST7735 use:
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
#include <Arduino_OV767X.h>

uint16_t pixels[176 *144];
uint16_t color;
uint8_t red, blue, green;

void setup(void) {
  Serial.begin(9600);

  // Use this initializer if using a 1.8" TFT screen:
  tft.initR(INITR_BLACKTAB); 
  delay(1000);// Init ST7735S chip, black tab
  Serial.println(F("Initialized"));
 
  if (!Camera.begin(QCIF, RGB565, 1)) {
    Serial.println("Failed to initialize camera!");
    while (1);
  }
  Serial.println(F("Initialized"));
  


  // large block of text
  tft.fillScreen(ST77XX_GREEN);
  
  
}

void loop() {
  Camera.readFrame(pixels);
  for(int i =0; i<112;i++){
    for(int j =0;j<112;j++){
      uint16_t pixel = pixels[176*i +j];
      Serial.print(red);
      tft.drawPixel(i,j,pixel);
      delay(0);
    }
  }
  delay(1000);
  //tft.fillScreen(ST77XX_BLACK);
}
