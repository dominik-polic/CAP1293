/*
 *	This file contains all the implementations of CAP1293 library functions.
 */
 
#include <Arduino.h>
#include <Wire.h>
#include "CAP1293_registers.h"
#include "CAP1293.h"
 
 
/*
 * Constructor
 * addr should be 0x50
 */
CAP1293::CAP1293(byte addr){
	_deviceAddress = addr;
}


/*
 * This function initializes the CAP1293 sensor.
 */
int CAP1293::begin(TwoWire &wirePort, uint8_t deviceAddress, uint8_t sensitivity, bool interrupts, bool sgEnable)
{
    // Set device address and wire port to private variable
    _deviceAddress = deviceAddress;
    _i2cPort = &wirePort;
	_singalGuardEnabled = sgEnable;
	
    if (isConnected() == false)
    {
        return ERR_NO_DEVICE_AT_ADDRESS;
    }
    // Read PROD_ID register
    byte prodIDValue = readRegister(PROD_ID);

    // PROD_ID should always be 0x6F
    if (prodIDValue != PROD_ID_VALUE)
    {
        return ERR_WRONG_PROD_ID;
    }

    setSensitivity(sensitivity); 
	
	//Signal Guard, if enabled disables touch 2
	
	if(sgEnable == true){
		enableSignalGuard();
	}else{
		disableSignalGuard();
	}
	
	//Interrupts are enabled by default
	if(interrupts)
		setInterruptEnabled();          
	else
		setInterruptDisabled();
	
    clearInterrupt();               // Clear interrupt on startup
    return BEGIN_SUCCESS;
}

/*
 *	Returns true if device ACKs the connection, false otherwise
 */
bool CAP1293::isConnected()
{
    for (byte i = 0; i < 5; i++)
    {
        // Apparently it's possible that sometimes the device only acknowelages the connection after about 2 tries so compensate for that. 
        _i2cPort->beginTransmission((uint8_t)_deviceAddress);
        if (_i2cPort->endTransmission() == 0)
            return (true); 
    }

    return (false);
}

void CAP1293::checkMainControl()
{
    MAIN_CONTROL_REG reg;
    reg.MAIN_CONTROL_COMBINED = readRegister(MAIN_CONTROL);
}

void CAP1293::checkStatus()
{
    GENERAL_STATUS_REG reg;
    reg.GENERAL_STATUS_COMBINED = readRegister(GENERAL_STATUS);
}

void CAP1293::clearInterrupt()
{
    MAIN_CONTROL_REG reg;
    reg.MAIN_CONTROL_COMBINED = readRegister(MAIN_CONTROL);
    reg.MAIN_CONTROL_FIELDS.INT = 0x00;
    writeRegister(MAIN_CONTROL, reg.MAIN_CONTROL_COMBINED);
}

//-----BEGIN CONFIGURATION-----

void CAP1293::enableSMBusTimeout(){
	CONFIGURATION_REG reg;
	reg.CONFIGURATION_COMBINED = readRegister(CONFIG);
	reg.CONFIGURATION_FIELDS.TIMEOUT = 0x01;
	writeRegister(CONFIG, reg.CONFIGURATION_COMBINED);
}

void CAP1293::disableSMBusTimeout(){
	CONFIGURATION_REG reg;
	reg.CONFIGURATION_COMBINED = readRegister(CONFIG);
	reg.CONFIGURATION_FIELDS.TIMEOUT = 0x00;
	writeRegister(CONFIG, reg.CONFIGURATION_COMBINED);
}

