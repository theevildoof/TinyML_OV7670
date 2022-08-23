#include <TensorFlowLite_ESP32.h>
#include "tensorflow/lite/experimental/micro/micro_error_reporter.h"
#include "tensorflow/lite/experimental/micro/micro_interpreter.h"
#include "tensorflow/lite/experimental/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"
//#include "tensorflow/lite/experimental/micro/kernels/all_ops_resolver.h"
#include "tensorflow/lite/experimental/micro/kernels/micro_ops.h"
#include "model.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include <Wire.h>
#include<Uduino.h>

int pushButton = 27;

Uduino uduino("mpu6050");
MPU6050 mpu(0x69);

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer
int count =1;
// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
int count_no;
#define THRESHOLD 15000

const tflite::Model*  tflModel; //
tflite::ErrorReporter*  tflErrorReporter; //
constexpr int tensorArenaSize = 102 * 1024; //
uint8_t tensorArena[tensorArenaSize];
const char* GESTURES[] = {
  "shoot",
};
TfLiteTensor* tflInputTensor; //
TfLiteTensor* tflOutputTensor; //
tflite::MicroInterpreter* tflInterpreter; //


void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties

  Serial.begin(115200);

  while (!Serial); // wait for Leonardo enumeration, others continue immediately
  pinMode(pushButton, INPUT);
  mpu.initialize();
  devStatus = mpu.dmpInitialize();
  mpu.setXGyroOffset(21); //++
  mpu.setYGyroOffset(-22); //--
  mpu.setZGyroOffset(28);

  if (devStatus == 0) {
    mpu.setDMPEnabled(true);
    // set our DMP Ready flag so the main loop() function knows it's okay to use it
    dmpReady = true;
    // get expected DMP packet size for later comparison
    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    // Error
    Serial.println("Error!");
  }

  static tflite::MicroErrorReporter micro_error_reporter;  // NOLINT
  tflErrorReporter = &micro_error_reporter;

   tflModel = tflite::GetModel(g_model);
  //if (1 != TFLITE_SCHEMA_VERSION) {
   // tflErrorReporter->Report(
     //   "Model provided is schema version %d not equal "
       // "to supported version %d.",
       // 1, TFLITE_SCHEMA_VERSION);
   // return;
 // }
  static tflite::MicroMutableOpResolver micro_mutable_op_resolver;  // NOLINT
  micro_mutable_op_resolver.AddBuiltin(
      tflite::BuiltinOperator_FULLY_CONNECTED,
      tflite::ops::micro::Register_FULLY_CONNECTED());
   micro_mutable_op_resolver.AddBuiltin(  
       tflite::BuiltinOperator_SOFTMAX,
      tflite::ops::micro::Register_SOFTMAX());
      
  static tflite::MicroInterpreter static_interpreter(tflModel, micro_mutable_op_resolver, tensorArena, tensorArenaSize, tflErrorReporter);
  tflInterpreter = &static_interpreter;
  
  tflInterpreter->AllocateTensors();
  //Serial.print("setup complete");
  tflInputTensor = tflInterpreter->input(0);
  tflOutputTensor = tflInterpreter->output(0);
  
  
}

void loop() {
  uduino.update();
  if(uduino.isConnected()){
  // put your main code here, to run repeatedly:
   if (!dmpReady) {
      Serial.println("IMU not connected.");
      delay(10);
      return;
    }

    int  mpuIntStatus = mpu.getIntStatus();
    fifoCount = mpu.getFIFOCount();

    if ((mpuIntStatus & 0x10) || fifoCount == 1024) { // check if overflow
      mpu.resetFIFO();
    } else if (mpuIntStatus & 0x02) {
      while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

      mpu.getFIFOBytes(fifoBuffer, packetSize);
      fifoCount -= packetSize;
      detect_motion_and_send_gyro_data();
    }
  }
}

void detect_motion_and_send_gyro_data(){
  mpu.dmpGetQuaternion(&q, fifoBuffer);
  
  
  mpu.dmpGetAccel(&aa, fifoBuffer);
  mpu.dmpGetGravity(&gravity, &q);
  mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
  mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
  //if(digitalRead(pushButton) == 1){
  Serial.print("r,");
  Serial.print(q.w, 4); Serial.print(",");
  Serial.print(q.x, 4); Serial.print(",");
  Serial.print(q.y, 4); Serial.print(",");
 Serial.println(q.z, 4);
  //}
  if( abs(aaWorld.x) +abs(aaWorld.y) + abs(aaWorld.y) > 15000){
    run_inference();
  }
}

void run_inference(){
  count_no = 0;
   while(count_no<40){
     int  mpuIntStatus = mpu.getIntStatus();
    fifoCount = mpu.getFIFOCount();

    if ((mpuIntStatus & 0x10) || fifoCount == 1024) { // check if overflow
      mpu.resetFIFO();
    } else if (mpuIntStatus & 0x02) {
      while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

      mpu.getFIFOBytes(fifoBuffer, packetSize);
      fifoCount -= packetSize;
     SendWorldAccel();  
    }
    };
 TfLiteStatus invokeStatus = tflInterpreter->Invoke();
  float out1 = tflInterpreter->output(0)->data.f[0];
   float out2 = tflInterpreter->output(0)->data.f[1];
  Serial.print(out1);
   Serial.print(" ");
    Serial.println(out2);
   //if(out >= 0){
    //Serial.println(1);
  //}
  //else{
   // Serial.println(0);
  //}
    
}

void SendWorldAccel(){ 
  mpu.dmpGetQuaternion(&q, fifoBuffer);
  mpu.dmpGetAccel(&aa, fifoBuffer);
  mpu.dmpGetGravity(&gravity, &q);
  mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
  mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
  tflInputTensor->data.f[count_no * 3 + 0] = (aaWorld.x) / 16234.0;
  tflInputTensor->data.f[count_no * 3 + 1] = (aaWorld.y) / 16234.0;
  tflInputTensor->data.f[count_no * 3 + 2] = (aaWorld.z) / 16234.0;
  count_no++;
}
