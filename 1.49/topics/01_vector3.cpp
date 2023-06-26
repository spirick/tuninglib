
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
// File topics/01_vector3.cpp

#include <iostream>
#include <vector>

using namespace std;

class Chatterbox
  {
  int i;
public:
  Chatterbox ()
    { cout << "+ Chatterbox" << endl; i = 0; }
  Chatterbox (int init)
    { cout << "+ Chatterbox " << init << endl; i = init; }
  Chatterbox (const Chatterbox & cp)
    { cout << "+ Chatterbox cp " << cp. i << endl; i = cp. i; }
  Chatterbox (Chatterbox && mv) noexcept
    { cout << "+ Chatterbox mv " << mv. i << endl; i = mv. i; }
  ~Chatterbox ()
    { cout << "- Chatterbox " << i << endl; }
  Chatterbox & operator = (const Chatterbox & asgn)
    { cout << ". Chatterbox = " << asgn. i << endl; i = asgn. i; return * this; }
  Chatterbox & operator = (Chatterbox && mv) noexcept
    { cout << ". Chatterbox mv = " << mv. i << endl; i = mv. i; return * this; }
  };

int main ()
  {
  cout << "Construct samples" << endl;
  Chatterbox cb1 (1);
  Chatterbox cb2 (2);
  Chatterbox cb3 (3);
  Chatterbox cb4 (4);
  Chatterbox cb5 (5);

  {
  vector <Chatterbox> cbvec;

  cout << "--------------" << endl;
  cout << "Append cb 3" << endl;
  cbvec. push_back (cb3);

  cout << "--------------" << endl;
  cout << "Insert cb 2" << endl;
  cbvec. insert (cbvec. begin (), cb2);

  cout << "--------------" << endl;
  cout << "Append cb 4" << endl;
  cbvec. push_back (cb4);

  cout << "--------------" << endl;
  cout << "Insert cb 1" << endl;
  cbvec. insert (cbvec. begin (), cb1);

  cout << "--------------" << endl;
  cout << "Append cb 5" << endl;
  cbvec. push_back (cb5);

  cout << "--------------" << endl;
  cout << "Remove first" << endl;
  cbvec. erase (cbvec. begin ());

  cout << "--------------" << endl;
  cout << "Remove last" << endl;
  cbvec. pop_back ();

  cout << "--------------" << endl;
  cout << "Insert cb 1" << endl;
  cbvec. insert (cbvec. begin (), cb1);

  cout << "--------------" << endl;
  cout << "Append cb 5" << endl;
  cbvec. push_back (cb5);

  cout << "--------------" << endl;
  cout << "Reserve" << endl;
  cbvec. reserve (10);

  cout << "--------------" << endl;
  cout << "Shrink" << endl;
  cbvec. shrink_to_fit ();

  cout << "--------------" << endl;
  cout << "Destruct vector" << endl;
  }

  cout << "--------------" << endl;
  cout << "Destruct samples" << endl;
  return 0;
  }
