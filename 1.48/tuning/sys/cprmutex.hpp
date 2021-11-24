
// Spirick Tuning
//
// A C++ class and template library
// for performance critical applications.
// Copyright (C) 1996-2021 Dietmar Deimling.
// All rights reserved.
// Internet  www.spirick.com
// E-Mail    info@spirick.com
//
// Version 1.48
// File tuning/sys/cprmutex.hpp

#ifndef TUNING_SYS_CPRMUTEX_HPP
#define TUNING_SYS_CPRMUTEX_HPP

#include "tuning/sys/csharedres.hpp"

//---------------------------------------------------------------------------
// Process mutex
// MS Windows implementation is recursive
// Linux implementation is not recursive

class TL_EXPORT ct_PrMutex: public ct_SharedResource
  {
  int                  i_LockCount;

public:
                       ct_PrMutex ();
                       ct_PrMutex (const char * pc_key);
                       ct_PrMutex (const char * pc_key, unsigned u_idx);
                       ~ct_PrMutex ();

  static inline void * operator new (size_t, void * pv) { return pv; }
  static inline void   operator delete (void *, void *) { }
  static inline void   operator delete (void *) { }

  et_ResError          Open ();
  et_ResError          Create (bool b_createNew = false);
  et_ResError          Close ();

  et_ResError          TryLock (bool & b_success, t_UInt32 u_milliSec = 0);
  et_ResError          Lock ();
  et_ResError          Unlock ();
  };

//---------------------------------------------------------------------------
// Process critical section

bool TL_EXPORT        tl_CriticalPrSectionInitSuccess ();
void TL_EXPORT        tl_DeleteCriticalPrSection ();
et_ResError TL_EXPORT tl_TryEnterCriticalPrSection (bool & b_success, t_UInt32 u_milliSec = 0);
et_ResError TL_EXPORT tl_EnterCriticalPrSection ();
et_ResError TL_EXPORT tl_LeaveCriticalPrSection ();

#endif