void CAP1293::setMaximumHoldDuration(uint8_t duration){
	SENSOR_INPUT_CONFIGURATION_REG reg;
	reg.SENSOR_INPUT_CONFIGURATION_COMBINED = readRegister(SENSOR_INPUT_CONFIG);
	if(duration == MAX_DURRATION_560) reg.SENSOR_INPUT_CONFIGURATION_FIELDS.MAX_DUR = MAX_DURRATION_560;
	else if(duration == MAX_DURRATION_840) reg.SENSOR_INPUT_CONFIGURATION_FIELDS.MAX_DUR = MAX_DURRATION_840;
	else if(duration == MAX_DURRATION_1120) reg.SENSOR_INPUT_CONFIGURATION_FIELDS.MAX_DUR = MAX_DURRATION_1120;
	else if(duration == MAX_DURRATION_1400) reg.SENSOR_INPUT_CONFIGURATION_FIELDS.MAX_DUR = MAX_DURRATION_1400;
	else if(duration == MAX_DURRATION_1680) reg.SENSOR_INPUT_CONFIGURATION_FIELDS.MAX_DUR = MAX_DURRATION_1680;
	else if(duration == MAX_DURRATION_2240) reg.SENSOR_INPUT_CONFIGURATION_FIELDS.MAX_DUR = MAX_DURRATION_2240;
	else if(duration == MAX_DURRATION_2800) reg.SENSOR_INPUT_CONFIGURATION_FIELDS.MAX_DUR = MAX_DURRATION_2800;
	else if(duration == MAX_DURRATION_3360) reg.SENSOR_INPUT_CONFIGURATION_FIELDS.MAX_DUR = MAX_DURRATION_3360;
	else if(duration == MAX_DURRATION_3920) reg.SENSOR_INPUT_CONFIGURATION_FIELDS.MAX_DUR = MAX_DURRATION_3920;
	else if(duration == MAX_DURRATION_4480) reg.SENSOR_INPUT_CONFIGURATION_FIELDS.MAX_DUR = MAX_DURRATION_4480;
	else if(duration == MAX_DURRATION_5600) reg.SENSOR_INPUT_CONFIGURATION_FIELDS.MAX_DUR = MAX_DURRATION_5600;
	else if(duration == MAX_DURRATION_6720) reg.SENSOR_INPUT_CONFIGURATION_FIELDS.MAX_DUR = MAX_DURRATION_6720;
	else if(duration == MAX_DURRATION_7840) reg.SENSOR_INPUT_CONFIGURATION_FIELDS.MAX_DUR = MAX_DURRATION_7840;
	else if(duration == MAX_DURRATION_8906) reg.SENSOR_INPUT_CONFIGURATION_FIELDS.MAX_DUR = MAX_DURRATION_8906;
	else if(duration == MAX_DURRATION_10080) reg.SENSOR_INPUT_CONFIGURATION_FIELDS.MAX_DUR = MAX_DURRATION_10080;
	else if(duration == MAX_DURRATION_11200) reg.SENSOR_INPUT_CONFIGURATION_FIELDS.MAX_DUR = MAX_DURRATION_11200;
	else reg.SENSOR_INPUT_CONFIGURATION_FIELDS.MAX_DUR = MAX_DURRATION_5600;	//Default
	writeRegister(SENSOR_INPUT_CONFIG, reg.SENSOR_INPUT_CONFIGURATION_COMBINED);
	enableMaximumHoldDuration();	
}

void CAP1293::enableMaximumHoldDuration(){
	CONFIGURATION_REG reg;
	reg.CONFIGURATION_COMBINED = readRegister(CONFIG);
	reg.CONFIGURATION_FIELDS.MAX_DUR_EN = 0x01;
	writeRegister(CONFIG, reg.CONFIGURATION_COMBINED);
}


void CAP1293::disableMaximumHoldDuration(){
	CONFIGURATION_REG reg;
	reg.CONFIGURATION_COMBINED = readRegister(CONFIG);
	reg.CONFIGURATION_FIELDS.MAX_DUR_EN = 0x00;
	writeRegister(CONFIG, reg.CONFIGURATION_COMBINED);
}

void CAP1293::enableRFNoiseFilter(){
	CONFIGURATION_2_REG reg;
	reg.CONFIGURATION_2_COMBINED = readRegister(CONFIG_2);
	reg.CONFIGURATION_2_FIELDS.DIS_RF_NOISE = 0x00;
	writeRegister(CONFIG_2, reg.CONFIGURATION_2_COMBINED);
}

void CAP1293::disableRFNoiseFilter(){
	CONFIGURATION_2_REG reg;
	reg.CONFIGURATION_2_COMBINED = readRegister(CONFIG_2);
	reg.CONFIGURATION_2_FIELDS.DIS_RF_NOISE = 0x01;
	writeRegister(CONFIG_2, reg.CONFIGURATION_2_COMBINED);
}

