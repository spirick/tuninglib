
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
// File tuning/sys/cutf.cpp

#include "tuning/sys/cutf.hpp"
#include "tuning/sys/cstring.hpp"

//---------------------------------------------------------------------------

static inline bool ReadU8Error
(
const t_UInt8 * & pu_char,
t_UInt32 & u_char
)
  {
  t_UInt8 u_mbChar = * pu_char ++;

  if ((u_mbChar >> 6u) != (t_UInt8) 2u)
    return true;

  // 10xx xxxx
  u_char <<= 6u;
  u_char += (u_mbChar & (t_UInt8) 0x3F);
  return false;
  }

//---------------------------------------------------------------------------

static et_UtfError ReadU8Mb
(
const t_UInt8 * & pu_char,
t_UInt & u_currSrcLen,
t_UInt32 & u_char
)
  {
  // 1xxx xxxx
  if (u_char < 0xC0)
    // 10xx xxxx
    return ec_UtfMbMissingStart;

  // 11xx xxxx
  if (u_char < 0xE0)
    {
    // 110x xxxx
    if (u_currSrcLen == 0)
      return ec_UtfMbEnd;

    u_currSrcLen --;
    u_char &= 0x1F;

    if (ReadU8Error (pu_char, u_char))
      return ec_UtfMbExpected;

    if (u_char < 0x80)
      return ec_UtfWideRange;
    }
  else
    {
    // 111x xxxx
    if (u_char < 0xF0)
      {
      // 1110 xxxx
      if (u_currSrcLen < 2)
        return ec_UtfMbEnd;

      u_currSrcLen -= 2;
      u_char &= 0x0F;

      if (ReadU8Error (pu_char, u_char) ||
          ReadU8Error (pu_char, u_char))
        return ec_UtfMbExpected;

      if (u_char < 0x800)
        return ec_UtfWideRange;

      if ((u_char >= 0xD800) &&
          (u_char <= 0xDFFF))
        return ec_UtfSurrogate;
      }
    else
      {
      // 1111 xxxx
      if (u_char < 0xF8)
        {
        // 1111 0xxx
        if (u_currSrcLen < 3)
          return ec_UtfMbEnd;

        u_currSrcLen -= 3;
        u_char &= 0x07;

        #ifndef TL_64

        for (int i = 0; i < 3; i ++)
          if (ReadU8Error (pu_char, u_char))
            return ec_UtfMbExpected;

        #else

        if (ReadU8Error (pu_char, u_char) ||
            ReadU8Error (pu_char, u_char) ||
            ReadU8Error (pu_char, u_char))
          return ec_UtfMbExpected;

        #endif

        if ((u_char < 0x10000) ||
            (u_char >= 0x110000))
          return ec_UtfWideRange;
        }
      else
        // 1111 1xxx
        return ec_UtfMbInvalidStart;
      }
    }

  return ec_UtfOK;
  }

//---------------------------------------------------------------------------

static et_UtfError WriteU8Mb
(
t_UInt8 * & pu_dst,
t_UInt & u_currDstLen,
t_UInt32 u_char
)
  {
  // u_char >= 0x80

  if (u_char < 0x800)
    {
    if (u_currDstLen < 2)
      return ec_UtfDestTooSmall;

    u_currDstLen -= 2;
    * pu_dst ++ = '\xC0' + (t_UInt8) (u_char >> 6);
    * pu_dst ++ = '\x80' + (t_UInt8) (u_char & 0x3F);
    }
  else
    {
    if (u_char < 0x10000)
      {
      if ((u_char >= 0xD800) &&
          (u_char <= 0xDFFF))
        return ec_UtfSurrogate;
      else
        {
        if (u_currDstLen < 3)
          return ec_UtfDestTooSmall;

        u_currDstLen -= 3;
        * pu_dst ++ = '\xE0' + (t_UInt8) ( u_char >> 12);
        * pu_dst ++ = '\x80' + (t_UInt8) ((u_char >>  6) & 0x3F);
        * pu_dst ++ = '\x80' + (t_UInt8) ( u_char        & 0x3F);
        }
      }
    else
      {
      if (u_char < 0x110000)
        {
        if (u_currDstLen < 4)
          return ec_UtfDestTooSmall;

        u_currDstLen -= 4;
        * pu_dst ++ = '\xF0' + (t_UInt8) ( u_char >> 18);
        * pu_dst ++ = '\x80' + (t_UInt8) ((u_char >> 12) & 0x3F);
        * pu_dst ++ = '\x80' + (t_UInt8) ((u_char >>  6) & 0x3F);
        * pu_dst ++ = '\x80' + (t_UInt8) ( u_char        & 0x3F);
        }
      else
        return ec_UtfWideRange;
      }
    }

  return ec_UtfOK;
  }

