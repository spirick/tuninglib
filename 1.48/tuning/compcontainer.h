
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
// File tuning/compcontainer.h

#ifndef TUNING_COMPCONTAINER_H
#define TUNING_COMPCONTAINER_H

#include "tuning/defs.hpp"

//---------------------------------------------------------------------------

template <class t_container>
  class TL_TM_EXPORT gct_CompContainer: public t_container
    {
  public:
    typedef TL_TYPENAME t_container::t_Length   t_Length;
    typedef TL_TYPENAME t_container::t_Position t_Position;
    typedef TL_TYPENAME t_container::t_Object   t_Object;

    inline bool          ContainsObj (const t_Object * po_obj) const;
    t_Length             CountObjs (const t_Object * po_obj) const;

    t_Position           SearchFirstObj (const t_Object * po_obj) const;
    t_Position           SearchLastObj (const t_Object * po_obj) const;
    t_Position           SearchNextObj (t_Position o_pos,
                           const t_Object * po_obj) const;
    t_Position           SearchPrevObj (t_Position o_pos,
                           const t_Object * po_obj) const;

    inline t_Object *    GetFirstEqualObj (const t_Object * po_obj) const;
    inline t_Object *    GetLastEqualObj (const t_Object * po_obj) const;

    inline t_Position    AddObjCond (const t_Object * po_obj);
    inline t_Position    AddObjBeforeFirstCond (const t_Object * po_obj);
    inline t_Position    AddObjAfterLastCond (const t_Object * po_obj);

    inline t_Position    DelFirstEqualObj (const t_Object * po_obj);
    inline t_Position    DelLastEqualObj (const t_Object * po_obj);
    inline t_Position    DelFirstEqualObjCond (const t_Object * po_obj);
    inline t_Position    DelLastEqualObjCond (const t_Object * po_obj);
    };

//---------------------------------------------------------------------------

template <class t_container>
  inline bool gct_CompContainer <t_container>::ContainsObj
  (
  const t_Object * po_obj
  ) const
    {
    return SearchFirstObj (po_obj) != 0;
    }

//---------------------------------------------------------------------------

template <class t_container>
  TL_TYPENAME gct_CompContainer <t_container>::t_Length
  gct_CompContainer <t_container>::CountObjs
  (
  const t_Object * po_obj
  ) const
    {
    TL_ASSERT (po_obj != 0);

    t_Length o_count = 0;

    for (t_Position o_pos = this-> FirstForSearch (po_obj);
         o_pos != 0;
         o_pos = this-> NextForSearch (o_pos, po_obj))
      if (* this-> GetObj (o_pos) == * po_obj)
        o_count ++;

    return o_count;
    }

//---------------------------------------------------------------------------

template <class t_container>
  TL_TYPENAME gct_CompContainer <t_container>::t_Position
  gct_CompContainer <t_container>::SearchFirstObj
  (
  const t_Object * po_obj
  ) const
    {
    TL_ASSERT (po_obj != 0);

    for (t_Position o_pos = this-> FirstForSearch (po_obj);
         o_pos != 0;
         o_pos = this-> NextForSearch (o_pos, po_obj))
      if (* this-> GetObj (o_pos) == * po_obj)
        return o_pos;

    return 0;
    }

//---------------------------------------------------------------------------

template <class t_container>
  TL_TYPENAME gct_CompContainer <t_container>::t_Position
  gct_CompContainer <t_container>::SearchLastObj
  (
  const t_Object * po_obj
  ) const
    {
    TL_ASSERT (po_obj != 0);

    for (t_Position o_pos = this-> LastForSearch (po_obj);
         o_pos != 0;
         o_pos = this-> PrevForSearch (o_pos, po_obj))
      if (* this-> GetObj (o_pos) == * po_obj)
        return o_pos;

    return 0;
    }

//---------------------------------------------------------------------------

template <class t_container>
  TL_TYPENAME gct_CompContainer <t_container>::t_Position
  gct_CompContainer <t_container>::SearchNextObj
  (
  t_Position o_pos,
  const t_Object * po_obj
  ) const
    {
    TL_ASSERT (o_pos != 0);
    TL_ASSERT (po_obj != 0);

    while ((o_pos = this-> NextForSearch (o_pos, po_obj)) != 0)
      if (* this-> GetObj (o_pos) == * po_obj)
        return o_pos;

    return 0;
    }

//---------------------------------------------------------------------------

template <class t_container>
  TL_TYPENAME gct_CompContainer <t_container>::t_Position
  gct_CompContainer <t_container>::SearchPrevObj
  (
  t_Position o_pos,
  const t_Object * po_obj
  ) const
    {
    TL_ASSERT (o_pos != 0);
    TL_ASSERT (po_obj != 0);

    while ((o_pos = this-> PrevForSearch (o_pos, po_obj)) != 0)
      if (* this-> GetObj (o_pos) == * po_obj)
        return o_pos;

    return 0;
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_CompContainer <t_container>::t_Object *
  gct_CompContainer <t_container>::GetFirstEqualObj
  (
  const t_Object * po_obj
  ) const
    {
    return this-> GetObj (SearchFirstObj (po_obj));
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_CompContainer <t_container>::t_Object *
  gct_CompContainer <t_container>::GetLastEqualObj
  (
  const t_Object * po_obj
  ) const
    {
    return this-> GetObj (SearchLastObj (po_obj));
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_CompContainer <t_container>::t_Position
  gct_CompContainer <t_container>::AddObjCond
  (
  const t_Object * po_obj
  )
    {
    t_Position o_pos = SearchFirstObj (po_obj);
    return (o_pos != 0) ? o_pos : this-> AddObj (po_obj);
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_CompContainer <t_container>::t_Position
  gct_CompContainer <t_container>::AddObjBeforeFirstCond
  (
  const t_Object * po_obj
  )
    {
    t_Position o_pos = SearchFirstObj (po_obj);
    return (o_pos != 0) ? o_pos : this-> AddObjBeforeFirst (po_obj);
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_CompContainer <t_container>::t_Position
  gct_CompContainer <t_container>::AddObjAfterLastCond
  (
  const t_Object * po_obj
  )
    {
    t_Position o_pos = SearchFirstObj (po_obj);
    return (o_pos != 0) ? o_pos : this-> AddObjAfterLast (po_obj);
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_CompContainer <t_container>::t_Position
  gct_CompContainer <t_container>::DelFirstEqualObj
  (
  const t_Object * po_obj
  )
    {
    return this-> DelObj (SearchFirstObj (po_obj));
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_CompContainer <t_container>::t_Position
  gct_CompContainer <t_container>::DelLastEqualObj
  (
  const t_Object * po_obj
  )
    {
    return this-> DelObj (SearchLastObj (po_obj));
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_CompContainer <t_container>::t_Position
  gct_CompContainer <t_container>::DelFirstEqualObjCond
  (
  const t_Object * po_obj
  )
    {
    t_Position o_pos = SearchFirstObj (po_obj);
    return (o_pos == 0) ? (t_Position) 0 : this-> DelObj (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_CompContainer <t_container>::t_Position
  gct_CompContainer <t_container>::DelLastEqualObjCond
  (
  const t_Object * po_obj
  )
    {
    t_Position o_pos = SearchLastObj (po_obj);
    return (o_pos == 0) ? (t_Position) 0 : this-> DelObj (o_pos);
    }

#endif
