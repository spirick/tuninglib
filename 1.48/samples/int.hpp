
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
// File samples/int.hpp

#ifndef TUNING_SAMPLES_INT_HPP
#define TUNING_SAMPLES_INT_HPP

#include "tuning/object.hpp"

//---------------------------------------------------------------------------

class TL_EXPORT ct_Int: public ct_Object
  {
  int *                pi_Value;

public:
                       ct_Int ();
                       ct_Int (int i);
                       ct_Int (const ct_Int & co_init);
                       ~ct_Int ();

  TL_CLASSID (ct_Int)

  virtual bool         operator < (const ct_Object & co_comp) const;
  virtual t_UInt       GetHash () const;

  ct_Int &             operator = (int i);
  ct_Int &             operator = (const ct_Int & co_asgn);
  bool                 operator == (const ct_Int & co_comp) const;
  bool                 operator < (const ct_Int & co_comp) const;
  int                  GetValue () const;
  };

#endif
