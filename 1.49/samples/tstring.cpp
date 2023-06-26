
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
// File samples/tstring.cpp

#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include "tuning/block.h"
#include "tuning/fixblock.h"
#include "tuning/miniblock.h"
#include "tuning/nulldatablock.h"
#include "tuning/string.h"
#include "tuning/string.hpp"
#include "tuning/printf.hpp"
#include "tuning/chn/store.hpp"
#include "tuning/sys/cstring.hpp"

#if defined TL_WINDOWS
#include <windows.h>
#endif

//typedef gct_String <gct_CharBlock <gct_NullDataBlock <gct_FixBlock <t_UInt32, 64>,  char>,    char>,    ct_Chn32Store> t_Str;
//typedef gct_String <gct_CharBlock <gct_NullDataBlock <gct_FixBlock <t_UInt32, 128>, wchar_t>, wchar_t>, ct_Chn32Store> t_WStr;

//typedef gct_String <gct_CharBlock <gct_NullDataBlock <gct_EmptyBaseMiniBlock <ct_Chn32Store>, char>,    char>,    ct_Chn32Store> t_Str;
//typedef gct_String <gct_CharBlock <gct_NullDataBlock <gct_EmptyBaseMiniBlock <ct_Chn32Store>, wchar_t>, wchar_t>, ct_Chn32Store> t_WStr;

//typedef gct_String <gct_CharBlock <gct_NullDataBlock <gct_EmptyBaseBlock <ct_Chn32Store>, char>,    char>,    ct_Chn32Store> t_Str;
//typedef gct_String <gct_CharBlock <gct_NullDataBlock <gct_EmptyBaseBlock <ct_Chn32Store>, wchar_t>, wchar_t>, ct_Chn32Store> t_WStr;

//#include "tuning/chn/string.h"
//#include "tuning/chn/wstring.h"
//typedef ct_Chn_String  t_Str;
//typedef ct_Chn_WString t_WStr;

//#include "tuning/rnd/block.h"
//STRING_DCL (gct_EmptyBaseBlock, ct_Rnd_)
//WSTRING_DCL (gct_EmptyBaseBlock, ct_Rnd_)
//typedef ct_Rnd_String  t_Str;
//typedef ct_Rnd_WString t_WStr;

//#include "tuning/rnd/block.h"
//typedef STRING_BASE  (gct_ObjectBaseBlock, ct_Rnd_) ct_Rnd_ObjString;
//typedef WSTRING_BASE (gct_ObjectBaseBlock, ct_Rnd_) ct_Rnd_WObjString;
//OBJ_STRING_DCL(ct_Rnd_Obj)
//OBJ_STRING_DCL(ct_Rnd_WObj)
//typedef ct_Rnd_ObjectString  t_Str;
//typedef ct_Rnd_WObjectString t_WStr;

#include "tuning/string.hpp"
#include "tuning/wstring.hpp"
typedef ct_String  t_Str;
typedef ct_WString t_WStr;

//---------------------------------------------------------------------------

void PrintStr
(
const char * pc,
t_UInt u_len
)
  {
  while (u_len -- != 0)
    printf ("%c", * (pc ++));
  }

//---------------------------------------------------------------------------

void PrintStr
(
const wchar_t * pc,
t_UInt u_len
)
  {
  while (u_len -- != 0)
    printf ("%lc", * (pc ++));
  }

//---------------------------------------------------------------------------

void PrintObj
(
const t_Str & co_str
)
  {
  TL_ASSERT (t_UInt (co_str. GetLen ()) == tl_StringLength (co_str. GetStr ()));
  PrintStr (co_str. GetStr (), co_str. GetLen ());
  printf ("\n");
  }

//---------------------------------------------------------------------------

void PrintObj
(
const t_WStr & co_wstr
)
  {
  TL_ASSERT (t_UInt (co_wstr. GetLen ()) == tl_StringLength (co_wstr. GetStr ()));
  PrintStr (co_wstr. GetStr (), co_wstr. GetLen ());
  printf ("\n");
  }

