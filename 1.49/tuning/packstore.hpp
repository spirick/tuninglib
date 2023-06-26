
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
// File tuning/packstore.hpp

#ifndef TUNING_PACKSTORE_HPP
#define TUNING_PACKSTORE_HPP

#include "tuning/defs.hpp"

//---------------------------------------------------------------------------

class TL_EXPORT ct_PackStoreBase
  {
public:
  typedef t_UInt           t_Size;
  typedef void *           t_Position;

private:
  struct st_Data
    {
    char * *                 ppc_Pages;
    char *                   pc_FirstPage;
    t_Size                   o_Pages;
    t_Size                   o_CurrPage;
    t_Size                   o_CurrPageSize;
    t_Size                   o_Align;
    t_Size                   o_AlignMask;
    t_Size                   o_PageSize;
    t_Size                   o_OwnPageSize;
    }                      so_Data;

  // Do not copy this object
                           ct_PackStoreBase (const ct_PackStoreBase &);
  ct_PackStoreBase &       operator = (const ct_PackStoreBase &);

protected:
  virtual void *           ReallocPtr (void * pv_mem, t_Size o_size) = 0;
  virtual t_UInt           MaxDataAlloc () const = 0;
  virtual void *           AllocData (t_Size o_size) = 0;
  virtual void             FreeData (void * pv_mem) = 0;

public:
                           ct_PackStoreBase ();
  virtual                  ~ct_PackStoreBase () { }
  void                     Swap (ct_PackStoreBase & co_swap);

  static inline t_UInt     StoreInfoSize ();
  inline t_UInt            MaxAlloc ();

  t_Position               Alloc (t_Size o_size);
  t_Position               Realloc (t_Position o_pos, t_Size o_size);
  void                     Free (t_Position o_pos);

  static inline void *     AddrOf (t_Position o_pos);
  static inline t_Position PosOf (void * pv_adr);

  t_Size                   SizeOf (t_Position o_pos);
  t_Size                   RoundedSizeOf (t_Position o_pos);

  bool                     CanFreeAll ();
  void                     FreeAll (bool b_keepPage = false);

  bool                     Init (t_Size o_align, t_Size o_pageSize,
                             t_Size o_ownPageSize = 0);
  };

//---------------------------------------------------------------------------

class TL_EXPORT ct_PackStore: public ct_PackStoreBase
  {
protected:
  virtual void *       ReallocPtr (void * pv_mem, t_Size o_size);
  virtual t_UInt       MaxDataAlloc () const;
  virtual void *       AllocData (t_Size o_size);
  virtual void         FreeData (void * pv_mem);

public:
                       ~ct_PackStore ();
  };

//---------------------------------------------------------------------------

inline t_UInt ct_PackStoreBase::StoreInfoSize ()
  {
  return 0;
  }

//---------------------------------------------------------------------------

inline t_UInt ct_PackStoreBase::MaxAlloc ()
  {
  return MaxDataAlloc ();
  }

//---------------------------------------------------------------------------

inline void * ct_PackStoreBase::AddrOf
(
t_Position o_pos
)
  {
  return o_pos;
  }

//---------------------------------------------------------------------------

inline ct_PackStoreBase::t_Position ct_PackStoreBase::PosOf
(
void * pv_adr
)
  {
  return pv_adr;
  }

#endif
