
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
// File tuning/hashtable.h

#ifndef TUNING_HASHTABLE_H
#define TUNING_HASHTABLE_H

#include "tuning/array.h"
#include "tuning/sys/cmemory.hpp"

//---------------------------------------------------------------------------

const unsigned       cu_HashPrime1  =  1013;
const unsigned       cu_HashPrime2  =  2039;
const unsigned       cu_HashPrime4  =  4079;
const unsigned       cu_HashPrime8  =  8179;
const unsigned       cu_HashPrime16 = 16369;

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  class TL_TM_EXPORT gct_HashTable;

template <class t_block>
  class TL_TM_EXPORT gct_HashTablePosition
    {
  public:
    TL_TYPENAME t_block::t_Size o_Global;
    TL_TYPENAME t_block::t_Size o_Local;

                         gct_HashTablePosition (TL_TYPENAME t_block::t_Size o_global,
                           TL_TYPENAME t_block::t_Size o_local)
                           { o_Global = o_global; o_Local  = o_local; }

    inline               gct_HashTablePosition (int i = 0);

    inline bool          operator == (int i) const;
    inline bool          operator != (int i) const;
    };

//---------------------------------------------------------------------------

template <class t_block>
  inline gct_HashTablePosition <t_block>::gct_HashTablePosition
  (
  int TL_DEBUG_CODE (i)
  )
    {
    TL_ASSERT (i == 0);

    o_Global = (TL_TYPENAME t_block::t_Size) 0;
    o_Local  = (TL_TYPENAME t_block::t_Size) 0;
    }

//---------------------------------------------------------------------------

template <class t_block>
  inline bool gct_HashTablePosition <t_block>::operator ==
  (
  int TL_DEBUG_CODE (i)
  ) const
    {
    TL_ASSERT (i == 0);

    return o_Local == (TL_TYPENAME t_block::t_Size) 0;
    }

//---------------------------------------------------------------------------

template <class t_block>
  inline bool gct_HashTablePosition <t_block>::operator !=
  (
  int TL_DEBUG_CODE (i)
  ) const
    {
    TL_ASSERT (i == 0);

    return o_Local != (TL_TYPENAME t_block::t_Size) 0;
    }

//===========================================================================

