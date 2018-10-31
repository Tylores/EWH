#include <iostream>
#include <string>
#include <vector>
#include "schedule_operator.h"
#include "tsu.h"

ScheduleOperator::ScheduleOperator (std::string input, ElectricWaterHeater *ewh) : file_name_(input), ptr_(ewh), index_(0){
	schedule_ = tsu::FileToMatrix(file_name_, ',', 4);
	//tsu::string_matrix data = tsu::FileToMatrix(file_name_, ',' , 4);
	//schedule_ = data;
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
	char time_formatted[100];
	tm now_local = *localtime(&now);
	strftime(time_formatted, sizeof(time_formatted), "%T", &now_local);

	for (int i = 0; i < schedule_.size(); i++) {
		if (schedule_[i][3] == time_formatted) {
			Op_command = schedule_[i][1];
			Op_amount = schedule_[i][2];
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

