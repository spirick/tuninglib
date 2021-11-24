
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
// File tuning/blockstore.h

#ifndef TUNING_BLOCKSTORE_H
#define TUNING_BLOCKSTORE_H

#include "tuning/charblock.h"
#include "tuning/itemblock.h"

//---------------------------------------------------------------------------

template <class t_itemBlock, class t_charBlock>
  class TL_TM_EXPORT gct_BlockStore: public t_itemBlock
    {
  public:
    typedef TL_TYPENAME t_itemBlock::t_Size t_Size;
    typedef TL_TYPENAME t_itemBlock::t_Size t_Position;

  private:
    using TL_TMU_TYPENAME t_itemBlock::GetFixSize;
    using TL_TMU_TYPENAME t_itemBlock::GetItemSize;
    using TL_TMU_TYPENAME t_itemBlock::SetItemSize;
    using TL_TMU_TYPENAME t_itemBlock::IncItemSize1;
    using TL_TMU_TYPENAME t_itemBlock::DecItemSize1;
    using TL_TMU_TYPENAME t_itemBlock::IncItemSize;
    using TL_TMU_TYPENAME t_itemBlock::DecItemSize;
    using TL_TMU_TYPENAME t_itemBlock::GetItemAddr;
    using TL_TMU_TYPENAME t_itemBlock::InsertItems;
    using TL_TMU_TYPENAME t_itemBlock::DeleteItems;
    using TL_TMU_TYPENAME t_itemBlock::AlignPageSize;

    struct st_Data
      {
      bool                 b_FreeSort;
      t_Size               o_PageSize;
      t_Position           o_FirstFree;
      }                  so_Data;

    inline t_Position *  IdxAddrOf (t_Position o_pos) const;
    void                 FreePlain (t_Position o_pos);
    void                 FreeSort (t_Position o_pos);

  public:
    inline               gct_BlockStore ();
    void                 Swap (gct_BlockStore & co_swap);

    inline t_UInt        StoreInfoSize () const;
    inline t_UInt        MaxAlloc () const;

    t_Position           Alloc (t_Size o_size);
    t_Position           Realloc (t_Position o_pos, t_Size o_size);
    inline void          Free (t_Position o_pos);

    inline void *        AddrOf (t_Position o_pos) const;
    inline t_Position    PosOf (void * pv_adr) const;

    inline t_Size        SizeOf (t_Position o_pos) const;
    inline t_Size        RoundedSizeOf (t_Position o_pos) const;

    inline bool          CanFreeAll () const;
    inline void          FreeAll ();

    void                 SetSortedFree (bool b);
    void                 SetPageSize (t_Size o_size);
    inline t_Position    LastIdx () const;
    inline bool          HasFree () const;
    void                 FreeUnused ();
    };

//---------------------------------------------------------------------------

template <class t_itemBlock, class t_charBlock>
  inline TL_TYPENAME gct_BlockStore <t_itemBlock, t_charBlock>::t_Position *
  gct_BlockStore <t_itemBlock, t_charBlock>::IdxAddrOf
  (
  t_Position o_pos
  ) const
    {
    return (t_Position *) this-> GetItemAddr ((t_Size) (o_pos - 1));
    }

//---------------------------------------------------------------------------

template <class t_itemBlock, class t_charBlock>
  void gct_BlockStore <t_itemBlock, t_charBlock>::FreePlain
  (
  t_Position o_pos
  )
    {
    TL_ASSERT (o_pos <= LastIdx ());

    if (o_pos != 0)
      {
      if (o_pos != LastIdx ())
        {
        if ((o_pos < so_Data. o_FirstFree) || (so_Data. o_FirstFree == 0))
          {
          * IdxAddrOf (o_pos) = so_Data. o_FirstFree;
          so_Data. o_FirstFree = o_pos;
          }
        else
          {
          * IdxAddrOf (o_pos) = * IdxAddrOf (so_Data. o_FirstFree);
          * IdxAddrOf (so_Data. o_FirstFree) = o_pos;
          }
        }
      else
        {
        t_Size o_dec = 1;
        t_Size o_lastIdx = LastIdx () - 1;

        while ((so_Data. o_FirstFree == o_lastIdx) && (so_Data. o_FirstFree != 0))
          {
          so_Data. o_FirstFree = * IdxAddrOf (so_Data. o_FirstFree);
          o_dec ++;
          o_lastIdx --;
          }

        this-> DecItemSize (o_dec);
        }
      }
    }

//---------------------------------------------------------------------------

