
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
// File tuning/array.h

#ifndef TUNING_ARRAY_H
#define TUNING_ARRAY_H

#include "tuning/sys/cmemory.hpp"
#include "tuning/extcontainer.h"
#include "tuning/itemblock.h"

//---------------------------------------------------------------------------

template <class t_obj>
  class TL_TM_EXPORT gct_ArrayNode
    {
  public:
    t_obj                o_Obj;

    inline               gct_ArrayNode () { }
    inline               gct_ArrayNode (const t_obj & o_obj): o_Obj (o_obj) { }

    static inline void * operator new (size_t, void * pv) { return pv; }
    static inline void   operator delete (void *, void *) { }
    static inline void   operator delete (void *) { }
    };

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  class TL_TM_EXPORT gct_Array: public t_block
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

    inline gct_ArrayNode <t_obj> * Node (t_Position o_pos) const;
    void                 CopyFrom (const gct_Array & co_copy);

  protected:
    inline t_Position    FirstForSearch (const t_Object * po_obj) const;
    inline t_Position    LastForSearch (const t_Object * po_obj) const;
    inline t_Position    NextForSearch (t_Position o_pos,
                           const t_Object * po_obj) const;
    inline t_Position    PrevForSearch (t_Position o_pos,
                           const t_Object * po_obj) const;

  public:
    inline               gct_Array ();
    inline               gct_Array (const gct_Array & co_init);
    inline               ~gct_Array () TL_CAN_THROW;
    inline gct_Array &   operator = (const gct_Array & co_asgn);

    inline bool          IsEmpty () const;
    inline t_Length      GetMaxLen () const;
    inline t_Length      GetLen () const;

    inline t_Position    First () const;
    inline t_Position    Last () const;
    inline t_Position    Next (t_Position o_pos) const;
    inline t_Position    Prev (t_Position o_pos) const;
    inline t_Position    Nth (t_Length u_idx) const;

    inline t_Object *    GetObj (t_Position o_pos) const;

    inline t_Position    AddObj (const t_Object * po_obj = 0);
    inline t_Position    AddObjBefore (t_Position o_pos,
                           const t_Object * po_obj = 0);
    t_Position           AddObjAfter (t_Position o_pos,
                           const t_Object * po_obj = 0);

    void                 AppendObj (const t_Object * po_obj = 0,
                           t_Length o_count = 1);
    void                 TruncateObj (t_Length o_count = 1);

    t_Position           DelObj (t_Position o_pos);
    void                 DelAll ();

    inline t_Position    FreeObj (t_Position o_pos);
    inline void          FreeAll ();

    inline void          SetPageSize (t_Size o_size);
    };

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline gct_ArrayNode <t_obj> *
  gct_Array <t_obj, t_block>::Node
  (
  t_Position o_pos
  ) const
    {
    TL_ASSERT (o_pos != 0);
    TL_ASSERT (o_pos <= Last ());

    return (gct_ArrayNode <t_obj> *) this-> GetItemAddr
      ((t_Position) (o_pos - 1));
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  void gct_Array <t_obj, t_block>::CopyFrom
  (
  const gct_Array & co_copy
  )
    {
    TL_ASSERT (IsEmpty ());

    t_Position o_pos = 0;
    this-> SetItemSize (co_copy. GetItemSize ());

    TL_TRY
      (
      for (o_pos = First (); o_pos != 0; o_pos = Next (o_pos))
        new (Node (o_pos))
          gct_ArrayNode <t_obj> (* co_copy. Node (o_pos));
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
  inline TL_TYPENAME gct_Array <t_obj, t_block>::t_Position
  gct_Array <t_obj, t_block>::FirstForSearch
  (
  const t_Object *
  ) const
    {
    return First ();
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline TL_TYPENAME gct_Array <t_obj, t_block>::t_Position
  gct_Array <t_obj, t_block>::LastForSearch
  (
  const t_Object *
  ) const
    {
    return Last ();
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline TL_TYPENAME gct_Array <t_obj, t_block>::t_Position
  gct_Array <t_obj, t_block>::NextForSearch
  (
  t_Position o_pos,
  const t_Object *
  ) const
    {
    return Next (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline TL_TYPENAME gct_Array <t_obj, t_block>::t_Position
  gct_Array <t_obj, t_block>::PrevForSearch
  (
  t_Position o_pos,
  const t_Object *
  ) const
    {
    return Prev (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline gct_Array <t_obj, t_block>::gct_Array ()
    {
    this-> AlignPageSize (sizeof (gct_ArrayNode <t_obj>),
      this-> GetDefaultPageSize ());
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline gct_Array <t_obj, t_block>::gct_Array
  (
  const gct_Array & co_init
  )
    {
    this-> AlignPageSize (sizeof (gct_ArrayNode <t_obj>),
      this-> GetDefaultPageSize ());
    CopyFrom (co_init);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline gct_Array <t_obj, t_block>::~gct_Array () TL_CAN_THROW
    {
    DelAll ();
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline gct_Array <t_obj, t_block> &
  gct_Array <t_obj, t_block>::operator =
  (
  const gct_Array & co_asgn
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
  inline bool gct_Array <t_obj, t_block>::IsEmpty () const
    {
    return this-> GetItemSize () == 0;
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline TL_TYPENAME gct_Array <t_obj, t_block>::t_Length
  gct_Array <t_obj, t_block>::GetMaxLen () const
    {
    return this-> GetMaxItemSize ();
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline TL_TYPENAME gct_Array <t_obj, t_block>::t_Length
  gct_Array <t_obj, t_block>::GetLen () const
    {
    return this-> GetItemSize ();
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline TL_TYPENAME gct_Array <t_obj, t_block>::t_Position
  gct_Array <t_obj, t_block>::First () const
    {
    return this-> GetItemSize () != 0 ? 1 : 0;
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline TL_TYPENAME gct_Array <t_obj, t_block>::t_Position
  gct_Array <t_obj, t_block>::Last () const
    {
    return GetLen ();
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline TL_TYPENAME gct_Array <t_obj, t_block>::t_Position
  gct_Array <t_obj, t_block>::Next
  (
  t_Position o_pos
  ) const
    {
    TL_ASSERT (o_pos != 0);

    return (t_Position) (o_pos < Last () ? o_pos + 1 : 0);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline TL_TYPENAME gct_Array <t_obj, t_block>::t_Position
  gct_Array <t_obj, t_block>::Prev
  (
  t_Position o_pos
  ) const
    {
    TL_ASSERT (o_pos != 0);

    return (t_Position) (o_pos - 1);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline TL_TYPENAME gct_Array <t_obj, t_block>::t_Position
  gct_Array <t_obj, t_block>::Nth
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
  inline TL_TYPENAME gct_Array <t_obj, t_block>::t_Object *
  gct_Array <t_obj, t_block>::GetObj
  (
  t_Position o_pos
  ) const
    {
    return (t_Object *) Node (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline TL_TYPENAME gct_Array <t_obj, t_block>::t_Position
  gct_Array <t_obj, t_block>::AddObj
  (
  const t_Object * po_obj
  )
    {
    return AddObjAfter (Last (), po_obj);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline TL_TYPENAME gct_Array <t_obj, t_block>::t_Position
  gct_Array <t_obj, t_block>::AddObjBefore
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
  TL_TYPENAME gct_Array <t_obj, t_block>::t_Position
  gct_Array <t_obj, t_block>::AddObjAfter
  (
  t_Position o_pos,
  const t_Object * po_obj
  )
    {
    void * pv = this-> InsertItems (o_pos, 1);

    TL_TRY
      (
      if (po_obj != 0)
        new (pv) gct_ArrayNode <t_obj> (* po_obj);
      else
        new (pv) gct_ArrayNode <t_obj>;
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
  void gct_Array <t_obj, t_block>::AppendObj
  (
  const t_Object * po_obj,
  t_Length o_count
  )
    {
    t_Position o_pos = GetLen ();
    this-> IncItemSize (o_count);

    TL_TRY
      (
      if (po_obj != 0)
        while (o_count -- > 0)
          new (this-> GetItemAddr (o_pos ++))
            gct_ArrayNode <t_obj> (* po_obj);
      else
        while (o_count -- > 0)
          new (this-> GetItemAddr (o_pos ++))
            gct_ArrayNode <t_obj>;
      )
    TL_CATCH
      (
      this-> DecItemSize ((t_Size) (o_count + 1));
      throw;
      )
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  void gct_Array <t_obj, t_block>::TruncateObj
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
  TL_TYPENAME gct_Array <t_obj, t_block>::t_Position
  gct_Array <t_obj, t_block>::DelObj
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
  void gct_Array <t_obj, t_block>::DelAll ()
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
  inline TL_TYPENAME gct_Array <t_obj, t_block>::t_Position
  gct_Array <t_obj, t_block>::FreeObj
  (
  t_Position o_pos
  )
    {
    this-> DeleteItems (o_pos - 1, 1);

    return (t_Position) (o_pos <= Last () ? o_pos : 0);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline void gct_Array <t_obj, t_block>::FreeAll ()
    {
    this-> SetItemSize (0);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline void gct_Array <t_obj, t_block>::SetPageSize
  (
  t_Size o_size
  )
    {
    this-> AlignPageSize (sizeof (gct_ArrayNode <t_obj>), o_size);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  class TL_TM_EXPORT gct_FixItemArray:
    public gct_Array <t_obj, gct_FixItemBlock
      <t_block, sizeof (gct_ArrayNode <t_obj>)> >
    {
    };

//---------------------------------------------------------------------------

#define ARRAY_DCL(StoreSpec)                                                \
  template <class t_obj>                                                    \
    class TL_TM_EXPORT g ## StoreSpec ## Array: public gct_ExtContainer     \
      <gct_FixItemArray <t_obj, StoreSpec ## Block> > { };

#define PTR_ARRAY_DCL(StoreSpec)                                            \
  template <class t_obj>                                                    \
    class TL_TM_EXPORT g ## StoreSpec ## PtrArray:                          \
      public gct_PtrContainer <t_obj, g ## StoreSpec ## Array <void *> > { };

#define ARRAY_DCLS(Obj)            \
  ARRAY_DCL (ct_ ## Obj ## _)      \
  ARRAY_DCL (ct_ ## Obj ## 8)      \
  ARRAY_DCL (ct_ ## Obj ## 16)     \
  ARRAY_DCL (ct_ ## Obj ## 32)

#define PTR_ARRAY_DCLS(Obj)        \
  PTR_ARRAY_DCL (ct_ ## Obj ## _)  \
  PTR_ARRAY_DCL (ct_ ## Obj ## 8)  \
  PTR_ARRAY_DCL (ct_ ## Obj ## 16) \
  PTR_ARRAY_DCL (ct_ ## Obj ## 32)

#endif
