#include "MPITimer.hpp"

#ifdef pFlow_Build_MPI
    #include "pFlowProcessors.hpp"
    #include "procCommunication.hpp"
#endif

pFlow::real pFlow::MPITimer::totalTimeMax() const
{
    return accTimersTotalMax();
}

std::vector<pFlow::real> pFlow::MPITimer::totalTimeAllToAll() const
{
#ifdef pFlow_Build_MPI
    MPI::procCommunication comm(pFlowProcessors());
    auto [totTime, succs] = comm.collectAllToAll(totalTime());
    return totTime;
#else
    return std::vector<real>(1, totalTime());
#endif
}

std::vector<pFlow::real> pFlow::MPITimer::totalTimeAllToMaster() const
{
#ifdef pFlow_Build_MPI
	MPI::procCommunication comm(pFlowProcessors());
	auto [totTime, succs] = comm.collectAllToMaster(totalTime());
	return totTime;
#else
		return std::vector<real>(1, totalTime());
#endif
}

pFlow::real pFlow::MPITimer::averageTimeMax() const
{
    return Timer::averageTimeMax();
}

std::vector<pFlow::real> pFlow::MPITimer::averageTimeAllToAll() const
{
#ifdef pFlow_Build_MPI
    MPI::procCommunication comm(pFlowProcessors());
    auto [totTime, succs] = comm.collectAllToAll(averageTime());
    return totTime;
#else
    return std::vector<real>(1, averageTime());
#endif
}

std::vector<pFlow::real> pFlow::MPITimer::averageTimeAllAtoMaster() const
{
#ifdef pFlow_Build_MPI
	MPI::procCommunication comm(pFlowProcessors());
	auto [totTime, succs] = comm.collectAllToMaster(averageTime());
	return totTime;
#else
		return std::vector<real>(1, averageTime());
#endif
}

bool pFlow::MPITimer::write(iOstream &os) const
{
	const auto ts = totalTimeAllToAll();
	auto maxloc = std::distance(ts.begin(), std::max_element(ts.begin(), ts.end()));
	os<<'(';
	for(auto i=0; i<ts.size(); i++)
	{
		if(maxloc == i)
			os<<Red_Text(ts[i]);
		else
			os<<ts[i];
		
		if(i != ts.size()-1)
			os<<' ';
		else
			os<<')'<<endl;
	}

    return true;
}

static pFlow::MPITimer ComputationTimer__{"ComputationTimer"};

pFlow::MPITimer &pFlow::ComputationTimer()
{
    return ComputationTimer__;
}
