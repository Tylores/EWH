#include <iostream>
#include "include/electric_water_heater.h"
#include "include/easylogging++.h"

ElectricWaterHeater::ElectricWaterHeater () : sp_("/dev/ttyUSB0") {
	if (!sp_.open ()) {
		LOG(ERROR) << "failed to open serial port: " << strerror(errno);
		exit (1);
	} else {
		device_ptr_ = cea2045::DeviceFactory::createUCM(&sp_, &ucm_);
		device_ptr_->start ();
	}
	response_codes_ = device_ptr_->querySuportDataLinkMessages().get();
	response_codes_ = device_ptr_->queryMaxPayload().get();
	response_codes_ = device_ptr_->querySuportIntermediateMessages().get();
	response_codes_ = device_ptr_->intermediateGetDeviceInformation().get();
	LOG(INFO) << "startup complete";

}  // end Constructor

ElectricWaterHeater::~ElectricWaterHeater () {
	delete device_ptr_;
}  // end Destructor

// Get Import Power
// -
unsigned int ElectricWaterHeater::GetImportPower () {
	return import_power_;
}  // end Get Import Power

// Get Import Energy
// -
unsigned int ElectricWaterHeater::GetImportEnergy () {
	return import_energy_;
}  // end Get Import Energy

// Set Import Watts
// -
void ElectricWaterHeater::SetImportWatts (unsigned int watts) {
	import_watts_ = watts;
}  // end Set Import Watts

// Update Commodity Data
// - 
void ElectricWaterHeater::UpdateCommodityData () {
	device_ptr_->intermediateGetCommodity ();
	std::vector <CommodityData> commodities = ucm_.GetCommodityData ();
	for (const auto &commodity : commodities) {
		if (commodity.code == 0) {
			import_power_ = commodity.rate;
		} else if (commodity.code == 7) {
			import_energy_ = commodity.cumulative;
		}
	}
}  // end Update Commodity Data

// Loop
// - 
void ElectricWaterHeater::Loop () {
	ElectricWaterHeater::UpdateCommodityData ();
	device_ptr_->basicQueryOperationalState ();
	if (import_watts_ > 0 
		&& import_energy_ > 0 
		&& ucm_.GetOpState () == OpState::IDLE) {
		device_ptr_->basicEndShed (0);
	} else {
		device_ptr_->basicShed (64);
	}
}  // end Loop

void ElectricWaterHeater::Print () {
	std::cout << "Import Power:\t" << import_power_ << "\twatts\n";
	std::cout << "Import Energy:\t" << import_energy_ << "\twatt-hours\n";
}  // end Print
