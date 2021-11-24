
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
// File samples/tall.cpp

#include <stdio.h>
#include <stdlib.h>
#include "tuning/file.hpp"
#include "tuning/chn/array.h"
#include "tuning/sys/cprocess.hpp"

ct_String co_Exe;

//---------------------------------------------------------------------------

void SplitString
(
const ct_String & co_str,
char c_delim,
gct_Chn_Array <ct_String> & co_array
)
  {
  if (! co_str. IsEmpty ())
    {
    const char * pc_text = co_str. GetStr ();
    const char * pc_delim = pc_text - 1;
    t_UInt u_len = co_str. GetLen ();

    do
      {
      u_len  -= (pc_delim - pc_text) + 1;
      pc_text = pc_delim + 1;
      pc_delim = (const char *) tl_FirstChar (pc_text, u_len, c_delim);

      if (pc_delim == 0)
        pc_delim = pc_text + u_len;

      ct_String co_text (pc_text, pc_delim - pc_text);

      // while ((co_text. GetLen () > 0) && (((unsigned char) co_text. GetChar (0)) <= 32))
      //   co_text. Delete (0, 1);

      while ((co_text. GetLen () > 0) && (((unsigned char) co_text. GetRevChar (0)) <= 32))
        co_text. DeleteRev (1);

      // if (co_text. GetLen () > 0) // Add non-empty lines only
      co_array. AddObj (& co_text);
      }
    while (* pc_delim == c_delim);
    }
  }

//---------------------------------------------------------------------------

void Check (const char * pc_exe, const char * pc_ref)
  {
  ct_File co_outFile;
  co_outFile. AssignAsPath (tl_GetTempPath ());
  co_outFile. SetNameExt ("test_output");

  ct_FileName co_exeName;
  co_exeName. AssignAsName (co_Exe);
  co_exeName. SetNameExt (pc_exe);

  ct_String co_cmd = co_exeName + " > " + co_outFile;
  printf ("%s\n", co_exeName ());
  int i_ret = system (co_cmd ());

  if (i_ret != 0)
    {
    printf ("* * * Error code: %d\n", i_ret);
    return;
    }

  ct_File co_refFile;
  co_refFile. AssignAsName (co_Exe);
  co_refFile. AppendPath ("../../../samples/ref");
  co_refFile. CompressPath ();
  co_refFile. SetNameExt (pc_ref);
  ct_String co_outString;

  if (! co_outFile. Load (& co_outString))
    {
    printf ("* * * Can't load file: %s\n", co_outFile ());
    return;
    }

  co_outFile. Delete ();
  ct_String co_refString;

  if (! co_refFile. Load (& co_refString))
    {
    printf ("* * * Can't load file: %s\n", co_refFile ());
    return;
    }

  gct_Chn_Array <ct_String> co_outArray;
  SplitString (co_outString, '\n', co_outArray);
  gct_Chn_Array <ct_String> co_refArray;
  SplitString (co_refString, '\n', co_refArray);
  unsigned u_line = 1;

  while (u_line <= co_refArray. GetLen ())
    {
    if (u_line <= co_outArray. GetLen ())
      {
      if (* co_refArray. GetNthObj (u_line) != * co_outArray. GetNthObj (u_line))
        {
        printf ("  <<<<< Ref %d\n  %s\n", u_line, co_refArray. GetNthObj (u_line)-> GetStr ());
        printf ("  >>>>> Out %d\n  %s\n", u_line, co_outArray. GetNthObj (u_line)-> GetStr ());
        }
      }
    else
      printf ("  <<<<< Missing Ref %d\n  %s\n", u_line, co_refArray. GetNthObj (u_line)-> GetStr ());

    u_line ++;
    }

  while (u_line <= co_outArray. GetLen ())
    {
    printf ("  >>>>> Extra Out %d\n  %s\n", u_line, co_outArray. GetNthObj (u_line)-> GetStr ());
    u_line ++;
    }
  }

//---------------------------------------------------------------------------

int main
(
int /*argc*/,
char * * argv
)
  {
  co_Exe = argv [0];

  #ifdef TL_64
  Check ("talign", "talign64");
  Check ("tblock", "tblock64");
  Check ("tblockstore", "tblockstore64");
  Check ("tchain", "tchain64");
  #else
  Check ("talign", "talign32");
  Check ("tblock", "tblock32");
  Check ("tblockstore", "tblockstore32");
  Check ("tchain", "tchain32");
  #endif

  Check ("tcollection", "tcollection");
  Check ("tcontainer", "tcontainer");
  Check ("tdir", "tdir");
  Check ("tdirscan", "tdirscan");
  Check ("texception", "texception");
  Check ("tfile", "tfile");

  #if defined TL_WINDOWS
  Check ("tfilename", "tfilename_win");
  #else
  Check ("tfilename", "tfilename_lin");
  #endif

  Check ("tmap", "tmap");
  Check ("tptrmap", "tptrmap");
  Check ("tsemaphore", "tsemaphore");
  Check ("tstring", "tstring");

  #if defined TL_WINDOWS
  Check ("tthread", "tthread_win");
  #else
  Check ("tthread", "tthread_lin");
  #endif

  Check ("tutf", "tutf");
  return 0;
  }
