# CAP1293

Simple Arduino Library for CAP1293 sensor.

## Summary

This is a simple library for the CAP1293 capacitive touch sensor for Arduino. It has most of the functionality from the datasheet. It connectes to the sensor via I2C, with the default address of `0x28`.


## Functions

Library implements the following functions. Function names should be self explanatory, but I will try to write a full documentation at some point.

Constructor: 

`CAP1293(byte addr = DEFAULT_I2C_ADDR);`

Begin:

`int begin(TwoWire &wirePort = Wire, uint8_t deviceAddress = DEFAULT_I2C_ADDR, uint8_t sensitivity = SENSITIVITY_32X, bool interrupts = true, bool sgEnable = false);`

Other functions:

```
bool isConnected();
void setSensitivity(uint8_t sensitivity);
uint8_t getSensitivity();

bool is1Touched();
bool is2Touched();
bool is3Touched();
bool isTouched(uint8_t id);
bool isTouched();
bool isMTPTouched();

void enableSMBusTimeout();
void disableSMBusTimeout();
void setMaximumHoldDuration(uint8_t duration);
void enableMaximumHoldDuration();
void disableMaximumHoldDuration();
void enableRFNoiseFilter();
void disableRFNoiseFilter();

void enableMultipleTouchLimit();
void disableMultipleTouchLimit();
void setMultipleTouchLimit(uint8_t touches);
void enableMTPDetection();
void disableMTPDetection();
void setMTPDetectionMode(uint8_t mode);
void setMTPPatternSpecificButtons(bool cs1_mtp, bool cs2_mtp, bool cs3_mtp);
void setMTPDetectionTreshold(uint8_t tresh);
void setMTPDetectionMinimalButtons(uint8_t btns);
void enableMTPInterrupt();
void disableMTPInterrupt();

void disableInterruptRepeatRate();
void enableInterruptRepeatRate();
void disableInterruptOnRelease();
void enableInterruptOnRelease();
  
void calibrateTouch(uint8_t id);
void calibrateAll();

void clearInterrupt();

void enableSignalGuard();
void disableSignalGuard();

void enableSensing(uint8_t id);
void disableSensing(uint8_t id);
bool isEnabledSensing(uint8_t id);

void setInterruptDisabled();
void setInterruptEnabled();
bool isInterruptEnabled();

void checkMainControl();
void checkStatus();


uint8_t getInputStatus();
uint8_t getGeneralStatus();
uint8_t getMainControl();
byte readRegister(CAP1293_Register reg);

```
  
  
  