//---------------------------------------------------------------------------

static inline et_UtfError ReadU16Surr
(
const t_UInt16 * & pu_char,
t_UInt & u_currSrcLen,
t_UInt32 & u_char
)
  {
  // ((u_char >= 0xD800) &&
  //  (u_char <= 0xDFFF))

  if (u_char >= 0xDC00)
    return ec_UtfHighSurrExpected;

  if (u_currSrcLen == 0)
    return ec_UtfSurrEnd;

  u_currSrcLen --;
  t_UInt32 u_char2 = * pu_char ++;

  if ((u_char2 < 0xDC00) ||
      (u_char2 > 0xDFFF))
    return ec_UtfLowSurrExpected;

  u_char &= 0x3FF;
  u_char <<= 10u;
  u_char += (u_char2 & 0x3FF);
  u_char += 0x10000;
  return ec_UtfOK;
  }

//---------------------------------------------------------------------------

static inline et_UtfError WriteU16Surr
(
t_UInt16 * & pu_dst,
t_UInt & u_currDstLen,
t_UInt32 u_char
)
  {
  // u_char >= 0x10000
  // u_char < 0x110000

  if (u_currDstLen < 2)
    return ec_UtfDestTooSmall;

  u_currDstLen -= 2;
  u_char -= 0x10000;
  * pu_dst ++ = (t_UInt16) (0xD800 + (u_char >> 10));
  * pu_dst ++ = (t_UInt16) (0xDC00 + (u_char & 0x3FF));
  return ec_UtfOK;
  }

//---------------------------------------------------------------------------

#define TL_COUNT_BEGIN(n)             \
  u_dstLen = 0;                       \
                                      \
  if (u_srcLen == 0)                  \
    {                                 \
    if (b_null)                       \
      return ec_UtfMissingNull;       \
    else                              \
      return ec_UtfOK;                \
    }                                 \
                                      \
  t_UInt u_currDstLen = 0;            \
  t_UInt u_currSrcLen = u_srcLen;     \
  const t_UInt ## n * pu_char = pu_src;


#define TL_LOOP_BEGIN                 \
    u_currSrcLen --;                  \
    t_UInt32 u_char = * pu_char ++;   \
    et_UtfError eo_err = ec_UtfOK;

#define TL_COUNT_LOOP_END             \
  if (u_currSrcLen == 0)              \
    {                                 \
    if ((u_char != 0) && b_null)      \
      {                               \
      pu_src = pu_char - 1;           \
      return ec_UtfMissingNull;       \
      }                               \
    else                              \
      {                               \
      u_dstLen = u_currDstLen;        \
      return ec_UtfOK;                \
      }                               \
    }

//--------------------------------------

et_UtfError TL_EXPORT tl_UtfConvertCount
(
t_UInt32 *,
t_UInt & u_dstLen,
const t_UInt8 * & pu_src,
t_UInt u_srcLen,
bool b_null
)
  {
  TL_COUNT_BEGIN (8)

  for (;;)
    {
    TL_LOOP_BEGIN

    if (u_char < 0x80)
      {
      // 0xxx xxxx
      if ((u_char == 0) && b_null && (u_currSrcLen > 0))
        eo_err = ec_UtfNullInside;
      }
    else
      {
      // 1xxx xxxx
      eo_err = ReadU8Mb (pu_char, u_currSrcLen, u_char);
      }

    if (eo_err != ec_UtfOK)
      {
      pu_src = pu_char - 1;
      return eo_err;
      }

    u_currDstLen ++;

    TL_COUNT_LOOP_END
    }
  }

