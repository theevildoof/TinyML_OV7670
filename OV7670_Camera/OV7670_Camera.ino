//MPU6050_model.ino
#include <TensorFlowLite.h>
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"
#include "model.h"
#include "num.h"


const tflite::Model*  tflModel = nullptr; 
tflite::ErrorReporter*  tflErrorReporter = nullptr; 
TfLiteTensor* tflInputTensor = nullptr;  
TfLiteTensor* tflOutputTensor = nullptr; 
tflite::MicroInterpreter* tflInterpreter = nullptr; 

constexpr int tensorArenaSize = 140 * 1024; 
uint8_t tensorArena[tensorArenaSize];
float out[10];

void setup() {

  Serial.begin(115200);
  while (!Serial)
    delay(10);
     
  
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
  

}

void test_func(){
  for(int i = 0; i < 28; i++){
    for(int j =0; j < 28; j++){
      tflInterpreter->input(0)->data.f[28*i+j] = num[28*i+j] / 255;
    } 
  }
  /*
   for(int i = 0; i < 28; i++){
    for(int j =0; j < 28; j++){
      Serial.print(tflInterpreter->input(0)->data.f[28*i+j]);
      Serial.print(", ");
    } 
      Serial.println("");
  }
 Serial.println("");
  for(int i = 0; i < 28; i++){
    for(int j =0; j < 28; j++){
      Serial.print(num[28*i+j]);
      Serial.print(", ");
    } 
      Serial.println("");
  }
  Serial.println("");
  for(int i = 0; i < 28; i++){
    for(int j =0; j < 28; j++){
      Serial.print(tflInterpreter->input(0)->data.f[28*i+j]-num[28*i+j]);
      Serial.print(", ");
    } 
      Serial.println("");
  }
  */
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

  
  /*
  Serial.print(out0);
  Serial.print(",");
  Serial.print(out1);
  Serial.print(",");
  Serial.print(out2);
  Serial.print(",");
  Serial.print(out3);
  Serial.print(",");
  Serial.print(out4);
  Serial.print(",");
  Serial.print(out5);
  Serial.print(",");
  Serial.print(out6);
  Serial.print(",");
  Serial.print(out7);
  Serial.print(",");
  Serial.print(out8);
  Serial.print(",");
  Serial.println(out9);
  */
  
}
void loop() {
  
    
   for(int i = 0; i < 28; i++){
    for(int j =0; j < 28; j++){
      tflInterpreter->input(0)->data.f[28*i+j] = num[28*i+j]/255;
    } 
  }
  /*
   for(int i = 0; i < 1; i++){
    for(int j =0; j < 1; j++){
      Serial.print(tflInterpreter->input(0)->data.f[28*i+j]);
      Serial.print(", ");
    } 

  }
  */
  
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
