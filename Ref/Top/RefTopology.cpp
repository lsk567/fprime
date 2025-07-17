// ======================================================================
// \title  Topology.cpp
// \author mstarch
// \brief cpp file containing the topology instantiation code
//
// \copyright
// Copyright 2009-2022, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
// ======================================================================
// Provides access to autocoded functions
#include <Ref/Top/RefTopologyAc.hpp>

// Necessary project-specified types
#include <Fw/Types/MallocAllocator.hpp>
#include <Os/Console.hpp>

// Used for 1Hz synthetic cycling
#include <Os/Mutex.hpp>

// Allows easy reference to objects in FPP/autocoder required namespaces
using namespace Ref;

// Instantiate a system logger that will handle Fw::Logger::log calls
Os::Console logger;

// The reference topology divides the incoming clock signal (1kHz) into sub-signals: 1kHz, 1kHz, and 1kHz and
// zero offset for all the dividers
Svc::RateGroupDriver::DividerSet rateGroupDivisorsSet{{{1, 0}, {1, 0}, {1, 0}}};

// Rate groups may supply a context token to each of the attached children whose purpose is set by the project. The
// reference topology sets each token to zero as these contexts are unused in this project.
U32 rateGroup1Context[Svc::ActivePhaser::CONNECTION_COUNT_MAX] = {};
U32 rateGroup2Context[Svc::ActivePhaser::CONNECTION_COUNT_MAX] = {};
U32 rateGroup3Context[Svc::ActivePhaser::CONNECTION_COUNT_MAX] = {};

/**
 * \brief configure/setup components in project-specific way
 *
 * This is a *helper* function which configures/sets up each component requiring project specific input. This includes
 * allocating resources, passing-in arguments, etc. This function may be inlined into the topology setup function if
 * desired, but is extracted here for clarity.
 */
void configureTopology() {
    // Rate group driver needs a divisor list
    rateGroupDriverComp.configure(rateGroupDivisorsSet);

    // Configure rate group cycles: each phaser cycle
    // consists of 1000 ticks = 1 sec.
    rateGroup1Comp.configure(1000);
    rateGroup2Comp.configure(1000);
    rateGroup3Comp.configure(1000);

    // Register tasks
    rateGroup1Comp.register_phased(0, 1); // Invoke port 0 with execution time bound 1.
    rateGroup1Comp.register_phased(1, 1); // Invoke port 1 with execution time bound 1.
    rateGroup1Comp.register_phased(2, 1); // Invoke port 2 with execution time bound 1.
    rateGroup1Comp.register_phased(3, 1); // Invoke port 3 with execution time bound 1.
    rateGroup1Comp.register_phased(4, 1); // Invoke port 4 with execution time bound 1.
    rateGroup1Comp.register_phased(5, 1); // Invoke port 5 with execution time bound 1.

    rateGroup2Comp.register_phased(0, 1); // Invoke port 0 with execution time bound 1.
    rateGroup2Comp.register_phased(1, 1); // Invoke port 1 with execution time bound 1.
    rateGroup2Comp.register_phased(2, 1); // Invoke port 2 with execution time bound 1.
    rateGroup2Comp.register_phased(3, 1); // Invoke port 3 with execution time bound 1.

    rateGroup3Comp.register_phased(0, 1); // Invoke port 0 with execution time bound 1.
    rateGroup3Comp.register_phased(1, 1); // Invoke port 1 with execution time bound 1.
    rateGroup3Comp.register_phased(2, 1); // Invoke port 2 with execution time bound 1.
    rateGroup3Comp.register_phased(3, 1); // Invoke port 3 with execution time bound 1.
    rateGroup3Comp.register_phased(4, 1); // Invoke port 4 with execution time bound 1.
    rateGroup3Comp.register_phased(5, 1); // Invoke port 5 with execution time bound 1.
    rateGroup3Comp.register_phased(6, 1); // Invoke port 6 with execution time bound 1.
}

// Public functions for use in main program are namespaced with deployment name Ref
namespace Ref {
void setupTopology(const TopologyState& state) {
    // Autocoded initialization. Function provided by autocoder.
    initComponents(state);
    // Autocoded id setup. Function provided by autocoder.
    setBaseIds();
    // Autocoded connection wiring. Function provided by autocoder.
    connectComponents();
    // Autocoded command registration. Function provided by autocoder.
    regCommands();
    // Autocoded configuration. Function provided by autocoder.
    configComponents(state);
    // Project-specific component configuration. Function provided above. May be inlined, if desired.
    configureTopology();
    // Autocoded parameter loading. Function provided by autocoder.
    loadParameters();
    // Autocoded task kick-off (active components). Function provided by autocoder.
    startTasks(state);
}

void startRateGroups(Fw::TimeInterval interval) {
    // This timer drives the fundamental tick rate of the system.
    // Svc::RateGroupDriver will divide this down to the slower rate groups.
    // This call will block until the stopRateGroups() call is made.
    // For this Linux demo, that call is made from a signal handler.
    linuxTimer.startTimer(interval.getSeconds()*1000+interval.getUSeconds()/1000);
}

void stopRateGroups() {
    linuxTimer.quit();
}

void teardownTopology(const TopologyState& state) {
    // Autocoded (active component) task clean-up. Functions provided by topology autocoder.
    stopTasks(state);
    freeThreads(state);
    tearDownComponents(state);
}
}  // namespace Ref
