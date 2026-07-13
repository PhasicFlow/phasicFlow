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
-----------------------------------------------------------------------------*/

#include "KokkosTypes.hpp"
#include "DEMSystem.hpp"

namespace pFlow 
{

// ========================================================================= //
// Section 1: Constructors and Destructors
// ========================================================================= //

DEMSystem::DEMSystem
(
    word                    demSystemName,
    const std::vector<box>& domains, 
    int                     argc, 
    char* argv[]
)
: 
    ControlDict_()
{
    // ------------------------------------------------------------------------
    // Kokkos Bootstrapping
    // Probes the hardware, allocates thread pools (CPU), and locks GPU resources.
    // ------------------------------------------------------------------------
    REPORT(0) << "Initializing host/device execution spaces . . . \n";
    REPORT(1) << "Host execution space is "   
              << Green_Text(DefaultHostExecutionSpace::name()) << END_REPORT;
    REPORT(1) << "Device execution space is " 
              << Green_Text(DefaultExecutionSpace::name())     << END_REPORT;

    Kokkos::initialize(argc, argv); 

    // ------------------------------------------------------------------------
    // System Control & Profiling Initialization
    // ------------------------------------------------------------------------
    REPORT(0) << "\nCreating Control repository . . ." << END_REPORT;
    Control_ = makeUnique<systemControl>
    (
        ControlDict_.startTime(),
        ControlDict_.endTime(),
        ControlDict_.saveInterval(),
        ControlDict_.startTimeName()
    );

    timers_ = makeUnique<Timers>(demSystemName, &Control_().timers());
}

DEMSystem::~DEMSystem()
{
    // Explicitly reset the control repository to release its memory before 
    // shutting down the execution spaces.
    Control_.reset();

    // ------------------------------------------------------------------------
    // Kokkos Shutdown
    // Safely flushes memory buffers and releases hardware locks.
    // ------------------------------------------------------------------------
    output << "\nFinalizing host/device execution space ...." << endl;
    Kokkos::finalize();
}

// ========================================================================= //
// Section 2: Factory Pattern Implementation
// ========================================================================= //

uniquePtr<DEMSystem> DEMSystem::create
(
    word                    demSystemName,
    const std::vector<box>& domains, 
    int                     argc, 
    char* argv[],
    bool                    requireRVel
)
{
    // Search the macro-generated hash map to see if a class matching 
    // the requested string has been registered.
    if (wordvCtorSelector_.search(demSystemName))
    {
        // Invoke the registered constructor function via function pointer
        return wordvCtorSelector_[demSystemName]
        (
            demSystemName, 
            domains, 
            argc, 
            argv, 
            requireRVel
        );
    }
    else
    {
        // Graceful error handling: List available options if a typo is made
        printKeys
        ( 
            fatalError << "Ctor Selector " << demSystemName << " does not exist. \n"
                       << "Available ones are: \n\n",
            wordvCtorSelector_
        );
        return nullptr;
    }

    return nullptr;
}

} // namespace pFlow


