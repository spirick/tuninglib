
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
// File tuning/ptrmap.h

#ifndef TUNING_PTRMAP_H
#define TUNING_PTRMAP_H

#include "tuning/key.h"

//---------------------------------------------------------------------------

template <class t_key>
  class TL_TM_EXPORT gct_PtrMapEntry: public t_key
    {
  public:
    typedef t_key        t_Key;

    void *               pv_Value;

                         gct_PtrMapEntry ()
                           { TL_ASSERT (false); }
                         gct_PtrMapEntry (t_key o_key, void * pv_value):
                           t_key (o_key), pv_Value (pv_value) { }
    };

//---------------------------------------------------------------------------

template <class t_container, class t_value>
  class TL_TM_EXPORT gct_PtrMap: public t_container
    {
  public:
    typedef TL_TYPENAME t_container::t_Length   t_Length;
    typedef TL_TYPENAME t_container::t_Position t_Position;
    typedef TL_TYPENAME t_container::t_Object   t_Object;
    typedef TL_TYPENAME t_Object::t_Key         t_Key;
    typedef t_value                             t_Value;

    inline bool          ContainsKey (t_Key o_key) const;
    t_Length             CountKeys (t_Key o_key) const;

    t_Position           SearchFirstKey (t_Key o_key) const;
    t_Position           SearchLastKey (t_Key o_key) const;
    t_Position           SearchNextKey (t_Position o_pos,
                           t_Key o_key) const;
    t_Position           SearchPrevKey (t_Position o_pos,
                           t_Key o_key) const;

    inline t_Key         GetKey (t_Position o_pos) const;
    inline t_Value *     GetValPtr (t_Position o_pos) const;
    inline t_Value *     GetFirstValPtr (t_Key o_key) const;
    inline t_Value *     GetLastValPtr (t_Key o_key) const;

    t_Position           AddKeyAndValPtr (t_Key o_key,
                           const t_Value * po_value);
    t_Position           AddKeyAndValPtrCond (t_Key o_key,
                           const t_Value * po_value);

    inline t_Position    DelKey (t_Position o_pos);
    inline t_Position    DelFirstKey (t_Key o_key);
    inline t_Position    DelLastKey (t_Key o_key);
    inline t_Position    DelFirstKeyCond (t_Key o_key);
    inline t_Position    DelLastKeyCond (t_Key o_key);
    inline void          DelAllKey ();

    inline t_Position    DelKeyAndValue (t_Position o_pos);
    inline t_Position    DelFirstKeyAndValue (t_Key o_key);
    inline t_Position    DelLastKeyAndValue (t_Key o_key);
    inline t_Position    DelFirstKeyAndValueCond (t_Key o_key);
    inline t_Position    DelLastKeyAndValueCond (t_Key o_key);
    void                 DelAllKeyAndValue ();
    };

//---------------------------------------------------------------------------

template <class t_container, class t_value>
  inline bool gct_PtrMap <t_container, t_value>::ContainsKey
  (
  t_Key o_key
  ) const
    {
    return SearchFirstKey (o_key) != 0;
    }

//---------------------------------------------------------------------------

template <class t_container, class t_value>
  TL_TYPENAME gct_PtrMap <t_container, t_value>::t_Length
  gct_PtrMap <t_container, t_value>::CountKeys
  (
  t_Key o_key
  ) const
    {
    t_Length o_count = 0;

    for (t_Position o_pos = this-> FirstForSearch ((const t_Object *) & o_key);
         o_pos != 0;
         o_pos = this-> NextForSearch (o_pos, (const t_Object *) & o_key))
      if (* (const t_Key *) this-> GetObj (o_pos) == o_key)
        o_count ++;

    return o_count;
    }

//---------------------------------------------------------------------------

template <class t_container, class t_value>
  TL_TYPENAME gct_PtrMap <t_container, t_value>::t_Position
  gct_PtrMap <t_container, t_value>::SearchFirstKey
  (
  t_Key o_key
  ) const
    {
    for (t_Position o_pos = this-> FirstForSearch ((const t_Object *) & o_key);
         o_pos != 0;
         o_pos = this-> NextForSearch (o_pos, (const t_Object *) & o_key))
      if (* (const t_Key *) this-> GetObj (o_pos) == o_key)
        return o_pos;

    return 0;
    }

