#ifndef ELECTRIC_WATER_HEATER_H_
#define ELECTRIC_WATER_HEATER_H_

#include "universal_control_module.h"
#include <string>
#include <cea2045/device/DeviceFactory.h>
#include <cea2045/communicationport/CEA2045SerialPort.h> 

class ElectricWaterHeater {
public:
	// Constructor/Destructor
	ElectricWaterHeater ();
	virtual ~ElectricWaterHeater ();

	// Get Methods
	unsigned int GetImportPower ();
	unsigned int GetImportEnergy ();


	// Set Methods
	void SetImportWatts (unsigned int watts);


	// Control
	void Loop ();
	void Print ();

public:
	// structures
	enum OpState {
		IDLE, NORMAL, CURTAILED, HEIGHTENED, GRID, SGD_ERROR,
	};

private:
	void UpdateCommodityData ();

private:
	// Composition
	// - These are objects that a smart electric water heater will "have" and 
	// - cannot exist without. 
	UniversalControlModule ucm_;
	cea2045::ResponseCodes response_codes_;
	cea2045::CEA2045SerialPort sp_;
	cea2045::ICEA2045DeviceUCM *device_ptr_;

private:
	unsigned int import_watts_;
	unsigned int import_power_;
	unsigned int import_energy_;


};

#endif  // end ELECTRIC_WATER_HEATER_H_