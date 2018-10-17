#include <iostream>
#include <sstream>
#include <chrono>
#include "include/electric_water_heater.h"
#include "include/tsu.h"
#include "include/logger.h"

// MACROS
#define DEBUG(x) std::cout << x << std::endl

ElectricWaterHeater::ElectricWaterHeater () : sp_("/dev/ttyUSB0"), heartbeat_(1), log_second_(0) {
	if (!sp_.open ()) {
		Logger("ERROR") << "failed to open serial port: " << strerror(errno);
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

	Logger("INFO") << "startup complete";

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
	if (watts > 0) {
		Logger("INFO") << "Import command received";
		opstate_ = 1;
	} else {
		Logger("INFO") << "Shed command received";
		opstate_ = 2;
	}
	import_watts_ = watts;
}  // end Set Import Watts

// Begin critical peak event status
void ElectricWaterHeater::SetCriticalPeak () {
	device_ptr_->basicCriticalPeakEvent (0);
	opstate_ = 4;
	Logger("INFO") << "Critical peak event command received";
}  /// end critical peak event status change

// Begin load up status
void ElectricWaterHeater::SetLoadUp () {
	opstate_ = 3;
	device_ptr_->basicLoadUp (0);
	Logger("INFO") << "Load up command received";
}  /// end load up command

// Begin Grid Emergency status
void ElectricWaterHeater::SetGridEmergency () {
	opstate_ = 5;
	device_ptr_->basicGridEmergency (0);
	Logger("INFO") << "Grid Emergency command received";
}  /// end grid emergency command

// EXPERIMENTAL: stop reheating by sending import and then export command
void ElectricWaterHeater::StopReheat () {
	device_ptr_->basicEndShed (0);
	device_ptr_->basicShed (0);
	opstate_ = 2;
	Logger("INFO") << "Stop Reheat command received";
}  ///

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
	if (tsu::GetMinutes() % heartbeat_ == 0 && tsu::GetSeconds() < 1){
		device_ptr_->basicOutsideCommConnectionStatus(
			cea2045::OutsideCommuncatonStatusCode::Found
		);
	}

	// log every minuteish, there will be some double logs here and there
	unsigned int sec = tsu::GetSeconds ();
	if (sec % 2 == 0 && log_second_ != sec){
		ElectricWaterHeater::Log ();
		log_second_ = sec;
	}
	if (opstate_ == 1) {
		device_ptr_->basicEndShed (0);
	} else if (opstate_ == 2) {
		device_ptr_->basicShed (0);
	}
}  // end Loop

void ElectricWaterHeater::Print () {
	std::cout << "Power:\t\t\t" << import_power_ << "\twatts\n";
	std::cout << "Import:\t\t\t" << import_watts_ << "\twatts\n";
	std::cout << "Energy Available:\t" << import_energy_ << "\twatt-hours\n";
	std::cout << "Energy Total:\t\t" << energy_total_ << "\twatt-hours\n";
	std::cout << "State:\t\t\t" << ucm_.GetOpState () << "\n";
	std::cout << "Operation State:\t" << opstate_ << "\n";
}  // end Print

void ElectricWaterHeater::Log () {
	Logger ("DATA") 
		<<  import_power_ << "\t"
		<< import_energy_ << "\t" 
		<< energy_total_ << "\t"
		<< ucm_.GetOpState () << "\t"
		<< import_watts_ << "\t"
		<< opstate_;
}  // end Log
