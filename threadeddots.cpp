/**********************************************************************
Copyright (C) 2014 by OpenEye Scientific Software, Inc.

***********************************************************************/
#include "threadeddots.h"

#include <openeye.h>
#include <oeplatform.h>
#include <oesystem.h>



using namespace OEPlatform;
using namespace OESystem;

typedef OEProtectedBuffer<OEBoundedBuffer<unsigned int> > UpdateBuffer;
class OEDotsThread : public OEThread
{
  OEDots       &_dots;
  UpdateBuffer &_buff;
public:
  OEDotsThread(OEDots &dots, UpdateBuffer &buff)
    : _dots(dots), _buff(buff)
  {
  }

  void *Run(void *)
  {
    unsigned int step = 0;
    unsigned int term = _buff.Null();
    while ((step = _buff.Get()) != term)
      _dots.Update(step);

    return NULL;
  }
};

class OEThreadedDotsImpl
{
  OEDots _dots;
  OEProtectedBuffer<OEBoundedBuffer<unsigned int> > _buff;
  OEDotsThread _thread;
  bool         _done; // only touched by the master thread that creates this object
 
  void TerminateThread()
  {
    if (!_done)
    {
      _buff.Put(_buff.Null());
      _thread.Join();
      _done = true;
    }
  }

public:
  OEThreadedDotsImpl(unsigned int bigstep, unsigned int smallstep, 
                     const char *objectname)
    : _dots(bigstep, smallstep, objectname),
      _buff(OEGetNumProcessors() * 2, OEGetNumProcessors()),
      _thread(_dots, _buff),
      _done(false)
  {
    _thread.Start();
  }

  ~OEThreadedDotsImpl()
  {
    TerminateThread();
  }

  void Total() // note, Total is not thread safe, only Update is
  {
    TerminateThread();
    _dots.Total();
  }

  void Update(unsigned int step)
  {
    if (_done)
    {
      OEThrow.Error("Attempting to Update a OEThreadedDots after Total already called.");
      return;
    }

    if (!_buff.Put(step))
      OEThrow.Error("Failed to Update a OEThreadedDots");
  }
};

OEThreadedDots::OEThreadedDots(unsigned int bigstep, unsigned int smallstep, 
                               const char *objectname)
  : _impl(new OEThreadedDotsImpl(bigstep, smallstep, objectname))
{
}

OEThreadedDots::~OEThreadedDots()
{
  delete _impl;
}

void OEThreadedDots::Total()
{
  return _impl->Total();
}

void OEThreadedDots::Update(unsigned int step)
{
  return _impl->Update(step);
}
