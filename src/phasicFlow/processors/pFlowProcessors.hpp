
#include "localProcessors.hpp"


namespace pFlow
{

localProcessors& pFlowProcessors();

void initialize_pFlowProcessors();

void initialize_pFlowProcessors(const std::vector<int>& ranks);

}