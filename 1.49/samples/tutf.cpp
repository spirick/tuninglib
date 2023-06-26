
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
// File samples/tutf.cpp

#include <stdio.h>
#include <locale.h>
#include "tuning/utfcit.h"
#include "tuning/sys/cstring.hpp"
#include "tuning/sys/cmemory.hpp"

//---------------------------------------------------------------------------

const t_UInt u_MaxDestSize = 16;
t_UInt8  au_Dest8  [u_MaxDestSize];
t_UInt16 au_Dest16 [u_MaxDestSize];
t_UInt32 au_Dest32 [u_MaxDestSize];

//---------------------------------------------------------------------------

void PrintError
(
et_UtfError eo_err
)
  {
  switch (eo_err)
    {
    case ec_UtfOK:               printf ("OK "); break;
    case ec_UtfMissingNull:      printf ("MissingNull "); break;
    case ec_UtfNullInside:       printf ("NullInside "); break;
    case ec_UtfMbMissingStart:   printf ("MbMissingStart "); break;
    case ec_UtfMbInvalidStart:   printf ("MbInvalidStart "); break;
    case ec_UtfMbExpected:       printf ("MbExpected "); break;
    case ec_UtfMbEnd:            printf ("MbEnd "); break;
    case ec_UtfWideRange:        printf ("WideRange "); break;
    case ec_UtfSurrogate:        printf ("Surrogate "); break;
    case ec_UtfHighSurrExpected: printf ("HighSurrExpected "); break;
    case ec_UtfLowSurrExpected:  printf ("LowSurrExpected "); break;
    case ec_UtfSurrEnd:          printf ("SurrEnd "); break;
    case ec_UtfDestTooSmall:     printf ("DestTooSmall "); break;
    case ec_UtfDestTooLarge:     printf ("DestTooLarge "); break;
    case ec_UtfEOS:              printf ("EndOfString "); break;
    default:                     printf ("??? "); break;
    }
  }

//---------------------------------------------------------------------------

template <class t_char1, class t_char2>
  void TestCount
  (
  t_char1 *,
  t_UInt u_dstLen,
  const t_char2 * pu_src,
  t_UInt u_srcLen,
  bool b_null,
  et_UtfError eo_err,
  t_UInt u_pos
  )
    {
    const t_char2 * pu_src2 = pu_src;
    t_UInt u_dstLen2;
    et_UtfError eo_ret = tl_UtfConvertCount ((t_char1 *) 0, u_dstLen2, pu_src2, u_srcLen, b_null);
    t_UInt u_diff = pu_src2 - pu_src;

    if (eo_ret != eo_err)
      printf ("* * * Error code %d %d\n", eo_ret, eo_err);

    if (u_diff != u_pos)
      printf ("* * * Error pos %d %d\n", (unsigned) u_diff, (unsigned) u_pos);

    if (u_dstLen2 != u_dstLen)
      printf ("* * * Error len %d %d\n", (unsigned) u_dstLen2, (unsigned) u_dstLen);

    for (t_UInt u_src = 0; u_src < u_srcLen; u_src ++)
      {
      if (u_src == u_diff)
        printf (">");

      printf ("%X ", pu_src [u_src]);
      }

    PrintError (eo_ret);
    printf (" %d\n", (unsigned) u_dstLen2);
    }

//---------------------------------------------------------------------------

void TestCount32_8 (t_UInt u_dstLen, const char * pc_src, t_UInt u_srcLen, bool b_null, et_UtfError eo_err, t_UInt u_pos)
  {
  TestCount ((t_UInt32 *) 0, u_dstLen, (const t_UInt8 *) pc_src, u_srcLen, b_null, eo_err, u_pos);
  }

void TestCount32_8_nn (t_UInt u_dstLen, const char * pc_src, et_UtfError eo_err, t_UInt u_pos)
  {
  TestCount32_8 (u_dstLen, pc_src, tl_StringLength (pc_src), false, eo_err, u_pos);
  }

//--------------------------------------

#define _ ,

#define TestCount8_32(u_dstLen, au_src, b_null, eo_err, u_pos)                      \
  {                                                                                 \
  const t_UInt32 au [] = { au_src };                                                \
  TestCount ((t_UInt8 *) 0, u_dstLen, au, sizeof (au) / 4, b_null, eo_err, u_pos);  \
  }

#define TestCount8_32_nn(u_dstLen, au_src, eo_err, u_pos)                           \
  {                                                                                 \
  const t_UInt32 au [] = { au_src };                                                \
  TestCount ((t_UInt8 *) 0, u_dstLen, au, sizeof (au) / 4, false, eo_err, u_pos);   \
  }

//--------------------------------------

#define TestCount32_16(u_dstLen, au_src, b_null, eo_err, u_pos)                     \
  {                                                                                 \
  const t_UInt16 au [] = { au_src };                                                \
  TestCount ((t_UInt32 *) 0, u_dstLen, au, sizeof (au) / 2, b_null, eo_err, u_pos); \
  }

#define TestCount32_16_nn(u_dstLen, au_src, eo_err, u_pos)                          \
  {                                                                                 \
  const t_UInt16 au [] = { au_src };                                                \
  TestCount ((t_UInt32 *) 0, u_dstLen, au, sizeof (au) / 2, false, eo_err, u_pos);  \
  }

//--------------------------------------

#define TestCount16_32(u_dstLen, au_src, b_null, eo_err, u_pos)                     \
  {                                                                                 \
  const t_UInt32 au [] = { au_src };                                                \
  TestCount ((t_UInt16 *) 0, u_dstLen, au, sizeof (au) / 4, b_null, eo_err, u_pos); \
  }

#define TestCount16_32_nn(u_dstLen, au_src, eo_err, u_pos)                          \
  {                                                                                 \
  const t_UInt32 au [] = { au_src };                                                \
  TestCount ((t_UInt16 *) 0, u_dstLen, au, sizeof (au) / 4, false, eo_err, u_pos);  \
  }

//---------------------------------------------------------------------------

void TestCount16_8 (t_UInt u_dstLen, const char * pc_src, t_UInt u_srcLen, bool b_null, et_UtfError eo_err, t_UInt u_pos)
  {
  TestCount ((t_UInt16 *) 0, u_dstLen, (const t_UInt8 *) pc_src, u_srcLen, b_null, eo_err, u_pos);
  }

void TestCount16_8_nn (t_UInt u_dstLen, const char * pc_src, et_UtfError eo_err, t_UInt u_pos)
  {
  TestCount16_8 (u_dstLen, pc_src, tl_StringLength (pc_src), false, eo_err, u_pos);
  }

//--------------------------------------

#define TestCount8_16(u_dstLen, au_src, b_null, eo_err, u_pos)                      \
  {                                                                                 \
  const t_UInt16 au [] = { au_src };                                                \
  TestCount ((t_UInt8 *) 0, u_dstLen, au, sizeof (au) / 2, b_null, eo_err, u_pos);  \
  }

#define TestCount8_16_nn(u_dstLen, au_src, eo_err, u_pos)                           \
  {                                                                                 \
  const t_UInt16 au [] = { au_src };                                                \
  TestCount ((t_UInt8 *) 0, u_dstLen, au, sizeof (au) / 2, false, eo_err, u_pos);   \
  }

//---------------------------------------------------------------------------

template <class t_char1, class t_char2>
  void TestConv
  (
  t_char1 * pu_dst,
  t_UInt u_dstLen,
  const t_char2 * pu_src,
  t_UInt u_srcLen,
  bool b_null,
  et_UtfError eo_err,
  t_char2 * pu_back
  )
    {
    TL_ASSERT (u_dstLen <= u_MaxDestSize);
    TL_ASSERT (u_srcLen <= u_MaxDestSize);

    for (t_UInt u_dst = 0; u_dst < u_dstLen; u_dst ++)
      pu_dst [u_dst] = (t_char1) -1;

    et_UtfError eo_ret = tl_UtfConvert (pu_dst, u_dstLen, pu_src, u_srcLen, b_null);

    if (eo_ret != eo_err)
      printf ("* * * Error code %d %d\n", eo_ret, eo_err);

    for (t_UInt u_src = 0; u_src < u_srcLen; u_src ++)
      printf ("%X ", pu_src [u_src]);

    if (eo_ret != ec_UtfOK)
      PrintError (eo_ret);
    else
      {
      if (u_dstLen == 0)
        printf ("OK");
      else
        {
        printf ("-> ");

        for (t_UInt u_dst = 0; u_dst < u_dstLen; u_dst ++)
          printf ("%X ", pu_dst [u_dst]);

        // Back conversion
        t_UInt u_calcLen = 0;
        const t_char1 * pu_calc = (const t_char1 *) pu_dst;
        eo_ret = tl_UtfConvertCount ((t_char2 *) 0, u_calcLen, pu_calc, u_dstLen, b_null);
        PrintError (eo_ret);

        if (u_calcLen != u_srcLen)
          printf ("* * * CalcLen ");

        eo_ret = tl_UtfConvert (pu_back, u_srcLen, (const t_char1 *) pu_dst, u_dstLen, b_null);
        PrintError (eo_ret);
        int i_ret = tl_CompareMemory ((const char *) pu_src, (const char *) pu_back,
          u_srcLen * sizeof (t_char2));

        if (i_ret != 0)
          printf ("* * * CompareMemory ");
        }
      }

    printf ("\n");
    }

//---------------------------------------------------------------------------

void TestConv32_8 (t_UInt u_dstLen, const char * pc_src, t_UInt u_srcLen, bool b_null, et_UtfError eo_err)
  {
  TestConv (au_Dest32, u_dstLen, (const t_UInt8 *) pc_src, u_srcLen, b_null, eo_err, au_Dest8);
  }

void TestConv32_8_nn (t_UInt u_dstLen, const char * pc_src, et_UtfError eo_err)
  {
  TestConv32_8 (u_dstLen, pc_src, tl_StringLength (pc_src), false, eo_err);
  }

//--------------------------------------

#define TestConv8_32(u_dstLen, au_src, b_null, eo_err)                            \
  {                                                                               \
  const t_UInt32 au [] = { au_src };                                              \
  TestConv (au_Dest8, u_dstLen, au, sizeof (au) / 4, b_null, eo_err, au_Dest32);  \
  }

#define TestConv8_32_nn(u_dstLen, au_src, eo_err)                                 \
  {                                                                               \
  const t_UInt32 au [] = { au_src };                                              \
  TestConv (au_Dest8, u_dstLen, au, sizeof (au) / 4, false, eo_err, au_Dest32);   \
  }

//--------------------------------------

#define TestConv32_16(u_dstLen, au_src, b_null, eo_err)                           \
  {                                                                               \
  const t_UInt16 au [] = { au_src };                                              \
  TestConv (au_Dest32, u_dstLen, au, sizeof (au) / 2, b_null, eo_err, au_Dest16); \
  }

#define TestConv32_16_nn(u_dstLen, au_src, eo_err)                                \
  {                                                                               \
  const t_UInt16 au [] = { au_src };                                              \
  TestConv (au_Dest32, u_dstLen, au, sizeof (au) / 2, false, eo_err, au_Dest16);  \
  }

//--------------------------------------

#define TestConv16_32(u_dstLen, au_src, b_null, eo_err)                           \
  {                                                                               \
  const t_UInt32 au [] = { au_src };                                              \
  TestConv (au_Dest16, u_dstLen, au, sizeof (au) / 4, b_null, eo_err, au_Dest32); \
  }

