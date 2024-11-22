

#ifndef __MPITimer_hpp__
#define __MPITimer_hpp__

#include "Timer.hpp"


namespace pFlow
{

class MPITimer
:
    public Timer
{
private:

    // hiding methods
    using Timer::accTimersTotal;
    using Timer::averageTime;

public:

    TypeInfo("MPITimer");

    explicit MPITimer(const word& name)
	: 
    Timer(name)
	{}

	
    real totalTimeMax()const;

    std::vector<real> totalTimeAllToAll()const;

    std::vector<real> totalTimeAllToMaster()const;

    real averageTimeMax()const;

    std::vector<real> averageTimeAllToAll()const;

    std::vector<real> averageTimeAllAtoMaster()const;

    // call this from all processors in pFlowProcessors
    bool write(iOstream& os)const;

};

MPITimer& ComputationTimer();

}



#endif