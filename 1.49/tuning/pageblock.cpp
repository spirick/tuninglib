
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
// File tuning/pageblock.cpp

#include "tuning/pageblock.hpp"
#include "tuning/std/store.hpp"
#include "tuning/chn/store.hpp"
#include "tuning/sys/cmemory.hpp"
#include "tuning/sys/cprocess.hpp"

//---------------------------------------------------------------------------

void ct_PageBlockBase::CopyFrom
(
const ct_PageBlockBase & co_copy
)
  {
  so_Data. o_FixSize      = co_copy. so_Data. o_FixSize;
  so_Data. o_FixPagePtrs  = co_copy. so_Data. o_FixPagePtrs;
  so_Data. o_OrigPageSize = co_copy. so_Data. o_OrigPageSize;
  so_Data. o_MaxItemSize  = co_copy. so_Data. o_MaxItemSize;
  so_Data. o_Size         = co_copy. so_Data. o_Size;
  so_Data. o_PageSize     = co_copy. so_Data. o_PageSize;
  so_Data. o_RoundedSize  = co_copy. so_Data. o_RoundedSize;
  so_Data. u_Shift        = co_copy. so_Data. u_Shift;
  so_Data. u_Mask         = co_copy. so_Data. u_Mask;

  if (so_Data. o_Size == 0)
    so_Data. ppc_Pages = 0;
  else
    {
    t_Size o_pages = Page (so_Data. o_Size - 1) + 1;
    so_Data. ppc_Pages = (char * *) AllocPtr (tl_Max (o_pages, so_Data. o_FixPagePtrs) * sizeof (char *));

    for (t_Size o_page = 0; o_page < o_pages; o_page ++)
      {
      so_Data. ppc_Pages [o_page] = (char *) AllocData (so_Data. o_PageSize);
      tl_CopyMemory (so_Data. ppc_Pages [o_page], co_copy. so_Data. ppc_Pages [o_page],
        so_Data. o_PageSize);
      }
    }
  }

//---------------------------------------------------------------------------

void ct_PageBlockBase::SetByteSize
(
t_Size o_size
)
  {
  so_Data. o_Size = o_size;

  if ((o_size > so_Data. o_RoundedSize) || (o_size + so_Data. o_PageSize <= so_Data. o_RoundedSize))
    {
    t_Size o_oldPages = so_Data. o_RoundedSize / so_Data. o_PageSize;
    t_Size o_newPages = o_size != 0 ? Page (o_size - 1) + 1 : 0;
    so_Data. o_RoundedSize = o_newPages * so_Data. o_PageSize;

    if (o_oldPages < o_newPages)
      {
      if (so_Data. o_FixPagePtrs == 0)
        so_Data. ppc_Pages = (char * *) ReallocPtr (so_Data. ppc_Pages, o_newPages * sizeof (char *));
      else
        {
        if ((o_oldPages == 0) && (o_newPages <= so_Data. o_FixPagePtrs))
          so_Data. ppc_Pages = (char * *) AllocPtr (so_Data. o_FixPagePtrs * sizeof (char *));
        else
          {
          if (o_newPages == so_Data. o_FixPagePtrs)
            LastPageWarning ();
          else if (o_newPages > so_Data. o_FixPagePtrs)
            {
            LastPageError ();
            tl_EndProcess (1); // Out of memory
            }
          }
        }

      for (t_Size o_page = o_oldPages; o_page < o_newPages; o_page ++)
        so_Data. ppc_Pages [o_page] = (char *) AllocData (so_Data. o_PageSize);
      }
    else
      {
      TL_ASSERT (o_oldPages > o_newPages);

      for (t_Size o_page = o_newPages; o_page < o_oldPages; o_page ++)
        FreeData (so_Data. ppc_Pages [o_page]);

      if ((o_newPages == 0) || (so_Data. o_FixPagePtrs == 0))
        so_Data. ppc_Pages = (char * *) ReallocPtr (so_Data. ppc_Pages, o_newPages * sizeof (char *));
      }
    }
  }

//---------------------------------------------------------------------------

void ct_PageBlockBase::SetByteSize0 ()
  {
  SetByteSize (0);
  }

//---------------------------------------------------------------------------

ct_PageBlockBase::ct_PageBlockBase ()
  {
  so_Data. o_FixSize = 0;
  so_Data. o_FixPagePtrs = 0;
  so_Data. o_OrigPageSize = 0;
  so_Data. o_MaxItemSize = 0;
  so_Data. o_Size = 0;
  so_Data. o_RoundedSize = 0;
  so_Data. ppc_Pages = 0;
  AlignPageSize (1, GetDefaultPageSize ());
  }

