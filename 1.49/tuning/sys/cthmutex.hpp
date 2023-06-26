
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
// File tuning/sys/cthmutex.hpp

#ifndef TUNING_SYS_CTHMUTEX_HPP
#define TUNING_SYS_CTHMUTEX_HPP

#include "tuning/sys/creserror.hpp"

//---------------------------------------------------------------------------
// Thread mutex

#if defined TL_MULTI

const unsigned       cu_ThMutexDataSize = 40; // Multiple of sizeof (t_UInt64)

class TL_EXPORT ct_ThMutex
  {
  // Don't use char buffer because of alignment
  t_UInt64             au_Data [cu_ThMutexDataSize / sizeof (t_UInt64)];
  bool                 b_InitSuccess;
  int                  i_LockCount;

  // Do not copy this object
                       ct_ThMutex (const ct_ThMutex &);
  ct_ThMutex &         operator = (const ct_ThMutex &);

public:
                       ct_ThMutex ();
                       ~ct_ThMutex ();

  static inline void * operator new (size_t, void * pv) { return pv; }
  static inline void   operator delete (void *, void *) { }
  static inline void   operator delete (void *) { }

  bool                 GetInitSuccess () const;
  et_ResError          TryLock (bool & b_success);
  et_ResError          Lock ();
  et_ResError          Unlock ();
  };

#else

class TL_EXPORT ct_ThMutex
  {
  // Do not copy this object
                       ct_ThMutex (const ct_ThMutex &);
  ct_ThMutex &         operator = (const ct_ThMutex &);

public:
  bool                 GetInitSuccess () const { return true; }
  et_ResError          TryLock (bool & b_success)
                         { b_success = true; return ec_ResOK; }
  et_ResError          Lock () { return ec_ResOK; }
  et_ResError          Unlock () { return ec_ResOK; }
  };

#endif

//---------------------------------------------------------------------------
// Thread critical section

bool TL_EXPORT        tl_CriticalSectionInitSuccess ();
void TL_EXPORT        tl_DeleteCriticalSection ();
et_ResError TL_EXPORT tl_TryEnterCriticalSection (bool & b_success);
et_ResError TL_EXPORT tl_EnterCriticalSection ();
et_ResError TL_EXPORT tl_LeaveCriticalSection ();

#endif
