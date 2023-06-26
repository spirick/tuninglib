
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
// File tuning/sys/csharedmem.hpp

#ifndef TUNING_SYS_CSHAREDMEM_HPP
#define TUNING_SYS_CSHAREDMEM_HPP

#include "tuning/sys/csharedres.hpp"

//---------------------------------------------------------------------------
// Shared memory

class TL_EXPORT ct_SharedMemory: public ct_SharedResource
  {
  t_UInt               u_Size;
  void *               pv_Data;

public:
                       ct_SharedMemory ();
                       ct_SharedMemory (const char * pc_key);
                       ct_SharedMemory (const char * pc_key, unsigned u_idx);
                       ~ct_SharedMemory ();

  et_ResError          Open (bool b_readOnly);
  et_ResError          Create (t_UInt u_size, bool b_createNew = false);
  et_ResError          Close ();

  t_UInt               GetSize () const;
  void *               GetData () const;
  };

#endif
