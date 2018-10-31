#ifndef SCHEDULE_OPERATOR_H_
#define SCHEDULE_OPERATOR_H_
#include "tsu.h"
#include "electric_water_heater.h"
#include <string>

class ScheduleOperator {
public:
	ScheduleOperator (std::string input, ElectricWaterHeater *ewh);
	virtual ~ScheduleOperator ();

	ElectricWaterHeater *ptr_;
	tsu::string_matrix schedule_;

	void Idle ();
	void Export (std::string amount);
	void Import (std::string amount);
	void Loop ();

private:
	std::string file_name_;
	unsigned int index_;
};

#endif