void CAP1293::enableMultipleTouchLimit(){
	MULTIPLE_TOUCH_CONFIGURATION_REG reg;
	reg.MULTIPLE_TOUCH_CONFIGURATION_COMBINED = readRegister(MULTIPLE_TOUCH_CONFIG);
	reg.MULTIPLE_TOUCH_CONFIGURATION_FIELDS.MULT_BLK_EN = 0x01;
	writeRegister(MULTIPLE_TOUCH_CONFIG, reg.MULTIPLE_TOUCH_CONFIGURATION_COMBINED);
}
void CAP1293::disableMultipleTouchLimit(){
	MULTIPLE_TOUCH_CONFIGURATION_REG reg;
	reg.MULTIPLE_TOUCH_CONFIGURATION_COMBINED = readRegister(MULTIPLE_TOUCH_CONFIG);
	reg.MULTIPLE_TOUCH_CONFIGURATION_FIELDS.MULT_BLK_EN = 0x00;
	writeRegister(MULTIPLE_TOUCH_CONFIG, reg.MULTIPLE_TOUCH_CONFIGURATION_COMBINED);
}
void CAP1293::setMultipleTouchLimit(uint8_t touches){
	MULTIPLE_TOUCH_CONFIGURATION_REG reg;
	reg.MULTIPLE_TOUCH_CONFIGURATION_COMBINED = readRegister(MULTIPLE_TOUCH_CONFIG);
	reg.MULTIPLE_TOUCH_CONFIGURATION_FIELDS.MULT_BLK_EN = 0x01;
	if(touches == 1) reg.MULTIPLE_TOUCH_CONFIGURATION_FIELDS.B_MULT_T = 0x00;
	else if(touches == 2) reg.MULTIPLE_TOUCH_CONFIGURATION_FIELDS.B_MULT_T = 0x01;
	else if(touches == 3) reg.MULTIPLE_TOUCH_CONFIGURATION_FIELDS.B_MULT_T = 0x02;
	//else if(touches == 4) reg.MULTIPLE_TOUCH_CONFIGURATION_FIELDS.B_MULT_T = 0x04;
	writeRegister(MULTIPLE_TOUCH_CONFIG, reg.MULTIPLE_TOUCH_CONFIGURATION_COMBINED);
}


void CAP1293::enableMTPDetection(){
	MULTIPLE_TOUCH_PATTERN_CONFIGURATION_REG reg;
	reg.MULTIPLE_TOUCH_PATTERN_CONFIGURATION_COMBINED = readRegister(MULTIPLE_TOUCH_PATTERN_CONFIG);
	reg.MULTIPLE_TOUCH_PATTERN_CONFIGURATION_FIELDS.MTP_EN = 0x01;
	writeRegister(MULTIPLE_TOUCH_PATTERN_CONFIG, reg.MULTIPLE_TOUCH_PATTERN_CONFIGURATION_COMBINED);
}
void CAP1293::disableMTPDetection(){
  	MULTIPLE_TOUCH_PATTERN_CONFIGURATION_REG reg;
	reg.MULTIPLE_TOUCH_PATTERN_CONFIGURATION_COMBINED = readRegister(MULTIPLE_TOUCH_PATTERN_CONFIG);
	reg.MULTIPLE_TOUCH_PATTERN_CONFIGURATION_FIELDS.MTP_EN = 0x00;
	writeRegister(MULTIPLE_TOUCH_PATTERN_CONFIG, reg.MULTIPLE_TOUCH_PATTERN_CONFIGURATION_COMBINED);
}