//---------------------------------------------------------------------------

#define TL_CONV_BEGIN(n)              \
  if (u_srcLen == 0)                  \
    {                                 \
    if (b_null)                       \
      return ec_UtfMissingNull;       \
                                      \
    if (u_dstLen > 0)                 \
      return ec_UtfDestTooLarge;      \
    else                              \
      return ec_UtfOK;                \
    }                                 \
                                      \
  t_UInt u_currDstLen = u_dstLen;     \
  t_UInt u_currSrcLen = u_srcLen;     \
  const t_UInt ## n * pu_char = pu_src;

#define TL_CONV_LOOP_END              \
  if (u_currSrcLen == 0)              \
    {                                 \
    if ((u_char != 0) && b_null)      \
      return ec_UtfMissingNull;       \
                                      \
    if (u_currDstLen > 0)             \
      return ec_UtfDestTooLarge;      \
    else                              \
      return ec_UtfOK;                \
    }

//--------------------------------------

et_UtfError TL_EXPORT tl_UtfConvert
(
t_UInt32 * pu_dst,
t_UInt u_dstLen,
const t_UInt8 * pu_src,
t_UInt u_srcLen,
bool b_null
)
  {
  TL_CONV_BEGIN (8)

  for (;;)
    {
    TL_LOOP_BEGIN

    if (u_char < 0x80)
      {
      // 0xxx xxxx
      if ((u_char == 0) && b_null && (u_currSrcLen > 0))
        eo_err = ec_UtfNullInside;
      }
    else
      {
      // 1xxx xxxx
      eo_err = ReadU8Mb (pu_char, u_currSrcLen, u_char);
      }

    if (eo_err != ec_UtfOK)
      return eo_err;

    if (u_currDstLen == 0)
      return ec_UtfDestTooSmall;

    u_currDstLen --;
    * pu_dst ++ = u_char;

    TL_CONV_LOOP_END
    }
  }

//---------------------------------------------------------------------------

et_UtfError TL_EXPORT tl_UtfConvertCount
(
t_UInt8 *,
t_UInt & u_dstLen,
const t_UInt32 * & pu_src,
t_UInt u_srcLen,
bool b_null
)
  {
  TL_COUNT_BEGIN (32)

  for (;;)
    {
    TL_LOOP_BEGIN

    if (u_char < 0x80)
      {
      if ((u_char == 0) && b_null && (u_currSrcLen > 0))
        eo_err = ec_UtfNullInside;
      else
        u_currDstLen ++;
      }
    else
      {
      if (u_char < 0x800)
        u_currDstLen += 2;
      else
        {
        if (u_char < 0x10000)
          {
          if ((u_char >= 0xD800) &&
              (u_char <= 0xDFFF))
            eo_err = ec_UtfSurrogate;
          else
            u_currDstLen += 3;
          }
        else
          {
          if (u_char < 0x110000)
            u_currDstLen += 4;
          else
            eo_err = ec_UtfWideRange;
          }
        }
      }

    if (eo_err != ec_UtfOK)
      {
      pu_src = pu_char - 1;
      return eo_err;
      }

    TL_COUNT_LOOP_END
    }
  }

//---------------------------------------------------------------------------

et_UtfError TL_EXPORT tl_UtfConvert
(
t_UInt8 * pu_dst,
t_UInt u_dstLen,
const t_UInt32 * pu_src,
t_UInt u_srcLen,
bool b_null
)
  {
  TL_CONV_BEGIN (32)

  for (;;)
    {
    TL_LOOP_BEGIN

    if (u_char < 0x80)
      {
      if ((u_char == 0) && b_null && (u_currSrcLen > 0))
        eo_err = ec_UtfNullInside;
      else
        {
        if (u_currDstLen == 0)
          eo_err = ec_UtfDestTooSmall;
        else
          {
          u_currDstLen --;
          * pu_dst ++ = (t_UInt8) u_char;
          }
        }
      }
    else
      {
      eo_err = WriteU8Mb (pu_dst, u_currDstLen, u_char);
      }

    if (eo_err != ec_UtfOK)
      return eo_err;

    TL_CONV_LOOP_END
    }
  }

