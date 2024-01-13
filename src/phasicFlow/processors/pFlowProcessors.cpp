#include "pFlowProcessors.hpp"

static pFlow::localProcessors pFlowProcessors__{"pFlowProcessors"};

pFlow::localProcessors& pFlow::pFlowProcessors()
{
    return pFlowProcessors__;
}

void pFlow::initialize_pFlowProcessors()
{
    pFlowProcessors__ = localProcessors("pFlowProcessors");
}

void pFlow::initialize_pFlowProcessors(const std::vector<int>& ranks)
{
    pFlowProcessors__ = localProcessors("pFlowProcessors", ranks);
}