
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
// File topics/04_vector.cpp

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include "tuning/chn/array.h"
#include "tuning/sys/cinfo.hpp"
#include "tuning/sys/ctimedate.hpp"

using namespace std;

struct MyPoint3D
  {
  double d_X;
  double d_Y;
  double d_Z;

  MyPoint3D () { d_X = d_Y = d_Z = 0.0; }
  MyPoint3D (double d_x, double d_y, double d_z) { d_X = d_x; d_Y = d_y; d_Z = d_z; }
  };

struct MyPointRef
  {
  MyPoint3D * P1;
  MyPoint3D * P2;
  MyPoint3D * P3;

  MyPointRef ()
    { P1 = P2 = P3 = 0; }
  MyPointRef (MyPoint3D * p1, MyPoint3D * p2, MyPoint3D * p3)
    { P1 = p1; P2 = p2; P3 = p3; }
  MyPointRef (const MyPointRef & cp)
    { P1 = cp. P1; P2 = cp. P2; P3 = cp. P3; }
  MyPointRef (MyPointRef && mv) noexcept
    { P1 = mv. P1; mv. P1 = 0; P2 = mv. P2;
      mv. P2 = 0; P3 = mv. P3; mv. P3 = 0; }
  ~MyPointRef ()
    { ; }
  MyPointRef & operator = (const MyPointRef & asgn)
    { P1 = asgn. P1; P2 = asgn. P2; P3 = asgn. P3; return * this; }
  MyPointRef & operator = (MyPointRef && mv) noexcept
    { P1 = mv. P1; mv. P1 = 0; P2 = mv. P2;
      mv. P2 = 0; P3 = mv. P3; mv. P3 = 0; return * this; }
  };

void PrintInfo (t_MicroTime i_time, const st_ProcessMemoryInfo & so_pmInfo)
  {
  cout << setw (5) << i_time / 1000 << " ms, "
       << "VM bytes: "  << setw (4) << so_pmInfo. u_VMBytes  / (1u << 20) << " mb, "
       << "RSS bytes: " << setw (4) << so_pmInfo. u_RSSBytes / (1u << 20) << " mb" << endl;
  }

#if defined TL_DEBUG

int main ()
  {
  cout << "Error: Don't use debug mode." << endl;
  return 1;
  }

#else

int main (int argc, char * * argv)
  {
  st_CompilerInfo so_compInfo;

  if (tl_QueryCompilerInfo (& so_compInfo))
    {
    cout << "Compiler        : " << (so_compInfo. eo_Compiler == ec_CompilerMSVC ? "MSVC" : "GCC") << endl;
    cout << "Compiler version: " << so_compInfo. pc_CompilerVersion << endl;
    cout << "Runtime  version: " << so_compInfo. pc_RuntimeVersion << endl << endl;
    }

  unsigned u_LoopSize = 10;
  unsigned u_FieldSize = 500000;
  unsigned u_TestSize = 500;

  if ((argc != 2) ||
      (strlen (argv [1]) != 1) ||
      ((argv [1] [0] != 'v') && (argv [1] [0] != 'a')))
    {
    cout << "Usage: 04_vector v | a" << endl;
    return 1;
    }

  MyPoint3D p3d [4];
  p3d [0] = MyPoint3D (1, 1, 1);
  p3d [1] = MyPoint3D (2, 2, 2);
  p3d [2] = MyPoint3D (3, 3, 3);
  p3d [3] = MyPoint3D (4, 4, 4);

  MyPointRef pr [4];
  pr [0] = MyPointRef (p3d + 0, p3d + 1, p3d + 2);
  pr [1] = MyPointRef (p3d + 1, p3d + 2, p3d + 3);
  pr [2] = MyPointRef (p3d + 2, p3d + 3, p3d + 0);
  pr [3] = MyPointRef (p3d + 3, p3d + 0, p3d + 1);

  t_MicroTime i_time0 = tl_QueryPrecisionTime ();
  t_MicroTime i_time1 = 0, i_time2 = 0, i_time3 = 0;
  st_ProcessMemoryInfo so_pmInfo1, so_pmInfo2, so_pmInfo3;

  if (argv [1] [0] == 'v')
    {
    vector <MyPointRef> * pvecpr = new vector <MyPointRef> [u_FieldSize];

    for (unsigned u_l = 0; u_l < u_LoopSize; u_l ++)
      {
      for (unsigned u_f = 0; u_f < u_FieldSize; u_f ++)
        {
        unsigned u_rnd = 61 * u_l + 29 * u_f;
        unsigned u_lim = u_TestSize / ((u_rnd % 71) + 1);
        unsigned u_cnt = u_TestSize / ((u_rnd % 311) + 1);

        for (unsigned u_t = 0; u_t < u_cnt; u_t ++)
          {
          if ((u_t & 1) == 0)
            pvecpr [u_f]. push_back (pr [(u_l + u_t) % 4]);
          else
            pvecpr [u_f]. insert (pvecpr [u_f]. begin (), pr [(u_l + u_t) % 4]);
          }

        if (pvecpr [u_f]. size () > u_lim)
          pvecpr [u_f]. clear ();

        // Save memory
        pvecpr [u_f]. shrink_to_fit ();
        }
      }

    i_time1 = tl_QueryPrecisionTime ();
    tl_QueryProcessMemoryInfo (& so_pmInfo1);

    for (unsigned u_f = 0; u_f < u_FieldSize; u_f ++)
      pvecpr [u_f]. shrink_to_fit ();

    tl_FreeUnused ();
    i_time2 = tl_QueryPrecisionTime ();
    tl_QueryProcessMemoryInfo (& so_pmInfo2);
    delete [] pvecpr;
    tl_FreeUnused ();
    }
  else
    {
    typedef gct_Chn_Array <MyPointRef> myArr;
    myArr * parrpr = new myArr [u_FieldSize];

    for (unsigned u_l = 0; u_l < u_LoopSize; u_l ++)
      {
      for (unsigned u_f = 0; u_f < u_FieldSize; u_f ++)
        {
        unsigned u_rnd = 61 * u_l + 29 * u_f;
        unsigned u_lim = u_TestSize / ((u_rnd % 71) + 1);
        unsigned u_cnt = u_TestSize / ((u_rnd % 311) + 1);

        for (unsigned u_t = 0; u_t < u_cnt; u_t ++)
          {
          if ((u_t & 1) == 0)
            parrpr [u_f]. AddObj (pr + ((u_l + u_t) % 4));
          else
            parrpr [u_f]. AddObjBeforeFirst (pr + ((u_l + u_t) % 4));
          }

        if (parrpr [u_f]. GetLen () > u_lim)
          parrpr [u_f]. DelAll ();
        }
      }

    i_time1 = tl_QueryPrecisionTime ();
    tl_QueryProcessMemoryInfo (& so_pmInfo1);
    GetChnStore ()-> FreeUnused ();
    i_time2 = tl_QueryPrecisionTime ();
    tl_QueryProcessMemoryInfo (& so_pmInfo2);
    delete [] parrpr;
    GetChnStore ()-> FreeUnused ();
    }

  i_time3 = tl_QueryPrecisionTime ();
  tl_QueryProcessMemoryInfo (& so_pmInfo3);

  if (argv [1] [0] == 'v')
    cout << "Vector" << endl;
  else
    cout << "gct_Array" << endl;

  PrintInfo (i_time1 - i_time0, so_pmInfo1);
  PrintInfo (i_time2 - i_time1, so_pmInfo2);
  PrintInfo (i_time3 - i_time2, so_pmInfo3);
  return 0;
  }

#endif
