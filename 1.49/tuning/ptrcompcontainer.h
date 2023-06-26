
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
// File tuning/ptrcompcontainer.h

#ifndef TUNING_PTRCOMPCONTAINER_H
#define TUNING_PTRCOMPCONTAINER_H

#include "tuning/defs.hpp"

//---------------------------------------------------------------------------

template <class t_container>
  class TL_TM_EXPORT gct_PtrCompContainer: public t_container
    {
  public:
    typedef TL_TYPENAME t_container::t_Length    t_Length;
    typedef TL_TYPENAME t_container::t_Position  t_Position;
    typedef TL_TYPENAME t_container::t_Object    t_Object;
    typedef TL_TYPENAME t_container::t_RefObject t_RefObject;

    inline bool          ContainsRef (const t_RefObject * po_obj) const;
    t_Length             CountRefs (const t_RefObject * po_obj) const;

    t_Position           SearchFirstRef (const t_RefObject * po_obj) const;
    t_Position           SearchLastRef (const t_RefObject * po_obj) const;
    t_Position           SearchNextRef (t_Position o_pos,
                           const t_RefObject * po_obj) const;
    t_Position           SearchPrevRef (t_Position o_pos,
                           const t_RefObject * po_obj) const;

    inline t_RefObject * GetFirstEqualRef (const t_RefObject * po_obj) const;
    inline t_RefObject * GetLastEqualRef (const t_RefObject * po_obj) const;

    inline t_Position    AddRefCond (const t_RefObject * po_obj);
    inline t_Position    AddRefBeforeFirstCond (const t_RefObject * po_obj);
    inline t_Position    AddRefAfterLastCond (const t_RefObject * po_obj);

    inline t_Position    DelFirstEqualRef (const t_RefObject * po_obj);
    inline t_Position    DelLastEqualRef (const t_RefObject * po_obj);
    inline t_Position    DelFirstEqualRefCond (const t_RefObject * po_obj);
    inline t_Position    DelLastEqualRefCond (const t_RefObject * po_obj);

    inline t_Position    DelFirstEqualRefAndObj (const t_RefObject * po_obj);
    inline t_Position    DelLastEqualRefAndObj (const t_RefObject * po_obj);
    inline t_Position    DelFirstEqualRefAndObjCond
                           (const t_RefObject * po_obj);
    inline t_Position    DelLastEqualRefAndObjCond
                           (const t_RefObject * po_obj);
    };

//---------------------------------------------------------------------------

template <class t_container>
  inline bool gct_PtrCompContainer <t_container>::ContainsRef
  (
  const t_RefObject * po_obj
  ) const
    {
    return SearchFirstRef (po_obj) != 0;
    }

//---------------------------------------------------------------------------

template <class t_container>
  TL_TYPENAME gct_PtrCompContainer <t_container>::t_Length
  gct_PtrCompContainer <t_container>::CountRefs
  (
  const t_RefObject * po_obj
  ) const
    {
    TL_ASSERT (po_obj != 0);

    t_Length o_count = 0;

    for (t_Position o_pos = this-> FirstForSearch (& (t_Object &) po_obj);
         o_pos != 0;
         o_pos = this-> NextForSearch (o_pos, & (t_Object &) po_obj))
      if (* this-> GetPtr (o_pos) == * po_obj)
        o_count ++;

    return o_count;
    }

//---------------------------------------------------------------------------

template <class t_container>
  TL_TYPENAME gct_PtrCompContainer <t_container>::t_Position
  gct_PtrCompContainer <t_container>::SearchFirstRef
  (
  const t_RefObject * po_obj
  ) const
    {
    TL_ASSERT (po_obj != 0);

    for (t_Position o_pos = this-> FirstForSearch (& (t_Object &) po_obj);
         o_pos != 0;
         o_pos = this-> NextForSearch (o_pos, & (t_Object &) po_obj))
      if (* this-> GetPtr (o_pos) == * po_obj)
        return o_pos;

    return 0;
    }

//---------------------------------------------------------------------------

template <class t_container>
  TL_TYPENAME gct_PtrCompContainer <t_container>::t_Position
  gct_PtrCompContainer <t_container>::SearchLastRef
  (
  const t_RefObject * po_obj
  ) const
    {
    TL_ASSERT (po_obj != 0);

    for (t_Position o_pos = this-> LastForSearch (& (t_Object &) po_obj);
         o_pos != 0;
         o_pos = this-> PrevForSearch (o_pos, & (t_Object &) po_obj))
      if (* this-> GetPtr (o_pos) == * po_obj)
        return o_pos;

    return 0;
    }

//---------------------------------------------------------------------------

