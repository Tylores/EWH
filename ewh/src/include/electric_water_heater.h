#ifndef ELECTRIC_WATER_HEATER_H_
#define ELECTRIC_WATER_HEATER_H_

#include "include/universal_control_module.h"
#include "include/easylogging++.h"

#include <cea2045/device/DeviceFactory.h>
#include <cea2045/communicationport/CEA2045SerialPort.h> 

class ElectricWaterHeater {
public:
	// Constructor/Destructor
	ElectricWaterHeater ();
	virtual ~ElectricWaterHeater ();

	// Get Methods


	// Set Methods


	// Control
	void Loop ();

private:
	void UpdateCommodityData ();
	void ImportPower ();


private:
	// composition
	UniversalControlModule ucm_;
	cea2045::CEA2045SerialPort sp_("/dev/ttyUSB0");
	cea2045::ResponseCodes response_codes_;
	cea2045::ICEA2045DeviceUCM *device_ptr_;

private:
	unsigned int import_watts_;
	unsigned int import_power_;
	unsigned int import_energy_;


};

#endif  // end ELECTRIC_WATER_HEATER_H_