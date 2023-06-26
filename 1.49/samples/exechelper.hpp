
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
// File samples/exechelper.hpp

#ifndef TUNING_SAMPLES_EXECHELPER_HPP
#define TUNING_SAMPLES_EXECHELPER_HPP

#include "tuning/sys/cprocess.hpp"

//---------------------------------------------------------------------------

#define PC_EXECHELPER_TO_HELPER     "Spirick_Tuning_ExecHelper_To_Helper"
#define PC_EXECHELPER_FROM_HELPER   "Spirick_Tuning_ExecHelper_From_Helper"
#define PC_EXECHELPER_SHARED_MEMORY "Spirick_Tuning_ExecHelper_Shared_Memory"

struct st_ShmHeader
  {
  t_Int32              i_ReturnValue;
  t_UInt32             u_Params;
  t_UInt32             u_Wait;
  };

class ct_ExecHelper
  {
  static volatile t_Int32 i_InstanceCount;
  t_Int32              i_Instance;
  unsigned             u_BufferSize;
  bool                 b_Intern;
  bool                 b_InitSuccess;
  ct_ThMutex           co_Mutex;
  ct_PrSemaphore *     pco_SendSemaphore;
  ct_PrSemaphore *     pco_ReceiveSemaphore;
  ct_SharedMemory *    pco_SharedMemory;

  // Do not copy this object
                       ct_ExecHelper (const ct_ExecHelper &);
  ct_ExecHelper &      operator = (const ct_ExecHelper &);

public:
                       ct_ExecHelper ();
                       ~ct_ExecHelper ();

  // Not thread-safe
  et_ResError          StartIntern ();
  et_ResError          StartExtern (const char * pc_path, unsigned u_bufferSize = 4096);
  et_ResError          Stop ();

  // Thread-safe
  int                  Exec (const char * pc_path, unsigned u_params,
                         const char * * ppc_params, bool b_wait = false);
  };

#endif
