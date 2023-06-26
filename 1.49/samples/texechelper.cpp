
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
// File samples/texechelper.cpp

#include <stdio.h>
#include "samples/exechelper.hpp"

//---------------------------------------------------------------------------

int main
(
int argc,
char * * argv
)
  {
  if (argc == 1)
    {
    ct_ExecHelper co_execHelper;

    #if 0

    et_ResError eo_ret = co_execHelper. StartIntern ();

    #else

    ct_FileName co_execName (argv [0]);
    co_execName. SetName ("exechelpermain");
    et_ResError eo_ret = co_execHelper. StartExtern (co_execName. GetAllStr ());

    #endif

    if (eo_ret != ec_ResOK)
      {
      printf ("Start ExecHelper failed\n");
      return 1;
      }

    printf ("Parent: Pid %d\n", tl_ProcessId ());
    const char * apc_params [5] =
      {
      "1",
      0,
      "",
      "4 a",
      "\"5 b\""
      };
    int i_child = co_execHelper. Exec (argv [0], 5, apc_params);
    printf ("Parent: Child Pid %d\n", i_child);

    while (tl_IsProcessRunning (i_child))
      tl_Delay (1);

    printf ("Parent: continue...\n");
    int i_childExit = co_execHelper. Exec (argv [0], 5, apc_params, true);
    printf ("Parent: Child Exit code %d\n", i_childExit);
    eo_ret = co_execHelper. Stop ();

    if (eo_ret != ec_ResOK)
      {
      printf ("Stop ExecHelper failed\n");
      return 1;
      }

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
