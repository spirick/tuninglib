
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
// File tuning/nulldatablock.h

#ifndef TUNING_NULLDATABLOCK_H
#define TUNING_NULLDATABLOCK_H

#include "tuning/defs.hpp"

//---------------------------------------------------------------------------

template <class t_block, class t_null>
  class TL_TM_EXPORT gct_NullDataBlock: public t_block
    {
  private:
    using TL_TMU_TYPENAME t_block::GetByteSize;
    using TL_TMU_TYPENAME t_block::SetByteSize;
    using TL_TMU_TYPENAME t_block::GetAddr;

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
    t_Size o_byteSize = t_block::GetByteSize ();

    if (o_byteSize != 0)
      return o_byteSize;
    else
      return sizeof (t_null);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_null>
  inline void gct_NullDataBlock <t_block, t_null>::SetByteSize
  (
  t_Size o_newSize
  )
    {
    if (o_newSize > (t_Size) sizeof (t_null))
      {
      void * pv_old = t_block::GetAddr ();
      t_block::SetByteSize (o_newSize);

      if (pv_old == 0)
        * (t_null *) t_block::GetAddr () = (t_null) 0;
      }
    else
      t_block::SetByteSize (0);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_null>
  inline void * gct_NullDataBlock <t_block, t_null>::GetAddr () const
    {
    void * pv = t_block::GetAddr ();

    if (pv != 0)
      return pv;
    else
      return & o_NullData;
    }

#endif
