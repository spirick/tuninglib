
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
// File tuning/fixblock.h

#ifndef TUNING_FIXBLOCK_H
#define TUNING_FIXBLOCK_H

#include "tuning/sys/cmemory.hpp"

//---------------------------------------------------------------------------

template <class t_size, t_UInt u_fixSize>
  class TL_TM_EXPORT gct_FixBlock
    {
  public:
    typedef t_size       t_Size;

  protected:
    t_Size               o_Size;
    char                 ac_Block [u_fixSize];

  public:
    inline               gct_FixBlock ();
    inline               gct_FixBlock (const gct_FixBlock & co_init);
    inline gct_FixBlock & operator = (const gct_FixBlock & co_asgn);
    void                 Swap (gct_FixBlock & co_swap);

    static inline t_UInt GetMaxByteSize ();
    inline t_Size        GetByteSize () const;
    inline void          SetByteSize (t_Size o_newSize);
    inline void *        GetAddr () const;
    };

//---------------------------------------------------------------------------

template <class t_size, t_UInt u_fixSize>
  inline gct_FixBlock <t_size, u_fixSize>::gct_FixBlock ()
    {
    o_Size = 0;
    }

//---------------------------------------------------------------------------

template <class t_size, t_UInt u_fixSize>
  inline gct_FixBlock <t_size, u_fixSize>::gct_FixBlock
  (
  const gct_FixBlock & co_init
  )
    {
    o_Size = co_init. o_Size;

    TL_ASSERT (o_Size <= u_fixSize);

    tl_CopyMemory ((char *) GetAddr (), (char *) co_init. GetAddr (), o_Size);
    }

//---------------------------------------------------------------------------

template <class t_size, t_UInt u_fixSize>
  inline gct_FixBlock <t_size, u_fixSize> &
  gct_FixBlock <t_size, u_fixSize>::operator =
  (
  const gct_FixBlock & co_asgn
  )
    {
    if (& co_asgn != this)
      {
      SetByteSize (co_asgn. o_Size);
      tl_CopyMemory ((char *) GetAddr (), (char *) co_asgn. GetAddr (), o_Size);
      }

    return * this;
    }

//---------------------------------------------------------------------------

template <class t_size, t_UInt u_fixSize>
  void gct_FixBlock <t_size, u_fixSize>::Swap
  (
  gct_FixBlock & co_swap
  )
    {
    if (& co_swap != this)
      {
      char ac_local [u_fixSize];
      tl_CopyMemory (ac_local,          co_swap. ac_Block, co_swap. o_Size);
      tl_CopyMemory (co_swap. ac_Block, ac_Block,          o_Size);
      tl_CopyMemory (ac_Block,          ac_local,          co_swap. o_Size);
      tl_SwapObj (o_Size, co_swap. o_Size);
      }
    }

//---------------------------------------------------------------------------

template <class t_size, t_UInt u_fixSize>
  inline t_UInt gct_FixBlock <t_size, u_fixSize>::GetMaxByteSize ()
    {
    return u_fixSize;
    }

//---------------------------------------------------------------------------

template <class t_size, t_UInt u_fixSize>
  inline TL_TYPENAME gct_FixBlock <t_size, u_fixSize>::t_Size
  gct_FixBlock <t_size, u_fixSize>::GetByteSize () const
    {
    return o_Size;
    }

//---------------------------------------------------------------------------

template <class t_size, t_UInt u_fixSize>
  inline void gct_FixBlock <t_size, u_fixSize>::SetByteSize
  (
  t_Size o_newSize
  )
    {
    o_Size = o_newSize;

    TL_ASSERT (o_Size <= u_fixSize);
    }

//---------------------------------------------------------------------------

template <class t_size, t_UInt u_fixSize>
  inline void * gct_FixBlock <t_size, u_fixSize>::GetAddr () const
    {
    return o_Size == 0 ? 0 : (void *) ac_Block;
    }

#endif
