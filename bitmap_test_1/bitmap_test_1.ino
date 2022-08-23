

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

unsigned char myBitmap [] PROGMEM = {
0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x01, 0xf0, 
0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x03, 0xf0, 
0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x07, 0xf0, 0x00, 0x00, 0x07, 0xf0, 
0x00, 0x00, 0x07, 0xf0, 0x00, 0x00, 0x07, 0xf0, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x07, 0xf0, 
0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x1f, 0xf0, 0x00, 0x00, 0x1f, 0xf0, 
0x00, 0x00, 0x3f, 0xf0, 0x00, 0x00, 0x3f, 0xf0, 0x00, 0x00, 0x3f, 0xf0, 0x00, 0x00, 0x7f, 0xf0, 
0x00, 0x00, 0x7f, 0xf0, 0x00, 0x00, 0xff, 0xf0, 0x00, 0x00, 0xff, 0xf0, 0x00, 0x00, 0xff, 0xe0
};
void setup()   {                
  // On my display, I had to used 0x3C as the address, something to do with the RESET not being
  // connected to the Arduino. THe 0x3D address below is the address used in the original
  // Adafruit OLED example 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  
  display.clearDisplay(); // Make sure the display is cleared
  // Draw the bitmap:
  // drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  display.drawBitmap(0, 0, myBitmap, 28, 28, WHITE);

  // Update the display
  
}


void loop() {
  display.clearDisplay();
  for(int i =0; i < 112; i++){
    myBitmap[i] = random(255);
  }
  display.drawBitmap(0, 0, myBitmap, 28, 28, WHITE);
  display.display();
  delay(100);
}
