#pragma once

#include <string>
#include <ctime>

class Timer
{
public:
	Timer(std::string start_time, std::string end_time);
public:
	std::string m_str_today;
	std::string m_str_tomorrow;
	std::string m_str_starttime;
	std::string m_str_endtime;
	std::time_t m_start_time;
	std::time_t m_end_time;
	int         m_weekday;

};