//---------------------------------------------------------------------------

template <class t_container, class t_value>
  TL_TYPENAME gct_PtrMap <t_container, t_value>::t_Position
  gct_PtrMap <t_container, t_value>::SearchLastKey
  (
  t_Key o_key
  ) const
    {
    for (t_Position o_pos = this-> LastForSearch ((const t_Object *) & o_key);
         o_pos != 0;
         o_pos = this-> PrevForSearch (o_pos, (const t_Object *) & o_key))
      if (* (const t_Key *) this-> GetObj (o_pos) == o_key)
        return o_pos;

    return 0;
    }

//---------------------------------------------------------------------------

template <class t_container, class t_value>
  TL_TYPENAME gct_PtrMap <t_container, t_value>::t_Position
  gct_PtrMap <t_container, t_value>::SearchNextKey
  (
  t_Position o_pos,
  t_Key o_key
  ) const
    {
    TL_ASSERT (o_pos != 0);

    while ((o_pos = this-> NextForSearch (o_pos, (const t_Object *) & o_key)) != 0)
      if (* (const t_Key *) this-> GetObj (o_pos) == o_key)
        return o_pos;

    return 0;
    }

//---------------------------------------------------------------------------

template <class t_container, class t_value>
  TL_TYPENAME gct_PtrMap <t_container, t_value>::t_Position
  gct_PtrMap <t_container, t_value>::SearchPrevKey
  (
  t_Position o_pos,
  t_Key o_key
  ) const
    {
    TL_ASSERT (o_pos != 0);

    while ((o_pos = this-> PrevForSearch (o_pos, (const t_Object *) & o_key)) != 0)
      if (* (const t_Key *) this-> GetObj (o_pos) == o_key)
        return o_pos;

    return 0;
    }

//---------------------------------------------------------------------------

