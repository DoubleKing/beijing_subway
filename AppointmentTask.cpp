#include "AppointmentTask.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <glog/logging.h>

#include "nlohmann/json.hpp"
#include "curl/curl.h"

using json = nlohmann::json;


size_t writeToStringForCurl(void* ptr, size_t size, size_t count, void* stream)
{
    ((std::string*)stream)->append((char*)ptr, size * count);
    return size * count;
}

static size_t header_callback(char* buffer, size_t size, size_t nitems, void* userdata)
{
    std::string* headers = (std::string*) userdata;
    headers->append(buffer, nitems * size);
    return nitems * size;
}


static
int my_trace(CURL* handle, curl_infotype type,
    char* data, size_t size,
    void* userp)
{
    const char* text;
    (void)handle; /* prevent compiler warning */

    switch (type) {
    case CURLINFO_TEXT:
        //fprintf(stderr, "== Info: %s", data);
        /* FALLTHROUGH */
    default: /* in case a new one is introduced to shock us */
        return 0;

    case CURLINFO_HEADER_OUT:
        text = "=> Send header";
        break;
    //case CURLINFO_DATA_OUT:
    //    text = "=> Send data";
    //    break;
    case CURLINFO_SSL_DATA_OUT:
        text = "=> Send SSL data";
        break;
    case CURLINFO_HEADER_IN:
        text = "<= Recv header";
        break;
    //case CURLINFO_DATA_IN:
    //    text = "<= Recv data";
    //    break;
    case CURLINFO_SSL_DATA_IN:
        text = "<= Recv SSL data";
        break;
    }

    LOG(INFO) << text <<  " " << data;
    return 0;
}

void AppointmentTask::execute()
{
	while (1)
	{
		time_t now = time(NULL);
		if (now > end_time)
		{
			break;
		}
		if (now < start_time)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			continue;
		}

        CURL* curl = curl_easy_init();
        std::string response;
        std::string responseHeader;

        struct curl_slist* http_header = NULL;

        http_header = curl_slist_append(http_header, "user-agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.93 Safari/537.36");
        std::string auth_header = std::string("authorization: ") + auth;
        http_header = curl_slist_append(http_header, auth_header.c_str());
        http_header = curl_slist_append(http_header, "accept: application/json, text/plain, */*");
        http_header = curl_slist_append(http_header, "accept-encoding: gzip, deflate, br");
        http_header = curl_slist_append(http_header, "accept-language: zh-CN,zh;q=0.9");
        http_header = curl_slist_append(http_header, "content-type: application/json;charset=UTF-8");
        http_header = curl_slist_append(http_header, "origin: https://webui.mybti.cn");
        http_header = curl_slist_append(http_header, "referer: https://webui.mybti.cn/");
        http_header = curl_slist_append(http_header, "sec-ch-ua: \" Not A; Brand\";v=\"99\", \"Chromium\";v=\"96\", \"Google Chrome\";v=\"96\"");
        http_header = curl_slist_append(http_header, "sec-ch-ua-mobile: ?0");
        http_header = curl_slist_append(http_header, "sec-ch-ua-platform: \"Windows\"");
        http_header = curl_slist_append(http_header, "sec-fetch-dest: empty");
        http_header = curl_slist_append(http_header, "sec-fetch-mode: cors");
        http_header = curl_slist_append(http_header, "sec-fetch-site: same-site");

        json jData;
        jData["lineName"] = lineName;
        jData["snapshotWeekOffset"] = 0;
        jData["stationName"] = stationName;
        jData["enterDate"] = date;
        jData["snapshotTimeSlot"] = "0630-0930";
        jData["timeSlot"] = timeSlot;
        std::string body = jData.dump(0, ' ', false, nlohmann::detail::error_handler_t::replace);

        curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, my_trace);
        curl_easy_setopt(curl, CURLOPT_DEBUGDATA, NULL);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        /* send all data to this function  */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToStringForCurl);
        /* we pass our 'chunk' struct to the callback function */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);



        curl_easy_setopt(curl, CURLOPT_URL, "https://webapi.mybti.cn/Appointment/CreateAppointment");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(body.c_str()));
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_header);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L);
        //curl_easy_setopt(curl, CURLOPT_HEADER, 1);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 500);
        curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1L);

        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &(responseHeader));

        //auto start = std::chrono::system_clock::now();
        //std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count() << std::endl;

        CURLcode res = curl_easy_perform(curl);

        //auto end = std::chrono::system_clock::now();
        //std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end.time_since_epoch()).count() << std::endl;

        curl_easy_cleanup(curl);
        curl_slist_free_all(http_header);
       
        json jRes = json::parse(response, nullptr, false);


        if (res != CURLE_OK)
        {
            std::cout << (char*)curl_easy_strerror(res) << std::endl;
            LOG(INFO) << "Request Error:" << (char*)curl_easy_strerror(res);
        }


        if (!jRes.is_discarded() && jRes["balance"].is_number_integer() && jRes["balance"] == -1024)
        {
            LOG(INFO) << "Task Stop : " << date << "   " << timeSlot << auth;
            return;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

    return;
}
