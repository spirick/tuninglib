
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
// File tuning/pageblock.hpp

#ifndef TUNING_PAGEBLOCK_HPP
#define TUNING_PAGEBLOCK_HPP

#include "tuning/defs.hpp"

//---------------------------------------------------------------------------

class TL_EXPORT ct_PageBlockBase
  {
public:
  typedef t_UInt       t_Size;

private:
  struct st_Data
    {
    t_Size               o_FixSize;
    t_Size               o_FixPagePtrs;
    t_Size               o_OrigPageSize;
    t_Size               o_MaxItemSize;
    t_Size               o_Size;
    char * *             ppc_Pages;
    t_Size               o_PageSize;
    t_Size               o_RoundedSize;
    unsigned             u_Shift;
    t_Size               u_Mask;
    }                  so_Data;

  inline t_Size        Page (t_Size o_pos) const;
  inline t_Size        Offset (t_Size o_pos) const;
  inline t_Size        ItemPage (t_Size o_pos) const;
  inline t_Size        ItemOffset (t_Size o_pos) const;
  void                 CopyFrom (const ct_PageBlockBase & co_copy);

  // Block
  inline t_Size        GetByteSize () const;
  void                 SetByteSize (t_Size o_newSize);
  inline void *        GetAddr () const;

protected:
  void                 SetByteSize0 ();
  virtual void *       AllocPtr (t_Size o_size) = 0;
  virtual void *       ReallocPtr (void * pv_mem, t_Size o_size) = 0;
  virtual void *       AllocData (t_Size o_size) = 0;
  virtual void         FreeData (void * pv_mem) = 0;
  virtual void         LastPageWarning () { }
  virtual void         LastPageError () { }

public:
  // Block
                       ct_PageBlockBase ();
  inline               ct_PageBlockBase (const ct_PageBlockBase & co_init);
  virtual              ~ct_PageBlockBase () TL_CAN_THROW { }
  inline ct_PageBlockBase & operator = (const ct_PageBlockBase & co_asgn);
  void                 Swap (ct_PageBlockBase & co_swap);

  // CharBlock
  inline t_Size        GetMaxCharSize () const;
  inline t_Size        GetCharSize () const;
  inline void          SetCharSize (t_Size o_size);
  inline void          IncCharSize (t_Size o_inc);
  inline void          DecCharSize (t_Size o_dec);
  inline char *        GetRawAddr () const;
  inline char *        GetRawAddr (t_Size o_pos) const;
  inline char *        GetCharAddr () const;
  inline char *        GetCharAddr (t_Size o_pos) const;

  char *               AppendChars (t_Size o_count);
  char *               InsertChars (t_Size o_pos, t_Size o_count);
  char *               DeleteChars (t_Size o_pos, t_Size o_count);
  char *               FillChars (t_Size o_pos, t_Size o_count,
                         char c_fill = '\0');

  // ItemBlock
  inline t_Size        GetFixSize () const;
  inline t_Size        GetMaxItemSize () const;
  inline t_Size        GetItemSize () const;
  inline void          SetItemSize (t_Size o_size);
  inline void          IncItemSize1 ();
  inline void          DecItemSize1 ();
  inline void          IncItemSize (t_Size o_inc);
  inline void          DecItemSize (t_Size o_dec);
  inline void *        GetItemAddr (t_Size o_pos) const;

  inline void *        AppendItems (t_Size o_count);
  inline void *        InsertItems (t_Size o_pos, t_Size o_count);
  inline void *        DeleteItems (t_Size o_pos, t_Size o_count);

  // PageBlock only Methods
  inline t_Size        GetDefaultPageSize () const;
  inline t_Size        GetFixPagePtrs () const;
  void                 SetFixPagePtrs (t_Size o_ptrs);
  void                 AlignPageSize (t_Size o_fixSize, t_Size o_pageSize);
  inline t_Size        GetPageSize () const;
  inline t_Size        GetRoundedSize () const;
  };

//---------------------------------------------------------------------------

class TL_EXPORT ct_PageBlock: public ct_PageBlockBase
  {
protected:
  virtual void *       AllocPtr (t_Size o_size);
  virtual void *       ReallocPtr (void * pv_mem, t_Size o_size);
  virtual void *       AllocData (t_Size o_size);
  virtual void         FreeData (void * pv_mem);

public:
                       ~ct_PageBlock () TL_CAN_THROW;
  };

//---------------------------------------------------------------------------