void CAP1293::setMTPDetectionTreshold(uint8_t tresh){
	MULTIPLE_TOUCH_PATTERN_CONFIGURATION_REG reg;
	reg.MULTIPLE_TOUCH_PATTERN_CONFIGURATION_COMBINED = readRegister(MULTIPLE_TOUCH_PATTERN_CONFIG);
	if(tresh == MTP_TRESHOLD_12_5) reg.MULTIPLE_TOUCH_PATTERN_CONFIGURATION_FIELDS.MTP_TH = 0x00;
	else if(tresh == MTP_TRESHOLD_25) reg.MULTIPLE_TOUCH_PATTERN_CONFIGURATION_FIELDS.MTP_TH = 0x01;
	else if(tresh == MTP_TRESHOLD_37_5) reg.MULTIPLE_TOUCH_PATTERN_CONFIGURATION_FIELDS.MTP_TH = 0x02;
	else if(tresh == MTP_TRESHOLD_100) reg.MULTIPLE_TOUCH_PATTERN_CONFIGURATION_FIELDS.MTP_TH = 0x03;
	writeRegister(MULTIPLE_TOUCH_PATTERN_CONFIG, reg.MULTIPLE_TOUCH_PATTERN_CONFIGURATION_COMBINED);
}
void CAP1293::setMTPDetectionMode(uint8_t mode){
	MULTIPLE_TOUCH_PATTERN_CONFIGURATION_REG reg;
	reg.MULTIPLE_TOUCH_PATTERN_CONFIGURATION_COMBINED = readRegister(MULTIPLE_TOUCH_PATTERN_CONFIG);
	if(mode == MTP_MODE_SPECIFIC) reg.MULTIPLE_TOUCH_PATTERN_CONFIGURATION_FIELDS.COMP_PTRN = 0x01;
	else if(mode == MTP_MODE_MINIMAL_TOUCHES) reg.MULTIPLE_TOUCH_PATTERN_CONFIGURATION_FIELDS.COMP_PTRN = 0x00;
	writeRegister(MULTIPLE_TOUCH_PATTERN_CONFIG, reg.MULTIPLE_TOUCH_PATTERN_CONFIGURATION_COMBINED);  
}
void CAP1293::setMTPPatternSpecificButtons(bool cs1_mtp, bool cs2_mtp, bool cs3_mtp){
	MULTIPLE_TOUCH_PATTERN_REG reg;
	reg.MULTIPLE_TOUCH_PATTERN_COMBINED = readRegister(MULTIPLE_TOUCH_PATTERN);
	reg.MULTIPLE_TOUCH_PATTERN_FIELDS.CS1_PTRN = cs1_mtp?0x01:0x00;
	reg.MULTIPLE_TOUCH_PATTERN_FIELDS.CS2_PTRN = cs2_mtp?0x01:0x00;
	reg.MULTIPLE_TOUCH_PATTERN_FIELDS.CS3_PTRN = cs3_mtp?0x01:0x00;
	writeRegister(MULTIPLE_TOUCH_PATTERN, reg.MULTIPLE_TOUCH_PATTERN_COMBINED);
}

void CAP1293::setMTPDetectionMinimalButtons(uint8_t btns){
	MULTIPLE_TOUCH_PATTERN_REG reg;
	reg.MULTIPLE_TOUCH_PATTERN_COMBINED = readRegister(MULTIPLE_TOUCH_PATTERN);
	reg.MULTIPLE_TOUCH_PATTERN_FIELDS.CS1_PTRN = (btns>=1)?0x01:0x00;
	reg.MULTIPLE_TOUCH_PATTERN_FIELDS.CS2_PTRN = (btns>=2)?0x01:0x00;
	reg.MULTIPLE_TOUCH_PATTERN_FIELDS.CS3_PTRN = (btns>=3)?0x01:0x00;
	writeRegister(MULTIPLE_TOUCH_PATTERN, reg.MULTIPLE_TOUCH_PATTERN_COMBINED);
}

void CAP1293::enableMTPInterrupt(){
	MULTIPLE_TOUCH_PATTERN_CONFIGURATION_REG reg;
	reg.MULTIPLE_TOUCH_PATTERN_CONFIGURATION_COMBINED = readRegister(MULTIPLE_TOUCH_PATTERN_CONFIG);
	reg.MULTIPLE_TOUCH_PATTERN_CONFIGURATION_FIELDS.MTP_ALERT = 0x01;
	writeRegister(MULTIPLE_TOUCH_PATTERN_CONFIG, reg.MULTIPLE_TOUCH_PATTERN_CONFIGURATION_COMBINED);  
}

void CAP1293::disableMTPInterrupt(){
	MULTIPLE_TOUCH_PATTERN_CONFIGURATION_REG reg;
	reg.MULTIPLE_TOUCH_PATTERN_CONFIGURATION_COMBINED = readRegister(MULTIPLE_TOUCH_PATTERN_CONFIG);
	reg.MULTIPLE_TOUCH_PATTERN_CONFIGURATION_FIELDS.MTP_ALERT = 0x00;
	writeRegister(MULTIPLE_TOUCH_PATTERN_CONFIG, reg.MULTIPLE_TOUCH_PATTERN_CONFIGURATION_COMBINED); 
}

