
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
// File tuning/refcollection.hpp

#ifndef TUNING_REFCOLLECTION_HPP
#define TUNING_REFCOLLECTION_HPP

#include "tuning/collection.hpp"
#include "tuning/refcount.hpp"

//---------------------------------------------------------------------------

class TL_EXPORT ct_RefCollection: public ct_Collection
  {
public:
  TL_CLASSID (ct_RefCollection)

  virtual void         IncRef (t_Position o_pos) = 0;
  virtual void         DecRef (t_Position o_pos) = 0;
  virtual t_RefCount   GetRef (t_Position o_pos) const = 0;
  virtual bool         IsAlloc (t_Position o_pos) const = 0;
  virtual bool         IsFree (t_Position o_pos) const = 0;
  };

#endif
