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

#ifndef __grainRolling_hpp__
#define __grainRolling_hpp__


namespace pFlow::cfModels
{

template<typename contactForceModel>
class grainRolling
:
	public contactForceModel
{
public:
	
	using contactForceStorage = 
		typename contactForceModel::contactForceStorage;


	realSymArray_D  	mur_;

	bool readGrainDict(const dictionary& dict)
	{
		auto mur = dict.getVal<realVector>("mur");

		uint32 nMat;

		if(!realSymArray_D::getN(mur.size(),nMat) || nMat != this->numMaterial())
		{
			fatalErrorInFunction<<
			"wrong number of values supplied in mur. \n";
			return false;
		}

		mur_.assign(mur);

		return true; 
	}

public:

	TypeInfoNV(word("normal<"+contactForceModel::TYPENAME()+">"));
	

	grainRolling(int32 nMaterial, const ViewType1D<real>& rho, const dictionary& dict)
	:
		contactForceModel(nMaterial, rho, dict),
		mur_("mur", nMaterial)
	{
		if(!readGrainDict(dict))
		{
			fatalExit;
		}
	}

	INLINE_FUNCTION_HD
	void rollingFriction
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
		const realx3& wi,
		const realx3& wj,
		const realx3& Nij,
		const realx3& FCn,
		realx3& Mri,
		realx3& Mrj
	)const
	{

		realx3 w_hat = wi-wj;
		real w_hat_mag = length(w_hat);

		if( !equal(w_hat_mag,zero) )
			w_hat /= w_hat_mag;
		else
			w_hat = zero;

		auto Reff = (Ri*Rj)/(Ri+Rj);

		Mri = ( -mur_(propId_i,propId_j) *length(FCn) * Reff ) * w_hat ;
       
       	//removing the normal part 
       	// Mri = Mri - ( (Mri .dot. nij)*nij )

       	Mrj = -Mri;
	}


};

}
 
#endif 