template <class t_container, class t_value>
  inline TL_TYPENAME gct_PtrMap <t_container, t_value>::t_Key
  gct_PtrMap <t_container, t_value>::GetKey
  (
  t_Position o_pos
  ) const
    {
    return * this-> GetObj (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_container, class t_value>
  inline TL_TYPENAME gct_PtrMap <t_container, t_value>::t_Value *
  gct_PtrMap <t_container, t_value>::GetValPtr
  (
  t_Position o_pos
  ) const
    {
    return (t_Value *) this-> GetObj (o_pos)-> pv_Value;
    }

//---------------------------------------------------------------------------

template <class t_container, class t_value>
  inline TL_TYPENAME gct_PtrMap <t_container, t_value>::t_Value *
  gct_PtrMap <t_container, t_value>::GetFirstValPtr
  (
  t_Key o_key
  ) const
    {
    return (t_Value *) this-> GetObj (SearchFirstKey (o_key))-> pv_Value;
    }

//---------------------------------------------------------------------------

template <class t_container, class t_value>
  inline TL_TYPENAME gct_PtrMap <t_container, t_value>::t_Value *
  gct_PtrMap <t_container, t_value>::GetLastValPtr
  (
  t_Key o_key
  ) const
    {
    return (t_Value *) this-> GetObj (SearchLastKey (o_key))-> pv_Value;
    }

//---------------------------------------------------------------------------

template <class t_container, class t_value>
  TL_TYPENAME gct_PtrMap <t_container, t_value>::t_Position
  gct_PtrMap <t_container, t_value>::AddKeyAndValPtr
  (
  t_Key o_key,
  const t_Value * po_value
  )
    {
    gct_PtrMapEntry <t_Key> co_entry (o_key, (void *) po_value);
    return this-> AddObj (& co_entry);
    }

//---------------------------------------------------------------------------

template <class t_container, class t_value>
  TL_TYPENAME gct_PtrMap <t_container, t_value>::t_Position
  gct_PtrMap <t_container, t_value>::AddKeyAndValPtrCond
  (
  t_Key o_key,
  const t_Value * po_value
  )
    {
    t_Position o_pos = SearchFirstKey (o_key);
    return (o_pos != 0) ? o_pos : AddKeyAndValPtr (o_key, po_value);
    }

//---------------------------------------------------------------------------

template <class t_container, class t_value>
  inline TL_TYPENAME gct_PtrMap <t_container, t_value>::t_Position
  gct_PtrMap <t_container, t_value>::DelKey
  (
  t_Position o_pos
  )
    {
    return this-> DelObj (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_container, class t_value>
  inline TL_TYPENAME gct_PtrMap <t_container, t_value>::t_Position
  gct_PtrMap <t_container, t_value>::DelFirstKey
  (
  t_Key o_key
  )
    {
    return this-> DelObj (SearchFirstKey (o_key));
    }

//---------------------------------------------------------------------------

template <class t_container, class t_value>
  inline TL_TYPENAME gct_PtrMap <t_container, t_value>::t_Position
  gct_PtrMap <t_container, t_value>::DelLastKey
  (
  t_Key o_key
  )
    {
    return this-> DelObj (SearchLastKey (o_key));
    }

//---------------------------------------------------------------------------

template <class t_container, class t_value>
  inline TL_TYPENAME gct_PtrMap <t_container, t_value>::t_Position
  gct_PtrMap <t_container, t_value>::DelFirstKeyCond
  (
  t_Key o_key
  )
    {
    t_Position o_pos = SearchFirstKey (o_key);
    return (o_pos == 0) ? (t_Position) 0 : this-> DelObj (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_container, class t_value>
  inline TL_TYPENAME gct_PtrMap <t_container, t_value>::t_Position
  gct_PtrMap <t_container, t_value>::DelLastKeyCond
  (
  t_Key o_key
  )
    {
    t_Position o_pos = SearchLastKey (o_key);
    return (o_pos == 0) ? (t_Position) 0 : this-> DelObj (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_container, class t_value>
  inline void gct_PtrMap <t_container, t_value>::DelAllKey ()
    {
    this-> DelAll ();
    }

//---------------------------------------------------------------------------

template <class t_container, class t_value>
  inline TL_TYPENAME gct_PtrMap <t_container, t_value>::t_Position
  gct_PtrMap <t_container, t_value>::DelKeyAndValue
  (
  t_Position o_pos
  )
    {
    delete GetValPtr (o_pos);
    return this-> DelObj (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_container, class t_value>
  inline TL_TYPENAME gct_PtrMap <t_container, t_value>::t_Position
  gct_PtrMap <t_container, t_value>::DelFirstKeyAndValue
  (
  t_Key o_key
  )
    {
    return DelKeyAndValue (SearchFirstKey (o_key));
    }

//---------------------------------------------------------------------------

template <class t_container, class t_value>
  inline TL_TYPENAME gct_PtrMap <t_container, t_value>::t_Position
  gct_PtrMap <t_container, t_value>::DelLastKeyAndValue
  (
  t_Key o_key
  )
    {
    return DelKeyAndValue (SearchLastKey (o_key));
    }

//---------------------------------------------------------------------------

template <class t_container, class t_value>
  inline TL_TYPENAME gct_PtrMap <t_container, t_value>::t_Position
  gct_PtrMap <t_container, t_value>::DelFirstKeyAndValueCond
  (
  t_Key o_key
  )
    {
    t_Position o_pos = SearchFirstKey (o_key);
    return (o_pos == 0) ? (t_Position) 0 : DelKeyAndValue (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_container, class t_value>
  inline TL_TYPENAME gct_PtrMap <t_container, t_value>::t_Position
  gct_PtrMap <t_container, t_value>::DelLastKeyAndValueCond
  (
  t_Key o_key
  )
    {
    t_Position o_pos = SearchLastKey (o_key);
    return (o_pos == 0) ? (t_Position) 0 : DelKeyAndValue (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_container, class t_value>
  void gct_PtrMap <t_container, t_value>::DelAllKeyAndValue ()
    {
    for (t_Position o_pos = this-> First (); o_pos != 0; o_pos = this-> Next (o_pos))
      delete GetValPtr (o_pos);

    this-> DelAll ();
    }

#endif