template <class t_itemBlock, class t_charBlock>
  void gct_BlockStore <t_itemBlock, t_charBlock>::FreeSort
  (
  t_Position o_pos
  )
    {
    TL_ASSERT (o_pos <= LastIdx ());

    if (o_pos != 0)
      {
      if (so_Data. o_FirstFree != 0)
        {
        if (o_pos == LastIdx ())
          {
          TL_ASSERT (o_pos > 1);

          t_Position o_free = so_Data. o_FirstFree;
          t_Position * po_free = IdxAddrOf (o_free);
          t_Position * po_toCluster = & so_Data. o_FirstFree;

          while (* po_free != 0)
            {
            if (* po_free != o_free + 1)
              po_toCluster = po_free;

            o_free = * po_free;
            po_free = IdxAddrOf (o_free);
            }

          if (o_free == o_pos - 1)
            {
            t_Position o_toCluster = * po_toCluster;

            * po_toCluster = 0;
            this-> SetItemSize ((t_Size) (o_toCluster - 1));
            }
          else
            this-> DecItemSize1 ();
          }
        else
          {
          TL_ASSERT (o_pos != so_Data. o_FirstFree);

          t_Position * po_free = & so_Data. o_FirstFree;

          while ((* po_free != 0) && (* po_free < o_pos))
            po_free = IdxAddrOf (* po_free);

          TL_ASSERT (o_pos != * po_free);

          * IdxAddrOf (o_pos) = * po_free;
          * po_free = o_pos;
          }
        }
      else
        if (o_pos == LastIdx ())
          this-> DecItemSize1 ();
        else
          {
          * IdxAddrOf (o_pos) = 0;
          so_Data. o_FirstFree = o_pos;
          }
      }
    }

//---------------------------------------------------------------------------

template <class t_itemBlock, class t_charBlock>
  inline gct_BlockStore <t_itemBlock, t_charBlock>::gct_BlockStore ()
    {
    so_Data. b_FreeSort  = false;
    so_Data. o_PageSize  = (t_Size) this-> GetDefaultPageSize ();
    so_Data. o_FirstFree = 0;
    }

//---------------------------------------------------------------------------

template <class t_itemBlock, class t_charBlock>
  void gct_BlockStore <t_itemBlock, t_charBlock>::Swap
  (
  gct_BlockStore & co_swap
  )
    {
    t_itemBlock::Swap (co_swap);
    tl_SwapObj (so_Data, co_swap. so_Data);
    }

//---------------------------------------------------------------------------

template <class t_itemBlock, class t_charBlock>
  inline t_UInt gct_BlockStore <t_itemBlock, t_charBlock>::StoreInfoSize () const
    {
    return 0;
    }

//---------------------------------------------------------------------------

template <class t_itemBlock, class t_charBlock>
  inline t_UInt gct_BlockStore <t_itemBlock, t_charBlock>::MaxAlloc () const
    {
    return this-> GetFixSize ();
    }

//---------------------------------------------------------------------------

template <class t_itemBlock, class t_charBlock>
  TL_TYPENAME gct_BlockStore <t_itemBlock, t_charBlock>::t_Position
  gct_BlockStore <t_itemBlock, t_charBlock>::Alloc
  (
  t_Size o_size
  )
    {
    if (o_size != 0)
      {
      if (this-> GetItemSize () == 0)
        {
        TL_ASSERT (o_size >= sizeof (t_Position));

        this-> AlignPageSize (o_size, so_Data. o_PageSize);
        }

      TL_ASSERT (o_size <= this-> GetFixSize ());

      if (so_Data. o_FirstFree != 0)
        {
        t_Position o_pos = so_Data. o_FirstFree;
        so_Data. o_FirstFree = * IdxAddrOf (o_pos);

        return o_pos;
        }
      else
        {
        this-> IncItemSize1 ();
        return LastIdx ();
        }
      }
    else
      return 0;
    }

//---------------------------------------------------------------------------

template <class t_itemBlock, class t_charBlock>
  TL_TYPENAME gct_BlockStore <t_itemBlock, t_charBlock>::t_Position
  gct_BlockStore <t_itemBlock, t_charBlock>::Realloc
  (
  t_Position o_pos,
  t_Size o_size
  )
    {
    if (o_pos != 0)
      {
      TL_ASSERT (o_pos <= LastIdx ());

      if (o_size != 0)
        {
        TL_ASSERT (o_size <= this-> GetFixSize ());

        return o_pos;
        }
      else
        {
        Free (o_pos);
        return 0;
        }
      }
    else
      return Alloc (o_size);
    }

//---------------------------------------------------------------------------

