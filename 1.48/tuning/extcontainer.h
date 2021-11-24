
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
// File tuning/extcontainer.h

#ifndef TUNING_EXTCONTAINER_H
#define TUNING_EXTCONTAINER_H

#include "tuning/defs.hpp"

//---------------------------------------------------------------------------

template <class t_container>
  class TL_TM_EXPORT gct_ExtContainer: public t_container
    {
  public:
    typedef TL_TYPENAME t_container::t_Length   t_Length;
    typedef TL_TYPENAME t_container::t_Position t_Position;
    typedef TL_TYPENAME t_container::t_Object   t_Object;

    inline t_Object *    GetFirstObj () const;
    inline t_Object *    GetLastObj () const;
    inline t_Object *    GetNextObj (t_Position o_pos) const;
    inline t_Object *    GetPrevObj (t_Position o_pos) const;
    inline t_Object *    GetNthObj (t_Length u_idx) const;

    inline t_Position    AddObjBeforeFirst (const t_Object * po_obj = 0);
    inline t_Position    AddObjAfterLast (const t_Object * po_obj = 0);
    inline t_Position    AddObjBeforeNth (t_Length u_idx,
                           const t_Object * po_obj = 0);
    inline t_Position    AddObjAfterNth (t_Length u_idx,
                           const t_Object * po_obj = 0);

    t_Object *           GetNewObj (const t_Object * po_obj = 0);
    t_Object *           GetNewFirstObj (const t_Object * po_obj = 0);
    t_Object *           GetNewLastObj (const t_Object * po_obj = 0);
    t_Object *           GetNewObjBefore (t_Position o_pos,
                           const t_Object * po_obj = 0);
    t_Object *           GetNewObjAfter (t_Position o_pos,
                           const t_Object * po_obj = 0);
    t_Object *           GetNewObjBeforeNth (t_Length u_idx,
                           const t_Object * po_obj = 0);
    t_Object *           GetNewObjAfterNth (t_Length u_idx,
                           const t_Object * po_obj = 0);

    inline t_Position    DelFirstObj ();
    inline t_Position    DelLastObj ();
    inline t_Position    DelNextObj (t_Position o_pos);
    inline t_Position    DelPrevObj (t_Position o_pos);
    inline t_Position    DelNthObj (t_Length u_idx);

    inline t_Position    FreeFirstObj ();
    inline t_Position    FreeLastObj ();
    inline t_Position    FreeNextObj (t_Position o_pos);
    inline t_Position    FreePrevObj (t_Position o_pos);
    inline t_Position    FreeNthObj (t_Length u_idx);
    };

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_ExtContainer <t_container>::t_Object *
  gct_ExtContainer <t_container>::GetFirstObj () const
    {
    return this-> GetObj (this-> First ());
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_ExtContainer <t_container>::t_Object *
  gct_ExtContainer <t_container>::GetLastObj () const
    {
    return this-> GetObj (this-> Last ());
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_ExtContainer <t_container>::t_Object *
  gct_ExtContainer <t_container>::GetNextObj
  (
  t_Position o_pos
  ) const
    {
    return this-> GetObj (this-> Next (o_pos));
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_ExtContainer <t_container>::t_Object *
  gct_ExtContainer <t_container>::GetPrevObj
  (
  t_Position o_pos
  ) const
    {
    return this-> GetObj (this-> Prev (o_pos));
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_ExtContainer <t_container>::t_Object *
  gct_ExtContainer <t_container>::GetNthObj
  (
  t_Length u_idx
  ) const
    {
    return this-> GetObj (this-> Nth (u_idx));
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_ExtContainer <t_container>::t_Position
  gct_ExtContainer <t_container>::AddObjBeforeFirst
  (
  const t_Object * po_obj
  )
    {
    return this-> AddObjAfter (0, po_obj);
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_ExtContainer <t_container>::t_Position
  gct_ExtContainer <t_container>::AddObjAfterLast
  (
  const t_Object * po_obj
  )
    {
    return this-> AddObjAfter (this-> Last (), po_obj);
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_ExtContainer <t_container>::t_Position
  gct_ExtContainer <t_container>::AddObjBeforeNth
  (
  t_Length u_idx,
  const t_Object * po_obj
  )
    {
    return this-> AddObjBefore (this-> Nth (u_idx), po_obj);
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_ExtContainer <t_container>::t_Position
  gct_ExtContainer <t_container>::AddObjAfterNth
  (
  t_Length u_idx,
  const t_Object * po_obj
  )
    {
    return this-> AddObjAfter (this-> Nth (u_idx), po_obj);
    }

//---------------------------------------------------------------------------

template <class t_container>
  TL_TYPENAME gct_ExtContainer <t_container>::t_Object *
  gct_ExtContainer <t_container>::GetNewObj
  (
  const t_Object * po_obj
  )
    {
    return this-> GetObj (this-> AddObj (po_obj));
    }

//---------------------------------------------------------------------------

template <class t_container>
  TL_TYPENAME gct_ExtContainer <t_container>::t_Object *
  gct_ExtContainer <t_container>::GetNewFirstObj
  (
  const t_Object * po_obj
  )
    {
    return this-> GetObj (AddObjBeforeFirst (po_obj));
    }

//---------------------------------------------------------------------------

template <class t_container>
  TL_TYPENAME gct_ExtContainer <t_container>::t_Object *
  gct_ExtContainer <t_container>::GetNewLastObj
  (
  const t_Object * po_obj
  )
    {
    return this-> GetObj (AddObjAfterLast (po_obj));
    }

//---------------------------------------------------------------------------

template <class t_container>
  TL_TYPENAME gct_ExtContainer <t_container>::t_Object *
  gct_ExtContainer <t_container>::GetNewObjBefore
  (
  t_Position o_pos,
  const t_Object * po_obj
  )
    {
    return this-> GetObj (this-> AddObjBefore (o_pos, po_obj));
    }

//---------------------------------------------------------------------------

template <class t_container>
  TL_TYPENAME gct_ExtContainer <t_container>::t_Object *
  gct_ExtContainer <t_container>::GetNewObjAfter
  (
  t_Position o_pos,
  const t_Object * po_obj
  )
    {
    return this-> GetObj (this-> AddObjAfter (o_pos, po_obj));
    }

//---------------------------------------------------------------------------

template <class t_container>
  TL_TYPENAME gct_ExtContainer <t_container>::t_Object *
  gct_ExtContainer <t_container>::GetNewObjBeforeNth
  (
  t_Length u_idx,
  const t_Object * po_obj
  )
    {
    return this-> GetObj (AddObjBeforeNth (u_idx, po_obj));
    }

//---------------------------------------------------------------------------

template <class t_container>
  TL_TYPENAME gct_ExtContainer <t_container>::t_Object *
  gct_ExtContainer <t_container>::GetNewObjAfterNth
  (
  t_Length u_idx,
  const t_Object * po_obj
  )
    {
    return this-> GetObj (AddObjAfterNth (u_idx, po_obj));
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_ExtContainer <t_container>::t_Position
  gct_ExtContainer <t_container>::DelFirstObj ()
    {
    return this-> DelObj (this-> First ());
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_ExtContainer <t_container>::t_Position
  gct_ExtContainer <t_container>::DelLastObj ()
    {
    return this-> DelObj (this-> Last ());
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_ExtContainer <t_container>::t_Position
  gct_ExtContainer <t_container>::DelNextObj
  (
  t_Position o_pos
  )
    {
    return this-> DelObj (this-> Next (o_pos));
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_ExtContainer <t_container>::t_Position
  gct_ExtContainer <t_container>::DelPrevObj
  (
  t_Position o_pos
  )
    {
    return this-> DelObj (this-> Prev (o_pos));
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_ExtContainer <t_container>::t_Position
  gct_ExtContainer <t_container>::DelNthObj
  (
  t_Length u_idx
  )
    {
    return this-> DelObj (this-> Nth (u_idx));
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_ExtContainer <t_container>::t_Position
  gct_ExtContainer <t_container>::FreeFirstObj ()
    {
    return this-> FreeObj (this-> First ());
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_ExtContainer <t_container>::t_Position
  gct_ExtContainer <t_container>::FreeLastObj ()
    {
    return this-> FreeObj (this-> Last ());
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_ExtContainer <t_container>::t_Position
  gct_ExtContainer <t_container>::FreeNextObj
  (
  t_Position o_pos
  )
    {
    return this-> FreeObj (this-> Next (o_pos));
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_ExtContainer <t_container>::t_Position
  gct_ExtContainer <t_container>::FreePrevObj
  (
  t_Position o_pos
  )
    {
    return this-> FreeObj (this-> Prev (o_pos));
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_ExtContainer <t_container>::t_Position
  gct_ExtContainer <t_container>::FreeNthObj
  (
  t_Length u_idx
  )
    {
    return this-> FreeObj (this-> Nth (u_idx));
    }

#endif
