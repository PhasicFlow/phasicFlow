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

#ifndef __VectorDual_H__
#define __VectorDual_H__

#include "globalSettings.H"
#include "types.H"
#include "typeInfo.H"
#include "Vector.H"
#include "streams.H"

#include "KokkosTypes.H"
#include "ViewAlgorithms.H"

#ifndef __RESERVE__
#define __RESERVE__
	struct RESERVE{};
#endif


namespace pFlow
{

template<typename T, typename MemorySpace>
class VectorDual;


template<typename T, typename MemorySpace=void>
class VectorDual
{
public:
	
	using iterator        = T*;
  	
  	using constIterator   = const T*;

	using reference       = T&;
  	
  	using constReference  = const T&;

	using valueType       = T;
  	
  	using pointer         = T*;
  	
  	using constPointer    = const T*;  	
  	
  	using VectorType	  = VectorDual<T, MemorySpace>;

  	// dualViewType (view of data host and device)
  	using dualViewType 		= Kokkos::DualView<T*, MemorySpace>;

  	// mirror scape of device view
  	using hostMirrorSpace 	= typename dualViewType::host_mirror_space;
  	
  	// - viewType of data on device 
  	using deviceViewType 	= typename dualViewType::t_dev;

  	// - viewType of data on host
  	using hostViewType 		= typename dualViewType::t_host;
  
	using deviceType 		= typename deviceViewType::device_type;

	using hostType 			= typename hostViewType::device_type;

	// to be consistent 
  	using viewType 			= dualViewType;

  	// we assume device view is the primary side
  	using memory_space 		= typename viewType::memory_space;

	// we assume device view is the primary side
	using execution_space 	= typename deviceType::execution_space;

protected:

	size_t 			size_ = 0;

	size_t 			capacity_ = 0;

	dualViewType   	dualView_;

	mutable deviceViewType 	deviceSubView_;

	mutable hostViewType 	hostSubView_;

	mutable bool   			subViewsUpdated_ = false;
	
	static const inline real 	growthFactor_ = vectorGrowthFactor__;

	// is host accepsibble from device prespective 
	static constexpr bool  isHostAccessible_ =	
  	Kokkos::SpaceAccessibility<execution_space,Kokkos::HostSpace>::accessible;

	// host-device name 
	static inline const word hdName__ = 
		word(hostType::memory_space::name())+
		word(deviceType::memory_space::name());

	
	constexpr static inline const char* memoerySpaceName()
  	{
  		return hdName__.data();
  	}

	static INLINE_FUNCTION_H size_t evalCapacity(size_t n)
	{
		return static_cast<size_t>(n*growthFactor_+1);
	}

	// use actualCap = true only for reserve
	INLINE_FUNCTION_H void changeSize(size_t n, bool actualCap=false)
	{
		if(n >= capacity_ )
		{
			if(actualCap)
				capacity_ = n;
			else
				capacity_ = evalCapacity(n);
	
			dualView_.resize(capacity_);
			subViewsUpdated_ = false;
			syncViews();
			
		}
		if(!actualCap) 
		{
			setSize(n);
		}
	}

	INLINE_FUNCTION_H void setSize(size_t n) {
		size_ = n;
		subViewsUpdated_ = false;
	}

	// - seems strange, since we actually are updating values,
	//   but in fact the content of object is actually const.
	INLINE_FUNCTION_H void updateSubViews()const
	{
		if(subViewsUpdated_)return;
		
		hostSubView_   = Kokkos::subview(dualView_.h_view, Kokkos::make_pair(0,int(size_)));
		deviceSubView_ = Kokkos::subview(dualView_.d_view, Kokkos::make_pair(0,int(size_)));
		subViewsUpdated_ = true;
	}

public:

	// - type info
	TypeNameTemplateNV2("VectorDual", T, memoerySpaceName());

	//// - Constructors

		// - empty
		VectorDual()
		:
			VectorDual("VectorDual")
		{
		}