//---------------------------------------------------------------------------

et_UtfError TL_EXPORT tl_UtfConvertCount
(
t_UInt32 *,
t_UInt & u_dstLen,
const t_UInt16 * & pu_src,
t_UInt u_srcLen,
bool b_null
)
  {
  TL_COUNT_BEGIN (16)

  for (;;)
    {
    TL_LOOP_BEGIN

    if (u_char < 0xD800)
      {
      if ((u_char == 0) && b_null && (u_currSrcLen > 0))
        eo_err = ec_UtfNullInside;
      }
    else
      {
      if (u_char <= 0xDFFF)
        eo_err = ReadU16Surr (pu_char, u_currSrcLen, u_char);
      }

    if (eo_err != ec_UtfOK)
      {
      pu_src = pu_char - 1;
      return eo_err;
      }

    u_currDstLen ++;

    TL_COUNT_LOOP_END
    }
  }

//---------------------------------------------------------------------------

et_UtfError TL_EXPORT tl_UtfConvert
(
t_UInt32 * pu_dst,
t_UInt u_dstLen,
const t_UInt16 * pu_src,
t_UInt u_srcLen,
bool b_null
)
  {
  TL_CONV_BEGIN (16)

  for (;;)
    {
    TL_LOOP_BEGIN

    if (u_char < 0xD800)
      {
      if ((u_char == 0) && b_null && (u_currSrcLen > 0))
        eo_err = ec_UtfNullInside;
      }
    else
      {
      if (u_char <= 0xDFFF)
        eo_err = ReadU16Surr (pu_char, u_currSrcLen, u_char);
      }

    if (eo_err != ec_UtfOK)
      return eo_err;

    if (u_currDstLen == 0)
      return ec_UtfDestTooSmall;

    u_currDstLen --;
    * pu_dst ++ = u_char;

    TL_CONV_LOOP_END
    }
  }

//---------------------------------------------------------------------------

et_UtfError TL_EXPORT tl_UtfConvertCount
(
t_UInt16 *,
t_UInt & u_dstLen,
const t_UInt32 * & pu_src,
t_UInt u_srcLen,
bool b_null
)
  {
  TL_COUNT_BEGIN (32)

  for (;;)
    {
    TL_LOOP_BEGIN

    if (u_char < 0xD800)
      {
      if ((u_char == 0) && b_null && (u_currSrcLen > 0))
        eo_err = ec_UtfNullInside;
      else
        u_currDstLen ++;
      }
    else
      {
      if (u_char <= 0xDFFF)
        eo_err = ec_UtfSurrogate;
      else
        {
        if (u_char < 0x10000)
          u_currDstLen ++;
        else
          {
          if (u_char < 0x110000)
            u_currDstLen += 2;
          else
            eo_err = ec_UtfWideRange;
          }
        }
      }

    if (eo_err != ec_UtfOK)
      {
      pu_src = pu_char - 1;
      return eo_err;
      }

    TL_COUNT_LOOP_END
    }
  }

//---------------------------------------------------------------------------

