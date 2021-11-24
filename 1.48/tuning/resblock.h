
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
// File tuning/resblock.h

#ifndef TUNING_RESBLOCK_H
#define TUNING_RESBLOCK_H

#include "tuning/object.hpp"
#include "tuning/sys/cmemory.hpp"

//---------------------------------------------------------------------------

template <class t_staticStore, class t_base>
  class TL_TM_EXPORT gct_ResBlockBase: public t_base
    {
  public:
    typedef t_staticStore t_StaticStore;
    typedef TL_TYPENAME t_StaticStore::t_Size t_Size;

  protected:
    TL_TYPENAME t_StaticStore::t_Position o_Pos;
    t_Size               o_Size;
    t_Size               o_MinSize;

  public:
    inline void          Swap (gct_ResBlockBase & co_swap);
    inline TL_TYPENAME t_StaticStore::t_Store * GetStore () const
      { return t_StaticStore::GetStore (); }
    };

//---------------------------------------------------------------------------

template <class t_staticStore, class t_base>
  inline void gct_ResBlockBase <t_staticStore, t_base>::Swap
  (
  gct_ResBlockBase & co_swap
  )
    {
    this-> t_base::Swap (co_swap);
    tl_SwapObj (o_Pos, co_swap. o_Pos);
    tl_SwapObj (o_Size, co_swap. o_Size);
    tl_SwapObj (o_MinSize, co_swap. o_MinSize);
    }

//---------------------------------------------------------------------------

template <class t_resBlockBase>
  class TL_TM_EXPORT gct_ResBlock: public t_resBlockBase
    {
  public:
    typedef TL_TYPENAME t_resBlockBase::t_Size t_Size;
    typedef TL_TYPENAME t_resBlockBase::t_StaticStore t_StaticStore;

    inline               gct_ResBlock ();
    inline               gct_ResBlock (const gct_ResBlock & co_init);
    inline               ~gct_ResBlock ();
    inline gct_ResBlock & operator = (const gct_ResBlock & co_asgn);

    static inline t_UInt GetMaxByteSize ();
    inline t_Size        GetByteSize () const;
    inline void          SetByteSize (t_Size o_newSize);
    inline void *        GetAddr () const;

    inline t_Size        GetMinByteSize () const;
    inline t_Size        GetAllocByteSize () const;
    inline void          SetMinByteSize (t_Size o_newSize);
    };

//---------------------------------------------------------------------------

template <class t_resBlockBase>
  inline gct_ResBlock <t_resBlockBase>::gct_ResBlock ()
    {
    this-> o_Size = 0;
    this-> o_MinSize = 0;
    this-> o_Pos = 0;
    }

//---------------------------------------------------------------------------

template <class t_resBlockBase>
  inline gct_ResBlock <t_resBlockBase>::gct_ResBlock
  (
  const gct_ResBlock & co_init
  )
    {
    this-> o_Size = co_init. o_Size;
    this-> o_MinSize = co_init. o_MinSize;
    this-> o_Pos = t_StaticStore::Alloc (tl_Max (this-> o_Size, this-> o_MinSize));
    tl_CopyMemory ((char *) GetAddr (), (char *) co_init. GetAddr (), this-> o_Size);
    }

//---------------------------------------------------------------------------

template <class t_resBlockBase>
  inline gct_ResBlock <t_resBlockBase>::~gct_ResBlock ()
    {
    t_StaticStore::Free (this-> o_Pos);
    }

//---------------------------------------------------------------------------

template <class t_resBlockBase>
  inline gct_ResBlock <t_resBlockBase> &
  gct_ResBlock <t_resBlockBase>::operator =
  (
  const gct_ResBlock & co_asgn
  )
    {
    if (& co_asgn != this)
      {
      t_Size o_oldMax = tl_Max (this-> o_Size, this-> o_MinSize);
      this-> o_Size = co_asgn. o_Size;
      this-> o_MinSize = co_asgn. o_MinSize;
      t_Size o_newMax = tl_Max (this-> o_Size, this-> o_MinSize);

      if (o_oldMax != o_newMax)
        this-> o_Pos = t_StaticStore::Realloc (this-> o_Pos, o_newMax);

      tl_CopyMemory ((char *) GetAddr (), (char *) co_asgn. GetAddr (), this-> o_Size);
      }

    return * this;
    }

//---------------------------------------------------------------------------

template <class t_blockBase>
  inline t_UInt gct_ResBlock <t_blockBase>::GetMaxByteSize ()
    {
    return TL_TYPENAME t_StaticStore::MaxAlloc ();
    }

//---------------------------------------------------------------------------

template <class t_resBlockBase>
  inline TL_TYPENAME gct_ResBlock <t_resBlockBase>::t_Size
  gct_ResBlock <t_resBlockBase>::GetByteSize () const
    {
    return this-> o_Size;
    }

//---------------------------------------------------------------------------

template <class t_resBlockBase>
  inline void gct_ResBlock <t_resBlockBase>::SetByteSize
  (
  t_Size o_newSize
  )
    {
    if (this-> o_Size != o_newSize)
      {
      t_Size o_oldMax = tl_Max (this-> o_Size, this-> o_MinSize);
      this-> o_Size = o_newSize;
      t_Size o_newMax = tl_Max (this-> o_Size, this-> o_MinSize);

      if (o_oldMax != o_newMax)
        this-> o_Pos = t_StaticStore::Realloc (this-> o_Pos, o_newMax);
      }
    }

//---------------------------------------------------------------------------

template <class t_resBlockBase>
  inline void * gct_ResBlock <t_resBlockBase>::GetAddr () const
    {
    return t_StaticStore::AddrOf (this-> o_Pos);
    }

//---------------------------------------------------------------------------

template <class t_resBlockBase>
  inline TL_TYPENAME gct_ResBlock <t_resBlockBase>::t_Size
  gct_ResBlock <t_resBlockBase>::GetMinByteSize () const
    {
    return this-> o_MinSize;
    }

//---------------------------------------------------------------------------

template <class t_resBlockBase>
  inline TL_TYPENAME gct_ResBlock <t_resBlockBase>::t_Size
  gct_ResBlock <t_resBlockBase>::GetAllocByteSize () const
    {
    return tl_Max (this-> o_Size, this-> o_MinSize);
    }

//---------------------------------------------------------------------------

template <class t_resBlockBase>
  inline void gct_ResBlock <t_resBlockBase>::SetMinByteSize
  (
  t_Size o_newSize
  )
    {
    if (this-> o_MinSize != o_newSize)
      {
      t_Size o_oldMax = tl_Max (this-> o_Size, this-> o_MinSize);
      this-> o_MinSize = o_newSize;
      t_Size o_newMax = tl_Max (this-> o_Size, this-> o_MinSize);

      if (o_oldMax != o_newMax)
        this-> o_Pos = t_StaticStore::Realloc (this-> o_Pos, o_newMax);
      }
    }

//---------------------------------------------------------------------------

template <class t_staticStore>
  class TL_TM_EXPORT gct_EmptyBaseResBlock:
    public gct_ResBlock <gct_ResBlockBase <t_staticStore, ct_Empty> >
    {
    };

//---------------------------------------------------------------------------

template <class t_staticStore>
  class TL_TM_EXPORT gct_ObjectBaseResBlock:
    public gct_ResBlock <gct_ResBlockBase <t_staticStore, ct_Object> >
    {
    };

#endif