template <class t_obj, class t_block>
  class TL_TM_EXPORT gct_HashTable
    {
  public:
    typedef TL_TYPENAME t_block::t_Size t_Length;
    typedef gct_HashTablePosition <t_block> t_Position;
    typedef t_obj        t_Object;

  private:
    typedef gct_FixItemArray <t_obj, t_block> t_LocalArray;
    gct_FixItemArray <t_LocalArray, t_block> co_Array;
    t_Length             o_Length;

  protected:
    t_Position           FirstForSearch (const t_Object * po_obj) const;
    t_Position           LastForSearch (const t_Object * po_obj) const;
    t_Position           NextForSearch (t_Position o_pos,
                           const t_Object * po_obj) const;
    t_Position           PrevForSearch (t_Position o_pos,
                           const t_Object * po_obj) const;

  public:
                         gct_HashTable ();
    void                 Swap (gct_HashTable & co_swap);

    inline bool          IsEmpty () const;
    inline t_Length      GetLen () const;

    t_Position           First () const;
    t_Position           Last () const;
    t_Position           Next (t_Position o_pos) const;
    t_Position           Prev (t_Position o_pos) const;
    t_Position           Nth (t_Length u_idx) const;

    inline t_Object *    GetObj (t_Position o_pos) const;

    t_Position           AddObj (const t_Object * po_obj);
    t_Position           AddObjBefore (t_Position o_pos,
                           const t_Object * po_obj);
    t_Position           AddObjAfter (t_Position o_pos,
                           const t_Object * po_obj);

    void                 AppendObj (const t_Object * po_obj = 0,
                           t_Length o_count = 1);
    void                 TruncateObj (t_Length o_count = 1);

    t_Position           DelObj (t_Position o_pos);
    void                 DelAll ();

    t_Position           FreeObj (t_Position o_pos);
    void                 FreeAll ();

    void                 SetHashSize (t_Length o_size);
    inline t_Length      GetHashSize () const;
    };

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  TL_TYPENAME gct_HashTable <t_obj, t_block>::t_Position
  gct_HashTable <t_obj, t_block>::FirstForSearch
  (
  const t_Object * po_obj
  ) const
    {
    TL_ASSERT (po_obj != 0);

    TL_TYPENAME t_block::t_Size o_global = (TL_TYPENAME t_block::t_Size)
      (((t_UInt) po_obj-> GetHash ()) % GetHashSize () + 1);

    return t_Position (o_global, co_Array. GetObj (o_global)-> First ());
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  TL_TYPENAME gct_HashTable <t_obj, t_block>::t_Position
  gct_HashTable <t_obj, t_block>::LastForSearch
  (
  const t_Object * po_obj
  ) const
    {
    TL_ASSERT (po_obj != 0);

    TL_TYPENAME t_block::t_Size o_global = (TL_TYPENAME t_block::t_Size)
      (((t_UInt) po_obj-> GetHash ()) % GetHashSize () + 1);

    return t_Position (o_global, co_Array. GetObj (o_global)-> Last ());
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  TL_TYPENAME gct_HashTable <t_obj, t_block>::t_Position
  gct_HashTable <t_obj, t_block>::NextForSearch
  (
  t_Position o_pos,
  const t_Object *
  ) const
    {
    return t_Position (o_pos. o_Global,
      co_Array. GetObj (o_pos. o_Global)-> Next (o_pos. o_Local));
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  TL_TYPENAME gct_HashTable <t_obj, t_block>::t_Position
  gct_HashTable <t_obj, t_block>::PrevForSearch
  (
  t_Position o_pos,
  const t_Object *
  ) const
    {
    return t_Position (o_pos. o_Global,
      co_Array. GetObj (o_pos. o_Global)-> Prev (o_pos. o_Local));
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  gct_HashTable <t_obj, t_block>::gct_HashTable ()
    {
    o_Length = 0;
    SetHashSize (cu_HashPrime4);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  void gct_HashTable <t_obj, t_block>::Swap
  (
  gct_HashTable & co_swap
  )
    {
    co_Array. Swap (co_swap. co_Array);
    tl_SwapObj (o_Length, co_swap. o_Length);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline bool gct_HashTable <t_obj, t_block>::IsEmpty () const
    {
    return o_Length == 0;
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline TL_TYPENAME gct_HashTable <t_obj, t_block>::t_Length
  gct_HashTable <t_obj, t_block>::GetLen () const
    {
    return o_Length;
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  TL_TYPENAME gct_HashTable <t_obj, t_block>::t_Position
  gct_HashTable <t_obj, t_block>::First () const
    {
    if (IsEmpty ())
      return t_Position (0, 0);
    else
      {
      TL_TYPENAME t_block::t_Size o_global = co_Array. First ();

      while (co_Array. GetObj (o_global)-> IsEmpty ())
        o_global = co_Array. Next (o_global);

      return t_Position (o_global, co_Array. GetObj (o_global)-> First ());
      }
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  TL_TYPENAME gct_HashTable <t_obj, t_block>::t_Position
  gct_HashTable <t_obj, t_block>::Last () const
    {
    if (IsEmpty ())
      return t_Position (0, 0);
    else
      {
      TL_TYPENAME t_block::t_Size o_global = co_Array. Last ();

      while (co_Array. GetObj (o_global)-> IsEmpty ())
        o_global = co_Array. Prev (o_global);

      return t_Position (o_global, co_Array. GetObj (o_global)-> Last ());
      }
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  TL_TYPENAME gct_HashTable <t_obj, t_block>::t_Position
  gct_HashTable <t_obj, t_block>::Next
  (
  t_Position o_pos
  ) const
    {
    TL_ASSERT (o_pos != 0);

    o_pos. o_Local =
      co_Array. GetObj (o_pos. o_Global)-> Next (o_pos. o_Local);

    if (o_pos. o_Local != 0)
      return o_pos;
    else
      {
      TL_TYPENAME t_block::t_Size o_global = co_Array. Next (o_pos. o_Global);

      while (o_global != 0)
        if (co_Array. GetObj (o_global)-> IsEmpty ())
          o_global = co_Array. Next (o_global);
        else
          return t_Position (o_global, co_Array. GetObj (o_global)-> First ());

      return t_Position (0, 0);
      }
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  TL_TYPENAME gct_HashTable <t_obj, t_block>::t_Position
  gct_HashTable <t_obj, t_block>::Prev
  (
  t_Position o_pos
  ) const
    {
    TL_ASSERT (o_pos != 0);

    o_pos. o_Local =
      co_Array. GetObj (o_pos. o_Global)-> Prev (o_pos. o_Local);

    if (o_pos. o_Local != 0)
      return o_pos;
    else
      {
      TL_TYPENAME t_block::t_Size o_global = co_Array. Prev (o_pos. o_Global);

      while (o_global != 0)
        if (co_Array. GetObj (o_global)-> IsEmpty ())
          o_global = co_Array. Prev (o_global);
        else
          return t_Position (o_global, co_Array. GetObj (o_global)-> Last ());

      return t_Position (0, 0);
      }
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  TL_TYPENAME gct_HashTable <t_obj, t_block>::t_Position
  gct_HashTable <t_obj, t_block>::Nth
  (
  t_Length u_idx
  ) const
    {
    TL_ASSERT (u_idx > 0);
    TL_ASSERT (u_idx <= GetLen ());

    t_Position o_pos = First ();

    while (-- u_idx > 0)
      o_pos = Next (o_pos);

    return o_pos;
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline TL_TYPENAME gct_HashTable <t_obj, t_block>::t_Object *
  gct_HashTable <t_obj, t_block>::GetObj
  (
  t_Position o_pos
  ) const
    {
    return co_Array. GetObj (o_pos. o_Global)-> GetObj (o_pos. o_Local);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  TL_TYPENAME gct_HashTable <t_obj, t_block>::t_Position
  gct_HashTable <t_obj, t_block>::AddObj
  (
  const t_Object * po_obj
  )
    {
    TL_ASSERT (po_obj != 0);

    TL_TYPENAME t_block::t_Size o_global;
    TL_TYPENAME t_block::t_Size o_local;

    TL_TRY
      (
      o_global = (TL_TYPENAME t_block::t_Size)
        (((t_UInt) po_obj-> GetHash ()) % GetHashSize () + 1);
      o_local = co_Array. GetObj (o_global)-> AddObj (po_obj);
      )
    TL_CATCH
      (
      throw;
      )

    o_Length ++;
    return t_Position (o_global, o_local);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  TL_TYPENAME gct_HashTable <t_obj, t_block>::t_Position
  gct_HashTable <t_obj, t_block>::AddObjBefore
  (
  t_Position,
  const t_Object *
  )
    {
    TL_ASSERT (false);

    return t_Position (0, 0);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  TL_TYPENAME gct_HashTable <t_obj, t_block>::t_Position
  gct_HashTable <t_obj, t_block>::AddObjAfter
  (
  t_Position,
  const t_Object *
  )
    {
    TL_ASSERT (false);

    return t_Position (0, 0);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  void gct_HashTable <t_obj, t_block>::AppendObj
  (
  const t_Object *,
  t_Length
  )
    {
    TL_ASSERT (false);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  void gct_HashTable <t_obj, t_block>::TruncateObj
  (
  t_Length
  )
    {
    TL_ASSERT (false);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  TL_TYPENAME gct_HashTable <t_obj, t_block>::t_Position
  gct_HashTable <t_obj, t_block>::DelObj
  (
  t_Position o_pos
  )
    {
    o_Length --;

    if (co_Array. GetObj (o_pos. o_Global)-> DelObj (o_pos. o_Local) != 0)
      return o_pos;
    else
      return Next (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  void gct_HashTable <t_obj, t_block>::DelAll ()
    {
    for (TL_TYPENAME t_block::t_Size o_global = co_Array. First ();
         o_global != 0;
         o_global = co_Array. Next (o_global))
      {
      t_LocalArray * pco_localArray = co_Array. GetObj (o_global);

      TL_TRY
        (
        o_Length -= pco_localArray-> GetLen ();
        pco_localArray-> DelAll ();
        )
      TL_CATCH
        (
        o_Length += pco_localArray-> GetLen ();
        DelAll ();
        throw;
        )
      }

    TL_ASSERT (o_Length == 0);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  TL_TYPENAME gct_HashTable <t_obj, t_block>::t_Position
  gct_HashTable <t_obj, t_block>::FreeObj
  (
  t_Position o_pos
  )
    {
    o_Length --;

    if (co_Array. GetObj (o_pos. o_Global)-> FreeObj (o_pos. o_Local) != 0)
      return o_pos;
    else
      return Next (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  void gct_HashTable <t_obj, t_block>::FreeAll ()
    {
    o_Length = 0;

    for (TL_TYPENAME t_block::t_Size o_global = co_Array. First ();
         o_global != 0;
         o_global = co_Array. Next (o_global))
      co_Array. GetObj (o_global)-> FreeAll ();
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  void gct_HashTable <t_obj, t_block>::SetHashSize
  (
  t_Length o_size
  )
    {
    TL_ASSERT (IsEmpty ());
    TL_ASSERT (o_size != 0);

    if (co_Array. GetLen () < o_size)
      co_Array. AppendObj (0, o_size - co_Array. GetLen ());
    else
      co_Array. TruncateObj (co_Array. GetLen () - o_size);

    TL_ASSERT (co_Array. GetLen () == o_size);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_block>
  inline TL_TYPENAME gct_HashTable <t_obj, t_block>::t_Length
  gct_HashTable <t_obj, t_block>::GetHashSize () const
    {
    return co_Array. GetLen ();
    }

//---------------------------------------------------------------------------

template <class t_obj>
  class TL_TM_EXPORT gct_HashTableRef
    {
    t_obj *              po_Obj;

  public:
                         operator t_obj * () const { return po_Obj; }
    t_UInt               GetHash () const { return po_Obj-> GetHash (); }
    };

//---------------------------------------------------------------------------

#define HASHTABLE_DCL(StoreSpec)                                         \
  template <class t_obj>                                                 \
    class TL_TM_EXPORT g ## StoreSpec ## HashTable:                      \
      public gct_ExtContainer <gct_HashTable <t_obj, StoreSpec ## Block> > { };

#define PTR_HASHTABLE_DCL(StoreSpec)                                     \
  template <class t_obj>                                                 \
    class TL_TM_EXPORT g ## StoreSpec ## PtrHashTable:                   \
      public gct_PtrContainer <t_obj, g ## StoreSpec ## HashTable        \
        <gct_HashTableRef <t_obj> > > { };

#define HASHTABLE_DCLS(Obj)            \
  HASHTABLE_DCL (ct_ ## Obj ## _)      \
  HASHTABLE_DCL (ct_ ## Obj ## 8)      \
  HASHTABLE_DCL (ct_ ## Obj ## 16)     \
  HASHTABLE_DCL (ct_ ## Obj ## 32)

#define PTR_HASHTABLE_DCLS(Obj)        \
  PTR_HASHTABLE_DCL (ct_ ## Obj ## _)  \
  PTR_HASHTABLE_DCL (ct_ ## Obj ## 8)  \
  PTR_HASHTABLE_DCL (ct_ ## Obj ## 16) \
  PTR_HASHTABLE_DCL (ct_ ## Obj ## 32)

#endif