et_UtfError TL_EXPORT tl_UtfConvert
(
t_UInt16 * pu_dst,
t_UInt u_dstLen,
const t_UInt32 * pu_src,
t_UInt u_srcLen,
bool b_null
)
  {
  TL_CONV_BEGIN (32)

  for (;;)
    {
    TL_LOOP_BEGIN

    if (u_char < 0x10000)
      {
      if ((u_char == 0) && b_null && (u_currSrcLen > 0))
        eo_err = ec_UtfNullInside;
      else
        {
        if ((u_char >= 0xD800) &&
            (u_char <= 0xDFFF))
          eo_err = ec_UtfSurrogate;
        else
          {
          if (u_currDstLen == 0)
            eo_err = ec_UtfDestTooSmall;
          else
            {
            u_currDstLen --;
            * pu_dst ++ = (t_UInt16) u_char;
            }
          }
        }
      }
    else
      {
      if (u_char < 0x110000)
        eo_err = WriteU16Surr (pu_dst, u_currDstLen, u_char);
      else
        eo_err = ec_UtfWideRange;
      }

    if (eo_err != ec_UtfOK)
      return eo_err;

    TL_CONV_LOOP_END
    }
  }

//---------------------------------------------------------------------------

et_UtfError TL_EXPORT tl_UtfConvertCount
(
t_UInt16 *,
t_UInt & u_dstLen,
const t_UInt8 * & pu_src,
t_UInt u_srcLen,
bool b_null
)
  {
  TL_COUNT_BEGIN (8)

  for (;;)
    {
    TL_LOOP_BEGIN

    if (u_char < 0x80)
      {
      // 0xxx xxxx
      if ((u_char == 0) && b_null && (u_currSrcLen > 0))
        eo_err = ec_UtfNullInside;
      }
    else
      {
      // 1xxx xxxx
      eo_err = ReadU8Mb (pu_char, u_currSrcLen, u_char);

      if (u_char >= 0x10000)
        u_currDstLen ++;
      }

    if (eo_err != ec_UtfOK)
      {
      pu_src = pu_char - 1;
      return eo_err;
      }

    u_currDstLen ++;

    TL_COUNT_LOOP_END
    }
  }

//---------------------------------------------------------------------------

et_UtfError TL_EXPORT tl_UtfConvert
(
t_UInt16 * pu_dst,
t_UInt u_dstLen,
const t_UInt8 * pu_src,
t_UInt u_srcLen,
bool b_null
)
  {
  TL_CONV_BEGIN (8)

  for (;;)
    {
    TL_LOOP_BEGIN

    if (u_char < 0x80)
      {
      // 0xxx xxxx
      if ((u_char == 0) && b_null && (u_currSrcLen > 0))
        eo_err = ec_UtfNullInside;
      }
    else
      {
      // 1xxx xxxx
      eo_err = ReadU8Mb (pu_char, u_currSrcLen, u_char);
      }

    if (eo_err != ec_UtfOK)
      return eo_err;

    if (u_char < 0x10000)
      {
      if (u_currDstLen == 0)
        eo_err = ec_UtfDestTooSmall;
      else
        {
        u_currDstLen --;
        * pu_dst ++ = (t_UInt16) u_char;
        }
      }
    else
      {
      eo_err = WriteU16Surr (pu_dst, u_currDstLen, u_char);
      }

    if (eo_err != ec_UtfOK)
      return eo_err;

    TL_CONV_LOOP_END
    }
  }

//---------------------------------------------------------------------------

et_UtfError TL_EXPORT tl_UtfConvertCount
(
t_UInt8 *,
t_UInt & u_dstLen,
const t_UInt16 * & pu_src,
t_UInt u_srcLen,
bool b_null
)
  {
  TL_COUNT_BEGIN (16)

  for (;;)
    {
    TL_LOOP_BEGIN

    if (u_char < 0x80)
      {
      if ((u_char == 0) && b_null && (u_currSrcLen > 0))
        eo_err = ec_UtfNullInside;
      else
        u_currDstLen ++;
      }
    else
      {
      if (u_char < 0x800)
        u_currDstLen += 2;
      else
        {
        if ((u_char >= 0xD800) &&
            (u_char <= 0xDFFF))
          {
          eo_err = ReadU16Surr (pu_char, u_currSrcLen, u_char);
          u_currDstLen += 4;
          }
        else
          u_currDstLen += 3;
        }
      }

    if (eo_err != ec_UtfOK)
      {
      pu_src = pu_char - 1;
      return eo_err;
      }

    TL_COUNT_LOOP_END
    }
  }

//---------------------------------------------------------------------------

