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


namespace pFlow
{
	//
	template<typename T>
	struct checkStatic
	{
		has_static_member(TYPENAME);	
		
		static constexpr
		bool hasMember()
		{
			return has_static_member_TYPENAME<T,word(void)>::value;
		}
	};

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
	inline word basicTypeName<int8>(){ return "int8"; }

	template<>
	inline word basicTypeName<uint64>(){ return "uint64"; }

	template<>
	inline word basicTypeName<uint32>(){ return "uint32"; }

	template<>
	inline word basicTypeName<uint8>(){ return "uint8"; }

	template<>
	inline word basicTypeName<real>(){ return "real"; }

	
	template<typename T>
	word constexpr getTypeName()
	{
		
		if constexpr ( checkStatic<T>::hasMember() )
		{
			return T::TYPENAME();
		}else
		{
			return basicTypeName<T>();
		}
	}
	template<typename T>
	word constexpr getTypeName(const T&)
	{
		if constexpr ( checkStatic<T>::hasMember() )
		{
			return T::TYPENAME();
		}else
		{
			return basicTypeName<T>();
		}
	}

	// compare the overriden typeName of object with concrete TYPENAME 
	// of Type1
	template <typename Type1, typename Type2>
	bool checkType(Type2* object)
	{
		return getTypeName<Type1>() == object->typeName();
	}

	template <typename Type1, typename Type2>
	bool checkType(Type2& object)
	{
		return getTypeName<Type1>() == object.typeName();
	}
}


#define TypeInfo(tName)  \
	inline static word TYPENAME() {return tName; } \
	virtual word typeName() const {return TYPENAME();} 

#define TypeInfoNV(tName)  \
	inline static word TYPENAME() {return tName; } \
	word typeName() const {return TYPENAME();} 


#define TypeInfoTemplate11(tName, Type)										\
	has_static_member(TYPENAME);											\
	inline static word TYPENAME()											\
	{ 																		\
		return word(tName)+"<"+getTypeName<Type1>()+">";					\
	}																		\
	virtual word typeName() const { return TYPENAME();}

#define TypeInfoTemplate12(tName, Type1, Type2)								\
	inline static word TYPENAME()											\
	{ 																		\
		return word(tName)+"<"+getTypeName<Type1>()+","+getTypeName<Type2>()+">";}	\
	}																		\
	virtual word typeName() const { return TYPENAME();}

#define TypeInfoTemplate13(tName, Type1, Type2, Type3)						\
	inline static word TYPENAME()											\
	{ 																		\
		return word(tName)+"<"+getTypeName<Type1>()+","+getTypeName<Type2>()+","+getTypeName<Type3>()+">";\
	}																		\
	virtual word typeName() const { return TYPENAME();}

// this is the non-virtual version
#define TypeInfoTemplateNV11(tName, Type)									\
	inline static word TYPENAME()											\
	{ 																		\
		return word(tName)+"<"+getTypeName<Type>()+">"; 					\
	}																		\
	inline word typeName() const { return TYPENAME();}


#define TypeInfoTemplateNV111(tName, Type, tName2)							\
	inline static word TYPENAME()											\
	{ 																		\
		return word(tName)+"<"+getTypeName<Type>()+","+word(tName2)+">";	\
	}																		\
	inline word typeName() const { return TYPENAME();}

#define TypeInfoTemplate111(tName, Type, tName2)							\
	inline static word TYPENAME()											\
	{ 																		\
		return word(tName)+"<"+getTypeName<Type>()+","+word(tName2)+">"; 	\
	}																		\
	virtual word typeName() const { return TYPENAME();}


/*#define TypeInfoTemplate11(tName, Type)										\
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

#define TypeInfoTemplate12(tName, Type1, Type2)								\
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

#define TypeInfoTemplate13(tName, Type1, Type2, Type3)						\
	inline static word TYPENAME()											\
	{ 																		\
		return word(tName)+"<"+Type1::TYPENAME()+","+Type2::TYPENAME()+","+Type3::TYPENAME()+">";\
	}																		\
	virtual word typeName() const { return TYPENAME();}

// this is the non-virtual version
#define TypeInfoTemplateNV11(tName, Type)										\
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


#define TypeInfoTemplateNV111(tName, Type, tName2)							\
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

#define TypeInfoTemplate111(tName, Type, tName2)							\
	has_static_member(TYPENAME);											\
	inline static word TYPENAME()											\
	{ 																		\
	if constexpr ( has_static_member_TYPENAME<Type,word(void)>::value) 		\
		{ return word(tName)+"<"+Type::TYPENAME()+","+word(tName2)+">";} 	\
	else																	\
		return word(tName)+"<"+basicTypeName<Type>()+","+word(tName2)+">";	\
	return "noTYPE"; 														\
	}																		\
	virtual word typeName() const { return TYPENAME();}*/






namespace pFlow
{

	



} // pFlow

#endif
