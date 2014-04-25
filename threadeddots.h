/**********************************************************************
Copyright (C) 2014 by OpenEye Scientific Software, Inc.
***********************************************************************/

class OEThreadedDotsImpl;
class OEThreadedDots
{
  OEThreadedDotsImpl *_impl;

  OEThreadedDots();
  OEThreadedDots(const OEThreadedDots&);
  OEThreadedDots& operator=(const OEThreadedDots&);
  public:
  OEThreadedDots(unsigned int bigstep, unsigned int smallstep, 
                 const char *objectname = "");
  ~OEThreadedDots();

  void Total();
  void Update(unsigned int step = 1);
};
