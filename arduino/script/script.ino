#include "SitSmart.h"
#include "Images.h"

SitSmart sitSmart(" ", " ");

File myFile;
String fileName = "test.txt";

void setup() {
  sitSmart.begin();

  writeToSD("abab", true);
  readFromSD();
}

void loop() {
  //sitSmart.readData();
}

void writeToSD(String input, bool clearSDFile) {
  if (clearSDFile) {
    SD.remove(fileName);
  }
  
  File myFile = SD.open(fileName, FILE_WRITE);

  if (myFile) {
    // print to the file
    myFile.println(input);
    myFile.close();
  } else {
    Serial.println("error1");
  }
}

void readFromSD() {

  File myFile = SD.open(fileName);
  if (myFile) {

    // read from the file
    while (myFile.available()) {
      // TODO: save in string instead of printing
      Serial.write(myFile.read());
    }
    myFile.close();
  } else {
    Serial.println("error2");
  }
}