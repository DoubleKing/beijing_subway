#pragma once

#include <string>
#include <vector>



class Config
{
	struct account
	{
		std::string phone;
		std::string auth;
		std::vector<std::string> timeSlots;
		std::string lineName;
		std::string stationName;
	};
public:
	std::string start_time;
	std::string next_day_end_time;
	std::vector<account> accounts;
public:
	int ConfigInit();

};

