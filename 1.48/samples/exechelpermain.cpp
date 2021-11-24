
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
// File samples/exechelpermain.cpp

#include <stdlib.h>
#include "samples/exechelper.hpp"
#include "tuning/std/array.h"

//---------------------------------------------------------------------------

int main
(
int argc,
char * * argv
)
  {
  if (argc != 4)
    return 1;

  int i_pid = atoi (argv [1]);

  if (i_pid <= 0)
    return 1;

  ct_String co_toHelperPid;
  ct_String co_fromHelperPid;
  ct_String co_sharedMemoryPid;
  co_toHelperPid. AssignF ("%s_%d_", PC_EXECHELPER_TO_HELPER, i_pid);
  co_fromHelperPid. AssignF ("%s_%d_", PC_EXECHELPER_FROM_HELPER, i_pid);
  co_sharedMemoryPid. AssignF ("%s_%d_", PC_EXECHELPER_SHARED_MEMORY, i_pid);
  int i_instance = atoi (argv [2]);

  if (i_instance <= 0)
    return 1;

  ct_PrSemaphore co_sendSemaphore (co_fromHelperPid. GetStr (), i_instance);

  if (co_sendSemaphore. Open () != ec_ResOK)
    return 1;

  ct_PrSemaphore co_receiveSemaphore (co_toHelperPid. GetStr (), i_instance);

  if (co_receiveSemaphore. Open () != ec_ResOK)
    return 1;

  ct_SharedMemory co_sharedMemory (co_sharedMemoryPid. GetStr (), i_instance);

  if (co_sharedMemory. Open (false) != ec_ResOK)
    return 1;

  unsigned u_bufferSize = atoi (argv [3]);

  if (co_sharedMemory. GetSize () != u_bufferSize)
    return 1;

  // Initial handshake
  if (co_receiveSemaphore. Acquire () != ec_ResOK)
    return 1;

  if (co_sendSemaphore. Release () != ec_ResOK)
    return 1;

  for (;;)
    {
    if (co_receiveSemaphore. Acquire () != ec_ResOK)
      return 1;

    st_ShmHeader * pso_header = (st_ShmHeader *) co_sharedMemory. GetData ();

    if (pso_header-> i_ReturnValue == -1)
      {
      if (co_sendSemaphore. Release () != ec_ResOK)
        return 1;

      return 0;
      }

    char * pc_strData = (char *) co_sharedMemory. GetData ();
    pc_strData += sizeof (st_ShmHeader);
    t_UInt32 * pu_sizeData = (t_UInt32 *) pc_strData;
    pc_strData += (pso_header-> u_Params + 1) * sizeof (t_UInt32);
    gct_Std_Array <const char *> co_params;

    for (unsigned u_param = 0; u_param < pso_header-> u_Params + 1; u_param ++)
      {
      t_UInt32 u_currSize = * (pu_sizeData ++);

      if (u_currSize == 0)
        * co_params. GetNewObj () = 0;
      else
        {
        * co_params. GetNewObj () = pc_strData;
        pc_strData += u_currSize;
        }
      }

    pso_header-> i_ReturnValue = tl_Exec (* co_params. GetNthObj (1), pso_header-> u_Params,
      pso_header-> u_Params == 0 ? 0 : co_params. GetNthObj (2), pso_header-> u_Wait == 1);

    if (co_sendSemaphore. Release () != ec_ResOK)
      return 1;
    }
  }
