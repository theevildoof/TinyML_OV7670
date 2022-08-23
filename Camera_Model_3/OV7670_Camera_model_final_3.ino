//MPU6050_model.ino
#include <TensorFlowLite.h>
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"
#include "model.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>
#include <Arduino_OV767X.h>

const tflite::Model*  tflModel = nullptr; 
tflite::ErrorReporter*  tflErrorReporter = nullptr; 
TfLiteTensor* tflInputTensor = nullptr;  
TfLiteTensor* tflOutputTensor = nullptr; 
tflite::MicroInterpreter* tflInterpreter = nullptr; 

#define TFT_CS        A7
#define TFT_RST        7 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC         A6

constexpr int tensorArenaSize = 140 * 1024; 
uint8_t tensorArena[tensorArenaSize];
float out[10];

uint16_t pixels[176*144];
uint16_t color, pixel;
uint8_t red, blue, green;
int grayscale;

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void setup() {

  Serial.begin(9600);
  while (!Serial)
    delay(10);

  tft.initR(INITR_BLACKTAB); 
  delay(1000);
   if (!Camera.begin(QCIF, RGB565, 1)) {
    Serial.println("Failed to initialize camera!");
    while (1);
  }
  Serial.println(F("Initialized"));
     
  static tflite::MicroErrorReporter micro_error_reporter; 
  tflErrorReporter = &micro_error_reporter;

   tflModel = tflite::GetModel(model);
   if (tflModel->version() != TFLITE_SCHEMA_VERSION) {
   TF_LITE_REPORT_ERROR(tflErrorReporter,
        "Model provided is schema version %d not equal "
        "to supported version %d.",
        tflModel->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  static tflite::MicroMutableOpResolver<6> micro_op_resolver;
  micro_op_resolver.AddMaxPool2D();
  micro_op_resolver.AddConv2D();
  micro_op_resolver.AddDepthwiseConv2D();
  micro_op_resolver.AddFullyConnected();
  micro_op_resolver.AddReshape();
  micro_op_resolver.AddSoftmax();

  static tflite::MicroInterpreter static_interpreter(tflModel, micro_op_resolver, tensorArena, tensorArenaSize, tflErrorReporter);
  tflInterpreter = &static_interpreter;

  TfLiteStatus allocate_status = tflInterpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(tflErrorReporter, "AllocateTensors() failed");
    return;
  }
  tflInputTensor = tflInterpreter->input(0);
  
  tft.fillScreen(ST77XX_BLACK);
  delay(100);
 
  
  
  
}

void loop() {
  Camera.readFrame(pixels);
  tft.fillScreen(ST77XX_BLACK);
  for(int i =0; i<112;i++){
    for(int j =0;j<112;j++){
       
      tft.drawPixel(i,j,pixels[176*i+j]);
      Serial.print("");
    }
  }
 // delay(1000);
  for(int i =0; i<112;i++){
    for(int j =0;j<112;j++){
       pixel = pixels[176*i +j];
       red   = ((pixel >> 11) & 0x1f) << 3;
       green = ((pixel >> 5) & 0x3f) << 2; 
       blue  = ((pixel >> 0) & 0x1f) << 3; 
      grayscale = (red + blue + green)/3 ;
      if(grayscale <160){
        grayscale =0;
      }
      pixels[176*i +j] = grayscale;
    
     //tflInterpreter->input(0)->data.f[28*i+j] = grayscale / 255;
    }
    }

  Serial.println("");
  for(int i =0; i< 28; i++)
   {
    for(int j =0; j < 28; j++)
    {
       int sum =0;
       for(int k =0; k<4;k++)
       {
        for(int l =0; l<4; l++)
        {
          sum += pixels[4*(176*i+j) + 176 * k + l];
        }
       }
      sum = sum /16;
      //arr1[i*28+j] = sum;
      tflInterpreter->input(0)->data.f[28*i+j] = float(sum / 255.0);
      Serial.print(sum);
      Serial.print(", ");
    }
    Serial.println("");
   }
  
  delay(1000);
  TfLiteStatus invokeStatus = tflInterpreter->Invoke();
  out[0] = tflInterpreter->output(0)->data.f[0];
  out[1] = tflInterpreter->output(0)->data.f[1];
  out[2] = tflInterpreter->output(0)->data.f[2];
  out[3] = tflInterpreter->output(0)->data.f[3];
  out[4] = tflInterpreter->output(0)->data.f[4];
  out[5] = tflInterpreter->output(0)->data.f[5];
  out[6] = tflInterpreter->output(0)->data.f[6];
  out[7] = tflInterpreter->output(0)->data.f[7];
  out[8] = tflInterpreter->output(0)->data.f[8];
  out[9] = tflInterpreter->output(0)->data.f[9];

    float maxVal = out[0];
  int maxIndex = 0;
  for(int k =0; k < 10;k++){
    if (out[k] > maxVal) {
         maxVal = out[k];
         maxIndex = k;
      } 
  }
  Serial.print("Number ");
  Serial.print(maxIndex);
  Serial.println(" detected");
  Serial.print("Confidence: ");
  Serial.println(maxVal);
     
}