et_UtfError TL_EXPORT tl_UtfConvert
(
t_UInt8 * pu_dst,
t_UInt u_dstLen,
const t_UInt16 * pu_src,
t_UInt u_srcLen,
bool b_null
)
  {
  TL_CONV_BEGIN (16)

  for (;;)
    {
    TL_LOOP_BEGIN

    if (u_char < 0x80)
      {
      if ((u_char == 0) && b_null && (u_currSrcLen > 0))
        eo_err = ec_UtfNullInside;
      else
        {
        if (u_currDstLen == 0)
          eo_err = ec_UtfDestTooSmall;
        else
          {
          u_currDstLen --;
          * pu_dst ++ = (t_UInt8) u_char;
          }
        }
      }
    else
      {
      if ((u_char >= 0xD800) &&
          (u_char <= 0xDFFF))
        eo_err = ReadU16Surr (pu_char, u_currSrcLen, u_char);

      if (eo_err == ec_UtfOK)
        eo_err = WriteU8Mb (pu_dst, u_currDstLen, u_char);
      }

    if (eo_err != ec_UtfOK)
      return eo_err;

    TL_CONV_LOOP_END
    }
  }

//---------------------------------------------------------------------------

et_UtfError TL_EXPORT tl_UtfLength
(
t_UInt & u_len,
const t_UInt8 * & pu_src,
t_UInt u_srcLen,
bool b_null
)
  {
  return tl_UtfConvertCount ((t_UInt32 *) 0, u_len, pu_src, u_srcLen, b_null);
  }

//---------------------------------------------------------------------------

et_UtfError TL_EXPORT tl_UtfLength
(
t_UInt & u_len,
const t_UInt16 * & pu_src,
t_UInt u_srcLen,
bool b_null
)
  {
  return tl_UtfConvertCount ((t_UInt32 *) 0, u_len, pu_src, u_srcLen, b_null);
  }

//---------------------------------------------------------------------------

et_UtfError TL_EXPORT tl_UtfLength
(
t_UInt & u_len,
const t_UInt32 * & pu_src,
t_UInt u_srcLen,
bool b_null
)
  {
  u_len = 0;

  if (u_srcLen == 0)
    {
    if (b_null)
      return ec_UtfMissingNull;
    else
      return ec_UtfOK;
    }

  t_UInt u_currSrcLen = u_srcLen;
  const t_UInt32 * pu_char = pu_src;

  for (;;)
    {
    u_currSrcLen --;
    t_UInt32 u_char = * pu_char ++;
    et_UtfError eo_err = ec_UtfOK;

    if (u_char < 0xD800)
      {
      if ((u_char == 0) && b_null && (u_currSrcLen > 0))
        eo_err = ec_UtfNullInside;
      }
    else
      {
      if (u_char <= 0xDFFF)
        eo_err = ec_UtfSurrogate;
      else
        {
        if (u_char >= 0x110000)
          eo_err = ec_UtfWideRange;
        }
      }

    if (eo_err != ec_UtfOK)
      {
      pu_src = pu_char - 1;
      return eo_err;
      }

    if (u_currSrcLen == 0)
      {
      if ((u_char != 0) && b_null)
        {
        pu_src = pu_char - 1;
        return ec_UtfMissingNull;
        }
      else
        {
        u_len = u_srcLen;
        return ec_UtfOK;
        }
      }
    }
  }

//---------------------------------------------------------------------------

