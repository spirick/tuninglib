
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
// File tuning/sys/cprocess.cpp

#include "tuning/sys/cprocess.hpp"
#include "tuning/sys/ctimedate.hpp"
#include "tuning/sys/cstring.hpp"
#include "tuning/sys/cmemory.hpp"
#include "tuning/std/store.hpp"
#include "tuning/rnd/store.hpp"
#include "tuning/chn/store.hpp"

//---------------------------------------------------------------------------
// Thread critical section

static ct_ThMutex * PCO_ThMutex;
// Don't use char buffer because of alignment
static t_UInt64 au_ThMutexBuffer [(sizeof (ct_ThMutex) / sizeof (t_UInt64)) + 1];

//---------------------------------------------------------------------------

static inline void tl_CreateThMutex ()
  {
  if (PCO_ThMutex == 0)
    PCO_ThMutex = new (au_ThMutexBuffer) ct_ThMutex;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_CriticalSectionInitSuccess ()
  {
  tl_CreateThMutex ();
  return PCO_ThMutex-> GetInitSuccess ();
  }

//---------------------------------------------------------------------------

void TL_EXPORT tl_DeleteCriticalSection ()
  {
  if (PCO_ThMutex != 0)
    {
    delete PCO_ThMutex;
    PCO_ThMutex = 0;
    }
  }

//---------------------------------------------------------------------------

et_ResError TL_EXPORT tl_TryEnterCriticalSection
(
bool & b_success
)
  {
  tl_CreateThMutex ();
  return PCO_ThMutex-> TryLock (b_success);
  }

//---------------------------------------------------------------------------

et_ResError TL_EXPORT tl_EnterCriticalSection ()
  {
  tl_CreateThMutex ();
  return PCO_ThMutex-> Lock ();
  }

//---------------------------------------------------------------------------

et_ResError TL_EXPORT tl_LeaveCriticalSection ()
  {
  tl_CreateThMutex ();
  return PCO_ThMutex-> Unlock ();
  }

//---------------------------------------------------------------------------
// Process critical section

static ct_PrMutex * PCO_PrMutex;
// Don't use char buffer because of alignment
static t_UInt64 au_PrMutexBuffer [(sizeof (ct_PrMutex) / sizeof (t_UInt64)) + 1];

//---------------------------------------------------------------------------

static inline void tl_CreatePrMutex ()
  {
  if (PCO_PrMutex == 0)
    {
    PCO_PrMutex = new (au_PrMutexBuffer) ct_PrMutex;
    PCO_PrMutex-> Create ();
    }
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_CriticalPrSectionInitSuccess ()
  {
  tl_CreatePrMutex ();
  return PCO_PrMutex-> GetInitSuccess ();
  }

//---------------------------------------------------------------------------

void TL_EXPORT tl_DeleteCriticalPrSection ()
  {
  if (PCO_PrMutex != 0)
    {
    delete PCO_PrMutex;
    PCO_PrMutex = 0;
    }
  }

//---------------------------------------------------------------------------

et_ResError TL_EXPORT tl_TryEnterCriticalPrSection
(
bool & b_success,
t_UInt32 u_milliSec
)
  {
  tl_CreatePrMutex ();
  return PCO_PrMutex-> TryLock (b_success, u_milliSec);
  }

//---------------------------------------------------------------------------

et_ResError TL_EXPORT tl_EnterCriticalPrSection ()
  {
  tl_CreatePrMutex ();
  return PCO_PrMutex-> Lock ();
  }

//---------------------------------------------------------------------------

et_ResError TL_EXPORT tl_LeaveCriticalPrSection ()
  {
  tl_CreatePrMutex ();
  return PCO_PrMutex-> Unlock ();
  }

//---------------------------------------------------------------------------

static void tl_CreateGlobals ()
  {
  CreateStdStore ();
  CreateRndStore ();
  CreateChnStore ();
  tl_CreateThMutex ();
  tl_CreatePrMutex ();
  }

//---------------------------------------------------------------------------
// Shared resource

ct_SharedResource::ct_SharedResource ()
  {
  b_InitSuccess = false;
  }

//---------------------------------------------------------------------------

ct_SharedResource::ct_SharedResource
(
const char * pc_key
)
  {
  b_InitSuccess = false;

  if ((pc_key != 0) && (* pc_key != '\0'))
    co_Key. Assign (pc_key);
  }

//---------------------------------------------------------------------------

ct_SharedResource::ct_SharedResource
(
const char * pc_key,
unsigned u_idx
)
  {
  b_InitSuccess = false;

  if ((pc_key != 0) && (* pc_key != '\0'))
    co_Key. AssignF ("%s%d", pc_key, u_idx);
  }

//---------------------------------------------------------------------------

bool ct_SharedResource::GetInitSuccess () const
  {
  return b_InitSuccess;
  }

//---------------------------------------------------------------------------

const char * ct_SharedResource::GetKey () const
  {
  return co_Key. GetStr ();
  }

//---------------------------------------------------------------------------

et_ResError ct_SharedResource::SetKey
(
const char * pc_key
)
  {
  if (b_InitSuccess)
    return ec_ResAlreadyInitialized;

  if ((pc_key != 0) && (* pc_key != '\0'))
    {
    co_Key. Assign (pc_key);
    return ec_ResOK;
    }
  else
    {
    co_Key. Clear ();
    return ec_ResInvalidKey;
    }
  }

//---------------------------------------------------------------------------

et_ResError ct_SharedResource::SetKey
(
const char * pc_key,
unsigned u_idx
)
  {
  if (b_InitSuccess)
    return ec_ResAlreadyInitialized;

  if ((pc_key != 0) && (* pc_key != '\0'))
    {
    co_Key. AssignF ("%s%d", pc_key, u_idx);
    return ec_ResOK;
    }
  else
    {
    co_Key. Clear ();
    return ec_ResInvalidKey;
    }
  }

//===========================================================================

#if defined TL_WINDOWS

#include <windows.h>
#include <process.h>
#include <stddef.h>

//---------------------------------------------------------------------------
// Interlocked memory access

class ct_InterlockedSizeChecker
  {
public:
  ct_InterlockedSizeChecker ()
    { TL_ASSERT (sizeof (t_Int32) == sizeof (LONG)); }
  }
static CO_InterlockedSizeChecker;

//---------------------------------------------------------------------------

t_Int32 TL_EXPORT tl_InterlockedRead
(
volatile t_Int32 * pi_value
)
  {
  return (t_Int32) InterlockedCompareExchange ((volatile LONG *) pi_value, 0, 0);
  }

//---------------------------------------------------------------------------

t_Int32 TL_EXPORT tl_InterlockedWrite
(
volatile t_Int32 * pi_value,
t_Int32 i_new
)
  {
  return (t_Int32) InterlockedExchange ((volatile LONG *) pi_value, i_new);
  }

//---------------------------------------------------------------------------

t_Int32 TL_EXPORT tl_InterlockedAdd
(
volatile t_Int32 * pi_value,
t_Int32 i_add
)
  {
  return (t_Int32) InterlockedExchangeAdd ((volatile LONG *) pi_value, i_add) + i_add;
  }

//---------------------------------------------------------------------------

t_Int32 TL_EXPORT tl_InterlockedIncrement
(
volatile t_Int32 * pi_value
)
  {
  return (t_Int32) InterlockedIncrement ((volatile LONG *) pi_value);
  }

//---------------------------------------------------------------------------

t_Int32 TL_EXPORT tl_InterlockedDecrement
(
volatile t_Int32 * pi_value
)
  {
  return (t_Int32) InterlockedDecrement ((volatile LONG *) pi_value);
  }

//---------------------------------------------------------------------------
// Delay

void TL_EXPORT tl_Delay
(
int i_milliSec
)
  {
  Sleep (i_milliSec);
  }

//---------------------------------------------------------------------------

void TL_EXPORT tl_RelinquishTimeSlice ()
  {
  Sleep (0);
  }

//---------------------------------------------------------------------------
// Environment variable

ct_String TL_EXPORT tl_GetEnv
(
const char * pc_name
)
  {
  ct_String co_result;

  if ((pc_name != 0) && (* pc_name != '\0'))
    {
    size_t o_size;
    getenv_s (& o_size, 0, 0, pc_name);

    if (o_size > 1)
      {
      co_result. Assign (' ', o_size - 1);
      getenv_s (& o_size, (char *) co_result. GetStr (), o_size, pc_name);
      }
    }

  return co_result;
  }

//---------------------------------------------------------------------------

ct_String TL_EXPORT tl_GetTempPath ()
  {
  ct_String co_result;
  char ac_tempPathBuffer [MAX_PATH];
  DWORD u_retVal = GetTempPathA (MAX_PATH, ac_tempPathBuffer);

  if ((u_retVal != 0) && (u_retVal <= MAX_PATH))
    co_result. Assign (ac_tempPathBuffer);

  return co_result;
  }

//---------------------------------------------------------------------------
// Threads

#if defined TL_MULTI

unsigned __MaxThreads = cu_MaxThreads;

//---------------------------------------------------------------------------

bool TL_EXPORT tl_BeginThread
(
ft_ThreadFunc fo_func,
void * pv_param,
t_UInt u_stackSize
)
  {
  tl_CreateGlobals ();
  return _beginthread (fo_func, (unsigned) u_stackSize, pv_param) != (uintptr_t) -1L;
  }

//---------------------------------------------------------------------------

void TL_EXPORT tl_EndThread ()
  {
  _endthread ();
  }

//---------------------------------------------------------------------------

t_UInt64 TL_EXPORT tl_ThreadId ()
  {
  return GetCurrentThreadId ();
  }

#endif

//---------------------------------------------------------------------------
// Processes

void TL_EXPORT tl_EndProcess
(
unsigned u_exitCode
)
  {
  ExitProcess (u_exitCode);
  }

//---------------------------------------------------------------------------

int TL_EXPORT tl_ProcessId ()
  {
  return GetCurrentProcessId ();
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_IsProcessRunning
(
int i_processId
)
  {
  HANDLE o_phd = OpenProcess (/*dwDesiredAccess*/ PROCESS_QUERY_INFORMATION,
    /*bInheritHandle*/ FALSE, i_processId);

  if (o_phd == NULL)
    return false;
  else
    {
    DWORD o_exitCode;
    bool b_run = (GetExitCodeProcess (o_phd, & o_exitCode) ?
      (o_exitCode == STILL_ACTIVE) : true);
    CloseHandle (o_phd);
    return b_run;
    }
  }

//---------------------------------------------------------------------------

int TL_EXPORT tl_Exec
(
const char * pc_path,
unsigned u_params,
const char * * ppc_params,
bool b_wait
)
  {
  if ((pc_path == 0) || (* pc_path == '\0'))
    return -1;

  ct_FileName co_path (pc_path);

  if (! co_path. HasExt ())
    co_path. SetExt ("exe");

  ct_String co_params;
  co_params. Append ("\"");
  co_params. Append (co_path);
  co_params. Append ("\"");

  for (unsigned u = 0; u < u_params; u ++)
    {
    co_params. Append (' ');

    if ((ppc_params [u] == 0) || (* ppc_params [u] == '\0'))
      co_params. Append ("\"\"");
    else
      {
      t_ConstVoidPtr pc_space =
        tl_FirstChar (ppc_params [u], tl_StringLength (ppc_params [u]), ' ');

      if ((pc_space != 0) && (* ppc_params [u] != '"'))
        {
        co_params. Append ("\"");
        co_params. Append (ppc_params [u]);
        co_params. Append ("\"");
        }
      else
        co_params. Append (ppc_params [u]);
      }
    }

  STARTUPINFOA so_si;
  PROCESS_INFORMATION so_pi;
  ZeroMemory (& so_si, sizeof (so_si));
  so_si. cb = sizeof (so_si);
  ZeroMemory (& so_pi, sizeof (so_pi));

  if (CreateProcessA (co_path. GetStr (), (LPSTR) co_params. GetStr (),
      NULL, NULL, FALSE, b_wait ? CREATE_SUSPENDED : 0, NULL, NULL, & so_si, & so_pi) == 0)
    return -1;

  if (b_wait)
    {
    ResumeThread (so_pi. hThread);
    WaitForSingleObject (so_pi. hProcess, INFINITE);
    DWORD o_exitCode = (DWORD) -1;
    GetExitCodeProcess (so_pi. hProcess, & o_exitCode);
    CloseHandle (so_pi. hProcess);
    CloseHandle (so_pi. hThread);
    return o_exitCode;
    }
  else
    {
    int i_id = GetProcessId (so_pi. hProcess);
    CloseHandle (so_pi. hProcess);
    CloseHandle (so_pi. hThread);
    return i_id;
    }
  }

//---------------------------------------------------------------------------
// Thread mutex

#if defined TL_MULTI

class ct_ThMutexSizeChecker
  {
public:
  ct_ThMutexSizeChecker ()
    { TL_ASSERT (cu_ThMutexDataSize >= sizeof (CRITICAL_SECTION)); }
  }
static CO_ThMutexSizeChecker;

#define PO_CRITICAL_SECTION ((CRITICAL_SECTION *) (t_UInt64 *) au_Data)

//---------------------------------------------------------------------------

ct_ThMutex::ct_ThMutex ()
  {
  // MSDN: The heap manager uses a spin count of roughly 4000 for its per-heap critical sections.
  b_InitSuccess = (InitializeCriticalSectionAndSpinCount (PO_CRITICAL_SECTION, 4000) != 0);
  i_LockCount = 0;
  }

//---------------------------------------------------------------------------

ct_ThMutex::~ct_ThMutex ()
  {
  TL_ASSERT (i_LockCount == 0);

  if (b_InitSuccess)
    DeleteCriticalSection (PO_CRITICAL_SECTION);
  }

//---------------------------------------------------------------------------

bool ct_ThMutex::GetInitSuccess () const
  {
  return b_InitSuccess;
  }

//---------------------------------------------------------------------------

et_ResError ct_ThMutex::TryLock
(
bool & b_success
)
  {
  b_success = false;

  if (b_InitSuccess)
    {
    if (TryEnterCriticalSection (PO_CRITICAL_SECTION) != 0)
      {
      i_LockCount ++;
      b_success = true;
      }

    return ec_ResOK;
    }

  return ec_ResUnknownError;
  }

//---------------------------------------------------------------------------

et_ResError ct_ThMutex::Lock ()
  {
  if (b_InitSuccess)
    {
    EnterCriticalSection (PO_CRITICAL_SECTION);
    i_LockCount ++;
    return ec_ResOK;
    }

  return ec_ResUnknownError;
  }

//---------------------------------------------------------------------------

et_ResError ct_ThMutex::Unlock ()
  {
  if (b_InitSuccess && (i_LockCount > 0))
    {
    i_LockCount --;
    LeaveCriticalSection (PO_CRITICAL_SECTION);
    return ec_ResOK;
    }

  return ec_ResUnknownError;
  }

#endif

//---------------------------------------------------------------------------
// Thread semaphore

#if defined TL_MULTI

class ct_ThSemaphoreSizeChecker
  {
public:
  ct_ThSemaphoreSizeChecker ()
    { TL_ASSERT (cu_ThSemaphoreDataSize >= sizeof (HANDLE)); }
  }
static CO_ThSemaphoreSizeChecker;

#define CO_SEM_HANDLE ((HANDLE &) (au_Data [0]))

//---------------------------------------------------------------------------

#ifndef LONG_MAX
#define LONG_MAX 2147483647L
#endif

ct_ThSemaphore::ct_ThSemaphore
(
t_Int32 i_initValue
)
  {
  b_InitSuccess = false;

  if (i_initValue < 0)
    return;

  CO_SEM_HANDLE = CreateSemaphoreA (NULL, i_initValue, LONG_MAX, NULL);

  if (CO_SEM_HANDLE == NULL)
    return;

  b_InitSuccess = true;
  }

//---------------------------------------------------------------------------

ct_ThSemaphore::~ct_ThSemaphore ()
  {
  if (b_InitSuccess)
    CloseHandle (CO_SEM_HANDLE);
  }

//---------------------------------------------------------------------------

bool ct_ThSemaphore::GetInitSuccess () const
  {
  return b_InitSuccess;
  }

//---------------------------------------------------------------------------

et_ResError ct_ThSemaphore::TryAcquire
(
bool & b_success,
t_UInt32 u_milliSec
)
  {
  b_success = false;

  if (! b_InitSuccess)
    return ec_ResUninitialized;

  switch (WaitForSingleObject (CO_SEM_HANDLE, u_milliSec))
    {
    case WAIT_OBJECT_0:
      b_success = true;
      return ec_ResOK;

    case WAIT_TIMEOUT:
      return ec_ResOK;

    default:
      return ec_ResUnknownError;
    }
  }

//---------------------------------------------------------------------------

et_ResError ct_ThSemaphore::Acquire ()
  {
  if (! b_InitSuccess)
    return ec_ResUninitialized;

  if (WaitForSingleObject (CO_SEM_HANDLE, INFINITE) == WAIT_OBJECT_0)
    return ec_ResOK;

  return ec_ResUnknownError;
  }

//---------------------------------------------------------------------------

et_ResError ct_ThSemaphore::Release ()
  {
  if (! b_InitSuccess)
    return ec_ResUninitialized;

  if (ReleaseSemaphore (CO_SEM_HANDLE, 1, NULL))
    return ec_ResOK;

  return ec_ResUnknownError;
  }

#endif

//---------------------------------------------------------------------------
// Process mutex

ct_PrMutex::ct_PrMutex ()
  {
  co_Key. Assign ("Spirick_Tuning_PrMutex");
  pv_Id = 0;
  i_LockCount = 0;
  }

//---------------------------------------------------------------------------

ct_PrMutex::ct_PrMutex
(
const char * pc_key
): ct_SharedResource (pc_key)
  {
  pv_Id = 0;
  i_LockCount = 0;
  }

//---------------------------------------------------------------------------

ct_PrMutex::ct_PrMutex
(
const char * pc_key,
unsigned u_idx
): ct_SharedResource (pc_key, u_idx)
  {
  pv_Id = 0;
  i_LockCount = 0;
  }

//---------------------------------------------------------------------------

ct_PrMutex::~ct_PrMutex ()
  {
  TL_ASSERT (i_LockCount == 0);

  if (b_InitSuccess)
    Close ();
  }

//---------------------------------------------------------------------------

et_ResError ct_PrMutex::Open ()
  {
  if (b_InitSuccess)
    return ec_ResAlreadyInitialized;

  if (co_Key. IsEmpty ())
    return ec_ResNoKey;

  pv_Id = OpenMutexA (SYNCHRONIZE, FALSE, co_Key. GetStr ());

  if (pv_Id == 0)
    {
    switch (GetLastError ())
      {
      case ERROR_ACCESS_DENIED:
        return ec_ResAccessDenied;

      case ERROR_FILE_NOT_FOUND:
        return ec_ResNotFound;

      default:
        return ec_ResUnknownError;
      }
    }

  b_InitSuccess = true;
  return ec_ResOK;
  }

//---------------------------------------------------------------------------

et_ResError ct_PrMutex::Create
(
bool b_createNew
)
  {
  if (b_InitSuccess)
    return ec_ResAlreadyInitialized;

  if (co_Key. IsEmpty ())
    return ec_ResNoKey;

  pv_Id = CreateMutexA (NULL, FALSE, co_Key. GetStr ());

  if (pv_Id == 0)
    {
    switch (GetLastError ())
      {
      case ERROR_ACCESS_DENIED:
        return ec_ResAccessDenied;

      case ERROR_INVALID_HANDLE:
        return ec_ResAlreadyExists;

      default:
        return ec_ResUnknownError;
      }
    }
  else
    {
    if (b_createNew && (GetLastError () == ERROR_ALREADY_EXISTS))
      {
      CloseHandle (pv_Id);
      pv_Id = 0;
      return ec_ResAlreadyExists;
      }
    }

  b_InitSuccess = true;
  return ec_ResOK;
  }

//---------------------------------------------------------------------------

et_ResError ct_PrMutex::Close ()
  {
  if (i_LockCount != 0)
    return ec_ResLockCountMismatch;

  if (! b_InitSuccess)
    return ec_ResUninitialized;

  b_InitSuccess = false;

  if (CloseHandle (pv_Id) == 0)
    {
    pv_Id = 0;
    return ec_ResUnknownError;
    }

  pv_Id = 0;
  return ec_ResOK;
  }

//---------------------------------------------------------------------------

et_ResError ct_PrMutex::TryLock
(
bool & b_success,
t_UInt32 u_milliSec
)
  {
  b_success = false;

  if (! b_InitSuccess)
    return ec_ResUninitialized;

  switch (WaitForSingleObject (pv_Id, u_milliSec))
    {
    case WAIT_OBJECT_0:
      i_LockCount ++;
      b_success = true;
      return ec_ResOK;

    case WAIT_TIMEOUT:
      return ec_ResOK;

    default:
      return ec_ResUnknownError;
    }
  }

//---------------------------------------------------------------------------

et_ResError ct_PrMutex::Lock ()
  {
  if (! b_InitSuccess)
    return ec_ResUninitialized;

  if (WaitForSingleObject (pv_Id, INFINITE) == WAIT_OBJECT_0)
    {
    i_LockCount ++;
    return ec_ResOK;
    }

  return ec_ResUnknownError;
  }

//---------------------------------------------------------------------------

et_ResError ct_PrMutex::Unlock ()
  {
  if (! b_InitSuccess)
    return ec_ResUninitialized;

  if (i_LockCount <= 0)
    return ec_ResLockCountMismatch;

  i_LockCount --;

  if (ReleaseMutex (pv_Id) != 0)
    return ec_ResOK;

  i_LockCount ++;
  return ec_ResUnknownError;
  }

//---------------------------------------------------------------------------
// Process semaphore

ct_PrSemaphore::ct_PrSemaphore ()
  {
  co_Key. Assign ("Spirick_Tuning_PrSemaphore");
  pv_Id = 0;
  }

//---------------------------------------------------------------------------

ct_PrSemaphore::ct_PrSemaphore
(
const char * pc_key
): ct_SharedResource (pc_key)
  {
  pv_Id = 0;
  }

//---------------------------------------------------------------------------

ct_PrSemaphore::ct_PrSemaphore
(
const char * pc_key,
unsigned u_idx
): ct_SharedResource (pc_key, u_idx)
  {
  pv_Id = 0;
  }

//---------------------------------------------------------------------------

ct_PrSemaphore::~ct_PrSemaphore ()
  {
  if (b_InitSuccess)
    Close ();
  }

//---------------------------------------------------------------------------

et_ResError ct_PrSemaphore::Open ()
  {
  if (b_InitSuccess)
    return ec_ResAlreadyInitialized;

  if (co_Key. IsEmpty ())
    return ec_ResNoKey;

  pv_Id = OpenSemaphoreA (SYNCHRONIZE | SEMAPHORE_MODIFY_STATE, FALSE, co_Key. GetStr ());

  if (pv_Id == 0)
    {
    switch (GetLastError ())
      {
      case ERROR_ACCESS_DENIED:
        return ec_ResAccessDenied;

      case ERROR_FILE_NOT_FOUND:
        return ec_ResNotFound;

      default:
        return ec_ResUnknownError;
      }
    }

  b_InitSuccess = true;
  return ec_ResOK;
  }

//---------------------------------------------------------------------------

et_ResError ct_PrSemaphore::Create
(
t_Int32 i_initValue,
bool b_createNew
)
  {
  if (i_initValue < 0)
    return ec_ResInvalidValue;

  if (b_InitSuccess)
    return ec_ResAlreadyInitialized;

  if (co_Key. IsEmpty ())
    return ec_ResNoKey;

  pv_Id = CreateSemaphoreA (NULL, i_initValue, LONG_MAX, co_Key. GetStr ());

  if (pv_Id == 0)
    {
    switch (GetLastError ())
      {
      case ERROR_ACCESS_DENIED:
        return ec_ResAccessDenied;

      case ERROR_INVALID_HANDLE:
        return ec_ResAlreadyExists;

      default:
        return ec_ResUnknownError;
      }
    }
  else
    {
    if (b_createNew && (GetLastError () == ERROR_ALREADY_EXISTS))
      {
      CloseHandle (pv_Id);
      pv_Id = 0;
      return ec_ResAlreadyExists;
      }
    }

  b_InitSuccess = true;
  return ec_ResOK;
  }

//---------------------------------------------------------------------------

et_ResError ct_PrSemaphore::Close ()
  {
  if (! b_InitSuccess)
    return ec_ResUninitialized;

  b_InitSuccess = false;

  if (CloseHandle (pv_Id) == 0)
    {
    pv_Id = 0;
    return ec_ResUnknownError;
    }

  pv_Id = 0;
  return ec_ResOK;
  }

//---------------------------------------------------------------------------

et_ResError ct_PrSemaphore::TryAcquire
(
bool & b_success,
t_UInt32 u_milliSec
)
  {
  b_success = false;

  if (! b_InitSuccess)
    return ec_ResUninitialized;

  switch (WaitForSingleObject (pv_Id, u_milliSec))
    {
    case WAIT_OBJECT_0:
      b_success = true;
      return ec_ResOK;

    case WAIT_TIMEOUT:
      return ec_ResOK;

    default:
      return ec_ResUnknownError;
    }
  }

//---------------------------------------------------------------------------

et_ResError ct_PrSemaphore::Acquire ()
  {
  if (! b_InitSuccess)
    return ec_ResUninitialized;

  if (WaitForSingleObject (pv_Id, INFINITE) == WAIT_OBJECT_0)
    return ec_ResOK;

  return ec_ResUnknownError;
  }

//---------------------------------------------------------------------------

et_ResError ct_PrSemaphore::Release ()
  {
  if (! b_InitSuccess)
    return ec_ResUninitialized;

  if (ReleaseSemaphore (pv_Id, 1, NULL))
    return ec_ResOK;

  return ec_ResUnknownError;
  }

//---------------------------------------------------------------------------
// Shared memory

ct_SharedMemory::ct_SharedMemory ()
  {
  co_Key. Assign ("Spirick_Tuning_SharedMemory");
  pv_Id = 0;
  u_Size = 0;
  pv_Data = 0;
  }

//---------------------------------------------------------------------------

ct_SharedMemory::ct_SharedMemory
(
const char * pc_key
): ct_SharedResource (pc_key)
  {
  pv_Id = 0;
  u_Size = 0;
  pv_Data = 0;
  }

//---------------------------------------------------------------------------

ct_SharedMemory::ct_SharedMemory
(
const char * pc_key,
unsigned u_idx
): ct_SharedResource (pc_key, u_idx)
  {
  pv_Id = 0;
  u_Size = 0;
  pv_Data = 0;
  }

//---------------------------------------------------------------------------

ct_SharedMemory::~ct_SharedMemory ()
  {
  if (b_InitSuccess)
    Close ();
  }

//---------------------------------------------------------------------------

et_ResError ct_SharedMemory::Open
(
bool b_readOnly
)
  {
  if (b_InitSuccess)
    return ec_ResAlreadyInitialized;

  u_Size = 0;
  pv_Data = 0;

  if (co_Key. IsEmpty ())
    return ec_ResNoKey;

  DWORD u_access = b_readOnly ? FILE_MAP_READ : FILE_MAP_ALL_ACCESS;
  pv_Id = OpenFileMappingA (u_access, FALSE, co_Key. GetStr ());

  if (pv_Id == 0)
    {
    switch (GetLastError ())
      {
      case ERROR_ACCESS_DENIED:
        return ec_ResAccessDenied;

      case ERROR_FILE_NOT_FOUND:
        return ec_ResNotFound;

      default:
        return ec_ResUnknownError;
      }
    }

  pv_Data = MapViewOfFile (pv_Id, u_access, 0, 0, 0);

  if (pv_Data == 0)
    {
    CloseHandle (pv_Id);
    pv_Id = 0;
    return ec_ResMemMapFailed;
    }

  MEMORY_BASIC_INFORMATION so_info;

  if (VirtualQuery (pv_Data, & so_info, sizeof (so_info)) == 0)
    {
    UnmapViewOfFile (pv_Data);
    pv_Data = 0;
    CloseHandle (pv_Id);
    pv_Id = 0;
    return ec_ResQuerySizeFailed;
    }

  u_Size = so_info. RegionSize;
  b_InitSuccess = true;
  return ec_ResOK;
  }

//---------------------------------------------------------------------------

et_ResError ct_SharedMemory::Create
(
t_UInt u_size,
bool b_createNew
)
  {
  if (b_InitSuccess)
    return ec_ResAlreadyInitialized;

  u_Size = 0;
  pv_Data = 0;

  if (co_Key. IsEmpty ())
    return ec_ResNoKey;

  pv_Id = CreateFileMappingA (INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE,
    (DWORD) (((DWORD64) u_size) >> 32), (DWORD) u_size, co_Key. GetStr ());

  if (pv_Id == 0)
    {
    switch (GetLastError ())
      {
      case ERROR_ACCESS_DENIED:
        return ec_ResAccessDenied;

      case ERROR_INVALID_HANDLE:
        return ec_ResAlreadyExists;

      default:
        return ec_ResUnknownError;
      }
    }
  else
    {
    if (b_createNew && (GetLastError () == ERROR_ALREADY_EXISTS))
      {
      CloseHandle (pv_Id);
      pv_Id = 0;
      return ec_ResAlreadyExists;
      }
    }

  pv_Data = MapViewOfFile (pv_Id, FILE_MAP_ALL_ACCESS, 0, 0, 0);

  if (pv_Data == 0)
    {
    CloseHandle (pv_Id);
    pv_Id = 0;
    return ec_ResMemMapFailed;
    }

  u_Size = u_size;
  b_InitSuccess = true;
  return ec_ResOK;
  }

//---------------------------------------------------------------------------

et_ResError ct_SharedMemory::Close ()
  {
  if (! b_InitSuccess)
    return ec_ResUninitialized;

  b_InitSuccess = false;
  u_Size = 0;

  if (UnmapViewOfFile (pv_Data) == 0)
    {
    pv_Data = 0;
    CloseHandle (pv_Id);
    pv_Id = 0;
    return ec_ResUnmapFailed;
    }

  pv_Data = 0;

  if (CloseHandle (pv_Id) == 0)
    {
    pv_Id = 0;
    return ec_ResUnknownError;
    }

  pv_Id = 0;
  return ec_ResOK;
  }

//---------------------------------------------------------------------------

t_UInt ct_SharedMemory::GetSize () const
  {
  return u_Size;
  }

//---------------------------------------------------------------------------

void * ct_SharedMemory::GetData () const
  {
  return pv_Data;
  }

//===========================================================================

#elif defined TL_UNIX

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <spawn.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "tuning/sys/cfile.hpp"

//---------------------------------------------------------------------------
// Interlocked memory access

t_Int32 TL_EXPORT tl_InterlockedRead
(
volatile t_Int32 * pi_value
)
  {
  return __sync_fetch_and_add (pi_value, 0);
  }

//---------------------------------------------------------------------------

t_Int32 TL_EXPORT tl_InterlockedWrite
(
volatile t_Int32 * pi_value,
t_Int32 i_new
)
  {
  __sync_synchronize ();
  return __sync_lock_test_and_set (pi_value, i_new);
  }

//---------------------------------------------------------------------------

t_Int32 TL_EXPORT tl_InterlockedAdd
(
volatile t_Int32 * pi_value,
t_Int32 i_add
)
  {
  return __sync_add_and_fetch (pi_value, i_add);
  }

//---------------------------------------------------------------------------

t_Int32 TL_EXPORT tl_InterlockedIncrement
(
volatile t_Int32 * pi_value
)
  {
  return __sync_add_and_fetch (pi_value, 1);
  }

//---------------------------------------------------------------------------

t_Int32 TL_EXPORT tl_InterlockedDecrement
(
volatile t_Int32 * pi_value
)
  {
  return __sync_sub_and_fetch (pi_value, 1);
  }

//---------------------------------------------------------------------------
// Delay

void TL_EXPORT tl_Delay
(
int i_milliSec
)
  {
  timespec so_rqtp;
  so_rqtp. tv_sec  = i_milliSec / 1000;
  so_rqtp. tv_nsec = 1000000 * (i_milliSec % 1000);
  nanosleep (& so_rqtp, 0);
  }

//---------------------------------------------------------------------------

void TL_EXPORT tl_RelinquishTimeSlice ()
  {
  sched_yield ();
  }

//---------------------------------------------------------------------------
// Environment variable

ct_String TL_EXPORT tl_GetEnv
(
const char * pc_name
)
  {
  ct_String co_result;

  if ((pc_name != 0) && (* pc_name != '\0'))
    {
    char * pc_result = getenv (pc_name);

    if ((pc_result != 0) && (* pc_result != '\0'))
      co_result. Assign (pc_result);
    }

  return co_result;
  }

//---------------------------------------------------------------------------

ct_String TL_EXPORT tl_GetTempPath ()
  {
  ct_String co_result;
  char * pc_tmpdir = getenv ("TMPDIR");

  if ((pc_tmpdir != 0) && (* pc_tmpdir != '\0'))
    co_result. Assign (pc_tmpdir);
  else
    co_result. Assign ("/tmp/");

  return co_result;
  }

//---------------------------------------------------------------------------
// Threads

#if defined TL_MULTI

#include <pthread.h>

static int            I_PthInitThread;
static pthread_attr_t O_PthAttr;

//---------------------------------------------------------------------------

static inline void CheckPthInitThread ()
  {
  if (I_PthInitThread == 0)
    {
    I_PthInitThread = 1;
    pthread_attr_init (& O_PthAttr);
    pthread_attr_setscope (& O_PthAttr, PTHREAD_SCOPE_PROCESS);
    //pthread_attr_setscope (& O_PthAttr, PTHREAD_SCOPE_SYSTEM);
    }
  }

//---------------------------------------------------------------------------

struct st_PrivateThreadMain
  {
  ft_ThreadFunc fo_func;
  void *        pv_param;
  };

//---------------------------------------------------------------------------

static void * tl_ThreadMain
(
void * pv_threadMain
)
  {
  st_PrivateThreadMain * pso_threadMain = (st_PrivateThreadMain *) pv_threadMain;
  st_PrivateThreadMain so_threadMain = * pso_threadMain;
  delete pso_threadMain;
  so_threadMain. fo_func (so_threadMain. pv_param);
  return 0;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_BeginThread
(
ft_ThreadFunc fo_func,
void * pv_param,
t_UInt /* u_stackSize */
)
  {
  tl_CreateGlobals ();
  CheckPthInitThread ();
  pthread_t o_thread;
  st_PrivateThreadMain * pso_threadMain = new st_PrivateThreadMain;

  if (pso_threadMain == 0)
    return false;

  pso_threadMain-> fo_func  = fo_func;
  pso_threadMain-> pv_param = pv_param;

  if (pthread_create (& o_thread, & O_PthAttr, tl_ThreadMain, pso_threadMain) == 0)
    {
    return true;
    }
  else
    {
    delete pso_threadMain;
    return false;
    }
  }

//---------------------------------------------------------------------------

void TL_EXPORT tl_EndThread ()
  {
  CheckPthInitThread ();
  pthread_exit (0);
  }

//---------------------------------------------------------------------------

t_UInt64 TL_EXPORT tl_ThreadId ()
  {
  CheckPthInitThread ();
  return pthread_self ();
  }

#endif

//---------------------------------------------------------------------------
// Processes

void TL_EXPORT tl_EndProcess
(
unsigned u_exitCode
)
  {
  _exit (u_exitCode);
  }

//---------------------------------------------------------------------------

int TL_EXPORT tl_ProcessId ()
  {
  return getpid ();
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_IsProcessRunning
(
int i_processId
)
  {
  ct_String co_procId;
  co_procId. AssignF ("/proc/%d", i_processId);

  if (access (co_procId. GetStr (), F_OK) != 0)
    return false;

  bool b_ret = false;
  FILE * p_stat;
  co_procId. Append ("/stat");

  if ((p_stat = fopen (co_procId. GetStr (), "r")) != 0)
    {
    int i_char = ' ';

    // Search end of (comm) entry
    while ((i_char != EOF) && (i_char != ')'))
      i_char = fgetc (p_stat);

    if (i_char != EOF)
      {
      i_char = fgetc (p_stat);

      while ((i_char != EOF) && (i_char <= ' '))
        i_char = fgetc (p_stat);

      if (i_char != EOF)
        b_ret = ((i_char != 'X') && (i_char != 'Z'));
      }

    fclose (p_stat);
    }

  return b_ret;
  }

//---------------------------------------------------------------------------

int TL_EXPORT tl_Exec
(
const char * pc_path,
unsigned u_params,
const char * * ppc_params,
bool b_wait
)
  {
  if ((pc_path == 0) || (* pc_path == '\0'))
    return -1;

  char * * ppc_spawnParams = (char * *) tl_Alloc ((u_params + 2) * sizeof (char *));
  t_UInt u_len = tl_StringLength (pc_path) + 1;
  ppc_spawnParams [0] = (char *) tl_Alloc (u_len);
  tl_CopyMemory (ppc_spawnParams [0], pc_path, u_len);
  unsigned u;

  for (u = 0; u < u_params; u ++)
    {
    if (ppc_params [u] == 0)
      {
      ppc_spawnParams [u + 1] = (char *) tl_Alloc (1);
      * ppc_spawnParams [u + 1] = '\0';
      }
    else
      {
      u_len = tl_StringLength (ppc_params [u]) + 1;
      ppc_spawnParams [u + 1] = (char *) tl_Alloc (u_len);

      if ((* ppc_params [u] == '"') && (u_len > 2))
        {
        tl_CopyMemory (ppc_spawnParams [u + 1], ppc_params [u] + 1, u_len - 1);

        if (ppc_spawnParams [u + 1][u_len - 3] == '"')
          ppc_spawnParams [u + 1][u_len - 3] = '\0';
        }
      else
        tl_CopyMemory (ppc_spawnParams [u + 1], ppc_params [u], u_len);
      }
    }

  ppc_spawnParams [u_params + 1] = 0;
  pid_t o_pid = 0;
  int i_ret = posix_spawn (& o_pid, pc_path, 0, 0, ppc_spawnParams, 0);

  for (u = 0; u < u_params + 1; u ++)
    tl_Free (ppc_spawnParams [u]);

  tl_Free (ppc_spawnParams);

  if (i_ret != 0)
    return -1;
  else
    {
    if (b_wait)
      {
      int i_status;
      pid_t o_wait;

      do
        o_wait = waitpid (o_pid, & i_status, 0);
      while ((o_wait == (pid_t) -1) && (errno == EINTR));

      if ((o_wait == o_pid) && WIFEXITED (i_status))
        return WEXITSTATUS (i_status);
      else
        return -1;
      }
    else
      return (int) o_pid;
    }
  }

//---------------------------------------------------------------------------
// Thread mutex

#if defined TL_MULTI

class ct_ThMutexSizeChecker
  {
public:
  ct_ThMutexSizeChecker ()
    { TL_ASSERT (cu_ThMutexDataSize >= sizeof (pthread_mutex_t)); }
  }
static CO_ThMutexSizeChecker;

#define PO_MUTEX ((pthread_mutex_t *) (t_UInt64 *) au_Data)

//---------------------------------------------------------------------------

ct_ThMutex::ct_ThMutex ()
  {
  b_InitSuccess = false;
  i_LockCount = 0;
  pthread_mutexattr_t o_pthMutexAttr;

  if (pthread_mutexattr_init (& o_pthMutexAttr) == 0)
    {
    pthread_mutexattr_settype (& o_pthMutexAttr, PTHREAD_MUTEX_RECURSIVE);
    b_InitSuccess = (pthread_mutex_init (PO_MUTEX, & o_pthMutexAttr) == 0);
    pthread_mutexattr_destroy (& o_pthMutexAttr);
    }
  }

//---------------------------------------------------------------------------

ct_ThMutex::~ct_ThMutex ()
  {
  TL_ASSERT (i_LockCount == 0);

  if (b_InitSuccess)
    {
    int i_ret;

    do
      i_ret = pthread_mutex_destroy (PO_MUTEX);
    while (i_ret == EINTR);
    }
  }

//---------------------------------------------------------------------------

bool ct_ThMutex::GetInitSuccess () const
  {
  return b_InitSuccess;
  }

//---------------------------------------------------------------------------

et_ResError ct_ThMutex::TryLock
(
bool & b_success
)
  {
  b_success = false;

  if (b_InitSuccess)
    {
    int i_ret;

    do
      i_ret = pthread_mutex_trylock (PO_MUTEX);
    while (i_ret == EINTR);

    if (i_ret == EBUSY)
      return ec_ResOK;

    if (i_ret == 0)
      {
      i_LockCount ++;
      b_success = true;
      return ec_ResOK;
      }
    }

  return ec_ResUnknownError;
  }

//---------------------------------------------------------------------------

et_ResError ct_ThMutex::Lock ()
  {
  if (b_InitSuccess)
    {
    int i_ret;

    do
      i_ret = pthread_mutex_lock (PO_MUTEX);
    while (i_ret == EINTR);

    if (i_ret == 0)
      {
      i_LockCount ++;
      return ec_ResOK;
      }
    }

  return ec_ResUnknownError;
  }

//---------------------------------------------------------------------------

et_ResError ct_ThMutex::Unlock ()
  {
  if (b_InitSuccess && (i_LockCount > 0))
    {
    i_LockCount --;
    int i_ret;

    do
      i_ret = pthread_mutex_unlock (PO_MUTEX);
    while (i_ret == EINTR);

    if (i_ret == 0)
      return ec_ResOK;

    i_LockCount ++;
    }

  return ec_ResUnknownError;
  }

#endif

//---------------------------------------------------------------------------
// Thread semaphore

#if defined TL_MULTI

struct st_PrivateSemaphore
  {
  pthread_mutex_t      o_Mutex;
  pthread_cond_t       o_Condition;
  clockid_t            o_ClockId;
  t_Int32              i_SemCount;
  };

class ct_ThSemaphoreSizeChecker
  {
public:
  ct_ThSemaphoreSizeChecker ()
    { TL_ASSERT (cu_ThSemaphoreDataSize >= sizeof (st_PrivateSemaphore)); }
  }
static CO_ThSemaphoreSizeChecker;

#define CO_SEM_DATA ((st_PrivateSemaphore &) (au_Data [0]))

//---------------------------------------------------------------------------

ct_ThSemaphore::ct_ThSemaphore
(
t_Int32 i_initValue
)
  {
  b_InitSuccess = false;

  if (i_initValue < 0)
    return;

  CO_SEM_DATA. i_SemCount = i_initValue;
  pthread_condattr_t o_condAttr;

  if (pthread_condattr_init (& o_condAttr) != 0)
    return;

  if (pthread_condattr_getclock (& o_condAttr, & CO_SEM_DATA. o_ClockId) != 0)
    return;

  if (pthread_mutex_init (& CO_SEM_DATA. o_Mutex, 0) != 0)
    return;

  int i_ret1 = pthread_cond_init (& CO_SEM_DATA. o_Condition, & o_condAttr);
  int i_ret2 = pthread_condattr_destroy (& o_condAttr);

  if ((i_ret1 != 0) || (i_ret2 != 0))
    {
    pthread_mutex_destroy (& CO_SEM_DATA. o_Mutex);
    return;
    }

  b_InitSuccess = true;
  }

//---------------------------------------------------------------------------

ct_ThSemaphore::~ct_ThSemaphore ()
  {
  if (b_InitSuccess)
    {
    pthread_cond_destroy (& CO_SEM_DATA. o_Condition);
    pthread_mutex_destroy (& CO_SEM_DATA. o_Mutex);
    }
  }

//---------------------------------------------------------------------------

bool ct_ThSemaphore::GetInitSuccess () const
  {
  return b_InitSuccess;
  }

//---------------------------------------------------------------------------

et_ResError ct_ThSemaphore::TryAcquire
(
bool & b_success,
t_UInt32 u_milliSec
)
  {
  b_success = false;

  if (! b_InitSuccess)
    return ec_ResUninitialized;

  struct timespec so_timeSpec;

  if (clock_gettime (CO_SEM_DATA. o_ClockId, & so_timeSpec) != 0)
    return ec_ResUnknownError;

  so_timeSpec. tv_sec  += u_milliSec / 1000;
  so_timeSpec. tv_nsec += 1000000 * (u_milliSec % 1000);

  if (so_timeSpec. tv_nsec >= 1000000000)
    {
    so_timeSpec. tv_sec  += 1;
    so_timeSpec. tv_nsec -= 1000000000;
    }

  int i_mutRet;

  do
    i_mutRet = pthread_mutex_lock (& CO_SEM_DATA. o_Mutex);
  while (i_mutRet == EINTR);

  if (i_mutRet != 0)
    return ec_ResLockFailed;

  int i_condRet = 0;

  while ((CO_SEM_DATA. i_SemCount <= 0) && (i_condRet == 0))
    {
    do
      i_condRet = pthread_cond_timedwait (& CO_SEM_DATA. o_Condition,
        & CO_SEM_DATA. o_Mutex, & so_timeSpec);
    while (i_condRet == EINTR);
    }

  if (i_condRet == 0)
    CO_SEM_DATA. i_SemCount --;

  do
    i_mutRet = pthread_mutex_unlock (& CO_SEM_DATA. o_Mutex);
  while (i_mutRet == EINTR);

  if (i_mutRet != 0)
    {
    if (i_condRet == 0)
      CO_SEM_DATA. i_SemCount ++;

    return ec_ResUnlockFailed;
    }

  if (i_condRet == ETIMEDOUT)
    return ec_ResOK;

  if (i_condRet != 0)
    return ec_ResUnknownError;

  b_success = true;
  return ec_ResOK;
  }

//---------------------------------------------------------------------------

et_ResError ct_ThSemaphore::Acquire ()
  {
  if (! b_InitSuccess)
    return ec_ResUninitialized;

  int i_mutRet;

  do
    i_mutRet = pthread_mutex_lock (& CO_SEM_DATA. o_Mutex);
  while (i_mutRet == EINTR);

  if (i_mutRet != 0)
    return ec_ResLockFailed;

  int i_condRet = 0;

  while ((CO_SEM_DATA. i_SemCount <= 0) && (i_condRet == 0))
    {
    do
      i_condRet = pthread_cond_wait (& CO_SEM_DATA. o_Condition,
        & CO_SEM_DATA. o_Mutex);
    while (i_condRet == EINTR);
    }

  if (i_condRet == 0)
    CO_SEM_DATA. i_SemCount --;

  do
    i_mutRet = pthread_mutex_unlock (& CO_SEM_DATA. o_Mutex);
  while (i_mutRet == EINTR);

  if (i_mutRet != 0)
    {
    if (i_condRet == 0)
      CO_SEM_DATA. i_SemCount ++;

    return ec_ResUnlockFailed;
    }

  if (i_condRet != 0)
    return ec_ResUnknownError;

  return ec_ResOK;
  }

//---------------------------------------------------------------------------

et_ResError ct_ThSemaphore::Release ()
  {
  if (! b_InitSuccess)
    return ec_ResUninitialized;

  int i_ret;

  do
    i_ret = pthread_mutex_lock (& CO_SEM_DATA. o_Mutex);
  while (i_ret == EINTR);

  if (i_ret != 0)
    return ec_ResLockFailed;

  CO_SEM_DATA. i_SemCount ++;

  do
    i_ret = pthread_mutex_unlock (& CO_SEM_DATA. o_Mutex);
  while (i_ret == EINTR);

  if (i_ret != 0)
    {
    CO_SEM_DATA. i_SemCount --;
    return ec_ResUnlockFailed;
    }

  do
    i_ret = pthread_cond_signal (& CO_SEM_DATA. o_Condition);
  while (i_ret == EINTR);

  if (i_ret != 0)
    return ec_ResUnknownError;

  return ec_ResOK;
  }

#endif

//---------------------------------------------------------------------------
// Shared resource

void ct_SharedResource::CloseDeleteFile ()
  {
  tl_CloseFile (o_FileId);
  tl_DeleteFile (co_TempName. GetStr ());
  }

//---------------------------------------------------------------------------

et_ResError ct_SharedResource::OpenKey
(
int & i_key
)
  {
  if (co_Key. IsEmpty ())
    return ec_ResNoKey;

  co_TempName. AssignAsPath (tl_GetTempPath ());
  co_TempName. SetNameExt (co_Key);

  if (access (co_TempName. GetStr (), F_OK) != 0)
    return ec_ResNotFound;

  i_key = ftok (co_TempName. GetStr (), 's');

  if (i_key == -1)
    return ec_ResAccessDenied;

  return ec_ResOK;
  }

//---------------------------------------------------------------------------

et_ResError ct_SharedResource::CreateKey
(
int & i_key,
bool b_createNew
)
  {
  if (co_Key. IsEmpty ())
    return ec_ResNoKey;

  co_TempName. AssignAsPath (tl_GetTempPath ());
  co_TempName. SetNameExt (co_Key);

  if (! tl_CreateFile (co_TempName. GetStr (), o_FileId, b_createNew))
    return ec_ResAccessDenied;

  i_key = ftok (co_TempName. GetStr (), 's');

  if (i_key == -1)
    {
    CloseDeleteFile ();
    return ec_ResAccessDenied;
    }

  return ec_ResOK;
  }

//---------------------------------------------------------------------------

et_ResError ct_SharedResource::OpenSem ()
  {
  if (b_InitSuccess)
    return ec_ResAlreadyInitialized;

  int i_key;
  et_ResError eo_ret = OpenKey (i_key);

  if (eo_ret != ec_ResOK)
    return eo_ret;

  i_Id = semget (i_key, 1, 0666);

  if (i_Id == -1)
    {
    i_Id = 0;
    return ec_ResNotFound;
    }

  b_InitSuccess = true;
  return ec_ResOK;
  }

//---------------------------------------------------------------------------

union semun
  {
  int               val;
  struct semid_ds * buf;
  unsigned short *  array;
  struct seminfo *  __buf;
  };

et_ResError ct_SharedResource::CreateSem
(
t_Int32 i_initValue,
bool b_createNew
)
  {
  if (b_InitSuccess)
    return ec_ResAlreadyInitialized;

  int i_key;
  et_ResError eo_ret = CreateKey (i_key, b_createNew);

  if (eo_ret != ec_ResOK)
    return eo_ret;

  int i_flags = 0666 | IPC_CREAT;

  if (b_createNew)
    i_flags |= IPC_EXCL;

  i_Id = semget (i_key, 1, i_flags);

  if (i_Id == -1)
    {
    CloseDeleteFile ();
    i_Id = 0;
    return ec_ResAccessDenied;
    }

  union semun uo_semArg;
  uo_semArg. val = i_initValue;

  if (semctl (i_Id, 0, SETVAL, uo_semArg) == -1)
    {
    CloseDeleteFile ();
    i_Id = 0;
    return ec_ResAccessDenied;
    }

  b_InitSuccess = true;
  return ec_ResOK;
  }

//---------------------------------------------------------------------------

et_ResError ct_SharedResource::CloseSem ()
  {
  if (! b_InitSuccess)
    return ec_ResUninitialized;

  b_InitSuccess = false;

  if (o_FileId != co_InvalidFileId)
    {
    CloseDeleteFile ();

    if (semctl (i_Id, 0, IPC_RMID, 0) == -1)
      {
      i_Id = 0;
      return ec_ResUnknownError;
      }
    }

  i_Id = 0;
  return ec_ResOK;
  }

//---------------------------------------------------------------------------

static bool SemOp
(
int i_id,
int i_op,
bool b_undo
)
  {
  struct sembuf so_semBuf;
  so_semBuf. sem_num = 0;
  so_semBuf. sem_op  = i_op;
  so_semBuf. sem_flg = 0;

  if (b_undo)
    so_semBuf. sem_flg |= SEM_UNDO;

  int i_ret;

  do
    i_ret = semop (i_id, & so_semBuf, 1);
  while ((i_ret != 0) && (errno == EINTR));

  return i_ret == 0;
  }

//---------------------------------------------------------------------------

static bool SemNowaitOp
(
int i_id,
int i_op,
bool b_undo,
bool & b_success
)
  {
  b_success = false;
  struct sembuf so_semBuf;
  so_semBuf. sem_num = 0;
  so_semBuf. sem_op  = i_op;
  so_semBuf. sem_flg = IPC_NOWAIT;

  if (b_undo)
    so_semBuf. sem_flg |= SEM_UNDO;

  int i_ret;

  do
    i_ret = semop (i_id, & so_semBuf, 1);
  while ((i_ret != 0) && (errno == EINTR));

  if ((i_ret != 0) && (errno == EAGAIN))
    return true;

  if (i_ret == 0)
    {
    b_success = true;
    return true;
    }

  return false;
  }

//---------------------------------------------------------------------------

static bool SemTimedOp
(
int i_id,
int i_op,
bool b_undo,
bool & b_success,
t_UInt32 u_milliSec
)
  {
  b_success = false;
  struct sembuf so_semBuf;
  so_semBuf. sem_num = 0;
  so_semBuf. sem_op  = i_op;
  so_semBuf. sem_flg = 0;

  if (b_undo)
    so_semBuf. sem_flg |= SEM_UNDO;

  int i_ret;

  do
    {
    struct timespec so_timeSpec;
    so_timeSpec. tv_sec = u_milliSec / 1000;
    so_timeSpec. tv_nsec = 1000000 * (u_milliSec % 1000);
    t_MicroTime i_time = tl_QueryPrecisionTime ();
    i_ret = semtimedop (i_id, & so_semBuf, 1, & so_timeSpec);
    int i_diff = (int) ((tl_QueryPrecisionTime () - i_time) / co_MilliSecondFactor);

    if ((i_diff >= 0) && (u_milliSec > (t_UInt32) i_diff))
      u_milliSec -= i_diff;
    else
      u_milliSec = 0;
    }
  while ((i_ret != 0) && (errno == EINTR));

  if ((i_ret != 0) && (errno == EAGAIN))
    return true;

  if (i_ret == 0)
    {
    b_success = true;
    return true;
    }

  return false;
  }

//---------------------------------------------------------------------------
// Process mutex
// cmd-line: ipcs ipcrm

ct_PrMutex::ct_PrMutex ()
  {
  co_Key. Assign ("Spirick_Tuning_PrMutex");
  o_FileId = co_InvalidFileId;
  i_Id = 0;
  i_LockCount = 0;
  }

//---------------------------------------------------------------------------

ct_PrMutex::ct_PrMutex
(
const char * pc_key
): ct_SharedResource (pc_key)
  {
  o_FileId = co_InvalidFileId;
  i_Id = 0;
  i_LockCount = 0;
  }

//---------------------------------------------------------------------------

ct_PrMutex::ct_PrMutex
(
const char * pc_key,
unsigned u_idx
): ct_SharedResource (pc_key, u_idx)
  {
  o_FileId = co_InvalidFileId;
  i_Id = 0;
  i_LockCount = 0;
  }

//---------------------------------------------------------------------------

ct_PrMutex::~ct_PrMutex ()
  {
  TL_ASSERT (i_LockCount == 0);

  if (b_InitSuccess)
    Close ();
  }

//---------------------------------------------------------------------------

et_ResError ct_PrMutex::Open ()
  {
  return OpenSem ();
  }

//---------------------------------------------------------------------------

et_ResError ct_PrMutex::Create
(
bool b_createNew
)
  {
  return CreateSem (1, b_createNew);
  }

//---------------------------------------------------------------------------

et_ResError ct_PrMutex::Close ()
  {
  if (i_LockCount != 0)
    return ec_ResLockCountMismatch;

  return CloseSem ();
  }

//---------------------------------------------------------------------------

et_ResError ct_PrMutex::TryLock
(
bool & b_success,
t_UInt32 u_milliSec
)
  {
  b_success = false;

  if (! b_InitSuccess)
    return ec_ResUninitialized;

  if ((u_milliSec == 0) ?
      SemNowaitOp (i_Id, -1, /*b_undo*/ true, b_success) :
      SemTimedOp (i_Id, -1, /*b_undo*/ true, b_success, u_milliSec))
    {
    if (b_success)
      i_LockCount ++;

    return ec_ResOK;
    }

  return ec_ResUnknownError;
  }

//---------------------------------------------------------------------------

et_ResError ct_PrMutex::Lock ()
  {
  if (! b_InitSuccess)
    return ec_ResUninitialized;

  if (SemOp (i_Id, -1, /*b_undo*/ true))
    {
    i_LockCount ++;
    return ec_ResOK;
    }

  return ec_ResUnknownError;
  }

//---------------------------------------------------------------------------

et_ResError ct_PrMutex::Unlock ()
  {
  if (! b_InitSuccess)
    return ec_ResUninitialized;

  if (i_LockCount <= 0)
    return ec_ResLockCountMismatch;

  i_LockCount --;

  if (SemOp (i_Id, 1, /*b_undo*/ true))
    return ec_ResOK;

  i_LockCount ++;
  return ec_ResUnknownError;
  }

//---------------------------------------------------------------------------
// Process semaphore
// cmd-line: ipcs ipcrm

ct_PrSemaphore::ct_PrSemaphore ()
  {
  co_Key. Assign ("Spirick_Tuning_PrSemaphore");
  o_FileId = co_InvalidFileId;
  i_Id = 0;
  }

//---------------------------------------------------------------------------

ct_PrSemaphore::ct_PrSemaphore
(
const char * pc_key
): ct_SharedResource (pc_key)
  {
  o_FileId = co_InvalidFileId;
  i_Id = 0;
  }

//---------------------------------------------------------------------------

ct_PrSemaphore::ct_PrSemaphore
(
const char * pc_key,
unsigned u_idx
): ct_SharedResource (pc_key, u_idx)
  {
  o_FileId = co_InvalidFileId;
  i_Id = 0;
  }

//---------------------------------------------------------------------------

ct_PrSemaphore::~ct_PrSemaphore ()
  {
  if (b_InitSuccess)
    Close ();
  }

//---------------------------------------------------------------------------

et_ResError ct_PrSemaphore::Open ()
  {
  return OpenSem ();
  }

//---------------------------------------------------------------------------

et_ResError ct_PrSemaphore::Create
(
t_Int32 i_initValue,
bool b_createNew
)
  {
  if (i_initValue < 0)
    return ec_ResInvalidValue;

  return CreateSem (i_initValue, b_createNew);
  }

//---------------------------------------------------------------------------

et_ResError ct_PrSemaphore::Close ()
  {
  return CloseSem ();
  }

//---------------------------------------------------------------------------

et_ResError ct_PrSemaphore::TryAcquire
(
bool & b_success,
t_UInt32 u_milliSec
)
  {
  b_success = false;

  if (! b_InitSuccess)
    return ec_ResUninitialized;

  if ((u_milliSec == 0) ?
      SemNowaitOp (i_Id, -1, /*b_undo*/ false, b_success) :
      SemTimedOp (i_Id, -1, /*b_undo*/ false, b_success, u_milliSec))
    return ec_ResOK;

  return ec_ResUnknownError;
  }

//---------------------------------------------------------------------------

et_ResError ct_PrSemaphore::Acquire ()
  {
  if (! b_InitSuccess)
    return ec_ResUninitialized;

  if (SemOp (i_Id, -1, /*b_undo*/ false))
    return ec_ResOK;

  return ec_ResUnknownError;
  }

//---------------------------------------------------------------------------

et_ResError ct_PrSemaphore::Release ()
  {
  if (! b_InitSuccess)
    return ec_ResUninitialized;

  if (SemOp (i_Id, 1, /*b_undo*/ false))
    return ec_ResOK;

  return ec_ResUnknownError;
  }

//---------------------------------------------------------------------------
// Shared memory

ct_SharedMemory::ct_SharedMemory ()
  {
  co_Key. Assign ("Spirick_Tuning_SharedMemory");
  o_FileId = co_InvalidFileId;
  i_Id = 0;
  u_Size = 0;
  pv_Data = 0;
  }

//---------------------------------------------------------------------------

ct_SharedMemory::ct_SharedMemory
(
const char * pc_key
): ct_SharedResource (pc_key)
  {
  o_FileId = co_InvalidFileId;
  i_Id = 0;
  u_Size = 0;
  pv_Data = 0;
  }

//---------------------------------------------------------------------------

ct_SharedMemory::ct_SharedMemory
(
const char * pc_key,
unsigned u_idx
): ct_SharedResource (pc_key, u_idx)
  {
  o_FileId = co_InvalidFileId;
  i_Id = 0;
  u_Size = 0;
  pv_Data = 0;
  }

//---------------------------------------------------------------------------

ct_SharedMemory::~ct_SharedMemory ()
  {
  if (b_InitSuccess)
    Close ();
  }

//---------------------------------------------------------------------------

et_ResError ct_SharedMemory::Open
(
bool b_readOnly
)
  {
  if (b_InitSuccess)
    return ec_ResAlreadyInitialized;

  u_Size = 0;
  pv_Data = 0;
  int i_key;
  et_ResError eo_ret = OpenKey (i_key);

  if (eo_ret != ec_ResOK)
    return eo_ret;

  i_Id = shmget (i_key, 0, b_readOnly ? 0444 : 0666);

  if (i_Id == -1)
    {
    i_Id = 0;
    return ec_ResAccessDenied;
    }

  pv_Data = shmat (i_Id, 0, b_readOnly ? SHM_RDONLY : 0);

  if (pv_Data == (void *) -1)
    {
    pv_Data = 0;
    i_Id = 0;
    return ec_ResMemMapFailed;
    }

  shmid_ds so_shmBuffer;

  if (shmctl (i_Id, IPC_STAT, & so_shmBuffer) == -1)
    {
    pv_Data = 0;
    i_Id = 0;
    return ec_ResQuerySizeFailed;
    }

  u_Size = so_shmBuffer. shm_segsz;
  b_InitSuccess = true;
  return ec_ResOK;
  }

//---------------------------------------------------------------------------

et_ResError ct_SharedMemory::Create
(
t_UInt u_size,
bool b_createNew
)
  {
  if (b_InitSuccess)
    return ec_ResAlreadyInitialized;

  u_Size = 0;
  pv_Data = 0;
  int i_key;
  et_ResError eo_ret = CreateKey (i_key, b_createNew);

  if (eo_ret != ec_ResOK)
    return eo_ret;

  int i_flags = 0666 | IPC_CREAT;

  if (b_createNew)
    i_flags |= IPC_EXCL;

  i_Id = shmget (i_key, u_size, i_flags);

  if (i_Id == -1)
    {
    CloseDeleteFile ();
    i_Id = 0;
    return ec_ResAccessDenied;
    }

  pv_Data = shmat (i_Id, 0, 0);

  if (pv_Data == (void *) -1)
    {
    pv_Data = 0;
    CloseDeleteFile ();
    i_Id = 0;
    return ec_ResMemMapFailed;
    }

  u_Size = u_size;
  b_InitSuccess = true;
  return ec_ResOK;
  }

//---------------------------------------------------------------------------

et_ResError ct_SharedMemory::Close ()
  {
  if (! b_InitSuccess)
    return ec_ResUninitialized;

  b_InitSuccess = false;
  u_Size = 0;

  if (shmdt (pv_Data) == -1)
    {
    pv_Data = 0;

    if (o_FileId != co_InvalidFileId)
      {
      CloseDeleteFile ();
      shmctl (i_Id, IPC_RMID, 0);
      }

    i_Id = 0;
    return ec_ResUnmapFailed;
    }

  pv_Data = 0;

  if (o_FileId != co_InvalidFileId)
    {
    CloseDeleteFile ();

    if (shmctl (i_Id, IPC_RMID, 0) == -1)
      {
      i_Id = 0;
      return ec_ResUnknownError;
      }
    }

  i_Id = 0;
  return ec_ResOK;
  }

//---------------------------------------------------------------------------

t_UInt ct_SharedMemory::GetSize () const
  {
  return u_Size;
  }

//---------------------------------------------------------------------------

void * ct_SharedMemory::GetData () const
  {
  return pv_Data;
  }

#endif