		VectorDual(const word& name)
		:
			size_(0),
			capacity_(2),
			dualView_(name,capacity_)
		{
			changeSize(size_);
		}

		// - with size n
		VectorDual(size_t n)
		:
			VectorDual("VectorDual",n)
		{}

		// - with name and size n
		VectorDual(const word& name, size_t n)
		:
			size_(n),
			capacity_(evalCapacity(n)),
			dualView_(name, capacity_)
		{
			changeSize(size_);
		}

		// with size and value, apply on both views
		VectorDual(size_t n, const T& val)
		:
			VectorDual("VectorDual", n , val)
		{}

		// with name, size and value, apply on both views
		VectorDual(const word& name, size_t n, const T& val)
		:
			VectorDual(name, n)
		{
			assign(n, val);
		}

		VectorDual(size_t cap, size_t n, RESERVE )
		:
			VectorDual("VectorDual", cap, n, RESERVE())
		{}

		VectorDual(const word& name, size_t cap, size_t n, RESERVE )
		:
			VectorDual(name)
		{
			reallocate(cap);
			setSize(n);
			
		}

		VectorDual(const Vector<T> & src)
		:
			VectorDual("VectorDual", src)
		{}

		VectorDual(const word& name, const Vector<T> & src)
		:
			VectorDual(name)
		{
			assign(src);
		}

		// - copy construct (perform deep copy)
		//   preserve the sync status of the views
		VectorDual(const VectorDual& src)
		:
			VectorDual(src.name(), src.capacity(), src.size(), RESERVE())
		{
			// transfer the sync status 
			if(src.hostRequiresSync())
			{
				modifyOnDevice();
			}else if( src.deviceRequiresSync())
			{
				modifyOnHost();
			}else
			{
				dualView_.clear_sync_state();
			}

			Kokkos::deep_copy(hostVector(), src.hostVector());
			Kokkos::deep_copy(deviceVector(), src.deviceVector());
		}

		// copy construct with new name 
		VectorDual(const word& name, const VectorDual& src)
		:
			VectorDual(name, src.capacity(), src.size(), RESERVE())
		{
			// transfer the sync status 
			if(src.hostRequiresSync())
			{
				modifyOnDevice();
			}else if( src.deviceRequiresSync())
			{
				modifyOnHost();
			}else
			{
				dualView_.clear_sync_state();
			}

			Kokkos::deep_copy(hostVector(), src.hostVector());
			Kokkos::deep_copy(deviceVector(), src.deviceVector());
		}

		// - copy assignment 
		VectorDual& operator = (const VectorDual& rhs)
		{
			if(&rhs == this) return *this;
				
			VectorDual temp(rhs);
			capacity_   = temp.capacity();
			setSize( temp.size());
			dualView_   = temp.dualView_;
			
			return *this;
		}

		// no move construct
		VectorDual(VectorDual&&) = delete;

		// no move assignment
		VectorDual& operator= (VectorDual&&) = delete;

		// - clone as a uniquePtr
		INLINE_FUNCTION_H uniquePtr<VectorDual> clone() const
		{
			return makeUnique<VectorDual>(*this);
		}

		// - clone as a pointer
		INLINE_FUNCTION_H VectorDual* clonePtr()const
		{
			return new VectorDual(*this);
		}


	//// - Methods
		// - return *this
		INLINE_FUNCTION_H
		VectorType& VectorField()
		{
			return *this;
		}

		// - return *this
		INLINE_FUNCTION_H
		const VectorType& VectorField()const
		{
			return *this;
		}

		// - Device vector
		INLINE_FUNCTION_H deviceViewType& deviceVectorAll(){
			return dualView_.d_view;
		}

		// - Device vector 
		INLINE_FUNCTION_H const deviceViewType& deviceVectorAll() const {
			return dualView_.d_view;
		}

		// - Host vector
		INLINE_FUNCTION_H hostViewType& hostVectorAll(){
			return dualView_.h_view;
		}

