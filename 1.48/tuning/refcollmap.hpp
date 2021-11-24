
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
// File tuning/refcollmap.hpp

#ifndef TUNING_REFCOLLMAP_HPP
#define TUNING_REFCOLLMAP_HPP

#include "tuning/refcollection.hpp"
#include "tuning/collmap.hpp"

//---------------------------------------------------------------------------

#define REFCOLLMAP_DCL(CollName)                                              \
    COLLMAP_DCL_ (CollName, ct_RefCollection)                                 \
    virtual void         IncRef (t_Position o_pos);                           \
    virtual void         DecRef (t_Position o_pos);                           \
    virtual t_RefCount   GetRef (t_Position o_pos) const;                     \
    virtual bool         IsAlloc (t_Position o_pos) const;                    \
    virtual bool         IsFree (t_Position o_pos) const;                     \
    };


#define REFCOLLMAP_DEF(CollName, ContName)                                    \
  COLLMAP_DEF_ (CollName, ContName, ct_RefCollection)                         \
  void ct_ ## CollName::IncRef (t_Position o_pos)                             \
    {                                                                         \
    pco_Cont-> IncRef ((t_Cont::t_Position) o_pos);                           \
    }                                                                         \
  void ct_ ## CollName::DecRef (t_Position o_pos)                             \
    {                                                                         \
    pco_Cont-> DecRef ((t_Cont::t_Position) o_pos);                           \
    }                                                                         \
  t_RefCount ct_ ## CollName::GetRef (t_Position o_pos) const                 \
    {                                                                         \
    return pco_Cont-> GetRef ((t_Cont::t_Position) o_pos);                    \
    }                                                                         \
  bool ct_ ## CollName::IsAlloc (t_Position o_pos) const                      \
    {                                                                         \
    return pco_Cont-> IsAlloc ((t_Cont::t_Position) o_pos);                   \
    }                                                                         \
  bool ct_ ## CollName::IsFree (t_Position o_pos) const                       \
    {                                                                         \
    return pco_Cont-> IsFree ((t_Cont::t_Position) o_pos);                    \
    }

#endif
