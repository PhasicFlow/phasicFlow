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

#ifndef __MotionModel_hpp__
#define __MotionModel_hpp__



#include "VectorSingles.hpp"
#include "Vector.hpp"
#include "List.hpp"
#include "dictionary.hpp"


namespace pFlow
{

/**
 * @brief Motion model abstract class (CRTP) for all the motion models 
*/
template<typename Model, typename Component>
class MotionModel
{
public:

	using ModelType = Model;

	using ModelComponent = Component;
	
	using ComponentVector_D = VectorSingle<ModelComponent>;

	/** Motion model class to be passed to computational units/kernels for
	 *  transfing points and returning velocities at various positions
	 */
	class ModelInterface
	{
	private:
		
		deviceViewType1D<ModelComponent> 	components_;

		uint32 							numComponents_=0;

	public:

		INLINE_FUNCTION_HD
		ModelInterface
		(
			deviceViewType1D<ModelComponent> Comps, 
			uint32 numComp
		):
			components_(Comps),
			numComponents_(numComp)
		{}

		INLINE_FUNCTION_HD
		ModelInterface(const ModelInterface&) = default;
		
		INLINE_FUNCTION_HD
		ModelInterface& operator=(const ModelInterface&) = default;

		INLINE_FUNCTION_HD
		ModelInterface(ModelInterface&&) noexcept = default;

		INLINE_FUNCTION_HD
		ModelInterface& operator=(ModelInterface&&) noexcept= default;
		
		INLINE_FUNCTION_HD
		~ModelInterface()=default;

		INLINE_FUNCTION_HD
		realx3 pointVelocity(uint32 n, const realx3& p, const realx3& wallNormal)const
		{
			return components_[n].linVelocityPoint(p, wallNormal);
		}

		INLINE_FUNCTION_HD
		realx3 operator()(uint32 n, const realx3& p, const realx3& wallNormal)const
		{
			return pointVelocity(n, p, wallNormal);
		}

		INLINE_FUNCTION_HD
		realx3 transferPoint(uint32 n, const realx3 p, real dt)const
		{
			return components_[n].transferPoint(p, dt); 
		}

		INLINE_FUNCTION_HD 
		uint32 size()const
		{
			return numComponents_;
		}
	};

private:

	// friends
	friend ModelType;

	/// Number of axes components
	uint32  			numComponents_= 0;

	/// Vector to store motion components 
	ComponentVector_D 	motionComponents_;
	
	/// Names of compoenents
	wordList  			componentNames_;

protected:
	
	/// @brief  obtain a reference to the actual motion model
	inline
	auto& getModel()
	{
		return static_cast<ModelType&>(*this);
	}

	/// @brief  Obtain a const reference to the actual motion model 
	inline
	const auto& getModel()const
	{
		return static_cast<const ModelType&>(*this);
	}

	/// Return a none object for the motion model 
	inline
	auto impl_noneComponent()const
	{
		return ModelType::noneComponent();
	}
	
	/// Name of the compoenent to index of the component 
	bool impl_nameToIndex(const word& name, uint32& idx)const;
	
	/// Component index to motion component name
	bool impl_indexToName(uint32 i, word& name)const;

	/// const reference to the list of component names 
	inline 
	const wordList& impl_componentNames()const
	{
		return componentNames_;
	}

	/// Return model interface 
	auto impl_getModelInterface(uint32 iter, real t, real dt)const
	{
		getModel().impl_setTime(iter, t, dt);

		return ModelInterface(
			motionComponents_.deviceViewAll(), 
			numComponents_);
	}

	/// Read from dictionary 
	bool impl_readDictionary(const dictionary& dict);

	/// Write to dictionary 
	bool impl_writeDictionary(dictionary& dict)const;

public:

	// - Constructors 
	
		/// Empty
		MotionModel();

		/// Copy constructor 
		MotionModel(const MotionModel&) = default;

		/// Move constructor 
		MotionModel(MotionModel&&) = default;

		/// Copy assignment
		MotionModel& operator=(const MotionModel&) = default;

		/// No move assignment
		MotionModel& operator=(MotionModel&&) = default;

		/// Destructor 
		~MotionModel() = default;

	// - Methods
		
		/// name of the compoenent to index of the component 
		bool nameToIndex(const word& name, uint32& idx)const
		{
			return getModel().impl_nameToIndex(name, idx);
		}
	
		/// Component index to motion component name 
		bool indexToName(uint32 idx, word& name)const
		{
			return getModel().impl_indexToName(idx, name);
		}

		/// @brief  Return a const reference to the list of compoenent names 
		/// @return 
		const wordList& componentNames()const
		{
			return getModel().impl_componentNames();
		}
		
		/// Is the wall assocciated to this motion component moving?
		bool isMoving()const
		{
			return getModel().impl_isMoving();
		}

		/// Move the component itself
		bool move(uint32 iter, real t, real dt)
		{
			return getModel().impl_move(iter, t, dt);
		}

		/// Obtain an object to model interface 
		auto getModelInterface(uint32 iter, real t, real dt)const
		{
			return getModel().impl_getModelInterface(iter, t, dt);
		}

};

} // pFlow

#include "MotionModel.cpp"


#endif //__MotionModel_hpp__
