#pragma once
#include <string>
#include <chrono>
#include <thread>
#include <utility>
#include <map>
#include <memory>
#include <fstream>
#include <iostream>
#include <type_traits>
#include <sstream>
#include <ctime>
#include <mutex>
#include <vector>
#include <iomanip>
#include <unordered_map>
#include <tuple>
#include <cstdio>


/* User should set up correctly before using */
//#define MEASURETIME_OUTPUT_PATH "C:\\Program Files (x86)\\CloudVolumes\\Agent\\Logs\\measure_time.log" //example for windows
#define MEASURETIME_OUTPUT_PATH "C:\\Users\\sxia\\Desktop\\m.log"
//#define MEASURETIME_OUTPUT_PATH "/Users/sxia/Desktop/measure_time.log" //example for mac os



/* !!! Don't change it */
#define MEASURE_BLOCK MeasureTime::StopWatch a_stop_watch{__FUNCTION__}; //It will measure the whole function block

#define MEASURE_START(name) MeasureTime::RecordCenter::RecordStartTimePoint(#name);//These two macro should use as a pair
#define MEASURE_END(name) MeasureTime::RecordCenter::RecordEndTimePoint(#name);

namespace MeasureTime {
	//class declaration
	class StopWatch;
	class Record;
	class RecordCenter;
	//type
	using Duration = unsigned long; //default unit is milliseconds
	using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
	using CTimePoint = std::time_t;
	using TimeMeta = std::tuple<CTimePoint, Duration>; // (start time point, duration)

												
	using String = std::string;
	using RecordCenterPtr = std::unique_ptr<RecordCenter>;
	template<typename K, typename V>
	using Map = std::unordered_map<K, V>;
	template<typename K, typename V>
	using MapPtr = std::unique_ptr<Map<K, V>>;
	template<typename T>
	using VectorPtr = std::unique_ptr<std::vector<T>>;

	//helper function
	inline TimePoint GetCurrentTime();
	String TimePointToString(CTimePoint tp);

	template<class T>
	std::string toString(const T& item)
	{
		std::ostringstream temp;
		temp << item;
		return temp.str();
	}

	template<typename ... Args>
	String FormatToString(const String& format, Args&&... args)
	{
		size_t size = snprintf(nullptr, 0, format.c_str(), std::forward<Args...>(args ...)) + 1; // Extra space for '\0'
		std::unique_ptr<char[]> buf(new char[size]);
		snprintf(buf.get(), size, format.c_str(), args ...);
		return String(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
	}

	//class definiton

	class StopWatch
	{
		StopWatch(const StopWatch&) = delete;
	public:
		StopWatch(const String& _function_name);
		~StopWatch();
	private:
		String function_name;
		TimePoint start_time_point;
		TimePoint end_time_point;
	};

	class Record
	{

	public:
		enum class NameType :unsigned char {
			Function,
			CustomTag
		};
		Record(NameType type, const String& name, const TimeMeta& meta_data);
		std::string str() const;
		NameType getType() { return type_; }
	private:
		NameType type_;
		String name_;
		TimeMeta meta_data_;
	};

	class RecordCenter
	{

	public:
		static RecordCenterPtr& Instance();
		static void RecordStartTimePoint(const String& timepoint_name);
		static void RecordEndTimePoint(const String& timepoint_name);
		static bool Enable();
        template<class T>
	    void PrintToFile(const T& obj);
        template<class T>
		void PrintToConsole(const T& msg);
		std::ostream& error() { return output_file; }
		inline void AddTempTimePoint(const String& name, TimePoint&& timepoint);
		inline void RemoveTempTimePoint(const String& name);
		inline bool GetTempTimePoint(const String& name, TimePoint& tp);

	private:

		//class variable
		std::mutex records_mutex;
		std::ofstream output_file;
		Map<String, TimePoint> temporary_timepoint;
		RecordCenter();
	};

	std::ostream& operator<<(std::ostream& s, const Record::NameType);
	std::ostream& operator<<(std::ostream& s, const Record&);

}
