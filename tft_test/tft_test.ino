/*
  OV767X - Camera Test Pattern

  This sketch waits for the letter 'c' on the Serial Monitor,
  it then reads a frame from the OmniVision OV7670 camera and 
  prints the data to the Serial Monitor as a hex string.

  The website https://rawpixels.net - can be used the visualize the data:
    width: 176
    height: 144
    RGB565
    Little Endian

  Circuit:
    - Arduino Nano 33 BLE board
    - OV7670 camera module:
      - 3.3 connected to 3.3
      - GND connected GND
      - SIOC connected to A5
      - SIOD connected to A4
      - VSYNC connected to 8
      - HREF connected to A1
      - PCLK connected to A0
      - XCLK connected to 9
      - D7 connected to 4
      - D6 connected to 6
      - D5 connected to 5
      - D4 connected to 3
      - D3 connected to 2
      - D2 connected to 0 / RX
      - D1 connected to 1 / TX
      - D0 connected to 10

  This example code is in the public domain.
*/

#include <Arduino_OV767X.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

#define TFT_CS        A7
  #define TFT_RST        7 // Or set to -1 and connect to Arduino RESET pin
  #define TFT_DC         A6


Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

uint16_t pixels[176 * 144];
void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("OV767X Camera Capture");
  Serial.println();

  tft.initR(INITR_BLACKTAB); 
  delay(1000);
  
  if (!Camera.begin(QCIF, RGB565, 1)) {
    Serial.println("Failed to initialize camera!");
    while (1);
  }

  Serial.println("Camera settings:");
  Serial.print("\twidth = ");
  Serial.println(Camera.width());
  Serial.print("\theight = ");
  Serial.println(Camera.height());
  Serial.print("\tbits per pixel = ");
  Serial.println(Camera.bitsPerPixel());
  Serial.println();

  Serial.println("Send the 'c' character to read a frame ...");
  Serial.println();
}

void loop() {
    Serial.println("Reading frame");
    Serial.println();
    Camera.readFrame(pixels);
    tft.fillScreen(ST77XX_BLACK);
    for (int i = 0; i < 112; i++) {
      for(int j = 0; j < 112; j++){
       uint16_t pixel = pixels[176*i +j];
       int red   = ((pixel >> 11) & 0x1f) << 3;
       int green = ((pixel >> 5) & 0x3f) << 2; 
       int blue  = ((pixel >> 0) & 0x1f) << 3; 
      
      Serial.println(red);
      Serial.println(green);
      Serial.println(blue);
      tft.drawPixel(i,j,pixels[176*i+j]);
      }
    }
    delay(2000);
}