#define TestConv16_32_nn(u_dstLen, au_src, eo_err)                                \
  {                                                                               \
  const t_UInt32 au [] = { au_src };                                              \
  TestConv (au_Dest16, u_dstLen, au, sizeof (au) / 4, false, eo_err, au_Dest32);  \
  }

//--------------------------------------

void TestConv16_8 (t_UInt u_dstLen, const char * pc_src, t_UInt u_srcLen, bool b_null, et_UtfError eo_err)
  {
  TestConv (au_Dest16, u_dstLen, (const t_UInt8 *) pc_src, u_srcLen, b_null, eo_err, au_Dest8);
  }

void TestConv16_8_nn (t_UInt u_dstLen, const char * pc_src, et_UtfError eo_err)
  {
  TestConv16_8 (u_dstLen, pc_src, tl_StringLength (pc_src), false, eo_err);
  }

//--------------------------------------

#define TestConv8_16(u_dstLen, au_src, b_null, eo_err)                            \
  {                                                                               \
  const t_UInt16 au [] = { au_src };                                              \
  TestConv (au_Dest8, u_dstLen, au, sizeof (au) / 2, b_null, eo_err, au_Dest16);  \
  }

#define TestConv8_16_nn(u_dstLen, au_src, eo_err)                                 \
  {                                                                               \
  const t_UInt16 au [] = { au_src };                                              \
  TestConv (au_Dest8, u_dstLen, au, sizeof (au) / 2, false, eo_err, au_Dest16);   \
  }

//---------------------------------------------------------------------------

template <class t_char>
  void TestLength
  (
  t_UInt u_len,
  const t_char * pu_src,
  t_UInt u_srcLen,
  bool b_null,
  et_UtfError eo_err,
  t_UInt u_pos
  )
    {
    const t_char * pu_src2 = pu_src;
    t_UInt u_len2;
    et_UtfError eo_ret = tl_UtfLength (u_len2, pu_src2, u_srcLen, b_null);
    t_UInt u_diff = pu_src2 - pu_src;

    if (eo_ret != eo_err)
      printf ("* * * Error code %d %d\n", eo_ret, eo_err);

    if (u_diff != u_pos)
      printf ("* * * Error pos %d %d\n", (unsigned) u_diff, (unsigned) u_pos);

    if (u_len2 != u_len)
      printf ("* * * Error len %d %d\n", (unsigned) u_len2, (unsigned) u_len);

    for (t_UInt u_src = 0; u_src < u_srcLen; u_src ++)
      {
      if (u_src == u_diff)
        printf (">");

      printf ("%X ", pu_src [u_src]);
      }

    PrintError (eo_ret);
    printf (" %d\n", (unsigned) u_len2);
    }

//--------------------------------------

void TestLength8_nn (t_UInt u_len, const char * pc_src, et_UtfError eo_err, t_UInt u_pos)
  {
  TestLength (u_len, (const t_UInt8 *) pc_src, tl_StringLength (pc_src), false, eo_err, u_pos);
  }

#define TestLength16_nn(u_len, au_src, eo_err, u_pos)              \
  {                                                                \
  const t_UInt16 au [] = { au_src };                               \
  TestLength (u_len, au, sizeof (au) / 2, false, eo_err, u_pos);   \
  }

//--------------------------------------

#define TestLength32(u_len, au_src, b_null, eo_err, u_pos)         \
  {                                                                \
  const t_UInt32 au [] = { au_src };                               \
  TestLength (u_len, au, sizeof (au) / 4, b_null, eo_err, u_pos);  \
  }

#define TestLength32_nn(u_len, au_src, eo_err, u_pos)              \
  {                                                                \
  const t_UInt32 au [] = { au_src };                               \
  TestLength (u_len, au, sizeof (au) / 4, false, eo_err, u_pos);   \
  }

//---------------------------------------------------------------------------

template <class t_char>
  void TestUL
  (
  t_char * pu_src,
  t_UInt u_srcLen
  )
    {
    for (t_UInt u_src = 0; u_src < u_srcLen; u_src ++)
      printf ("%X ", pu_src [u_src]);

    t_char * pu_src2 = pu_src;
    et_UtfError eo_ret = tl_UtfToUpper (pu_src2, u_srcLen);
    PrintError (eo_ret);

    if (eo_ret != ec_UtfOK)
      {
      t_UInt u_diff = pu_src2 - pu_src;

      for (t_UInt u_src = 0; u_src < u_srcLen; u_src ++)
        {
        if (u_src == u_diff)
          printf (">");

        printf ("%X ", pu_src [u_src]);
        }
      }
    else
      {
      printf ("-> ");

      for (t_UInt u_src = 0; u_src < u_srcLen; u_src ++)
        printf ("%X ", pu_src [u_src]);

      pu_src2 = pu_src;
      eo_ret = tl_UtfToLower (pu_src2, u_srcLen);
      PrintError (eo_ret);

      if (eo_ret == ec_UtfOK)
        {
        printf ("-> ");

        for (t_UInt u_src = 0; u_src < u_srcLen; u_src ++)
          printf ("%X ", pu_src [u_src]);
        }
      }

    printf ("\n");
    }

//--------------------------------------

#define TestUL8(pc_src)         \
  {                             \
  t_UInt8 ac [] = pc_src;       \
  TestUL (ac, sizeof (ac) - 1); \
  }

#define TestUL16(au_src)        \
  {                             \
  t_UInt16 au [] = { au_src };  \
  TestUL (au, sizeof (au) / 2); \
  }

#define TestUL32(au_src)        \
  {                             \
  t_UInt32 au [] = { au_src };  \
  TestUL (au, sizeof (au) / 4); \
  }

//---------------------------------------------------------------------------

template <class t_char>
  void TestCit
  (
  const t_char * pu_src,
  t_UInt u_srcLen,
  et_UtfError eo_err
  )
    {
    gct_UtfCit <t_char> co_cit;
    //gct_UtfCit <t_char> co_cit (pu_src, u_srcLen);

    for (co_cit. First (pu_src, u_srcLen);
         co_cit. Ready ();
         co_cit. Next ())
      printf ("%X %d %d %d; ", co_cit. GetChar (), (unsigned) co_cit. GetCharPos (),
        (unsigned) co_cit. GetRawPos (), (unsigned) co_cit. GetRawLen ());

    if (co_cit. GetError () == eo_err)
      printf ("OK\n");
    else
      {
      PrintError (co_cit. GetError ());
      printf ("\n");
      }
    }

//--------------------------------------

#define TestCit8(pc_src, eo_err)         \
  {                                      \
  t_UInt8 ac [] = pc_src;                \
  TestCit (ac, sizeof (ac) - 1, eo_err); \
  }

#define TestCit16(au_src, eo_err)        \
  {                                      \
  t_UInt16 au [] = { au_src };           \
  TestCit (au, sizeof (au) / 2, eo_err); \
  }

#define TestCit32(au_src, eo_err)        \
  {                                      \
  t_UInt32 au [] = { au_src };           \
  TestCit (au, sizeof (au) / 4, eo_err); \
  }

//---------------------------------------------------------------------------