void CAP1293::disableInterruptRepeatRate(){
	REPEAT_RATE_ENABLE_REG reg;
	reg.REPEAT_RATE_ENABLE_COMBINED = readRegister(REPEAT_RATE_ENABLE);
	reg.REPEAT_RATE_ENABLE_FIELDS.CS3_RPT_EN = 0x00;
	reg.REPEAT_RATE_ENABLE_FIELDS.CS2_RPT_EN = 0x00;
	reg.REPEAT_RATE_ENABLE_FIELDS.CS1_RPT_EN = 0x00;
	writeRegister(REPEAT_RATE_ENABLE, reg.REPEAT_RATE_ENABLE_COMBINED); 
}

void CAP1293::enableInterruptRepeatRate(){
	REPEAT_RATE_ENABLE_REG reg;
	reg.REPEAT_RATE_ENABLE_COMBINED = readRegister(REPEAT_RATE_ENABLE);
	reg.REPEAT_RATE_ENABLE_FIELDS.CS3_RPT_EN = 0x01;
	reg.REPEAT_RATE_ENABLE_FIELDS.CS2_RPT_EN = 0x01;
	reg.REPEAT_RATE_ENABLE_FIELDS.CS1_RPT_EN = 0x01;
	writeRegister(REPEAT_RATE_ENABLE, reg.REPEAT_RATE_ENABLE_COMBINED); 
}

void CAP1293::enableInterruptOnRelease(){
	CONFIGURATION_2_REG reg;
	reg.CONFIGURATION_2_COMBINED = readRegister(CONFIG_2);
	reg.CONFIGURATION_2_FIELDS.INT_REL_n= 0x00;
	writeRegister(CONFIG_2, reg.CONFIGURATION_2_COMBINED); 
}

void CAP1293::disableInterruptOnRelease(){
	CONFIGURATION_2_REG reg;
	reg.CONFIGURATION_2_COMBINED = readRegister(CONFIG_2);
	reg.CONFIGURATION_2_FIELDS.INT_REL_n= 0x01;
	writeRegister(CONFIG_2, reg.CONFIGURATION_2_COMBINED); 
}

//-----END CONFIGURATION-----

void CAP1293::calibrateTouch(uint8_t id){
	CALIBRATION_ACTIVATE_AND_STATUS_REG reg;
    reg.CALIBRATION_ACTIVATE_AND_STATUS_COMBINED = readRegister(CALIBRATION_ACTIVATE_AND_STATUS);
	if(id == 1) reg.CALIBRATION_ACTIVATE_AND_STATUS_FIELDS.CS1_CAL = 0x01;
	else if (id == 2) reg.CALIBRATION_ACTIVATE_AND_STATUS_FIELDS.CS2_CAL = 0x01;
	else if (id == 3) reg.CALIBRATION_ACTIVATE_AND_STATUS_FIELDS.CS3_CAL = 0x01;
	writeRegister(CALIBRATION_ACTIVATE_AND_STATUS, reg.CALIBRATION_ACTIVATE_AND_STATUS_COMBINED);
}

void CAP1293::calibrateAll(){
	CALIBRATION_ACTIVATE_AND_STATUS_REG reg;
    reg.CALIBRATION_ACTIVATE_AND_STATUS_COMBINED = readRegister(CALIBRATION_ACTIVATE_AND_STATUS);
	reg.CALIBRATION_ACTIVATE_AND_STATUS_FIELDS.CS1_CAL = 0x01;
	reg.CALIBRATION_ACTIVATE_AND_STATUS_FIELDS.CS2_CAL = 0x01;
	reg.CALIBRATION_ACTIVATE_AND_STATUS_FIELDS.CS3_CAL = 0x01;
	writeRegister(CALIBRATION_ACTIVATE_AND_STATUS, reg.CALIBRATION_ACTIVATE_AND_STATUS_COMBINED);
}

void CAP1293::setInterruptDisabled()
{
    INTERRUPT_ENABLE_REG reg;
    reg.INTERRUPT_ENABLE_COMBINED = readRegister(INTERRUPT_ENABLE);
    reg.INTERRUPT_ENABLE_FIELDS.CS1_INT_EN = 0x00;
    reg.INTERRUPT_ENABLE_FIELDS.CS2_INT_EN = 0x00;
    reg.INTERRUPT_ENABLE_FIELDS.CS3_INT_EN = 0x00;
    writeRegister(INTERRUPT_ENABLE, reg.INTERRUPT_ENABLE_COMBINED);
}

