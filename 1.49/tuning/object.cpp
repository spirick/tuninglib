
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
// File tuning/object.cpp

#include "tuning/object.hpp"

//---------------------------------------------------------------------------

bool ct_Object::IsKindOf
(
t_ClassId o_id
) const
  {
  return ClassId () == o_id;
  }

//---------------------------------------------------------------------------

bool ct_Object::operator <
(
const ct_Object & co_comp
) const
  {
  return GetHash () < co_comp. GetHash ();
  }

//---------------------------------------------------------------------------

t_UInt ct_Object::GetHash () const
  {
  return (t_UInt) this;
  }

//---------------------------------------------------------------------------

#include "tuning/collection.hpp"

TL_ISKINDOF (ct_Collection, ct_Object)

//---------------------------------------------------------------------------

#include "tuning/refcollection.hpp"

TL_ISKINDOF (ct_RefCollection, ct_Collection)
