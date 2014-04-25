/**********************************************************************
Copyright (C) 2008,2009 by OpenEye Scientific Software, Inc.
***********************************************************************/
#include "openeye.h"

#include "oeplatform.h"
#include "oesystem.h"
#include "oechem.h"

#include <vector>

using namespace OEPlatform;
using namespace OESystem;
using namespace OEChem;

typedef std::vector<OEGraphMol>::size_type size_type;

void LoadDatabase(const char *fname, 
                  std::vector<OEGraphMol> &mvec,
                  unsigned int type)
{
  OEDots dots(10000, 200, "molecules");

  oemolistream ifs(fname);

  OEWallTimer sw;
  while (true)
  {
    mvec.push_back(OEGraphMol(type));
    OEGraphMol &mol = mvec.back();

    if (!OEReadMolecule(ifs, mol))
    {
      // Remove the last molecule since it is empty
      mvec.resize(mvec.size() - 1);
      break;
    }

    mol.Compress();
    dots.Update();
  }

  dots.Total();
  OEThrow.Info("%f seconds to load database", sw.Elapsed());
}

unsigned int SearchDBMols(const OESubSearch &ss,
                          std::vector<OEGraphMol> &mvec)
{
  unsigned int count=0;
  
  std::vector<OEGraphMol>::iterator iter;
#pragma omp parallel for reduction( + : count)
  for(int i = 0; i < (int)mvec.size(); ++i) 
  {
    mvec[i].UnCompress();
    if (ss.SingleMatch(mvec[i]))
      ++count;
    mvec[i].Compress();
  }

  return count;
}

unsigned int SearchMols(const OESubSearch &ss,
                        const std::vector<OEGraphMol> &mvec)
{
  unsigned int count=0;

#pragma omp parallel for reduction( + : count)
  for(int i = 0; i < (int)mvec.size(); ++i) 
    if (ss.SingleMatch(mvec[i]))
      ++count;

  return count;
}

int main(int argc, const char **argv)
{
  if (argc != 2 && argc != 3)
    OEThrow.Usage("%s <database> [moltype, defaults to 1]", argv[0]);

  unsigned int type = OEMolBaseType::OEDefault;
  if (argc == 3) 
    type = atoi(argv[2]);

  if (type == OEMolBaseType::Undefined || type >= OEMolBaseType::MaxType)
    OEThrow.Fatal("Illegal OEMolBaseType (%i)", type);

  // load input file into a vector
  std::vector<OEGraphMol> mvec;
  LoadDatabase(argv[1], mvec, type);

  
  // Read smarts patterns from stdin
  OEWallTimer sw;
  OESubSearch ss;
  char line[256];
  while (true)
  {
    oeerr << "Enter Smarts> ";
    oeerr.flush();
    oein.getline(line, 256);

    if (strlen(line) == 0)
      break;
    
    if (!ss.Init(line))
    {
      OEThrow.Warning("Unable to initialize subsearch object");
      continue;
    }
    
    sw.Start();
    unsigned int count;
    if (type == OEMolBaseType::OEDBMol)
      count = SearchDBMols(ss, mvec);
    else
      count = SearchMols(ss, mvec);
    
    OEThrow.Info("Found %i matches out of a database of %i in %f seconds", 
                 count, 
                 mvec.size(),
                 sw.Elapsed());

  }

  return 0;
}

