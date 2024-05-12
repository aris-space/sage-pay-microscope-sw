#include "Arducam_Mega.h"


#define BUFFER_SIZE 0xff

const int CS = 16;
uint8_t count = 0;
char name[10] = {0};
uint8_t rtLength = 0;
uint8_t imageData = 0;
uint8_t imageDataNext = 0;
uint8_t headFlag = 0;
unsigned int i = 0;
uint8_t imageBuff[BUFFER_SIZE] = {0};
char cmd;
char arducamSpiCsPinLow = 1;
char arducamSpiCsPinHigh = 1;
char arducamCsOutputMode = 1;

Arducam_Mega myCam( CS );
uint8_t keyState = 0;
uint8_t isCaptureFlag = 0;

// count the number of values printed, every 16 bytes a new line is printed
int row_count = 0;

void image() {
  myCam.takePicture(
    CAM_IMAGE_MODE_QVGA,    // resolution: QVGA (256x256)
    CAM_IMAGE_PIX_FMT_JPG   // data format: JPG
  );

  while(myCam.getReceivedLength()) {
    imageData = imageDataNext;
    imageDataNext = myCam.readByte();

    //beginning of image
    if(imageData == 0xFF && imageDataNext == 0xD8) {
      headFlag = 1;
      //sprintf(name, "%d.jpg", count);
      count++;
      //Serial.print(imageData, HEX);
      //Serial.print("done");
      imageBuff[i++] = imageData;
      //imageBuff[i++]=imageDataNext;
    }

    //reads out the image
    if(headFlag == 1) {
      imageBuff[i++] = imageDataNext;
      if(i >= BUFFER_SIZE) {
        for (int j = 0; j < sizeof(imageBuff); j++) {
          //Serial.write(imageBuff[j]);
          Serial.print(imageBuff[j]);
          Serial.print(" ");
          row_count++;

          if (row_count == 16) {
            row_count = 0;
            Serial.println("");
          }
        }
        //Serial.write(imageBuff, sizeof(imageBuff));
        i = 0;
      }
    }

    //end of image
    if(imageData == 0xFF && imageDataNext == 0xD9) {
      headFlag = 0;
      //Serial.write(imageBuff, sizeof(imageBuff));
      i = 0;

      // Send end of JPEG
      Serial.print(0xFF);
      Serial.print(" ");
      Serial.print(0xD9);
      Serial.println("");

      //Serial.print(" ");
      //Serial.println("Image succeed");
      break;
    }
  }
  delay(5000);
}

void setup() {
  Serial.begin(115200);
  myCam.begin();
  myCam.takePicture(CAM_IMAGE_MODE_QVGA,CAM_IMAGE_PIX_FMT_JPG);
  Serial.println("Setup completed!");
  delay(1000);
  // uncompiled, untested
  // uint8_t message[] = {0xAA, 0xBB, 0x06, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x03 };

  // Serial.print(0xFF);
}

void loop() {
  if(Serial.available()){
    //cmd = Serial.readStringUntil('\n');
    cmd = Serial.read();

    if(cmd != -1){
      //Serial.println("Imaging mode engaged");
      image();
    }
  }


}