template <class t_itemBlock, class t_charBlock>
  inline void gct_BlockStore <t_itemBlock, t_charBlock>::Free
  (
  t_Position o_pos
  )
    {
    if (so_Data. b_FreeSort)
      FreeSort (o_pos);
    else
      FreePlain (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_itemBlock, class t_charBlock>
  inline void * gct_BlockStore <t_itemBlock, t_charBlock>::AddrOf
  (
  t_Position o_pos
  ) const
    {
    if (o_pos != 0)
      return this-> GetItemAddr ((t_Size) (o_pos - 1));
    else
      return 0;
    }

//---------------------------------------------------------------------------

template <class t_itemBlock, class t_charBlock>
  inline TL_TYPENAME gct_BlockStore <t_itemBlock, t_charBlock>::t_Position
  gct_BlockStore <t_itemBlock, t_charBlock>::PosOf
  (
  void * pv_adr
  ) const
    {
    TL_ASSERT (false);
    return 0;
    }

//---------------------------------------------------------------------------

template <class t_itemBlock, class t_charBlock>
  inline TL_TYPENAME gct_BlockStore <t_itemBlock, t_charBlock>::t_Size
  gct_BlockStore <t_itemBlock, t_charBlock>::SizeOf
  (
  t_Position
  ) const
    {
    TL_ASSERT (false);
    return 0;
    }

//---------------------------------------------------------------------------

template <class t_itemBlock, class t_charBlock>
  inline TL_TYPENAME gct_BlockStore <t_itemBlock, t_charBlock>::t_Size
  gct_BlockStore <t_itemBlock, t_charBlock>::RoundedSizeOf
  (
  t_Position o_pos
  ) const
    {
    return o_pos == 0 ? 0 : this-> GetFixSize ();
    }

//---------------------------------------------------------------------------

template <class t_itemBlock, class t_charBlock>
  inline bool gct_BlockStore <t_itemBlock, t_charBlock>::CanFreeAll () const
    {
    return true;
    }

//---------------------------------------------------------------------------

template <class t_itemBlock, class t_charBlock>
  inline void gct_BlockStore <t_itemBlock, t_charBlock>::FreeAll ()
    {
    this-> SetItemSize (0);
    so_Data. o_FirstFree = 0;
    }

//---------------------------------------------------------------------------

template <class t_itemBlock, class t_charBlock>
  void gct_BlockStore <t_itemBlock, t_charBlock>::SetSortedFree
  (
  bool b
  )
    {
    so_Data. b_FreeSort = b;
    }

//---------------------------------------------------------------------------

template <class t_itemBlock, class t_charBlock>
  void gct_BlockStore <t_itemBlock, t_charBlock>::SetPageSize
  (
  t_Size o_size
  )
    {
    so_Data. o_PageSize = o_size;
    }

//---------------------------------------------------------------------------

template <class t_itemBlock, class t_charBlock>
  inline TL_TYPENAME gct_BlockStore <t_itemBlock, t_charBlock>::t_Position
  gct_BlockStore <t_itemBlock, t_charBlock>::LastIdx () const
    {
    return (t_Position) this-> GetItemSize ();
    }

//---------------------------------------------------------------------------

template <class t_itemBlock, class t_charBlock>
  inline bool gct_BlockStore <t_itemBlock, t_charBlock>::HasFree () const
    {
    return so_Data. o_FirstFree != 0;
    }

//---------------------------------------------------------------------------

template <class t_itemBlock, class t_charBlock>
  void gct_BlockStore <t_itemBlock, t_charBlock>::FreeUnused ()
    {
    if (so_Data. o_FirstFree != 0)
      {
      t_charBlock co_block;
      t_Position o_free = so_Data. o_FirstFree;

      co_block. AlignPageSize (1, LastIdx ());
      co_block. SetCharSize (LastIdx ());
      co_block. FillChars (0, co_block. GetCharSize (), '\0');

      while (o_free != 0)
        {
        * co_block. GetCharAddr ((o_free - 1)) = '\01';
        o_free = * IdxAddrOf (o_free);
        }

      t_Position o_last = co_block. GetCharSize ();

      while ((o_last > 0) &&
             (* co_block. GetCharAddr ((o_last - 1)) == '\01'))
        o_last --;

      this-> SetItemSize (o_last);
      so_Data. o_FirstFree = 0;

      if (o_last > 0)
        {
        t_Position o_idx = 0;
        o_free = 0;

        while (o_idx < o_last)
          {
          if  (* co_block. GetCharAddr (o_idx) == '\01')
            {
            if (so_Data. o_FirstFree == 0)
              so_Data. o_FirstFree = (t_Position) (o_idx + 1);

            if (o_free != 0)
              * IdxAddrOf (o_free) = (t_Position) (o_idx + 1);

            o_free = (t_Position) (o_idx + 1);
            }

          o_idx ++;
          }

        if (o_free != 0)
          * IdxAddrOf (o_free) = 0;
        }
      }
    }

//---------------------------------------------------------------------------

#define BLOCK_STORE_DCL(StoreSpec)                                   \
  class TL_EXPORT StoreSpec ## BlockStore:                           \
    public gct_BlockStore <gct_VarItemBlock <StoreSpec ## Block>,    \
      gct_CharBlock <StoreSpec ## Block, char> > { };

#define BLOCKREF_STORE_DCL(StoreSpec)                  \
  class TL_EXPORT StoreSpec ## BlockRefStore:          \
    public gct_RefStore <StoreSpec ## BlockStore> { };

#define BLOCK_STORE_DCLS(Obj)           \
  BLOCK_STORE_DCL (ct_ ## Obj ## _)     \
  BLOCK_STORE_DCL (ct_ ## Obj ## 8)     \
  BLOCK_STORE_DCL (ct_ ## Obj ## 16)    \
  BLOCK_STORE_DCL (ct_ ## Obj ## 32)

#define BLOCKREF_STORE_DCLS(Obj)        \
  BLOCKREF_STORE_DCL (ct_ ## Obj ## _)  \
  BLOCKREF_STORE_DCL (ct_ ## Obj ## 8)  \
  BLOCKREF_STORE_DCL (ct_ ## Obj ## 16) \
  BLOCKREF_STORE_DCL (ct_ ## Obj ## 32)

#endif
