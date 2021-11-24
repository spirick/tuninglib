
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
// File tuning/nulldatablock.h

#ifndef TUNING_NULLDATABLOCK_H
#define TUNING_NULLDATABLOCK_H

#include "tuning/defs.hpp"

//---------------------------------------------------------------------------

template <class t_block, class t_null>
  class TL_TM_EXPORT gct_NullDataBlock: public t_block
    {
  public:
    typedef TL_TYPENAME t_block::t_Size t_Size;

  private:
    static t_null        o_NullData;

  public:
    inline t_Size        GetByteSize () const;
    inline void          SetByteSize (t_Size o_newSize);
    inline void *        GetAddr () const;
    };

//---------------------------------------------------------------------------

template <class t_block, class t_null>
  t_null gct_NullDataBlock <t_block, t_null>::o_NullData;

//---------------------------------------------------------------------------

template <class t_block, class t_null>
  inline TL_TYPENAME gct_NullDataBlock <t_block, t_null>::t_Size
  gct_NullDataBlock <t_block, t_null>::GetByteSize () const
    {
    if (t_block::GetByteSize () == 0)
      return sizeof (t_null);
    else
      return t_block::GetByteSize ();
    }

//---------------------------------------------------------------------------

template <class t_block, class t_null>
  inline void gct_NullDataBlock <t_block, t_null>::SetByteSize
  (
  t_Size o_newSize
  )
    {
    if (o_newSize <= (t_Size) sizeof (t_null))
      t_block::SetByteSize (0);
    else
      {
      t_Size o_oldSize = t_block::GetByteSize ();
      t_block::SetByteSize (o_newSize);

      if (o_oldSize == 0)
        * (t_null *) t_block::GetAddr () = (t_null) 0;
      }
    }

//---------------------------------------------------------------------------

template <class t_block, class t_null>
  inline void * gct_NullDataBlock <t_block, t_null>::GetAddr () const
    {
    if (t_block::GetByteSize () == 0)
      return & o_NullData;
    else
      return t_block::GetAddr ();
    }

#endif
