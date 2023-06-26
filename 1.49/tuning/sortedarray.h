
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
// File tuning/sortedarray.h

#ifndef TUNING_SORTEDARRAY_H
#define TUNING_SORTEDARRAY_H

#include "tuning/sys/cmemory.hpp"
#include "tuning/extcontainer.h"
#include "tuning/itemblock.h"

//---------------------------------------------------------------------------

template <class t_obj>
  class TL_TM_EXPORT gct_SortedArrayNode
    {
  public:
    t_obj         o_Obj;

    inline        gct_SortedArrayNode () { }
    inline        gct_SortedArrayNode (const t_obj & o_obj): o_Obj (o_obj) { }

    static inline void * operator new (size_t, void * pv) { return pv; }
    static inline void   operator delete (void *, void *) { }
    static inline void   operator delete (void *) { }
    };

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  class TL_TM_EXPORT gct_SortedArray: public t_block
    {
  public:
    typedef TL_TYPENAME t_block::t_Size t_Size;
    typedef TL_TYPENAME t_block::t_Size t_Length;
    typedef TL_TYPENAME t_block::t_Size t_Position;
    typedef t_obj                       t_Object;

  private:
    using TL_TMU_TYPENAME t_block::GetFixSize;
    using TL_TMU_TYPENAME t_block::GetItemSize;
    using TL_TMU_TYPENAME t_block::SetItemSize;
    using TL_TMU_TYPENAME t_block::IncItemSize1;
    using TL_TMU_TYPENAME t_block::DecItemSize1;
    using TL_TMU_TYPENAME t_block::IncItemSize;
    using TL_TMU_TYPENAME t_block::DecItemSize;
    using TL_TMU_TYPENAME t_block::GetItemAddr;
    using TL_TMU_TYPENAME t_block::InsertItems;
    using TL_TMU_TYPENAME t_block::DeleteItems;
    using TL_TMU_TYPENAME t_block::AlignPageSize;

    inline gct_SortedArrayNode <t_obj> *
                         Node (t_Position o_pos) const;
    void                 CopyFrom (const gct_SortedArray & co_copy);

  protected:
    t_Position           FirstForSearch (const t_Object * po_obj) const;
    t_Position           LastForSearch (const t_Object * po_obj) const;
    t_Position           NextForSearch (t_Position o_pos,
                           const t_Object * po_obj) const;
    t_Position           PrevForSearch (t_Position o_pos,
                           const t_Object * po_obj) const;

  public:
    inline               gct_SortedArray ();
    inline               gct_SortedArray (const gct_SortedArray & co_init);
    inline               ~gct_SortedArray () TL_CAN_THROW;
    inline gct_SortedArray & operator = (const gct_SortedArray & co_asgn);

    inline bool          IsEmpty () const;
    inline t_Length      GetMaxLen () const;
    inline t_Length      GetLen () const;

    inline t_Position    First () const;
    inline t_Position    Last () const;
    inline t_Position    Next (t_Position o_pos) const;
    inline t_Position    Prev (t_Position o_pos) const;
    inline t_Position    Nth (t_Length u_idx) const;

    inline t_Object *    GetObj (t_Position o_pos) const;

    inline t_Position    AddObj (const t_Object * po_obj);
    inline t_Position    AddObjBefore (t_Position o_pos,
                           const t_Object * po_obj);
    t_Position           AddObjAfter (t_Position o_pos,
                           const t_Object * po_obj);

    void                 AppendObj (const t_Object * po_obj = 0,
                           t_Length o_count = 1);
    void                 TruncateObj (t_Length o_count = 1);

    t_Position           DelObj (t_Position o_pos);
    void                 DelAll ();

    inline t_Position    FreeObj (t_Position o_pos);
    inline void          FreeAll ();

    inline void          SetPageSize (t_Size o_size);
    t_Position           Before (const t_Object * po_obj) const;
    };

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline gct_SortedArrayNode <t_obj> *
  gct_SortedArray <t_obj, t_block>::Node
  (
  t_Position o_pos
  ) const
    {
    TL_ASSERT (o_pos != 0);
    TL_ASSERT (o_pos <= Last ());

    return (gct_SortedArrayNode <t_obj> *)
      this-> GetItemAddr ((t_Position) (o_pos - 1));
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  void gct_SortedArray <t_obj, t_block>::CopyFrom
  (
  const gct_SortedArray & co_copy
  )
    {
    TL_ASSERT (IsEmpty ());

    t_Position o_pos = 0;
    this-> SetItemSize (co_copy. GetItemSize ());

    TL_TRY
      (
      for (o_pos = First (); o_pos != 0; o_pos = Next (o_pos))
        new (Node (o_pos))
          gct_SortedArrayNode <t_obj> (* co_copy. Node (o_pos));
      )
    TL_CATCH
      (
      this-> SetItemSize (o_pos - 1);
      DelAll ();
      throw;
      )
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  TL_TYPENAME gct_SortedArray <t_obj, t_block>::t_Position
  gct_SortedArray <t_obj, t_block>::FirstForSearch
  (
  const t_Object * po_obj
  ) const
    {
    TL_ASSERT (po_obj != 0);

    t_Position o_min = 1;
    t_Position o_mid;
    t_Position o_max = GetLen ();

    if ((o_max == 0) ||
        (* po_obj < * GetObj (o_min)) ||
        (* GetObj (o_max) < * po_obj))
      return 0;
    else
      if (! (* GetObj (o_min) < * po_obj))
        return o_min;
      else
        for (;;)
          {
          // * GetObj (o_min) < * po_obj <= * GetObj (o_max)

          o_mid = o_max - ((o_max - o_min) / 2);

          if (o_mid == o_max)
            {
            if (* po_obj < * GetObj (o_max))
              return 0;
            else
              return o_max;
            }

          if (* GetObj (o_mid) < * po_obj)
            o_min = o_mid;
          else
            o_max = o_mid;
          }
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  TL_TYPENAME gct_SortedArray <t_obj, t_block>::t_Position
  gct_SortedArray <t_obj, t_block>::LastForSearch
  (
  const t_Object * po_obj
  ) const
    {
    TL_ASSERT (po_obj != 0);

    t_Position o_min = 1;
    t_Position o_mid;
    t_Position o_max = GetLen ();

    if ((o_max == 0) ||
        (* po_obj < * GetObj (o_min)) ||
        (* GetObj (o_max) < * po_obj))
      return 0;
    else
      if (! (* po_obj < * GetObj (o_max)))
        return o_max;
      else
        for (;;)
          {
          // * GetObj (o_min) <= * po_obj < * GetObj (o_max)

          o_mid = o_min + ((o_max - o_min) / 2);

          if (o_mid == o_min)
            {
            if (* GetObj (o_min) < * po_obj)
              return 0;
            else
              return o_min;
            }

          if (* po_obj < * GetObj (o_mid))
            o_max = o_mid;
          else
            o_min = o_mid;
          }
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  TL_TYPENAME gct_SortedArray <t_obj, t_block>::t_Position
  gct_SortedArray <t_obj, t_block>::NextForSearch
  (
  t_Position o_pos,
  const t_Object * po_obj
  ) const
    {
    t_Position o_next = Next (o_pos);

    if ((o_next != 0) && (! (* po_obj < * GetObj (o_next))))
      return o_next;
    else
      return 0;
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  TL_TYPENAME gct_SortedArray <t_obj, t_block>::t_Position
  gct_SortedArray <t_obj, t_block>::PrevForSearch
  (
  t_Position o_pos,
  const t_Object * po_obj
  ) const
    {
    t_Position o_prev = Prev (o_pos);

    if ((o_prev != 0) && (! (* GetObj (o_prev) < * po_obj)))
      return o_prev;
    else
      return 0;
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline gct_SortedArray <t_obj, t_block>::gct_SortedArray ()
    {
    this-> AlignPageSize (sizeof (gct_SortedArrayNode <t_obj>),
      this-> GetDefaultPageSize ());
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline gct_SortedArray <t_obj, t_block>::gct_SortedArray
  (
  const gct_SortedArray & co_init
  )
    {
    this-> AlignPageSize (sizeof (gct_SortedArrayNode <t_obj>),
      this-> GetDefaultPageSize ());
    CopyFrom (co_init);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline gct_SortedArray <t_obj, t_block>::~gct_SortedArray () TL_CAN_THROW
    {
    DelAll ();
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline gct_SortedArray <t_obj, t_block> &
  gct_SortedArray <t_obj, t_block>::operator =
  (
  const gct_SortedArray & co_asgn
  )
    {
    if (& co_asgn != this)
      {
      DelAll ();
      CopyFrom (co_asgn);
      }

    return * this;
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline bool gct_SortedArray <t_obj, t_block>::IsEmpty () const
    {
    return this-> GetItemSize () == 0;
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline TL_TYPENAME gct_SortedArray <t_obj, t_block>::t_Length
  gct_SortedArray <t_obj, t_block>::GetMaxLen () const
    {
    return this-> GetMaxItemSize ();
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline TL_TYPENAME gct_SortedArray <t_obj, t_block>::t_Length
  gct_SortedArray <t_obj, t_block>::GetLen () const
    {
    return this-> GetItemSize ();
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline TL_TYPENAME gct_SortedArray <t_obj, t_block>::t_Position
  gct_SortedArray <t_obj, t_block>::First () const
    {
    return this-> GetItemSize () != 0 ? 1 : 0;
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline TL_TYPENAME gct_SortedArray <t_obj, t_block>::t_Position
  gct_SortedArray <t_obj, t_block>::Last () const
    {
    return GetLen ();
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline TL_TYPENAME gct_SortedArray <t_obj, t_block>::t_Position
  gct_SortedArray <t_obj, t_block>::Next
  (
  t_Position o_pos
  ) const
    {
    TL_ASSERT (o_pos != 0);

    return (t_Position) (o_pos < Last () ? o_pos + 1 : 0);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline TL_TYPENAME gct_SortedArray <t_obj, t_block>::t_Position
  gct_SortedArray <t_obj, t_block>::Prev
  (
  t_Position o_pos
  ) const
    {
    TL_ASSERT (o_pos != 0);

    return (t_Position) (o_pos - 1);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline TL_TYPENAME gct_SortedArray <t_obj, t_block>::t_Position
  gct_SortedArray <t_obj, t_block>::Nth
  (
  t_Length u_idx
  ) const
    {
    TL_ASSERT (u_idx > 0);
    TL_ASSERT (u_idx <= GetLen ());

    return u_idx;
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline TL_TYPENAME gct_SortedArray <t_obj, t_block>::t_Object *
  gct_SortedArray <t_obj, t_block>::GetObj
  (
  t_Position o_pos
  ) const
    {
    return (t_Object *) Node (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline TL_TYPENAME gct_SortedArray <t_obj, t_block>::t_Position
  gct_SortedArray <t_obj, t_block>::AddObj
  (
  const t_Object * po_obj
  )
    {
    return AddObjAfter (Before (po_obj), po_obj);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline TL_TYPENAME gct_SortedArray <t_obj, t_block>::t_Position
  gct_SortedArray <t_obj, t_block>::AddObjBefore
  (
  t_Position o_pos,
  const t_Object * po_obj
  )
    {
    if (o_pos != 0)
      return AddObjAfter (Prev (o_pos), po_obj);
    else
      return AddObjAfter (Last (), po_obj);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  TL_TYPENAME gct_SortedArray <t_obj, t_block>::t_Position
  gct_SortedArray <t_obj, t_block>::AddObjAfter
  (
  t_Position o_pos,
  const t_Object * po_obj
  )
    {
    TL_ASSERT (po_obj != 0);
    TL_ASSERT ((o_pos == 0) || (! (* po_obj < * GetObj (o_pos))));
    TL_ASSERT ((o_pos == GetLen ()) || (! (* GetObj (o_pos + 1) < * po_obj)));

    void * pv = this-> InsertItems (o_pos, 1);

    TL_TRY
      (
      new (pv) gct_SortedArrayNode <t_obj> (* po_obj);
      )
    TL_CATCH
      (
      this-> DeleteItems (o_pos, 1);
      throw;
      )

    return (t_Position) (o_pos + 1);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  void gct_SortedArray <t_obj, t_block>::AppendObj
  (
  const t_Object *,
  t_Length
  )
    {
    TL_ASSERT (false);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  void gct_SortedArray <t_obj, t_block>::TruncateObj
  (
  t_Length o_count
  )
    {
    TL_ASSERT (o_count <= GetLen ());

    t_Position o_pos = GetLen ();
    t_Size o_decCount = o_count;

    TL_TRY
      (
      while (o_count -- > 0)
        delete Node (o_pos --);
      )
    TL_CATCH
      (
      this-> SetItemSize (o_pos);
      throw;
      )

    this-> DecItemSize (o_decCount);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  TL_TYPENAME gct_SortedArray <t_obj, t_block>::t_Position
  gct_SortedArray <t_obj, t_block>::DelObj
  (
  t_Position o_pos
  )
    {
    TL_TRY
      (
      delete Node (o_pos);
      )
    TL_CATCH
      (
      FreeObj (o_pos);
      throw;
      )

    return FreeObj (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  void gct_SortedArray <t_obj, t_block>::DelAll ()
    {
    t_Position o_pos = GetLen ();

    TL_TRY
      (
      while (o_pos > 0)
        delete Node (o_pos --);
      )
    TL_CATCH
      (
      this-> SetItemSize (o_pos);
      DelAll ();
      throw;
      )

    this-> SetItemSize (0);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline TL_TYPENAME gct_SortedArray <t_obj, t_block>::t_Position
  gct_SortedArray <t_obj, t_block>::FreeObj
  (
  t_Position o_pos
  )
    {
    this-> DeleteItems (o_pos - 1, 1);

    return (t_Position) (o_pos <= Last () ? o_pos : 0);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline void gct_SortedArray <t_obj, t_block>::FreeAll ()
    {
    this-> SetItemSize (0);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline void gct_SortedArray <t_obj, t_block>::SetPageSize
  (
  t_Size o_size
  )
    {
    this-> AlignPageSize (sizeof (gct_SortedArrayNode <t_obj>), o_size);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  TL_TYPENAME gct_SortedArray <t_obj, t_block>::t_Position
  gct_SortedArray <t_obj, t_block>::Before
  (
  const t_Object * po_obj
  ) const
    {
    TL_ASSERT (po_obj != 0);

    t_Position o_min = 1;
    t_Position o_mid;
    t_Position o_max = GetLen ();

    if ((o_max == 0) || (* po_obj < * GetObj (o_min)))
      return 0;
    else
      if (! (* po_obj < * GetObj (o_max)))
        return o_max;
      else
        for (;;)
          {
          // * GetObj (o_min) <= * po_obj < * GetObj (o_max)

          o_mid = o_min + ((o_max - o_min) / 2);

          if (o_mid == o_min)
            return o_mid;

          if (* po_obj < * GetObj (o_mid))
            o_max = o_mid;
          else
            o_min = o_mid;
          }
    }

//---------------------------------------------------------------------------

template <class t_obj>
  class TL_TM_EXPORT gct_SortedArrayRef
    {
    t_obj *              po_Obj;

  public:
                         operator t_obj * () const { return po_Obj; }
    bool                 operator < (const gct_SortedArrayRef & co_comp) const
                           { return * po_Obj < * co_comp. po_Obj; }
    };

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  class TL_TM_EXPORT gct_FixItemSortedArray:
    public gct_SortedArray <t_obj, gct_FixItemBlock
      <t_block, sizeof (gct_SortedArrayNode <t_obj>)> >
    {
    };

//---------------------------------------------------------------------------

#define SORTEDARRAY_DCL(StoreSpec)                                            \
  template <class t_obj>                                                      \
    class TL_TM_EXPORT g ## StoreSpec ## SortedArray: public gct_ExtContainer \
      <gct_FixItemSortedArray <t_obj, StoreSpec ## Block> > { };

#define PTR_SORTEDARRAY_DCL(StoreSpec)                                        \
  template <class t_obj>                                                      \
    class TL_TM_EXPORT g ## StoreSpec ## PtrSortedArray:                      \
      public gct_PtrContainer <t_obj, g ## StoreSpec ## SortedArray           \
        <gct_SortedArrayRef <t_obj> > > { };

#define SORTEDARRAY_DCLS(Obj)            \
  SORTEDARRAY_DCL (ct_ ## Obj ## _)      \
  SORTEDARRAY_DCL (ct_ ## Obj ## 8)      \
  SORTEDARRAY_DCL (ct_ ## Obj ## 16)     \
  SORTEDARRAY_DCL (ct_ ## Obj ## 32)

#define PTR_SORTEDARRAY_DCLS(Obj)        \
  PTR_SORTEDARRAY_DCL (ct_ ## Obj ## _)  \
  PTR_SORTEDARRAY_DCL (ct_ ## Obj ## 8)  \
  PTR_SORTEDARRAY_DCL (ct_ ## Obj ## 16) \
  PTR_SORTEDARRAY_DCL (ct_ ## Obj ## 32)

#endif
