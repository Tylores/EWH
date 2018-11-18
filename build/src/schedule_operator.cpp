#include <iostream>
#include <string>
#include <vector>
#include "schedule_operator.h"
#include "tsu.h"

ScheduleOperator::ScheduleOperator (std::string input, ElectricWaterHeater *ewh) : file_name_(input), ptr_(ewh), index_(0){
	schedule_ = tsu::FileToMatrix(file_name_, ',', 4);
}

ScheduleOperator::~ScheduleOperator () {};

void ScheduleOperator::Idle () {
	ptr_->SetImportWatts(0);
};

void ScheduleOperator::Export (std::string amount){
	ptr_->SetImportWatts(0);
};

void ScheduleOperator::Import (std::string amount){
	int watts = std::stoi(amount);
	ptr_->SetImportWatts(watts);
};

void ScheduleOperator::Loop () {
	std::string Op_command;
	std::string Op_amount;

	time_t now = time(0);
	now = now % 86400;
	if (now == 0) {
		index_ = 0;
	}

	for (int i = index_; i < schedule_.size(); i++) {
		if (std::stoi(schedule_[i][0]) % 86400 == now) {
			Op_command = schedule_[i][1];
			Op_amount = schedule_[i][2];
			index_ = i;
			break;
		}
	}

	
	if(Op_command == "idle") {
		ScheduleOperator::Idle ();
	} else if (Op_command == "import") {
		ScheduleOperator::Import (Op_amount);
	} else if (Op_command == "export") {
		ScheduleOperator::Export (Op_amount);
	} else {
		printf("Error. No grid command identified.\n");
	}
};

