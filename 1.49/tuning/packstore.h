
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
// File tuning/packstore.h

#ifndef TUNING_PACKSTORE_H
#define TUNING_PACKSTORE_H

//---------------------------------------------------------------------------

template <class t_staticStore>
  class TL_TM_EXPORT gct_PackStore
    {
  public:
    typedef t_staticStore t_StaticStore;
    typedef TL_TYPENAME t_StaticStore::t_Size t_Size;
    typedef void *       t_Position;

  private:
    struct st_Data
      {
      char *               pc_FirstPage;
      char *               pc_CurrPage;
      char *               pc_CurrData;
      t_UInt               u_CurrSize;
      t_UInt               u_Align;
      t_UInt               u_AlignMask;
      t_UInt               u_PageSize;
      t_UInt               u_OwnPageSize;
      }                  so_Data;

    // Do not copy this object
                         gct_PackStore (const gct_PackStore &);
    gct_PackStore &      operator = (const gct_PackStore &);

    inline void          AllocPage (t_Size o_size);
    inline void          AllocDataPage ();

  public:
                         gct_PackStore ();
                         ~gct_PackStore ();
    inline void          Swap (gct_PackStore & co_swap);

    static inline t_UInt StoreInfoSize ();
    static inline t_UInt MaxAlloc ();

    t_Position           Alloc (t_Size o_size);
    inline t_Position    Realloc (t_Position o_pos, t_Size o_size);
    inline void          Free (t_Position o_pos);

    static inline void * AddrOf (t_Position o_pos);
    static inline t_Position PosOf (void * pv_adr);

    static inline t_Size SizeOf (t_Position o_pos);
    static inline t_Size RoundedSizeOf (t_Position o_pos);

    static inline bool   CanFreeAll ();
    void                 FreeAll (bool b_keepPage = false);

    bool                 Init (unsigned u_align, unsigned u_pageExp,
                           t_Size o_ownPageSize = 0);
    };

//---------------------------------------------------------------------------

template <class t_staticStore>
  inline void gct_PackStore <t_staticStore>::AllocPage
  (
  t_Size o_size
  )
    {
    char * pc_new = (char *) t_StaticStore::Alloc (o_size);
    * ((char * *) pc_new) = so_Data. pc_CurrPage;
    so_Data. pc_CurrPage = pc_new;
    }

//---------------------------------------------------------------------------

template <class t_staticStore>
  inline void gct_PackStore <t_staticStore>::AllocDataPage ()
    {
    AllocPage (so_Data. u_PageSize);

    if (so_Data. pc_FirstPage == 0)
      so_Data. pc_FirstPage = so_Data. pc_CurrPage;

    so_Data. pc_CurrData = so_Data. pc_CurrPage + sizeof (char *);
    so_Data. u_CurrSize = so_Data. u_PageSize - sizeof (char *);
    }

//---------------------------------------------------------------------------

template <class t_staticStore>
  gct_PackStore <t_staticStore>::gct_PackStore ()
    {
    so_Data. pc_FirstPage = 0;
    so_Data. pc_CurrPage = 0;
    so_Data. pc_CurrData = 0;
    so_Data. u_CurrSize = 0;
    Init (8, 12); // 4 KB
    }

//---------------------------------------------------------------------------

template <class t_staticStore>
  gct_PackStore <t_staticStore>::~gct_PackStore ()
    {
    FreeAll (false);
    }

//---------------------------------------------------------------------------

template <class t_staticStore>
  void gct_PackStore <t_staticStore>::Swap
  (
  gct_PackStore & co_swap
  )
    {
    tl_SwapObj (so_Data, co_swap. so_Data);
    }

//---------------------------------------------------------------------------

template <class t_staticStore>
  inline t_UInt gct_PackStore <t_staticStore>::StoreInfoSize ()
    {
    return 0;
    }

//---------------------------------------------------------------------------

template <class t_staticStore>
  inline t_UInt gct_PackStore <t_staticStore>::MaxAlloc ()
    {
    return t_StaticStore::MaxAlloc ();
    }

//---------------------------------------------------------------------------

template <class t_staticStore>
  TL_TYPENAME gct_PackStore <t_staticStore>::t_Position
  gct_PackStore <t_staticStore>::Alloc
  (
  t_Size o_size
  )
    {
    if (o_size == 0)
      return 0;

    if (o_size > so_Data. u_OwnPageSize)
      {
      AllocPage (o_size + sizeof (char *));
      return so_Data. pc_CurrPage + sizeof (char *);
      }

    o_size = (o_size + so_Data. u_Align) & so_Data. u_AlignMask;

    if (o_size > so_Data. u_CurrSize)
      AllocDataPage ();

    void * pv_data = so_Data. pc_CurrData;
    so_Data. pc_CurrData += o_size;
    so_Data. u_CurrSize -= o_size;
    return pv_data;
    }

