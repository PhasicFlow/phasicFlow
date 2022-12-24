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

#ifndef __linearCF_hpp__
#define __linearCF_hpp__

#include "types.hpp"
#include "symArrays.hpp"

namespace pFlow::cfModels
{

template<bool limited=true>
class linear
{
public:

	struct contactForceStorage
	{
		realx3 overlap_t_ = 0.0;
	};

	struct linearProperties
	{		
		real 	kn_ 	= 1000.0;
		real 	kt_		= 800.0;
		real 	ethan_ 	= 0.0;
		real 	ethat_ 	= 0.0;
		real    mu_ 	= 0.00001;

		INLINE_FUNCTION_HD
		linearProperties(){}

		INLINE_FUNCTION_HD
		linearProperties(real kn, real kt, real etha_n, real etha_t, real mu ):
			kn_(kn), kt_(kt), ethan_(etha_n),ethat_(etha_t), mu_(mu)
		{}		

		INLINE_FUNCTION_HD
		linearProperties(const linearProperties&)=default;

		INLINE_FUNCTION_HD
		linearProperties& operator=(const linearProperties&)=default;

		INLINE_FUNCTION_HD
		~linearProperties() = default;
	};

protected:

	using LinearArrayType = symArray<linearProperties>;

	int32 					numMaterial_ = 0;

	ViewType1D<real>  		 rho_;

	LinearArrayType  		linearProperties_;

	/**/

	bool readLinearDictionary(const dictionary& dict)
	{
		auto kn = dict.getVal<realVector>("kn");
		auto kt = dict.getVal<realVector>("kt");
		auto en = dict.getVal<realVector>("en");
		auto et = dict.getVal<realVector>("et");
		auto mu = dict.getVal<realVector>("mu");

		auto nElem = kn.size();


		if(nElem != kt.size())
		{
			fatalErrorInFunction<<
			"sizes of kn("<<nElem<<") and kt("<<kt.size()<<") do not match.\n";
			return false;
		}

		if(nElem != en.size())
		{
			fatalErrorInFunction<<
			"sizes of kn("<<nElem<<") and en("<<en.size()<<") do not match.\n";
			return false;
		}

		if(nElem != et.size())
		{
			fatalErrorInFunction<<
			"sizes of kn("<<nElem<<") and et("<<et.size()<<") do not match.\n";
			return false;
		}

		if(nElem != mu.size())
		{
			fatalErrorInFunction<<
			"sizes of kn("<<nElem<<") and mu("<<mu.size()<<") do not match.\n";
			return false;
		}

		
		// check if size of vector matchs a symetric array
		uint32 nMat;
		if( !LinearArrayType::getN(nElem, nMat) )
		{
			fatalErrorInFunction<<
			"sizes of properties do not match a symetric array with size ("<<
			numMaterial_<<"x"<<numMaterial_<<").\n";
			return false;
		}
		else if( numMaterial_ != nMat)
		{
			fatalErrorInFunction<<
			"size mismatch for porperties. \n"<<
			"you supplied "<< numMaterial_<<" items in materials list and "<<
			nMat << " for other properties.\n";
			return false;
		}

		realVector etha_n(nElem);
		realVector etha_t(nElem);
		ForAll(i , kn)
		{
			etha_n[i] = -2.0*log(en[i])*sqrt(kn[i])/
					sqrt(pow(log(en[i]),2.0)+ pow(Pi,2.0));

			etha_t[i] = -2.0*log( et[i]*sqrt(kt[i]) )/
					sqrt(pow(log(et[i]),2.0)+ pow(Pi,2.0));
		}

		Vector<linearProperties> prop(nElem);
		ForAll(i,kn)
		{
			prop[i] = {kn[i], kt[i], etha_n[i], etha_t[i], mu[i]};
		}

		linearProperties_.assign(prop);

		return true;

	}

	static const char* modelName()
	{
		if constexpr (limited)
		{
			return "linearLimited";
		}
		else
		{
			return "linearNonLimited";
		}
		return "";
	}

public:


	TypeInfoNV(modelName());

	INLINE_FUNCTION_HD
	linear(){}

	linear(int32 nMaterial, const ViewType1D<real>& rho, const dictionary& dict)
	:
		numMaterial_(nMaterial),
		rho_("rho",nMaterial),
		linearProperties_("linearProperties",nMaterial)
	{

		Kokkos::deep_copy(rho_,rho);
		if(!readLinearDictionary(dict))
		{
			fatalExit;
		}
	}
	
	INLINE_FUNCTION_HD
	linear(const linear&) = default;

	INLINE_FUNCTION_HD
	linear(linear&&) = default;

	INLINE_FUNCTION_HD
	linear& operator=(const linear&) = default;

	INLINE_FUNCTION_HD
	linear& operator=(linear&&) = default;


	INLINE_FUNCTION_HD
	~linear()=default;

	INLINE_FUNCTION_HD
	int32 numMaterial()const
	{
		return numMaterial_;
	}

	//// - Methods

	INLINE_FUNCTION_HD
	void contactForce
	(
		const real dt,
		const int32 i,
		const int32 j,
		const int32 propId_i,
		const int32 propId_j,
		const real Ri,
		const real Rj,
		const real ovrlp_n,
		const realx3& Vr,
		const realx3& Nij,
		contactForceStorage& history,
		realx3& FCn,
		realx3& FCt
	)const
	{

		auto prop = linearProperties_(propId_i,propId_j);


		real vrn = dot(Vr, Nij);	
		realx3 Vt = Vr - vrn*Nij;

		history.overlap_t_ += Vt*dt;

		real mi = 3*Pi/4*pow(Ri,3.0)*rho_[propId_i];
		real mj = 3*Pi/4*pow(Rj,3.0)*rho_[propId_j];

		real sqrt_meff = sqrt((mi*mj)/(mi+mj));

		FCn = (-prop.kn_ * ovrlp_n - sqrt_meff * prop.ethan_ * vrn)*Nij;
		FCt = -prop.kt_ * history.overlap_t_ - sqrt_meff * prop.ethat_*Vt;

		real ft = length(FCt);
		real ft_fric = prop.mu_ * length(FCn);
		
		if(ft > ft_fric)
		{
			if( length(history.overlap_t_) >static_cast<real>(0.0))
			{
				if constexpr (limited)
				{
					FCt *= (ft_fric/ft);
                	history.overlap_t_ = - (FCt/prop.kt_);
				}
				else
				{
					FCt = (FCt/ft)*ft_fric;	
				}
				//cout<<"friction is applied here \n";
				
			}
			else
			{
				FCt = 0.0;
			}
		}

	}
	
};

} //pFlow::cfModels

#endif
