
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
// File samples/tblockstore.cpp

#include <stdio.h>
#include "tuning/block.h"
#include "tuning/pageblock.hpp"
#include "tuning/blockstore.h"
#include "tuning/packstore.hpp"
#include "tuning/sys/cprocess.hpp"
#include "samples/ptrid.cpp"

//---------------------------------------------------------------------------

class TL_EXPORT ct_TestPageBlock: public ct_PageBlockBase
  {
public:
  //typedef t_UInt16     t_Size;

protected:
  virtual void *       AllocPtr (ct_PageBlockBase::t_Size o_size);
  virtual void *       ReallocPtr (void * pv_mem, ct_PageBlockBase::t_Size o_size);
  virtual void *       AllocData (ct_PageBlockBase::t_Size o_size);
  virtual void         FreeData (void * pv_mem);
  virtual void         LastPageWarning ();
  virtual void         LastPageError ();

public:
                       ~ct_TestPageBlock ();
  };

//---------------------------------------------------------------------------

void * ct_TestPageBlock::AllocPtr
(
ct_PageBlockBase::t_Size o_size
)
  {
  printf ("* ct_TestPageBlock::AllocPtr\n");
  return ct_TestStore::Alloc ((ct_TestStore::t_Size) o_size);
  }

//---------------------------------------------------------------------------

void * ct_TestPageBlock::ReallocPtr
(
void * pv_mem,
ct_PageBlockBase::t_Size o_size
)
  {
  printf ("* ct_TestPageBlock::ReallocPtr\n");
  return ct_TestStore::Realloc (pv_mem, (ct_TestStore::t_Size) o_size);
  }

//---------------------------------------------------------------------------

void * ct_TestPageBlock::AllocData
(
ct_PageBlockBase::t_Size o_size
)
  {
  printf ("* ct_TestPageBlock::AllocData\n");
  return ct_TestStore::Alloc ((ct_TestStore::t_Size) o_size);
  }

//---------------------------------------------------------------------------

void ct_TestPageBlock::FreeData
(
void * pv_mem
)
  {
  printf ("* ct_TestPageBlock::FreeData\n");
  ct_TestStore::Free (pv_mem);
  }

//---------------------------------------------------------------------------

void ct_TestPageBlock::LastPageWarning ()
  {
  printf ("* * * ct_TestPageBlock::LastPageWarning * * *\n");
  }

//---------------------------------------------------------------------------

void ct_TestPageBlock::LastPageError ()
  {
  printf ("* * * ct_TestPageBlock::LastPageError * * *\n");
  fflush (stdout);
  tl_EndProcess (0); // Return 0 for automatic test
  }

//---------------------------------------------------------------------------

ct_TestPageBlock::~ct_TestPageBlock ()
  {
  printf ("* ~ct_TestPageBlock\n");
  SetByteSize0 ();
  }

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

class TL_EXPORT ct_TestPackStore: public ct_PackStoreBase
  {
protected:
  virtual void *       ReallocPtr (void * pv_mem, t_Size o_size);
  virtual t_UInt       MaxDataAlloc () const;
  virtual void *       AllocData (t_Size o_size);
  virtual void         FreeData (void * pv_mem);

public:
                       ~ct_TestPackStore ();
  };

//---------------------------------------------------------------------------

void * ct_TestPackStore::ReallocPtr
(
void * pv_mem,
t_Size o_size
)
  {
  return ct_TestStore::Realloc (pv_mem, (ct_TestStore::t_Size) o_size);
  }

//---------------------------------------------------------------------------

t_UInt ct_TestPackStore::MaxDataAlloc () const
  {
  return ct_TestStore::MaxAlloc ();
  }

//---------------------------------------------------------------------------

void * ct_TestPackStore::AllocData
(
t_Size o_size
)
  {
  return ct_TestStore::Alloc ((ct_TestStore::t_Size) o_size);
  }

//---------------------------------------------------------------------------

void ct_TestPackStore::FreeData
(
void * pv_mem
)
  {
  ct_TestStore::Free (pv_mem);
  }

//---------------------------------------------------------------------------

ct_TestPackStore::~ct_TestPackStore ()
  {
  FreeAll ();
  }

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void TestPackStore ()
  {
    {
    ct_TestPackStore o_packStore;
    o_packStore. Init (8, 64, 32);
    void * pv = o_packStore. Alloc (10);
    printf ("PackStore::Alloc: 10 p%d\n", PtrId (pv));
    pv = o_packStore. Alloc (33);
    printf ("PackStore::Alloc: 33 p%d\n", PtrId (pv));
    pv = o_packStore. Alloc (11);
    printf ("PackStore::Alloc: 11 p%d\n", PtrId (pv));
    pv = o_packStore. Alloc (12);
    printf ("PackStore::Alloc: 12 p%d\n", PtrId (pv));
    pv = o_packStore. Alloc (13);
    printf ("PackStore::Alloc: 13 p%d\n", PtrId (pv));
    pv = o_packStore. Alloc (14);
    printf ("PackStore::Alloc: 14 p%d\n", PtrId (pv));
    pv = o_packStore. Alloc (32);
    printf ("PackStore::Alloc: 32 p%d\n", PtrId (pv));
    pv = o_packStore. Alloc (15);
    printf ("PackStore::Alloc: 15 p%d\n", PtrId (pv));
    o_packStore. FreeAll (true);
    printf ("PackStore::FreeAll (true)\n");
    pv = o_packStore. Alloc (4);
    printf ("PackStore::Alloc: 4 p%d\n", PtrId (pv));
    o_packStore. FreeAll (false);
    printf ("PackStore::FreeAll (false)\n");
    pv = o_packStore. Alloc (5);
    printf ("PackStore::Alloc: 5 p%d\n", PtrId (pv));
    }

  printf ("~PackStore\n");
  }

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

