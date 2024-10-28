#include "SitSmart.h"
#include "Images.h"

SitSmart sitSmart("MAGS-OLC", "Merc1234!");

void setup() {
  sitSmart.begin();
}

void loop() {
  sitSmart.readData();
}