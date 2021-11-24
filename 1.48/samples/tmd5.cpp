
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
// File samples/tmd5.cpp

#include <stdio.h>
#include "tuning/md5.hpp"
#include "tuning/file.hpp"

//---------------------------------------------------------------------------

const t_FileSize cu_BfSize = 4096;

int main
(
int argc,
char * * argv
)
  {
  if (argc == 1)
    {
    printf ("Usage: tmd5 filename\n");
    return 1;
    }

  ct_File co_file (argv [1]);

  if (! co_file. Open ())
    {
    printf ("Can't open file\n");
    return 1;
    }

  t_FileSize o_size;

  if (! co_file. QuerySize (o_size))
    {
    printf ("Can't query size\n");
    return 1;
    }

  char ac_buffer [cu_BfSize];
  ct_MD5 co_md5;

  while (o_size != 0)
    {
    t_FileSize o_read = tl_Min (cu_BfSize, o_size);

    if (! co_file. Read (ac_buffer, o_read))
      {
      printf ("Can't read data\n");
      return 1;
      }

    co_md5. Update (ac_buffer, (t_UInt) o_read);
    o_size -= o_read;
    }

  if (! co_file. Close ())
    {
    printf ("Can't close file\n");
    return 1;
    }

  co_md5. Finalize ();
  printf ("%s\n", co_md5. GetResultStr ());
  ct_MD5 co_md5_2 (co_md5. GetResult ());
  printf ("%s %d\n", co_md5_2. GetResultStr (), co_md5 == co_md5_2);
  return 0;
  }