void CAP1293::setInterruptEnabled()
{
    INTERRUPT_ENABLE_REG reg;
    reg.INTERRUPT_ENABLE_COMBINED = readRegister(INTERRUPT_ENABLE);
    reg.INTERRUPT_ENABLE_FIELDS.CS1_INT_EN = 0x01;
    reg.INTERRUPT_ENABLE_FIELDS.CS2_INT_EN = 0x01;
    reg.INTERRUPT_ENABLE_FIELDS.CS3_INT_EN = 0x01;
    writeRegister(INTERRUPT_ENABLE, reg.INTERRUPT_ENABLE_COMBINED);
}

void CAP1293::enableSensing(uint8_t id){
	SENSOR_INPUT_ENABLE_REG reg;
	reg.SENSOR_INPUT_ENABLE_COMBINED = readRegister(SENSOR_INPUT_ENABLE);
	if (id == 1) reg.SENSOR_INPUT_ENABLE_FIELDS.CS1_EN = 0x01;
	else if (id == 2) reg.SENSOR_INPUT_ENABLE_FIELDS.CS2_EN = 0x01;
	else if (id == 3) reg.SENSOR_INPUT_ENABLE_FIELDS.CS3_EN = 0x01;
	writeRegister(SENSOR_INPUT_ENABLE, reg.SENSOR_INPUT_ENABLE_COMBINED);
}

void CAP1293::disableSensing(uint8_t id){
	SENSOR_INPUT_ENABLE_REG reg;
	reg.SENSOR_INPUT_ENABLE_COMBINED = readRegister(SENSOR_INPUT_ENABLE);
	if (id == 1) reg.SENSOR_INPUT_ENABLE_FIELDS.CS1_EN = 0x00;
	else if (id == 2) reg.SENSOR_INPUT_ENABLE_FIELDS.CS2_EN = 0x00;
	else if (id == 3) reg.SENSOR_INPUT_ENABLE_FIELDS.CS3_EN = 0x00;
	writeRegister(SENSOR_INPUT_ENABLE, reg.SENSOR_INPUT_ENABLE_COMBINED);
}

bool CAP1293::isEnabledSensing(uint8_t id){
	SENSOR_INPUT_ENABLE_REG reg;
	reg.SENSOR_INPUT_ENABLE_COMBINED = readRegister(SENSOR_INPUT_ENABLE);
	if (id == 1) return (reg.SENSOR_INPUT_ENABLE_FIELDS.CS1_EN == 0x01);
	else if (id == 2) return (reg.SENSOR_INPUT_ENABLE_FIELDS.CS2_EN == 0x01);
	else if (id == 3) return (reg.SENSOR_INPUT_ENABLE_FIELDS.CS3_EN == 0x01);
	
	return false;
}

void CAP1293::enableSignalGuard(){
	disableSensing(2);
	SIGNAL_GUARD_ENABLE_REG reg;
	reg.SIGNAL_GUARD_ENABLE_COMBINED = readRegister(SIGNAL_GUARD_ENABLE);
	reg.SIGNAL_GUARD_ENABLE_FIELDS.CS1_SG_EN = 0x01;
	reg.SIGNAL_GUARD_ENABLE_FIELDS.CS3_SG_EN = 0x01;
	writeRegister(SIGNAL_GUARD_ENABLE, reg.SIGNAL_GUARD_ENABLE_COMBINED);	
	_singalGuardEnabled = true;
}

void CAP1293::disableSignalGuard(){
	SIGNAL_GUARD_ENABLE_REG reg;
	reg.SIGNAL_GUARD_ENABLE_COMBINED = readRegister(SIGNAL_GUARD_ENABLE);
	reg.SIGNAL_GUARD_ENABLE_FIELDS.CS1_SG_EN = 0x00;
	reg.SIGNAL_GUARD_ENABLE_FIELDS.CS3_SG_EN = 0x00;
	writeRegister(SIGNAL_GUARD_ENABLE, reg.SIGNAL_GUARD_ENABLE_COMBINED);	
	enableSensing(2);
	_singalGuardEnabled = false;
}

