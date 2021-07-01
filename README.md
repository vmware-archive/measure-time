# VMware has ended active development of this project, this repository will no longer be updated.

# measure-time

## Overview

## Try it out

### Prerequisites

* Compiler must support C++ 11 or above.

### Build & Run
1. Create new folder Build(or whatever you like)
2. cd(enter) the new folder
3. $ cmake ..(where CMakeLists.txt is)

## Documentation

###Example for MEASURE_BLOCK
```C++
#include <iostream> 
#include <thread> 
#include "measure_time.h" 

void TestFunction1() 
{ 
    MEASURE_BLOCK 
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
} 

int main(int argc, const char*argv[]) 
{ 
    TestFunction1(); 
    return 0; 
} 
```
###Possible Output
```
Fri Jul 29 11:35 : 10 2016 >> [Function] TestFunction1 : 5000 ms 
```

###Example for MEASURE_START/MEASURE_END
```C++
#include <iostream> 
#include <thread> 
#include "measure_time.h" 

void TestFunction2() 
{ 
    MEASURE_START(procedure1) 
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
}

void TestFunction3() 
{ 
    std::this_thread::sleep_for(std::chrono::seconds(5));
    MEASURE_END(procedure1) 
} 

int main(int argc, const char*argv[]) 
{ 
    TestFunction2(); 
    TestFunction3(); 
    return 0; 
} 
```
###Possible Output
```
Fri Jul 29 11:35 : 10 2016 >> [Tag] procedure1 : 10000 ms 
```
## Contributing

The measure-time project team welcomes contributions from the community. If you wish to contribute code and you have not
signed our contributor license agreement (CLA), our bot will update the issue when you open a Pull Request. For any
questions about the CLA process, please refer to our [FAQ](https://cla.vmware.com/faq). For more detailed information,
refer to [CONTRIBUTING.md](CONTRIBUTING.md).

## License
GNU General Public License 2.0 (GPL 2.0)
