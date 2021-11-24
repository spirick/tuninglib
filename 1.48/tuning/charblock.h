
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
// File tuning/charblock.h

#ifndef TUNING_CHARBLOCK_H
#define TUNING_CHARBLOCK_H

#include "tuning/sys/cmemory.hpp"

//---------------------------------------------------------------------------

template <class t_size, class t_char>
  class TL_TM_EXPORT gct_CharSize
    {
    };

template <class t_size>
  class TL_TM_EXPORT gct_CharSize <t_size, char>
    {
  public:
    static inline t_size ToCharSize (t_size o_size)
      { return o_size; }
    static inline t_size FromCharSize (t_size o_size)
      { return o_size; }
    };

template <class t_size>
  class TL_TM_EXPORT gct_CharSize <t_size, wchar_t>
    {
  public:
    static const t_size  o_SizeMax = ((t_size) -1) >> TL_WCHAR_SHIFT;
    static inline t_size ToCharSize (t_size o_size)
      { return (t_size) (o_size >> TL_WCHAR_SHIFT); }
    static inline t_size FromCharSize (t_size o_size)
      {
      TL_ASSERT (o_size <= o_SizeMax);
      return (t_size) (o_size << TL_WCHAR_SHIFT);
      }
    };

//---------------------------------------------------------------------------

template <class t_block, class t_char>
  class TL_TM_EXPORT gct_CharBlock: public t_block
    {
  public:
    typedef TL_TYPENAME t_block::t_Size t_Size;
    typedef t_char                      t_Char;

  private:
    using TL_TMU_TYPENAME t_block::GetByteSize;
    using TL_TMU_TYPENAME t_block::SetByteSize;

    typedef ::gct_CharSize <t_Size, t_char> t_CharSize;
    inline t_char *      GetRawAddr_ (t_Size o_pos) const;

  public:
    inline gct_CharBlock & operator = (const gct_CharBlock & co_asgn);

    inline t_Size        GetMaxCharSize () const;
    inline t_Size        GetCharSize () const;
    inline void          SetCharSize (t_Size o_size);
    inline void          IncCharSize (t_Size o_inc);
    inline void          DecCharSize (t_Size o_dec);
    inline t_char *      GetRawAddr () const;
    inline t_char *      GetRawAddr (t_Size o_pos) const;
    inline t_char *      GetCharAddr () const;
    inline t_char *      GetCharAddr (t_Size o_pos) const;

    t_char *             AppendChars (t_Size o_len);
    t_char *             InsertChars (t_Size o_pos, t_Size o_len);
    t_char *             DeleteChars (t_Size o_pos, t_Size o_len);
    inline t_char *      FillChars (t_Size o_pos, t_Size o_len,
                           t_char c_fill = (t_char) 0);

    inline void          AssignChars (const t_char * pc_asgn, t_Size o_len);
    inline void          AppendChars (const t_char * pc_app, t_Size o_len);
    inline void          InsertChars (t_Size o_pos, const t_char * pc_ins,
                           t_Size o_len);
    void                 ReplaceChars (t_Size o_pos, t_Size o_delLen,
                           const t_char * pc_ins, t_Size o_insLen);

    inline t_Size        GetDefaultPageSize () const;
    inline void          AlignPageSize (t_Size o_itemSize, t_Size o_pageSize);
    };

//---------------------------------------------------------------------------

