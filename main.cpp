#include <cstdio>
#include <iostream>
#include <curl/curl.h>
#include <glog/logging.h>
#include "Timer.h"
#include "Config.h"
#include "dynamic_thread_pool.h"
#include "AppointmentTask.h"

int main(int argc ,char * argv[])
{
    google::InitGoogleLogging(argv[0]);
    FLAGS_max_log_size = 100;  // Set max log file size
    FLAGS_stop_logging_if_full_disk = true;
    DynamicThreadPool threadpool(6);
    std::string today;
    while (1)
    {
        Config conf;
        conf.ConfigInit();
        Timer timer(conf.start_time, conf.next_day_end_time);
        if (timer.m_weekday != 5 && timer.m_weekday != 6)
        {
            if (timer.m_str_today != today)
            {
                for (unsigned int i = 0; i < conf.accounts.size(); i++)
                {
                    AppointmentTask task;
                    task.start_time = timer.m_start_time;
                    task.end_time = timer.m_end_time;
                    task.auth = conf.accounts[i].auth;
                    task.date = timer.m_str_tomorrow;
                    task.lineName = conf.accounts[i].lineName;
                    task.stationName = conf.accounts[i].stationName;
                    for (unsigned int j = 0; j < conf.accounts[i].timeSlots.size(); j++)
                    {
                        task.timeSlot = conf.accounts[i].timeSlots[j];
                        threadpool.Add([=]()mutable {task.execute(); });
                        LOG(INFO) << "Start Task : " << task.date << "   " << task.timeSlot << task.auth ;
                    }
                }
                today = timer.m_str_today;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    }
    return 0;
}
