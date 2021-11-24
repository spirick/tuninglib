
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
// File tuning/object.hpp

#ifndef TUNING_OBJECT_HPP
#define TUNING_OBJECT_HPP

#include "tuning/defs.hpp"

//---------------------------------------------------------------------------

#define TL_CLASSID(t_class)                                                           \
  static t_ClassId     ClassId ()            { return (t_ClassId) t_class::ClassId; } \
  virtual t_ClassId    GetClassId () const   { return (t_ClassId) t_class::ClassId; } \
  virtual const char * GetClassName () const { return # t_class; }                    \
  virtual bool         IsKindOf (t_ClassId o_id) const;

#define TL_ISKINDOF(t_class, t_base)             \
  bool t_class::IsKindOf (t_ClassId o_id) const  \
    {                                            \
    if (ClassId () == o_id)                      \
      return true;                               \
    else                                         \
      return t_base::IsKindOf (o_id);            \
    }

//---------------------------------------------------------------------------

class TL_EXPORT ct_Object
  {
public:
  virtual              ~ct_Object () { }

  typedef void (* t_ClassId) ();
  TL_CLASSID (ct_Object)

  virtual bool         operator < (const ct_Object & co_comp) const;
  virtual t_UInt       GetHash () const;
  inline void          Swap (ct_Object &) { }
  };

//---------------------------------------------------------------------------

class TL_EXPORT ct_Empty
  {
public:
  inline void          Swap (ct_Empty &) { }
  };

#endif