int main ()
  {
  printf ("\n");
  printf ("Count UTF-32 = UTF-8 --------------------------------------\n\n");

  TestCount32_8 (0, "",     0, true,  ec_UtfMissingNull, 0);
  TestCount32_8 (0, "",     0, false, ec_UtfOK,          0);
  TestCount32_8 (1, "",     1, true,  ec_UtfOK,          0);
  TestCount32_8 (1, "",     1, false, ec_UtfOK,          0);
  TestCount32_8 (0, "abc",  3, true,  ec_UtfMissingNull, 2);
  TestCount32_8 (3, "abc",  3, false, ec_UtfOK,          0);
  TestCount32_8 (4, "abc",  4, true,  ec_UtfOK,          0);
  TestCount32_8 (4, "abc",  4, false, ec_UtfOK,          0);
  TestCount32_8 (0, "a\0c", 4, true,  ec_UtfNullInside,  1);
  TestCount32_8 (4, "a\0c", 4, false, ec_UtfOK,          0);
  TestCount32_8 (0, "ab\0", 4, true,  ec_UtfNullInside,  2);
  TestCount32_8 (4, "ab\0", 4, false, ec_UtfOK,          0);
  printf ("\n");

  TestCount32_8_nn (3, "ghi",                ec_UtfOK, 0);
  TestCount32_8_nn (3, "gh\xC3\xA4",         ec_UtfOK, 0);
  TestCount32_8_nn (3, "g\xC3\xA4h",         ec_UtfOK, 0);
  TestCount32_8_nn (3, "\xC3\xA4gh",         ec_UtfOK, 0);
  TestCount32_8_nn (3, "gh\xE2\x82\xAC",     ec_UtfOK, 0);
  TestCount32_8_nn (3, "g\xE2\x82\xACh",     ec_UtfOK, 0);
  TestCount32_8_nn (3, "\xE2\x82\xACgh",     ec_UtfOK, 0);
  TestCount32_8_nn (3, "gh\xF0\x9D\x84\x9E", ec_UtfOK, 0);
  TestCount32_8_nn (3, "g\xF0\x9D\x84\x9Eh", ec_UtfOK, 0);
  TestCount32_8_nn (3, "\xF0\x9D\x84\x9Egh", ec_UtfOK, 0);
  printf ("\n");

  TestCount32_8_nn (0, "gh\xC3",         ec_UtfMbEnd, 2);
  TestCount32_8_nn (0, "gh\xE2\x82",     ec_UtfMbEnd, 2);
  TestCount32_8_nn (0, "gh\xF0\x9D\x84", ec_UtfMbEnd, 2);
  printf ("\n");

  TestCount32_8_nn (0, "gh\x80", ec_UtfMbMissingStart, 2);
  TestCount32_8_nn (0, "gh\xBF", ec_UtfMbMissingStart, 2);
  TestCount32_8_nn (0, "gh\xC0", ec_UtfMbEnd, 2);
  TestCount32_8_nn (0, "gh\xF7", ec_UtfMbEnd, 2);
  TestCount32_8_nn (0, "gh\xF8", ec_UtfMbInvalidStart, 2);
  TestCount32_8_nn (0, "gh\xFF", ec_UtfMbInvalidStart, 2);
  printf ("\n");

  TestCount32_8_nn (0, "gh\xC3\x7F",         ec_UtfMbExpected, 3);
  TestCount32_8_nn (3, "gh\xC3\x80",         ec_UtfOK, 0);
  TestCount32_8_nn (3, "gh\xC3\xBF",         ec_UtfOK, 0);
  TestCount32_8_nn (0, "gh\xC3\xC0",         ec_UtfMbExpected, 3);
  TestCount32_8_nn (0, "gh\xE2\x82\x7F",     ec_UtfMbExpected, 4);
  TestCount32_8_nn (3, "gh\xE2\x82\x80",     ec_UtfOK, 0);
  TestCount32_8_nn (3, "gh\xE2\x82\xBF",     ec_UtfOK, 0);
  TestCount32_8_nn (0, "gh\xE2\x82\xC0",     ec_UtfMbExpected, 4);
  TestCount32_8_nn (0, "gh\xF0\x9D\x84\x7F", ec_UtfMbExpected, 5);
  TestCount32_8_nn (3, "gh\xF0\x9D\x84\x80", ec_UtfOK, 0);
  TestCount32_8_nn (3, "gh\xF0\x9D\x84\xBF", ec_UtfOK, 0);
  TestCount32_8_nn (0, "gh\xF0\x9D\x84\xC0", ec_UtfMbExpected, 5);
  printf ("\n");

  TestCount32_8_nn (0, "gh\xC0\x80",         ec_UtfWideRange, 3);
  TestCount32_8_nn (0, "gh\xC1\xBF",         ec_UtfWideRange, 3);
  TestCount32_8_nn (3, "gh\xC2\x80",         ec_UtfOK, 0);
  TestCount32_8_nn (3, "gh\xDF\xBF",         ec_UtfOK, 0);
  TestCount32_8_nn (0, "gh\xE0\x80\x80",     ec_UtfWideRange, 4);
  TestCount32_8_nn (0, "gh\xE0\x9F\xBF",     ec_UtfWideRange, 4);
  TestCount32_8_nn (3, "gh\xE0\xA0\x80",     ec_UtfOK, 0);
  TestCount32_8_nn (3, "gh\xEF\xBF\xBF",     ec_UtfOK, 0);
  TestCount32_8_nn (0, "gh\xF0\x80\x80\x80", ec_UtfWideRange, 5);
  TestCount32_8_nn (0, "gh\xF0\x8F\xBF\xBF", ec_UtfWideRange, 5);
  TestCount32_8_nn (3, "gh\xF0\x90\x80\x80", ec_UtfOK, 0);
  TestCount32_8_nn (3, "gh\xF4\x8F\xBF\xBF", ec_UtfOK, 0);
  TestCount32_8_nn (0, "gh\xF4\x90\x80\x80", ec_UtfWideRange, 5);
  printf ("\n");

  TestCount32_8_nn (3, "gh\xED\x9F\xBF", ec_UtfOK, 0);
  TestCount32_8_nn (0, "gh\xED\xA0\x80", ec_UtfSurrogate, 4);
  TestCount32_8_nn (0, "gh\xED\xBF\xBF", ec_UtfSurrogate, 4);
  TestCount32_8_nn (3, "gh\xEE\x80\x80", ec_UtfOK, 0);
  printf ("\n");

  printf ("Conv UTF-32 = UTF-8 --------------------------------------\n\n");

  TestConv32_8 (1, "",     0, false, ec_UtfDestTooLarge);
  TestConv32_8 (0, "",     1, true,  ec_UtfDestTooSmall);
  TestConv32_8 (0, "",     1, false, ec_UtfDestTooSmall);
  TestConv32_8 (2, "",     1, true,  ec_UtfDestTooLarge);
  TestConv32_8 (2, "",     1, false, ec_UtfDestTooLarge);
  TestConv32_8 (3, "abc",  4, true,  ec_UtfDestTooSmall);
  TestConv32_8 (3, "abc",  4, false, ec_UtfDestTooSmall);
  TestConv32_8 (5, "abc",  4, true,  ec_UtfDestTooLarge);
  TestConv32_8 (5, "abc",  4, false, ec_UtfDestTooLarge);
  printf ("\n");

  TestConv32_8 (0, "",     0, true,  ec_UtfMissingNull);
  TestConv32_8 (0, "",     0, false, ec_UtfOK);
  TestConv32_8 (1, "",     1, true,  ec_UtfOK);
  TestConv32_8 (1, "",     1, false, ec_UtfOK);
  TestConv32_8 (3, "abc",  3, true,  ec_UtfMissingNull);
  TestConv32_8 (3, "abc",  3, false, ec_UtfOK);
  TestConv32_8 (4, "abc",  4, true,  ec_UtfOK);
  TestConv32_8 (4, "abc",  4, false, ec_UtfOK);
  TestConv32_8 (4, "a\0c", 4, true,  ec_UtfNullInside);
  TestConv32_8 (4, "a\0c", 4, false, ec_UtfOK);
  TestConv32_8 (4, "ab\0", 4, true,  ec_UtfNullInside);
  TestConv32_8 (4, "ab\0", 4, false, ec_UtfOK);
  printf ("\n");

  TestConv32_8_nn (3, "ghi",                ec_UtfOK);
  TestConv32_8_nn (3, "gh\xC3\xA4",         ec_UtfOK);
  TestConv32_8_nn (3, "g\xC3\xA4h",         ec_UtfOK);
  TestConv32_8_nn (3, "\xC3\xA4gh",         ec_UtfOK);
  TestConv32_8_nn (3, "gh\xE2\x82\xAC",     ec_UtfOK);
  TestConv32_8_nn (3, "g\xE2\x82\xACh",     ec_UtfOK);
  TestConv32_8_nn (3, "\xE2\x82\xACgh",     ec_UtfOK);
  TestConv32_8_nn (3, "gh\xF0\x9D\x84\x9E", ec_UtfOK);
  TestConv32_8_nn (3, "g\xF0\x9D\x84\x9Eh", ec_UtfOK);
  TestConv32_8_nn (3, "\xF0\x9D\x84\x9Egh", ec_UtfOK);
  printf ("\n");

  TestConv32_8_nn (3, "gh\xC3",         ec_UtfMbEnd);
  TestConv32_8_nn (3, "gh\xE2\x82",     ec_UtfMbEnd);
  TestConv32_8_nn (3, "gh\xF0\x9D\x84", ec_UtfMbEnd);
  printf ("\n");

  TestConv32_8_nn (3, "gh\x80", ec_UtfMbMissingStart);
  TestConv32_8_nn (3, "gh\xBF", ec_UtfMbMissingStart);
  TestConv32_8_nn (3, "gh\xC0", ec_UtfMbEnd);
  TestConv32_8_nn (3, "gh\xF7", ec_UtfMbEnd);
  TestConv32_8_nn (3, "gh\xF8", ec_UtfMbInvalidStart);
  TestConv32_8_nn (3, "gh\xFF", ec_UtfMbInvalidStart);
  printf ("\n");

  TestConv32_8_nn (3, "gh\xC3\x7F",         ec_UtfMbExpected);
  TestConv32_8_nn (3, "gh\xC3\x80",         ec_UtfOK);
  TestConv32_8_nn (3, "gh\xC3\xBF",         ec_UtfOK);
  TestConv32_8_nn (3, "gh\xC3\xC0",         ec_UtfMbExpected);
  TestConv32_8_nn (3, "gh\xE2\x82\x7F",     ec_UtfMbExpected);
  TestConv32_8_nn (3, "gh\xE2\x82\x80",     ec_UtfOK);
  TestConv32_8_nn (3, "gh\xE2\x82\xBF",     ec_UtfOK);
  TestConv32_8_nn (3, "gh\xE2\x82\xC0",     ec_UtfMbExpected);
  TestConv32_8_nn (3, "gh\xF0\x9D\x84\x7F", ec_UtfMbExpected);
  TestConv32_8_nn (3, "gh\xF0\x9D\x84\x80", ec_UtfOK);
  TestConv32_8_nn (3, "gh\xF0\x9D\x84\xBF", ec_UtfOK);
  TestConv32_8_nn (3, "gh\xF0\x9D\x84\xC0", ec_UtfMbExpected);
  printf ("\n");

  TestConv32_8_nn (3, "gh\xC0\x80",         ec_UtfWideRange);
  TestConv32_8_nn (3, "gh\xC1\xBF",         ec_UtfWideRange);
  TestConv32_8_nn (3, "gh\xC2\x80",         ec_UtfOK);
  TestConv32_8_nn (3, "gh\xDF\xBF",         ec_UtfOK);
  TestConv32_8_nn (3, "gh\xE0\x80\x80",     ec_UtfWideRange);
  TestConv32_8_nn (3, "gh\xE0\x9F\xBF",     ec_UtfWideRange);
  TestConv32_8_nn (3, "gh\xE0\xA0\x80",     ec_UtfOK);
  TestConv32_8_nn (3, "gh\xEF\xBF\xBF",     ec_UtfOK);
  TestConv32_8_nn (3, "gh\xF0\x80\x80\x80", ec_UtfWideRange);
  TestConv32_8_nn (3, "gh\xF0\x8F\xBF\xBF", ec_UtfWideRange);
  TestConv32_8_nn (3, "gh\xF0\x90\x80\x80", ec_UtfOK);
  TestConv32_8_nn (3, "gh\xF4\x8F\xBF\xBF", ec_UtfOK);
  TestConv32_8_nn (3, "gh\xF4\x90\x80\x80", ec_UtfWideRange);
  printf ("\n");

  TestConv32_8_nn (3, "gh\xED\x9F\xBF", ec_UtfOK);
  TestConv32_8_nn (3, "gh\xED\xA0\x80", ec_UtfSurrogate);
  TestConv32_8_nn (3, "gh\xED\xBF\xBF", ec_UtfSurrogate);
  TestConv32_8_nn (3, "gh\xEE\x80\x80", ec_UtfOK);
  printf ("\n");

  printf ("Count UTF-8 = UTF-32 --------------------------------------\n\n");

  {
  const t_UInt32 au [] = { 0 };
  TestCount ((t_UInt8 *) 0, 0, au, 0, true,  ec_UtfMissingNull, 0);
  TestCount ((t_UInt8 *) 0, 0, au, 0, false, ec_UtfOK,          0);
  }
  TestCount8_32 (1, 0,                   true,  ec_UtfOK,          0);
  TestCount8_32 (1, 0,                   false, ec_UtfOK,          0);
  TestCount8_32 (0, 'a' _ 'b' _ 'c',     true,  ec_UtfMissingNull, 2);
  TestCount8_32 (3, 'a' _ 'b' _ 'c',     false, ec_UtfOK,          0);
  TestCount8_32 (4, 'a' _ 'b' _ 'c' _ 0, true,  ec_UtfOK,          0);
  TestCount8_32 (4, 'a' _ 'b' _ 'c' _ 0, false, ec_UtfOK,          0);
  TestCount8_32 (0, 'a' _  0  _ 'c' _ 0, true,  ec_UtfNullInside,  1);
  TestCount8_32 (4, 'a' _  0  _ 'c' _ 0, false, ec_UtfOK,          0);
  TestCount8_32 (0, 'a' _ 'b' _  0  _ 0, true,  ec_UtfNullInside,  2);
  TestCount8_32 (4, 'a' _ 'b' _  0  _ 0, false, ec_UtfOK,          0);
  printf ("\n");

  TestCount8_32_nn (3, 'g' _ 'h' _ 'i',     ec_UtfOK, 0);
  TestCount8_32_nn (4, 'g' _ 'h' _ 0xE4,    ec_UtfOK, 0);
  TestCount8_32_nn (4, 'g' _ 0xE4 _ 'h',    ec_UtfOK, 0);
  TestCount8_32_nn (4, 0xE4 _ 'g' _ 'h',    ec_UtfOK, 0);
  TestCount8_32_nn (5, 'g' _ 'h' _ 0x20AC,  ec_UtfOK, 0);
  TestCount8_32_nn (5, 'g' _ 0x20AC _ 'h',  ec_UtfOK, 0);
  TestCount8_32_nn (5, 0x20AC _ 'g' _ 'h',  ec_UtfOK, 0);
  TestCount8_32_nn (6, 'g' _ 'h' _ 0x1D11E, ec_UtfOK, 0);
  TestCount8_32_nn (6, 'g' _ 0x1D11E _ 'h', ec_UtfOK, 0);
  TestCount8_32_nn (6, 0x1D11E _ 'g' _ 'h', ec_UtfOK, 0);
  printf ("\n");

  TestCount8_32_nn (3, 'g' _ 'h' _ 0x0,      ec_UtfOK, 0);
  TestCount8_32_nn (3, 'g' _ 'h' _ 0x7F,     ec_UtfOK, 0);
  TestCount8_32_nn (4, 'g' _ 'h' _ 0x80,     ec_UtfOK, 0);
  TestCount8_32_nn (4, 'g' _ 'h' _ 0x7FF,    ec_UtfOK, 0);
  TestCount8_32_nn (5, 'g' _ 'h' _ 0x800,    ec_UtfOK, 0);
  TestCount8_32_nn (5, 'g' _ 'h' _ 0xFFFF,   ec_UtfOK, 0);
  TestCount8_32_nn (6, 'g' _ 'h' _ 0x10000,  ec_UtfOK, 0);
  TestCount8_32_nn (6, 'g' _ 'h' _ 0x10FFFF, ec_UtfOK, 0);
  TestCount8_32_nn (0, 'g' _ 'h' _ 0x110000, ec_UtfWideRange, 2);
  printf ("\n");

  TestCount8_32_nn (5, 'g' _ 'h' _ 0xD7FF, ec_UtfOK, 0);
  TestCount8_32_nn (0, 'g' _ 'h' _ 0xD800, ec_UtfSurrogate, 2);
  TestCount8_32_nn (0, 'g' _ 'h' _ 0xDFFF, ec_UtfSurrogate, 2);
  TestCount8_32_nn (5, 'g' _ 'h' _ 0xE000, ec_UtfOK, 0);
  printf ("\n");

  printf ("Conv UTF-8 = UTF-32 --------------------------------------\n\n");

  {
  const t_UInt32 au [] = { 0 };
  TestConv (au_Dest8, 1, au, 0, false, ec_UtfDestTooLarge, au_Dest32);
  }
  TestConv8_32 (0, 0,                   true,  ec_UtfDestTooSmall);
  TestConv8_32 (0, 0,                   false, ec_UtfDestTooSmall);
  TestConv8_32 (2, 0,                   true,  ec_UtfDestTooLarge);
  TestConv8_32 (2, 0,                   false, ec_UtfDestTooLarge);
  TestConv8_32 (3, 'a' _ 'b' _ 'c' _ 0, true,  ec_UtfDestTooSmall);
  TestConv8_32 (3, 'a' _ 'b' _ 'c' _ 0, false, ec_UtfDestTooSmall);
  TestConv8_32 (5, 'a' _ 'b' _ 'c' _ 0, true,  ec_UtfDestTooLarge);
  TestConv8_32 (5, 'a' _ 'b' _ 'c' _ 0, false, ec_UtfDestTooLarge);
  printf ("\n");

  TestConv8_32_nn (3, 'g' _ 'h' _ 0xE4,    ec_UtfDestTooSmall);
  TestConv8_32_nn (5, 'g' _ 'h' _ 0xE4,    ec_UtfDestTooLarge);
  TestConv8_32_nn (4, 'g' _ 'h' _ 0x20AC,  ec_UtfDestTooSmall);
  TestConv8_32_nn (6, 'g' _ 'h' _ 0x20AC,  ec_UtfDestTooLarge);
  TestConv8_32_nn (5, 'g' _ 'h' _ 0x1D11E, ec_UtfDestTooSmall);
  TestConv8_32_nn (7, 'g' _ 'h' _ 0x1D11E, ec_UtfDestTooLarge);
  printf ("\n");

  {
  const t_UInt32 au [] = { 0 };
  TestConv (au_Dest8, 0, au, 0, true,  ec_UtfMissingNull, au_Dest32);
  TestConv (au_Dest8, 0, au, 0, false, ec_UtfOK, au_Dest32);
  }
  TestConv8_32 (1, 0,                   true,  ec_UtfOK);
  TestConv8_32 (1, 0,                   false, ec_UtfOK);
  TestConv8_32 (3, 'a' _ 'b' _ 'c',     true,  ec_UtfMissingNull);
  TestConv8_32 (3, 'a' _ 'b' _ 'c',     false, ec_UtfOK);
  TestConv8_32 (4, 'a' _ 'b' _ 'c' _ 0, true,  ec_UtfOK);
  TestConv8_32 (4, 'a' _ 'b' _ 'c' _ 0, false, ec_UtfOK);
  TestConv8_32 (4, 'a' _  0  _ 'c' _ 0, true,  ec_UtfNullInside);
  TestConv8_32 (4, 'a' _  0  _ 'c' _ 0, false, ec_UtfOK);
  TestConv8_32 (4, 'a' _ 'b' _  0  _ 0, true,  ec_UtfNullInside);
  TestConv8_32 (4, 'a' _ 'b' _  0  _ 0, false, ec_UtfOK);
  printf ("\n");

  TestConv8_32_nn (3, 'g' _ 'h' _ 'i',     ec_UtfOK);
  TestConv8_32_nn (4, 'g' _ 'h' _ 0xE4,    ec_UtfOK);
  TestConv8_32_nn (4, 'g' _ 0xE4 _ 'h',    ec_UtfOK);
  TestConv8_32_nn (4, 0xE4 _ 'g' _ 'h',    ec_UtfOK);
  TestConv8_32_nn (5, 'g' _ 'h' _ 0x20AC,  ec_UtfOK);
  TestConv8_32_nn (5, 'g' _ 0x20AC _ 'h',  ec_UtfOK);
  TestConv8_32_nn (5, 0x20AC _ 'g' _ 'h',  ec_UtfOK);
  TestConv8_32_nn (6, 'g' _ 'h' _ 0x1D11E, ec_UtfOK);
  TestConv8_32_nn (6, 'g' _ 0x1D11E _ 'h', ec_UtfOK);
  TestConv8_32_nn (6, 0x1D11E _ 'g' _ 'h', ec_UtfOK);
  printf ("\n");

  TestConv8_32_nn (3, 'g' _ 'h' _ 0x0,      ec_UtfOK);
  TestConv8_32_nn (3, 'g' _ 'h' _ 0x7F,     ec_UtfOK);
  TestConv8_32_nn (4, 'g' _ 'h' _ 0x80,     ec_UtfOK);
  TestConv8_32_nn (4, 'g' _ 'h' _ 0x7FF,    ec_UtfOK);
  TestConv8_32_nn (5, 'g' _ 'h' _ 0x800,    ec_UtfOK);
  TestConv8_32_nn (5, 'g' _ 'h' _ 0xFFFF,   ec_UtfOK);
  TestConv8_32_nn (6, 'g' _ 'h' _ 0x10000,  ec_UtfOK);
  TestConv8_32_nn (6, 'g' _ 'h' _ 0x10FFFF, ec_UtfOK);
  TestConv8_32_nn (6, 'g' _ 'h' _ 0x110000, ec_UtfWideRange);
  printf ("\n");

  TestConv8_32_nn (5, 'g' _ 'h' _ 0xD7FF, ec_UtfOK);
  TestConv8_32_nn (5, 'g' _ 'h' _ 0xD800, ec_UtfSurrogate);
  TestConv8_32_nn (5, 'g' _ 'h' _ 0xDFFF, ec_UtfSurrogate);
  TestConv8_32_nn (5, 'g' _ 'h' _ 0xE000, ec_UtfOK);
  printf ("\n");

  printf ("Count UTF-32 = UTF-16 --------------------------------------\n\n");

  {
  const t_UInt16 au [] = { 0 };
  TestCount ((t_UInt32 *) 0, 0, au, 0, true,  ec_UtfMissingNull, 0);
  TestCount ((t_UInt32 *) 0, 0, au, 0, false, ec_UtfOK,          0);
  }
  TestCount32_16 (1, 0,                   true,  ec_UtfOK,          0);
  TestCount32_16 (1, 0,                   false, ec_UtfOK,          0);
  TestCount32_16 (0, 'a' _ 'b' _ 'c',     true,  ec_UtfMissingNull, 2);
  TestCount32_16 (3, 'a' _ 'b' _ 'c',     false, ec_UtfOK,          0);
  TestCount32_16 (4, 'a' _ 'b' _ 'c' _ 0, true,  ec_UtfOK,          0);
  TestCount32_16 (4, 'a' _ 'b' _ 'c' _ 0, false, ec_UtfOK,          0);
  TestCount32_16 (0, 'a' _  0  _ 'c' _ 0, true,  ec_UtfNullInside,  1);
  TestCount32_16 (4, 'a' _  0  _ 'c' _ 0, false, ec_UtfOK,          0);
  TestCount32_16 (0, 'a' _ 'b' _  0  _ 0, true,  ec_UtfNullInside,  2);
  TestCount32_16 (4, 'a' _ 'b' _  0  _ 0, false, ec_UtfOK,          0);
  printf ("\n");

  TestCount32_16_nn (3, 'g' _ 'h' _ 'i',             ec_UtfOK, 0);
  TestCount32_16_nn (3, 'g' _ 'h' _ 0xD834 _ 0xDD1E, ec_UtfOK, 0);
  TestCount32_16_nn (3, 'g' _ 0xD834 _ 0xDD1E _ 'h', ec_UtfOK, 0);
  TestCount32_16_nn (3, 0xD834 _ 0xDD1E _ 'g' _ 'h', ec_UtfOK, 0);
  printf ("\n");

  TestCount32_16_nn (4, 'g' _ 'h' _ 0xD7FF _ 0,      ec_UtfOK, 0);
  TestCount32_16_nn (3, 'g' _ 'h' _ 0xD800 _ 0xDC00, ec_UtfOK, 0);
  TestCount32_16_nn (3, 'g' _ 'h' _ 0xDBFF _ 0xDC00, ec_UtfOK, 0);
  TestCount32_16_nn (0, 'g' _ 'h' _ 0xDC00 _ 0xDC00, ec_UtfHighSurrExpected, 2);
  TestCount32_16_nn (0, 'g' _ 'h' _ 0xDFFF _ 0xDC00, ec_UtfHighSurrExpected, 2);
  TestCount32_16_nn (4, 'g' _ 'h' _ 0xE000 _ 0,      ec_UtfOK, 0);
  TestCount32_16_nn (0, 'g' _ 'h' _ 0xD800 _ 0,      ec_UtfLowSurrExpected, 3);
  TestCount32_16_nn (0, 'g' _ 'h' _ 0xD800 _ 0xDBFF, ec_UtfLowSurrExpected, 3);
  TestCount32_16_nn (3, 'g' _ 'h' _ 0xD800 _ 0xDC00, ec_UtfOK, 0);
  TestCount32_16_nn (3, 'g' _ 'h' _ 0xD800 _ 0xDFFF, ec_UtfOK, 0);
  TestCount32_16_nn (3, 'g' _ 'h' _ 0xDBFF _ 0xDFFF, ec_UtfOK, 0);
  TestCount32_16_nn (0, 'g' _ 'h' _ 0xD800 _ 0xE000, ec_UtfLowSurrExpected, 3);
  TestCount32_16_nn (0, 'g' _ 'h' _ 0xD800,          ec_UtfSurrEnd, 2);
  printf ("\n");

  printf ("Conv UTF-32 = UTF-16 --------------------------------------\n\n");

  {
  const t_UInt16 au [] = { 0 };
  TestConv (au_Dest32, 1, au, 0, false, ec_UtfDestTooLarge, au_Dest16);
  }
  TestConv32_16 (0, 0,                   true,  ec_UtfDestTooSmall);
  TestConv32_16 (0, 0,                   false, ec_UtfDestTooSmall);
  TestConv32_16 (2, 0,                   true,  ec_UtfDestTooLarge);
  TestConv32_16 (2, 0,                   false, ec_UtfDestTooLarge);
  TestConv32_16 (3, 'a' _ 'b' _ 'c' _ 0, true,  ec_UtfDestTooSmall);
  TestConv32_16 (3, 'a' _ 'b' _ 'c' _ 0, false, ec_UtfDestTooSmall);
  TestConv32_16 (5, 'a' _ 'b' _ 'c' _ 0, true,  ec_UtfDestTooLarge);
  TestConv32_16 (5, 'a' _ 'b' _ 'c' _ 0, false, ec_UtfDestTooLarge);
  printf ("\n");

  {
  const t_UInt16 au [] = { 0 };
  TestConv ((t_UInt32 *) 0, 0, au, 0, true,  ec_UtfMissingNull, au_Dest16);
  TestConv ((t_UInt32 *) 0, 0, au, 0, false, ec_UtfOK, au_Dest16);
  }
  TestConv32_16 (1, 0,                   true,  ec_UtfOK);
  TestConv32_16 (1, 0,                   false, ec_UtfOK);
  TestConv32_16 (3, 'a' _ 'b' _ 'c',     true,  ec_UtfMissingNull);
  TestConv32_16 (3, 'a' _ 'b' _ 'c',     false, ec_UtfOK);
  TestConv32_16 (4, 'a' _ 'b' _ 'c' _ 0, true,  ec_UtfOK);
  TestConv32_16 (4, 'a' _ 'b' _ 'c' _ 0, false, ec_UtfOK);
  TestConv32_16 (4, 'a' _  0  _ 'c' _ 0, true,  ec_UtfNullInside);
  TestConv32_16 (4, 'a' _  0  _ 'c' _ 0, false, ec_UtfOK);
  TestConv32_16 (4, 'a' _ 'b' _  0  _ 0, true,  ec_UtfNullInside);
  TestConv32_16 (4, 'a' _ 'b' _  0  _ 0, false, ec_UtfOK);
  printf ("\n");

  TestConv32_16_nn (3, 'g' _ 'h' _ 'i',             ec_UtfOK);
  TestConv32_16_nn (3, 'g' _ 'h' _ 0xD834 _ 0xDD1E, ec_UtfOK);
  TestConv32_16_nn (3, 'g' _ 0xD834 _ 0xDD1E _ 'h', ec_UtfOK);
  TestConv32_16_nn (3, 0xD834 _ 0xDD1E _ 'g' _ 'h', ec_UtfOK);
  printf ("\n");

  TestConv32_16_nn (4, 'g' _ 'h' _ 0xD7FF _ 0,      ec_UtfOK);
  TestConv32_16_nn (3, 'g' _ 'h' _ 0xD800 _ 0xDC00, ec_UtfOK);
  TestConv32_16_nn (3, 'g' _ 'h' _ 0xDBFF _ 0xDC00, ec_UtfOK);
  TestConv32_16_nn (3, 'g' _ 'h' _ 0xDC00 _ 0xDC00, ec_UtfHighSurrExpected);
  TestConv32_16_nn (3, 'g' _ 'h' _ 0xDFFF _ 0xDC00, ec_UtfHighSurrExpected);
  TestConv32_16_nn (4, 'g' _ 'h' _ 0xE000 _ 0,      ec_UtfOK);
  TestConv32_16_nn (3, 'g' _ 'h' _ 0xD800 _ 0,      ec_UtfLowSurrExpected);
  TestConv32_16_nn (3, 'g' _ 'h' _ 0xD800 _ 0xDBFF, ec_UtfLowSurrExpected);
  TestConv32_16_nn (3, 'g' _ 'h' _ 0xD800 _ 0xDC00, ec_UtfOK);
  TestConv32_16_nn (3, 'g' _ 'h' _ 0xD800 _ 0xDFFF, ec_UtfOK);
  TestConv32_16_nn (3, 'g' _ 'h' _ 0xDBFF _ 0xDFFF, ec_UtfOK);
  TestConv32_16_nn (3, 'g' _ 'h' _ 0xD800 _ 0xE000, ec_UtfLowSurrExpected);
  TestConv32_16_nn (3, 'g' _ 'h' _ 0xD800,          ec_UtfSurrEnd);
  printf ("\n");

  printf ("Count UTF-16 = UTF-32 --------------------------------------\n\n");

  {
  const t_UInt32 au [] = { 0 };
  TestCount ((t_UInt16 *) 0, 0, au, 0, true,  ec_UtfMissingNull, 0);
  TestCount ((t_UInt16 *) 0, 0, au, 0, false, ec_UtfOK,          0);
  }
  TestCount16_32 (1, 0,                   true,  ec_UtfOK,          0);
  TestCount16_32 (1, 0,                   false, ec_UtfOK,          0);
  TestCount16_32 (0, 'a' _ 'b' _ 'c',     true,  ec_UtfMissingNull, 2);
  TestCount16_32 (3, 'a' _ 'b' _ 'c',     false, ec_UtfOK,          0);
  TestCount16_32 (4, 'a' _ 'b' _ 'c' _ 0, true,  ec_UtfOK,          0);
  TestCount16_32 (4, 'a' _ 'b' _ 'c' _ 0, false, ec_UtfOK,          0);
  TestCount16_32 (0, 'a' _  0  _ 'c' _ 0, true,  ec_UtfNullInside,  1);
  TestCount16_32 (4, 'a' _  0  _ 'c' _ 0, false, ec_UtfOK,          0);
  TestCount16_32 (0, 'a' _ 'b' _  0  _ 0, true,  ec_UtfNullInside,  2);
  TestCount16_32 (4, 'a' _ 'b' _  0  _ 0, false, ec_UtfOK,          0);
  printf ("\n");

  TestCount16_32_nn (3, 'g' _ 'h' _ 'i',     ec_UtfOK, 0);
  TestCount16_32_nn (3, 'g' _ 'h' _ 0x20AC,  ec_UtfOK, 0);
  TestCount16_32_nn (3, 'g' _ 0x20AC _ 'h',  ec_UtfOK, 0);
  TestCount16_32_nn (3, 0x20AC _ 'g' _ 'h',  ec_UtfOK, 0);
  TestCount16_32_nn (4, 'g' _ 'h' _ 0x1D11E, ec_UtfOK, 0);
  TestCount16_32_nn (4, 'g' _ 0x1D11E _ 'h', ec_UtfOK, 0);
  TestCount16_32_nn (4, 0x1D11E _ 'g' _ 'h', ec_UtfOK, 0);
  printf ("\n");

  TestCount16_32_nn (3, 'g' _ 'h' _ 0x0,      ec_UtfOK, 0);
  TestCount16_32_nn (3, 'g' _ 'h' _ 0xFFFF,   ec_UtfOK, 0);
  TestCount16_32_nn (4, 'g' _ 'h' _ 0x10000,  ec_UtfOK, 0);
  TestCount16_32_nn (4, 'g' _ 'h' _ 0x10FFFF, ec_UtfOK, 0);
  TestCount16_32_nn (0, 'g' _ 'h' _ 0x110000, ec_UtfWideRange, 2);
  printf ("\n");

  TestCount16_32_nn (3, 'g' _ 'h' _ 0xD7FF, ec_UtfOK, 0);
  TestCount16_32_nn (0, 'g' _ 'h' _ 0xD800, ec_UtfSurrogate, 2);
  TestCount16_32_nn (0, 'g' _ 'h' _ 0xDFFF, ec_UtfSurrogate, 2);
  TestCount16_32_nn (3, 'g' _ 'h' _ 0xE000, ec_UtfOK, 0);
  printf ("\n");

  printf ("Conv UTF-16 = UTF-32 --------------------------------------\n\n");

  {
  const t_UInt32 au [] = { 0 };
  TestConv (au_Dest16, 1, au, 0, false, ec_UtfDestTooLarge, au_Dest32);
  }
  TestConv16_32 (0, 0,                   true,  ec_UtfDestTooSmall);
  TestConv16_32 (0, 0,                   false, ec_UtfDestTooSmall);
  TestConv16_32 (2, 0,                   true,  ec_UtfDestTooLarge);
  TestConv16_32 (2, 0,                   false, ec_UtfDestTooLarge);
  TestConv16_32 (3, 'a' _ 'b' _ 'c' _ 0, true,  ec_UtfDestTooSmall);
  TestConv16_32 (3, 'a' _ 'b' _ 'c' _ 0, false, ec_UtfDestTooSmall);
  TestConv16_32 (5, 'a' _ 'b' _ 'c' _ 0, true,  ec_UtfDestTooLarge);
  TestConv16_32 (5, 'a' _ 'b' _ 'c' _ 0, false, ec_UtfDestTooLarge);
  printf ("\n");

  TestConv16_32_nn (3, 'g' _ 'h' _ 0x1D11E, ec_UtfDestTooSmall);
  TestConv16_32_nn (4, 'g' _ 'h' _ 0x1D11E, ec_UtfOK);
  TestConv16_32_nn (5, 'g' _ 'h' _ 0x1D11E, ec_UtfDestTooLarge);
  printf ("\n");

  {
  const t_UInt32 au [] = { 0 };
  TestConv (au_Dest16, 0, au, 0, true,  ec_UtfMissingNull, au_Dest32);
  TestConv (au_Dest16, 0, au, 0, false, ec_UtfOK, au_Dest32);
  }
  TestConv16_32 (1, 0,                   true,  ec_UtfOK);
  TestConv16_32 (1, 0,                   false, ec_UtfOK);
  TestConv16_32 (3, 'a' _ 'b' _ 'c',     true,  ec_UtfMissingNull);
  TestConv16_32 (3, 'a' _ 'b' _ 'c',     false, ec_UtfOK);
  TestConv16_32 (4, 'a' _ 'b' _ 'c' _ 0, true,  ec_UtfOK);
  TestConv16_32 (4, 'a' _ 'b' _ 'c' _ 0, false, ec_UtfOK);
  TestConv16_32 (4, 'a' _  0  _ 'c' _ 0, true,  ec_UtfNullInside);
  TestConv16_32 (4, 'a' _  0  _ 'c' _ 0, false, ec_UtfOK);
  TestConv16_32 (4, 'a' _ 'b' _  0  _ 0, true,  ec_UtfNullInside);
  TestConv16_32 (4, 'a' _ 'b' _  0  _ 0, false, ec_UtfOK);
  printf ("\n");

  TestConv16_32_nn (3, 'g' _ 'h' _ 'i',     ec_UtfOK);
  TestConv16_32_nn (3, 'g' _ 'h' _ 0x20AC,  ec_UtfOK);
  TestConv16_32_nn (3, 'g' _ 0x20AC _ 'h',  ec_UtfOK);
  TestConv16_32_nn (3, 0x20AC _ 'g' _ 'h',  ec_UtfOK);
  TestConv16_32_nn (4, 'g' _ 'h' _ 0x1D11E, ec_UtfOK);
  TestConv16_32_nn (4, 'g' _ 0x1D11E _ 'h', ec_UtfOK);
  TestConv16_32_nn (4, 0x1D11E _ 'g' _ 'h', ec_UtfOK);
  printf ("\n");

  TestConv16_32_nn (3, 'g' _ 'h' _ 0x0,      ec_UtfOK);
  TestConv16_32_nn (3, 'g' _ 'h' _ 0xFFFF,   ec_UtfOK);
  TestConv16_32_nn (4, 'g' _ 'h' _ 0x10000,  ec_UtfOK);
  TestConv16_32_nn (4, 'g' _ 'h' _ 0x10FFFF, ec_UtfOK);
  TestConv16_32_nn (4, 'g' _ 'h' _ 0x110000, ec_UtfWideRange);
  printf ("\n");

  TestConv16_32_nn (3, 'g' _ 'h' _ 0xD7FF, ec_UtfOK);
  TestConv16_32_nn (3, 'g' _ 'h' _ 0xD800, ec_UtfSurrogate);
  TestConv16_32_nn (3, 'g' _ 'h' _ 0xDFFF, ec_UtfSurrogate);
  TestConv16_32_nn (3, 'g' _ 'h' _ 0xE000, ec_UtfOK);
  printf ("\n");

  printf ("Count UTF-16 = UTF-8 --------------------------------------\n\n");

  TestCount16_8 (0, "",     0, true,  ec_UtfMissingNull, 0);
  TestCount16_8 (0, "",     0, false, ec_UtfOK,          0);
  TestCount16_8 (1, "",     1, true,  ec_UtfOK,          0);
  TestCount16_8 (1, "",     1, false, ec_UtfOK,          0);
  TestCount16_8 (0, "abc",  3, true,  ec_UtfMissingNull, 2);
  TestCount16_8 (3, "abc",  3, false, ec_UtfOK,          0);
  TestCount16_8 (4, "abc",  4, true,  ec_UtfOK,          0);
  TestCount16_8 (4, "abc",  4, false, ec_UtfOK,          0);
  TestCount16_8 (0, "a\0c", 4, true,  ec_UtfNullInside,  1);
  TestCount16_8 (4, "a\0c", 4, false, ec_UtfOK,          0);
  TestCount16_8 (0, "ab\0", 4, true,  ec_UtfNullInside,  2);
  TestCount16_8 (4, "ab\0", 4, false, ec_UtfOK,          0);
  printf ("\n");

  TestCount16_8_nn (3, "ghi",                ec_UtfOK, 0);
  TestCount16_8_nn (3, "gh\xC3\xA4",         ec_UtfOK, 0);
  TestCount16_8_nn (3, "g\xC3\xA4h",         ec_UtfOK, 0);
  TestCount16_8_nn (3, "\xC3\xA4gh",         ec_UtfOK, 0);
  TestCount16_8_nn (3, "gh\xE2\x82\xAC",     ec_UtfOK, 0);
  TestCount16_8_nn (3, "g\xE2\x82\xACh",     ec_UtfOK, 0);
  TestCount16_8_nn (3, "\xE2\x82\xACgh",     ec_UtfOK, 0);
  TestCount16_8_nn (4, "gh\xF0\x9D\x84\x9E", ec_UtfOK, 0);
  TestCount16_8_nn (4, "g\xF0\x9D\x84\x9Eh", ec_UtfOK, 0);
  TestCount16_8_nn (4, "\xF0\x9D\x84\x9Egh", ec_UtfOK, 0);
  printf ("\n");

  TestCount16_8_nn (0, "gh\xC3",         ec_UtfMbEnd, 2);
  TestCount16_8_nn (0, "gh\xE2\x82",     ec_UtfMbEnd, 2);
  TestCount16_8_nn (0, "gh\xF0\x9D\x84", ec_UtfMbEnd, 2);
  printf ("\n");

  TestCount16_8_nn (0, "gh\x80", ec_UtfMbMissingStart, 2);
  TestCount16_8_nn (0, "gh\xBF", ec_UtfMbMissingStart, 2);
  TestCount16_8_nn (0, "gh\xC0", ec_UtfMbEnd, 2);
  TestCount16_8_nn (0, "gh\xF7", ec_UtfMbEnd, 2);
  TestCount16_8_nn (0, "gh\xF8", ec_UtfMbInvalidStart, 2);
  TestCount16_8_nn (0, "gh\xFF", ec_UtfMbInvalidStart, 2);
  printf ("\n");

  TestCount16_8_nn (0, "gh\xC3\x7F",         ec_UtfMbExpected, 3);
  TestCount16_8_nn (3, "gh\xC3\x80",         ec_UtfOK, 0);
  TestCount16_8_nn (3, "gh\xC3\xBF",         ec_UtfOK, 0);
  TestCount16_8_nn (0, "gh\xC3\xC0",         ec_UtfMbExpected, 3);
  TestCount16_8_nn (0, "gh\xE2\x82\x7F",     ec_UtfMbExpected, 4);
  TestCount16_8_nn (3, "gh\xE2\x82\x80",     ec_UtfOK, 0);
  TestCount16_8_nn (3, "gh\xE2\x82\xBF",     ec_UtfOK, 0);
  TestCount16_8_nn (0, "gh\xE2\x82\xC0",     ec_UtfMbExpected, 4);
  TestCount16_8_nn (0, "gh\xF0\x9D\x84\x7F", ec_UtfMbExpected, 5);
  TestCount16_8_nn (4, "gh\xF0\x9D\x84\x80", ec_UtfOK, 0);
  TestCount16_8_nn (4, "gh\xF0\x9D\x84\xBF", ec_UtfOK, 0);
  TestCount16_8_nn (0, "gh\xF0\x9D\x84\xC0", ec_UtfMbExpected, 5);
  printf ("\n");

  TestCount16_8_nn (0, "gh\xC0\x80",         ec_UtfWideRange, 3);
  TestCount16_8_nn (0, "gh\xC1\xBF",         ec_UtfWideRange, 3);
  TestCount16_8_nn (3, "gh\xC2\x80",         ec_UtfOK, 0);
  TestCount16_8_nn (3, "gh\xDF\xBF",         ec_UtfOK, 0);
  TestCount16_8_nn (0, "gh\xE0\x80\x80",     ec_UtfWideRange, 4);
  TestCount16_8_nn (0, "gh\xE0\x9F\xBF",     ec_UtfWideRange, 4);
  TestCount16_8_nn (3, "gh\xE0\xA0\x80",     ec_UtfOK, 0);
  TestCount16_8_nn (3, "gh\xEF\xBF\xBF",     ec_UtfOK, 0);
  TestCount16_8_nn (0, "gh\xF0\x80\x80\x80", ec_UtfWideRange, 5);
  TestCount16_8_nn (0, "gh\xF0\x8F\xBF\xBF", ec_UtfWideRange, 5);
  TestCount16_8_nn (4, "gh\xF0\x90\x80\x80", ec_UtfOK, 0);
  TestCount16_8_nn (4, "gh\xF4\x8F\xBF\xBF", ec_UtfOK, 0);
  TestCount16_8_nn (0, "gh\xF4\x90\x80\x80", ec_UtfWideRange, 5);
  printf ("\n");

  TestCount16_8_nn (3, "gh\xED\x9F\xBF", ec_UtfOK, 0);
  TestCount16_8_nn (0, "gh\xED\xA0\x80", ec_UtfSurrogate, 4);
  TestCount16_8_nn (0, "gh\xED\xBF\xBF", ec_UtfSurrogate, 4);
  TestCount16_8_nn (3, "gh\xEE\x80\x80", ec_UtfOK, 0);
  printf ("\n");

  printf ("Conv UTF-16 = UTF-8 --------------------------------------\n\n");

  TestConv16_8 (1, "",     0, false, ec_UtfDestTooLarge);
  TestConv16_8 (0, "",     1, true,  ec_UtfDestTooSmall);
  TestConv16_8 (0, "",     1, false, ec_UtfDestTooSmall);
  TestConv16_8 (2, "",     1, true,  ec_UtfDestTooLarge);
  TestConv16_8 (2, "",     1, false, ec_UtfDestTooLarge);
  TestConv16_8 (3, "abc",  4, true,  ec_UtfDestTooSmall);
  TestConv16_8 (3, "abc",  4, false, ec_UtfDestTooSmall);
  TestConv16_8 (5, "abc",  4, true,  ec_UtfDestTooLarge);
  TestConv16_8 (5, "abc",  4, false, ec_UtfDestTooLarge);
  printf ("\n");

  TestConv16_8_nn (3, "gh\xF0\x9D\x84\x9E", ec_UtfDestTooSmall);
  TestConv16_8_nn (4, "gh\xF0\x9D\x84\x9E", ec_UtfOK);
  TestConv16_8_nn (5, "gh\xF0\x9D\x84\x9E", ec_UtfDestTooLarge);
  printf ("\n");

  TestConv16_8 (0, "",     0, true,  ec_UtfMissingNull);
  TestConv16_8 (0, "",     0, false, ec_UtfOK);
  TestConv16_8 (1, "",     1, true,  ec_UtfOK);
  TestConv16_8 (1, "",     1, false, ec_UtfOK);
  TestConv16_8 (3, "abc",  3, true,  ec_UtfMissingNull);
  TestConv16_8 (3, "abc",  3, false, ec_UtfOK);
  TestConv16_8 (4, "abc",  4, true,  ec_UtfOK);
  TestConv16_8 (4, "abc",  4, false, ec_UtfOK);
  TestConv16_8 (4, "a\0c", 4, true,  ec_UtfNullInside);
  TestConv16_8 (4, "a\0c", 4, false, ec_UtfOK);
  TestConv16_8 (4, "ab\0", 4, true,  ec_UtfNullInside);
  TestConv16_8 (4, "ab\0", 4, false, ec_UtfOK);
  printf ("\n");

  TestConv16_8_nn (3, "ghi",                ec_UtfOK);
  TestConv16_8_nn (3, "gh\xC3\xA4",         ec_UtfOK);
  TestConv16_8_nn (3, "g\xC3\xA4h",         ec_UtfOK);
  TestConv16_8_nn (3, "\xC3\xA4gh",         ec_UtfOK);
  TestConv16_8_nn (3, "gh\xE2\x82\xAC",     ec_UtfOK);
  TestConv16_8_nn (3, "g\xE2\x82\xACh",     ec_UtfOK);
  TestConv16_8_nn (3, "\xE2\x82\xACgh",     ec_UtfOK);
  TestConv16_8_nn (4, "gh\xF0\x9D\x84\x9E", ec_UtfOK);
  TestConv16_8_nn (4, "g\xF0\x9D\x84\x9Eh", ec_UtfOK);
  TestConv16_8_nn (4, "\xF0\x9D\x84\x9Egh", ec_UtfOK);
  printf ("\n");

  TestConv16_8_nn (3, "gh\xC3",         ec_UtfMbEnd);
  TestConv16_8_nn (3, "gh\xE2\x82",     ec_UtfMbEnd);
  TestConv16_8_nn (3, "gh\xF0\x9D\x84", ec_UtfMbEnd);
  printf ("\n");

  TestConv16_8_nn (3, "gh\x80", ec_UtfMbMissingStart);
  TestConv16_8_nn (3, "gh\xBF", ec_UtfMbMissingStart);
  TestConv16_8_nn (3, "gh\xC0", ec_UtfMbEnd);
  TestConv16_8_nn (3, "gh\xF7", ec_UtfMbEnd);
  TestConv16_8_nn (3, "gh\xF8", ec_UtfMbInvalidStart);
  TestConv16_8_nn (3, "gh\xFF", ec_UtfMbInvalidStart);
  printf ("\n");

  TestConv16_8_nn (3, "gh\xC3\x7F",         ec_UtfMbExpected);
  TestConv16_8_nn (3, "gh\xC3\x80",         ec_UtfOK);
  TestConv16_8_nn (3, "gh\xC3\xBF",         ec_UtfOK);
  TestConv16_8_nn (3, "gh\xC3\xC0",         ec_UtfMbExpected);
  TestConv16_8_nn (3, "gh\xE2\x82\x7F",     ec_UtfMbExpected);
  TestConv16_8_nn (3, "gh\xE2\x82\x80",     ec_UtfOK);
  TestConv16_8_nn (3, "gh\xE2\x82\xBF",     ec_UtfOK);
  TestConv16_8_nn (3, "gh\xE2\x82\xC0",     ec_UtfMbExpected);
  TestConv16_8_nn (4, "gh\xF0\x9D\x84\x7F", ec_UtfMbExpected);
  TestConv16_8_nn (4, "gh\xF0\x9D\x84\x80", ec_UtfOK);
  TestConv16_8_nn (4, "gh\xF0\x9D\x84\xBF", ec_UtfOK);
  TestConv16_8_nn (4, "gh\xF0\x9D\x84\xC0", ec_UtfMbExpected);
  printf ("\n");

  TestConv16_8_nn (3, "gh\xC0\x80",         ec_UtfWideRange);
  TestConv16_8_nn (3, "gh\xC1\xBF",         ec_UtfWideRange);
  TestConv16_8_nn (3, "gh\xC2\x80",         ec_UtfOK);
  TestConv16_8_nn (3, "gh\xDF\xBF",         ec_UtfOK);
  TestConv16_8_nn (3, "gh\xE0\x80\x80",     ec_UtfWideRange);
  TestConv16_8_nn (3, "gh\xE0\x9F\xBF",     ec_UtfWideRange);
  TestConv16_8_nn (3, "gh\xE0\xA0\x80",     ec_UtfOK);
  TestConv16_8_nn (3, "gh\xEF\xBF\xBF",     ec_UtfOK);
  TestConv16_8_nn (4, "gh\xF0\x80\x80\x80", ec_UtfWideRange);
  TestConv16_8_nn (4, "gh\xF0\x8F\xBF\xBF", ec_UtfWideRange);
  TestConv16_8_nn (4, "gh\xF0\x90\x80\x80", ec_UtfOK);
  TestConv16_8_nn (4, "gh\xF4\x8F\xBF\xBF", ec_UtfOK);
  TestConv16_8_nn (4, "gh\xF4\x90\x80\x80", ec_UtfWideRange);
  printf ("\n");

  TestConv16_8_nn (3, "gh\xED\x9F\xBF", ec_UtfOK);
  TestConv16_8_nn (3, "gh\xED\xA0\x80", ec_UtfSurrogate);
  TestConv16_8_nn (3, "gh\xED\xBF\xBF", ec_UtfSurrogate);
  TestConv16_8_nn (3, "gh\xEE\x80\x80", ec_UtfOK);
  printf ("\n");

  printf ("Count UTF-8 = UTF-16 --------------------------------------\n\n");

  {
  const t_UInt16 au [] = { 0 };
  TestCount ((t_UInt8 *) 0, 0, au, 0, true,  ec_UtfMissingNull, 0);
  TestCount ((t_UInt8 *) 0, 0, au, 0, false, ec_UtfOK,          0);
  }
  TestCount8_16 (1, 0,                   true,  ec_UtfOK,          0);
  TestCount8_16 (1, 0,                   false, ec_UtfOK,          0);
  TestCount8_16 (0, 'a' _ 'b' _ 'c',     true,  ec_UtfMissingNull, 2);
  TestCount8_16 (3, 'a' _ 'b' _ 'c',     false, ec_UtfOK,          0);
  TestCount8_16 (4, 'a' _ 'b' _ 'c' _ 0, true,  ec_UtfOK,          0);
  TestCount8_16 (4, 'a' _ 'b' _ 'c' _ 0, false, ec_UtfOK,          0);
  TestCount8_16 (0, 'a' _  0  _ 'c' _ 0, true,  ec_UtfNullInside,  1);
  TestCount8_16 (4, 'a' _  0  _ 'c' _ 0, false, ec_UtfOK,          0);
  TestCount8_16 (0, 'a' _ 'b' _  0  _ 0, true,  ec_UtfNullInside,  2);
  TestCount8_16 (4, 'a' _ 'b' _  0  _ 0, false, ec_UtfOK,          0);
  printf ("\n");

  TestCount8_16_nn (3, 'g' _ 'h' _ 'i',    ec_UtfOK, 0);
  TestCount8_16_nn (4, 'g' _ 'h' _ 0xE4,   ec_UtfOK, 0);
  TestCount8_16_nn (4, 'g' _ 0xE4 _ 'h',   ec_UtfOK, 0);
  TestCount8_16_nn (4, 0xE4 _ 'g' _ 'h',   ec_UtfOK, 0);
  TestCount8_16_nn (5, 'g' _ 'h' _ 0x20AC, ec_UtfOK, 0);
  TestCount8_16_nn (5, 'g' _ 0x20AC _ 'h', ec_UtfOK, 0);
  TestCount8_16_nn (5, 0x20AC _ 'g' _ 'h', ec_UtfOK, 0);
  TestCount8_16_nn (6, 'g' _ 'h' _ 0xD834 _ 0xDD1E, ec_UtfOK, 0);
  TestCount8_16_nn (6, 'g' _ 0xD834 _ 0xDD1E _ 'h', ec_UtfOK, 0);
  TestCount8_16_nn (6, 0xD834 _ 0xDD1E _ 'g' _ 'h', ec_UtfOK, 0);
  printf ("\n");

  TestCount8_16_nn (3, 'g' _ 'h' _ 0x0,    ec_UtfOK, 0);
  TestCount8_16_nn (3, 'g' _ 'h' _ 0x7F,   ec_UtfOK, 0);
  TestCount8_16_nn (4, 'g' _ 'h' _ 0x80,   ec_UtfOK, 0);
  TestCount8_16_nn (4, 'g' _ 'h' _ 0x7FF,  ec_UtfOK, 0);
  TestCount8_16_nn (5, 'g' _ 'h' _ 0x800,  ec_UtfOK, 0);
  TestCount8_16_nn (5, 'g' _ 'h' _ 0xFFFF, ec_UtfOK, 0);
  printf ("\n");

  TestCount8_16_nn (6, 'g' _ 'h' _ 0xD7FF _ 0,      ec_UtfOK, 0);
  TestCount8_16_nn (6, 'g' _ 'h' _ 0xD800 _ 0xDC00, ec_UtfOK, 0);
  TestCount8_16_nn (6, 'g' _ 'h' _ 0xDBFF _ 0xDC00, ec_UtfOK, 0);
  TestCount8_16_nn (0, 'g' _ 'h' _ 0xDC00 _ 0xDC00, ec_UtfHighSurrExpected, 2);
  TestCount8_16_nn (0, 'g' _ 'h' _ 0xDFFF _ 0xDC00, ec_UtfHighSurrExpected, 2);
  TestCount8_16_nn (6, 'g' _ 'h' _ 0xE000 _ 0,      ec_UtfOK, 0);
  TestCount8_16_nn (0, 'g' _ 'h' _ 0xD800 _ 0,      ec_UtfLowSurrExpected, 3);
  TestCount8_16_nn (0, 'g' _ 'h' _ 0xD800 _ 0xDBFF, ec_UtfLowSurrExpected, 3);
  TestCount8_16_nn (6, 'g' _ 'h' _ 0xD800 _ 0xDC00, ec_UtfOK, 0);
  TestCount8_16_nn (6, 'g' _ 'h' _ 0xD800 _ 0xDFFF, ec_UtfOK, 0);
  TestCount8_16_nn (6, 'g' _ 'h' _ 0xDBFF _ 0xDFFF, ec_UtfOK, 0);
  TestCount8_16_nn (0, 'g' _ 'h' _ 0xD800 _ 0xE000, ec_UtfLowSurrExpected, 3);
  TestCount8_16_nn (0, 'g' _ 'h' _ 0xD800,          ec_UtfSurrEnd, 2);
  printf ("\n");

  printf ("Conv UTF-8 = UTF-16 --------------------------------------\n\n");

  {
  const t_UInt16 au [] = { 0 };
  TestConv (au_Dest8, 1, au, 0, false, ec_UtfDestTooLarge, au_Dest16);
  }
  TestConv8_16 (0, 0,                   true,  ec_UtfDestTooSmall);
  TestConv8_16 (0, 0,                   false, ec_UtfDestTooSmall);
  TestConv8_16 (2, 0,                   true,  ec_UtfDestTooLarge);
  TestConv8_16 (2, 0,                   false, ec_UtfDestTooLarge);
  TestConv8_16 (3, 'a' _ 'b' _ 'c' _ 0, true,  ec_UtfDestTooSmall);
  TestConv8_16 (3, 'a' _ 'b' _ 'c' _ 0, false, ec_UtfDestTooSmall);
  TestConv8_16 (5, 'a' _ 'b' _ 'c' _ 0, true,  ec_UtfDestTooLarge);
  TestConv8_16 (5, 'a' _ 'b' _ 'c' _ 0, false, ec_UtfDestTooLarge);
  printf ("\n");

  TestConv8_16_nn (3, 'g' _ 'h' _ 0xE4,   ec_UtfDestTooSmall);
  TestConv8_16_nn (5, 'g' _ 'h' _ 0xE4,   ec_UtfDestTooLarge);
  TestConv8_16_nn (4, 'g' _ 'h' _ 0x20AC, ec_UtfDestTooSmall);
  TestConv8_16_nn (6, 'g' _ 'h' _ 0x20AC, ec_UtfDestTooLarge);
  TestConv8_16_nn (5, 'g' _ 'h' _ 0xD834 _ 0xDD1E, ec_UtfDestTooSmall);
  TestConv8_16_nn (7, 'g' _ 'h' _ 0xD834 _ 0xDD1E, ec_UtfDestTooLarge);
  printf ("\n");

  {
  const t_UInt16 au [] = { 0 };
  TestConv (au_Dest8, 0, au, 0, true,  ec_UtfMissingNull, au_Dest16);
  TestConv (au_Dest8, 0, au, 0, false, ec_UtfOK, au_Dest16);
  }
  TestConv8_16 (1, 0,                   true,  ec_UtfOK);
  TestConv8_16 (1, 0,                   false, ec_UtfOK);
  TestConv8_16 (3, 'a' _ 'b' _ 'c',     true,  ec_UtfMissingNull);
  TestConv8_16 (3, 'a' _ 'b' _ 'c',     false, ec_UtfOK);
  TestConv8_16 (4, 'a' _ 'b' _ 'c' _ 0, true,  ec_UtfOK);
  TestConv8_16 (4, 'a' _ 'b' _ 'c' _ 0, false, ec_UtfOK);
  TestConv8_16 (4, 'a' _  0  _ 'c' _ 0, true,  ec_UtfNullInside);
  TestConv8_16 (4, 'a' _  0  _ 'c' _ 0, false, ec_UtfOK);
  TestConv8_16 (4, 'a' _ 'b' _  0  _ 0, true,  ec_UtfNullInside);
  TestConv8_16 (4, 'a' _ 'b' _  0  _ 0, false, ec_UtfOK);
  printf ("\n");

  TestConv8_16_nn (3, 'g' _ 'h' _ 'i',    ec_UtfOK);
  TestConv8_16_nn (4, 'g' _ 'h' _ 0xE4,   ec_UtfOK);
  TestConv8_16_nn (4, 'g' _ 0xE4 _ 'h',   ec_UtfOK);
  TestConv8_16_nn (4, 0xE4 _ 'g' _ 'h',   ec_UtfOK);
  TestConv8_16_nn (5, 'g' _ 'h' _ 0x20AC, ec_UtfOK);
  TestConv8_16_nn (5, 'g' _ 0x20AC _ 'h', ec_UtfOK);
  TestConv8_16_nn (5, 0x20AC _ 'g' _ 'h', ec_UtfOK);
  TestConv8_16_nn (6, 'g' _ 'h' _ 0xD834 _ 0xDD1E, ec_UtfOK);
  TestConv8_16_nn (6, 'g' _ 0xD834 _ 0xDD1E _ 'h', ec_UtfOK);
  TestConv8_16_nn (6, 0xD834 _ 0xDD1E _ 'g' _ 'h', ec_UtfOK);
  printf ("\n");

  TestConv8_16_nn (3, 'g' _ 'h' _ 0x0,    ec_UtfOK);
  TestConv8_16_nn (3, 'g' _ 'h' _ 0x7F,   ec_UtfOK);
  TestConv8_16_nn (4, 'g' _ 'h' _ 0x80,   ec_UtfOK);
  TestConv8_16_nn (4, 'g' _ 'h' _ 0x7FF,  ec_UtfOK);
  TestConv8_16_nn (5, 'g' _ 'h' _ 0x800,  ec_UtfOK);
  TestConv8_16_nn (5, 'g' _ 'h' _ 0xFFFF, ec_UtfOK);
  printf ("\n");

  TestConv8_16_nn (6, 'g' _ 'h' _ 0xD7FF _ 0,      ec_UtfOK);
  TestConv8_16_nn (6, 'g' _ 'h' _ 0xD800 _ 0xDC00, ec_UtfOK);
  TestConv8_16_nn (6, 'g' _ 'h' _ 0xDBFF _ 0xDC00, ec_UtfOK);
  TestConv8_16_nn (6, 'g' _ 'h' _ 0xDC00 _ 0xDC00, ec_UtfHighSurrExpected);
  TestConv8_16_nn (6, 'g' _ 'h' _ 0xDFFF _ 0xDC00, ec_UtfHighSurrExpected);
  TestConv8_16_nn (6, 'g' _ 'h' _ 0xE000 _ 0,      ec_UtfOK);
  TestConv8_16_nn (6, 'g' _ 'h' _ 0xD800 _ 0,      ec_UtfLowSurrExpected);
  TestConv8_16_nn (6, 'g' _ 'h' _ 0xD800 _ 0xDBFF, ec_UtfLowSurrExpected);
  TestConv8_16_nn (6, 'g' _ 'h' _ 0xD800 _ 0xDC00, ec_UtfOK);
  TestConv8_16_nn (6, 'g' _ 'h' _ 0xD800 _ 0xDFFF, ec_UtfOK);
  TestConv8_16_nn (6, 'g' _ 'h' _ 0xDBFF _ 0xDFFF, ec_UtfOK);
  TestConv8_16_nn (6, 'g' _ 'h' _ 0xD800 _ 0xE000, ec_UtfLowSurrExpected);
  TestConv8_16_nn (6, 'g' _ 'h' _ 0xD800,          ec_UtfSurrEnd);
  printf ("\n");

  printf ("Length --------------------------------------\n\n");

  TestLength8_nn (3, "ghi",                ec_UtfOK, 0);
  TestLength8_nn (3, "gh\xC3\xA4",         ec_UtfOK, 0);
  TestLength8_nn (3, "gh\xE2\x82\xAC",     ec_UtfOK, 0);
  TestLength8_nn (3, "gh\xF0\x9D\x84\x9E", ec_UtfOK, 0);
  printf ("\n");

  TestLength16_nn (3, 'g' _ 'h' _ 'i',             ec_UtfOK, 0);
  TestLength16_nn (3, 'g' _ 'h' _ 0xD834 _ 0xDD1E, ec_UtfOK, 0);
  TestLength16_nn (3, 'g' _ 0xD834 _ 0xDD1E _ 'h', ec_UtfOK, 0);
  TestLength16_nn (3, 0xD834 _ 0xDD1E _ 'g' _ 'h', ec_UtfOK, 0);
  printf ("\n");

  {
  const t_UInt32 au [] = { 0 };
  TestLength (0, au, 0, true,  ec_UtfMissingNull, 0);
  TestLength (0, au, 0, false, ec_UtfOK,          0);
  }
  TestLength32 (1, 0,                   true,  ec_UtfOK,          0);
  TestLength32 (1, 0,                   false, ec_UtfOK,          0);
  TestLength32 (0, 'a' _ 'b' _ 'c',     true,  ec_UtfMissingNull, 2);
  TestLength32 (3, 'a' _ 'b' _ 'c',     false, ec_UtfOK,          0);
  TestLength32 (4, 'a' _ 'b' _ 'c' _ 0, true,  ec_UtfOK,          0);
  TestLength32 (4, 'a' _ 'b' _ 'c' _ 0, false, ec_UtfOK,          0);
  TestLength32 (0, 'a' _  0  _ 'c' _ 0, true,  ec_UtfNullInside,  1);
  TestLength32 (4, 'a' _  0  _ 'c' _ 0, false, ec_UtfOK,          0);
  TestLength32 (0, 'a' _ 'b' _  0  _ 0, true,  ec_UtfNullInside,  2);
  TestLength32 (4, 'a' _ 'b' _  0  _ 0, false, ec_UtfOK,          0);
  printf ("\n");

  TestLength32_nn (3, 'g' _ 'h' _ 'i',      ec_UtfOK, 0);
  TestLength32_nn (3, 'g' _ 'h' _ 0xE4,     ec_UtfOK, 0);
  TestLength32_nn (3, 'g' _ 'h' _ 0x20AC,   ec_UtfOK, 0);
  TestLength32_nn (3, 'g' _ 'h' _ 0x1D11E,  ec_UtfOK, 0);
  TestLength32_nn (3, 'g' _ 'h' _ 0x10FFFF, ec_UtfOK, 0);
  TestLength32_nn (0, 'g' _ 'h' _ 0x110000, ec_UtfWideRange, 2);
  printf ("\n");

  TestLength32_nn (3, 'g' _ 'h' _ 0xD7FF, ec_UtfOK, 0);
  TestLength32_nn (0, 'g' _ 'h' _ 0xD800, ec_UtfSurrogate, 2);
  TestLength32_nn (0, 'g' _ 'h' _ 0xDFFF, ec_UtfSurrogate, 2);
  TestLength32_nn (3, 'g' _ 'h' _ 0xE000, ec_UtfOK, 0);
  printf ("\n");

  printf ("Upper/Lower --------------------------------------\n\n");

  #ifdef TL_UNIX
  setlocale (LC_ALL, "de_DE.utf8");
  #endif

  TestUL8 ("gh\xC3");
  TestUL8 ("gh\xE2\x82");
  TestUL8 ("gh\xF0\x9D\x84");
  printf ("\n");

  TestUL8 ("");
  TestUL8 (" -");
  TestUL8 (" aA-");
  TestUL8 (" \xC3\xA4\xC3\x84-");
  TestUL8 (" \xE1\xB8\x81\xE1\xB8\x80-");
  printf ("\n");

  TestUL16 ('a' _ 0xD834);
  TestUL16 ('a' _ 0xD834 _ 0xDBFF _ 'A');
  TestUL16 ('a' _ 0xD834 _ 0xDD1E _ 'A');
  printf ("\n");

  TestUL16 (' ' _ '-');
  TestUL16 (' ' _ 'a' _ 'A' _ '-');
  TestUL16 (' ' _ 0xE4 _ 0xC4 _ '-');
  TestUL16 (' ' _ 0x1E01 _ 0x1E00 _ '-');
  printf ("\n");

  TestUL32 ('a' _ 0xD7FF _ 'A');
  TestUL32 ('a' _ 0xD800 _ 'A');
  TestUL32 ('a' _ 0xDFFF _ 'A');
  TestUL32 ('a' _ 0xE000 _ 'A');
  TestUL32 ('a' _ 0x10FFFF _ 'A');
  TestUL32 ('a' _ 0x110000 _ 'A');
  printf ("\n");

  TestUL32 (' ' _ '-');
  TestUL32 (' ' _ 'a' _ 'A' _ '-');
  TestUL32 (' ' _ 0xE4 _ 0xC4 _ '-');
  TestUL32 (' ' _ 0x1E01 _ 0x1E00 _ '-');
  printf ("\n");

  printf ("Const iterator --------------------------------------\n\n");

  TestCit8 ("",               ec_UtfEOS);
  TestCit8 ("gh\xC3",         ec_UtfMbEnd);
  TestCit8 ("gh\xE2\x82",     ec_UtfMbEnd);
  TestCit8 ("gh\xF0\x9D\x84", ec_UtfMbEnd);
  printf ("\n");

  TestCit8 ("ghi",                ec_UtfEOS);
  TestCit8 ("gh\xC3\xA4",         ec_UtfEOS);
  TestCit8 ("g\xC3\xA4h",         ec_UtfEOS);
  TestCit8 ("\xC3\xA4gh",         ec_UtfEOS);
  TestCit8 ("gh\xE2\x82\xAC",     ec_UtfEOS);
  TestCit8 ("g\xE2\x82\xACh",     ec_UtfEOS);
  TestCit8 ("\xE2\x82\xACgh",     ec_UtfEOS);
  TestCit8 ("gh\xF0\x9D\x84\x9E", ec_UtfEOS);
  TestCit8 ("g\xF0\x9D\x84\x9Eh", ec_UtfEOS);
  TestCit8 ("\xF0\x9D\x84\x9Egh", ec_UtfEOS);
  printf ("\n");

  TestCit ((const t_UInt16 *) 0, 0,       ec_UtfEOS);
  TestCit16 ('g' _ 'h' _ 0xD800,          ec_UtfSurrEnd);
  TestCit16 ('g' _ 'h' _ 0xD800 _ 0,      ec_UtfLowSurrExpected);
  TestCit16 ('g' _ 'h' _ 0xDC00 _ 0xDC00, ec_UtfHighSurrExpected);
  printf ("\n");

  TestCit16 ('g' _ 'h' _ 'i',             ec_UtfEOS);
  TestCit16 ('g' _ 'h' _ 0xE4,            ec_UtfEOS);
  TestCit16 ('g' _ 0xE4 _ 'h',            ec_UtfEOS);
  TestCit16 (0xE4 _ 'g' _ 'h',            ec_UtfEOS);
  TestCit16 ('g' _ 'h' _ 0x20AC,          ec_UtfEOS);
  TestCit16 ('g' _ 0x20AC _ 'h',          ec_UtfEOS);
  TestCit16 (0x20AC _ 'g' _ 'h',          ec_UtfEOS);
  TestCit16 ('g' _ 'h' _ 0xD834 _ 0xDD1E, ec_UtfEOS);
  TestCit16 ('g' _ 0xD834 _ 0xDD1E _ 'h', ec_UtfEOS);
  TestCit16 (0xD834 _ 0xDD1E _ 'g' _ 'h', ec_UtfEOS);
  printf ("\n");

  TestCit ((const t_UInt32 *) 0, 0, ec_UtfEOS);
  TestCit32 ('g' _ 'h' _ 0xD800,    ec_UtfSurrogate);
  TestCit32 ('g' _ 'h' _ 0x110000,  ec_UtfWideRange);
  printf ("\n");

  TestCit32 ('g' _ 'h' _ 'i',     ec_UtfEOS);
  TestCit32 ('g' _ 'h' _ 0xE4,    ec_UtfEOS);
  TestCit32 ('g' _ 0xE4 _ 'h',    ec_UtfEOS);
  TestCit32 (0xE4 _ 'g' _ 'h',    ec_UtfEOS);
  TestCit32 ('g' _ 'h' _ 0x20AC,  ec_UtfEOS);
  TestCit32 ('g' _ 0x20AC _ 'h',  ec_UtfEOS);
  TestCit32 (0x20AC _ 'g' _ 'h',  ec_UtfEOS);
  TestCit32 ('g' _ 'h' _ 0x1D11E, ec_UtfEOS);
  TestCit32 ('g' _ 0x1D11E _ 'h', ec_UtfEOS);
  TestCit32 (0x1D11E _ 'g' _ 'h', ec_UtfEOS);
  printf ("\n");

  return 0;
  }
