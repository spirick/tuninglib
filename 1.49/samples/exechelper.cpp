
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
// File samples/exechelper.cpp

#include "samples/exechelper.hpp"
#include "tuning/std/array.h"

volatile t_Int32 ct_ExecHelper::i_InstanceCount;

//---------------------------------------------------------------------------

ct_ExecHelper::ct_ExecHelper ()
  {
  i_Instance = tl_InterlockedIncrement (& i_InstanceCount);
  u_BufferSize = 0;
  b_InitSuccess = false;
  b_Intern = false;
  pco_SendSemaphore = 0;
  pco_ReceiveSemaphore = 0;
  pco_SharedMemory = 0;
  }

//---------------------------------------------------------------------------

ct_ExecHelper::~ct_ExecHelper ()
  {
  if (b_InitSuccess)
    Stop ();
  }

//---------------------------------------------------------------------------

et_ResError ct_ExecHelper::StartIntern ()
  {
  if (b_InitSuccess)
    return ec_ResAlreadyInitialized;

  b_InitSuccess = true;
  b_Intern = true;
  return ec_ResOK;
  }

//---------------------------------------------------------------------------

et_ResError ct_ExecHelper::StartExtern
(
const char * pc_path,
unsigned u_bufferSize
)
  {
  if (b_InitSuccess)
    return ec_ResAlreadyInitialized;

  if ((pc_path == 0) || (* pc_path == '\0'))
    return ec_ResInvalidValue;

  if ((u_bufferSize < 1024) || (u_bufferSize > 16384))
    return ec_ResInvalidValue;

  u_BufferSize = u_bufferSize;

  if (! co_Mutex. GetInitSuccess ())
    return ec_ResUnknownError;

  int i_pid = tl_ProcessId ();
  ct_String co_toHelperPid;
  ct_String co_fromHelperPid;
  ct_String co_sharedMemoryPid;
  co_toHelperPid. AssignF ("%s_%d_", PC_EXECHELPER_TO_HELPER, i_pid);
  co_fromHelperPid. AssignF ("%s_%d_", PC_EXECHELPER_FROM_HELPER, i_pid);
  co_sharedMemoryPid. AssignF ("%s_%d_", PC_EXECHELPER_SHARED_MEMORY, i_pid);
  pco_SendSemaphore    = new ct_PrSemaphore (co_toHelperPid. GetStr (), i_Instance);
  pco_ReceiveSemaphore = new ct_PrSemaphore (co_fromHelperPid. GetStr (), i_Instance);
  pco_SharedMemory     = new ct_SharedMemory (co_sharedMemoryPid. GetStr (), i_Instance);
  et_ResError eo_ret = pco_SendSemaphore-> Create (0, true);

  if (eo_ret == ec_ResOK)
    {
    eo_ret = pco_ReceiveSemaphore-> Create (0, true);

    if (eo_ret == ec_ResOK)
      {
      eo_ret = pco_SharedMemory-> Create (u_BufferSize, true);

      if (eo_ret == ec_ResOK)
        {
        ct_String co_pid;
        co_pid. AssignF ("%d", i_pid);
        ct_String co_instance;
        co_instance. AssignF ("%d", i_Instance);
        ct_String co_bufferSize;
        co_bufferSize. AssignF ("%d", u_BufferSize);
        const char * apc_params [3] =
          { co_pid. GetStr (), co_instance. GetStr (), co_bufferSize. GetStr () };

        // Optional: Use another ExecHelper to start the new ExecHelper
        if (tl_Exec (pc_path, 3, apc_params, false) == -1)
          eo_ret = ec_ResNotFound;
        else
          {
          // Initial handshake
          eo_ret = pco_SendSemaphore-> Release ();

          if (eo_ret == ec_ResOK)
            {
            eo_ret = pco_ReceiveSemaphore-> Acquire ();

            if (eo_ret == ec_ResOK)
              {
              b_InitSuccess = true;
              b_Intern = false;
              return ec_ResOK;
              }
            }
          }
        }
      }
    }

  delete pco_SendSemaphore;
  delete pco_ReceiveSemaphore;
  delete pco_SharedMemory;
  pco_SendSemaphore = 0;
  pco_ReceiveSemaphore = 0;
  pco_SharedMemory = 0;
  return eo_ret;
  }

