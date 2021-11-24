
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
// File samples/texec.cpp

#include <stdio.h>
#include "tuning/sys/cprocess.hpp"

//---------------------------------------------------------------------------

int main
(
int argc,
char * * argv
)
  {
  if (argc == 1)
    {
    printf ("Parent: Pid %d\n", tl_ProcessId ());
    const char * apc_params [5] =
      {
      "1",
      0,
      "",
      "4 a",
      "\"5 b\""
      };
    int i_child = tl_Exec (argv [0], 5, apc_params);
    printf ("Parent: Child Pid %d\n", i_child);

    while (tl_IsProcessRunning (i_child))
      tl_Delay (1);

    printf ("Parent: continue...\n");
    int i_childExit = tl_Exec (argv [0], 5, apc_params, true);
    printf ("Parent: Child Exit code %d\n", i_childExit);
    return 0;
    }
  else
    {
    printf ("Child: Pid %d\n", tl_ProcessId ());
    printf ("Child: wait...\n");
    tl_Delay (500);

    for (int i = 0; i < argc; i ++)
      printf ("Child: Param %d <%s>\n", i, argv [i]);

    return 7;
    }
  }
