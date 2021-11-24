
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
// File tuning/string.h

#ifndef TUNING_STRING_H
#define TUNING_STRING_H

#include "tuning/sys/cstring.hpp"
#include "tuning/sys/cmemory.hpp"
#include "tuning/printf.hpp"
#include "tuning/block.h"
#include "tuning/charblock.h"
#include "tuning/array.h"

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  class TL_TM_EXPORT gct_String: public t_block
    {
  public:
    typedef t_block       t_Block;
    typedef t_staticStore t_StaticStore;
    typedef TL_TYPENAME t_block::t_Char t_Char;
    typedef TL_TYPENAME t_block::t_Size t_Size;

  private:
    using TL_TMU_TYPENAME t_block::GetCharSize;
    using TL_TMU_TYPENAME t_block::SetCharSize;
    using TL_TMU_TYPENAME t_block::IncCharSize;
    using TL_TMU_TYPENAME t_block::DecCharSize;
    using TL_TMU_TYPENAME t_block::GetRawAddr;
    using TL_TMU_TYPENAME t_block::GetCharAddr;
    using TL_TMU_TYPENAME t_block::AppendChars;
    using TL_TMU_TYPENAME t_block::InsertChars;
    using TL_TMU_TYPENAME t_block::DeleteChars;
    using TL_TMU_TYPENAME t_block::FillChars;
    using TL_TMU_TYPENAME t_block::AssignChars;
    using TL_TMU_TYPENAME t_block::ReplaceChars;
    using TL_TMU_TYPENAME t_block::AlignPageSize;

  protected:
    static const t_Char  c_Null = (t_Char) 0;
    inline void          SetLen (t_Size o_len);
                         gct_String (const gct_String & co_init,
                           t_Char c_app);
                         gct_String (const gct_String & co_init,
                           const t_Char * pc_app);
                         gct_String (const gct_String & co_init,
                           const gct_String & co_app);
                         gct_String (t_Char c_init,
                           const gct_String & co_app);
                         gct_String (const t_Char * pc_init,
                           const gct_String & co_app);

  public:
    inline               gct_String ();
    inline               gct_String (t_Char c_init);
    inline               gct_String (t_Char c_init, t_Size o_len);
    inline               gct_String (const t_Char * pc_init);
    inline               gct_String (const t_Char * pc_init, t_Size o_len);
    inline               gct_String (const gct_String & co_init);

    inline t_UInt        GetHash () const;
    inline bool          IsEmpty () const;
    inline t_Size        GetMaxLen () const;
    inline t_Size        GetLen () const;
    inline const t_Char * GetStr () const;
    inline const t_Char * operator () () const;
    inline const t_Char * GetStr (t_Size o_pos) const;
    inline const t_Char * operator () (t_Size o_pos) const;
    inline t_Char &      GetChar (t_Size o_pos) const;
    inline t_Char &      operator [] (t_Size o_pos) const;
    inline t_Char &      GetRevChar (t_Size o_pos) const;
    gct_String           SubStr (t_Size o_len) const;
    gct_String           RevSubStr (t_Size o_len) const;
    gct_String           SubStr (t_Size o_pos, t_Size o_len) const;
    gct_String           operator () (t_Size o_pos, t_Size o_len) const;

    t_Int                First (t_Char c_search, t_Size o_pos = 0) const;
    t_Int                First (const t_Char * pc_search,
                           t_Size o_pos = 0) const;
    t_Int                First (const gct_String & co_search,
                           t_Size o_pos = 0) const;

    t_Int                Last (t_Char c_search, t_Size o_pos = 0) const;
    t_Int                Last (const t_Char * pc_search,
                           t_Size o_pos = 0) const;
    t_Int                Last (const gct_String & co_search,
                           t_Size o_pos = 0) const;

    inline int           CompSubStr (t_Size o_pos, t_Char c_comp) const;
    inline int           CompSubStr (t_Size o_pos, const t_Char * pc_comp) const;
    inline int           CompSubStr (t_Size o_pos,
                           const t_Char * pc_comp, t_Size o_len) const;
    inline int           CompSubStr (t_Size o_pos,
                           const gct_String & co_comp) const;

    inline int           CompTo (t_Char c_comp) const;
    inline int           CompTo (const t_Char * pc_comp) const;
    inline int           CompTo (const t_Char * pc_comp, t_Size o_len) const;
    inline int           CompTo (const gct_String & co_comp) const;

    inline void          Clear ();
    inline void          Assign (t_Char c_asgn);
    inline void          Assign (t_Char c_asgn, t_Size o_len);
    void                 Assign (const t_Char * pc_asgn);
    inline void          Assign (const t_Char * pc_asgn, t_Size o_len);
    void                 Assign (const gct_String & co_asgn);
    inline void          Append (t_Char c_app);
    inline void          Append (t_Char c_app, t_Size o_len);
    void                 Append (const t_Char * pc_app);
    inline void          Append (const t_Char * pc_app, t_Size o_len);
    void                 Append (const gct_String & co_app);

    inline void          Insert (t_Size o_pos, t_Char c_ins);
    inline void          Insert (t_Size o_pos, t_Char c_ins, t_Size o_len);
    inline void          Insert (t_Size o_pos, const t_Char * pc_ins);
    inline void          Insert (t_Size o_pos, const t_Char * pc_ins,
                           t_Size o_len);
    inline void          Insert (t_Size o_pos, const gct_String & co_ins);
    inline void          Delete (t_Size o_pos);
    inline void          Delete (t_Size o_pos, t_Size o_len);
    inline void          DeleteRev (t_Size o_len);
    void                 Replace (t_Size o_pos, t_Size o_delLen, t_Char c_ins);
    void                 Replace (t_Size o_pos, t_Size o_delLen, t_Char c_ins,
                           t_Size o_insLen);
    void                 Replace (t_Size o_pos, t_Size o_delLen,
                           const t_Char * pc_ins);
    void                 Replace (t_Size o_pos, t_Size o_delLen,
                           const t_Char * pc_ins, t_Size o_insLen);
    void                 Replace (t_Size o_pos, t_Size o_delLen,
                           const gct_String & co_ins);
    t_Size               ReplaceAll (const gct_String & co_search,
                           const gct_String & co_replace);

    int                  AssignF (const t_Char * pc_format, ...);
    int                  AppendF (const t_Char * pc_format, ...);
    int                  InsertF (t_Size o_pos, const t_Char * pc_format, ...);
    int                  ReplaceF (t_Size o_pos, t_Size o_delLen,
                           const t_Char * pc_format, ...);

    inline bool          ToUpper ();
    inline bool          ToLower ();
    inline bool          ToUpper2 ();
    inline bool          ToLower2 ();

    inline bool          operator == (const t_Char * pc_comp) const;
    inline bool          operator == (const gct_String & co_comp) const;
    inline bool          operator != (const t_Char * pc_comp) const;
    inline bool          operator != (const gct_String & co_comp) const;
    inline bool          operator <  (const t_Char * pc_comp) const;
    inline bool          operator <  (const gct_String & co_comp) const;
    inline bool          operator <= (const t_Char * pc_comp) const;
    inline bool          operator <= (const gct_String & co_comp) const;
    inline bool          operator >  (const t_Char * pc_comp) const;
    inline bool          operator >  (const gct_String & co_comp) const;
    inline bool          operator >= (const t_Char * pc_comp) const;
    inline bool          operator >= (const gct_String & co_comp) const;

    inline gct_String &  operator = (t_Char c_asgn);
    inline gct_String &  operator = (const t_Char * pc_asgn);
    inline gct_String &  operator = (const gct_String & co_asgn);
    inline gct_String &  operator += (t_Char c_app);
    inline gct_String &  operator += (const t_Char * pc_app);
    inline gct_String &  operator += (const gct_String & co_app);

    inline gct_String    operator + (t_Char c_app) const;
    inline gct_String    operator + (const t_Char * pc_app) const;
    inline gct_String    operator + (const gct_String & co_app) const;

    friend inline gct_String TL_EXPORT
      operator + (t_Char c_init, const gct_String & co_app)
        { return gct_String <t_block, t_staticStore> (c_init, co_app); }
    friend inline gct_String TL_EXPORT
      operator + (const t_Char * pc_init, const gct_String & co_app)
        { return gct_String <t_block, t_staticStore> (pc_init, co_app); }
    template <class t_string>
      void Convert (const t_string & co_asgn)
        {
        TL_TYPENAME t_string::t_Size o_len = co_asgn. GetLen ();
        TL_ASSERT (o_len == (t_Size) o_len);
        SetLen ((t_Size) o_len ++);
        const TL_TYPENAME t_string::t_Char * pc_src = co_asgn. GetStr ();
        t_Char * pc_dst = this-> GetCharAddr ();

        while (o_len -- != 0)
          * (pc_dst ++) = (t_Char) * (pc_src ++);
        }
    template <class t_string>
      bool MbConvert (const t_string & co_asgn)
        {
        t_UInt u_count = tl_MbConvertCount ((t_Char *) 0, co_asgn. GetStr ());

        if (u_count > 0)
          {
          TL_ASSERT (u_count == (t_Size) u_count);
          this-> SetCharSize ((t_Size) u_count);
          return tl_MbConvert (this-> GetCharAddr (), co_asgn. GetStr (), u_count);
          }
        else
          return false;
        }
    template <class t_asgnChar>
      bool MbConvert (const t_asgnChar * po_asgn)
        {
        t_UInt u_count = tl_MbConvertCount ((t_Char *) 0, po_asgn);

        if (u_count > 0)
          {
          TL_ASSERT (u_count == (t_Size) u_count);
          this-> SetCharSize ((t_Size) u_count);
          return tl_MbConvert (this-> GetCharAddr (), po_asgn, u_count);
          }
        else
          return false;
        }
    };

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline void gct_String <t_block, t_staticStore>::SetLen
  (
  t_Size o_len
  )
    {
    TL_ASSERT ((o_len + 1) == (t_Size) (o_len + 1)); // Overflow
    this-> SetCharSize ((t_Size) (o_len + 1));
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  gct_String <t_block, t_staticStore>::gct_String
  (
  const gct_String & co_init,
  t_Char c_app
  ):
    t_block (co_init)
    {
    Append (c_app);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  gct_String <t_block, t_staticStore>::gct_String
  (
  const gct_String & co_init,
  const t_Char * pc_app
  ):
    t_block (co_init)
    {
    Append (pc_app);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  gct_String <t_block, t_staticStore>::gct_String
  (
  const gct_String & co_init,
  const gct_String & co_app
  ):
    t_block (co_init)
    {
    Append (co_app);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline gct_String <t_block, t_staticStore>::gct_String
  (
  t_Char c_init,
  const gct_String & co_app
  )
    {
    Assign (c_init);
    Append (co_app);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline gct_String <t_block, t_staticStore>::gct_String
  (
  const t_Char * pc_init,
  const gct_String & co_app
  )
    {
    Assign (pc_init);
    Append (co_app);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline gct_String <t_block, t_staticStore>::gct_String ()
    {
    SetLen (0);
    * this-> GetCharAddr () = c_Null;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline gct_String <t_block, t_staticStore>::gct_String
  (
  t_Char c_init
  )
    {
    Assign (c_init);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline gct_String <t_block, t_staticStore>::gct_String
  (
  t_Char c_init,
  t_Size o_len
  )
    {
    Assign (c_init, o_len);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline gct_String <t_block, t_staticStore>::gct_String
  (
  const t_Char * pc_init
  )
    {
    Assign (pc_init);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline gct_String <t_block, t_staticStore>::gct_String
  (
  const t_Char * pc_init,
  t_Size o_len
  )
    {
    Assign (pc_init, o_len);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline gct_String <t_block, t_staticStore>::gct_String
  (
  const gct_String & co_init
  ):
    t_block (co_init)
    {
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline t_UInt gct_String <t_block, t_staticStore>::GetHash () const
    {
    return tl_StringHash (this-> GetCharAddr (), GetLen ());
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline bool gct_String <t_block, t_staticStore>::IsEmpty () const
    {
    return this-> GetCharSize () == 1;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline TL_TYPENAME gct_String <t_block, t_staticStore>::t_Size
  gct_String <t_block, t_staticStore>::GetMaxLen () const
    {
    return (t_Size) (this-> GetMaxCharSize () - 1);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline TL_TYPENAME gct_String <t_block, t_staticStore>::t_Size
  gct_String <t_block, t_staticStore>::GetLen () const
    {
    return (t_Size) (this-> GetCharSize () - 1);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline const TL_TYPENAME gct_String <t_block, t_staticStore>::t_Char *
  gct_String <t_block, t_staticStore>::GetStr () const
    {
    return this-> GetCharAddr ();
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline const TL_TYPENAME gct_String <t_block, t_staticStore>::t_Char *
  gct_String <t_block, t_staticStore>::operator () () const
    {
    return this-> GetCharAddr ();
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline const TL_TYPENAME gct_String <t_block, t_staticStore>::t_Char *
  gct_String <t_block, t_staticStore>::GetStr
  (
  t_Size o_pos
  ) const
    {
    return this-> GetCharAddr (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline const TL_TYPENAME gct_String <t_block, t_staticStore>::t_Char *
  gct_String <t_block, t_staticStore>::operator ()
  (
  t_Size o_pos
  ) const
    {
    return this-> GetCharAddr (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline TL_TYPENAME gct_String <t_block, t_staticStore>::t_Char &
  gct_String <t_block, t_staticStore>::GetChar
  (
  t_Size o_pos
  ) const
    {
    TL_ASSERT (o_pos < GetLen ());

    return this-> GetCharAddr () [o_pos];
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline TL_TYPENAME gct_String <t_block, t_staticStore>::t_Char &
  gct_String <t_block, t_staticStore>::operator []
  (
  t_Size o_pos
  ) const
    {
    TL_ASSERT (o_pos < GetLen ());

    return this-> GetCharAddr () [o_pos];
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline TL_TYPENAME gct_String <t_block, t_staticStore>::t_Char &
  gct_String <t_block, t_staticStore>::GetRevChar
  (
  t_Size o_pos
  ) const
    {
    TL_ASSERT (o_pos < GetLen ());

    return this-> GetCharAddr () [this-> GetCharSize () - 2 - o_pos];
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  gct_String <t_block, t_staticStore>
  gct_String <t_block, t_staticStore>::SubStr
  (
  t_Size o_len
  ) const
    {
    TL_ASSERT (o_len < this-> GetCharSize ());

    return gct_String <t_block, t_staticStore> (this-> GetCharAddr (), o_len);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  gct_String <t_block, t_staticStore>
  gct_String <t_block, t_staticStore>::RevSubStr
  (
  t_Size o_len
  ) const
    {
    TL_ASSERT (o_len < this-> GetCharSize ());

    return gct_String <t_block, t_staticStore> (this-> GetCharAddr (GetLen () - o_len), o_len);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  gct_String <t_block, t_staticStore>
  gct_String <t_block, t_staticStore>::SubStr
  (
  t_Size o_pos,
  t_Size o_len
  ) const
    {
    TL_DEBUG_CODE
    (
    t_Size o_sum = (t_Size) (o_pos + o_len);
    TL_ASSERT (o_sum >= o_len); // Overflow
    TL_ASSERT (o_sum < this-> GetCharSize ());
    )

    return gct_String <t_block, t_staticStore> (this-> GetCharAddr (o_pos), o_len);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  gct_String <t_block, t_staticStore>
  gct_String <t_block, t_staticStore>::operator ()
  (
  t_Size o_pos,
  t_Size o_len
  ) const
    {
    TL_DEBUG_CODE
    (
    t_Size o_sum = (t_Size) (o_pos + o_len);
    TL_ASSERT (o_sum >= o_len); // Overflow
    TL_ASSERT (o_sum < this-> GetCharSize ());
    )

    return gct_String <t_block, t_staticStore> (this-> GetCharAddr (o_pos), o_len);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  t_Int gct_String <t_block, t_staticStore>::First
  (
  t_Char c_search,
  t_Size o_pos
  ) const
    {
    if (IsEmpty ())
      return -1;

    const t_Char * pc =
      tl_FirstChar (this-> GetCharAddr (o_pos), GetLen () - o_pos, c_search);

    if (pc != 0)
      return pc - this-> GetCharAddr ();
    else
      return -1;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  t_Int gct_String <t_block, t_staticStore>::First
  (
  const t_Char * pc_search,
  t_Size o_pos
  ) const
    {
    if (IsEmpty ())
      return -1;

    const t_Char * pc = tl_FirstMemory (this-> GetCharAddr (o_pos),
      GetLen () - o_pos, pc_search, tl_StringLength (pc_search));

    if (pc != 0)
      return pc - this-> GetCharAddr ();
    else
      return -1;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  t_Int gct_String <t_block, t_staticStore>::First
  (
  const gct_String & co_search,
  t_Size o_pos
  ) const
    {
    if (IsEmpty ())
      return -1;

    const t_Char * pc = tl_FirstMemory (this-> GetCharAddr (o_pos),
      GetLen () - o_pos, co_search. GetCharAddr (), co_search. GetLen ());

    if (pc != 0)
      return pc - this-> GetCharAddr ();
    else
      return -1;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  t_Int gct_String <t_block, t_staticStore>::Last
  (
  t_Char c_search,
  t_Size o_pos
  ) const
    {
    if (IsEmpty ())
      return -1;

    const t_Char * pc =
      tl_LastChar (this-> GetCharAddr (o_pos), GetLen () - o_pos, c_search);

    if (pc != 0)
      return pc - this-> GetCharAddr ();
    else
      return -1;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  t_Int gct_String <t_block, t_staticStore>::Last
  (
  const t_Char * pc_search,
  t_Size o_pos
  ) const
    {
    if (IsEmpty ())
      return -1;

    const t_Char * pc = tl_LastMemory (this-> GetCharAddr (o_pos),
      GetLen () - o_pos, pc_search, tl_StringLength (pc_search));

    if (pc != 0)
      return pc - this-> GetCharAddr ();
    else
      return -1;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  t_Int gct_String <t_block, t_staticStore>::Last
  (
  const gct_String & co_search,
  t_Size o_pos
  ) const
    {
    if (IsEmpty ())
      return -1;

    const t_Char * pc = tl_LastMemory (this-> GetCharAddr (o_pos),
      GetLen () - o_pos, co_search. GetCharAddr (), co_search. GetLen ());

    if (pc != 0)
      return pc - this-> GetCharAddr ();
    else
      return -1;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline int gct_String <t_block, t_staticStore>::CompSubStr
  (
  t_Size o_pos,
  t_Char c_comp
  ) const
    {
    return tl_CompareChar (GetChar (o_pos), c_comp);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline int gct_String <t_block, t_staticStore>::CompSubStr
  (
  t_Size o_pos,
  const t_Char * pc_comp
  ) const
    {
    return tl_CompareMemory (this-> GetCharAddr (o_pos), pc_comp,
      tl_Min ((t_UInt) (this-> GetCharSize () - o_pos),
        tl_StringLength (pc_comp)));
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline int gct_String <t_block, t_staticStore>::CompSubStr
  (
  t_Size o_pos,
  const t_Char * pc_comp,
  t_Size o_len
  ) const
    {
    return tl_CompareMemory (this-> GetCharAddr (o_pos), pc_comp,
      tl_Min ((t_UInt) (this-> GetCharSize () - o_pos), (t_UInt) o_len));
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline int gct_String <t_block, t_staticStore>::CompSubStr
  (
  t_Size o_pos,
  const gct_String & co_comp
  ) const
    {
    return tl_CompareMemory (this-> GetCharAddr (o_pos), co_comp. GetCharAddr (),
      tl_Min ((t_UInt) (this-> GetCharSize () - o_pos),
        (t_UInt) (co_comp. GetCharSize () - 1)));
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline int gct_String <t_block, t_staticStore>::CompTo
  (
  t_Char c_comp
  ) const
    {
    int i = tl_CompareChar (* (this-> GetCharAddr ()), c_comp);

    if (i != 0)
      return i;
    else
      return (int) (this-> GetCharSize () - 2);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline int gct_String <t_block, t_staticStore>::CompTo
  (
  const t_Char * pc_comp
  ) const
    {
    return tl_CompareMemory (this-> GetCharAddr (), pc_comp,
      tl_Min ((t_UInt) this-> GetCharSize (), tl_StringLength (pc_comp) + 1));
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline int gct_String <t_block, t_staticStore>::CompTo
  (
  const t_Char * pc_comp,
  t_Size o_len
  ) const
    {
    int i = tl_CompareMemory (this-> GetCharAddr (), pc_comp,
      tl_Min ((t_UInt) this-> GetCharSize (), (t_UInt) o_len));

    if (i != 0)
      return i;
    else
      return (int) (GetLen () - o_len);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline int gct_String <t_block, t_staticStore>::CompTo
  (
  const gct_String & co_comp
  ) const
    {
    return tl_CompareMemory (this-> GetCharAddr (), co_comp. GetCharAddr (),
      tl_Min ((t_UInt) this-> GetCharSize (), (t_UInt) co_comp. GetCharSize ()));
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline void gct_String <t_block, t_staticStore>::Clear ()
    {
    Delete (0);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline void gct_String <t_block, t_staticStore>::Assign
  (
  t_Char c_asgn
  )
    {
    TL_ASSERT (c_asgn != c_Null);

    SetLen (1);
    t_Char * pc = this-> GetCharAddr ();
    pc [0] = c_asgn;
    pc [1] = c_Null;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline void gct_String <t_block, t_staticStore>::Assign
  (
  t_Char c_asgn,
  t_Size o_len
  )
    {
    TL_ASSERT (c_asgn != c_Null);

    SetLen (o_len);
    this-> FillChars (0, o_len, c_asgn);
    * this-> GetCharAddr (o_len) = c_Null;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  void gct_String <t_block, t_staticStore>::Assign
  (
  const t_Char * pc_asgn
  )
    {
    if (pc_asgn != this-> GetCharAddr ())
      {
      t_UInt u_size = tl_StringLength (pc_asgn) + 1;

      TL_ASSERT (u_size == (t_Size) u_size);

      this-> SetCharSize ((t_Size) u_size);
      tl_CopyMemory (this-> GetCharAddr (), pc_asgn, u_size);
      }
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline void gct_String <t_block, t_staticStore>::Assign
  (
  const t_Char * pc_asgn,
  t_Size o_len
  )
    {
    SetLen (o_len);
    tl_CopyMemory (this-> GetCharAddr (), pc_asgn, o_len);
    * this-> GetCharAddr (o_len) = c_Null;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  void gct_String <t_block, t_staticStore>::Assign
  (
  const gct_String & co_asgn
  )
    {
    if (& co_asgn != this)
      {
      this-> SetCharSize (co_asgn. GetCharSize ());
      tl_CopyMemory (this-> GetCharAddr (), co_asgn. GetCharAddr (), this-> GetCharSize ());
      }
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline void gct_String <t_block, t_staticStore>::Append
  (
  t_Char c_app
  )
    {
    TL_ASSERT (c_app != c_Null);

    this-> IncCharSize (1);
    t_Char * pc = this-> GetCharAddr (GetLen () - 1);
    pc [0] = c_app;
    pc [1] = c_Null;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline void gct_String <t_block, t_staticStore>::Append
  (
  t_Char c_app,
  t_Size o_len
  )
    {
    TL_ASSERT (c_app != c_Null);

    Insert (GetLen (), c_app, o_len);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  void gct_String <t_block, t_staticStore>::Append
  (
  const t_Char * pc_app
  )
    {
    if (pc_app != this-> GetCharAddr ())
      Insert (GetLen (), pc_app);
    else
      {
      t_Size u_len = GetLen ();
      this-> IncCharSize (u_len);
      tl_CopyMemory (this-> GetCharAddr (u_len),
        this-> GetCharAddr (), u_len);
      * this-> GetCharAddr (GetLen ()) = c_Null;
      }
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline void gct_String <t_block, t_staticStore>::Append
  (
  const t_Char * pc_app,
  t_Size o_len
  )
    {
    Insert (GetLen (), pc_app, o_len);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  void gct_String <t_block, t_staticStore>::Append
  (
  const gct_String & co_app
  )
    {
    if (& co_app != this)
      Insert (GetLen (), co_app);
    else
      {
      t_Size u_len = GetLen ();
      this-> IncCharSize (u_len);
      tl_CopyMemory (this-> GetCharAddr (u_len),
        this-> GetCharAddr (), u_len);
      * this-> GetCharAddr (GetLen ()) = c_Null;
      }
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline void gct_String <t_block, t_staticStore>::Insert
  (
  t_Size o_pos,
  t_Char c_ins
  )
    {
    TL_ASSERT (o_pos < this-> GetCharSize ());
    TL_ASSERT (c_ins != c_Null);

    * this-> InsertChars (o_pos, 1) = c_ins;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline void gct_String <t_block, t_staticStore>::Insert
  (
  t_Size o_pos,
  t_Char c_ins,
  t_Size o_len
  )
    {
    TL_ASSERT (o_pos < this-> GetCharSize ());
    TL_ASSERT (c_ins != c_Null);

    tl_FillMemory (this-> InsertChars (o_pos, o_len), o_len, c_ins);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline void gct_String <t_block, t_staticStore>::Insert
  (
  t_Size o_pos,
  const t_Char * pc_ins
  )
    {
    TL_ASSERT (o_pos < this-> GetCharSize ());

    t_UInt u_insLen = tl_StringLength (pc_ins);

    TL_ASSERT (u_insLen == (t_Size) u_insLen);

    tl_CopyMemory (this-> InsertChars (o_pos, (t_Size) u_insLen), pc_ins, u_insLen);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline void gct_String <t_block, t_staticStore>::Insert
  (
  t_Size o_pos,
  const t_Char * pc_ins,
  t_Size o_len
  )
    {
    TL_ASSERT (o_pos < this-> GetCharSize ());

    tl_CopyMemory (this-> InsertChars (o_pos, o_len), pc_ins, o_len);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline void gct_String <t_block, t_staticStore>::Insert
  (
  t_Size o_pos,
  const gct_String & co_ins
  )
    {
    TL_ASSERT (o_pos < this-> GetCharSize ());

    tl_CopyMemory (this-> InsertChars (o_pos, co_ins. GetLen ()),
      co_ins. GetCharAddr (), co_ins. GetLen ());
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline void gct_String <t_block, t_staticStore>::Delete
  (
  t_Size o_pos
  )
    {
    TL_ASSERT (o_pos < this-> GetCharSize ());

    SetLen (o_pos);
    * this-> GetCharAddr (o_pos) = c_Null;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline void gct_String <t_block, t_staticStore>::Delete
  (
  t_Size o_pos,
  t_Size o_len
  )
    {
    TL_ASSERT (o_pos < this-> GetCharSize ());

    this-> DeleteChars (o_pos, o_len);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline void gct_String <t_block, t_staticStore>::DeleteRev
  (
  t_Size o_len
  )
    {
    TL_ASSERT (o_len < this-> GetCharSize ());

    this-> DecCharSize (o_len);
    * this-> GetCharAddr (GetLen ()) = c_Null;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  void gct_String <t_block, t_staticStore>::Replace
  (
  t_Size o_pos,
  t_Size o_delLen,
  t_Char c_ins
  )
    {
    TL_ASSERT (o_pos < this-> GetCharSize ());
    TL_ASSERT (c_ins != c_Null);

    if (o_delLen > 1)
      * this-> DeleteChars (o_pos, (t_Size) (o_delLen - 1)) = c_ins;
    else
      * this-> InsertChars (o_pos, (t_Size) (1 - o_delLen)) = c_ins;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  void gct_String <t_block, t_staticStore>::Replace
  (
  t_Size o_pos,
  t_Size o_delLen,
  t_Char c_ins,
  t_Size o_insLen
  )
    {
    TL_ASSERT (o_pos < this-> GetCharSize ());
    TL_ASSERT (c_ins != c_Null);

    t_Char * pc = o_delLen > o_insLen ?
      this-> DeleteChars (o_pos, (t_Size) (o_delLen - o_insLen)) :
      this-> InsertChars (o_pos, (t_Size) (o_insLen - o_delLen));

    tl_FillMemory (pc, o_insLen, c_ins);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  void gct_String <t_block, t_staticStore>::Replace
  (
  t_Size o_pos,
  t_Size o_delLen,
  const t_Char * pc_ins
  )
    {
    TL_ASSERT (o_pos < this-> GetCharSize ());

    t_UInt u_insLen = tl_StringLength (pc_ins);

    TL_ASSERT (u_insLen == (t_Size) u_insLen);

    t_Char * pc = o_delLen > (t_Size) u_insLen ?
      this-> DeleteChars (o_pos, (t_Size) (o_delLen - u_insLen)) :
      this-> InsertChars (o_pos, (t_Size) (u_insLen - o_delLen));

    tl_CopyMemory (pc, pc_ins, u_insLen);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  void gct_String <t_block, t_staticStore>::Replace
  (
  t_Size o_pos,
  t_Size o_delLen,
  const t_Char * pc_ins,
  t_Size o_insLen
  )
    {
    TL_ASSERT (o_pos < this-> GetCharSize ());

    t_Char * pc = o_delLen > o_insLen ?
      this-> DeleteChars (o_pos, (t_Size) (o_delLen - o_insLen)) :
      this-> InsertChars (o_pos, (t_Size) (o_insLen - o_delLen));

    tl_CopyMemory (pc, pc_ins, o_insLen);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  void gct_String <t_block, t_staticStore>::Replace
  (
  t_Size o_pos,
  t_Size o_delLen,
  const gct_String & co_ins
  )
    {
    TL_ASSERT (o_pos < this-> GetCharSize ());

    t_Char * pc = o_delLen > co_ins. GetLen () ?
      this-> DeleteChars (o_pos, (t_Size) (o_delLen - co_ins. GetLen ())) :
      this-> InsertChars (o_pos, (t_Size) (co_ins. GetLen () - o_delLen));

    tl_CopyMemory (pc, co_ins. GetCharAddr (), co_ins. GetLen ());
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  TL_TYPENAME gct_String <t_block, t_staticStore>::t_Size
  gct_String <t_block, t_staticStore>::ReplaceAll
  (
  const gct_String & co_search,
  const gct_String & co_replace
  )
    {
    const t_Char * pc_search  = co_search.  GetCharAddr ();
    const t_Char * pc_replace = co_replace. GetCharAddr ();
    t_Size u_searchLen  = co_search.  GetLen ();
    t_Size u_replaceLen = co_replace. GetLen ();
    const t_Char * pc_last = this-> GetCharAddr (GetLen ());

    TL_ASSERT (* pc_last == c_Null);

    if (u_searchLen == 0)
      return 0;

    t_Char * pc_found = (t_Char *)
      tl_FirstMemory (this-> GetCharAddr (), GetLen (), pc_search, u_searchLen);

    if (pc_found == 0)
      return 0;

    if (u_searchLen == u_replaceLen)
      {
      t_Size o_found = 0;

      while (pc_found != 0)
        {
        o_found ++;
        tl_CopyMemory (pc_found, pc_replace, u_replaceLen);
        pc_found += u_searchLen;
        pc_found = (t_Char *)
          tl_FirstMemory (pc_found, pc_last - pc_found, pc_search, u_searchLen);
        }

      return o_found;
      }
    else
      {
      typedef gct_FixItemArray <const t_Char *, gct_EmptyBaseBlock <t_staticStore> > t_array;
      t_array co_found;
      TL_TYPENAME t_array::t_Position o_pos;

      while (pc_found != 0)
        {
        co_found. AddObjAfter (co_found. Last (), & pc_found);
        pc_found += u_searchLen;
        pc_found = (t_Char *)
          tl_FirstMemory (pc_found, pc_last - pc_found, pc_search, u_searchLen);
        }

      t_block co_result;
      t_UInt o_newSize = this-> GetCharSize () + co_found. GetLen () *
        ((t_Int) u_replaceLen - u_searchLen);

      TL_ASSERT (o_newSize == (t_Size) o_newSize); // Overflow

      co_result. SetCharSize ((t_Size) o_newSize);
      t_Char * pc_dst = co_result. GetCharAddr ();
      const t_Char * pc_src = this-> GetCharAddr ();

      for (o_pos = co_found. First ();
           o_pos != 0;
           o_pos = co_found. Next (o_pos))
        {
        t_Size i_textLen = (t_Size) ((* co_found. GetObj (o_pos)) - pc_src);

        if (i_textLen != 0)
          {
          tl_CopyMemory (pc_dst, pc_src, i_textLen);
          pc_src += i_textLen;
          pc_dst += i_textLen;
          }

        tl_CopyMemory (pc_dst, pc_replace, u_replaceLen);
        pc_src += u_searchLen;
        pc_dst += u_replaceLen;
        }

      t_Size i_rest = (t_Size) ((pc_last - pc_src) + 1);

      TL_ASSERT (pc_src + i_rest == pc_last + 1);
      TL_ASSERT (pc_dst + i_rest == co_result. GetRawAddr (co_result. GetCharSize ()));

      tl_CopyMemory (pc_dst, pc_src, i_rest);
      t_block::Swap (co_result);
      return (t_Size) co_found. GetLen ();
      }
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  int gct_String <t_block, t_staticStore>::AssignF
  (
  const t_Char * pc_format,
  ...
  )
    {
    va_list o_argList;
    va_start (o_argList, pc_format);
    t_Char * pc_buffer = 0;
    int i_len = tl_VSprintf (& pc_buffer, pc_format, o_argList);
    va_end (o_argList);

    if (i_len < 0)
      return -1;
    else
      {
      TL_ASSERT ((unsigned) i_len == (t_Size) i_len);

      Assign (pc_buffer, (t_Size) i_len);
      free (pc_buffer);
      return i_len;
      }
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  int gct_String <t_block, t_staticStore>::AppendF
  (
  const t_Char * pc_format,
  ...
  )
    {
    va_list o_argList;
    va_start (o_argList, pc_format);
    t_Char * pc_buffer = 0;
    int i_len = tl_VSprintf (& pc_buffer, pc_format, o_argList);
    va_end (o_argList);

    if (i_len < 0)
      return -1;
    else
      {
      TL_ASSERT ((unsigned) i_len == (t_Size) i_len);

      Append (pc_buffer, (t_Size) i_len);
      free (pc_buffer);
      return i_len;
      }
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  int gct_String <t_block, t_staticStore>::InsertF
  (
  t_Size o_pos,
  const t_Char * pc_format,
  ...
  )
    {
    va_list o_argList;
    va_start (o_argList, pc_format);
    t_Char * pc_buffer = 0;
    int i_len = tl_VSprintf (& pc_buffer, pc_format, o_argList);
    va_end (o_argList);

    if (i_len < 0)
      return -1;
    else
      {
      TL_ASSERT ((unsigned) i_len == (t_Size) i_len);

      Insert (o_pos, pc_buffer, (t_Size) i_len);
      free (pc_buffer);
      return i_len;
      }
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  int gct_String <t_block, t_staticStore>::ReplaceF
  (
  t_Size o_pos,
  t_Size o_delLen,
  const t_Char * pc_format,
  ...
  )
    {
    va_list o_argList;
    va_start (o_argList, pc_format);
    t_Char * pc_buffer = 0;
    int i_len = tl_VSprintf (& pc_buffer, pc_format, o_argList);
    va_end (o_argList);

    if (i_len < 0)
      return -1;
    else
      {
      TL_ASSERT ((unsigned) i_len == (t_Size) i_len);

      Replace (o_pos, o_delLen, pc_buffer, (t_Size) i_len);
      free (pc_buffer);
      return i_len;
      }
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline bool gct_String <t_block, t_staticStore>::ToUpper ()
    {
    return tl_ToUpper (this-> GetCharAddr ());
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline bool gct_String <t_block, t_staticStore>::ToLower ()
    {
    return tl_ToLower (this-> GetCharAddr ());
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline bool gct_String <t_block, t_staticStore>::ToUpper2 ()
    {
    return tl_ToUpper2 (this-> GetCharAddr ());
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline bool gct_String <t_block, t_staticStore>::ToLower2 ()
    {
    return tl_ToLower2 (this-> GetCharAddr ());
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline bool gct_String <t_block, t_staticStore>::operator ==
  (
  const t_Char * pc_comp
  ) const
    {
    return CompTo (pc_comp) == 0;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline bool gct_String <t_block, t_staticStore>::operator ==
  (
  const gct_String & co_comp
  ) const
    {
    return (this-> GetCharSize () == co_comp. GetCharSize ()) &&
      (tl_CompareMemory (this-> GetCharAddr (), co_comp. GetCharAddr (),
        this-> GetCharSize ()) == 0);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline bool gct_String <t_block, t_staticStore>::operator !=
  (
  const t_Char * pc_comp
  ) const
    {
    return CompTo (pc_comp) != 0;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline bool gct_String <t_block, t_staticStore>::operator !=
  (
  const gct_String & co_comp
  ) const
    {
    return (this-> GetCharSize () != co_comp. GetCharSize ()) ||
      (tl_CompareMemory (this-> GetCharAddr (), co_comp. GetCharAddr (),
        this-> GetCharSize ()) != 0);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline bool gct_String <t_block, t_staticStore>::operator <
  (
  const t_Char * pc_comp
  ) const
    {
    return CompTo (pc_comp) < 0;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline bool gct_String <t_block, t_staticStore>::operator <
  (
  const gct_String & co_comp
  ) const
    {
    return CompTo (co_comp) < 0;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline bool gct_String <t_block, t_staticStore>::operator <=
  (
  const t_Char * pc_comp
  ) const
    {
    return CompTo (pc_comp) <= 0;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline bool gct_String <t_block, t_staticStore>::operator <=
  (
  const gct_String & co_comp
  ) const
    {
    return CompTo (co_comp) <= 0;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline bool gct_String <t_block, t_staticStore>::operator >
  (
  const t_Char * pc_comp
  ) const
    {
    return CompTo (pc_comp) > 0;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline bool gct_String <t_block, t_staticStore>::operator >
  (
  const gct_String & co_comp
  ) const
    {
    return CompTo (co_comp) > 0;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline bool gct_String <t_block, t_staticStore>::operator >=
  (
  const t_Char * pc_comp
  ) const
    {
    return CompTo (pc_comp) >= 0;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline bool gct_String <t_block, t_staticStore>::operator >=
  (
  const gct_String & co_comp
  ) const
    {
    return CompTo (co_comp) >= 0;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline gct_String <t_block, t_staticStore> &
  gct_String <t_block, t_staticStore>::operator =
  (
  t_Char c_asgn
  )
    {
    Assign (c_asgn);
    return * this;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline gct_String <t_block, t_staticStore> &
  gct_String <t_block, t_staticStore>::operator =
  (
  const t_Char * pc_asgn
  )
    {
    Assign (pc_asgn);
    return * this;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline gct_String <t_block, t_staticStore> &
  gct_String <t_block, t_staticStore>::operator =
  (
  const gct_String & co_asgn
  )
    {
    return (gct_String &) t_block::operator = (co_asgn);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline gct_String <t_block, t_staticStore> &
  gct_String <t_block, t_staticStore>::operator +=
  (
  t_Char c_app
  )
    {
    Append (c_app);
    return * this;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline gct_String <t_block, t_staticStore> &
  gct_String <t_block, t_staticStore>::operator +=
  (
  const t_Char * pc_app
  )
    {
    Append (pc_app);
    return * this;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline gct_String <t_block, t_staticStore> &
  gct_String <t_block, t_staticStore>::operator +=
  (
  const gct_String & co_app
  )
    {
    Append (co_app);
    return * this;
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline gct_String <t_block, t_staticStore>
  gct_String <t_block, t_staticStore>::operator +
  (
  t_Char c_app
  ) const
    {
    return gct_String <t_block, t_staticStore> (* this, c_app);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline gct_String <t_block, t_staticStore>
  gct_String <t_block, t_staticStore>::operator +
  (
  const t_Char * pc_app
  ) const
    {
    return gct_String <t_block, t_staticStore> (* this, pc_app);
    }

//---------------------------------------------------------------------------

template <class t_block, class t_staticStore>
  inline gct_String <t_block, t_staticStore>
  gct_String <t_block, t_staticStore>::operator +
  (
  const gct_String & co_app
  ) const
    {
    return gct_String <t_block, t_staticStore> (* this, co_app);
    }

//---------------------------------------------------------------------------

#define STRING_BASE(t_Block, StoreSpec) gct_String <gct_CharBlock <gct_NullDataBlock \
  <t_Block <StoreSpec ## Store>, char>, char>, StoreSpec ## Store>

#define WSTRING_BASE(t_Block, StoreSpec) gct_String <gct_CharBlock <gct_NullDataBlock \
  <t_Block <StoreSpec ## Store>, wchar_t>, wchar_t>, StoreSpec ## Store>

#define STRING_DCL(t_Block, StoreSpec)  typedef STRING_BASE (t_Block, StoreSpec) StoreSpec ## String;

#define WSTRING_DCL(t_Block, StoreSpec) typedef WSTRING_BASE(t_Block, StoreSpec) StoreSpec ## WString;

#define OBJ_STRING_DCL(StoreSpec)                                            \
  class StoreSpec ## ectString: public StoreSpec ## String                   \
    {                                                                        \
  public:                                                                    \
    inline StoreSpec ## ectString ();                                        \
    inline StoreSpec ## ectString (t_Char c_init);                           \
    inline StoreSpec ## ectString (t_Char c_init, t_Size o_len);             \
    inline StoreSpec ## ectString (const t_Char * pc_init);                  \
    inline StoreSpec ## ectString (const t_Char * pc_init, t_Size o_len);    \
    inline StoreSpec ## ectString (const StoreSpec ## String & co_init);     \
    inline StoreSpec ## ectString (const StoreSpec ## ectString & co_init);  \
    TL_CLASSID (StoreSpec ## ectString)                                      \
    virtual bool operator < (const ct_Object & co_comp) const;               \
    virtual t_UInt GetHash () const;                                         \
    inline StoreSpec ## ectString & operator = (t_Char c_asgn);              \
    inline StoreSpec ## ectString & operator = (const t_Char * pc_asgn);     \
    inline StoreSpec ## ectString & operator =                               \
      (const StoreSpec ## ectString & co_asgn);                              \
    };                                                                       \
  inline StoreSpec ## ectString::StoreSpec ## ectString () { }               \
  inline StoreSpec ## ectString::StoreSpec ## ectString                      \
    (t_Char c_init):                                                         \
    StoreSpec ## String (c_init) { }                                         \
  inline StoreSpec ## ectString::StoreSpec ## ectString                      \
    (t_Char c_init, t_Size o_len):                                           \
    StoreSpec ## String (c_init, o_len) { }                                  \
  inline StoreSpec ## ectString::StoreSpec ## ectString                      \
    (const t_Char * pc_init):                                                \
    StoreSpec ## String (pc_init) { }                                        \
  inline StoreSpec ## ectString::StoreSpec ## ectString                      \
    (const t_Char * pc_init, t_Size o_len):                                  \
    StoreSpec ## String (pc_init, o_len) { }                                 \
  inline StoreSpec ## ectString::StoreSpec ## ectString                      \
    (const StoreSpec ## String & co_init):                                   \
    StoreSpec ## String (co_init) { }                                        \
  inline StoreSpec ## ectString::StoreSpec ## ectString                      \
    (const StoreSpec ## ectString & co_init):                                \
    StoreSpec ## String (co_init) { }                                        \
  inline TL_ISKINDOF (StoreSpec ## ectString, ct_Object)                     \
  inline bool StoreSpec ## ectString::                                       \
    operator < (const ct_Object & co_comp) const                             \
    {                                                                        \
    if (co_comp. IsKindOf (ClassId ()))                                      \
      return this-> StoreSpec ## String::operator <                          \
        ((const StoreSpec ## ectString &) co_comp);                          \
    else                                                                     \
      return ct_Object::operator < (co_comp);                                \
    }                                                                        \
  inline t_UInt StoreSpec ## ectString::GetHash () const                     \
    { return StoreSpec ## String::GetHash (); }                              \
  inline StoreSpec ## ectString & StoreSpec ## ectString::                   \
    operator = (t_Char c_asgn)                                               \
    {                                                                        \
    StoreSpec ## String::operator = (c_asgn);                                \
    return * this;                                                           \
    }                                                                        \
  inline StoreSpec ## ectString & StoreSpec ## ectString::                   \
    operator = (const t_Char * pc_asgn)                                      \
    {                                                                        \
    StoreSpec ## String::operator = (pc_asgn);                               \
    return * this;                                                           \
    }                                                                        \
  inline StoreSpec ## ectString & StoreSpec ## ectString::                   \
    operator = (const StoreSpec ## ectString & co_asgn)                      \
    {                                                                        \
    StoreSpec ## String::operator = (co_asgn);                               \
    return * this;                                                           \
    }

#endif
