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

#ifndef __sphereInteractionKernels_hpp__
#define __sphereInteractionKernels_hpp__


#include "sphereTriSurfaceContact.hpp"

namespace pFlow::sphereInteractionKernels
{

template<
	typename ContactForceModel, 
	typename ContactListType>
struct ppInteractionFunctor
{

	using PairType = typename ContactListType::PairType;
	using ValueType = typename ContactListType::ValueType;

	real dt_;

	ContactForceModel forceModel_;
	ContactListType   tobeFilled_;

	deviceViewType1D<real>  		diam_;
	deviceViewType1D<uint32> 		propId_;
	deviceViewType1D<realx3>  		pos_;
	deviceViewType1D<realx3>  		lVel_;
	deviceViewType1D<realx3>  		rVel_;
	deviceViewType1D<realx3>	cForce_;
	deviceViewType1D<realx3>	cTorque_;


	ppInteractionFunctor(
		real dt,
		ContactForceModel 		forceModel,
		ContactListType 		tobeFilled,
		deviceViewType1D<real>  		diam,
		deviceViewType1D<uint32> 		propId,
		deviceViewType1D<realx3>  		pos,
		deviceViewType1D<realx3>  		lVel,
		deviceViewType1D<realx3>  		rVel,
		deviceViewType1D<realx3>		cForce,
		deviceViewType1D<realx3> 		cTorque )
	:
		dt_(dt),
		forceModel_(forceModel),
		tobeFilled_(tobeFilled),
		diam_(diam),
		propId_(propId),
		pos_(pos),
		lVel_(lVel),
		rVel_(rVel),
		cForce_(cForce), // this is converted to an atomic vector 
		cTorque_(cTorque) // this is converted to an atomic vector 
	{}

	INLINE_FUNCTION_HD
	void operator()(const uint32 n)const
	{
		
		if(!tobeFilled_.isValid(n))return;

		auto [i,j] = tobeFilled_.getPair(n);
		
		real Ri = 0.5*diam_[i];
		real Rj = 0.5*diam_[j];
		realx3 xi = pos_[i];
		realx3 xj = pos_[j];
		real dist = length(xj-xi);
		real ovrlp = (Ri+Rj) - dist;
		
		if( ovrlp >0.0 )
		{
			
			auto Vi = lVel_[i];
			auto Vj = lVel_[j];
			auto wi = rVel_[i];
			auto wj = rVel_[j];
			auto Nij = (xj-xi)/dist;
			auto Vr = Vi - Vj + cross((Ri*wi+Rj*wj), Nij);
			
			auto history = tobeFilled_.getValue(n);

			int32 propId_i = propId_[i];
			int32 propId_j = propId_[j];

			realx3 FCn, FCt, Mri, Mrj, Mij, Mji;

			// calculates contact force 
			forceModel_.contactForce(
				dt_, i, j,
				propId_i, propId_j,
				Ri, Rj,
				ovrlp,
				Vr, Nij,
				history,
				FCn, FCt
				);

			forceModel_.rollingFriction(
				dt_, i, j,
				propId_i, propId_j,
				Ri, Rj,
				wi, wj,
				Nij,
				FCn,
				Mri, Mrj
				);

			auto M = cross(Nij,FCt);
			Mij = Ri*M+Mri;
			Mji = Rj*M+Mrj;
			
			auto FC = FCn + FCt;
			
			Kokkos::atomic_add(&cForce_[i].x_,FC.x_);
			Kokkos::atomic_add(&cForce_[i].y_,FC.y_);
			Kokkos::atomic_add(&cForce_[i].z_,FC.z_);

			Kokkos::atomic_add(&cForce_[j].x_,-FC.x_);
			Kokkos::atomic_add(&cForce_[j].y_,-FC.y_);
			Kokkos::atomic_add(&cForce_[j].z_,-FC.z_);

			Kokkos::atomic_add(&cTorque_[i].x_, Mij.x_);
			Kokkos::atomic_add(&cTorque_[i].y_, Mij.y_);
			Kokkos::atomic_add(&cTorque_[i].z_, Mij.z_);

			Kokkos::atomic_add(&cTorque_[j].x_, Mji.x_);
			Kokkos::atomic_add(&cTorque_[j].y_, Mji.y_);
			Kokkos::atomic_add(&cTorque_[j].z_, Mji.z_);
			

			tobeFilled_.setValue(n,history);

		}
		else
		{
			tobeFilled_.setValue(n, ValueType());
		}

	}
};


template<
	typename ContactForceModel, 
	typename ContactListType,
	typename TraingleAccessor,
	typename MotionModel>
struct pwInteractionFunctor
{
	using PairType = typename ContactListType::PairType;
	using ValueType = typename ContactListType::ValueType;

	real dt_;

	ContactForceModel 	forceModel_;
	ContactListType   	tobeFilled_;

	TraingleAccessor 		triangles_;
	MotionModel  			motionModel_;