		// - Host Vector
		INLINE_FUNCTION_H const hostViewType& hostVectorAll() const {
			return dualView_.h_view;
		}

		INLINE_FUNCTION_H deviceViewType& deviceVector(){
			updateSubViews();
			return deviceSubView_;
		}

		INLINE_FUNCTION_H const deviceViewType& deviceVector() const{
			updateSubViews();
			return deviceSubView_;
		}

		INLINE_FUNCTION_H hostViewType& hostVector(){
			updateSubViews();
			return hostSubView_;
		}

		INLINE_FUNCTION_H const hostViewType& hostVector()const{
			updateSubViews();
			return hostSubView_;
		}

		INLINE_FUNCTION_H
		hostViewType hostVector(int32 start, int32 end)const
		{
			return Kokkos::subview(dualView_.h_view, Kokkos::make_pair(start,end));
		}

		INLINE_FUNCTION_H
		deviceViewType deviceVector(int32 start, int32 end)const
		{
			return Kokkos::subview(dualView_.d_view, Kokkos::make_pair(start,end));
		}
		
		INLINE_FUNCTION_H  const word  name()const
		{
			return dualView_.h_view.label();
		}

		INLINE_FUNCTION_H size_t size()const
		{
			return size_;
		}

		INLINE_FUNCTION_H size_t capacity()const
		{
			return capacity_;
		}
		
		INLINE_FUNCTION_H bool empty()const
		{
			return size_==0;
		}

		// - reserve capacity for vector 
		//   preserve the content
		INLINE_FUNCTION_H void reserve(size_t cap)
		{
			changeSize(cap, true);
		}
		// resize the views 
		// no syncronization occurs
		INLINE_FUNCTION_H void resize(size_t n)
		{
			changeSize(n);
		}

		INLINE_FUNCTION_H void reallocate(size_t cap)
		{
			capacity_ = cap;
			setSize(0);
			dualView_.realloc(cap);
			dualView_.clear_sync_state();
			
			
		}

		INLINE_FUNCTION_H void resizeSync(size_t n)
		{
			changeSize(n);		
			syncViews();
		}

		// resize the view and assign value to the most recent view (most updated)
		// no syncronization occurs
		INLINE_FUNCTION_H void resize(size_t n, const T& val) {
			assign(n, val);
		}

		// resize the view and assign value to the most recent view (most updated)
		// and syncronize both views 
		INLINE_FUNCTION_H void resizeSync(size_t n, const T& val){
			assign(n,val);
			syncViews();
		}

		INLINE_FUNCTION_H void clear() {
			 
			 setSize(0);
			 dualView_.clear_sync_state();
		}

		// - fill both views with val
		INLINE_FUNCTION_H void fill(const T& val)
		{
			if(empty())return;
			Kokkos::deep_copy(deviceVector(),val);
			Kokkos::deep_copy(hostVector(),val);
			dualView_.clear_sync_state();
		}

		INLINE_FUNCTION_H void fillHost(const T& val)
		{
			if(empty())return;
			Kokkos::deep_copy(hostVector(),val);
			modifyOnHost();
		}

		INLINE_FUNCTION_H void fillDevice(const T& val)
		{
			if(empty())return;
			Kokkos::deep_copy(deviceVector(),val);
			modifyOnDevice();
		}

		// - host calls only
		// - assign n first elements to val
		//   resize views
		//   assign value to both sides (device&host)
		FUNCTION_H void assign(size_t n, const T& val)
		{
			
			if( n>= capacity_ )
			{
				reallocate(evalCapacity(n));
			}
			setSize(n);
			fill(val);
		}

		// - host calls only
		// - assign source vector
		//   resize views
		//   assign to both sides (device&host)
		FUNCTION_H void assign(const Vector<T>& src)
		{
			auto srcSize = src.size();
			if( capacity() < srcSize ) 
			{
				reallocate( evalCapacity(srcSize) );
			}

			setSize(0);
			dualView_.clear_sync_state();
			for( auto& elem:src )
			{
				this->push_back(elem);
			}
			syncViews();
		}