inline ct_PageBlockBase::t_Size ct_PageBlockBase::Page
(
t_Size o_pos
) const
  {
  return o_pos / so_Data. o_PageSize;
  }

//---------------------------------------------------------------------------

inline ct_PageBlockBase::t_Size ct_PageBlockBase::Offset
(
t_Size o_pos
) const
  {
  return o_pos % so_Data. o_PageSize;
  }

//---------------------------------------------------------------------------

inline ct_PageBlockBase::t_Size ct_PageBlockBase::ItemPage
(
t_Size o_pos
) const
  {
  return o_pos >> so_Data. u_Shift;
  }

//---------------------------------------------------------------------------

inline ct_PageBlockBase::t_Size ct_PageBlockBase::ItemOffset
(
t_Size o_pos
) const
  {
  return (o_pos & so_Data. u_Mask) * so_Data. o_FixSize;
  }

//---------------------------------------------------------------------------

inline ct_PageBlockBase::ct_PageBlockBase
(
const ct_PageBlockBase & co_init
)
  {
  CopyFrom (co_init);
  }

//---------------------------------------------------------------------------

inline ct_PageBlockBase & ct_PageBlockBase::operator =
(
const ct_PageBlockBase & co_asgn
)
  {
  if (& co_asgn != this)
    {
    SetByteSize (0);
    CopyFrom (co_asgn);
    }

  return * this;
  }

//---------------------------------------------------------------------------

inline ct_PageBlockBase::t_Size ct_PageBlockBase::GetByteSize () const
  {
  return so_Data. o_Size;
  }

//---------------------------------------------------------------------------

inline void * ct_PageBlockBase::GetAddr () const
  {
  return so_Data. ppc_Pages != 0 ? so_Data. ppc_Pages [0] : 0;
  }

//---------------------------------------------------------------------------

inline ct_PageBlockBase::t_Size ct_PageBlockBase::GetMaxCharSize () const
  {
  if ((so_Data. o_PageSize == 0) || (so_Data. o_FixPagePtrs == 0))
    return (t_Size) -1;
  else
    return so_Data. o_PageSize * so_Data. o_FixPagePtrs;
  }

//---------------------------------------------------------------------------

inline ct_PageBlockBase::t_Size ct_PageBlockBase::GetCharSize () const
  {
  return so_Data. o_Size;
  }

//---------------------------------------------------------------------------

inline void ct_PageBlockBase::SetCharSize
(
t_Size o_size
)
  {
  SetByteSize (o_size);
  }

//---------------------------------------------------------------------------

inline void ct_PageBlockBase::IncCharSize
(
t_Size o_inc
)
  {
  t_Size o_newSize = so_Data. o_Size + o_inc;

  TL_ASSERT (o_newSize >= o_inc); // Overflow

  SetByteSize (o_newSize);
  }

//---------------------------------------------------------------------------

inline void ct_PageBlockBase::DecCharSize
(
t_Size o_dec
)
  {
  TL_ASSERT (so_Data. o_Size >= o_dec); // Underflow

  SetByteSize (so_Data. o_Size - o_dec);
  }

//---------------------------------------------------------------------------

inline char * ct_PageBlockBase::GetRawAddr () const
  {
  return (char *) GetAddr ();
  }

//---------------------------------------------------------------------------

inline char * ct_PageBlockBase::GetRawAddr
(
t_Size o_pos
) const
  {
  TL_ASSERT (o_pos < so_Data. o_Size);

  return so_Data. ppc_Pages [Page (o_pos)] + Offset (o_pos);
  }

//---------------------------------------------------------------------------

inline char * ct_PageBlockBase::GetCharAddr () const
  {
  TL_ASSERT (0 < so_Data. o_Size);

  return (char *) GetAddr ();
  }

//---------------------------------------------------------------------------

inline char * ct_PageBlockBase::GetCharAddr
(
t_Size o_pos
) const
  {
  TL_ASSERT (o_pos < so_Data. o_Size);

  return so_Data. ppc_Pages [Page (o_pos)] + Offset (o_pos);
  }

//---------------------------------------------------------------------------

inline char * ct_PageBlockBase::AppendChars
(
t_Size o_count
)
  {
  t_Size o_oldSize = GetCharSize ();
  IncCharSize (o_count);
  return GetCharAddr (o_oldSize);
  }

//---------------------------------------------------------------------------

inline ct_PageBlockBase::t_Size ct_PageBlockBase::GetFixSize () const
  {
  return so_Data. o_FixSize;
  }

//---------------------------------------------------------------------------

