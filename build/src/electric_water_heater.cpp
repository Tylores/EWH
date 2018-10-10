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
		critical_peak_ = false;
		load_up_ = false;
	} else {
		Logger("INFO") << "Shed command received";
		critical_peak_ = false;
		load_up_ = false;
	}
	import_watts_ = watts;
}  // end Set Import Watts

// Begin critical peak event status
void ElectricWaterHeater::SetCriticalPeak () {
	device_ptr_->basicCriticalPeakEvent (0);
	critical_peak_ = true;
	load_up_ = false;
	Logger("INFO") << "Critical peak event command received";
}  /// end critical peak event status change

// Begin load up status
void ElectricWaterHeater::SetLoadUp () {
	device_ptr_->basicLoadUp (0);
	load_up_ = true;
	Logger("INFO") << "Load up command received";
	critical_peak_ = false;
}  /// end load up command

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
	if (import_watts_ > 0 && import_energy_ > 0 && load_up_ == false && critical_peak_ == false) {
		device_ptr_->basicEndShed (0);
	} else if (critical_peak_ == false && load_up_ == false) {
		device_ptr_->basicShed (0);
	}
}  // end Loop

void ElectricWaterHeater::Print () {
	std::cout << "Power:\t\t\t" << import_power_ << "\twatts\n";
	std::cout << "Import:\t\t\t" << import_watts_ << "\twatts\n";
	std::cout << "Energy Available:\t" << import_energy_ << "\twatt-hours\n";
	std::cout << "Energy Total:\t\t" << energy_total_ << "\twatt-hours\n";
	std::cout << "State:\t\t\t" << ucm_.GetOpState () << "\n";
	std::cout << "Critical Peak Event:\t" << critical_peak_ << "\n";
	std::cout << "Loading up:\t\t" << load_up_ << "\n";
}  // end Print

void ElectricWaterHeater::Log () {
	Logger ("DATA") 
		<<  import_power_ << "\t"
		<< import_energy_ << "\t" 
		<< energy_total_ << "\t"
		<< ucm_.GetOpState () << "\t"
		<< import_watts_;
}  // end Log
