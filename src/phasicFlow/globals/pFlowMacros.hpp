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

#ifndef __pFlowMacros_hpp__
#define __pFlowMacros_hpp__

#include "phasicFlowConfig.H"

#ifdef __GNUC__
    #define FUNCTION_NAME __PRETTY_FUNCTION__
#else
    #define FUNCTION_NAME __func__
#endif

#ifdef __GNUC__
#  define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
#else
#  define UNUSED(x) UNUSED_ ## x
#endif

#define CONSUME_PARAM(x) (void)(x);

#ifdef __CUDACC__
    #define INLINE_FUNCTION_HD inline __host__ __device__
    #define INLINE_FUNCTION_D  inline __device__
    #define INLINE_FUNCTION_H  inline __host__ 
    #define LAMBDA_HD       [=]   __host__ __device__
    #define LAMBDA_D        [=]   __device__
    #define CLASS_LAMBDA_HD [=,*this] __host__ __device__
    #define FUNCTION_HD     __host__ __device__
    #define FUNCTION_H      __host__
    #define FUNCTION_D      __device__
#else
    #define INLINE_FUNCTION_HD inline 
    #define INLINE_FUNCTION_D  inline 
    #define INLINE_FUNCTION_H  inline 
    #define LAMBDA_HD [=]
    #define LAMBDA_D        [=] 
    #define CLASS_LAMBDA_HD [=,*this]
    #define FUNCTION_HD
    #define FUNCTION_H      
    #define FUNCTION_D      
#endif

#define INLINE_FUNCTION inline

#ifdef BUILD_SHARED_LIBS
    #define USE_INSTANTIATION 1
#else
    #define USE_INSTANTIATION 0
#endif


#define ForAll(i, container) for(auto i=0; i < container.size(); ++i)

#ifdef USE_STD_PARALLEL_ALG
static inline const bool useStdParallel__ = true;
#else
static inline const bool useStdParallel__ = false;
#endif


#endif //__pFlowMacros_hpp__
