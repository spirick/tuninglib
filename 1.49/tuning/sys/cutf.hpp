
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
// File tuning/sys/cutf.hpp

#ifndef TUNING_SYS_CUTF_HPP
#define TUNING_SYS_CUTF_HPP

#include "tuning/defs.hpp"

//---------------------------------------------------------------------------
// UTF error codes

enum et_UtfError
  {
  ec_UtfOK = 0,
  ec_UtfMissingNull,      // Missing null character
  ec_UtfNullInside,       // Null character inside of string
  ec_UtfMbMissingStart,   // Multibyte (10xx xxxx) without startbyte (11xx xxxx)
  ec_UtfMbInvalidStart,   // Invalid startbyte (1111 1xxx)
  ec_UtfMbExpected,       // Multibyte (10xx xxxx) expected
  ec_UtfMbEnd,            // String end in multibyte sequence
  ec_UtfWideRange,        // Wide character out of range
  ec_UtfSurrogate,        // UTF-16 surrogate in wide character
  ec_UtfHighSurrExpected, // High surrogate expected
  ec_UtfLowSurrExpected,  // Low surrogate expected
  ec_UtfSurrEnd,          // String end in surrogate
  ec_UtfDestTooSmall,     // Destination buffer size too small
  ec_UtfDestTooLarge,     // Destination buffer size too large
  ec_UtfEOS,              // End of string
  ec_UtfLastError
  };

//---------------------------------------------------------------------------

// UTF-8 <-> UTF-32
et_UtfError TL_EXPORT tl_UtfConvertCount (t_UInt32 *, t_UInt & u_dstLen,
                        const t_UInt8 * & pu_src, t_UInt u_srcLen, bool b_null = true);
et_UtfError TL_EXPORT tl_UtfConvert (t_UInt32 * pu_dst, t_UInt u_dstLen,
                        const t_UInt8 * pu_src, t_UInt u_srcLen, bool b_null = true);
et_UtfError TL_EXPORT tl_UtfConvertCount (t_UInt8 *, t_UInt & u_dstLen,
                        const t_UInt32 * & pu_src, t_UInt u_srcLen, bool b_null = true);
et_UtfError TL_EXPORT tl_UtfConvert (t_UInt8 * pu_dst, t_UInt u_dstLen,
                        const t_UInt32 * pu_src, t_UInt u_srcLen, bool b_null = true);

// UTF-16 <-> UTF-32
et_UtfError TL_EXPORT tl_UtfConvertCount (t_UInt32 *, t_UInt & u_dstLen,
                        const t_UInt16 * & pu_src, t_UInt u_srcLen, bool b_null = true);
et_UtfError TL_EXPORT tl_UtfConvert (t_UInt32 * pu_dst, t_UInt u_dstLen,
                        const t_UInt16 * pu_src, t_UInt u_srcLen, bool b_null = true);
et_UtfError TL_EXPORT tl_UtfConvertCount (t_UInt16 *, t_UInt & u_dstLen,
                        const t_UInt32 * & pu_src, t_UInt u_srcLen, bool b_null = true);
et_UtfError TL_EXPORT tl_UtfConvert (t_UInt16 * pu_dst, t_UInt u_dstLen,
                        const t_UInt32 * pu_src, t_UInt u_srcLen, bool b_null = true);

// UTF-8 <-> UTF-16
et_UtfError TL_EXPORT tl_UtfConvertCount (t_UInt16 *, t_UInt & u_dstLen,
                        const t_UInt8 * & pu_src, t_UInt u_srcLen, bool b_null = true);
et_UtfError TL_EXPORT tl_UtfConvert (t_UInt16 * pu_dst, t_UInt u_dstLen,
                        const t_UInt8 * pu_src, t_UInt u_srcLen, bool b_null = true);
et_UtfError TL_EXPORT tl_UtfConvertCount (t_UInt8 *, t_UInt & u_dstLen,
                        const t_UInt16 * & pu_src, t_UInt u_srcLen, bool b_null = true);
et_UtfError TL_EXPORT tl_UtfConvert (t_UInt8 * pu_dst, t_UInt u_dstLen,
                        const t_UInt16 * pu_src, t_UInt u_srcLen, bool b_null = true);

// UTF length
et_UtfError TL_EXPORT tl_UtfLength (t_UInt & u_len, const t_UInt8 * & pu_src,
                        t_UInt u_srcLen, bool b_null = true);
et_UtfError TL_EXPORT tl_UtfLength (t_UInt & u_len, const t_UInt16 * & pu_src,
                        t_UInt u_srcLen, bool b_null = true);
et_UtfError TL_EXPORT tl_UtfLength (t_UInt & u_len, const t_UInt32 * & pu_src,
                        t_UInt u_srcLen, bool b_null = true);

// UTF upper/lower
et_UtfError TL_EXPORT tl_UtfToUpper (t_UInt8 * & pu_src, t_UInt u_srcLen);
et_UtfError TL_EXPORT tl_UtfToLower (t_UInt8 * & pu_src, t_UInt u_srcLen);
et_UtfError TL_EXPORT tl_UtfToUpper (t_UInt16 * & pu_src, t_UInt u_srcLen);
et_UtfError TL_EXPORT tl_UtfToLower (t_UInt16 * & pu_src, t_UInt u_srcLen);
et_UtfError TL_EXPORT tl_UtfToUpper (t_UInt32 * & pu_src, t_UInt u_srcLen);
et_UtfError TL_EXPORT tl_UtfToLower (t_UInt32 * & pu_src, t_UInt u_srcLen);

// UTF const iterator
et_UtfError tl_UtfNext (t_UInt32 & u_char, const t_UInt8 * & pu_src, t_UInt & u_srcLen);
et_UtfError tl_UtfNext (t_UInt32 & u_char, const t_UInt16 * & pu_src, t_UInt & u_srcLen);
et_UtfError tl_UtfNext (t_UInt32 & u_char, const t_UInt32 * & pu_src, t_UInt & u_srcLen);

#endif