//---------------------------------------------------------------------------

int Test_VSprintf
(
const char * pc_format,
...
)
  {
  va_list o_argList;
  va_start (o_argList, pc_format);
  char * pc_buffer = 0;
  int i_len = tl_VSprintf (& pc_buffer, pc_format, o_argList);
  va_end (o_argList);

  if (i_len < 0)
    return -1;
  else
    {
    printf ("%d \"", i_len);
    PrintStr (pc_buffer, i_len);
    printf ("\"\n");
    free (pc_buffer);
    return i_len;
    }
  }

//---------------------------------------------------------------------------

int Test_VSprintf
(
const wchar_t * pc_format,
...
)
  {
  va_list o_argList;
  va_start (o_argList, pc_format);
  wchar_t * pc_buffer = 0;
  int i_len = tl_VSprintf (& pc_buffer, pc_format, o_argList);
  va_end (o_argList);

  if (i_len < 0)
    return -1;
  else
    {
    printf ("%d \"", i_len);
    PrintStr (pc_buffer, i_len);
    printf ("\"\n");
    free (pc_buffer);
    return i_len;
    }
  }

//---------------------------------------------------------------------------

inline unsigned UnsignedChar (char c)
  {
  return (unsigned char) c;
  }

inline unsigned UnsignedChar (wchar_t c)
  {
  return c;
  }

template <class t_char1, class t_char2>
  void TestConv
  (
  t_char1 * pc_dst,
  const t_char2 * pc_src,
  t_UInt u_size
  )
    {
    tl_FillMemory (pc_dst, u_size, (t_char1) -1);
    t_UInt u_convLen = tl_MbConvertCount (pc_dst, pc_src);

    if (u_convLen == 0)
      printf ("Count error\n");
    else
      {
      if (! tl_MbConvert (pc_dst, pc_src, u_convLen))
        printf ("Conv error\n");
      else
        {
        printf ("Conv (%d) ", (unsigned) u_convLen);

        for (t_UInt u_char = 0; u_char <= u_convLen; u_char ++)
          printf ("%x ", UnsignedChar (pc_dst [u_char]));

        printf ("\n");
        }
      }
    }

//---------------------------------------------------------------------------

int IntSign
(
int i
)
  {
  if (i < 0)
    return -1;

  if (i > 0)
    return 1;
  else
    return 0;
  }

//---------------------------------------------------------------------------

template <class t_char>
  void TestConv
  (
  t_char * pc_conv,
  bool (* pf_conv) (t_char *)
  )
    {
    t_char * pc_str = pc_conv;

    while (* pc_str != (t_char) 0)
      printf ("%x ", UnsignedChar (* pc_str ++));

    printf ("-> ");

    if (! (* pf_conv) (pc_conv))
      printf ("Conversion failed\n");

    pc_str = pc_conv;

    while (* pc_str != (t_char) 0)
      printf ("%x ", UnsignedChar (* pc_str ++));

    printf ("\n");
    }

//---------------------------------------------------------------------------

