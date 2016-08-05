#include <iostream>
#include <thread>

#include "measure_time.h"


void TestFunction1()
{
	MEASURE_BLOCK
	std::this_thread::sleep_for(std::chrono::seconds(5));
}

void TestFunction2()
{
	MEASURE_START(procedure1)
	std::this_thread::sleep_for(std::chrono::seconds(2));

}

void TestFunction3()
{
	std::this_thread::sleep_for(std::chrono::seconds(3));
	MEASURE_END(procedure1)
}


int main(int argc, const char*argv[])
{
	TestFunction1();

	TestFunction2();
	TestFunction3();

	system("pause");
    return 0;
}


