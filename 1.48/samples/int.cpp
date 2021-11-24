
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
// File samples/int.cpp

#include "samples/int.hpp"
#include <stdio.h>

//---------------------------------------------------------------------------

ct_Int::ct_Int ()
  {
  pi_Value = new int (0);
  printf ("ct_Int ()\n");
  }

//---------------------------------------------------------------------------

ct_Int::ct_Int
(
int i
)
  {
  pi_Value = new int (i);
  printf ("ct_Int (int i = %d)\n", * pi_Value);
  }

//---------------------------------------------------------------------------

ct_Int::ct_Int
(
const ct_Int & co_init
)
  {
  pi_Value = new int (* co_init. pi_Value);
  printf ("ct_Int (const ct_Int & co_init = %d)\n", * pi_Value);
  }

//---------------------------------------------------------------------------

ct_Int::~ct_Int ()
  {
  printf ("~ct_Int (%d)\n", * pi_Value);
  delete pi_Value;
  }

//---------------------------------------------------------------------------

TL_ISKINDOF (ct_Int, ct_Object)

//---------------------------------------------------------------------------

bool ct_Int::operator <
(
const ct_Object & co_comp
) const
  {
  if (co_comp. IsKindOf (ClassId ()))
    return * this < (ct_Int &) co_comp;
  else
    return ct_Object::operator < (co_comp);
  }

//---------------------------------------------------------------------------

t_UInt ct_Int::GetHash () const
  {
  return * pi_Value;
  }

//---------------------------------------------------------------------------

ct_Int & ct_Int::operator =
(
int i
)
  {
  * pi_Value = i;
  printf ("operator = (int i = %d)\n", * pi_Value);
  return * this;
  }

//---------------------------------------------------------------------------

ct_Int & ct_Int::operator =
(
const ct_Int & co_asgn
)
  {
  * pi_Value = * co_asgn. pi_Value;
  printf ("operator = (const ct_Int & co_asgn = %d)\n", * pi_Value);
  return * this;
  }

//---------------------------------------------------------------------------

bool ct_Int::operator ==
(
const ct_Int & co_comp
) const
  {
  return * pi_Value == * co_comp. pi_Value;
  }

//---------------------------------------------------------------------------

bool ct_Int::operator <
(
const ct_Int & co_comp
) const
  {
  return * pi_Value < * co_comp. pi_Value;
  }

//---------------------------------------------------------------------------

int ct_Int::GetValue () const
  {
  return * pi_Value;
  }
