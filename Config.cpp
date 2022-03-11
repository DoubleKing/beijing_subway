#include <iostream>
#include<fstream>
#include "Config.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

std::vector<std::string> time_point = { "0630" ,"0640" ,"0650" ,
										"0700" ,"0710" ,"0720" ,
										"0730" ,"0740" ,"0750" ,
										"0800" ,"0810" ,"0820" ,
										"0830" ,"0840" ,"0850" ,
										"0910" ,"0920" ,"0930" };


int Config::ConfigInit()
{
	
	std::ifstream read("./conf/config.json");
	json jConfig = json::parse(read);
	if (jConfig.is_discarded())
	{
		//std::cout << "parse json data failed" << std::endl;
		return -1;
	}
	//std::cout << jConfig["start_time"] << std::endl;
	if (jConfig["start_time"].is_string())
	{
		start_time = jConfig["start_time"];
	}
	if (jConfig["next_day_end_time"].is_string())
	{
		next_day_end_time = jConfig["next_day_end_time"];
	}
	if (jConfig["account"].is_array())
	{
		
		std::string timeslot;
		for (unsigned int i = 0; i < jConfig["account"].size(); i++)
		{
			Config::account a;
			a.phone = jConfig["account"][i]["phone"];
			a.auth = jConfig["account"][i]["auth"];
			a.lineName = jConfig["account"][i]["lineName"];
			a.stationName = jConfig["account"][i]["stationName"];
			timeslot = jConfig["account"][i]["timeSlot"];
			std::string start = timeslot.substr(0,4);
			std::string end = timeslot.substr(5, 4);
			for (unsigned int j = 0; j < time_point.size() - 1; j++)
			{
				if (time_point[j] >= start && time_point[j+1] <= end)
				{
					std::string slot = time_point[j] + "-" + time_point[j + 1];
					a.timeSlots.push_back(slot);
				}
			}
			accounts.push_back(a);	
		}
	}

	return 0;
}
