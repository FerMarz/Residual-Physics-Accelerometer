/// <summary>
/// The purpose of this code is to take the 
/// data input from an MPU6050 accelerometer
/// and log it to a micro SD card. The data
/// acquisition starts after a certain number
/// of milliseconds of activating a switch and
/// stops when the switch is deactivated. An LED
/// is in charge of showing whether data is being
/// recorded or not.
/// </summary>

/// Arduino Uno connections
/// Terminal | Pin
///   - MPU6050 Sensor -
/// VCC  --> 5V
/// GND  --> Gnd
/// SCL  --> A4
/// SDA  --> A5
///   - SD Adapter -
/// GND  --> Gnd
/// VCC  --> 5V
/// MISO --> D12
/// MOSI --> D11
/// SCK  --> D13
/// CS   --> D9
///   - Switch -
/// GND  --> Gnd
/// VCC  --> D3
///   - LED -
/// GND  --> Gnd
/// VCC  --> D7

// Libraries to control the accelerometer
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"

// Library to control the SD adapter
#include <SD.h>

/// <summary> Reference to initialize and control the sensor </summary>
MPU6050 sensor;
/// <summary> Reference to the data log file of the SD card </summary>
File logFile;

/// <summary> Pin to which it is connected the switch</summary>
int switchPin = 3;
/// <summary> Pin to which it is connected the led positive terminal </summary>
int ledPin = 7;
/// <summary> Delay between data collection and data writing </summary>
const int DELAY = 10;
/// <summary> Delay after the switch is activated for data collection to begin </summary>
const int START_DELAY = 5000;
/// <summary> Check indicates if the time for data collection has elapsed </summary>
bool logData = false;
/// <summary> Variables to store the acceleration in the three axes </summary>
int ax, ay, az;

void setup()
{
  // Switch pins and LED are initialized.
  pinMode(switchPin, INPUT);
  pinMode(ledPin, OUTPUT);
  // Start with LED off
  digitalWrite(ledPin, LOW);
  // The sensor is initialized
  sensor.initialize();
}

void loop()
{
  // The negative value of Stop is taken
  bool start = !Stop();

  // If the switch is enabled and logData is true,
  // write the accelaration on the SD card file
  if (start && logData)
  {
    Write();
  }
  // If only the switch is enabled call Wait
  else if (start)
  {
    Wait();
  }
  // Wait DELAY time
  delay(DELAY);
}

/// <summary> Returns the current value of the switch </summary>
bool Stop()
{
  int value = digitalRead(switchPin);
  if (value == HIGH)
  {
    return false;
  }
  else
  {
    // Turns off the LED
    digitalWrite(ledPin, LOW);
    return true;
  }
}

/// <summary> Wait the time after the switch is activated and register that the data can be written </summary>
void Wait()
{
  delay(START_DELAY);
  digitalWrite(ledPin, HIGH);
  logData = true;
}

/// <summary> Wait the time after the switch is activated and register that the data can be written </summary>
void Write()
{
  // The file is opened to write the data
  logFile = SD.open("datalog.txt", FILE_WRITE);
  // Acceleration is recorded
  sensor.getAcceleration(&ax, &ay, &az);
  // If the file was opened successfully, the data is added
  if (logFile) {
    logFile.print(ax); logFile.print(",");
    logFile.print(ay); logFile.print(",");
    logFile.print(az); logFile.print(",");
    logFile.println(millis());
    logFile.close();
  }
  // Else, an error and the time at the time of the error are recorded
  else {
    logFile.print("Error en t = ");
    logFile.println(millis());
  }
}