static et_UtfError tl_UtfToUL
(
t_UInt8 * & pu_src,
t_UInt u_srcLen,
bool b_upper
)
  {
  if (u_srcLen == 0)
    return ec_UtfOK;

  t_UInt u_currSrcLen = u_srcLen;
  const t_UInt8 * pu_char = pu_src;

  do
    {
    u_currSrcLen --;
    t_UInt8 * pu_dst = (t_UInt8 *) pu_char;
    t_UInt32 u_char = * pu_char ++;

    if (u_char < 0x80)
      {
      // 0xxx xxxx
      t_UInt8 u_conv = b_upper ? tl_ToUpper ((char) u_char) :
                                 tl_ToLower ((char) u_char);

      if (u_conv != u_char)
        * pu_dst = u_conv;
      }
    else
      {
      // 1xxx xxxx
      et_UtfError eo_err = ReadU8Mb (pu_char, u_currSrcLen, u_char);

      if (eo_err == ec_UtfOK)
        {
        if (u_char < 0x800)
          {
          t_UInt32 u_conv = b_upper ? tl_ToUpper2 ((wchar_t) u_char) :
                                      tl_ToLower2 ((wchar_t) u_char);

          if ((u_conv != u_char) &&
              (u_conv >= 0x80) &&
              (u_conv < 0x800))
            {
            t_UInt u_dstLen = 2;
            WriteU8Mb (pu_dst, u_dstLen, u_conv);
            }
          }
        else
          {
          if (u_char < 0x10000)
            {
            t_UInt32 u_conv = b_upper ? tl_ToUpper2 ((wchar_t) u_char) :
                                        tl_ToLower2 ((wchar_t) u_char);

            if ((u_conv != u_char) &&
                (u_conv >= 0x800) &&
                (u_conv < 0x10000))
              {
              t_UInt u_dstLen = 3;
              WriteU8Mb (pu_dst, u_dstLen, u_conv);
              }
            }
          }
        }
      else
        {
        pu_src = (t_UInt8 *) (pu_char - 1);
        return eo_err;
        }
      }
    }
  while (u_currSrcLen > 0);

  return ec_UtfOK;
  }

//---------------------------------------------------------------------------

et_UtfError TL_EXPORT tl_UtfToUpper
(
t_UInt8 * & pu_src,
t_UInt u_srcLen
)
  {
  return tl_UtfToUL (pu_src, u_srcLen, true);
  }

//---------------------------------------------------------------------------

et_UtfError TL_EXPORT tl_UtfToLower
(
t_UInt8 * & pu_src,
t_UInt u_srcLen
)
  {
  return tl_UtfToUL (pu_src, u_srcLen, false);
  }

//---------------------------------------------------------------------------

static et_UtfError tl_UtfToUL
(
t_UInt16 * & pu_src,
t_UInt u_srcLen,
bool b_upper
)
  {
  if (u_srcLen == 0)
    return ec_UtfOK;

  t_UInt u_currSrcLen = u_srcLen;
  t_UInt16 * pu_char = pu_src;

  do
    {
    u_currSrcLen --;
    t_UInt32 u_char = * pu_char;

    if (u_char < 0x80)
      {
      t_UInt8 u_conv = b_upper ? tl_ToUpper ((char) u_char) :
                                 tl_ToLower ((char) u_char);

      if (u_conv != u_char)
        * pu_char = u_conv;
      }
    else
      {
      if ((u_char >= 0xD800) &&
          (u_char <= 0xDFFF))
        {
        const t_UInt16 * pu_cchar = pu_char + 1;
        et_UtfError eo_err = ReadU16Surr (pu_cchar, u_currSrcLen, u_char);

        if (eo_err == ec_UtfOK)
          pu_char ++;
        else
          {
          pu_src = (t_UInt16 *) (pu_cchar - 1);
          return eo_err;
          }
        }
      else
        {
        t_UInt32 u_conv = b_upper ? tl_ToUpper2 ((wchar_t) u_char) :
                                    tl_ToLower2 ((wchar_t) u_char);

        if ((u_conv != u_char) &&
            (u_conv < 0x10000))
          if ((u_conv < 0xD800) ||
              (u_conv > 0xDFFF))
            * pu_char = (t_UInt16) u_conv;
        }
      }

    pu_char ++;
    }
  while (u_currSrcLen > 0);

  return ec_UtfOK;
  }

//---------------------------------------------------------------------------

et_UtfError TL_EXPORT tl_UtfToUpper
(
t_UInt16 * & pu_src,
t_UInt u_srcLen
)
  {
  return tl_UtfToUL (pu_src, u_srcLen, true);
  }