template <class t_container>
  TL_TYPENAME gct_PtrCompContainer <t_container>::t_Position
  gct_PtrCompContainer <t_container>::SearchNextRef
  (
  t_Position o_pos,
  const t_RefObject * po_obj
  ) const
    {
    TL_ASSERT (o_pos != 0);
    TL_ASSERT (po_obj != 0);

    while ((o_pos = this-> NextForSearch (o_pos, & (t_Object &) po_obj)) != 0)
      if (* this-> GetPtr (o_pos) == * po_obj)
        return o_pos;

    return 0;
    }

//---------------------------------------------------------------------------

template <class t_container>
  TL_TYPENAME gct_PtrCompContainer <t_container>::t_Position
  gct_PtrCompContainer <t_container>::SearchPrevRef
  (
  t_Position o_pos,
  const t_RefObject * po_obj
  ) const
    {
    TL_ASSERT (o_pos != 0);
    TL_ASSERT (po_obj != 0);

    while ((o_pos = this-> PrevForSearch (o_pos, & (t_Object &) po_obj)) != 0)
      if (* this-> GetPtr (o_pos) == * po_obj)
        return o_pos;

    return 0;
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_PtrCompContainer <t_container>::t_RefObject *
  gct_PtrCompContainer <t_container>::GetFirstEqualRef
  (
  const t_RefObject * po_obj
  ) const
    {
    return this-> GetPtr (SearchFirstRef (po_obj));
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_PtrCompContainer <t_container>::t_RefObject *
  gct_PtrCompContainer <t_container>::GetLastEqualRef
  (
  const t_RefObject * po_obj
  ) const
    {
    return this-> GetPtr (SearchLastRef (po_obj));
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_PtrCompContainer <t_container>::t_Position
  gct_PtrCompContainer <t_container>::AddRefCond
  (
  const t_RefObject * po_obj
  )
    {
    t_Position o_pos = SearchFirstRef (po_obj);
    return (o_pos != 0) ? o_pos : this-> AddPtr (po_obj);
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_PtrCompContainer <t_container>::t_Position
  gct_PtrCompContainer <t_container>::AddRefBeforeFirstCond
  (
  const t_RefObject * po_obj
  )
    {
    t_Position o_pos = SearchFirstRef (po_obj);
    return (o_pos != 0) ? o_pos : this-> AddPtrBeforeFirst (po_obj);
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_PtrCompContainer <t_container>::t_Position
  gct_PtrCompContainer <t_container>::AddRefAfterLastCond
  (
  const t_RefObject * po_obj
  )
    {
    t_Position o_pos = SearchFirstRef (po_obj);
    return (o_pos != 0) ? o_pos : this-> AddPtrAfterLast (po_obj);
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_PtrCompContainer <t_container>::t_Position
  gct_PtrCompContainer <t_container>::DelFirstEqualRef
  (
  const t_RefObject * po_obj
  )
    {
    return this-> DelPtr (SearchFirstRef (po_obj));
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_PtrCompContainer <t_container>::t_Position
  gct_PtrCompContainer <t_container>::DelLastEqualRef
  (
  const t_RefObject * po_obj
  )
    {
    return this-> DelPtr (SearchLastRef (po_obj));
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_PtrCompContainer <t_container>::t_Position
  gct_PtrCompContainer <t_container>::DelFirstEqualRefCond
  (
  const t_RefObject * po_obj
  )
    {
    t_Position o_pos = SearchFirstRef (po_obj);
    return (o_pos == 0) ? (t_Position) 0 : this-> DelPtr (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_PtrCompContainer <t_container>::t_Position
  gct_PtrCompContainer <t_container>::DelLastEqualRefCond
  (
  const t_RefObject * po_obj
  )
    {
    t_Position o_pos = SearchLastRef (po_obj);
    return (o_pos == 0) ? (t_Position) 0 : this-> DelPtr (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_PtrCompContainer <t_container>::t_Position
  gct_PtrCompContainer <t_container>::DelFirstEqualRefAndObj
  (
  const t_RefObject * po_obj
  )
    {
    return this-> DelPtrAndObj (SearchFirstRef (po_obj));
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_PtrCompContainer <t_container>::t_Position
  gct_PtrCompContainer <t_container>::DelLastEqualRefAndObj
  (
  const t_RefObject * po_obj
  )
    {
    return this-> DelPtrAndObj (SearchLastRef (po_obj));
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_PtrCompContainer <t_container>::t_Position
  gct_PtrCompContainer <t_container>::DelFirstEqualRefAndObjCond
  (
  const t_RefObject * po_obj
  )
    {
    t_Position o_pos = SearchFirstRef (po_obj);
    return (o_pos == 0) ? (t_Position) 0 : this-> DelPtrAndObj (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_PtrCompContainer <t_container>::t_Position
  gct_PtrCompContainer <t_container>::DelLastEqualRefAndObjCond
  (
  const t_RefObject * po_obj
  )
    {
    t_Position o_pos = SearchLastRef (po_obj);
    return (o_pos == 0) ? (t_Position) 0 : this-> DelPtrAndObj (o_pos);
    }

#endif