		// TODO: this part may be implemented in parallel 
		bool deleteElement
		(
			const Vector<label>& indices
		)
		{
			if( indices.size() == 0 )return true;

			if( *(indices.end()-1) >= size() ) return false;
			
			auto oldSize = this->size();
			auto nextInd = indices.begin();
			label j = indices[0];
			for(label i=indices[0]; i < oldSize; ++i)
			{
				if( nextInd != indices.end() && i == *nextInd )
				{
					++nextInd;
				}
				else
				{
					dualView_.h_view[j] = dualView_.h_view[i];
					++j;	
				}				
			}
			
			setSize( oldSize - indices.size() );
			modifyOnHost();

			// TODO: deep_copy should be changed to a range shorter than the vector size
			syncViews();

			return true;
		}

		INLINE_FUNCTION_H
		bool insertSetElement(const int32IndexContainer& indices, const T& val)
		{
			if(indices.size() == 0)return true;
			
			auto maxInd = indices.max();

			if(this->empty() || maxInd > size()-1 )
			{
				resize(maxInd+1);
			} 
			fillSelected(hostVectorAll(), indices.hostView(), indices.size(), val);
			
			auto dIndices = indices.deviceView();
			auto dVals = deviceVectorAll();

			Kokkos::parallel_for(
				"fillSelected",
				indices.size(),
				LAMBDA_HD(int32 i){
				dVals[dIndices[i]]= val;
			
				});
			Kokkos::fence();

			return true;

		}

		template<typename side=HostSide>
		INLINE_FUNCTION_H
		bool insertSetElement
		(
			const int32IndexContainer& indices,
			const Vector<T>& vals
		)
		{
			if(indices.size() == 0)return true;
			if(indices.size() != vals.size())return false;

			auto maxInd = indices.max();
			auto minInd = indices.min(); 
			
			if(this->empty() || maxInd > size()-1 )
			{
				resize(maxInd+1);
			} 

			if constexpr (std::is_same<side,HostSide>::value )
			{

				hostViewType1D<T> hVecVals( const_cast<T*>(vals.data()), vals.size());
				//fillSelected(hostVector(), indices.hostView(), hVecVals, indices.size());

				pFlow::algorithms::KOKKOS::fillSelected<T, int32, DefaultHostExecutionSpace>(
					hostVectorAll().data(),
					indices.hostView().data(),
					hVecVals.data(),
					indices.size());

				modifyOnHost();
			
				syncViews(minInd, maxInd+1);
			}
			else
			{
				
				pFlow::hostViewType1D<T> hVecVals( const_cast<T*>(vals.data()), vals.size());
				pFlow::deviceViewType1D<T> dVecVals("dVecVals", indices.size());
				
				Kokkos::deep_copy(dVecVals, hVecVals);
				

				//fillSelected(deviceVector(), indices.deviceView(), dVecVals, indices.size());
				pFlow::algorithms::KOKKOS::fillSelected<T, int32, execution_space>(
					deviceVectorAll().data(),
					indices.deviceView().data(),
					dVecVals.data(),
					indices.size());

				modifyOnDevice();
				
				// TODO: deep_copy should be changed to a range shorter than the vector size
				syncViews(minInd, maxInd+1);

			}

			return true;
			
		}

		template<typename side=HostSide>
		INLINE_FUNCTION_H
		bool insertSetElement(const Vector<int32>& indices, const T& val)
		{
			if(indices.size() == 0)return true;
			
			auto maxInd = max(indices);
			auto minInd = min(indices);

			if(this->empty() || maxInd > size()-1 )
			{
				resize(maxInd+1);
			} 
					
			if constexpr (std::is_same<side,HostSide>::value )
			{
				hostViewType1D<int32> hVecInd( const_cast<int32*>(indices.data()), indices.size());
				fillSelected( hostVectorAll(), hVecInd, indices.size(), val);

				modifyOnHost();
				syncViews(minInd, maxInd+1);

				return true;
			}
			else
			{
				
				hostViewType1D<int32> hVecInd( const_cast<int32*>(indices.data()), indices.size());
				deviceViewType1D<int32> dVecInd("dVecInd", indices.size());
				Kokkos::deep_copy(dVecInd, hVecInd);
				fillSelected(deviceVectorAll(), dVecInd, indices.size(), val);
				
				modifyOnDevice();

				// TODO: deep_copy should be changed to a range shorter than the vector size
				syncViews(minInd, maxInd+1);
				return true;
				
			}
		
		return false;

		}

