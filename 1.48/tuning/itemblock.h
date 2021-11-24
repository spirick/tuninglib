
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
// File tuning/itemblock.h

#ifndef TUNING_ITEMBLOCK_H
#define TUNING_ITEMBLOCK_H

#include "tuning/sys/cmemory.hpp"

//---------------------------------------------------------------------------

template <class t_block>
  class TL_TM_EXPORT gct_VarItemBlockBase: public t_block
    {
  public:
    typedef TL_TYPENAME t_block::t_Size t_Size;

  protected:
    t_Size               o_FixSize;

  public:
    inline               gct_VarItemBlockBase ();

    inline void          SetFixSize (t_Size o_fs);
    };

//---------------------------------------------------------------------------

template <class t_block>
  inline gct_VarItemBlockBase <t_block>::gct_VarItemBlockBase ()
    {
    o_FixSize = 0;
    }

//---------------------------------------------------------------------------

template <class t_block>
  inline void gct_VarItemBlockBase <t_block>::SetFixSize
  (
  t_Size o_fs
  )
    {
    TL_ASSERT (o_fs != 0);

    if (o_FixSize == 0)
      o_FixSize = o_fs;

    TL_ASSERT (o_FixSize == o_fs);
    }

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

template <class t_block, t_UInt o_fixSize>
  class TL_TM_EXPORT gct_FixItemBlockBase: public t_block
    {
  public:
    typedef TL_TYPENAME t_block::t_Size t_Size;

  protected:
    static const t_Size  o_FixSize = (t_Size) o_fixSize;

  public:
    inline void          SetFixSize (t_Size o_fs);
    };

//---------------------------------------------------------------------------

template <class t_block, t_UInt o_fixSize>
  inline void gct_FixItemBlockBase <t_block, o_fixSize>::SetFixSize
  (
  t_Size TL_DEBUG_CODE (o_fs)
  )
    {
    TL_ASSERT (o_fs == o_fixSize);
    }

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

template <class t_block>
  class TL_TM_EXPORT gct_ItemBlock: public t_block
    {
  public:
    typedef TL_TYPENAME t_block::t_Size t_Size;

  private:
    using TL_TMU_TYPENAME t_block::GetByteSize;
    using TL_TMU_TYPENAME t_block::SetByteSize;

    inline char *        GetRawAddr_ (t_Size o_pos) const;

  public:
    inline t_Size        GetFixSize () const;
    inline t_Size        GetMaxItemSize () const;
    inline t_Size        GetItemSize () const;
    inline void          SetItemSize (t_Size o_size);
    inline void          IncItemSize1 ();
    inline void          DecItemSize1 ();
    inline void          IncItemSize (t_Size o_inc);
    inline void          DecItemSize (t_Size o_dec);
    inline void *        GetItemAddr (t_Size o_pos) const;

    void *               InsertItems (t_Size o_pos, t_Size o_count);
    void *               DeleteItems (t_Size o_pos, t_Size o_count);

    inline t_Size        GetDefaultPageSize () const;
    inline void          AlignPageSize (t_Size o_fixSize, t_Size o_pageSize);
    };

//---------------------------------------------------------------------------

template <class t_block>
  inline char * gct_ItemBlock <t_block>::GetRawAddr_
  (
  t_Size o_pos
  ) const
    {
    return (char *) this-> GetAddr () + o_pos;
    }

//---------------------------------------------------------------------------

template <class t_block>
  inline TL_TYPENAME gct_ItemBlock <t_block>::t_Size
  gct_ItemBlock <t_block>::GetFixSize () const
    {
    return this-> o_FixSize;
    }

//---------------------------------------------------------------------------

template <class t_block>
  inline TL_TYPENAME gct_ItemBlock <t_block>::t_Size
  gct_ItemBlock <t_block>::GetMaxItemSize () const
    {
    return ((t_Size) this-> GetMaxByteSize ()) / this-> o_FixSize;
    }

//---------------------------------------------------------------------------

template <class t_block>
  inline TL_TYPENAME gct_ItemBlock <t_block>::t_Size
  gct_ItemBlock <t_block>::GetItemSize () const
    {
    t_Size o_size = this-> GetByteSize ();

    if (o_size == 0)
      return 0;
    else
      return (t_Size) (o_size / this-> o_FixSize);
    }

//---------------------------------------------------------------------------

template <class t_block>
  inline void gct_ItemBlock <t_block>::SetItemSize
  (
  t_Size o_size
  )
    {
    TL_ASSERT (o_size <= this-> GetMaxItemSize ());

    this-> SetByteSize ((t_Size) (o_size * this-> o_FixSize));
    }

//---------------------------------------------------------------------------

