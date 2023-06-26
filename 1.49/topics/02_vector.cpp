
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
// File topics/02_vector.cpp

#include <iostream>
#include <vector>
#include "tuning/sys/cinfo.hpp"

using namespace std;

struct MyPoint3D
  {
  double X;
  double Y;
  double Z;

  MyPoint3D () { X = Y = Z = 0.0; }
  MyPoint3D (double x, double y, double z) { X = x; Y = y; Z = z; }
  };

int main ()
  {
  #if defined TL_DEBUG

  cout << "Error: Don't use debug mode." << endl;
  return 1;

  #else

  st_CompilerInfo so_compInfo;

  if (tl_QueryCompilerInfo (& so_compInfo))
    {
    cout << "Compiler        : " << (so_compInfo. eo_Compiler == ec_CompilerMSVC ? "MSVC" : "GCC") << endl;
    cout << "Compiler version: " << so_compInfo. pc_CompilerVersion << endl;
    cout << "Runtime  version: " << so_compInfo. pc_RuntimeVersion << endl << endl;
    }

  cout << "vector" << endl;
  MyPoint3D p1 (1, 1, 1);
  vector <MyPoint3D> vec3d;
  cout << "  SizeOf: " << sizeof (vec3d) << endl;
  cout << "  Size: " << vec3d. size () << " Cap: " << vec3d. capacity () << endl;
  size_t u_cap = vec3d. capacity ();
  cout << "  Cap inc: ";

  for (unsigned u = 0; u < 1000; u ++)
    {
    vec3d. push_back (p1);

    if (vec3d. capacity () != u_cap)
      {
      u_cap = vec3d. capacity ();
      cout << " " << u_cap;
      }
    }

  cout << endl;
  cout << "  Cap dec: ";

  for (unsigned u = 0; u < 1000; u ++)
    {
    vec3d. pop_back ();

    if (vec3d. capacity () != u_cap)
      {
      u_cap = vec3d. capacity ();
      cout << " " << u_cap;
      }
    }

  cout << endl;
  cout << "  Size: " << vec3d. size () << " Cap: " << vec3d. capacity () << endl;
  vec3d. clear ();
  cout << "  Clear, Size: " << vec3d. size () << " Cap: " << vec3d. capacity () << endl;
  vec3d. shrink_to_fit ();
  cout << "  Shrink, Size: " << vec3d. size () << " Cap: " << vec3d. capacity () << endl << endl;

  cout << "string" << endl;
  string tstr;
  cout << "  SizeOf: " << sizeof (tstr) << endl;
  cout << "  Size: " << tstr. size () << " Cap: " << tstr. capacity () << endl;
  u_cap = tstr. capacity ();
  cout << "  Cap inc: ";

  for (unsigned u = 0; u < 1000; u ++)
    {
    tstr. push_back ('a');

    if (tstr. capacity () != u_cap)
      {
      u_cap = tstr. capacity ();
      cout << " " << u_cap;
      }
    }

  cout << endl;
  cout << "  Cap dec: ";

  for (unsigned u = 0; u < 1000; u ++)
    {
    tstr. pop_back ();

    if (tstr. capacity () != u_cap)
      {
      u_cap = tstr. capacity ();
      cout << " " << u_cap;
      }
    }

  cout << endl;
  cout << "  Size: " << tstr. size () << " Cap: " << tstr. capacity () << endl;
  tstr. clear ();
  cout << "  Clear, Size: " << tstr. size () << " Cap: " << tstr. capacity () << endl;
  tstr. shrink_to_fit ();
  cout << "  Shrink, Size: " << tstr. size () << " Cap: " << tstr. capacity () << endl;

  return 0;

  #endif
  }