		template<typename side=HostSide>
		INLINE_FUNCTION_H
		bool insertSetElement
		(
			const Vector<int32>& indices,
			const Vector<T>& vals
		)
		{
			if(indices.size() == 0)return true;
			if(indices.size() != vals.size())return false;

			auto maxInd = max(indices);
			auto minInd = min(indices); 
			
			if(this->empty() || maxInd > size()-1 )
			{
				resize(maxInd+1);
			} 

			if constexpr (std::is_same<side,HostSide>::value )
			{
				hostViewType1D<int32> hVecInd( const_cast<int32*>(indices.data()), indices.size());
				hostViewType1D<T> hVecVals( const_cast<T*>(vals.data()), vals.size());

				fillSelected(hostVectorAll(), hVecInd, hVecVals, indices.size());

				modifyOnHost();
			
				syncViews(minInd, maxInd+1);
			}
			else
			{
				
				pFlow::hostViewType1D<int32> hVecInd( const_cast<int32*>(indices.data()), indices.size());
				pFlow::deviceViewType1D<int32> dVecInd("dVecInd", indices.size());

				pFlow::hostViewType1D<T> hVecVals( const_cast<T*>(vals.data()), vals.size());
				pFlow::deviceViewType1D<T> dVecVals("dVecVals", indices.size());
				
				Kokkos::deep_copy(dVecVals, hVecVals);
				Kokkos::deep_copy(dVecInd, hVecInd);

				fillSelected(deviceVectorAll(), dVecInd, dVecVals, indices.size());

				modifyOnDevice();
				
				// TODO: deep_copy should be changed to a range shorter than the vector size
				syncViews(minInd, maxInd+1);

			}

			return true;
			
		}

		// push a new element at the end
		// resize if necessary
		// first sycn to host side
		void push_back(const T& val)
		{
			
			syncToHost();
			modifyOnHost();
			
			if(size_ == capacity_) changeSize(capacity_);
			data()[size_++] = val;
			subViewsUpdated_ = false;
			
		}

		INLINE_FUNCTION_H pointer data(){
			return dualView_.h_view.data();
		}

		INLINE_FUNCTION_H constPointer data()const{
			return dualView_.h_view.data();
		}

		// host call
		// returns begin iterator 
		INLINE_FUNCTION_H iterator begin(){
			return data();
		}

		// host call
		// returns begin iterator 
		INLINE_FUNCTION_H constIterator begin()const {
			return data();
		}

		// host call
		// returns end iterator 
		INLINE_FUNCTION_H iterator end(){
			return size_ > 0 ? data() + size_: data();
		}

		// host call
		// returns end iterator 
		INLINE_FUNCTION_H constIterator end()const{
			return size_ > 0 ? data() + size_: data();
		}

		INLINE_FUNCTION_H reference operator[](label i){
			return dualView_.h_view[i];
		}

		INLINE_FUNCTION_H constReference operator[](label i)const{
			return dualView_.h_view[i];
		}

		
	//// - managing sync between views

		INLINE_FUNCTION_H void modifyOnHost()
		{
			dualView_.modify_host();
		}

		INLINE_FUNCTION_H void modifyOnDevice()
		{
			dualView_.modify_device();
		}
		
		INLINE_FUNCTION_H bool hostRequiresSync()const
		{
			return dualView_.template need_sync<hostType>();
		}

