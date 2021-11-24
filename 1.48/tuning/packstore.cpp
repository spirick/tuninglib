
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
// File tuning/packstore.cpp

#include "tuning/packstore.hpp"
#include "tuning/std/store.hpp"
#include "tuning/chn/store.hpp"
#include "tuning/sys/cmemory.hpp"

//---------------------------------------------------------------------------

ct_PackStoreBase::ct_PackStoreBase ()
  {
  so_Data. ppc_Pages = 0;
  so_Data. pc_FirstPage = 0;
  Init (8, 0x1000); // 4 KB
  }

//---------------------------------------------------------------------------

void ct_PackStoreBase::Swap
(
ct_PackStoreBase & co_swap
)
  {
  tl_SwapObj (so_Data, co_swap. so_Data);
  }

//---------------------------------------------------------------------------

ct_PackStoreBase::t_Position ct_PackStoreBase::Alloc
(
t_Size o_size
)
  {
  if (o_size == 0)
    return 0;

  if (o_size > so_Data. o_OwnPageSize)
    {
    t_Size o_page = so_Data. o_Pages;
    so_Data. o_Pages ++;
    so_Data. ppc_Pages = (char * *) ReallocPtr (so_Data. ppc_Pages, so_Data. o_Pages * sizeof (char *));
    return so_Data. ppc_Pages [o_page] = (char *) AllocData (o_size);
    }

  o_size = (o_size + so_Data. o_Align) & so_Data. o_AlignMask;

  if (so_Data. o_CurrPageSize + o_size > so_Data. o_PageSize)
    {
    so_Data. o_CurrPage = so_Data. o_Pages;
    so_Data. o_CurrPageSize = o_size;
    so_Data. o_Pages ++;
    so_Data. ppc_Pages = (char * *) ReallocPtr (so_Data. ppc_Pages, so_Data. o_Pages * sizeof (char *));
    char * pc = (char *) AllocData (so_Data. o_PageSize);

    if (so_Data. pc_FirstPage == 0)
      so_Data. pc_FirstPage = pc;

    return so_Data. ppc_Pages [so_Data. o_CurrPage] = pc;
    }

  t_Size o_pos = so_Data. o_CurrPageSize;
  so_Data. o_CurrPageSize += o_size;
  return so_Data. ppc_Pages [so_Data. o_CurrPage] + o_pos;
  }

//---------------------------------------------------------------------------

ct_PackStoreBase::t_Position ct_PackStoreBase::Realloc
(
t_Position /*o_pos*/,
t_Size TL_DEBUG_CODE(o_size)
)
  {
  TL_ASSERT (o_size == 0);
  return 0;
  }

//---------------------------------------------------------------------------

void ct_PackStoreBase::Free
(
t_Position /*o_pos*/
)
  {
  }

//---------------------------------------------------------------------------

ct_PackStoreBase::t_Size ct_PackStoreBase::SizeOf
(
t_Position /*o_pos*/
)
  {
  TL_ASSERT (false);
  return 0;
  }

//---------------------------------------------------------------------------

ct_PackStoreBase::t_Size ct_PackStoreBase::RoundedSizeOf
(
t_Position /*o_pos*/
)
  {
  TL_ASSERT (false);
  return 0;
  }

//---------------------------------------------------------------------------

bool ct_PackStoreBase::CanFreeAll ()
  {
  return true;
  }

//---------------------------------------------------------------------------

void ct_PackStoreBase::FreeAll
(
bool b_keepPage
)
  {
  if (b_keepPage && (so_Data. pc_FirstPage != 0))
    {
    for (t_Size o_page = 0; o_page < so_Data. o_Pages; o_page ++)
      if (so_Data. ppc_Pages [o_page] != so_Data. pc_FirstPage)
        FreeData (so_Data. ppc_Pages [o_page]);

    so_Data. o_CurrPage = 0;
    so_Data. o_CurrPageSize = 0;
    so_Data. o_Pages = 1;
    so_Data. ppc_Pages = (char * *) ReallocPtr (so_Data. ppc_Pages, sizeof (char *));
    so_Data. ppc_Pages [0] = so_Data. pc_FirstPage;
    }
  else
    {
    for (t_Size o_page = 0; o_page < so_Data. o_Pages; o_page ++)
      FreeData (so_Data. ppc_Pages [o_page]);

    so_Data. o_CurrPage = 0;
    so_Data. o_CurrPageSize = so_Data. o_PageSize;
    so_Data. o_Pages = 0;
    so_Data. ppc_Pages = (char * *) ReallocPtr (so_Data. ppc_Pages, 0);
    so_Data. pc_FirstPage = 0;
    }
  }

//---------------------------------------------------------------------------

bool ct_PackStoreBase::Init
(
t_Size o_align,
t_Size o_pageSize,
t_Size o_ownPageSize
)
  {
  if (so_Data. ppc_Pages != 0)
    return false;

  if ((o_align != 1) && (o_align != 2) && (o_align != 4) &&
      (o_align != 8) && (o_align != 16))
    return false;

  if (o_pageSize < 64)
    return false;

  if (o_ownPageSize > o_pageSize / 2)
    return false;

  if (o_ownPageSize == 0)
    o_ownPageSize = o_pageSize / 4;

  if (o_ownPageSize < 16)
    return false;

  so_Data. o_Pages = 0;
  so_Data. o_CurrPage = 0;
  so_Data. o_CurrPageSize = o_pageSize;
  so_Data. o_Align = o_align - 1;
  so_Data. o_AlignMask = ~so_Data. o_Align;
  so_Data. o_PageSize = o_pageSize;
  so_Data. o_OwnPageSize = o_ownPageSize;
  return true;
  }

//---------------------------------------------------------------------------

typedef ct_Chn_Store t_PackStorePtrStore;
typedef ct_Std_Store t_PackStoreDataStore;

//---------------------------------------------------------------------------

void * ct_PackStore::ReallocPtr
(
void * pv_mem,
t_Size o_size
)
  {
  return t_PackStorePtrStore::Realloc (pv_mem, o_size);
  }

//---------------------------------------------------------------------------

t_UInt ct_PackStore::MaxDataAlloc () const
  {
  return t_PackStoreDataStore::MaxAlloc ();
  }

//---------------------------------------------------------------------------

void * ct_PackStore::AllocData
(
t_Size o_size
)
  {
  return t_PackStoreDataStore::Alloc (o_size);
  }

//---------------------------------------------------------------------------

void ct_PackStore::FreeData
(
void * pv_mem
)
  {
  t_PackStoreDataStore::Free (pv_mem);
  }

//---------------------------------------------------------------------------

ct_PackStore::~ct_PackStore ()
  {
  FreeAll ();
  }
