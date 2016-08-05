/*
Copyright Notice / Copying Permission Statement for Expedited Contributions released under GPL 2.0: 
Copyright © 2016 VMware, Inc. All Rights Reserved. 
This program is free software: you can redistribute it and/or modify it under 
the terms of the GNU General Public License version 2, as published by the Free Software Foundation. 
This program is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTIBILITY or FITNESS 
FOR A PARTICULAR PURPOSE. See the GNU General Public License version 2 for more details. 
You should have received a copy of the General Public License version 2 along with this program. 
If not, see https://www.gnu.org/licenses/gpl-2.0.html. 
The full text of the General Public License 2.0 is provided in the COPYING file. 
Some files may be comprised of various open source software components, each of which 
has its own license that is located in the source code of the respective component. 
*/
#include "measure_time.h"
#include <fstream>

#pragma warning(disable : 4996)

namespace MeasureTime {


	TimePoint GetCurrentTime()
	{
		return std::chrono::system_clock::now();
	}


	String TimePointToString(CTimePoint tp)
	{
		struct tm timeinfo;
		char timebuf[26];
#ifdef _WIN32
		localtime_s(&timeinfo, &tp);
		asctime_s(timebuf, 26, &timeinfo);
#else
		localtime_r(&tp, &timeinfo);
		asctime_r(&timeinfo, timebuf);
#endif
		timebuf[24] = '\0';
		return std::move(String(timebuf));
	}

	RecordCenter::RecordCenter()
	{
		//use append mode to open file
		output_file.open(MEASURETIME_OUTPUT_PATH, std::fstream::app);
	}

	RecordCenterPtr& RecordCenter::Instance()
	{
		//global variable
		static RecordCenterPtr center(new RecordCenter);
		return center;
	}

    template<class T>
	void RecordCenter::PrintToFile(const T& obj)
	{
        #ifdef _DEBUG
        PrintToConsole(obj);
        #endif
		if (output_file.is_open())
		{
			output_file << obj << std::endl;
			output_file.flush();
		}
	}

	template<class T>
    void RecordCenter::PrintToConsole(const T& obj)
    {
        std::cout << obj << std::endl;
    }
	bool RecordCenter::Enable()
	{
		return true;
	}

	void RecordCenter::AddTempTimePoint(const String& name, TimePoint&& timepoint)
	{
		temporary_timepoint[name] = timepoint;
	}

	void RecordCenter::RemoveTempTimePoint(const String& name)
	{
		temporary_timepoint.erase(name);
	}

	bool RecordCenter::GetTempTimePoint(const String& name, TimePoint& tp)
	{
		auto t = temporary_timepoint.find(name);
		if (t == temporary_timepoint.end()) return false;
		tp = t->second;
		return true;
	}

	void RecordCenter::RecordEndTimePoint(const String& timepoint_name)
	{
		TimePoint current_timepoint = GetCurrentTime();
		auto& center = RecordCenter::Instance();
		TimePoint start_timepoint;
		bool ret = center->GetTempTimePoint(timepoint_name, start_timepoint);
		if (!ret) {
			center->error() << "[Error] Unable to find start time point for "<< timepoint_name << std::endl;
			return;
		}
		Duration duration = static_cast< MeasureTime::Duration >(std::chrono::duration_cast< std::chrono::milliseconds >(current_timepoint - start_timepoint).count());
		CTimePoint p = std::chrono::system_clock::to_time_t(start_timepoint);
		Record record(Record::NameType::CustomTag, timepoint_name, std::make_tuple(p, duration));
		center->PrintToFile(record);
		center->RemoveTempTimePoint(timepoint_name);
	}

	void RecordCenter::RecordStartTimePoint(const String& timepoint_name)
	{
		auto& center = RecordCenter::Instance();
		center->AddTempTimePoint(timepoint_name, GetCurrentTime());
	}


	Record::Record(NameType type, const String& name, const TimeMeta& meta_data):
		type_(type), name_(std::move(name)), meta_data_(std::move(meta_data))
	{

	}


	String Record::str() const {
		std::ostringstream stream;
		//Fri Jul 29 11:35 : 10 2016 >> [type] name : duration 
		stream << TimePointToString(std::get<0>(meta_data_)) << " >> [" << type_ << "] " << name_ << " : " << std::get<1>(meta_data_)
			<< " ms";
		return stream.str();
	}

	StopWatch::StopWatch(const String& _function_name) :function_name(std::move(_function_name))
	{
		start_time_point = GetCurrentTime();
	}

	StopWatch::~StopWatch()
	{
		end_time_point = GetCurrentTime();
		Duration duration = static_cast< MeasureTime::Duration >(std::chrono::duration_cast< std::chrono::milliseconds >(end_time_point - start_time_point).count());
		CTimePoint cp = std::chrono::system_clock::to_time_t(start_time_point);
		auto& center = RecordCenter::Instance();
		Record record(Record::NameType::Function, function_name, std::make_tuple(cp, duration));
		center->PrintToFile(record);
	}


	std::ostream& operator<<(std::ostream& s, const Record::NameType r)
	{
		switch (r) {
		case Record::NameType::Function:
			s << "Function";
			break;
		case Record::NameType::CustomTag:
			s << "Tag";
			break;
		default:
			s << "Unknown";
			break;
		}
		return s;
	}

	std::ostream& operator<<(std::ostream& s, const Record& r)
	{
		return s << r.str();
	}


}