#include "num.h"
float arr1[28 * 28];
int filterWidth = 4;
int filterheight = 4;
void setup() {
  Serial.begin(9600);
  
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i=0; i < 28; i++){
    for(int j=0; j< 28; j++){
      int sum =0;

     for(int k =0; k<4;k++){
      for(int l =0; l<4; l++){
        sum += arr[4*(112*i+j) + 112 * k + l];
      }
     }
      sum = sum /16;
      arr1[i*28+j] = sum;
      Serial.print(sum);
     Serial.print(", ");
    }
    Serial.println("");
  }

Serial.println("");
 
}
