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

unsigned short pixels[176 * 144]; // QCIF: 176x144 X 2 bytes per pixel (RGB565)

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("OV767X Camera Capture");
  Serial.println();

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
  if (Serial.read() == 'c') {
    Serial.println("Reading frame");
    Serial.println();
    Camera.readFrame(pixels);

    int numPixels = Camera.width() * Camera.height();

    for (int i = 0; i < 1; i++) {
      for(int j =0; j < 1; j++){
        unsigned short p = 1234;
        if (p < 0x1000) {
          Serial.print('0');
        }
        if (p < 0x0100) {
         Serial.print('0');
        }
        if (p < 0x0010) {
          Serial.print('0');
        }
        
        Serial.println(p, HEX);
        Serial.println(p);    

        uint8_t bitsCount = sizeof( p ) * 8;
        char str[ bitsCount + 1 ];

        uint8_t i = 0;
        while ( bitsCount-- ){
          str[ i++ ] = bitRead( p, bitsCount ) + '0';
        }
        str[ i ] = '\0';
        Serial.println( str );
        char r[6];
        char g[7]; 
        char b[6];
        
        int r_val = 0;
        int g_val = 0;
        int b_val = 0;
        
        r[5] = '\0';
        g[6] = '\0';
        b[5] = '\0';
        for (int k = 0; k < 5; k = k + 1) {
         r[k] = str[k];
         //b[k] = str[11 + k] ;
          
         // r_val += str[k];
          Serial.println(str[k]);
          
          
         
         
        }
        for (int k = 0; k < 6; k = k + 1) {
         g[k] = str[5 + k];  
        // g_val += str[5+k] * pow(2,k); 
        }
      
        Serial.println(r);  
        //Serial.println(r_val);
        //Serial.println(g);
        //Serial.println(g_val);
        //Serial.println(b);
        //Serial.println(b_val);
      }
    }
  }
}