inline ct_PageBlockBase::t_Size ct_PageBlockBase::GetMaxItemSize () const
  {
  return so_Data. o_MaxItemSize;
  }

//---------------------------------------------------------------------------

inline ct_PageBlockBase::t_Size ct_PageBlockBase::GetItemSize () const
  {
  if (so_Data. o_Size == 0)
    return 0;
  else
    return so_Data. o_Size / so_Data. o_FixSize;
  }

//---------------------------------------------------------------------------

inline void ct_PageBlockBase::SetItemSize
(
t_Size o_size
)
  {
  TL_ASSERT (o_size <= so_Data. o_MaxItemSize);

  SetByteSize (o_size * so_Data. o_FixSize);
  }

//---------------------------------------------------------------------------

inline void ct_PageBlockBase::IncItemSize1 ()
  {
  t_Size o_newSize = so_Data. o_Size + so_Data. o_FixSize;

  TL_ASSERT (o_newSize >= so_Data. o_FixSize); // Overflow

  SetByteSize (o_newSize);
  }

//---------------------------------------------------------------------------

inline void ct_PageBlockBase::DecItemSize1 ()
  {
  TL_ASSERT (so_Data. o_Size >= so_Data. o_FixSize); // Underflow

  SetByteSize (so_Data. o_Size - so_Data. o_FixSize);
  }

//---------------------------------------------------------------------------

inline void ct_PageBlockBase::IncItemSize
(
t_Size o_inc
)
  {
  TL_ASSERT (o_inc <= so_Data. o_MaxItemSize);

  o_inc *= so_Data. o_FixSize;
  t_Size o_newSize = so_Data. o_Size + o_inc;

  TL_ASSERT (o_newSize >= o_inc); // Overflow

  SetByteSize (o_newSize);
  }

//---------------------------------------------------------------------------

inline void ct_PageBlockBase::DecItemSize
(
t_Size o_dec
)
  {
  TL_ASSERT (o_dec <= so_Data. o_MaxItemSize);

  o_dec *= so_Data. o_FixSize;

  TL_ASSERT (so_Data. o_Size >= o_dec); // Underflow

  SetByteSize (so_Data. o_Size - o_dec);
  }

//---------------------------------------------------------------------------

inline void * ct_PageBlockBase::GetItemAddr
(
t_Size o_pos
) const
  {
  TL_ASSERT (o_pos <= so_Data. o_MaxItemSize);
  TL_ASSERT (o_pos * so_Data. o_FixSize < so_Data. o_Size);

  return so_Data. ppc_Pages [ItemPage (o_pos)] + ItemOffset (o_pos);
  }

//---------------------------------------------------------------------------

inline void * ct_PageBlockBase::AppendItems
(
t_Size o_count
)
  {
  t_Size o_oldSize = GetCharSize ();
  IncCharSize (o_count * so_Data. o_FixSize);
  return GetCharAddr (o_oldSize);
  }

//---------------------------------------------------------------------------

inline void * ct_PageBlockBase::InsertItems
(
t_Size o_pos,
t_Size o_count
)
  {
  TL_ASSERT (o_pos   <= so_Data. o_MaxItemSize);
  TL_ASSERT (o_count <= so_Data. o_MaxItemSize);

  return InsertChars (o_pos * so_Data. o_FixSize, o_count * so_Data. o_FixSize);
  }

//---------------------------------------------------------------------------

inline void * ct_PageBlockBase::DeleteItems
(
t_Size o_pos,
t_Size o_count
)
  {
  TL_ASSERT (o_pos   <= so_Data. o_MaxItemSize);
  TL_ASSERT (o_count <= so_Data. o_MaxItemSize);

  return DeleteChars (o_pos * so_Data. o_FixSize, o_count * so_Data. o_FixSize);
  }

//---------------------------------------------------------------------------

inline ct_PageBlockBase::t_Size ct_PageBlockBase::GetDefaultPageSize () const
  {
  return 0x1000; // 4 KB
  }

//---------------------------------------------------------------------------

inline ct_PageBlockBase::t_Size ct_PageBlockBase::GetFixPagePtrs () const
  {
  return so_Data. o_FixPagePtrs;
  }

//---------------------------------------------------------------------------

inline ct_PageBlockBase::t_Size ct_PageBlockBase::GetPageSize () const
  {
  return so_Data. o_PageSize;
  }

//---------------------------------------------------------------------------

inline ct_PageBlockBase::t_Size ct_PageBlockBase::GetRoundedSize () const
  {
  return so_Data. o_RoundedSize;
  }

#endif
