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

#ifndef __nonLinearModCF_hpp__
#define __nonLinearModCF_hpp__

#include "types.hpp"

namespace pFlow::cfModels
{

template<bool limited=true>
class nonLinearMod
{
public:

	struct contactForceStorage
	{
		realx3 overlap_t_ = 0.0;
	};

	struct nonLinearProperties
	{		
		real 	Yeff_	= 1000000.0;
		real 	Geff_	= 8000000.0;
		real 	ethan_= 0.0;
		real  mu_ 	= 0.00001;

		INLINE_FUNCTION_HD
		nonLinearProperties(){}

		INLINE_FUNCTION_HD
		nonLinearProperties(real Yeff, real Geff, real etha_n, real mu ):
			Yeff_(Yeff), Geff_(Geff), ethan_(etha_n), mu_(mu)
		{}		

		INLINE_FUNCTION_HD
		nonLinearProperties(const nonLinearProperties&)=default;

		INLINE_FUNCTION_HD
		nonLinearProperties& operator=(const nonLinearProperties&)=default;

		INLINE_FUNCTION_HD
		~nonLinearProperties() = default;
	};

protected:

	using NonLinearArrayType = 	symArray<nonLinearProperties>;

	int32 						numMaterial_ = 0;

	ViewType1D<real>  		 	rho_;

	NonLinearArrayType  		nonlinearProperties_;

	bool readNonLinearDictionary(const dictionary& dict)
	{
		auto Yeff = dict.getVal<realVector>("Yeff");
		auto Geff = dict.getVal<realVector>("Geff");
		auto nu = dict.getVal<realVector>("nu");
		auto etha_n = dict.getVal<realVector>("etha_n");
		auto mu = dict.getVal<realVector>("mu");

		auto nElem = Yeff.size();

		if(nElem != nu.size())
		{
			fatalErrorInFunction<<
			"sizes of Yeff("<<nElem<<") and nu("<<nu.size()<<") do not match.\n";
			return false;
		}

		if(nElem != etha_n.size())
		{
			fatalErrorInFunction<<
			"sizes of Yeff("<<nElem<<") and etha_n("<<etha_n.size()<<") do not match.\n";
			return false;
		}


		if(nElem != mu.size())
		{
			fatalErrorInFunction<<
			"sizes of Yeff("<<nElem<<") and mu("<<mu.size()<<") do not match.\n";
			return false;
		}

		
		// check if size of vector matchs a symetric array
		uint32 nMat;
		if( !NonLinearArrayType::getN(nElem, nMat) )
		{
			fatalErrorInFunction<<
			"sizes of properties do not match a symetric array with size ("<<
			numMaterial_<<"x"<<numMaterial_<<").\n";
			return false;
		}
		else if( numMaterial_ != nMat)
		{
			fatalErrorInFunction<<
			"size mismatch for porperties. \n";
			return false;
		}


		Vector<nonLinearProperties> prop("prop",nElem);
		ForAll(i,Yeff)
		{
			prop[i] = {Yeff[i], Geff[i], etha_n[i], mu[i]};
		}

		nonlinearProperties_.assign(prop);

		return true;

	}

	static const char* modelName()
	{
		if constexpr (limited)
		{
			return "nonLinearModLimited";
		}
		else
		{
			return "nonLinearModNonLimited";
		}
		return "";
	}

public:

	TypeInfoNV(modelName());

	INLINE_FUNCTION_HD
	nonLinearMod(){}

	nonLinearMod(
		int32 nMaterial,
		const ViewType1D<real>& rho,
		const dictionary& dict)
	:
		numMaterial_(nMaterial),
		rho_("rho",nMaterial),
		nonlinearProperties_("nonLinearProperties",nMaterial)
	{

		Kokkos::deep_copy(rho_,rho);
		if(!readNonLinearDictionary(dict))
		{
			fatalExit;
		}
	}
	
	INLINE_FUNCTION_HD
	nonLinearMod(const nonLinearMod&) = default;

	INLINE_FUNCTION_HD
	nonLinearMod(nonLinearMod&&) = default;

	INLINE_FUNCTION_HD
	nonLinearMod& operator=(const nonLinearMod&) = default;

	INLINE_FUNCTION_HD
	nonLinearMod& operator=(nonLinearMod&&) = default;


	INLINE_FUNCTION_HD
	~nonLinearMod()=default;

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
		const uint32 i,
		const uint32 j,
		const uint32 propId_i,
		const uint32 propId_j,
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

		auto prop = nonlinearProperties_(propId_i,propId_j);


		real vrn = dot(Vr, Nij);	
		realx3 Vt = Vr - vrn*Nij;

		history.overlap_t_ += Vt*dt;

		real mi = 3*Pi/4*pow(Ri,static_cast<real>(3))*rho_[propId_i];
		real mj = 3*Pi/4*pow(Rj,static_cast<real>(3))*rho_[propId_j];
		real Reff = 1.0/(1/Ri + 1/Rj);

		real K_hertz = 4.0/3.0*prop.Yeff_*sqrt(Reff);
		real sqrt_meff_K_hertz = sqrt((mi*mj)/(mi+mj) * K_hertz);

		FCn = (static_cast<real>(-4.0/3.0) * prop.Yeff_ * sqrt(Reff)* pow(ovrlp_n,static_cast<real>(1.5)) - 
			   prop.ethan_*pow(ovrlp_n,static_cast<real>(0.5))*vrn)*Nij;

		FCt = (- static_cast<real>(16.0/3.0) * prop.Geff_ * sqrt(Reff*ovrlp_n) ) * history.overlap_t_;

		real ft = length(FCt);
		real ft_fric = prop.mu_ * length(FCn);

		// apply friction 
		if(ft > ft_fric)
		{
			if( length(history.overlap_t_) >0.0)
			{
				if constexpr (limited)
				{
					real kt = static_cast<real>(16.0/3.0) * prop.Geff_ * sqrt(Reff*ovrlp_n);
					FCt *= (ft_fric/ft);
                	history.overlap_t_ = - (FCt/kt);
				}
				else
				{
					FCt = (FCt/ft)*ft_fric;	
				}
				
			}
			else
			{
				FCt = 0.0;
			}
		}

	}
   

};

} //pFlow::CFModels

#endif //__nonLinearModCF_hpp__
