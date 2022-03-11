#include <chrono>
#include <ctime>
#include "Timer.h"
#include <iostream>


std::time_t string2timestamp(const std::string& str)
{
    struct tm tm;
    //setlocale(LC_ALL, "/QSYS.LIB/EN_US.LOCALE");
    //std::cout << "---" <<str.c_str() << std::endl;
    ::strptime(str.c_str(), "%Y%m%d %H:%M:%S", &tm);
    return mktime(&tm);
}

Timer::Timer(std::string start_time, std::string end_time)
{
    std::chrono::duration<int, std::ratio<60 * 60 * 24> > one_day(1);
    std::chrono::system_clock::time_point today = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point tomorrow = today + one_day;

    std::time_t tt;
    char date[30] = "";
    tt = std::chrono::system_clock::to_time_t(today);
    struct tm * ptm =  localtime(&tt);
    m_weekday = ptm->tm_wday;
    std::strftime(date, sizeof(date), "%Y%m%d", std::localtime(&tt));
    m_str_today = date;
    tt = std::chrono::system_clock::to_time_t(tomorrow);
    std::strftime(date, sizeof(date), "%Y%m%d", std::localtime(&tt));
    m_str_tomorrow = date;
    m_str_starttime = m_str_today + " " + start_time;
    m_str_endtime = m_str_tomorrow + " " + end_time;

    //std::cout << "+++" << m_str_starttime << std::endl;
    //std::cout << "+++" << m_str_endtime << std::endl;
    m_start_time = string2timestamp(m_str_starttime);
    m_end_time = string2timestamp(m_str_endtime);

}