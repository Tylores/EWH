#include <iostream>
#include <sstream>
#include <chrono>
#include "include/electric_water_heater.h"
#include "include/utility.h"
#include "include/easylogging++.h"

// MACROS
#define DEBUG(x) std::cout << x << std::endl

ElectricWaterHeater::ElectricWaterHeater () : sp_("/dev/ttyUSB0"), heartbeat_(1) {
	if (!sp_.open ()) {
		LOG(ERROR) << "failed to open serial port: " << strerror(errno);
		exit (1);
	} else {
		device_ptr_ = cea2045::DeviceFactory::createUCM(&sp_, &ucm_);
		device_ptr_->start ();
		device_ptr_->basicOutsideCommConnectionStatus(
			cea2045::OutsideCommuncatonStatusCode::Found
		);
	}
	response_codes_ = device_ptr_->querySuportDataLinkMessages().get();
	response_codes_ = device_ptr_->queryMaxPayload().get();
	response_codes_ = device_ptr_->querySuportIntermediateMessages().get();
	response_codes_ = device_ptr_->intermediateGetDeviceInformation().get();

	LOG (INFO) << "startup complete";

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
		} else if (commodity.code == 6) {
			energy_total_ = commodity.cumulative;
		} else if (commodity.code == 7) {
			import_energy_ = commodity.cumulative;
		}
	}
}  // end Update Commodity Data

// Loop
// - 
void ElectricWaterHeater::Loop () {
	ElectricWaterHeater::UpdateCommodityData ();

	// if modulus is = 0, then the minute will be equal to the heardbeat
	// increment. Then only send the signal while the time is at the start 
	// of the minute
	if (tsu::GetMinutes() % heartbeat_ == 0 && tsu::GetSeconds() < 2){
		device_ptr_->basicOutsideCommConnectionStatus(
			cea2045::OutsideCommuncatonStatusCode::Found
		);
	}

	// log every minute
	if (tsu::GetSeconds() < 2){
		ElectricWaterHeater::Log ();
	}
	if (import_watts_ > 0 && import_energy_ > 0) {
		device_ptr_->basicEndShed (0);
	} else {
		device_ptr_->basicShed (0);
	}
}  // end Loop

void ElectricWaterHeater::Print () {
	std::cout << "Power:\t\t\t" << import_power_ << "\twatts\n";
	std::cout << "Energy Available:\t" << import_energy_ << "\twatt-hours\n";
	std::cout << "Energy Total:\t\t" << energy_total_ << "\twatt-hours\n";
}  // end Print

void ElectricWaterHeater::Log () {
	LOG(INFO) << "power = " <<  import_power_ << "\tenergy take = "
		<< import_energy_ << "\tenergy total = " << energy_total_
		<< "\tstate = " << ucm_.GetOpState ();
}  // end Log