
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
// File tuning/utfcit.h

#ifndef TUNING_UTFCIT_H
#define TUNING_UTFCIT_H

#include "tuning/sys/cutf.hpp"

//---------------------------------------------------------------------------

template <class t_char>
  class TL_TM_EXPORT gct_UtfCit
    {
  public:
    typedef t_char       t_Char;

  private:
    const t_Char *       pu_Start;
    const t_Char *       pu_CharStart;
    const t_Char *       pu_Char;
    t_UInt               u_SrcLen;
    t_UInt               u_CharPos;
    t_UInt32             u_Char;
    et_UtfError          eo_Error;

  public:
    inline               gct_UtfCit ();
    inline               gct_UtfCit (const t_Char * pu_src, t_UInt u_srcLen);

    void                 First (const t_Char * pu_src, t_UInt u_srcLen);
    bool                 Ready () const;
    void                 Next ();

    t_UInt32             GetChar () const;
    t_UInt               GetCharPos () const;
    t_UInt               GetRawPos () const;
    t_UInt               GetRawLen () const;
    et_UtfError          GetError () const;
    };

//---------------------------------------------------------------------------

template <class t_char>
  inline gct_UtfCit <t_char>::gct_UtfCit ()
    {
    pu_Start = 0;
    pu_CharStart = 0;
    pu_Char = 0;
    u_SrcLen = 0;
    u_CharPos = 0;
    u_Char = 0;
    eo_Error = ec_UtfEOS;
    }

//---------------------------------------------------------------------------

template <class t_char>
  inline gct_UtfCit <t_char>::gct_UtfCit
  (
  const t_Char * pu_src,
  t_UInt u_srcLen
  )
    {
    First (pu_src, u_srcLen);
    }

//---------------------------------------------------------------------------

template <class t_char>
  inline void gct_UtfCit <t_char>::First
  (
  const t_Char * pu_src,
  t_UInt u_srcLen
  )
    {
    pu_Start = pu_src;
    pu_Char = pu_src;
    u_SrcLen = u_srcLen;
    u_CharPos = 0;
    pu_CharStart = pu_Char;
    eo_Error = tl_UtfNext (u_Char, pu_Char, u_SrcLen);
    }

//---------------------------------------------------------------------------

template <class t_char>
  inline bool gct_UtfCit <t_char>::Ready () const
    {
    return eo_Error == ec_UtfOK;
    }

//---------------------------------------------------------------------------

template <class t_char>
  inline void gct_UtfCit <t_char>::Next ()
    {
    if (Ready ())
      {
      u_CharPos ++;
      pu_CharStart = pu_Char;
      eo_Error = tl_UtfNext (u_Char, pu_Char, u_SrcLen);
      }
    }

//---------------------------------------------------------------------------

template <class t_char>
  inline t_UInt32 gct_UtfCit <t_char>::GetChar () const
    {
    if (Ready ())
      return u_Char;
    else
      return 0;
    }

//---------------------------------------------------------------------------

template <class t_char>
  inline t_UInt gct_UtfCit <t_char>::GetCharPos () const
    {
    if (Ready ())
      return u_CharPos;
    else
      return 0;
    }

//---------------------------------------------------------------------------

template <class t_char>
  inline t_UInt gct_UtfCit <t_char>::GetRawPos () const
    {
    if (Ready ())
      return pu_CharStart - pu_Start;
    else
      return 0;
    }

//---------------------------------------------------------------------------

template <class t_char>
  inline t_UInt gct_UtfCit <t_char>::GetRawLen () const
    {
    if (Ready ())
      return pu_Char - pu_CharStart;
    else
      return 0;
    }

//---------------------------------------------------------------------------

template <class t_char>
  inline et_UtfError gct_UtfCit <t_char>::GetError () const
    {
    return eo_Error;
    }

#endif