bool CAP1293::isInterruptEnabled()
{
    INTERRUPT_ENABLE_REG reg;
    reg.INTERRUPT_ENABLE_COMBINED = readRegister(INTERRUPT_ENABLE);
    if (reg.INTERRUPT_ENABLE_FIELDS.CS1_INT_EN == 0x01 && reg.INTERRUPT_ENABLE_FIELDS.CS2_INT_EN == 0x01 && reg.INTERRUPT_ENABLE_FIELDS.CS3_INT_EN == 0x01)
    {
        return true;
    }
    return false;
}

void CAP1293::setSensitivity(uint8_t sensitivity)
{
    SENSITIVITY_CONTROL_REG reg;
    reg.SENSITIVITY_CONTROL_COMBINED = readRegister(SENSITIVITY_CONTROL);
    if (sensitivity == SENSITIVITY_128X)
    {
        reg.SENSITIVITY_CONTROL_FIELDS.DELTA_SENSE = SENSITIVITY_128X;
    }
    else if (sensitivity == SENSITIVITY_64X)
    {
        reg.SENSITIVITY_CONTROL_FIELDS.DELTA_SENSE = SENSITIVITY_64X;
    }
    else if (sensitivity == SENSITIVITY_32X)
    {
        reg.SENSITIVITY_CONTROL_FIELDS.DELTA_SENSE = SENSITIVITY_32X;
    }
    else if (sensitivity == SENSITIVITY_16X)
    {
        reg.SENSITIVITY_CONTROL_FIELDS.DELTA_SENSE = SENSITIVITY_16X;
    }
    else if (sensitivity == SENSITIVITY_8X)
    {
        reg.SENSITIVITY_CONTROL_FIELDS.DELTA_SENSE = SENSITIVITY_8X;
    }
    else if (sensitivity == SENSITIVITY_4X)
    {
        reg.SENSITIVITY_CONTROL_FIELDS.DELTA_SENSE = SENSITIVITY_4X;
    }
	else if (sensitivity == SENSITIVITY_2X)
    {
        reg.SENSITIVITY_CONTROL_FIELDS.DELTA_SENSE = SENSITIVITY_2X;
    }
    else if (sensitivity == SENSITIVITY_1X)
    {
        reg.SENSITIVITY_CONTROL_FIELDS.DELTA_SENSE = SENSITIVITY_1X;
    }
    else
    {
        reg.SENSITIVITY_CONTROL_FIELDS.DELTA_SENSE = SENSITIVITY_32X;
    }
    writeRegister(SENSITIVITY_CONTROL, reg.SENSITIVITY_CONTROL_COMBINED);
}

uint8_t CAP1293::getSensitivity()
{
    SENSITIVITY_CONTROL_REG reg;
    reg.SENSITIVITY_CONTROL_COMBINED = readRegister(SENSITIVITY_CONTROL);
    uint16_t sensitivity = reg.SENSITIVITY_CONTROL_FIELDS.DELTA_SENSE;
    if (sensitivity == SENSITIVITY_128X)
    {
        return 128;
    }
    else if (sensitivity == SENSITIVITY_64X)
    {
        return 64;
    }
    else if (sensitivity == SENSITIVITY_32X)
    {
        return 32;
    }
    else if (sensitivity == SENSITIVITY_16X)
    {
        return 16;
    }
    else if (sensitivity == SENSITIVITY_8X)
    {
        return 8;
    }
    else if (sensitivity == SENSITIVITY_4X)
    {
        return 4;
    }
    else if (sensitivity == SENSITIVITY_2X)
    {
        return 2;
    }
    else if (sensitivity == SENSITIVITY_1X)
    {
        return 1;
    }
    else
    {
        // Error - no possible register value (pg. 25)
        return 0;
    }
}

bool CAP1293::is1Touched()
{
    SENSOR_INPUT_STATUS_REG reg;
    reg.SENSOR_INPUT_STATUS_COMBINED = readRegister(SENSOR_INPUT_STATUS);

    // Touch detected
    if (reg.SENSOR_INPUT_STATUS_FIELDS.CS1 == ON)
    {
        clearInterrupt();
        return true;
    }
    return false;
}