template <class t_block>
  inline void gct_ItemBlock <t_block>::IncItemSize1 ()
    {
    t_Size o_newSize = (t_Size) (this-> GetByteSize () + this-> o_FixSize);

    TL_ASSERT (o_newSize >= this-> o_FixSize); // Overflow

    this-> SetByteSize (o_newSize);
    }

//---------------------------------------------------------------------------

template <class t_block>
  inline void gct_ItemBlock <t_block>::DecItemSize1 ()
    {
    TL_ASSERT (this-> GetByteSize () >= this-> o_FixSize); // Underflow

    this-> SetByteSize ((t_Size) (this-> GetByteSize () - this-> o_FixSize));
    }

//---------------------------------------------------------------------------

template <class t_block>
  inline void gct_ItemBlock <t_block>::IncItemSize
  (
  t_Size o_inc
  )
    {
    TL_ASSERT (o_inc <= this-> GetMaxItemSize ());

    o_inc = (t_Size) (o_inc * this-> o_FixSize);
    t_Size o_newSize = (t_Size) (this-> GetByteSize () + o_inc);

    TL_ASSERT (o_newSize >= o_inc); // Overflow

    this-> SetByteSize (o_newSize);
    }

//---------------------------------------------------------------------------

template <class t_block>
  inline void gct_ItemBlock <t_block>::DecItemSize
  (
  t_Size o_dec
  )
    {
    TL_ASSERT (o_dec <= this-> GetMaxItemSize ());

    o_dec = (t_Size) (o_dec * this-> o_FixSize);

    TL_ASSERT (this-> GetByteSize () >= o_dec); // Underflow

    this-> SetByteSize ((t_Size) (this-> GetByteSize () - o_dec));
    }

//---------------------------------------------------------------------------

template <class t_block>
  inline void * gct_ItemBlock <t_block>::GetItemAddr
  (
  t_Size o_pos
  ) const
    {
    TL_ASSERT (o_pos <= this-> GetMaxItemSize ());

    o_pos = (t_Size) (o_pos * this-> o_FixSize);

    TL_ASSERT (o_pos < this-> GetByteSize ());

    return GetRawAddr_ (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_block>
  void * gct_ItemBlock <t_block>::InsertItems
  (
  t_Size o_pos,
  t_Size o_count
  )
    {
    TL_ASSERT (o_pos   <= this-> GetMaxItemSize ());
    TL_ASSERT (o_count <= this-> GetMaxItemSize ());

    o_pos   = (t_Size) (o_pos   * this-> o_FixSize);
    o_count = (t_Size) (o_count * this-> o_FixSize);
    t_Size o_oldSize = this-> GetByteSize ();

    TL_ASSERT (o_pos <= o_oldSize);

    t_Size o_newSize = (t_Size) (o_oldSize + o_count);

    TL_ASSERT (o_newSize >= o_count); // Overflow

    this-> SetByteSize (o_newSize);
    char * pc = GetRawAddr_ (o_pos);
    tl_MoveMemory (pc + o_count, pc, o_oldSize - o_pos);
    return pc;
    }

//---------------------------------------------------------------------------

template <class t_block>
  void * gct_ItemBlock <t_block>::DeleteItems
  (
  t_Size o_pos,
  t_Size o_count
  )
    {
    TL_ASSERT (o_pos   <= this-> GetMaxItemSize ());
    TL_ASSERT (o_count <= this-> GetMaxItemSize ());

    o_pos   = (t_Size) (o_pos   * this-> o_FixSize);
    o_count = (t_Size) (o_count * this-> o_FixSize);
    t_Size o_sum = (t_Size) (o_pos + o_count);
    t_Size o_oldSize = this-> GetByteSize ();

    TL_ASSERT (o_sum >= o_count); // Overflow
    TL_ASSERT (o_sum <= o_oldSize);

    tl_MoveMemory (GetRawAddr_ (o_pos),
      GetRawAddr_ (o_sum), o_oldSize - o_sum);
    this-> SetByteSize ((t_Size) (o_oldSize - o_count));
    return GetRawAddr_ (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_block>
  inline TL_TYPENAME gct_ItemBlock <t_block>::t_Size
  gct_ItemBlock <t_block>::GetDefaultPageSize () const
    {
    return 0;
    }

//---------------------------------------------------------------------------

template <class t_block>
  inline void gct_ItemBlock <t_block>::AlignPageSize
  (
  t_Size o_fixSize,
  t_Size
  )
    {
    this-> SetFixSize (o_fixSize);
    }

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

template <class t_block>
  class TL_TM_EXPORT gct_VarItemBlock:
    public gct_ItemBlock <gct_VarItemBlockBase <t_block> >
    {
    };

template <class t_block, t_UInt o_itemSize>
  class TL_TM_EXPORT gct_FixItemBlock:
    public gct_ItemBlock <gct_FixItemBlockBase <t_block, o_itemSize> >
    {
    };

#endif