template <class t_block, class t_char>
  inline t_char * gct_CharBlock <t_block, t_char>::GetRawAddr_
  (
  t_Size o_pos
  ) const
    {
    return (t_char *) this-> GetAddr () + o_pos;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_char>
  inline gct_CharBlock <t_block, t_char> &
  gct_CharBlock <t_block, t_char>::operator =
  (
  const gct_CharBlock & co_asgn
  )
    {
    return (gct_CharBlock &) t_block::operator = (co_asgn);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_char>
  inline TL_TYPENAME gct_CharBlock <t_block, t_char>::t_Size
  gct_CharBlock <t_block, t_char>::GetMaxCharSize () const
    {
    return t_CharSize::ToCharSize (this-> GetMaxByteSize ());
    }

//---------------------------------------------------------------------------

template <class t_block, class t_char>
  inline TL_TYPENAME gct_CharBlock <t_block, t_char>::t_Size
  gct_CharBlock <t_block, t_char>::GetCharSize () const
    {
    return t_CharSize::ToCharSize (this-> GetByteSize ());
    }

//---------------------------------------------------------------------------

template <class t_block, class t_char>
  inline void gct_CharBlock <t_block, t_char>::SetCharSize
  (
  t_Size o_size
  )
    {
    this-> SetByteSize (t_CharSize::FromCharSize (o_size));
    }

//---------------------------------------------------------------------------

template <class t_block, class t_char>
  inline void gct_CharBlock <t_block, t_char>::IncCharSize
  (
  t_Size o_inc
  )
    {
    o_inc = t_CharSize::FromCharSize (o_inc);
    t_Size o_newSize = (t_Size) (this-> GetByteSize () + o_inc);

    TL_ASSERT (o_newSize >= o_inc); // Overflow

    this-> SetByteSize (o_newSize);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_char>
  inline void gct_CharBlock <t_block, t_char>::DecCharSize
  (
  t_Size o_dec
  )
    {
    o_dec = t_CharSize::FromCharSize (o_dec);

    TL_ASSERT (this-> GetByteSize () >= o_dec); // Underflow

    this-> SetByteSize ((t_Size) (this-> GetByteSize () - o_dec));
    }

//---------------------------------------------------------------------------

template <class t_block, class t_char>
  inline t_char * gct_CharBlock <t_block, t_char>::GetRawAddr () const
    {
    return (t_char *) this-> GetAddr ();
    }

//---------------------------------------------------------------------------

template <class t_block, class t_char>
  inline t_char * gct_CharBlock <t_block, t_char>::GetRawAddr
  (
  t_Size o_pos
  ) const
    {
    TL_ASSERT (o_pos <= GetCharSize ());

    return (t_char *) this-> GetAddr () + o_pos;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_char>
  inline t_char * gct_CharBlock <t_block, t_char>::GetCharAddr () const
    {
    TL_ASSERT (0 < this-> GetByteSize ());

    return (t_char *) this-> GetAddr ();
    }

//---------------------------------------------------------------------------

template <class t_block, class t_char>
  inline t_char * gct_CharBlock <t_block, t_char>::GetCharAddr
  (
  t_Size o_pos
  ) const
    {
    TL_ASSERT (o_pos < GetCharSize ());

    return (t_char *) this-> GetAddr () + o_pos;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_char>
  t_char * gct_CharBlock <t_block, t_char>::AppendChars
  (
  t_Size o_len
  )
    {
    t_Size o_oldSize = GetCharSize ();
    IncCharSize (o_len);
    return GetRawAddr_ (o_oldSize);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_char>
  t_char * gct_CharBlock <t_block, t_char>::InsertChars
  (
  t_Size o_pos,
  t_Size o_len
  )
    {
    t_Size o_oldSize = GetCharSize ();

    if (o_pos <= o_oldSize)
      {
      IncCharSize (o_len);
      t_char * pc = GetRawAddr_ (o_pos);
      tl_MoveMemory (pc + o_len, pc, o_oldSize - o_pos);
      return pc;
      }
    else
      {
      TL_ASSERT (false);
      return 0;
      }
    }

//---------------------------------------------------------------------------

template <class t_block, class t_char>
  t_char * gct_CharBlock <t_block, t_char>::DeleteChars
  (
  t_Size o_pos,
  t_Size o_len
  )
    {
    t_Size o_charSize = GetCharSize ();
    t_Size o_sum = (t_Size) (o_pos + o_len);

    TL_ASSERT (o_sum >= o_len); // Overflow
    TL_ASSERT (o_sum <= o_charSize);

    tl_MoveMemory (GetRawAddr_ (o_pos),
      GetRawAddr_ (o_sum), o_charSize - o_sum);
    this-> SetByteSize ((t_Size) (this-> GetByteSize () -
      t_CharSize::FromCharSize (o_len)));
    return GetRawAddr_ (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_char>
  inline t_char * gct_CharBlock <t_block, t_char>::FillChars
  (
  t_Size o_pos,
  t_Size o_len,
  t_char c_fill
  )
    {
    TL_DEBUG_CODE
    (
    t_Size o_sum = (t_Size) (o_pos + o_len);
    TL_ASSERT (o_sum >= o_len); // Overflow
    TL_ASSERT (o_sum <= GetCharSize ());
    )

    return (t_char *) tl_FillMemory (GetRawAddr_ (o_pos), o_len, c_fill);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_char>
  inline void gct_CharBlock <t_block, t_char>::AssignChars
  (
  const t_char * pc_asgn,
  t_Size o_len
  )
    {
    SetCharSize (o_len);
    tl_CopyMemory (GetRawAddr (), pc_asgn, o_len);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_char>
  inline void gct_CharBlock <t_block, t_char>::AppendChars
  (
  const t_char * pc_app,
  t_Size o_len
  )
    {
    tl_CopyMemory (AppendChars (o_len), pc_app, o_len);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_char>
  inline void gct_CharBlock <t_block, t_char>::InsertChars
  (
  t_Size o_pos,
  const t_char * pc_ins,
  t_Size o_len
  )
    {
    tl_CopyMemory (InsertChars (o_pos, o_len), pc_ins, o_len);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_char>
  void gct_CharBlock <t_block, t_char>::ReplaceChars
  (
  t_Size o_pos,
  t_Size o_delLen,
  const t_char * pc_ins,
  t_Size o_insLen
  )
    {
    t_char * pc = o_delLen > o_insLen ?
      DeleteChars (o_pos, (t_Size) (o_delLen - o_insLen)) :
      InsertChars (o_pos, (t_Size) (o_insLen - o_delLen));

    tl_CopyMemory (pc, pc_ins, o_insLen);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_char>
  inline TL_TYPENAME gct_CharBlock <t_block, t_char>::t_Size
  gct_CharBlock <t_block, t_char>::GetDefaultPageSize () const
    {
    return 0;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_char>
  inline void gct_CharBlock <t_block, t_char>::AlignPageSize
  (
  t_Size,
  t_Size
  )
    {
    }

#endif
