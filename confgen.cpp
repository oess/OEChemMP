/****************************************************************************
 Copyright (C) 2015 OpenEye Scientific Software, Inc.
*****************************************************************************
* Perform substructure search on molecule file
****************************************************************************/
#include <openeye.h>
#include <oesystem.h>
#include <oechem.h>
#include <oeomega2.h>
#include "confgen.itf"

using namespace OESystem;
using namespace OEChem;
using namespace OEConfGen;

static void ConfGen(oemolithread& ithread, oemolothread& othread)
{
  OEOmega omega;
  omega.SetRotorOffset(true);

  OEGraphMol mol;
#pragma omp parallel private(omega) private(mol)
  while (OEReadMolecule(ithread, mol))
  {
    for (OEIter<OEMolBase> enantiomer = OEFlipper(mol); enantiomer; ++enantiomer)
    {
      OEMol mcmol(*enantiomer);

      if (omega(mcmol))
        OEWriteMolecule(othread, mcmol);
      else
        OEThrow.Warning("Failed to generate conformers for %s", mcmol.GetTitle());
    }
  }
}

int main(int argc, char *argv[])
{
  OEInterface itf(InterfaceData, argc, argv);

  oemolithread ifs;
  if (!ifs.open(itf.Get<std::string>("-i")))
    OEThrow.Fatal("Unable to open %s for reading", itf.Get<std::string>("-i").c_str());

  oemolothread ofs;
  if (!ofs.open(itf.Get<std::string>("-o")))
    OEThrow.Fatal("Unable to open %s for writing", itf.Get<std::string>("-o").c_str());

  ConfGen(ifs, ofs);

  return 0;
}
