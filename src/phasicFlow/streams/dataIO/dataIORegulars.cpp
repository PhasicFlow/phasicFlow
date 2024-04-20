

#include "types.hpp"
#include "dataIO.hpp"
#include "dataIORegular.hpp"

template class pFlow::dataIO<pFlow::uint8>;
template class pFlow::dataIORegular<pFlow::uint8>;

template class pFlow::dataIO<pFlow::int8>;
template class pFlow::dataIORegular<pFlow::int8>;

template class pFlow::dataIO<pFlow::int32>;
template class pFlow::dataIORegular<pFlow::int32>;

template class pFlow::dataIO<pFlow::int64>;
template class pFlow::dataIORegular<pFlow::int64>;

template class pFlow::dataIO<pFlow::uint32>;
template class pFlow::dataIORegular<pFlow::uint32>;

template class pFlow::dataIO<pFlow::uint64>;
template class pFlow::dataIORegular<pFlow::uint64>;

template class pFlow::dataIO<pFlow::real>;
template class pFlow::dataIORegular<pFlow::real>;

template class pFlow::dataIO<pFlow::realx3>;
template class pFlow::dataIORegular<pFlow::realx3>;

template class pFlow::dataIO<pFlow::realx4>;
template class pFlow::dataIORegular<pFlow::realx4>;

template class pFlow::dataIO<pFlow::word>;
template class pFlow::dataIORegular<pFlow::word>;

/*template class pFlow::dataIO<char>;
template class pFlow::dataIORegular<char>;*/
