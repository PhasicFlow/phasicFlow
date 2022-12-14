/*------------------------------- phasicFlow ---------------------------------
      O        C enter of
     O O       E ngineering and
    O   O      M ultiscale modeling of
   OOOOOOO     F luid flow       
------------------------------------------------------------------------------
  Copyright (C): www.cemf.ir
  email: hamid.r.norouzi AT gmail.com
------------------------------------------------------------------------------  
Licence:
  This file is part of phasicFlow code. It is a free software for simulating 
  granular and multiphase flows. You can redistribute it and/or modify it under
  the terms of GNU General Public License v3 or any other later versions. 
 
  phasicFlow is distributed to help others in their research in the field of 
  granular and multiphase flows, but WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

-----------------------------------------------------------------------------*/


#ifndef __typeInfo_hpp__
#define __typeInfo_hpp__

#include <typeinfo>
#include <cxxabi.h>

#include "bTypes.hpp"


#define has_static_member(name)		 								\
	template<typename, typename> 									\
	struct has_static_member_##name;								\
																	\
	template<typename testType, typename Ret, typename... Args>		\
	struct has_static_member_##name<testType, Ret(Args...)> {		\
    	template<typename U, U> struct Check; 						\
																	\
    template<typename U> 											\
    static std::true_type Test(Check<Ret(*)(Args...), &U::name>*); 	\
    template<typename U>											\
    static std::false_type Test(...);								\
    static const bool value = decltype(Test<testType>(0))::value;	\
};



#define TypeInfo(tName)  \
	inline static word TYPENAME() {return tName; } \
	virtual word typeName() const {return TYPENAME();} 

#define TypeInfoNV(tName)  \
	inline static word TYPENAME() {return tName; } \
	word typeName() const {return TYPENAME();} 


#define TypeInfoTemplate(tName, Type)										\
	has_static_member(TYPENAME);											\
	inline static word TYPENAME()											\
	{ 																		\
	if constexpr( has_static_member_TYPENAME<Type,word(void)>::value) 		\
		{ return word(tName)+"<"+Type::TYPENAME()+">";} 						\
	else																	\
		return word(tName)+"<"+basicTypeName<Type>()+">";						\
	return "noTYPE"; 														\
	}																		\
	virtual word typeName() const { return TYPENAME();}

#define TypeInfoTemplate2(tName, Type1, Type2)								\
	has_static_member(TYPENAME);											\
	inline static word TYPENAME()											\
	{ 																		\
	if constexpr( has_static_member_TYPENAME<Type1,word(void)>::value) 		\
		{ return word(tName)+"<"+Type1::TYPENAME()+","+Type2::TYPENAME()+">";}	\
	else																	\
		return word(tName)+"<"+basicTypeName<Type1>()+","+Type2::TYPENAME()+">";\
	return "noTYPE"; 														\
	}																		\
	virtual word typeName() const { return TYPENAME();}

#define TypeInfoTemplate3(tName, Type1, Type2, Type3)						\
	inline static word TYPENAME()											\
	{ 																		\
		return word(tName)+"<"+Type1::TYPENAME()+","+Type2::TYPENAME()+","+Type3::TYPENAME()+">";\
	}																		\
	virtual word typeName() const { return TYPENAME();}

// this is the non-virtual version
#define TypeInfoTemplateNV(tName, Type)										\
	has_static_member(TYPENAME);											\
	inline static word TYPENAME()											\
	{ 																		\
	if constexpr( has_static_member_TYPENAME<Type,word(void)>::value) 		\
		{ return word(tName)+"<"+Type::TYPENAME()+">";} 					\
	else																	\
		return word(tName)+"<"+basicTypeName<Type>()+">";					\
	return "noTYPE"; 														\
	}																		\
	inline word typeName() const { return TYPENAME();}


#define TypeInfoTemplateNV2(tName, Type, tName2)							\
	has_static_member(TYPENAME);											\
	inline static word TYPENAME()											\
	{ 																		\
	if constexpr ( has_static_member_TYPENAME<Type,word(void)>::value) 		\
		{ return word(tName)+"<"+Type::TYPENAME()+","+word(tName2)+">";} 	\
	else																	\
		return word(tName)+"<"+basicTypeName<Type>()+","+word(tName2)+">";	\
	return "noTYPE"; 														\
	}																		\
	inline word typeName() const { return TYPENAME();}




namespace pFlow
{

	template <typename T>
	inline word basicTypeName()
	{
		int status;
		auto& ti = typeid(T);
		char* realname = abi::__cxa_demangle(ti.name(), 0, 0, &status);
	  	word name(realname);
	  	free(realname);
	  	return name;
	}

	template<>
	inline word basicTypeName<word>(){ return "word"; }

	template<>
	inline word basicTypeName<int64>(){ return "int64"; }

	template<>
	inline word basicTypeName<int32>(){ return "int32"; }

	template<>
	inline word basicTypeName<int16>(){ return "int16"; }

	template<>
	inline word basicTypeName<int8>(){ return "int8"; }

	template<>
	inline word basicTypeName<label>(){ return "label"; }

	template<>
	inline word basicTypeName<uint32>(){ return "uint32"; }

	template<>
	inline word basicTypeName<real>(){ return "real"; }

		
	// compare the overriden typeName of object with concrete TYPENAME 
	// of Type1
	template <typename Type1, typename Type2>
	bool checkType(Type2* object)
	{
		return word(Type1::TYPENAME()) == object->typeName();
	}

	template <typename Type1, typename Type2>
	bool checkType(Type2& object)
	{
		return word(Type1::TYPENAME()) == object.typeName();
	}



} // pFlow

#endif
