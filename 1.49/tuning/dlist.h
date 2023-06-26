
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
// File tuning/dlist.h

#ifndef TUNING_DLIST_H
#define TUNING_DLIST_H

#include "tuning/blockstore.h"
#include "tuning/extcontainer.h"
#include "tuning/sys/cmemory.hpp"

//---------------------------------------------------------------------------

template <class t_obj, class t_ptr>
  class TL_TM_EXPORT gct_DListNode
    {
  public:
    t_obj                o_Obj;
    t_ptr                o_Prev;
    t_ptr                o_Next;

    inline               gct_DListNode () { }
    inline               gct_DListNode (const t_obj & o_obj): o_Obj (o_obj) { }

    static inline void * operator new (size_t, void * pv) { return pv; }
    static inline void   operator delete (void *, void *) { }
    static inline void   operator delete (void *) { }
    };

//---------------------------------------------------------------------------

template <class t_obj, class t_store>
  class TL_TM_EXPORT gct_DList
    {
  public:
    typedef TL_TYPENAME t_store::t_Size     t_Length;
    typedef TL_TYPENAME t_store::t_Position t_Position;
    typedef t_obj                           t_Object;

  private:
    t_Position           o_First;
    t_Length             o_Length;

    inline gct_DListNode <t_Object, t_Position> * Node (t_Position o_pos) const
      { TL_ASSERT (o_pos != 0);
        return (gct_DListNode <t_Object, t_Position> *) o_Store. AddrOf (o_pos); }

    t_Position           NewNode (t_Position, t_Position, const t_obj *);
    void                 CopyFrom (const gct_DList & co_copy);

  protected:
    t_store              o_Store;

    inline t_Position    FirstForSearch (const t_Object * po_obj) const;
    inline t_Position    LastForSearch (const t_Object * po_obj) const;
    inline t_Position    NextForSearch (t_Position o_pos,
                           const t_Object * po_obj) const;
    inline t_Position    PrevForSearch (t_Position o_pos,
                           const t_Object * po_obj) const;

  public:
    inline               gct_DList ();
    inline               gct_DList (const gct_DList & co_init);
    inline               ~gct_DList () TL_CAN_THROW;
    inline gct_DList &   operator = (const gct_DList & co_asgn);
    void                 Swap (gct_DList & co_swap);

    inline bool          IsEmpty () const;
    inline t_Length      GetLen () const;

    inline t_Position    First () const;
    inline t_Position    Last () const;
    inline t_Position    Next (t_Position o_pos) const;
    inline t_Position    Prev (t_Position o_pos) const;
    t_Position           Nth (t_Length u_idx) const;

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

    t_Position           FreeObj (t_Position o_pos);
    void                 FreeAll ();

    inline t_store *     GetStore ();
    };

//---------------------------------------------------------------------------