		INLINE_FUNCTION_H bool deviceRequiresSync()const
		{
			return dualView_.template need_sync<deviceType>();
		}

		INLINE_FUNCTION_H bool areViewsSimilar()const
		{
			return std::is_same<hostType,deviceType>::value;
		}

		// - copy from host to device 
		//   set both views to updated
		INLINE_FUNCTION_H void copyHostToDevice()
		{
			if(empty())return;
			
			Kokkos::deep_copy(deviceVector(), hostVector());
			dualView_.clear_sync_state();
		}

		INLINE_FUNCTION_H 
		void copyHostToDevice(int32 start, int32 end, bool setUpdated = true)
		{
			if(empty())return;
			Kokkos::deep_copy(deviceVector(start, end), hostVector(start, end));
			if(setUpdated)
				dualView_.clear_sync_state();
		}

		// - copy from device to host
		//   set both views to updated
		INLINE_FUNCTION_H void copyDeviceToHost()
		{
			if(empty())return;
			Kokkos::deep_copy(hostVector(), deviceVector());
			dualView_.clear_sync_state();
		}

		INLINE_FUNCTION_H 
		void copyDeviceToHost(int32 start, int32 end, bool setUpdated = true)
		{
			if(empty())return;
			Kokkos::deep_copy(hostVector(start, end), deviceVector(start, end));
			if(setUpdated)
				dualView_.clear_sync_state();
		}

		INLINE_FUNCTION_H void syncToHost()
		{
			if(hostRequiresSync())
			{
				copyDeviceToHost();
			}
		}

		INLINE_FUNCTION_H void syncToDevice()
		{
			if(deviceRequiresSync())
			{
				copyHostToDevice();
			}	
		}
		// - check which side requires update and 
		//   apply the update 
		INLINE_FUNCTION_H void syncViews()
		{
			if(deviceRequiresSync())
			{
				copyHostToDevice();
			}
			else if(hostRequiresSync())
			{
				copyDeviceToHost();
			}
		}

		INLINE_FUNCTION_H void syncViews(int32 start, int32 end)
		{
			if(deviceRequiresSync())
			{
				copyHostToDevice(start, end);
			}
			else if(hostRequiresSync())
			{
				copyDeviceToHost(start, end);
			}
		}

	//// - IO operations
		FUNCTION_H
		bool read(iIstream& is)
		{
			Vector<T> vecFromFile;
			if( !vecFromFile.read(is) ) return false;
			
			this->assign(vecFromFile);

			return true;
		}

		FUNCTION_H
		bool write(iOstream& os) const
		{
			// since the object should be const, no way to syncViews
			
			Vector<T, noConstructAllocator<T>> vecToFile(this->size());
			hostViewType1D<T> mirror(vecToFile.data(), vecToFile.size());

			
			if(hostRequiresSync()) // device is updated
			{
				//const auto dVec = Kokkos::subview(dualView_.d_view, Kokkos::make_pair(0,int(size_)));
				Kokkos::deep_copy(mirror,deviceVector());
			}
			else // either host is updated or both sides are syncronized
			{
				//const auto hVec = Kokkos::subview(dualView_.h_view, Kokkos::make_pair(0,int(size_)));
				Kokkos::deep_copy(mirror,hostVector());
			}				
			return vecToFile.write(os);
		}
};

template<typename T, typename memory_space>
inline iIstream& operator >> (iIstream & is, VectorDual<T, memory_space> & ivec )
{
	if( !ivec.read(is) )
	{
		ioErrorInFile (is.name(), is.lineNumber());
		fatalExit;
	}
	return is;
}

template<typename T, typename memory_space>
inline iOstream& operator << (iOstream& os, const VectorDual<T, memory_space>& ovec )
{
	
	if( !ovec.write(os) )
	{
		ioErrorInFile(os.name(), os.lineNumber());
		fatalExit;
	}

	return os; 
}


} // pFlow

#include "VectorDualAlgorithms.H"


#endif //__VectorDual_H__

