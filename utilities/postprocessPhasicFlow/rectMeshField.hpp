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

#ifndef __rectMeshField_hpp__
#define __rectMeshField_hpp__

#include "rectangleMesh.hpp"
#include "ViewAlgorithms.hpp"

namespace pFlow
{

template<typename T>
class rectMeshField
:
	public IOobject
{
public:

	using viewType = ViewType3D<T,HostSpace>;

	using memory_space = typename viewType::memory_space;

protected:

	const rectangleMesh* mesh_;

	word 			name_="noName";

	viewType 		field_;

	T 				defaultValue_{};

	constexpr static inline const char* memoerySpaceName()
  	{
  		return memory_space::name();
  	}

public:


	TypeInfoTemplateNV111("rectMeshField", T, memoerySpaceName());
	
	rectMeshField(rectangleMesh& mesh, const word& name ,const T& defVal)
	:
		IOobject(
			objectFile
			(
				name,
				"",
				objectFile::READ_NEVER,
				objectFile::WRITE_NEVER
			),
			IOPattern::MasterProcessorOnly,
			nullptr
		),
		mesh_(&mesh),
		name_(name),
		field_("reactMeshField."+name, mesh_->nx(), mesh_->ny(), mesh_->nz()),
		defaultValue_(defVal)
	{
		this->fill(defaultValue_);
	}

	rectMeshField(rectangleMesh& mesh, const T& defVal)
	:
		IOobject(
			objectFile
			(
				"noName",
				"",
				objectFile::READ_NEVER,
				objectFile::WRITE_NEVER
			),
			IOPattern::MasterProcessorOnly,
			nullptr
		),
		mesh_(&mesh),
		name_("noName"),
		field_("reactMeshField.noName", mesh_->nx(), mesh_->ny(), mesh_->nz()),
		defaultValue_(defVal)
	{
		this->fill(defaultValue_);
	}

	rectMeshField(const rectMeshField&) = default;

	rectMeshField& operator = (const rectMeshField&) = default;

	rectMeshField(rectMeshField&&) = default;

	rectMeshField& operator =(rectMeshField&&) = default;


	inline uniquePtr<rectMeshField> clone() const
	{
		return makeUnique<rectMeshField>(*this);
	}

	inline rectMeshField* clonePtr()const
	{
		return new rectMeshField(*this);
	}

	INLINE_FUNCTION_H
	const word& name()const
	{
		return name_;
	}

	INLINE_FUNCTION_HD
	int64 size()const
	{
		return mesh_->size();
	}

	auto nx()const
	{
		return mesh_->nx();
	}

	auto ny()const
	{
		return mesh_->ny();
	}

	auto nz()const
	{
		return mesh_->nz();
	}

	const auto& mesh()
	{
		return *mesh_;
	}

	INLINE_FUNCTION_HD
	real cellVol()const
	{
		return mesh_->cellVol();
	}

	INLINE_FUNCTION_HD
	T& operator()(int32 i, int32 j, int32 k)
	{
		return field_(i,j,k);
	}

	INLINE_FUNCTION_HD
	const T& operator()(int32 i, int32 j, int32 k)const
	{
		return field_(i,j,k);
	}

	void fill(T val)
	{
		pFlow::fill(
			field_,
			val
			);
	}

	 
	bool write(iOstream& is, const IOPattern& iop)const override
	{
		notImplementedFunction;
		return true;
	}

	bool read(iIstream& is, const IOPattern& iop) override
	{
		notImplementedFunction;
		return true;
	}

	bool read(iIstream& is)
	{
		notImplementedFunction;
		return true;
	}

	bool write(iOstream& os)const
	{
		notImplementedFunction;
		return true;
	}

};



}


#endif // __rectMeshField_hpp__