//---------------------------------------------------------------------------

template <class t_staticStore>
  TL_TYPENAME gct_PackStore <t_staticStore>::t_Position
  gct_PackStore <t_staticStore>::Realloc
  (
  t_Position o_pos,
  t_Size o_size
  )
    {
    if (o_pos != 0)
      {
      TL_ASSERT (o_size == 0);
      return 0;
      }
    else
      return Alloc (o_size);
    }

//---------------------------------------------------------------------------

template <class t_staticStore>
  inline void gct_PackStore <t_staticStore>::Free
  (
  t_Position /*o_pos*/
  )
    {
    }

//---------------------------------------------------------------------------

template <class t_staticStore>
  inline void * gct_PackStore <t_staticStore>::AddrOf
  (
  t_Position o_pos
  )
    {
    return o_pos;
    }

//---------------------------------------------------------------------------

template <class t_staticStore>
  inline TL_TYPENAME gct_PackStore <t_staticStore>::t_Position
  gct_PackStore <t_staticStore>::PosOf
  (
  void * pv_adr
  )
    {
    return pv_adr;
    }

//---------------------------------------------------------------------------

template <class t_staticStore>
  inline TL_TYPENAME gct_PackStore <t_staticStore>::t_Size
  gct_PackStore <t_staticStore>::SizeOf
  (
  t_Position /*o_pos*/
  )
    {
    TL_ASSERT (false);
    return 0;
    }

//---------------------------------------------------------------------------

template <class t_staticStore>
  inline TL_TYPENAME gct_PackStore <t_staticStore>::t_Size
  gct_PackStore <t_staticStore>::RoundedSizeOf
  (
  t_Position /*o_pos*/
  )
    {
    TL_ASSERT (false);
    return 0;
    }

//---------------------------------------------------------------------------

template <class t_staticStore>
  inline bool gct_PackStore <t_staticStore>::CanFreeAll ()
    {
    return true;
    }

//---------------------------------------------------------------------------

template <class t_staticStore>
  void gct_PackStore <t_staticStore>::FreeAll
  (
  bool b_keepPage
  )
    {
    char * pc_page = so_Data. pc_CurrPage;

    if (b_keepPage && (so_Data. pc_FirstPage != 0))
      {
      so_Data. pc_CurrPage = so_Data. pc_FirstPage;
      so_Data. pc_CurrData = so_Data. pc_CurrPage + sizeof (char *);
      so_Data. u_CurrSize = so_Data. u_PageSize - sizeof (char *);

      while (pc_page != 0)
        {
        char * pc_next = * ((char * *) pc_page);

        if (pc_page == so_Data. pc_FirstPage)
          * ((char * *) pc_page) = 0;
        else
          t_StaticStore::Free (pc_page);

        pc_page = pc_next;
        }
      }
    else
      {
      so_Data. pc_FirstPage = 0;
      so_Data. pc_CurrPage = 0;
      so_Data. pc_CurrData = 0;
      so_Data. u_CurrSize = 0;

      while (pc_page != 0)
        {
        char * pc_next = * ((char * *) pc_page);
        t_StaticStore::Free (pc_page);
        pc_page = pc_next;
        }
      }
    }

//---------------------------------------------------------------------------

template <class t_staticStore>
  bool gct_PackStore <t_staticStore>::Init
  (
  unsigned u_align,
  unsigned u_pageExp,
  t_Size o_ownPageSize
  )
    {
    if (so_Data. pc_CurrPage != 0)
      return false;

    if ((u_align != 1) && (u_align != 2) && (u_align != 4) &&
        (u_align != 8) && (u_align != 16))
      return false;

    if (u_pageExp < 7u) // 128 Byte
      return false;

    so_Data. u_Align = u_align - 1;
    so_Data. u_AlignMask = ~so_Data. u_Align;
    so_Data. u_PageSize = (((t_UInt) 1) << u_pageExp) - t_StaticStore::StoreInfoSize ();

    if (o_ownPageSize == 0)
      so_Data. u_OwnPageSize = so_Data. u_PageSize >> 2;
    else
      so_Data. u_OwnPageSize = tl_Min (tl_Max (o_ownPageSize, (t_Size) 16), so_Data. u_PageSize >> 1);

    return true;
    }

#endif
