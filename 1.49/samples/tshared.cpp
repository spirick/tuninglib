
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
// File samples/tshared.cpp

#include <stdio.h>
#include <stdlib.h>
#include "tuning/sys/cprocess.hpp"
#include "tuning/sys/cstring.hpp"
#include "tuning/sys/cmemory.hpp"

//---------------------------------------------------------------------------

int main
(
int argc,
char * * argv
)
  {
  if (argc == 1)
    {
    printf ("Begin main\n");
    ct_PrMutex co_mutex ("tshared_mutex");

    if (co_mutex. Create () != ec_ResOK)
      {
      printf (" * * * Create mutex failed\n");
      return 1;
      }

    ct_PrSemaphore co_semaphore ("tshared_semaphore");

    if (co_semaphore. Create () != ec_ResOK)
      {
      printf (" * * * Create semaphore failed\n");
      return 1;
      }

    const char * apc_params [1] = { "0" };
    int i_pid = tl_Exec (argv [0], 1, apc_params);
    printf ("Child pid: %d  Running: %d\n", i_pid, tl_IsProcessRunning (i_pid));
    tl_Delay (450);
    co_mutex. Lock ();
    printf ("Main waites in critical section\n");
    tl_Delay (500);
    printf ("Main leaves critical section\n");
    co_mutex. Unlock ();
    tl_Delay (1000);
    printf ("Child pid: %d  Running: %d\n", i_pid, tl_IsProcessRunning (i_pid));

    ct_SharedMemory co_shm ("tshared_shm");

    if (co_shm. Create (10 * 0x1000) != ec_ResOK)
      {
      printf (" * * * Create shm failed\n");
      return 1;
      }

    const char * pc_msg = "<shm test>";
    tl_CopyMemory ((char *) co_shm. GetData (), pc_msg, tl_StringLength (pc_msg) + 1);
    apc_params [0] = "1";
    i_pid = tl_Exec (argv [0], 1, apc_params);
    printf ("Child pid: %d  Running: %d\n", i_pid, tl_IsProcessRunning (i_pid));
    tl_Delay (500);
    printf ("Child pid: %d  Running: %d\n", i_pid, tl_IsProcessRunning (i_pid));

    if (co_shm. Close () != ec_ResOK)
      {
      printf (" * * * Close shm failed\n");
      return 1;
      }

    if (co_semaphore. Close () != ec_ResOK)
      {
      printf (" * * * Close semaphore failed\n");
      return 1;
      }

    if (co_mutex. Close () != ec_ResOK)
      {
      printf (" * * * Close mutex failed\n");
      return 1;
      }

    printf ("End main\n");
    }
  else
    {
    int i_param = atoi (argv [1]);
    printf ("Begin main %d  Pid: %d\n", i_param, tl_ProcessId ());
    ct_PrMutex co_mutex ("tshared_mutex");

    if (co_mutex. Create (true) == ec_ResOK)
      {
      printf (" * * * Create mutex twice\n");
      return 1;
      }

    if (co_mutex. Open () != ec_ResOK)
      {
      printf (" * * * Open mutex failed\n");
      return 1;
      }

    ct_PrSemaphore co_semaphore ("tshared_semaphore");

    if (co_semaphore. Create (1, true) == ec_ResOK)
      {
      printf (" * * * Create semaphore twice\n");
      return 1;
      }

    if (co_semaphore. Open () != ec_ResOK)
      {
      printf (" * * * Open semaphore failed\n");
      return 1;
      }

    if (i_param == 0)
      {
      for (int i = 0; i < 10; i ++)
        {
        co_mutex. Lock ();
        printf ("Main %d in critical section (%d)\n", i_param, i);
        co_mutex. Unlock ();
        tl_Delay (100);
        }
      }

    if (i_param == 1)
      {
      ct_SharedMemory co_shm ("tshared_shm");

      if (co_shm. Create (10 * 0x1000, true) == ec_ResOK)
        {
        printf (" * * * Create shm twice\n");
        return 1;
        }

      //if (co_shm. Open (true) != ec_ResOK)
      if (co_shm. Open (false) != ec_ResOK)
        {
        printf (" * * * Open shm failed\n");
        return 1;
        }

      printf ("Main %d shm size: %x\n", i_param, (unsigned) co_shm. GetSize ());
      printf ("Main %d shm text: %s\n", i_param, (const char *) co_shm. GetData ());
      * (char *) co_shm. GetData () = '$';

      if (co_shm. Close () != ec_ResOK)
        {
        printf (" * * * Close shm failed\n");
        return 1;
        }
      }

    if (co_semaphore. Close () != ec_ResOK)
      {
      printf (" * * * Close semaphore failed\n");
      return 1;
      }

    if (co_mutex. Close () != ec_ResOK)
      {
      printf (" * * * Close mutex failed\n");
      return 1;
      }

    printf ("End main %d\n", i_param);
    }

  return 0;
  }
