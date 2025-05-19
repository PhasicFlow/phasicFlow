
#include "types.hpp"
#include "dataIOMPI.hpp"


template class pFlow::MPI::dataIOMPI<pFlow::uint8>;

template class pFlow::MPI::dataIOMPI<pFlow::int8>;

template class pFlow::MPI::dataIOMPI<pFlow::int32>;

template class pFlow::MPI::dataIOMPI<pFlow::int64>;

template class pFlow::MPI::dataIOMPI<pFlow::uint32>;
template class pFlow::MPI::dataIOMPI<pFlow::uint32x3>;

template class pFlow::MPI::dataIOMPI<pFlow::uint64>;

//template class pFlow::MPI::dataIOMPI<pFlow::size_t>;

template class pFlow::MPI::dataIOMPI<pFlow::real>;

template class pFlow::MPI::dataIOMPI<pFlow::realx3>;

template class pFlow::MPI::dataIOMPI<pFlow::realx4>;

template class pFlow::MPI::dataIOMPI<pFlow::word>;