template <class t_itemBlock>
  class gct_TestBlockStore: public gct_BlockStore
    <t_itemBlock, gct_CharBlock <gct_EmptyBaseBlock <ct_TestStore>, char> >
    {
  public:
    void                 BeginPageStore () { }
    void                 TestPageStore () { }
    void                 TestPageError () { }
    };

//---------------------------------------------------------------------------

template <class t_itemBlock>
  class gct_TestPageBlockStore: public gct_TestBlockStore <t_itemBlock>
    {
  public:
    void                 BeginPageStore ();
    void                 TestPageStore ();
    void                 TestPageError ();
    };

//---------------------------------------------------------------------------

template <class t_itemBlock>
  void gct_TestPageBlockStore <t_itemBlock>::BeginPageStore ()
    {
    this-> SetFixPagePtrs (2);
    }

//---------------------------------------------------------------------------

template <class t_itemBlock>
  void gct_TestPageBlockStore <t_itemBlock>::TestPageStore ()
    {
    printf ("* * * TestPageStore * * *\n");
    printf ("ct_PageBlock::GetFixPagePtrs: %d\n", (unsigned) this-> GetFixPagePtrs ());
    printf ("ct_PageBlock::GetPageSize   : %d\n", (unsigned) this-> GetPageSize ());
    printf ("ct_PageBlock::GetRoundedSize: %d\n", (unsigned) this-> GetRoundedSize ());
    printf ("ct_PageBlock::GetMaxItemSize: %" TL_FMT_UINT "d\n", this-> GetMaxItemSize ());
    }

//---------------------------------------------------------------------------

template <class t_itemBlock>
  void gct_TestPageBlockStore <t_itemBlock>::TestPageError ()
    {
    this-> Alloc (1);
    this-> Alloc (1);
    this-> Alloc (1);
    this-> Alloc (1);
    this-> Alloc (1);
    this-> Alloc (1); // LastPageError
    }

//---------------------------------------------------------------------------

template <class t_blockStore>
  void TestBlockStore
  (
  bool b_testPageStore
  )
    {
      {
      //const unsigned cu_testSize = 16;
      const unsigned cu_testSize = 20;
      t_blockStore co_blockStore;
      co_blockStore. SetPageSize (64);

      if (b_testPageStore)
        co_blockStore. BeginPageStore ();

      TL_TYPENAME t_blockStore::t_Position o_pos0 = co_blockStore. Alloc (cu_testSize);
      printf ("BlockStore::Alloc: pos0 %u  addr0 p%d\n", (unsigned) o_pos0,
        PtrId (co_blockStore. AddrOf (o_pos0)));
      TL_TYPENAME t_blockStore::t_Position o_pos1 = co_blockStore. Alloc (cu_testSize);
      printf ("BlockStore::Alloc: pos1 %u  addr0 p%d  addr1 p%d\n", (unsigned) o_pos1,
        PtrId (co_blockStore. AddrOf (o_pos0)), PtrId (co_blockStore. AddrOf (o_pos1)));
      TL_TYPENAME t_blockStore::t_Position o_pos2 = co_blockStore. Alloc (cu_testSize);
      printf ("BlockStore::Alloc: pos2 %u  addr0 p%d  addr2 p%d\n", (unsigned) o_pos2,
        PtrId (co_blockStore. AddrOf (o_pos0)), PtrId (co_blockStore. AddrOf (o_pos2)));
      TL_TYPENAME t_blockStore::t_Position o_pos3 = co_blockStore. Alloc (cu_testSize);
      printf ("BlockStore::Alloc: pos3 %u  addr0 p%d  addr3 p%d\n", (unsigned) o_pos3,
        PtrId (co_blockStore. AddrOf (o_pos0)), PtrId (co_blockStore. AddrOf (o_pos3)));
      TL_TYPENAME t_blockStore::t_Position o_pos4 = co_blockStore. Alloc (cu_testSize);
      printf ("BlockStore::Alloc: pos4 %u  addr0 p%d  addr4 p%d\n", (unsigned) o_pos4,
        PtrId (co_blockStore. AddrOf (o_pos0)), PtrId (co_blockStore. AddrOf (o_pos4)));
      co_blockStore. Free (o_pos3);
      printf ("BlockStore::Free : %u\n", (unsigned) o_pos3);
      co_blockStore. Free (o_pos4);
      printf ("BlockStore::Free : %u\n", (unsigned) o_pos4);
      co_blockStore. FreeUnused ();
      printf ("BlockStore::FreeUnused\n");
      co_blockStore. TestPageStore ();

      if (b_testPageStore)
        co_blockStore. TestPageError ();
      }

    printf ("~BlockStore\n");
    }

//---------------------------------------------------------------------------

int main ()
  {
  printf ("\nct_TestPackStore\n\n");
  TestPackStore ();
  DelPtrIds ();

  printf ("\ngct_VarItemBlock <gct_EmptyBaseBlock <ct_TestStore> >\n\n");
  TestBlockStore <gct_TestBlockStore <gct_VarItemBlock <gct_EmptyBaseBlock <ct_TestStore> > > > (false);
  DelPtrIds ();

  printf ("\nct_TestPageBlock\n\n");
  TestBlockStore <gct_TestPageBlockStore <ct_TestPageBlock> > (false);
  DelPtrIds ();

  printf ("\nct_TestPageBlock 2\n\n");
  TestBlockStore <gct_TestPageBlockStore <ct_TestPageBlock> > (true);
  DelPtrIds ();

  return 0;
  }
