#ifndef __forceChain_hpp__
#define __forceChain_hpp__

#include "dynamicPointStructure.hpp"
#include "systemControl.hpp"

namespace pFlow
{

class forceChain
{
private:

     bool        forceChainActive_ = false;
     
     bool        fieldsInitialized_ = false;
    
    /// forceChain force field
    realx3PointField_D              forceChainFCn_;
        
    /// Distance Colliding Particles
    realx3PointField_D            forceChainPos_;

    /// Pairs of Colliding Particles  
    realx3PointField_D            forceChainPairs_;

    /// pair Counter
    uint32PointField_D            pairCounter_;

public:

    forceChain(
        systemControl& control,
        dynamicPointStructure& dynPointStruct
    );
    
    // Destructor
    ~forceChain() = default;

    
    // Member functions
    void zeroFCn();
    void zeroDist();
    void zeroPairs();
    void zeroAll();
    
    // Initialize force chain from dictionary
    bool initializeFromDict(const dictionary& modelDict);
    
    // Reset pair counter (used before sphere-sphere interaction)
    void resetPairCounter();

    // Activate writing force chain
    void activateForceChain(systemControl& control,dynamicPointStructure& dynPointStruct);
  
    void addInteraction(uint32 i, uint32 j, const realx3& FCn, const realx3& xi, const realx3& xj);
    
    // Getters
    inline auto& forceChainFCn()
    {
        return forceChainFCn_;
    }

    inline const auto& forceChainFCn() const
    {
        return forceChainFCn_;
    }
        
    inline auto& forceChainPos()
    {
        return forceChainPos_;
    }

    inline const auto& forceChainPos() const
    {
        return forceChainPos_;
    }
	
    inline auto& forceChainPairs()
    {
        return forceChainPairs_;
    }

    inline const auto& forceChainPairs() const
    {
        return forceChainPairs_;
    }
	
    inline auto& pairCounter()
    {
        return pairCounter_;
    }

    inline const auto& pairCounter() const
    {
        return pairCounter_;
    }
    
    bool forceChainActive() const 
    {
        return forceChainActive_; 
    }

    bool fieldsInitialized() const 
    {  
       return fieldsInitialized_; 
    }
    
    bool isActive() const;
    ////
  

}; // forceChain

} // namespace pFlow

#endif //__forceChain_hpp__