bool CAP1293::is2Touched()
{
	if(_singalGuardEnabled) return false;
    SENSOR_INPUT_STATUS_REG reg;
    reg.SENSOR_INPUT_STATUS_COMBINED = readRegister(SENSOR_INPUT_STATUS);

    // Touch detected
    if (reg.SENSOR_INPUT_STATUS_FIELDS.CS2 == ON)
    {
        clearInterrupt();
        return true;
    }
    return false;
}

bool CAP1293::is3Touched()
{
    SENSOR_INPUT_STATUS_REG reg;
    reg.SENSOR_INPUT_STATUS_COMBINED = readRegister(SENSOR_INPUT_STATUS);

    // Touch detected
    if (reg.SENSOR_INPUT_STATUS_FIELDS.CS3 == ON)
    {
        clearInterrupt();
        return true;
    }
    return false;
}

bool CAP1293::isTouched()
{
    GENERAL_STATUS_REG reg;
    reg.GENERAL_STATUS_COMBINED = readRegister(GENERAL_STATUS);

    // Touch detected
    if (reg.GENERAL_STATUS_FIELDS.TOUCH == ON)
    {
        clearInterrupt();
        return true;
    }
    return false;
}

bool CAP1293::isMTPTouched(){
	GENERAL_STATUS_REG reg;
    reg.GENERAL_STATUS_COMBINED = readRegister(GENERAL_STATUS);
	if(reg.GENERAL_STATUS_FIELDS.MTP == 0x01){
		clearInterrupt();
		return true;
	}
	return false;
}

bool CAP1293::isTouched(uint8_t id){
	if (id == 1) return is1Touched();
	else if (id == 2) return is2Touched();
	else if (id == 3) return is3Touched();	
	else return false;
}
// dodano VM
uint8_t CAP1293::getInputStatus(){
	uint8_t r=readRegister(SENSOR_INPUT_STATUS);
	clearInterrupt();
	return r;
}
uint8_t CAP1293::getGeneralStatus(){
	return readRegister(GENERAL_STATUS);
}
uint8_t CAP1293::getMainControl(){
	return readRegister(MAIN_CONTROL);
}

// kraj dodavanja VM

/* READ A SINGLE REGISTER
    Read a single byte of data from the CAP1293 register "reg"
*/
byte CAP1293::readRegister(CAP1293_Register reg)
{
    _i2cPort->beginTransmission(_deviceAddress);
    _i2cPort->write(reg);
    _i2cPort->endTransmission(false);               // endTransmission but keep the connection active
    _i2cPort->requestFrom(_deviceAddress, (byte)1); // Ask for 1 byte, once done, bus is released by default

    // Wait for the data to come back
    if (_i2cPort->available())
    {
        return _i2cPort->read(); // Return this one byte
    }
    else
    {
        return 0;
    }
}

/* READ MULTIPLE REGISTERS
    Read "en" bytes from the CAP1293m, starting at register "reg." Bytes are 
    stored in "buffer" on exit.
*/
void CAP1293::readRegisters(CAP1293_Register reg, byte *buffer, byte len)
{
    _i2cPort->beginTransmission(_deviceAddress);
    _i2cPort->write(reg);
    _i2cPort->endTransmission(false);           // endTransmission but keep the connection active
    _i2cPort->requestFrom(_deviceAddress, len); // Ask for bytes, once done, bus is released by default

    // Wait for data to come back
    if (_i2cPort->available() == len)
    {
        // Iterate through data from buffer
        for (int i = 0; i < len; i++)
            buffer[i] = _i2cPort->read();
    }
}

/* WRITE TO A SINGLE REGISTER
    Wire a single btyte of data to a register in CAP1293
*/
void CAP1293::writeRegister(CAP1293_Register reg, byte data)
{
    writeRegisters(reg, &data, 1);
}

/* WRITE TO MULTIPLE REGISTERS
    Write an array of "len" bytes ("buffer"), starting at register "reg,"
    and auto-incementing to the next
*/
void CAP1293::writeRegisters(CAP1293_Register reg, byte *buffer, byte len)
{
    _i2cPort->beginTransmission(_deviceAddress);
    _i2cPort->write(reg);
    for (int i = 0; i < len; i++)
        _i2cPort->write(buffer[i]);
    _i2cPort->endTransmission(); // Stop transmitting
}