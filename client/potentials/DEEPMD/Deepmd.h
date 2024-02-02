//-----------------------------------------------------------------------------------
// eOn is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// A copy of the GNU General Public License is available at
// http://www.gnu.org/licenses/


//-----------------------------------------------------------------------------------

#ifndef Deepmd_POTENTIAL
#define Deepmd_POTENTIAL

#include "../../Potential.h"
#include "deepmd/DeepPot.h"
class Deepmd : public Potential
{

    public:
        Deepmd(void);
		~Deepmd();
        void initialize() {};
        void cleanMemory(void);    
        void force(long N, const double *R, const int *atomicNrs, 
                   double *F, double *U, const double *box);


    private:
	static deepmd::DeepPot dp;
        //void writePOSCAR(long N, const double *R, const int *atomicNrs,
          //               const double *box);
        //void readFU(long N, double *F, double *U);
        //void spawnDeepmd();
        //bool DeepmdRunning();
        //static bool firstRun;
        static long DeepmdRunCount;
        //static pid_t DeepmdPID;
};

#endif

