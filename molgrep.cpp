/****************************************************************************
 Copyright (C) 2002-2011, 2014 OpenEye Scientific Software, Inc.
*****************************************************************************
* Perform substructure search on molecule file
****************************************************************************/
#include <openeye.h>
#include <oesystem.h>
#include <oechem.h>
#include "molgrep.itf"

using namespace OESystem;
using namespace OEChem;

static void SubSearch(OEInterface& itf, OESubSearch& ss,
                      oemolithread& ithread, oemolothread& othread)
{
  bool reverseflag = itf.Get<bool>("-r");
  bool countflag   = itf.Get<bool>("-c");
  unsigned int count = 0;
  OEGraphMol mol;
#pragma omp parallel private(mol) reduction(+ : count)
  while (OEReadMolecule(ithread, mol))
  {
    OEPrepareSearch(mol, ss);
    if (ss.SingleMatch(mol) != reverseflag)
    {
      if (countflag)
        count++;
      else
        OEWriteMolecule(othread, mol);
    }
  }
  if (countflag)
    std::cout << count << " matching molecules"  <<  std::endl;
}

int main(int argc, char *argv[])
{
  OEInterface itf(InterfaceData, argc, argv);
  if (!(itf.Get<bool>("-c") ^ itf.Has<std::string>("-o")))
    OEThrow.Fatal("Counting (-c) or output (-o) must be specified and are mutually exclusive.");

  oemolithread ifs;
  if (!ifs.open(itf.Get<std::string>("-i")))
    OEThrow.Fatal("Unable to open %s for reading", itf.Get<std::string>("-i").c_str());

  oemolothread ofs;
  if (!itf.Get<bool>("-c"))
    if (!ofs.open(itf.Get<std::string>("-o")))
      OEThrow.Fatal("Unable to open %s for writing", itf.Get<std::string>("-o").c_str());

  std::string smarts = itf.Get<std::string>("-p");
  OESubSearch ss;
  if (!ss.Init(smarts.c_str()))
    OEThrow.Fatal("Unable to parse SMARTS: %s", smarts.c_str());

  SubSearch(itf, ss, ifs, ofs);

  return 0;
}
