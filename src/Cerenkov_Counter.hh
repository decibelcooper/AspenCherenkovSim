/*
 * Cerenkov_Counter.h
 *
 *  Created on: Apr 4, 2012
 *      Author: dblyth
 *
 *  This class is written to implement the construction of small auxiliary
 *  Cerenkov counters from the Alarcon group of Arizona State University.  The
 *  structure of this code is designed to mimic conventions used elsewhere in
 *  the MC code.
 */

#ifndef CERENKOV_COUNTER_H_
#define CERENKOV_COUNTER_H_

//	I like to include here rather than having incomplete class declarations
//	that have the potential to be problematic.
#include <G4VPhysicalVolume.hh>

class Cerenkov_Counter {
   public:
    //	Obligatory constructor/destructor
    Cerenkov_Counter();
    virtual ~Cerenkov_Counter();

    //	Method for carrying out Cerenkov component construction
    void Build(G4VPhysicalVolume *World_Phys);
};

#endif /* CERENKOV_COUNTER_H_ */
