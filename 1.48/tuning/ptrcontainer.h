
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
// File tuning/ptrcontainer.h

#ifndef TUNING_PTRCONTAINER_H
#define TUNING_PTRCONTAINER_H

#include "tuning/compcontainer.h"

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  class TL_TM_EXPORT gct_PtrContainer:
    public gct_CompContainer <t_container>
    {
  public:
    typedef TL_TYPENAME gct_CompContainer <t_container>::t_Length   t_Length;
    typedef TL_TYPENAME gct_CompContainer <t_container>::t_Position t_Position;
    typedef TL_TYPENAME gct_CompContainer <t_container>::t_Object   t_Object;
    typedef t_obj                                                   t_RefObject;

    inline               ~gct_PtrContainer ();

    inline t_obj *       GetPtr (t_Position o_pos) const;
    inline t_obj *       GetFirstPtr () const;
    inline t_obj *       GetLastPtr () const;
    inline t_obj *       GetNextPtr (t_Position o_pos) const;
    inline t_obj *       GetPrevPtr (t_Position o_pos) const;
    inline t_obj *       GetNthPtr (t_Length u_idx) const;

    inline t_Position    AddPtr (const t_obj * po_obj);
    inline t_Position    AddPtrBefore (t_Position o_pos, const t_obj * po_obj);
    inline t_Position    AddPtrAfter (t_Position o_pos, const t_obj * po_obj);
    inline t_Position    AddPtrBeforeFirst (const t_obj * po_obj);
    inline t_Position    AddPtrAfterLast (const t_obj * po_obj);
    inline t_Position    AddPtrBeforeNth (t_Length u_idx, const t_obj * po_obj);
    inline t_Position    AddPtrAfterNth (t_Length u_idx, const t_obj * po_obj);

    inline t_Position    DelPtr (t_Position o_pos);
    inline t_Position    DelFirstPtr ();
    inline t_Position    DelLastPtr ();
    inline t_Position    DelNextPtr (t_Position o_pos);
    inline t_Position    DelPrevPtr (t_Position o_pos);
    inline t_Position    DelNthPtr (t_Length u_idx);
    inline void          DelAllPtr ();

    inline t_Position    DelPtrAndObj (t_Position o_pos);
    inline t_Position    DelFirstPtrAndObj ();
    inline t_Position    DelLastPtrAndObj ();
    inline t_Position    DelNextPtrAndObj (t_Position o_pos);
    inline t_Position    DelPrevPtrAndObj (t_Position o_pos);
    inline t_Position    DelNthPtrAndObj (t_Length u_idx);
    inline void          DelAllPtrAndObj ();

    inline bool          ContainsPtr (const t_obj * po_obj) const;
    inline t_Length      CountPtrs (const t_obj * po_obj) const;

    inline t_Position    SearchFirstPtr (const t_obj * po_obj) const;
    inline t_Position    SearchLastPtr (const t_obj * po_obj) const;
    inline t_Position    SearchNextPtr
                           (t_Position o_pos, const t_obj * po_obj) const;
    inline t_Position    SearchPrevPtr
                           (t_Position o_pos, const t_obj * po_obj) const;

    inline t_Position    AddPtrCond (const t_obj * po_obj);
    inline t_Position    AddPtrBeforeFirstCond (const t_obj * po_obj);
    inline t_Position    AddPtrAfterLastCond (const t_obj * po_obj);

    inline t_Position    DelFirstEqualPtr (const t_obj * po_obj);
    inline t_Position    DelLastEqualPtr (const t_obj * po_obj);
    inline t_Position    DelFirstEqualPtrCond (const t_obj * po_obj);
    inline t_Position    DelLastEqualPtrCond (const t_obj * po_obj);

    inline t_Position    DelFirstEqualPtrAndObj (const t_obj * po_obj);
    inline t_Position    DelLastEqualPtrAndObj (const t_obj * po_obj);
    inline t_Position    DelFirstEqualPtrAndObjCond (const t_obj * po_obj);
    inline t_Position    DelLastEqualPtrAndObjCond (const t_obj * po_obj);
    };

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline gct_PtrContainer <t_obj, t_container>::~gct_PtrContainer ()
    {
    this-> FreeAll ();
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline t_obj * gct_PtrContainer <t_obj, t_container>::GetPtr
  (
  t_Position o_pos
  ) const
    {
    return (t_obj *) * this-> GetObj (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline t_obj * gct_PtrContainer <t_obj, t_container>::GetFirstPtr () const
    {
    return (t_obj *) * this-> GetFirstObj ();
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline t_obj * gct_PtrContainer <t_obj, t_container>::GetLastPtr () const
    {
    return (t_obj *) * this-> GetLastObj ();
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline t_obj * gct_PtrContainer <t_obj, t_container>::GetNextPtr
  (
  t_Position o_pos
  ) const
    {
    return (t_obj *) * this-> GetNextObj (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline t_obj * gct_PtrContainer <t_obj, t_container>::GetPrevPtr
  (
  t_Position o_pos
  ) const
    {
    return (t_obj *) * this-> GetPrevObj (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline t_obj * gct_PtrContainer <t_obj, t_container>::GetNthPtr
  (
  t_Length u_idx
  ) const
    {
    return (t_obj *) * this-> GetNthObj (u_idx);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Position
  gct_PtrContainer <t_obj, t_container>::AddPtr
  (
  const t_obj * po_obj
  )
    {
    return this-> AddObj ((t_Object *) (void *) & po_obj);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Position
  gct_PtrContainer <t_obj, t_container>::AddPtrBefore
  (
  t_Position o_pos,
  const t_obj * po_obj
  )
    {
    return this-> AddObjBefore (o_pos, (t_Object *) (void *) & po_obj);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Position
  gct_PtrContainer <t_obj, t_container>::AddPtrAfter
  (
  t_Position o_pos,
  const t_obj * po_obj
  )
    {
    return this-> AddObjAfter (o_pos, (t_Object *) (void *) & po_obj);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Position
  gct_PtrContainer <t_obj, t_container>::AddPtrBeforeFirst
  (
  const t_obj * po_obj
  )
    {
    return this-> AddObjBeforeFirst ((t_Object *) (void *) & po_obj);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Position
  gct_PtrContainer <t_obj, t_container>::AddPtrAfterLast
  (
  const t_obj * po_obj
  )
    {
    return this-> AddObjAfterLast ((t_Object *) (void *) & po_obj);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Position
  gct_PtrContainer <t_obj, t_container>::AddPtrBeforeNth
  (
  t_Length u_idx,
  const t_obj * po_obj
  )
    {
    return this-> AddObjBeforeNth (u_idx, (t_Object *) (void *) & po_obj);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Position
  gct_PtrContainer <t_obj, t_container>::AddPtrAfterNth
  (
  t_Length u_idx,
  const t_obj * po_obj
  )
    {
    return this-> AddObjAfterNth (u_idx, (t_Object *) (void *) & po_obj);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Position
  gct_PtrContainer <t_obj, t_container>::DelPtr
  (
  t_Position o_pos
  )
    {
    return this-> FreeObj (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Position
  gct_PtrContainer <t_obj, t_container>::DelFirstPtr ()
    {
    return this-> FreeFirstObj ();
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Position
  gct_PtrContainer <t_obj, t_container>::DelLastPtr ()
    {
    return this-> FreeLastObj ();
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Position
  gct_PtrContainer <t_obj, t_container>::DelNextPtr
  (
  t_Position o_pos
  )
    {
    return this-> FreeNextObj (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Position
  gct_PtrContainer <t_obj, t_container>::DelPrevPtr
  (
  t_Position o_pos
  )
    {
    return this-> FreePrevObj (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Position
  gct_PtrContainer <t_obj, t_container>::DelNthPtr
  (
  t_Length u_idx
  )
    {
    return this-> FreeNthObj (u_idx);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline void gct_PtrContainer <t_obj, t_container>::DelAllPtr ()
    {
    this-> FreeAll ();
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Position
  gct_PtrContainer <t_obj, t_container>::DelPtrAndObj
  (
  t_Position o_pos
  )
    {
    delete GetPtr (o_pos);
    return this-> FreeObj (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Position
  gct_PtrContainer <t_obj, t_container>::DelFirstPtrAndObj ()
    {
    return DelPtrAndObj (this-> First ());
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Position
  gct_PtrContainer <t_obj, t_container>::DelLastPtrAndObj ()
    {
    return DelPtrAndObj (this-> Last ());
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Position
  gct_PtrContainer <t_obj, t_container>::DelNextPtrAndObj
  (
  t_Position o_pos
  )
    {
    return DelPtrAndObj (this-> Next (o_pos));
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Position
  gct_PtrContainer <t_obj, t_container>::DelPrevPtrAndObj
  (
  t_Position o_pos
  )
    {
    return DelPtrAndObj (this-> Prev (o_pos));
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Position
  gct_PtrContainer <t_obj, t_container>::DelNthPtrAndObj
  (
  t_Length u_idx
  )
    {
    return this-> DelPtrAndObj (this-> Nth (u_idx));
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline void gct_PtrContainer <t_obj, t_container>::DelAllPtrAndObj ()
    {
    for (t_Position o_pos = this-> First (); o_pos != 0; o_pos = this-> Next (o_pos))
      delete GetPtr (o_pos);

    this-> FreeAll ();
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline bool gct_PtrContainer <t_obj, t_container>::ContainsPtr
  (
  const t_obj * po_obj
  ) const
    {
    return this-> ContainsObj ((t_Object *) (void *) & po_obj);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Length
  gct_PtrContainer <t_obj, t_container>::CountPtrs
  (
  const t_obj * po_obj
  ) const
    {
    return this-> CountObjs ((t_Object *) (void *) & po_obj);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Position
  gct_PtrContainer <t_obj, t_container>::SearchFirstPtr
  (
  const t_obj * po_obj
  ) const
    {
    return this-> SearchFirstObj ((t_Object *) (void *) & po_obj);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Position
  gct_PtrContainer <t_obj, t_container>::SearchLastPtr
  (
  const t_obj * po_obj
  ) const
    {
    return this-> SearchLastObj ((t_Object *) (void *) & po_obj);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Position
  gct_PtrContainer <t_obj, t_container>::SearchNextPtr
  (
  t_Position o_pos,
  const t_obj * po_obj
  ) const
    {
    return this-> SearchNextObj (o_pos, (t_Object *) (void *) & po_obj);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Position
  gct_PtrContainer <t_obj, t_container>::SearchPrevPtr
  (
  t_Position o_pos,
  const t_obj * po_obj
  ) const
    {
    return this-> SearchPrevObj (o_pos, (t_Object *) (void *) & po_obj);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Position
  gct_PtrContainer <t_obj, t_container>::AddPtrCond
  (
  const t_obj * po_obj
  )
    {
    return this-> AddObjCond ((t_Object *) (void *) & po_obj);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Position
  gct_PtrContainer <t_obj, t_container>::AddPtrBeforeFirstCond
  (
  const t_obj * po_obj
  )
    {
    return this-> AddObjBeforeFirstCond ((t_Object *) (void *) & po_obj);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Position
  gct_PtrContainer <t_obj, t_container>::AddPtrAfterLastCond
  (
  const t_obj * po_obj
  )
    {
    return this-> AddObjAfterLastCond ((t_Object *) (void *) & po_obj);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Position
  gct_PtrContainer <t_obj, t_container>::DelFirstEqualPtr
  (
  const t_obj * po_obj
  )
    {
    return this-> DelFirstEqualObj ((t_Object *) (void *) & po_obj);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Position
  gct_PtrContainer <t_obj, t_container>::DelLastEqualPtr
  (
  const t_obj * po_obj
  )
    {
    return this-> DelLastEqualObj ((t_Object *) (void *) & po_obj);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Position
  gct_PtrContainer <t_obj, t_container>::DelFirstEqualPtrCond
  (
  const t_obj * po_obj
  )
    {
    return this-> DelFirstEqualObjCond ((t_Object *) (void *) & po_obj);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Position
  gct_PtrContainer <t_obj, t_container>::DelLastEqualPtrCond
  (
  const t_obj * po_obj
  )
    {
    return this-> DelLastEqualObjCond ((t_Object *) (void *) & po_obj);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Position
  gct_PtrContainer <t_obj, t_container>::DelFirstEqualPtrAndObj
  (
  const t_obj * po_obj
  )
    {
    return DelPtrAndObj (SearchFirstPtr (po_obj));
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Position
  gct_PtrContainer <t_obj, t_container>::DelLastEqualPtrAndObj
  (
  const t_obj * po_obj
  )
    {
    return DelPtrAndObj (SearchLastPtr (po_obj));
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Position
  gct_PtrContainer <t_obj, t_container>::DelFirstEqualPtrAndObjCond
  (
  const t_obj * po_obj
  )
    {
    t_Position o_pos = SearchFirstPtr (po_obj);
    return (o_pos == 0) ? (t_Position) 0 : DelPtrAndObj (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_container>
  inline TL_TYPENAME gct_PtrContainer <t_obj, t_container>::t_Position
  gct_PtrContainer <t_obj, t_container>::DelLastEqualPtrAndObjCond
  (
  const t_obj * po_obj
  )
    {
    t_Position o_pos = SearchLastPtr (po_obj);
    return (o_pos == 0) ? (t_Position) 0 : DelPtrAndObj (o_pos);
    }

#endif
