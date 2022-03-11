#pragma once

#include <string>

class AppointmentTask
{
public:
	time_t start_time;
	time_t end_time;
	std::string auth;
	std::string date;
	std::string lineName;
	std::string stationName;
	std::string timeSlot;
public:
	void execute();
};

