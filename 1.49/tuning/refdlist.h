
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
// File tuning/refdlist.h

#ifndef TUNING_REFDLIST_H
#define TUNING_REFDLIST_H

#include "tuning/dlist.h"

//---------------------------------------------------------------------------

template <class t_obj, class t_store>
  class TL_TM_EXPORT gct_RefDList:
    public gct_ExtContainer <gct_DList <t_obj, t_store> >
    {
  public:
    typedef TL_TYPENAME gct_DList <t_obj, t_store>::t_Position t_Position;

    inline void          IncRef (t_Position o_pos);
    inline void          DecRef (t_Position o_pos);
    inline t_RefCount    GetRef (t_Position o_pos) const;
    inline bool          IsAlloc (t_Position o_pos) const;
    inline bool          IsFree (t_Position o_pos) const;
    };

//---------------------------------------------------------------------------

template <class t_obj, class t_store>
  inline void gct_RefDList <t_obj, t_store>::IncRef
  (
  t_Position o_pos
  )
    {
    this-> o_Store. IncRef (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_store>
  inline void gct_RefDList <t_obj, t_store>::DecRef
  (
  t_Position o_pos
  )
    {
    this-> o_Store. DecRef (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_store>
  inline t_RefCount gct_RefDList <t_obj, t_store>::GetRef
  (
  t_Position o_pos
  ) const
    {
    return this-> o_Store. GetRef (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_store>
  inline bool gct_RefDList <t_obj, t_store>::IsAlloc
  (
  t_Position o_pos
  ) const
    {
    return this-> o_Store. IsAlloc (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_obj, class t_store>
  inline bool gct_RefDList <t_obj, t_store>::IsFree
  (
  t_Position o_pos
  ) const
    {
    return this-> o_Store. IsFree (o_pos);
    }

//---------------------------------------------------------------------------

#define REF_DLIST_DCL(StoreSpec)                                             \
  template <class t_obj>                                                     \
    class TL_TM_EXPORT g ## StoreSpec ## RefDList:                           \
      public gct_RefDList <t_obj, StoreSpec ## RefStore> { };

#define BLOCKREF_DLIST_DCL(StoreSpec)                                        \
  template <class t_obj>                                                     \
    class TL_TM_EXPORT g ## StoreSpec ## BlockRefDList:                      \
      public gct_RefDList <t_obj, StoreSpec ## BlockRefStore> { };

#define REFPTR_DLIST_DCL(StoreSpec)                                          \
  template <class t_obj>                                                     \
    class TL_TM_EXPORT g ## StoreSpec ## RefPtrDList: public                 \
      gct_PtrContainer <t_obj, g ## StoreSpec ## RefDList <void *> > { };

#define BLOCKREFPTR_DLIST_DCL(StoreSpec)                                     \
  template <class t_obj>                                                     \
    class TL_TM_EXPORT g ## StoreSpec ## BlockRefPtrDList: public            \
      gct_PtrContainer <t_obj, g ## StoreSpec ## BlockRefDList <void *> > { };

#define REF_DLIST_DCLS(Obj)              \
  REF_DLIST_DCL (ct_ ## Obj ## _)        \
  REF_DLIST_DCL (ct_ ## Obj ## 8)        \
  REF_DLIST_DCL (ct_ ## Obj ## 16)       \
  REF_DLIST_DCL (ct_ ## Obj ## 32)

#define BLOCKREF_DLIST_DCLS(Obj)         \
  BLOCKREF_DLIST_DCL (ct_ ## Obj ## _)   \
  BLOCKREF_DLIST_DCL (ct_ ## Obj ## 8)   \
  BLOCKREF_DLIST_DCL (ct_ ## Obj ## 16)  \
  BLOCKREF_DLIST_DCL (ct_ ## Obj ## 32)

#define REFPTR_DLIST_DCLS(Obj)           \
  REFPTR_DLIST_DCL (ct_ ## Obj ## _)     \
  REFPTR_DLIST_DCL (ct_ ## Obj ## 8)     \
  REFPTR_DLIST_DCL (ct_ ## Obj ## 16)    \
  REFPTR_DLIST_DCL (ct_ ## Obj ## 32)

#define BLOCKREFPTR_DLIST_DCLS(Obj)        \
  BLOCKREFPTR_DLIST_DCL (ct_ ## Obj ## _)  \
  BLOCKREFPTR_DLIST_DCL (ct_ ## Obj ## 8)  \
  BLOCKREFPTR_DLIST_DCL (ct_ ## Obj ## 16) \
  BLOCKREFPTR_DLIST_DCL (ct_ ## Obj ## 32)

#endif
