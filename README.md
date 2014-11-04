OEChemMP
========

OEChem examples ported to OpenMP to be much faster

QuickStart

1. git clone https://github.com/oess/OEChemMP.git 
2. cd OEChemMP
3. tar -vzxf OpenEye-toolkits-YOUR-ARCH.tar.gz 
4. ./configure
5. make
6. ls ./bin

OEChem Examples Performance Increases

- cansmi - intimidating
- catmols - hard
- cliquealign - easy
- convert - easy
- csv2sdf - intimidating 
- dots - easy
- extractscaffold - easy
- gettitles - intimidating
- libgen - unknown
- mcs3dalign - unknown
- minpath - too simple
- molchunk - easy
- molcount - easy
- molextract - easy
- molgrep - easy - 776 seconds to 192 seconds (4x speedup)
- molsort - hard
- molstats - easy
- oecheminfo - non-sense
- parts2mols - easy
- printatomnames - easy
- randomizeatoms - easy
- randomsample - easy
- ringsubset - easy
- rotorcount - easy
- sdf2csv - should be changed
- sdfilter - easy
- sdfmodprops - easy
- sdfrename - easy
- sizefilter - easy
- smartsalign - unknown
- stripsalts - easy
- unimolrxn - easy
- uniqinchi - hard
- uniqmol - hard

Other examples

- nxnlingos - efficiently split up a NxN Lingos calculation over multiple threads
- memsearch - multi-threaded in-memory substructure search (compares different molecule implementations)
