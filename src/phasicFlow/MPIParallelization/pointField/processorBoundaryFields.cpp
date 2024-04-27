
//#include "Field.hpp"

#include "processorBoundaryField.hpp"

template class pFlow::MPI::processorBoundaryField<pFlow::uint8>;
template class pFlow::MPI::processorBoundaryField<pFlow::uint8, pFlow::HostSpace>;

template class pFlow::MPI::processorBoundaryField<pFlow::uint32>;
template class pFlow::MPI::processorBoundaryField<pFlow::uint32, pFlow::HostSpace>;

template class pFlow::MPI::processorBoundaryField<pFlow::uint64>;
template class pFlow::MPI::processorBoundaryField<pFlow::uint64, pFlow::HostSpace>;

template class pFlow::MPI::processorBoundaryField<pFlow::real>;
template class pFlow::MPI::processorBoundaryField<pFlow::real, pFlow::HostSpace>;

template class pFlow::MPI::processorBoundaryField<pFlow::realx3>;
template class pFlow::MPI::processorBoundaryField<pFlow::realx3, pFlow::HostSpace>;

template class pFlow::MPI::processorBoundaryField<pFlow::realx4>;
template class pFlow::MPI::processorBoundaryField<pFlow::realx4, pFlow::HostSpace>;