int main ()
  {
  printf ("sizeof (wchar_t) %d\n\n", (unsigned) sizeof (wchar_t));
  printf ("Test First/Last Char/Memory\n\n");

  const char * pc = "012aab678aba";
  t_UInt u_len = tl_StringLength (pc) + 1;
  const char * pc1 = tl_FirstChar   (pc, u_len, 'a');
  const char * pc2 = tl_FirstMemory (pc, u_len, "ab", 2);
  const char * pc3 = tl_LastChar    (pc, u_len, 'a');
  const char * pc4 = tl_LastMemory  (pc, u_len, "ab", 2);
  printf ("%d %d %d %d\n", (int) (pc1 - pc),
    (int) (pc2 - pc), (int) (pc3 - pc), (int) (pc4 - pc));

  const wchar_t * wpc = L"012aab678aba";
  t_UInt u_wlen = tl_StringLength (wpc) + 1;
  const wchar_t * wpc1 = tl_FirstChar   (wpc, u_wlen, L'a');
  const wchar_t * wpc2 = tl_FirstMemory (wpc, u_wlen, L"ab", 2);
  const wchar_t * wpc3 = tl_LastChar    (wpc, u_wlen, L'a');
  const wchar_t * wpc4 = tl_LastMemory  (wpc, u_wlen, L"ab", 2);
  printf ("%d %d %d %d\n", (int) (wpc1 - wpc),
    (int) (wpc2 - wpc), (int) (wpc3 - wpc), (int) (wpc4 - wpc));
  printf ("\n");

  printf ("Test Compare Char/Memory\n\n");

  printf ("%d %d %d\n",
    IntSign (tl_CompareChar ('a', 'A')),
    IntSign (tl_CompareChar ('\xE4', '\xC4')),
    IntSign (tl_CompareChar ('a', '\xE4')));
  printf ("%d %d %d\n",
    IntSign (tl_CompareChar (L'a', L'A')),
    IntSign (tl_CompareChar (L'\xE4', L'\xC4')),
    IntSign (tl_CompareChar (L'a', L'\xE4')));
  printf ("\n");

  printf ("%d %d %d\n",
    IntSign (tl_CompareMemory ("012a", "012A", 4)),
    IntSign (tl_CompareMemory ("012\xE4", "012\xC4", 4)),
    IntSign (tl_CompareMemory ("012a", "012\xE4", 4)));
  printf ("%d %d %d\n",
    IntSign (tl_CompareMemory (L"012a", L"012A", 4)),
    IntSign (tl_CompareMemory (L"012\xE4", L"012\xC4", 4)),
    IntSign (tl_CompareMemory (L"012a", L"012\xE4", 4)));
  printf ("\n");

  printf ("Test Insert/Delete/Replace\n\n");

  t_Str co1;
  TL_ASSERT (co1. IsEmpty ());
  TL_ASSERT (t_UInt (co1. GetLen ()) == tl_StringLength (co1. GetStr ()));

  t_WStr wco1;
  TL_ASSERT (wco1. IsEmpty ());
  TL_ASSERT (t_UInt (wco1. GetLen ()) == tl_StringLength (wco1. GetStr ()));

  co1. Insert (0, 'a');
  PrintObj (co1);

  wco1. Insert (0, L'a');
  PrintObj (wco1);
  printf ("\n");

  co1. Clear ();
  TL_ASSERT (t_UInt (co1. GetLen ()) == tl_StringLength (co1. GetStr ()));

  wco1. Clear ();
  TL_ASSERT (t_UInt (wco1. GetLen ()) == tl_StringLength (wco1. GetStr ()));

  co1 = "0123456789";
  PrintObj (co1);

  wco1 = L"0123456789";
  PrintObj (wco1);
  printf ("\n");

  co1. Replace (5, 2, "abc");
  PrintObj (co1);

  wco1. Replace (5, 2, L"abc");
  PrintObj (wco1);
  printf ("\n");

  co1. Replace (5, 3, "56");
  PrintObj (co1);

  wco1. Replace (5, 3, L"56");
  PrintObj (wco1);
  printf ("\n");

  co1. Insert (5, "abc");
  PrintObj (co1);

  wco1. Insert (5, L"abc");
  PrintObj (wco1);
  printf ("\n");

  co1. Replace (5, 3, '.', 3);
  PrintObj (co1);

  wco1. Replace (5, 3, L'.', 3);
  PrintObj (wco1);
  printf ("\n");

  co1. Delete (5, 3);
  PrintObj (co1);

  wco1. Delete (5, 3);
  PrintObj (wco1);
  printf ("\n");

  t_Str co2;
  co2 = co1;
  PrintObj (co2);

  t_WStr wco2;
  wco2 = wco1;
  PrintObj (wco2);
  printf ("\n");

  co2 += co2;
  PrintObj (co2);

  wco2 += wco2;
  PrintObj (wco2);
  printf ("\n");

  co2. Delete (10);
  PrintObj (co2);

  wco2. Delete (10);
  PrintObj (wco2);
  printf ("\n");

  co1 = "abc";
  co1. Swap (co2);
  PrintObj (co1);

  wco1 = L"abc";
  wco1. Swap (wco2);
  PrintObj (wco1);
  printf ("\n");

  PrintObj (co2);
  PrintObj (wco2);
  printf ("\n");

  printf ("Test operator +\n\n");

  PrintObj ( co1 +  co2);
  PrintObj (wco1 + wco2);
  printf ("\n");

  PrintObj ( co1 +  "abc");
  PrintObj (wco1 + L"abc");
  printf ("\n");

  PrintObj ( "abc" +  co1);
  PrintObj (L"abc" + wco1);
  printf ("\n");

  PrintObj ( co1. SubStr (3));
  PrintObj (wco1. SubStr (3));
  printf ("\n");

  PrintObj ( co1. RevSubStr (3));
  PrintObj (wco1. RevSubStr (3));
  printf ("\n");

  PrintObj ( co1. SubStr (3, 4));
  PrintObj (wco1. SubStr (3, 4));
  printf ("\n");

  printf ("Test ReplaceAll\n\n");

  co1 = "123abcabc456";
  printf ("%d ", (int) co1. ReplaceAll ("abc", "\\/"));
  PrintObj (co1);

  wco1 = L"123abcabc456";
  printf ("%d ", (int) wco1. ReplaceAll (L"abc", L"\\/"));
  PrintObj (wco1);
  printf ("\n");

  co1 = "abc123abc4abc567abc";
  printf ("%d ", (int) co1. ReplaceAll ("abc", "\\/"));
  PrintObj (co1);

  wco1 = L"abc123abc4abc567abc";
  printf ("%d ", (int) wco1. ReplaceAll (L"abc", L"\\/"));
  PrintObj (wco1);
  printf ("\n");

  co1 = "123abcabc456";
  printf ("%d ", (int) co1. ReplaceAll ("abc", "\\./"));
  PrintObj (co1);

  wco1 = L"123abcabc456";
  printf ("%d ", (int) wco1. ReplaceAll (L"abc", L"\\./"));
  PrintObj (wco1);
  printf ("\n");

  co1 = "abc123abc4abc567abc";
  printf ("%d ", (int) co1. ReplaceAll ("abc", "\\./"));
  PrintObj (co1);

  wco1 = L"abc123abc4abc567abc";
  printf ("%d ", (int) wco1. ReplaceAll (L"abc", L"\\./"));
  PrintObj (wco1);
  printf ("\n");

  co1 = "123abcabc456";
  printf ("%d ", (int) co1. ReplaceAll ("abc", "\\.,/"));
  PrintObj (co1);

  wco1 = L"123abcabc456";
  printf ("%d ", (int) wco1. ReplaceAll (L"abc", L"\\.,/"));
  PrintObj (wco1);
  printf ("\n");

  co1 = "abc123abc4abc567abc";
  printf ("%d ", (int) co1. ReplaceAll ("abc", "\\.,/"));
  PrintObj (co1);

  wco1 = L"abc123abc4abc567abc";
  printf ("%d ", (int) wco1. ReplaceAll (L"abc", L"\\.,/"));
  PrintObj (wco1);
  printf ("\n");

  printf ("Test format\n\n");

  Test_VSprintf ("abc%ddef%.3f", 123, 456.789);
  Test_VSprintf (L"abc%ddef%.3f", 123, 456.789);
  printf ("\n");

  co1. AssignF ("abc%ddef%.3f", 123, 456.789);
  PrintObj (co1);

  wco1. AssignF (L"abc%ddef%.3f", 123, 456.789);
  PrintObj (wco1);
  printf ("\n");

  co1. AppendF ("ghi%.3fjkl%dmno", 456.789, 123);
  PrintObj (co1);

  wco1. AppendF (L"ghi%.3fjkl%dmno", 456.789, 123);
  PrintObj (wco1);
  printf ("\n");

  co1. InsertF (16, "\\/%.3f\\/%.d\\/", 456.789, 123);
  PrintObj (co1);

  wco1. InsertF (16, L"\\/%.3f\\/%.d\\/", 456.789, 123);
  PrintObj (wco1);
  printf ("\n");

  co1. ReplaceF (16, 16, "/\\%.d/\\%.3f/\\", 123, 456.789);
  PrintObj (co1);

  wco1. ReplaceF (16, 16, L"/\\%.d/\\%.3f/\\", 123, 456.789);
  PrintObj (wco1);
  printf ("\n");

  printf ("Test constructors ...\n\n");

  t_Str co3 ('a');
  t_Str co4 ('a', 5);
  t_Str co5 ("abc");
  t_Str co6 ("abc", 3);
  t_Str co7 (co6);

  t_WStr wco3 (L'a');
  t_WStr wco4 (L'a', 5);
  t_WStr wco5 (L"abc");
  t_WStr wco6 (L"abc", 3);
  t_WStr wco7 (wco6);

  printf ("Test Compare Strings\n\n");

  co1 = "";
  co2 = "";
  printf ("%2d %2d %2d %2d\n",
    IntSign (co1. CompTo ('\0')),
    IntSign (co1. CompTo ("")),
    IntSign (co1. CompTo ("", 0)),
    IntSign (co1. CompTo (co2)));
  co2 = "b";
  printf ("%2d %2d %2d %2d\n",
    IntSign (co1. CompTo ('b')),
    IntSign (co1. CompTo ("b")),
    IntSign (co1. CompTo ("b", 1)),
    IntSign (co1. CompTo (co2)));
  printf ("\n");

  co1 = "a";
  printf ("%2d %2d %2d %2d\n",
    IntSign (co1. CompTo ('b')),
    IntSign (co1. CompTo ("b")),
    IntSign (co1. CompTo ("b", 1)),
    IntSign (co1. CompTo (co2)));
  co1 = "b";
  printf ("%2d %2d %2d %2d\n",
    IntSign (co1. CompTo ('b')),
    IntSign (co1. CompTo ("b")),
    IntSign (co1. CompTo ("b", 1)),
    IntSign (co1. CompTo (co2)));
  co1 = "c";
  printf ("%2d %2d %2d %2d\n",
    IntSign (co1. CompTo ('b')),
    IntSign (co1. CompTo ("b")),
    IntSign (co1. CompTo ("b", 1)),
    IntSign (co1. CompTo (co2)));
  printf ("\n");

  co1 = "a";
  co2 = "ab";
  printf ("%2d %2d %2d %2d\n",
    IntSign (co1. CompTo ('a')),
    IntSign (co1. CompTo ("ab")),
    IntSign (co1. CompTo ("ab", 2)),
    IntSign (co1. CompTo (co2)));
  co1 = "ab";
  printf ("%2d %2d %2d %2d\n",
    IntSign (co1. CompTo ('a')),
    IntSign (co1. CompTo ("ab")),
    IntSign (co1. CompTo ("ab", 2)),
    IntSign (co1. CompTo (co2)));
  co2 = "a";
  printf ("%2d %2d %2d %2d\n",
    IntSign (co1. CompTo ('a')),
    IntSign (co1. CompTo ("a")),
    IntSign (co1. CompTo ("a", 1)),
    IntSign (co1. CompTo (co2)));
  printf ("\n");

  printf ("Test operator < / operator ==\n\n");

  co1 = "012345";
  co2 = "012345";
  printf ("%d %d\n", co1 < co2, co1 == co2);
  co2 = "012346";
  printf ("%d %d\n", co1 < co2, co1 == co2);
  co2 = "0123456";
  printf ("%d %d\n", co1 < co2, co1 == co2);
  printf ("\n");

  printf ("Test raw conversion\n\n");

  #ifdef TL_UNIX
  setlocale (LC_ALL, "de_DE.utf8");
  #endif

  const t_UInt u_testSize = 16;
  char    ac1  [u_testSize] =
    {
    /*a*/ 0x61, /*b*/ 0x62, /*c*/ 0x63,
    /*EuroSign*/ '\xe2', '\x82', '\xac',
    /*x*/ 0x78, /*y*/ 0x79, /*z*/ 0x7a, 0
    };
  wchar_t wac1 [u_testSize] =
    {
    /*a*/ 0x61, /*b*/ 0x62, /*c*/ 0x63,
    /*EuroSign*/ 0x20ac,
    /*x*/ 0x78, /*y*/ 0x79, /*z*/ 0x7a, 0
    };
  char    ac2  [u_testSize];
  wchar_t wac2 [u_testSize];

  TestConv (ac2, ac1, u_testSize);
  TestConv (ac2, wac1, u_testSize);
  printf ("\n");
  TestConv (wac2, wac1, u_testSize);
  TestConv (wac2, ac1, u_testSize);
  printf ("\n");

  printf ("Test conversion\n\n");

  co2 = "0123";
  co1. Convert (co2);
  PrintObj (co1);

  wco2 = L"abcde";
  wco1. Convert (wco2);
  PrintObj (wco1);

  co2 = "012345";
  wco1. Convert (co2);
  PrintObj (wco1);

  wco2 = L"abcdefg";
  co1. Convert (wco2);
  PrintObj (co1);

  co2 = "0123";
  co1. MbConvert (co2);
  PrintObj (co1);

  wco2 = L"abcde";
  wco1. MbConvert (wco2);
  PrintObj (wco1);

  co2 = "012345";
  wco1. MbConvert (co2);
  PrintObj (wco1);

  wco2 = L"abcdefg";
  co1. MbConvert (wco2);
  PrintObj (co1);

  co2 = "0123";
  co1. MbConvert (co2. GetStr ());
  PrintObj (co1);

  wco2 = L"abcde";
  wco1. MbConvert (wco2. GetStr ());
  PrintObj (wco1);

  co2 = "012345";
  wco1. MbConvert (co2. GetStr ());
  PrintObj (wco1);

  wco2 = L"abcdefg";
  co1. MbConvert (wco2. GetStr ());
  PrintObj (co1);
  printf ("\n");

  printf ("Test upper/lower\n\n");

  printf ("%x %x " , UnsignedChar (tl_ToUpper ( 'a')), UnsignedChar (tl_ToUpper ( '\xe4')));
  printf ("%x %x\n", UnsignedChar (tl_ToUpper (L'a')), UnsignedChar (tl_ToUpper (L'\xe4')));
  printf ("%x %x " , UnsignedChar (tl_ToLower ( 'A')), UnsignedChar (tl_ToLower ( '\xc4')));
  printf ("%x %x\n", UnsignedChar (tl_ToLower (L'A')), UnsignedChar (tl_ToLower (L'\xc4')));
  char    ac_sl [] =  "a\xe4";
  wchar_t ac_wl [] = L"a\xe4";
  char    ac_su [] =  "A\xc4";
  wchar_t ac_wu [] = L"A\xc4";
  TestConv (ac_sl, tl_ToUpper);
  TestConv (ac_wl, tl_ToUpper);
  TestConv (ac_su, tl_ToLower);
  TestConv (ac_wu, tl_ToLower);
  printf ("\n");

  printf ("Test upper2/lower2\n\n");

  printf ("%x %x " , UnsignedChar (tl_ToUpper2 (L'a')), UnsignedChar (tl_ToUpper2 (L'\xe4')));
  printf ("%x %x\n", UnsignedChar (tl_ToLower2 (L'A')), UnsignedChar (tl_ToLower2 (L'\xc4')));
  char    ac_sl2 [] =  "a\xc3\xa4";
  wchar_t ac_wl2 [] = L"a\xe4";
  char    ac_su2 [] =  "A\xc3\x84";
  wchar_t ac_wu2 [] = L"A\xc4";
  TestConv (ac_sl2, tl_ToUpper2);
  TestConv (ac_wl2, tl_ToUpper2);
  TestConv (ac_su2, tl_ToLower2);
  TestConv (ac_wu2, tl_ToLower2);
  printf ("\n");

  return 0;
  }