	deviceViewType1D<real>  	diam_;
	deviceViewType1D<uint32>	propId_;
	deviceViewType1D<realx3>  	pos_;
	deviceViewType1D<realx3>  	lVel_;
	deviceViewType1D<realx3>  	rVel_;
	deviceViewType1D<realx3>	cForce_;
	deviceViewType1D<realx3>	cTorque_;
	deviceViewType1D<uint32> 	wTriMotionIndex_;
	deviceViewType1D<uint32>    wPropId_;
	deviceViewType1D<realx3>    wCForce_;
	

	pwInteractionFunctor(
		real dt,
		ContactForceModel 		forceModel,
		ContactListType   		tobeFilled,
		TraingleAccessor		triangles,
		MotionModel 			motionModel ,
		deviceViewType1D<real>  	diam ,
		deviceViewType1D<uint32>  	propId,
		deviceViewType1D<realx3>   	pos ,
		deviceViewType1D<realx3>   	lVel,
		deviceViewType1D<realx3>   	rVel,
		deviceViewType1D<realx3> 	cForce,
		deviceViewType1D<realx3> 	cTorque ,
		deviceViewType1D<uint32> 	wTriMotionIndex,
		deviceViewType1D<uint32>    wPropId,
		deviceViewType1D<realx3>    wCForce)
	:
		dt_(dt),
		forceModel_(forceModel),
		tobeFilled_(tobeFilled),
		triangles_(triangles) ,
		motionModel_(motionModel) ,
		diam_(diam) ,
		propId_(propId),
		pos_(pos) ,
		lVel_(lVel),
		rVel_(rVel) ,
		cForce_(cForce),
		cTorque_(cTorque) ,
		wTriMotionIndex_(wTriMotionIndex) ,
		wPropId_(wPropId),
		wCForce_(wCForce)
	{}

	INLINE_FUNCTION_HD
	void operator()(const int32 n)const
	{

		if(!tobeFilled_.isValid(n))return;

		auto [i,tj] = tobeFilled_.getPair(n);
		
		real Ri = 0.5*diam_[i];
		real Rj = 10000.0;
		realx3 xi = pos_[i];
		
		realx3x3 tri = triangles_(tj); 
		real ovrlp;
		realx3 Nij, cp;

		if(	pFlow::sphTriInteraction::isSphereInContactBothSides(
			tri, xi, Ri, ovrlp, Nij, cp) )
		{
			auto  Vi = lVel_[i];
			auto wi = rVel_[i];

			int32 mInd	= wTriMotionIndex_[tj];
			
			auto Vw = motionModel_(mInd, cp);
			
			//output<< "par-wall index "<< i<<" - "<< tj<<endl;
			
			auto Vr = Vi - Vw + cross(Ri*wi, Nij);
			
			auto history = tobeFilled_.getValue(n);

			int32 propId_i = propId_[i];
			int32 wPropId_j = wPropId_[tj];

			realx3 FCn, FCt, Mri, Mrj, Mij;
			//output<< "before "<<history.overlap_t_<<endl;
			// calculates contact force 
			forceModel_.contactForce(
				dt_, i, tj,
				propId_i, wPropId_j,
				Ri, Rj,
				ovrlp,
				Vr, Nij,
				history,
				FCn, FCt
				);

			//output<< "after "<<history.overlap_t_<<endl;

			forceModel_.rollingFriction(
				dt_, i, tj,
				propId_i, wPropId_j,
				Ri, Rj,
				wi, 0.0,
				Nij,
				FCn,
				Mri, Mrj
				);

			auto M = cross(Nij,FCt);
			Mij = Ri*M+Mri;
			//output<< "FCt = "<<FCt <<endl;
			//output<< "FCn = "<<FCn <<endl;
			//output<<"propId i, tj"<< propId_i << " "<<wPropId_j<<endl;
			//output<<"par i , wj"<< i<<" "<< tj<<endl;

			auto FC = FCn + FCt;

			Kokkos::atomic_add(&cForce_[i].x_,FC.x_);
			Kokkos::atomic_add(&cForce_[i].y_,FC.y_);
			Kokkos::atomic_add(&cForce_[i].z_,FC.z_);

			Kokkos::atomic_add(&wCForce_[tj].x_,-FC.x_);
			Kokkos::atomic_add(&wCForce_[tj].y_,-FC.y_);
			Kokkos::atomic_add(&wCForce_[tj].z_,-FC.z_);

			
			Kokkos::atomic_add(&cTorque_[i].x_, Mij.x_);
			Kokkos::atomic_add(&cTorque_[i].y_, Mij.y_);
			Kokkos::atomic_add(&cTorque_[i].z_, Mij.z_);

			tobeFilled_.setValue(n,history);

		}
		else
		{
			tobeFilled_.setValue(n,ValueType());
		}
		
	}

};

} 

#endif //__sphereInteractionKernels_hpp__