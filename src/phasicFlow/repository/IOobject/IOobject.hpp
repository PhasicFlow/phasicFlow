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


#ifndef __IOobject_hpp__
#define __IOobject_hpp__



#include "IOfileHeader.hpp"


namespace pFlow
{

class repository;

class IOobject
:
	public	IOfileHeader
{

private:

	class iObject
	{
	public:

		virtual ~iObject()=default;

		// - clone 
		virtual uniquePtr<iObject> clone() const = 0;

		virtual word typeName()const = 0;

		virtual bool read_object_t(iIstream& is, IOPattern iop) = 0;

		virtual bool write_object_t(iOstream& os, IOPattern iop)const = 0;

	};

	template<typename dataType>
	class object_t
	:
		public iObject
	{
	protected:
		dataType 	data_;

	public:

		template<typename... Args,
          		typename = std::enable_if_t<std::is_constructible<dataType, Args&&...>::value>>
		object_t(Args&&... args)
		:
			data_(std::forward<Args>(args)...)
		{
			/*constexpr word msg(dataType::TYPENAME()+"input is not a member function.")
			static_assert(std::is_member_function_pointer<decltype(&dataType::write)>::value,
                  msg.c_str());*/
		}

		// cunstruct by copying data
		object_t(const dataType& data): data_(data)
		{}

		
		virtual uniquePtr<iObject> clone() const
		{
			return makeUnique<object_t>(*this);
		}

		virtual word typeName()const
		{
			return data_.typeName();
		}

		bool read_object_t(iIstream& is, IOPattern iop) override
		{
			return data_.read(is, iop);
		}

		bool write_object_t(iOstream& os, IOPattern iop)const override
		{
			return data_.write(os, iop);
		}

		auto& data()
		{
			return data_;
		}

		const auto& data()const
		{
			return data_;
		}

	};

protected:

	//// - data members

		// underlaying data object 
		uniquePtr<iObject> object_ = nullptr;

		

public:

	// - typeinfo
	word typeName()const
	{
		return object_->typeName();
	}

	//// - Constructors

		// - construct from components, transfer the ownership of iObject (object_t) to the 
		//   onwner and read the object from file 
		IOobject(const objectFile& objf, const repository* owner, uniquePtr<iObject>&& obj );

		// - construct from components, transfer the ownership of IOobject to the owner (no read happens)
		IOobject(const objectFile& objf, const repository* owner, uniquePtr<IOobject>&& obj);
		
		
		template<typename T, 
			typename = std::enable_if_t<
				!std::is_same<T, uniquePtr<IOobject::iObject>>::value && 
				!std::is_same<T, uniquePtr<IOobject>>::value>>
		IOobject(const objectFile& objf, const repository* owner, const T& data)
		:
			IOfileHeader(objf, owner),
			object_(makeUnique<object_t<T>>(data))
		{
			if(!read(this->readWriteHeader()))
			{
				fatalErrorInFunction<<
				"error in reading " << name() << " from path " << path()<<endl;
				fatalExit;
			}
		}

		
		template<typename T, 
			typename = std::enable_if_t<
				!std::is_same<T, uniquePtr<IOobject::iObject>>::value && 
				!std::is_same<T, uniquePtr<IOobject>>::value>>
		IOobject(const objectFile& objf, const repository* owner,  T&& data)
		:
			IOfileHeader(objf, owner),
			object_(makeUnique<object_t<T>>(data))
		{
			if(!read(this->readWriteHeader()))
			{
				fatalErrorInFunction<<
				"error in reading " << name() << " from path " << path()<<endl;
				fatalExit;
			}
		}		

		// - copy construct 
		IOobject(const IOobject& src)=delete;	

		// - move construct
		IOobject(IOobject&& src) = default;

		
		// - make object from components, considering no owner for this object, and 
		//   read from file 
		//   Args are the arguments of object constructor 
		template<typename T, typename... Args>
		static auto make(const objectFile& objf, Args&&... args);
		
		// - construct object_t with the Args as the arguments of object constructor 
		template<typename T, typename... Args>
		static uniquePtr<iObject> make_object_t(Args&&... args);
		

	//// - Access to data object 

		// - is object valid 
		bool isObjectValid()const;

		// - ref to data object
		template<typename T>
		auto& getObject();

		// - const ref to data object 
		template<typename T>
		const auto& getObject()const;
	

	//// - IO operations 

		// - read from file 
		bool read(bool rdHdr = true);

		// - write to file 
		bool write() const;

		// - read from istream 
		bool read(iIstream& is, bool rdHdr = true);

		// - write to istream 
		bool write(iOstream& os) const;
		

};

}

#include "IOobjectTemplates.cpp"

#endif //__IOobject_hpp__
