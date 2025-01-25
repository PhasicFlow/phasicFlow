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

#ifndef __cGAbsoluteLinearCF_hpp__
#define __cGAbsoluteLinearCF_hpp__

#include "types.hpp"
#include "symArrays.hpp"


namespace pFlow::cfModels
{

template<bool limited=true>
class cGAbsoluteLinear
{
public:

	struct contactForceStorage
	{
		realx3 overlap_t_ = 0.0;
	};
	
	
	struct linearProperties
	{		
		real 	kn_ 	= 1000.0;
		real 	kt_	= 800.0;
		real 	en_ 	= 0.0;
		real 	ethat_ 	= 0.0;
		real    mu_ 	= 0.00001;

		INLINE_FUNCTION_HD
		linearProperties(){}

		INLINE_FUNCTION_HD
		linearProperties(real kn, real kt, real en, real etha_t, real mu ):
			kn_(kn), kt_(kt), en_(en),ethat_(etha_t), mu_(mu) 
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

	LinearArrayType  	linearProperties_;
	
	int32 		addDissipationModel_;	
	

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


		realVector etha_t("etha_t", nElem);

		ForAll(i , kn)
		{

			etha_t[i] = -2.0*log( et[i])*sqrt(kt[i]*2/7) /
					sqrt(log(pow(et[i],2))+ pow(Pi,2));
		}

		Vector<linearProperties> prop("prop", nElem);
		ForAll(i,kn)
		{
			prop[i] = {kn[i], kt[i], en[i], etha_t[i], mu[i] };
		}

		linearProperties_.assign(prop);
		
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
			return "cGAbsoluteLinearLimited";
		}
		else
		{
			return "cGAbsoluteLinearNonLimited";
		}
		return "";
	}

public:


	TypeInfoNV(modelName());

	INLINE_FUNCTION_HD
	cGAbsoluteLinear(){}

	cGAbsoluteLinear(int32 nMaterial, const ViewType1D<real>& rho, const dictionary& dict)
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
	cGAbsoluteLinear(const cGAbsoluteLinear&) = default;

	INLINE_FUNCTION_HD
	cGAbsoluteLinear(cGAbsoluteLinear&&) = default;

	INLINE_FUNCTION_HD
	cGAbsoluteLinear& operator=(const cGAbsoluteLinear&) = default;

	INLINE_FUNCTION_HD
	cGAbsoluteLinear& operator=(cGAbsoluteLinear&&) = default;


	INLINE_FUNCTION_HD
	~cGAbsoluteLinear()=default;

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

		
		auto prop = linearProperties_(propId_i,propId_j);		


		real f_ = ( cGFi + cGFj )/2 ;


		real vrn = dot(Vr, Nij);	
		realx3 Vt = Vr - vrn*Nij;

		history.overlap_t_ += Vt*dt;

		real mi = 3*Pi/4*pow(Ri,3)*rho_[propId_i];
		real mj = 3*Pi/4*pow(Rj,3)*rho_[propId_j];

		real sqrt_meff = sqrt((mi*mj)/(mi+mj));


		// disipation model
		if (addDissipationModel_==2)
		{
			prop.en_ = sqrt(1+((pow(prop.en_,2)-1)*f_));
		}
		else if (addDissipationModel_==3)
		{
			auto pie =3.14;
			prop.en_ = exp((pow(f_,static_cast<real>(1.5))*log(prop.en_)*sqrt( (1-((pow(log(prop.en_),2))/(pow(log(prop.en_),2)+pow(pie,2))))/(1-(pow(f_,3)*(pow(log(prop.en_),2))/(pow(log(prop.en_),2)+pow(pie,2)))) ) ));
		}

		real ethan_  = -2.0*log(prop.en_)*sqrt(prop.kn_)/
					sqrt(pow(log(prop.en_),2)+ pow(Pi,2));

			//REPORT(0)<<"\n en n is  :  "<<END_REPORT;
			//REPORT(0)<< prop.en_ <<END_REPORT;

		FCn = (  -pow(f_,3)*prop.kn_ * ovrlp_n - sqrt_meff * pow(f_,static_cast<real>(1.5)) * ethan_ * vrn)*Nij;
		FCt = ( -pow(f_,3)*prop.kt_ * history.overlap_t_ - sqrt_meff * pow(f_,static_cast<real>(1.5)) *  prop.ethat_*Vt);
				
		real ft = length(FCt);
		real ft_fric = prop.mu_ * length(FCn);
		
		if(ft > ft_fric)
		{
			if( length(history.overlap_t_) >zero)
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
