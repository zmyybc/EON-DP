#include <time.h>

#include "Potential.h"
#include "Parameters.h"
#include "Log.h"
#include "HelperFunctions.h"

#include "potentials/IMD/IMD.h"
#include "potentials/EDIP/EDIP.h"
#include "potentials/EMT/EffectiveMediumTheory.h"
#include "potentials/Morse/Morse.h"
#include "potentials/LJ/LJ.h"
#include "potentials/LJCluster/LJCluster.h"
#include "potentials/SW/SW.h"
#include "potentials/Tersoff/Tersoff.h"
#include "potentials/Aluminum/Aluminum.h"
#include "potentials/EAM/EAM.h"
#include "potentials/Lenosky/Lenosky.h"
#include "potentials/QSC/QSC.h"
#include "potentials/Water/Water.hpp"
#include "potentials/Water_Pt/Tip4p_Pt.hpp"
#include "potentials/Water_H/Tip4p_H.h"
#include "potentials/FeHe/FeHe.h"
#include "potentials/ExtPot/ExtPot.h"
#include "potentials/PyAMFF/PyAMFF.h"

#ifdef EONMPI
    #include "potentials/MPIPot/MPIPot.h"
#endif
#ifdef LAMMPS_POT
    #include "potentials/LAMMPS/LAMMPS.h"
#endif
#ifdef NEW_POT
    #include "potentials/NewPot/NewPot.h"
#endif
#ifndef WIN32
    #include "potentials/VASP/VASP.h"
#endif
#ifdef PYAMFF_POT
    #include "potentials/PyAMFF/PyAMFF.h"
#endif

#ifdef DEEP_POT
  #include "potentials/DEEPMD/Deepmd.h"
#endif

#include <cstdlib>

const char Potential::POT_LJ[] =          "lj";
const char Potential::POT_LJCLUSTER[] =   "lj_cluster";
const char Potential::POT_IMD[] =         "imd";
const char Potential::POT_EAM_AL[] =      "eam_al";
const char Potential::POT_MORSE_PT[] =    "morse_pt";
const char Potential::POT_EMT[] =         "emt";
const char Potential::POT_QSC[] =         "qsc";
const char Potential::POT_TIP4P[] =       "tip4p";
const char Potential::POT_TIP4P_PT[] =    "tip4p_pt";
const char Potential::POT_TIP4P_H[] =     "tip4p_h";
const char Potential::POT_SPCE[] =        "spce";
const char Potential::POT_LENOSKY_SI[] =  "lenosky_si";
const char Potential::POT_SW_SI[] =       "sw_si";
const char Potential::POT_TERSOFF_SI[] =  "tersoff_si";
const char Potential::POT_EDIP[] =        "edip";
const char Potential::POT_FEHE[] =        "fehe";
const char Potential::POT_VASP[] =        "vasp";
const char Potential::POT_LAMMPS[] =      "lammps";
const char Potential::POT_MPI[] =         "mpi";
const char Potential::POT_EXT[] =         "ext_pot";
const char Potential::POT_NEW[] =         "new_pot";
const char Potential::POT_PYAMFF[] =       "pyamff";
const char Potential::POT_DEEPMD[] =       "deepmd";

Potential* Potential::pot = NULL;

Potential *Potential::getPotential(Parameters *parameters)
{
    if(pot) {
        return pot;
    }
    if(parameters->potential == POT_LJ)
        pot = new LJ();
    else if(parameters->potential == POT_LJCLUSTER)
        pot = new LJCluster();
    else if(parameters->potential == POT_IMD)
        pot = new IMD();
    else if(parameters->potential == POT_MORSE_PT)
        pot = new Morse();
    else if(parameters->potential == POT_EMT)
        pot = new EffectiveMediumTheory(parameters);
    else if(parameters->potential == POT_QSC)
        pot = new QSC();
    else if(parameters->potential == POT_TIP4P)
        pot = new Tip4p();
    else if(parameters->potential == POT_TIP4P_PT)
        pot = new Tip4p_Pt();
    else if(parameters->potential == POT_SPCE)
        pot = new SpceCcl();
    else if(parameters->potential == POT_EXT)
        pot = new ExtPot(parameters);
 
#ifdef NO_FORTRAN
    else if(parameters->potential == POT_EAM_AL)
        pot = new Aluminum();
    else if(parameters->potential == POT_LENOSKY_SI)
        pot = new Lenosky();
    else if(parameters->potential == POT_SW_SI)
        pot = new SW();
    else if(parameters->potential == POT_TERSOFF_SI)
        pot = new Tersoff();
    else if(parameters->potential == POT_EDIP)
        pot = new EDIP();
    else if(parameters->potential == POT_TIP4P_H)
        pot = new Tip4p_H();
    else if(parameters->potential == POT_FEHE)
        pot = new FeHe();
#endif

#ifdef DEEP_POT
    else if(parameters->potential == POT_DEEPMD)
        pot = new Deepmd();
#endif

#ifdef PYAMFF_POT
    else if(parameters->potential == POT_PYAMFF)
        pot = new PyAMFF();
#endif

#ifdef EONMPI
    else if(parameters->potential == POT_MPI)
        pot = new MPIPot(parameters);
#endif

#ifdef LAMMPS_POT
    else if(parameters->potential == POT_LAMMPS)
        pot = new lammps(parameters);
#endif

#ifdef NEW_POT
    else if(parameters->potential == POT_NEW)
        pot = new NewPot(parameters);
#endif

#ifndef WIN32
    else if(parameters->potential == POT_VASP)
        pot = new VASP();
#endif

    else {
        
        printf("Unknown Potential: %s\n", parameters->potential.c_str());
       
    }
    pot->initialize();
    pot->params = parameters;
    return pot;
};

int Potential::fcalls = 0;
int Potential::fcallsTotal = 0;
double Potential::totalUserTime=0;

AtomMatrix Potential::force(long nAtoms, AtomMatrix positions,
                            VectorXi atomicNrs, double *energy, Matrix3d box)
{
    AtomMatrix forces(nAtoms,3);

    double start, userStart, sysStart;
    if (params->LogPotential) {
        helper_functions::getTime(&start, &userStart, &sysStart);
    }
    force(nAtoms, positions.data(), atomicNrs.data(), forces.data(), energy,
          box.data());

    double finish, userFinish, sysFinish;
    if (params->LogPotential) {
        helper_functions::getTime(&finish, &userFinish, &sysFinish);

        log_file("[Potential] fcall#: %4d  real: %.6e  user: %.6e  sys: %.6e seconds\n",
                 fcalls, finish - start, userFinish - userStart, sysFinish - sysStart);
        totalUserTime += userFinish - userStart;
    }

    fcalls += 1;
    fcallsTotal += 1;
//    printf("nAtoms %f\n",nAtoms);
//    printf("AtomMatrix positions %f\n",positions);
//    printf("VectorXi atomicNmrs %f\n",atomicNrs);
//    printf("energy %f\n",energy);
//    printf("Matrix3d box %f\n",box);
   
    if (params->maxForceCalls != 0) {
        if (fcallsTotal > params->maxForceCalls) {
            throw 1017;
        }
    }
    
    return forces;
};