//---------------------------------------------------------------------------

et_ResError ct_ExecHelper::Stop ()
  {
  if (! b_InitSuccess)
    return ec_ResUninitialized;

  if (! b_Intern)
    {
    st_ShmHeader * pso_header = (st_ShmHeader *) pco_SharedMemory-> GetData ();
    pso_header-> i_ReturnValue = -1;
    et_ResError eo_ret = pco_SendSemaphore-> Release ();

    if (eo_ret == ec_ResOK)
      eo_ret = pco_ReceiveSemaphore-> Acquire ();

    if (eo_ret != ec_ResOK)
      return eo_ret;

    delete pco_SendSemaphore;
    delete pco_ReceiveSemaphore;
    delete pco_SharedMemory;
    pco_SendSemaphore = 0;
    pco_ReceiveSemaphore = 0;
    pco_SharedMemory = 0;
    }

  b_InitSuccess = false;
  return ec_ResOK;
  }

//---------------------------------------------------------------------------

int ct_ExecHelper::Exec
(
const char * pc_path,
unsigned u_params,
const char * * ppc_params,
bool b_wait
)
  {
  if (! b_InitSuccess)
    return -1;

  if ((pc_path == 0) || (* pc_path == '\0'))
    return -1;

  if (b_Intern)
    return tl_Exec (pc_path, u_params, ppc_params, b_wait);
  else
    {
    gct_Std_Array <t_UInt32> co_strSizes;
    t_UInt u_size = sizeof (st_ShmHeader) + (u_params + 1) * sizeof (t_UInt32);
    t_UInt u_strSize = tl_StringLength (pc_path) + 1;
    u_size += u_strSize;
    * co_strSizes. GetNewObj () = (t_UInt32) u_strSize;
    unsigned u_param;

    for (u_param = 0; u_param < u_params; u_param ++)
      {
      if ((ppc_params [u_param] == 0) || (* ppc_params [u_param] == '\0'))
        * co_strSizes. GetNewObj () = 0;
      else
        {
        u_strSize = tl_StringLength (ppc_params [u_param]) + 1;
        u_size += u_strSize;
        * co_strSizes. GetNewObj () = (t_UInt32) u_strSize;
        }
      }

    if (u_size > u_BufferSize)
      return -1;

    co_Mutex. Lock ();
    st_ShmHeader * pso_header = (st_ShmHeader *) pco_SharedMemory-> GetData ();
    pso_header-> i_ReturnValue = 0;
    pso_header-> u_Params = u_params;
    pso_header-> u_Wait = (t_UInt32) b_wait;

    char * pc_strData = (char *) pco_SharedMemory-> GetData ();
    pc_strData += sizeof (st_ShmHeader);
    t_UInt32 * pu_sizeData = (t_UInt32 *) pc_strData;
    pc_strData += (u_params + 1) * sizeof (t_UInt32);

    t_UInt32 u_currSize = * co_strSizes. GetNthObj (1);
    * (pu_sizeData ++) = u_currSize;
    tl_CopyMemory (pc_strData, pc_path, u_currSize);
    pc_strData += u_currSize;

    for (u_param = 0; u_param < u_params; u_param ++)
      {
      u_currSize = * co_strSizes. GetNthObj (u_param + 2);
      * (pu_sizeData ++) = u_currSize;
      tl_CopyMemory (pc_strData, ppc_params [u_param], u_currSize);
      pc_strData += u_currSize;
      }

    et_ResError eo_ret = pco_SendSemaphore-> Release ();

    if (eo_ret == ec_ResOK)
      eo_ret = pco_ReceiveSemaphore-> Acquire ();

    int i_ret = pso_header-> i_ReturnValue;
    co_Mutex. Unlock ();

    if (eo_ret == ec_ResOK)
      return i_ret;
    else
      return -1;
    }
  }