//---------------------------------------------------------------------------

void ct_PageBlockBase::Swap
(
ct_PageBlockBase & co_swap
)
  {
  tl_SwapObj (so_Data, co_swap. so_Data);
  }

//---------------------------------------------------------------------------

char * ct_PageBlockBase::InsertChars
(
t_Size o_pos,
t_Size o_count
)
  {
  TL_ASSERT (o_pos <= so_Data. o_Size);

  IncCharSize (o_count);

  t_Size o_posPage = Page (o_pos);
  t_Size o_posOffs = Offset (o_pos);
  char * pc_pos = so_Data. ppc_Pages [o_posPage] + o_posOffs;

  if (o_count != 0)
    {
    t_Size o_sizePage = Page (so_Data. o_Size);
    t_Size o_sizeOffs = Offset (so_Data. o_Size);

    if (o_sizeOffs == 0)
      {
      o_sizePage --;
      o_sizeOffs = so_Data. o_PageSize;
      }

    TL_ASSERT (o_count <= o_sizeOffs);
    TL_ASSERT (o_posOffs + o_count <= so_Data. o_PageSize);

    if (o_posPage == o_sizePage)
      {
      tl_MoveMemory (pc_pos + o_count,
                     pc_pos,
                     o_sizeOffs - o_posOffs - o_count);
      }
    else
      {
      tl_MoveMemory (so_Data. ppc_Pages [o_sizePage] + o_count,
                     so_Data. ppc_Pages [o_sizePage],
                     o_sizeOffs - o_count);

      for (;;)
        {
        tl_CopyMemory (so_Data. ppc_Pages [o_sizePage],
                       so_Data. ppc_Pages [o_sizePage - 1] + so_Data. o_PageSize - o_count,
                       o_count);
        o_sizePage --;

        if (o_sizePage != o_posPage)
          tl_MoveMemory (so_Data. ppc_Pages [o_sizePage] + o_count,
                         so_Data. ppc_Pages [o_sizePage],
                         so_Data. o_PageSize - o_count);
        else
          break;
        }

      tl_MoveMemory (pc_pos + o_count,
                     pc_pos,
                     so_Data. o_PageSize - o_posOffs - o_count);
      }
    }

  return pc_pos;
  }

//---------------------------------------------------------------------------

char * ct_PageBlockBase::DeleteChars
(
t_Size o_pos,
t_Size o_count
)
  {
  TL_DEBUG_CODE
  (
  t_Size o_countPos = o_pos + o_count;
  TL_ASSERT (o_countPos >= o_count); // Overflow
  TL_ASSERT (o_countPos <= so_Data. o_Size);
  )

  t_Size o_posPage = Page (o_pos);
  t_Size o_posOffs = Offset (o_pos);
  char * pc_pos = so_Data. ppc_Pages [o_posPage] + o_posOffs;

  if (o_count != 0)
    {
    t_Size o_sizePage = Page (so_Data. o_Size);
    t_Size o_sizeOffs = Offset (so_Data. o_Size);

    if (o_sizeOffs == 0)
      {
      o_sizePage --;
      o_sizeOffs = so_Data. o_PageSize;
      }

    TL_ASSERT (o_posOffs + o_count <= so_Data. o_PageSize);
    TL_ASSERT (o_count <= o_sizeOffs);

    if (o_posPage == o_sizePage)
      {
      tl_MoveMemory (pc_pos,
                     pc_pos + o_count,
                     o_sizeOffs - o_posOffs - o_count);
      }
    else
      {
      tl_MoveMemory (pc_pos,
                     pc_pos + o_count,
                     so_Data. o_PageSize - o_posOffs - o_count);

      for (;;)
        {
        tl_CopyMemory (so_Data. ppc_Pages [o_posPage] + so_Data. o_PageSize - o_count,
                       so_Data. ppc_Pages [o_posPage + 1],
                       o_count);
        o_posPage ++;

        if (o_posPage != o_sizePage)
          tl_MoveMemory (so_Data. ppc_Pages [o_posPage],
                         so_Data. ppc_Pages [o_posPage] + o_count,
                         so_Data. o_PageSize - o_count);
        else
          break;
        }

      tl_MoveMemory (so_Data. ppc_Pages [o_sizePage],
                     so_Data. ppc_Pages [o_sizePage] + o_count,
                     o_sizeOffs - o_count);
      }

    DecCharSize (o_count);
    }

  return pc_pos;
  }

