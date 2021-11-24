
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
// File tuning/map.h

#ifndef TUNING_MAP_H
#define TUNING_MAP_H

#include "tuning/key.h"

//---------------------------------------------------------------------------

template <class t_key, class t_value>
  class TL_TM_EXPORT gct_MapEntry: public t_key
    {
  public:
    typedef t_key        t_Key;
    typedef t_value      t_Value;

    t_Value              o_Value;

                         gct_MapEntry ()
                           { TL_ASSERT (false); }
                         gct_MapEntry (t_key o_key):
                           t_key (o_key) { }
                         gct_MapEntry (t_key o_key, const t_value * po_value):
                           t_key (o_key), o_Value (* po_value) { }
    };

//---------------------------------------------------------------------------

template <class t_container>
  class TL_TM_EXPORT gct_Map: public t_container
    {
  public:
    typedef TL_TYPENAME t_container::t_Length   t_Length;
    typedef TL_TYPENAME t_container::t_Position t_Position;
    typedef TL_TYPENAME t_container::t_Object   t_Object;
    typedef TL_TYPENAME t_Object::t_Key         t_Key;
    typedef TL_TYPENAME t_Object::t_Value       t_Value;

    inline bool          ContainsKey (t_Key o_key) const;
    t_Length             CountKeys (t_Key o_key) const;

    t_Position           SearchFirstKey (t_Key o_key) const;
    t_Position           SearchLastKey (t_Key o_key) const;
    t_Position           SearchNextKey (t_Position o_pos,
                           t_Key o_key) const;
    t_Position           SearchPrevKey (t_Position o_pos,
                           t_Key o_key) const;

    inline t_Key         GetKey (t_Position o_pos) const;
    inline t_Value *     GetValue (t_Position o_pos) const;
    inline t_Value *     GetFirstValue (t_Key o_key) const;
    inline t_Value *     GetLastValue (t_Key o_key) const;

    t_Position           AddKeyAndValue (t_Key o_key,
                           const t_Value * po_value = 0);
    t_Position           AddKeyAndValueCond (t_Key o_key,
                           const t_Value * po_value = 0);

    inline t_Position    DelKeyAndValue (t_Position o_pos);
    inline t_Position    DelFirstKeyAndValue (t_Key o_key);
    inline t_Position    DelLastKeyAndValue (t_Key o_key);
    inline t_Position    DelFirstKeyAndValueCond (t_Key o_key);
    inline t_Position    DelLastKeyAndValueCond (t_Key o_key);
    inline void          DelAllKeyAndValue ();
    };

//---------------------------------------------------------------------------

template <class t_container>
  inline bool gct_Map <t_container>::ContainsKey
  (
  t_Key o_key
  ) const
    {
    return SearchFirstKey (o_key) != 0;
    }

//---------------------------------------------------------------------------

template <class t_container>
  TL_TYPENAME gct_Map <t_container>::t_Length
  gct_Map <t_container>::CountKeys
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

template <class t_container>
  TL_TYPENAME gct_Map <t_container>::t_Position
  gct_Map <t_container>::SearchFirstKey
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

template <class t_container>
  TL_TYPENAME gct_Map <t_container>::t_Position
  gct_Map <t_container>::SearchLastKey
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

template <class t_container>
  TL_TYPENAME gct_Map <t_container>::t_Position
  gct_Map <t_container>::SearchNextKey
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

template <class t_container>
  TL_TYPENAME gct_Map <t_container>::t_Position
  gct_Map <t_container>::SearchPrevKey
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

template <class t_container>
  inline TL_TYPENAME gct_Map <t_container>::t_Key
  gct_Map <t_container>::GetKey
  (
  t_Position o_pos
  ) const
    {
    return * this-> GetObj (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_Map <t_container>::t_Value *
  gct_Map <t_container>::GetValue
  (
  t_Position o_pos
  ) const
    {
    return & this-> GetObj (o_pos)-> o_Value;
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_Map <t_container>::t_Value *
  gct_Map <t_container>::GetFirstValue
  (
  t_Key o_key
  ) const
    {
    return & this-> GetObj (SearchFirstKey (o_key))-> o_Value;
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_Map <t_container>::t_Value *
  gct_Map <t_container>::GetLastValue
  (
  t_Key o_key
  ) const
    {
    return & this-> GetObj (SearchLastKey (o_key))-> o_Value;
    }

//---------------------------------------------------------------------------

template <class t_container>
  TL_TYPENAME gct_Map <t_container>::t_Position
  gct_Map <t_container>::AddKeyAndValue
  (
  t_Key o_key,
  const t_Value * po_value
  )
    {
    if (po_value == 0)
      {
      gct_MapEntry <t_Key, t_Value> co_entry (o_key);
      return this-> AddObj (& co_entry);
      }
    else
      {
      gct_MapEntry <t_Key, t_Value> co_entry (o_key, po_value);
      return this-> AddObj (& co_entry);
      }
    }

//---------------------------------------------------------------------------

template <class t_container>
  TL_TYPENAME gct_Map <t_container>::t_Position
  gct_Map <t_container>::AddKeyAndValueCond
  (
  t_Key o_key,
  const t_Value * po_value
  )
    {
    t_Position o_pos = SearchFirstKey (o_key);
    return (o_pos != 0) ? o_pos : AddKeyAndValue (o_key, po_value);
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_Map <t_container>::t_Position
  gct_Map <t_container>::DelKeyAndValue
  (
  t_Position o_pos
  )
    {
    return this-> DelObj (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_Map <t_container>::t_Position
  gct_Map <t_container>::DelFirstKeyAndValue
  (
  t_Key o_key
  )
    {
    return this-> DelObj (SearchFirstKey (o_key));
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_Map <t_container>::t_Position
  gct_Map <t_container>::DelLastKeyAndValue
  (
  t_Key o_key
  )
    {
    return this-> DelObj (SearchLastKey (o_key));
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_Map <t_container>::t_Position
  gct_Map <t_container>::DelFirstKeyAndValueCond
  (
  t_Key o_key
  )
    {
    t_Position o_pos = SearchFirstKey (o_key);
    return (o_pos == 0) ? (t_Position) 0 : this-> DelObj (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline TL_TYPENAME gct_Map <t_container>::t_Position
  gct_Map <t_container>::DelLastKeyAndValueCond
  (
  t_Key o_key
  )
    {
    t_Position o_pos = SearchLastKey (o_key);
    return (o_pos == 0) ? (t_Position) 0 : this-> DelObj (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_container>
  inline void gct_Map <t_container>::DelAllKeyAndValue ()
    {
    this-> DelAll ();
    }

#endif
