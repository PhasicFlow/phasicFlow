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

#ifndef __cGNonLinearCF_hpp__
#define __cGNonLinearCF_hpp__

#include "types.hpp"

namespace pFlow::cfModels
{

template<bool limited=true>
class cGNonLinear
{
public:

	struct contactForceStorage
	{
		realx3 overlap_t_ = 0.0;
	};

	struct cGNonLinearProperties
	{		
		real 	Yeff_	= 1000000.0;
		real 	Geff_	= 8000000.0;
		real 	en_ 	= 1.0;
		real  	mu_ 	= 0.00001;

		INLINE_FUNCTION_HD
		cGNonLinearProperties(){}

		INLINE_FUNCTION_HD
		cGNonLinearProperties(real Yeff, real Geff, real en, real mu ):
			Yeff_(Yeff), Geff_(Geff), en_(en), mu_(mu)
		{}		

		INLINE_FUNCTION_HD
		cGNonLinearProperties(const cGNonLinearProperties&)=default;

		INLINE_FUNCTION_HD
		cGNonLinearProperties& operator=(const cGNonLinearProperties&)=default;

		INLINE_FUNCTION_HD
		~cGNonLinearProperties() = default;
	};

protected:

	using cGNonLinearArrayType = 	symArray<cGNonLinearProperties>;

	int32 						numMaterial_ = 0;

	ViewType1D<real>  		 	rho_;

	int32 						addDissipationModel_;

	cGNonLinearArrayType  		nonlinearProperties_;

	bool readNonLinearDictionary(const dictionary& dict)
	{
		auto Yeff = dict.getVal<realVector>("Yeff");
		auto Geff = dict.getVal<realVector>("Geff");
		auto nu = dict.getVal<realVector>("nu");
		auto en = dict.getVal<realVector>("en");
		auto mu = dict.getVal<realVector>("mu");

		auto nElem = Yeff.size();

		if(nElem != nu.size())
		{
			fatalErrorInFunction<<
			"sizes of Yeff("<<nElem<<") and nu("<<nu.size()<<") do not match.\n";
			return false;
		}

		if(nElem != en.size())
		{
			fatalErrorInFunction<<
			"sizes of Yeff("<<nElem<<") and en("<<en.size()<<") do not match.\n";
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
		if( !cGNonLinearArrayType::getN(nElem, nMat) )
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

		Vector<cGNonLinearProperties> prop("prop",nElem);
		ForAll(i,Yeff)
		{
			prop[i] = {Yeff[i], Geff[i], en[i], mu[i]};
		}

		nonlinearProperties_.assign(prop);

		auto adm = dict.getVal<word>("additionalDissipationModel");
		if(adm == "none")
		{
			addDissipationModel_ = 1;
		}
		else if(adm == "LU")
		{
			addDissipationModel_ = 2;
		}
		else if (adm == "GB")
		{
			addDissipationModel_ = 3;
		}
		else
		{
			addDissipationModel_ = 1;
		}

		return true;

	}

	static const char* modelName()
	{
		if constexpr (limited)
		{
			return "cGNonLinearLimited";
		}
		else
		{
			return "cGNonLinearNonLimited";
		}
		return "";
	}

public:

	TypeInfoNV(modelName());

	INLINE_FUNCTION_HD
	cGNonLinear(){}

	cGNonLinear(
		int32 nMaterial,
		const ViewType1D<real>& rho,
		const dictionary& dict)
	:
		numMaterial_(nMaterial),
		rho_("rho",nMaterial),
		nonlinearProperties_("cGNonLinearProperties",nMaterial)
	{

		Kokkos::deep_copy(rho_,rho);
		if(!readNonLinearDictionary(dict))
		{
			fatalExit;
		}
	}
	
	INLINE_FUNCTION_HD
	cGNonLinear(const cGNonLinear&) = default;

	INLINE_FUNCTION_HD
	cGNonLinear(cGNonLinear&&) = default;

	INLINE_FUNCTION_HD
	cGNonLinear& operator=(const cGNonLinear&) = default;

	INLINE_FUNCTION_HD
	cGNonLinear& operator=(cGNonLinear&&) = default;


	INLINE_FUNCTION_HD
	~cGNonLinear()=default;

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
		const real cGFi,
		const real cGFj,
		const real ovrlp_n,
		const realx3& Vr,
		const realx3& Nij,
		contactForceStorage& history,
		realx3& FCn,
		realx3& FCt
	)const
	{

		const auto prop = nonlinearProperties_(propId_i,propId_j);

		const real f = 2.0/( 1.0/cGFi + 1.0/cGFj );

		real vrn = dot(Vr, Nij);	
		realx3 Vt = Vr - vrn*Nij;

		history.overlap_t_ += Vt*dt;

		real mi = 3*Pi/4*pow(Ri,static_cast<real>(3))*rho_[propId_i];
		real mj = 3*Pi/4*pow(Rj,static_cast<real>(3))*rho_[propId_j];
		real Reff = 1.0/(1/Ri + 1/Rj);

		real K_hertz = 4.0/3.0*prop.Yeff_*sqrt(Reff);
		real sqrt_meff_K_hertz = sqrt((mi*mj)/(mi+mj) * K_hertz);

		real en = prop.en_;
		if (addDissipationModel_==2)
		{
			en = sqrt(1+((pow(prop.en_,2)-1)*f));
		}
		else if (addDissipationModel_==3)
		{
			
			en = exp((pow(f,1.5)*log(prop.en_)*sqrt( (1-((pow(log(prop.en_),2))/(pow(log(prop.en_),2)+pow(Pi,2))))/(1-(pow(f,3)*(pow(log(prop.en_),2))/(pow(log(prop.en_),2)+pow(Pi,2)))) ) ));
		}
		
		real Kn = static_cast<real>(4.0/3.0) * prop.Yeff_ * sqrt(Reff*ovrlp_n);

		real ethan_  = -2.0*log(en)*sqrt(Kn)/
			sqrt(pow(log(en),2.0)+ pow(Pi,2.0));

		FCn = ( - Kn*ovrlp_n - 
			   sqrt_meff_K_hertz*ethan_*pow(ovrlp_n,static_cast<real>(0.25))*vrn)*Nij;

		FCt = (f*f)*(- static_cast<real>(8.0) * prop.Geff_ * sqrt(Reff*ovrlp_n) ) * history.overlap_t_;

		real ft = length(FCt);
		real ft_fric = prop.mu_ * length(FCn);

		// apply friction 
		if(ft > ft_fric)
		{
			if( length(history.overlap_t_) >0.0)
			{
				if constexpr (limited)
				{
					real kt = static_cast<real>(8.0) * prop.Geff_ * sqrt(Reff*ovrlp_n);
					FCt *= (ft_fric/ft);
                	history.overlap_t_ = - FCt/(f*f*kt);
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

#endif
