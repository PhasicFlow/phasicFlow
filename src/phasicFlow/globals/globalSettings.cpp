
#include <chrono>
#include <thread>

#include "globalSettings.hpp"



const double pFlow::gSettings::vectorGrowthFactor__ = 1.1;

void pFlow::gSettings::sleepMiliSeconds(int miliSeconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(miliSeconds));
}
