#include "include/electric_water_heater.h"
#include "include/easylogging++.h"

ElectricWaterHeater::ElectricWaterHeater () : 
	sp_ptr_(new cea2045::CEA2045SerialPort("/dev/ttyUSB0")) {

	if (!sp_ptr_->open ()) {
		LOG(ERROR) << "failed to open serial port: " << strerror(errno);
		exit (1);
	} else {
		device_ptr_ = cea2045::DeviceFactory::createUCM(sp_ptr_, &ucm_);
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
	delete sp_ptr_;
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
		device_ptr_->basicShed (0);
	}
}  // end Loop

void ElectricWaterHeater::Print () {
	LOG(INFO) << "Import Power: " << import_power_ << " watts";
	LOG(INFO) << "Import Energy: " << import_energy_ << "watt-hours";
}  // end Print
