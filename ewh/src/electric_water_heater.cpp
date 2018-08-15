#include "include/electric_water_heater.h"

ElectricWaterHeater::ElectricWaterHeater () {
	if (!sp.open ()) {
		LOG(ERROR) << "failed to open serial port: " << strerror(errno);
		return 0;
	} else {
		device_ptr_ = new DeviceFactory::createUCM(&sp_, &ucm_);
		device_ptr_->start();
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

// Update Commodity Data
// - 
void ElectricWaterHeater::UpdateCommodityData () {
	response_codes_ = device_ptr_->GetCommodity ();
	std::vector <CommodityData> commodities = ucm_.GetCommodityData ();
	for (const auto &commodity : commodities) {
		if (commodity.code == 0) {
			import_power_ = commodity.rate;
		} else if (commodity.code == 7) {
			import_energy_ = commodity.cumulative;
		}
	}
}  // end Update Commodity Data


