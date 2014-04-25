/***********************************************************************
Copyright (C) 2008,2009 OpenEye Scientific Software, Inc.
***********************************************************************/
#include "openeye.h"

#include <iostream>

#include "oechem.h"
#include "oesystem.h"

using namespace OEPlatform;
using namespace OESystem;
using namespace OEChem;
using namespace std;

int main(int argc, char *argv[])
{
  if (argc != 2) 
    OEThrow.Usage("nxnlingos <infile>");

  oemolithread ifs(argv[1]);
  if(!ifs) 
    OEThrow.Fatal("Unable to open %s for reading",argv[2]);

  oemolostream ofs;
  ofs.SetFormat(OEFormat::ISM);
  ofs.openstring();

  // read all molecules into memory
  vector<oefpos_t> positions;
  OEGraphMol mol;
  while (OEReadMolecule(ifs, mol))
  {
    positions.push_back(ofs.tell());
    OEWriteMolecule(ofs, mol);
  }
  string data = ofs.GetString();
  oeisstream smiles(data.c_str(), data.size(), false);

  size_t size = positions.size();
  // # of comparisons = size*(size-1)/2
  float *sims = new float[size*(size-1)/2];

#pragma omp parallel for firstprivate(smiles) schedule(dynamic)
  for (int i = 0; i < (int)size-1; i++) 
  {
    // seek to the next molecule
    smiles.seek(positions[i]);

    // initialize the lingo object
    char ref[32768];
    smiles.getline(ref, 32768);
    OELingoSim sim(ref);

    // compare to the rest of the molecules
    size_t base = i*(size-1)-i*(i-1)/2;
    for (size_t j = 0; j < size-i-1; j++)
    {
      char fit[32768];
      smiles.getline(fit, 32768);
      
      sims[base + j] = sim.Similarity(fit);
    }
  }

  // print out the results
  for (size_t i = 0; i < size; i++)
  {
    for (size_t j = 0; j < i + 1; j++)
        oeout << "         ";

    size_t base = i*(size-1)-i*(i-1)/2;
    for (size_t j = 0; j < size-i-1; j++)
        oeout << sims[base + j] << " ";

    oeout << oeendl;
  }

  return 0;
}

