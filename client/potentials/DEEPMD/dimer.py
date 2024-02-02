# -*- coding: utf-8 -*-
"""
Created on Thu Jul  6 14:03:27 2023

@author: ASUS
"""
import os
from ase import Atom, Atoms
from ase.constraints import FixAtoms
from ase.io import read,write
from deepmd.calculator import DP
import numpy as np
import faulthandler

def main():
    
    atoms=read("POSCAR")
   
    atoms.calc = DP(model="graph.pb")
    f=open("FU","w")
    print(atoms.get_potential_energy(),file=f)
    forces=atoms.get_forces()
    for force in forces:
        for forc in force:
            f.write(str(forc)+" ")
    f.close()

if __name__ == "__main__":
    main()

