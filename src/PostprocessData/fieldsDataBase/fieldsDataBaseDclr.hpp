
#ifndef __fieldsDataBaseDclr_hpp__
#define __fieldsDataBaseDclr_hpp__

#include <variant>
#include <concepts>
#include <type_traits>

#include "types.hpp"
#include "span.hpp"

namespace pFlow::postprocessData
{


template<typename T>
concept ValidFieldType = 
    std::same_as<T, real> || 
    std::same_as<T, realx3> || 
    std::same_as<T, realx4> || 
    std::same_as<T, uint32>;

template<typename T> 
concept VectorType = 
    std::same_as<T, realx3> || 
    std::same_as<T, realx4>;

template<typename T>
concept ScalarType = 
    std::same_as<T, real>;


template<typename T>
concept ValidRegionFieldType = 
    std::same_as<T, real> || 
    std::same_as<T, realx3> || 
    std::same_as<T, realx4> ;


using allPointFieldTypes = std::variant<span<real>, span<realx3>, span<realx4>>; 


} // namespace pFlow

#endif //__fieldsDataBaseDclr_hpp__