#include <Adafruit_INA219.h>
#include <SoftwareSerial.h>
#include "EmonLib.h"

#define pinRx 7
#define pinTx 8
#define pinLed 11
// LoRa Transmitter serial comunication object
SoftwareSerial Lora_Transmitter(pinRx, pinTx);

// INA219 current and power sensor object
Adafruit_INA219 ina219;

// EnergyMonitor object to measure current (using EmonLib library)
EnergyMonitor emon1;

String co; // String to hold the current value
String in; // String to hold the solar irradiance value
unsigned long looptime = 0; // Variable to store the current time in milliseconds
float current_mA = 0;       // Variable to store the measured current in milliamperes (mA)
float solar_Wm2 = 0;        // Variable to store the calculated solar irradiance in Watts per square meter (W/m2)
uint32_t currentFrequency;
double Irms = 0;
unsigned long time2 = 0;
int pino_sct = A0; 


void setup() {
  pinMode(pinLed, OUTPUT);

  Serial.begin(9600);
  Lora_Transmitter.begin(9600);
  

  if (!ina219.begin()) {     // Initialize the INA219 current and power sensor
    Serial.println("Failed to find INA219 chip");
    while (1) {
      delay(10);
    }
  }
  ina219.setCalibration_16V_400mA(); // Set the calibration values for INA219
  emon1.current(pino_sct, 10.6);            // Calibrate the EnergyMonitor object for current measurement
  
  Serial.println("## LoRa Transmitter ##");
}

void loop() {

  looptime = millis(); // Record the current time

  // Measure the current using the EnergyMonitor library
  double Irms = emon1.calcIrms(1480);
  // Read the current from INA219 sensor and calculate solar irradiance
  current_mA = ina219.getCurrent_mA();
  solar_Wm2 = 6.923130768114198 * current_mA + 19.12791886444639;
  
  co = String(Irms, 2); // Convert current value to String with 2 decimal places
  in = String(solar_Wm2, 2); // Convert solar irradiance value to String

  
  // Send measurements to LoRa Transmitter via SoftwareSerial
  if (looptime - time2 >= 1000) { // Update every minute
    time2 = looptime; // Update the last update time for ESP8266 communication
    String message = "/exec?corrente=" + co + "&irradiancia=" + in + "\0";
    Lora_Transmitter.print(message); // Send data to ESP8266
    Serial.println("##sent##" + message);
    debug();
  }
}

void debug(){
  digitalWrite(pinLed, HIGH);
  delay(1000);
  digitalWrite(pinLed, LOW);
  delay(1000);
}