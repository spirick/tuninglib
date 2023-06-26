
// Spirick Tuning
//
// A C++ class and template library
// for performance critical applications.
// Copyright (C) 1996-2023 Dietmar Deimling.
// All rights reserved.
// Internet  www.spirick.com
// E-Mail    info@spirick.com
//
// Version 1.49
// File tuning/sys/cprsemaphore.hpp

#ifndef TUNING_SYS_CPRSEMAPHORE_HPP
#define TUNING_SYS_CPRSEMAPHORE_HPP

#include "tuning/sys/csharedres.hpp"

//---------------------------------------------------------------------------
// Process semaphore

class TL_EXPORT ct_PrSemaphore: public ct_SharedResource
  {
public:
                       ct_PrSemaphore ();
                       ct_PrSemaphore (const char * pc_key);
                       ct_PrSemaphore (const char * pc_key, unsigned u_idx);
                       ~ct_PrSemaphore ();

  et_ResError          Open ();
  et_ResError          Create (t_Int32 i_initValue = 1, bool b_createNew = false);
  et_ResError          Close ();

  et_ResError          TryAcquire (bool & b_success, t_UInt32 u_milliSec = 0);
  et_ResError          Acquire ();
  et_ResError          Release ();
  };

#endif