template <class t_obj, class t_store>
  TL_TYPENAME gct_DList <t_obj, t_store>::t_Position
  gct_DList <t_obj, t_store>::NewNode
  (
  t_Position o_prev,
  t_Position o_next,
  const t_obj * po_obj
  )
    {
    typedef gct_DListNode <t_Object, t_Position> t_DListNode;
    gct_DListNode <t_Object, t_Position> * pco_node;
    t_Position o_new =
      o_Store. Alloc (sizeof (t_DListNode));
    void * pv = o_Store. AddrOf (o_new);

    TL_ASSERT (pv != 0);

    TL_TRY
      (
      if (po_obj != 0)
        pco_node = new (pv) t_DListNode (* po_obj);
      else
        pco_node = new (pv) t_DListNode;
      )
    TL_CATCH
      (
      o_Length --;
      o_Store. Free (o_new);
      throw;
      )

    if (o_prev != 0)
      {
      pco_node-> o_Prev = o_prev;
      pco_node-> o_Next = o_next;
      Node (o_prev)-> o_Next = o_new;
      Node (o_next)-> o_Prev = o_new;
      }
    else
      pco_node-> o_Prev = pco_node-> o_Next = o_new;

    return o_new;
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_store>
  void gct_DList <t_obj, t_store>::CopyFrom
  (
  const gct_DList & co_copy
  )
    {
    TL_ASSERT (o_Length == 0);

    t_Position o_thisPos = 0;

    TL_TRY
      (
      for (t_Position o_copyPos = co_copy. First ();
           o_copyPos != 0;
           o_copyPos = co_copy. Next (o_copyPos))
        o_thisPos = AddObjAfter (o_thisPos, co_copy. GetObj (o_copyPos));
      )
    TL_CATCH
      (
      DelAll ();
      throw;
      )
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_store>
  inline TL_TYPENAME gct_DList <t_obj, t_store>::t_Position
  gct_DList <t_obj, t_store>::FirstForSearch
  (
  const t_Object *
  ) const
    {
    return First ();
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_store>
  inline TL_TYPENAME gct_DList <t_obj, t_store>::t_Position
  gct_DList <t_obj, t_store>::LastForSearch
  (
  const t_Object *
  ) const
    {
    return Last ();
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_store>
  inline TL_TYPENAME gct_DList <t_obj, t_store>::t_Position
  gct_DList <t_obj, t_store>::NextForSearch
  (
  t_Position o_pos,
  const t_Object *
  ) const
    {
    return Next (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_store>
  inline TL_TYPENAME gct_DList <t_obj, t_store>::t_Position
  gct_DList <t_obj, t_store>::PrevForSearch
  (
  t_Position o_pos,
  const t_Object *
  ) const
    {
    return Prev (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_store>
  inline gct_DList <t_obj, t_store>::gct_DList ()
    {
    o_Length = 0;
    o_First = 0;
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_store>
  inline gct_DList <t_obj, t_store>::gct_DList
  (
  const gct_DList & co_init
  )
    {
    o_Length = 0;
    o_First = 0;
    CopyFrom (co_init);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_store>
  inline gct_DList <t_obj, t_store>::~gct_DList () TL_CAN_THROW
    {
    DelAll ();
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_store>
  inline gct_DList <t_obj, t_store> &
  gct_DList <t_obj, t_store>::operator =
  (
  const gct_DList & co_asgn
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

template <class t_obj, class t_store>
  void gct_DList <t_obj, t_store>::Swap
  (
  gct_DList & co_swap
  )
    {
    tl_SwapObj (o_First, co_swap. o_First);
    tl_SwapObj (o_Length, co_swap. o_Length);
    o_Store. Swap (co_swap. o_Store);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_store>
  inline bool gct_DList <t_obj, t_store>::IsEmpty () const
    {
    return o_Length == 0;
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_store>
  inline TL_TYPENAME gct_DList <t_obj, t_store>::t_Length
  gct_DList <t_obj, t_store>::GetLen () const
    {
    return o_Length;
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_store>
  inline TL_TYPENAME gct_DList <t_obj, t_store>::t_Position
  gct_DList <t_obj, t_store>::First () const
    {
    return o_First;
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_store>
  inline TL_TYPENAME gct_DList <t_obj, t_store>::t_Position
  gct_DList <t_obj, t_store>::Last () const
    {
    return o_First != (t_Position) 0 ? Node (o_First)-> o_Prev : (t_Position) 0;
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_store>
  inline TL_TYPENAME gct_DList <t_obj, t_store>::t_Position
  gct_DList <t_obj, t_store>::Next
  (
  t_Position o_pos
  ) const
    {
    return (o_pos = Node (o_pos)-> o_Next) != o_First ? o_pos : (t_Position) 0;
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_store>
  inline TL_TYPENAME gct_DList <t_obj, t_store>::t_Position
  gct_DList <t_obj, t_store>::Prev
  (
  t_Position o_pos
  ) const
    {
    return o_pos != o_First ? Node (o_pos)-> o_Prev : (t_Position) 0;
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_store>
  TL_TYPENAME gct_DList <t_obj, t_store>::t_Position
  gct_DList <t_obj, t_store>::Nth
  (
  t_Length u_idx
  ) const
    {
    TL_ASSERT (u_idx > 0);
    TL_ASSERT (u_idx <= GetLen ());

    t_Position o_pos = First ();

    while (-- u_idx > 0)
      o_pos = Node (o_pos)-> o_Next;

    return o_pos;
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_store>
  inline TL_TYPENAME gct_DList <t_obj, t_store>::t_Object *
  gct_DList <t_obj, t_store>::GetObj
  (
  t_Position o_pos
  ) const
    {
    return & Node (o_pos)-> o_Obj;
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_store>
  inline TL_TYPENAME gct_DList <t_obj, t_store>::t_Position
  gct_DList <t_obj, t_store>::AddObj
  (
  const t_Object * po_obj
  )
    {
    return AddObjAfter (Last (), po_obj);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_store>
  inline TL_TYPENAME gct_DList <t_obj, t_store>::t_Position
  gct_DList <t_obj, t_store>::AddObjBefore
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

template <class t_obj, class t_store>
  TL_TYPENAME gct_DList <t_obj, t_store>::t_Position
  gct_DList <t_obj, t_store>::AddObjAfter
  (
  t_Position o_pos,
  const t_Object * po_obj
  )
    {
    o_Length ++;

    if (o_pos != 0)
      return NewNode (o_pos, Node (o_pos)-> o_Next, po_obj);
    else
      if (o_First != 0)
        return o_First = NewNode (Node (o_First)-> o_Prev, o_First, po_obj);
      else
        return o_First = NewNode (0, 0, po_obj);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_store>
  void gct_DList <t_obj, t_store>::AppendObj
  (
  const t_Object * po_obj,
  t_Length o_count
  )
    {
    while (o_count -- > 0)
      AddObjAfter (Last (), po_obj);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_store>
  void gct_DList <t_obj, t_store>::TruncateObj
  (
  t_Length o_count
  )
    {
    TL_ASSERT (o_count <= GetLen ());

    while (o_count -- > 0)
      DelObj (Last ());
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_store>
  TL_TYPENAME gct_DList <t_obj, t_store>::t_Position
  gct_DList <t_obj, t_store>::DelObj
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

template <class t_obj, class t_store>
  void gct_DList <t_obj, t_store>::DelAll ()
    {
    t_Position o_pos = First ();

    if (o_Store. CanFreeAll ())
      {
      t_Length o_rest = o_Length;
      gct_DListNode <t_Object, t_Position> * po_node;

      TL_TRY
        (
        while (o_rest != 0)
          {
          o_rest --;
          po_node = Node (o_pos);
          delete po_node; // May throw
          o_pos = po_node-> o_Next;
          }
        )
      TL_CATCH
        (
        t_Position o_next = Next (o_pos);

        while (First () != o_next)
          FreeObj (First ());

        DelAll ();
        throw;
        )

      o_Store. FreeAll ();
      }
    else
      {
      t_Position o_next = 0;
      #if defined TL_EXCEPTION
      t_Position o_last = Last ();
      #endif
      gct_DListNode <t_Object, t_Position> * po_node;

      TL_TRY
        (
        while (o_Length != 0)
          {
          o_Length --;
          po_node = Node (o_pos);
          o_next = po_node-> o_Next;
          delete po_node; // May throw
          o_Store. Free (o_pos);
          o_pos = o_next;
          }
        )
      TL_CATCH
        (
        o_Store. Free (o_pos);

        if (o_Length == 0)
          {
          o_First = 0;
          }
        else
          {
          o_First = o_next;
          Node (o_last)->  o_Next = o_First;
          Node (o_First)-> o_Prev = o_last;
          }

        DelAll ();
        throw;
        )
      }

    o_Length = 0;
    o_First = 0;
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_store>
  TL_TYPENAME gct_DList <t_obj, t_store>::t_Position
  gct_DList <t_obj, t_store>::FreeObj
  (
  t_Position o_pos
  )
    {
    TL_ASSERT (o_Length != 0);

    o_Length --;
    t_Position o_next = Next (o_pos);

    if (o_pos == o_First)
      o_First = o_next;

    gct_DListNode <t_Object, t_Position> * po_node = Node (o_pos);

    Node (po_node-> o_Prev)-> o_Next = po_node-> o_Next;
    Node (po_node-> o_Next)-> o_Prev = po_node-> o_Prev;
    o_Store. Free (o_pos);
    return o_next;
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_store>
  void gct_DList <t_obj, t_store>::FreeAll ()
    {
    if (o_Store. CanFreeAll ())
      {
      o_Store. FreeAll ();
      o_Length = 0;
      }
    else
      {
      t_Position o_pos = First ();
      t_Position o_next;

      while (o_Length != 0)
        {
        o_Length --;
        o_next = Node (o_pos)-> o_Next;
        o_Store. Free (o_pos);
        o_pos = o_next;
        }
      }

    o_First = 0;
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_store>
  inline t_store * gct_DList <t_obj, t_store>::GetStore ()
    {
    return & o_Store;
    }

//---------------------------------------------------------------------------

#define DLIST_DCL(StoreSpec)                                                  \
  template <class t_obj>                                                      \
    class TL_TM_EXPORT g ## StoreSpec ## DList:                               \
      public gct_ExtContainer <gct_DList <t_obj, StoreSpec ## Store> > { };

#define BLOCK_DLIST_DCL(StoreSpec)                                            \
  template <class t_obj>                                                      \
    class TL_TM_EXPORT g ## StoreSpec ## BlockDList:                          \
      public gct_ExtContainer <gct_DList <t_obj, StoreSpec ## BlockStore> > {};

#define PTR_DLIST_DCL(StoreSpec)                                              \
  template <class t_obj>                                                      \
    class TL_TM_EXPORT g ## StoreSpec ## PtrDList: public                     \
      gct_PtrContainer <t_obj, g ## StoreSpec ## DList <void *> > { };

#define BLOCKPTR_DLIST_DCL(StoreSpec)                                         \
  template <class t_obj>                                                      \
    class TL_TM_EXPORT g ## StoreSpec ## BlockPtrDList: public                \
      gct_PtrContainer <t_obj, g ## StoreSpec ## BlockDList <void *> > { };

#define DLIST_DCLS(Obj)               \
  DLIST_DCL (ct_ ## Obj ## _)         \
  DLIST_DCL (ct_ ## Obj ## 8)         \
  DLIST_DCL (ct_ ## Obj ## 16)        \
  DLIST_DCL (ct_ ## Obj ## 32)

#define BLOCK_DLIST_DCLS(Obj)         \
  BLOCK_DLIST_DCL (ct_ ## Obj ## _)   \
  BLOCK_DLIST_DCL (ct_ ## Obj ## 8)   \
  BLOCK_DLIST_DCL (ct_ ## Obj ## 16)  \
  BLOCK_DLIST_DCL (ct_ ## Obj ## 32)

#define PTR_DLIST_DCLS(Obj)           \
  PTR_DLIST_DCL (ct_ ## Obj ## _)     \
  PTR_DLIST_DCL (ct_ ## Obj ## 8)     \
  PTR_DLIST_DCL (ct_ ## Obj ## 16)    \
  PTR_DLIST_DCL (ct_ ## Obj ## 32)

#define BLOCKPTR_DLIST_DCLS(Obj)        \
  BLOCKPTR_DLIST_DCL (ct_ ## Obj ## _)  \
  BLOCKPTR_DLIST_DCL (ct_ ## Obj ## 8)  \
  BLOCKPTR_DLIST_DCL (ct_ ## Obj ## 16) \
  BLOCKPTR_DLIST_DCL (ct_ ## Obj ## 32)

#endif