//---------------------------------------------------------------------------

et_UtfError TL_EXPORT tl_UtfToLower
(
t_UInt16 * & pu_src,
t_UInt u_srcLen
)
  {
  return tl_UtfToUL (pu_src, u_srcLen, false);
  }

//---------------------------------------------------------------------------

static et_UtfError tl_UtfToUL
(
t_UInt32 * & pu_src,
t_UInt u_srcLen,
bool b_upper
)
  {
  if (u_srcLen == 0)
    return ec_UtfOK;

  t_UInt u_currSrcLen = u_srcLen;
  t_UInt32 * pu_char = pu_src;

  do
    {
    u_currSrcLen --;
    t_UInt32 u_char = * pu_char;

    if (u_char < 0x80)
      {
      t_UInt8 u_conv = b_upper ? tl_ToUpper ((char) u_char) :
                                 tl_ToLower ((char) u_char);

      if (u_conv != u_char)
        * pu_char = u_conv;
      }
    else
      {
      et_UtfError eo_err = ec_UtfOK;

      if ((u_char >= 0xD800) &&
          (u_char <= 0xDFFF))
        eo_err = ec_UtfSurrogate;
      else
        {
        if (u_char < 0x10000)
          {
          t_UInt32 u_conv = b_upper ? tl_ToUpper2 ((wchar_t) u_char) :
                                      tl_ToLower2 ((wchar_t) u_char);

          if ((u_conv != u_char) &&
              (u_conv < 0x10000))
            if ((u_conv < 0xD800) ||
                (u_conv > 0xDFFF))
              * pu_char = u_conv;
          }
        else
          {
          if (u_char >= 0x110000)
            eo_err = ec_UtfWideRange;
          }
        }

      if (eo_err != ec_UtfOK)
        {
        pu_src = pu_char;
        return eo_err;
        }
      }

    pu_char ++;
    }
  while (u_currSrcLen > 0);

  return ec_UtfOK;
  }

//---------------------------------------------------------------------------

et_UtfError TL_EXPORT tl_UtfToUpper
(
t_UInt32 * & pu_src,
t_UInt u_srcLen
)
  {
  return tl_UtfToUL (pu_src, u_srcLen, true);
  }

//---------------------------------------------------------------------------

et_UtfError TL_EXPORT tl_UtfToLower
(
t_UInt32 * & pu_src,
t_UInt u_srcLen
)
  {
  return tl_UtfToUL (pu_src, u_srcLen, false);
  }

//---------------------------------------------------------------------------

et_UtfError tl_UtfNext
(
t_UInt32 & u_char,
const t_UInt8 * & pu_src,
t_UInt & u_srcLen
)
  {
  if (u_srcLen == 0)
    return ec_UtfEOS;

  u_srcLen --;
  u_char = * pu_src ++;

  if (u_char >= 0x80)
    return ReadU8Mb (pu_src, u_srcLen, u_char);
  else
    return ec_UtfOK;
  }

//---------------------------------------------------------------------------

et_UtfError tl_UtfNext
(
t_UInt32 & u_char,
const t_UInt16 * & pu_src,
t_UInt & u_srcLen
)
  {
  if (u_srcLen == 0)
    return ec_UtfEOS;

  u_srcLen --;
  u_char = * pu_src ++;

  if ((u_char >= 0xD800) &&
      (u_char <= 0xDFFF))
    return ReadU16Surr (pu_src, u_srcLen, u_char);
  else
    return ec_UtfOK;
  }

//---------------------------------------------------------------------------

et_UtfError tl_UtfNext
(
t_UInt32 & u_char,
const t_UInt32 * & pu_src,
t_UInt & u_srcLen
)
  {
  if (u_srcLen == 0)
    return ec_UtfEOS;

  u_srcLen --;
  u_char = * pu_src ++;

  if ((u_char >= 0xD800) &&
      (u_char <= 0xDFFF))
    return ec_UtfSurrogate;

  if (u_char >= 0x110000)
    return ec_UtfWideRange;
  else
    return ec_UtfOK;
  }