//---------------------------------------------------------------------------

char * ct_PageBlockBase::FillChars
(
t_Size o_pos,
t_Size o_count,
char c_fill
)
  {
  TL_DEBUG_CODE
  (
  t_Size o_countPos = o_pos + o_count;
  TL_ASSERT (o_countPos >= o_count); // Overflow
  TL_ASSERT (o_countPos <= so_Data. o_Size);
  )

  t_Size o_posPage = Page (o_pos);
  t_Size o_posOffs = Offset (o_pos);
  char * pc_pos = so_Data. ppc_Pages [o_posPage] + o_posOffs;

  if (o_count != 0)
    {
    t_Size o_endPage = Page (o_pos + o_count);
    t_Size o_endOffs = Offset (o_pos + o_count);

    if (o_endOffs == 0)
      {
      o_endPage --;
      o_endOffs = so_Data. o_PageSize;
      }

    if (o_posPage == o_endPage)
      tl_FillMemory (pc_pos, o_count, c_fill);
    else
      {
      tl_FillMemory (pc_pos, so_Data. o_PageSize - o_posOffs, c_fill);

      while (++ o_posPage != o_endPage)
        tl_FillMemory (so_Data. ppc_Pages [o_posPage], so_Data. o_PageSize, c_fill);

      tl_FillMemory (so_Data. ppc_Pages [o_endPage], o_endOffs, c_fill);
      }
    }

  return pc_pos;
  }

//---------------------------------------------------------------------------

void ct_PageBlockBase::SetFixPagePtrs
(
t_Size o_ptrs
)
  {
  if (so_Data. o_Size == 0)
    {
    so_Data. o_FixPagePtrs = o_ptrs;

    if (so_Data. o_FixSize != 0)
      {
      if (o_ptrs == 0)
        so_Data. o_MaxItemSize = ((t_Size) -1) / so_Data. o_FixSize;
      else
        so_Data. o_MaxItemSize = (so_Data. o_PageSize / so_Data. o_FixSize) * o_ptrs;
      }
    }
  }

//---------------------------------------------------------------------------

void ct_PageBlockBase::AlignPageSize
(
t_Size o_fixSize,
t_Size o_pageSize
)
  {
  TL_ASSERT (o_fixSize != 0);
  TL_ASSERT (o_fixSize <= o_pageSize);

  if (so_Data. o_Size == 0)
    {
    so_Data. o_FixSize = o_fixSize;
    so_Data. o_OrigPageSize = o_pageSize;
    t_Size o_pageIdxSize = (o_pageSize - 1) / o_fixSize;
    so_Data. u_Shift = 1;

    while ((o_pageIdxSize >>= 1) != 0)
      so_Data. u_Shift ++;

    so_Data. u_Mask = (((t_Size) 1) << so_Data. u_Shift) - 1;
    so_Data. o_PageSize = (((t_Size) 1) << so_Data. u_Shift) * o_fixSize;

    if (so_Data. o_FixPagePtrs == 0)
      so_Data. o_MaxItemSize = ((t_Size) -1) / o_fixSize;
    else
      so_Data. o_MaxItemSize = (so_Data. o_PageSize / o_fixSize) * so_Data. o_FixPagePtrs;
    }

  TL_ASSERT (so_Data. o_FixSize == o_fixSize);
  TL_ASSERT (so_Data. o_OrigPageSize == o_pageSize);
  }

//---------------------------------------------------------------------------

typedef ct_Chn_Store t_PageBlockPtrStore;
typedef ct_Std_Store t_PageBlockDataStore;

//---------------------------------------------------------------------------

void * ct_PageBlock::AllocPtr
(
t_Size o_size
)
  {
  return t_PageBlockPtrStore::Alloc (o_size);
  }

//---------------------------------------------------------------------------

void * ct_PageBlock::ReallocPtr
(
void * pv_mem,
t_Size o_size
)
  {
  return t_PageBlockPtrStore::Realloc (pv_mem, o_size);
  }

//---------------------------------------------------------------------------

void * ct_PageBlock::AllocData
(
t_Size o_size
)
  {
  return t_PageBlockDataStore::Alloc (o_size);
  }

//---------------------------------------------------------------------------

void ct_PageBlock::FreeData
(
void * pv_mem
)
  {
  t_PageBlockDataStore::Free (pv_mem);
  }

//---------------------------------------------------------------------------

ct_PageBlock::~ct_PageBlock () TL_CAN_THROW
  {
  SetByteSize0 ();
  }
