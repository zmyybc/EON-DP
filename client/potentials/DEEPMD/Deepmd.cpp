//-----------------------------------------------------------------------------------
// eOn is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// A copy of the GNU General Public License is available at
// http://www.gnu.org/licenses/
//-----------------------------------------------------------------------------------

#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#ifdef WIN32
#include <windows.h>
//#define popen _popen
#else
#include <sys/wait.h>
#include <fcntl.h>
#endif
#include <map>
#include "deepmd/DeepPot.h"
#include "Deepmd.h"
#include <iostream>
#include <vector>
#include <algorithm>  // For std::find
#include <string>
#include <sstream>

long  Deepmd::DeepmdRunCount = 0;
deepmd::DeepPot Deepmd::dp("./graph.pb");
Deepmd::Deepmd(void)
{
    DeepmdRunCount++;
    // deleting leftovers from previous run
    
    //system("rm -f FU");      
   // system("rm -f POSCAR"); 
    //system("rm -f graph.pb"); 
    return;
}

void Deepmd::cleanMemory(void)
{
	DeepmdRunCount--;
	if(DeepmdRunCount < 1) {
	
	}
    return;
}

Deepmd::~Deepmd()
{
	cleanMemory();
}

const std::string atomicNumberToSymbol[93] = {
    "", "H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne",
    "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca",
    "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn",
    "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr",
    "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn",
    "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd",
    "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb",
    "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg",
    "Tl", "Pb", "Bi", "Th", "Pa", "U"
};
/***
const std::vector<std::string> elementList = {
   "Si","Al","O","Li", "Pt", "Ag", "As", "Au", "B", "Bi", "C", "Ca", "Cd", "Cl", "Co",
    "Cr", "Cs", "Cu", "Fe", "Ga", "Ge", "H", "Hf", "Hg", "In",
    "Ir", "K", "Mg", "Mn", "Mo", "N", "Na", "Nb", "Ni",
    "Os", "P", "Pb", "Pd", "Rb", "Re", "Rh", "Ru", "S",
    "Sb", "Sc", "Se", "Sn", "Sr", "Ta", "Tc", "Te", "Ti",
    "Tl", "V", "W", "Y", "Zn", "Zr"
};
***/
void Deepmd::force(long N, const double *R, const int *atomicNrs, double *F, 
                   double *U, const double *box)
{
    // Initial print statements
 /***   std::cout << "Initial Values:" << std::endl;
    for(long i = 0; i < N; i++) {
        std::cout << "R[" << 3*i << ":" << 3*i+2 << "] = " << R[3*i] << ", " << R[3*i+1] << ", " << R[3*i+2] << std::endl;
        std::cout << "AtomicNrs[" << i << "] = " << atomicNrs[i] << std::endl;
        std::cout << "F[" << 3*i << ":" << 3*i+2 << "] = " << F[3*i] << ", " << F[3*i+1] << ", " << F[3*i+2] << std::endl;
    }
    std::cout << "U = " << *U << std::endl;
    
    for(int i = 0; i < 9; i += 3) {
        std::cout << "Box[" << i/3 << "] = " << box[i] << ", " << box[i+1] << ", " << box[i+2] << std::endl;
    }
***/
    std::vector<double> R_vec(R, R + N * 3);
    std::vector<int> atomicNrs_vec(atomicNrs, atomicNrs + N);
    std::vector<double> F_vec(F, F + N * 3);
    std::vector<double> U_vec(U, U + 1);
    std::vector<double> box_vec(box, box + 9);
    std::vector<double> v;

    // Create a new mapping for atomicNrs_vec
    std::string typeMap; // 定义一个字符串来接收类型映射

    // 调用get_type_map函数，传入typeMap变量
    dp.get_type_map(typeMap);
    std::stringstream ss(typeMap);
    std::string item;
    std::vector<std::string> elementList;

    // 读取每个元素并添加到列表中
    while (ss >> item) {
        elementList.push_back(item);
    }
    std::map<int, int> atomicNrMapping;
    int nextIdx = 0;
    for(auto &atomNr : atomicNrs_vec) {
        std::string elementSymbol = atomicNumberToSymbol[atomNr];
        auto it = std::find(elementList.begin(), elementList.end(), elementSymbol);
        if(it != elementList.end()) {
            int indexInList = std::distance(elementList.begin(), it);
            atomicNrMapping[atomNr] = indexInList;
        }
        atomNr = atomicNrMapping[atomNr];
    }


    // Converted Vectors print statements
   /*** std::cout << "Converted Vectors:" << std::endl;
    for(size_t i = 0; i < R_vec.size(); i+=3) {
        std::cout << "R_vec[" << i << ":" << i+2 << "] = " << R_vec[i] << ", " << R_vec[i+1] << ", " << R_vec[i+2] << std::endl;
        std::cout << "AtomicNrs_vec[" << i/3 << "] = " << atomicNrs_vec[i/3] << std::endl;
        std::cout << "F_vec[" << i << ":" << i+2 << "] = " << F_vec[i] << ", " << F_vec[i+1] << ", " << F_vec[i+2] << std::endl;
    }
    std::cout << "U_vec = " << U_vec[0] << std::endl;
    for(size_t i = 0; i < box_vec.size(); i+=3) {
        std::cout << "Box_vec[" << i/3 << "] = " << box_vec[i] << ", " << box_vec[i+1] << ", " << box_vec[i+2] << std::endl;
    }
***/
    dp.compute(*U, F_vec, v, R_vec, atomicNrs_vec, box_vec);

    // After dp.compute print statements
   /***  std::cout << "After dp.compute:" << std::endl;
    for(size_t i = 0; i < F_vec.size(); i+=3) {
        std::cout << "F_vec[" << i << ":" << i+2 << "] = " << F_vec[i] << ", " << F_vec[i+1] << ", " << F_vec[i+2] << std::endl;
    }
    std::cout << "U_vec = " << U_vec[0] << std::endl;
***/
    std::copy(F_vec.begin(), F_vec.end(), F);
    //*U = U_vec[0];

    // Final Pointer Values print statements
   /*** std::cout << "Final Pointer Values:" << std::endl;
    for(long i = 0; i < N; i++) {
        std::cout << "F[" << 3*i << ":" << 3*i+2 << "] = " << F[3*i] << ", " << F[3*i+1] << ", " << F[3*i+2] << std::endl;
    }
    std::cout << "U = " << *U << std::endl;***/
}




