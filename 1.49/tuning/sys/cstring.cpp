
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
// File tuning/sys/cstring.cpp

#include "tuning/sys/cstring.hpp"
#include "tuning/sys/cmemory.hpp"
#include "tuning/sys/calloc.hpp"
#include "tuning/chn/store.hpp"
#include "tuning/miniblock.h"
#include "tuning/charblock.h"
#include <string.h>
#include <stdlib.h>
#include <wchar.h>

#if defined TL_WINDOWS
  #include <windows.h>
#else
  #include <wctype.h>
#endif

//---------------------------------------------------------------------------

static const char ac_Upper [256] =
  {
  '\00',  /*'\00' */ '\01',  /*'\01' */ '\02',  /*'\02' */ '\03',  /*'\03' */
  '\04',  /*'\04' */ '\05',  /*'\05' */ '\06',  /*'\06' */ '\07',  /*'\07' */
  '\010', /*'\010'*/ '\011', /*'\011'*/ '\012', /*'\012'*/ '\013', /*'\013'*/
  '\014', /*'\014'*/ '\015', /*'\015'*/ '\016', /*'\016'*/ '\017', /*'\017'*/
  '\020', /*'\020'*/ '\021', /*'\021'*/ '\022', /*'\022'*/ '\023', /*'\023'*/
  '\024', /*'\024'*/ '\025', /*'\025'*/ '\026', /*'\026'*/ '\027', /*'\027'*/
  '\030', /*'\030'*/ '\031', /*'\031'*/ '\032', /*'\032'*/ '\033', /*'\033'*/
  '\034', /*'\034'*/ '\035', /*'\035'*/ '\036', /*'\036'*/ '\037', /*'\037'*/
  ' ',  /* ' ' */  '!',  /* '!' */  '"',  /* '"' */  '#',  /* '#' */
  '$',  /* '$' */  '%',  /* '%' */  '&',  /* '&' */  '\'', /* '\''*/
  '(',  /* '(' */  ')',  /* ')' */  '*',  /* '*' */  '+',  /* '+' */
  ',',  /* ',' */  '-',  /* '-' */  '.',  /* '.' */  '/',  /* '/' */
  '0',  /* '0' */  '1',  /* '1' */  '2',  /* '2' */  '3',  /* '3' */
  '4',  /* '4' */  '5',  /* '5' */  '6',  /* '6' */  '7',  /* '7' */
  '8',  /* '8' */  '9',  /* '9' */  ':',  /* ':' */  ';',  /* ';' */
  '<',  /* '<' */  '=',  /* '=' */  '>',  /* '>' */  '?',  /* '?' */
  '@',  /* '@' */  'A',  /* 'A' */  'B',  /* 'B' */  'C',  /* 'C' */
  'D',  /* 'D' */  'E',  /* 'E' */  'F',  /* 'F' */  'G',  /* 'G' */
  'H',  /* 'H' */  'I',  /* 'I' */  'J',  /* 'J' */  'K',  /* 'K' */
  'L',  /* 'L' */  'M',  /* 'M' */  'N',  /* 'N' */  'O',  /* 'O' */
  'P',  /* 'P' */  'Q',  /* 'Q' */  'R',  /* 'R' */  'S',  /* 'S' */
  'T',  /* 'T' */  'U',  /* 'U' */  'V',  /* 'V' */  'W',  /* 'W' */
  'X',  /* 'X' */  'Y',  /* 'Y' */  'Z',  /* 'Z' */  '[',  /* '[' */
  '\\', /* '\\'*/  ']',  /* ']' */  '^',  /* '^' */  '_',  /* '_' */
  '`',  /* '`' */  'A',  /* 'a' */  'B',  /* 'b' */  'C',  /* 'c' */
  'D',  /* 'd' */  'E',  /* 'e' */  'F',  /* 'f' */  'G',  /* 'g' */
  'H',  /* 'h' */  'I',  /* 'i' */  'J',  /* 'j' */  'K',  /* 'k' */
  'L',  /* 'l' */  'M',  /* 'm' */  'N',  /* 'n' */  'O',  /* 'o' */
  'P',  /* 'p' */  'Q',  /* 'q' */  'R',  /* 'r' */  'S',  /* 's' */
  'T',  /* 't' */  'U',  /* 'u' */  'V',  /* 'v' */  'W',  /* 'w' */
  'X',  /* 'x' */  'Y',  /* 'y' */  'Z',  /* 'z' */  '{',  /* '{' */
  '|',  /* '|' */  '}',  /* '}' */  '~',  /* '~' */  '',  /* '' */
  'Ä',  /* 'Ä' */  'Å',  /* 'Å' */  'Ç',  /* 'Ç' */  'É',  /* 'É' */
  'Ñ',  /* 'Ñ' */  'Ö',  /* 'Ö' */  'Ü',  /* 'Ü' */  'á',  /* 'á' */
  'à',  /* 'à' */  'â',  /* 'â' */  'ä',  /* 'ä' */  'ã',  /* 'ã' */
  'å',  /* 'å' */  'ç',  /* 'ç' */  'é',  /* 'é' */  'è',  /* 'è' */
  'ê',  /* 'ê' */  'ë',  /* 'ë' */  'í',  /* 'í' */  'ì',  /* 'ì' */
  'î',  /* 'î' */  'ï',  /* 'ï' */  'ñ',  /* 'ñ' */  'ó',  /* 'ó' */
  'ò',  /* 'ò' */  'ô',  /* 'ô' */  'ä',  /* 'ö' */  'õ',  /* 'õ' */
  'å',  /* 'ú' */  'ù',  /* 'ù' */  'é',  /* 'û' */  'ü',  /* 'ü' */
  '†',  /* '†' */  '°',  /* '°' */  '¢',  /* '¢' */  '£',  /* '£' */
  '§',  /* '§' */  '•',  /* '•' */  '¶',  /* '¶' */  'ß',  /* 'ß' */
  '®',  /* '®' */  '©',  /* '©' */  '™',  /* '™' */  '´',  /* '´' */
  '¨',  /* '¨' */  '≠',  /* '≠' */  'Æ',  /* 'Æ' */  'Ø',  /* 'Ø' */
  '∞',  /* '∞' */  '±',  /* '±' */  '≤',  /* '≤' */  '≥',  /* '≥' */
  '¥',  /* '¥' */  'µ',  /* 'µ' */  '∂',  /* '∂' */  '∑',  /* '∑' */
  '∏',  /* '∏' */  'π',  /* 'π' */  '∫',  /* '∫' */  'ª',  /* 'ª' */
  'º',  /* 'º' */  'Ω',  /* 'Ω' */  'æ',  /* 'æ' */  'ø',  /* 'ø' */
  '¿',  /* '¿' */  '¡',  /* '¡' */  '¬',  /* '¬' */  '√',  /* '√' */
  'ƒ',  /* 'ƒ' */  '≈',  /* '≈' */  '∆',  /* '∆' */  '«',  /* '«' */
  '»',  /* '»' */  '…',  /* '…' */  ' ',  /* ' ' */  'À',  /* 'À' */
  'Ã',  /* 'Ã' */  'Õ',  /* 'Õ' */  'Œ',  /* 'Œ' */  'œ',  /* 'œ' */
  '–',  /* '–' */  '—',  /* '—' */  '“',  /* '“' */  '”',  /* '”' */
  '‘',  /* '‘' */  '’',  /* '’' */  '÷',  /* '÷' */  '◊',  /* '◊' */
  'ÿ',  /* 'ÿ' */  'Ÿ',  /* 'Ÿ' */  '⁄',  /* '⁄' */  '€',  /* '€' */
  '‹',  /* '‹' */  '›',  /* '›' */  'ﬁ',  /* 'ﬁ' */  'ﬂ',  /* 'ﬂ' */
  '¿',  /* '‡' */  '¡',  /* '·' */  '¬',  /* '‚' */  '√',  /* '„' */
  'ƒ',  /* '‰' */  '≈',  /* 'Â' */  '∆',  /* 'Ê' */  '«',  /* 'Á' */
  '»',  /* 'Ë' */  '…',  /* 'È' */  ' ',  /* 'Í' */  'À',  /* 'Î' */
  'Ã',  /* 'Ï' */  'Õ',  /* 'Ì' */  'Œ',  /* 'Ó' */  'œ',  /* 'Ô' */
  '–',  /* '' */  '—',  /* 'Ò' */  '“',  /* 'Ú' */  '”',  /* 'Û' */
  '‘',  /* 'Ù' */  '’',  /* 'ı' */  '÷',  /* 'ˆ' */  '˜',  /* '˜' */
  'ÿ',  /* '¯' */  'Ÿ',  /* '˘' */  '⁄',  /* '˙' */  '€',  /* '˚' */
  '‹',  /* '¸' */  '›',  /* '˝' */  'ﬁ',  /* '˛' */  'ü',  /* 'ˇ' */
  };

//---------------------------------------------------------------------------

static const char ac_Lower [256] =
  {
  '\00',  /*'\00' */ '\01',  /*'\01' */ '\02',  /*'\02' */ '\03',  /*'\03' */
  '\04',  /*'\04' */ '\05',  /*'\05' */ '\06',  /*'\06' */ '\07',  /*'\07' */
  '\010', /*'\010'*/ '\011', /*'\011'*/ '\012', /*'\012'*/ '\013', /*'\013'*/
  '\014', /*'\014'*/ '\015', /*'\015'*/ '\016', /*'\016'*/ '\017', /*'\017'*/
  '\020', /*'\020'*/ '\021', /*'\021'*/ '\022', /*'\022'*/ '\023', /*'\023'*/
  '\024', /*'\024'*/ '\025', /*'\025'*/ '\026', /*'\026'*/ '\027', /*'\027'*/
  '\030', /*'\030'*/ '\031', /*'\031'*/ '\032', /*'\032'*/ '\033', /*'\033'*/
  '\034', /*'\034'*/ '\035', /*'\035'*/ '\036', /*'\036'*/ '\037', /*'\037'*/
  ' ',  /* ' ' */  '!',  /* '!' */  '"',  /* '"' */  '#',  /* '#' */
  '$',  /* '$' */  '%',  /* '%' */  '&',  /* '&' */  '\'', /* '\''*/
  '(',  /* '(' */  ')',  /* ')' */  '*',  /* '*' */  '+',  /* '+' */
  ',',  /* ',' */  '-',  /* '-' */  '.',  /* '.' */  '/',  /* '/' */
  '0',  /* '0' */  '1',  /* '1' */  '2',  /* '2' */  '3',  /* '3' */
  '4',  /* '4' */  '5',  /* '5' */  '6',  /* '6' */  '7',  /* '7' */
  '8',  /* '8' */  '9',  /* '9' */  ':',  /* ':' */  ';',  /* ';' */
  '<',  /* '<' */  '=',  /* '=' */  '>',  /* '>' */  '?',  /* '?' */
  '@',  /* '@' */  'a',  /* 'A' */  'b',  /* 'B' */  'c',  /* 'C' */
  'd',  /* 'D' */  'e',  /* 'E' */  'f',  /* 'F' */  'g',  /* 'G' */
  'h',  /* 'H' */  'i',  /* 'I' */  'j',  /* 'J' */  'k',  /* 'K' */
  'l',  /* 'L' */  'm',  /* 'M' */  'n',  /* 'N' */  'o',  /* 'O' */
  'p',  /* 'P' */  'q',  /* 'Q' */  'r',  /* 'R' */  's',  /* 'S' */
  't',  /* 'T' */  'u',  /* 'U' */  'v',  /* 'V' */  'w',  /* 'W' */
  'x',  /* 'X' */  'y',  /* 'Y' */  'z',  /* 'Z' */  '[',  /* '[' */
  '\\', /* '\\'*/  ']',  /* ']' */  '^',  /* '^' */  '_',  /* '_' */
  '`',  /* '`' */  'a',  /* 'a' */  'b',  /* 'b' */  'c',  /* 'c' */
  'd',  /* 'd' */  'e',  /* 'e' */  'f',  /* 'f' */  'g',  /* 'g' */
  'h',  /* 'h' */  'i',  /* 'i' */  'j',  /* 'j' */  'k',  /* 'k' */
  'l',  /* 'l' */  'm',  /* 'm' */  'n',  /* 'n' */  'o',  /* 'o' */
  'p',  /* 'p' */  'q',  /* 'q' */  'r',  /* 'r' */  's',  /* 's' */
  't',  /* 't' */  'u',  /* 'u' */  'v',  /* 'v' */  'w',  /* 'w' */
  'x',  /* 'x' */  'y',  /* 'y' */  'z',  /* 'z' */  '{',  /* '{' */
  '|',  /* '|' */  '}',  /* '}' */  '~',  /* '~' */  '',  /* '' */
  'Ä',  /* 'Ä' */  'Å',  /* 'Å' */  'Ç',  /* 'Ç' */  'É',  /* 'É' */
  'Ñ',  /* 'Ñ' */  'Ö',  /* 'Ö' */  'Ü',  /* 'Ü' */  'á',  /* 'á' */
  'à',  /* 'à' */  'â',  /* 'â' */  'ö',  /* 'ä' */  'ã',  /* 'ã' */
  'ú',  /* 'å' */  'ç',  /* 'ç' */  'û',  /* 'é' */  'è',  /* 'è' */
  'ê',  /* 'ê' */  'ë',  /* 'ë' */  'í',  /* 'í' */  'ì',  /* 'ì' */
  'î',  /* 'î' */  'ï',  /* 'ï' */  'ñ',  /* 'ñ' */  'ó',  /* 'ó' */
  'ò',  /* 'ò' */  'ô',  /* 'ô' */  'ö',  /* 'ö' */  'õ',  /* 'õ' */
  'ú',  /* 'ú' */  'ù',  /* 'ù' */  'û',  /* 'û' */  'ˇ',  /* 'ü' */
  '†',  /* '†' */  '°',  /* '°' */  '¢',  /* '¢' */  '£',  /* '£' */
  '§',  /* '§' */  '•',  /* '•' */  '¶',  /* '¶' */  'ß',  /* 'ß' */
  '®',  /* '®' */  '©',  /* '©' */  '™',  /* '™' */  '´',  /* '´' */
  '¨',  /* '¨' */  '≠',  /* '≠' */  'Æ',  /* 'Æ' */  'Ø',  /* 'Ø' */
  '∞',  /* '∞' */  '±',  /* '±' */  '≤',  /* '≤' */  '≥',  /* '≥' */
  '¥',  /* '¥' */  'µ',  /* 'µ' */  '∂',  /* '∂' */  '∑',  /* '∑' */
  '∏',  /* '∏' */  'π',  /* 'π' */  '∫',  /* '∫' */  'ª',  /* 'ª' */
  'º',  /* 'º' */  'Ω',  /* 'Ω' */  'æ',  /* 'æ' */  'ø',  /* 'ø' */
  '‡',  /* '¿' */  '·',  /* '¡' */  '‚',  /* '¬' */  '„',  /* '√' */
  '‰',  /* 'ƒ' */  'Â',  /* '≈' */  'Ê',  /* '∆' */  'Á',  /* '«' */
  'Ë',  /* '»' */  'È',  /* '…' */  'Í',  /* ' ' */  'Î',  /* 'À' */
  'Ï',  /* 'Ã' */  'Ì',  /* 'Õ' */  'Ó',  /* 'Œ' */  'Ô',  /* 'œ' */
  '',  /* '–' */  'Ò',  /* '—' */  'Ú',  /* '“' */  'Û',  /* '”' */
  'Ù',  /* '‘' */  'ı',  /* '’' */  'ˆ',  /* '÷' */  '◊',  /* '◊' */
  '¯',  /* 'ÿ' */  '˘',  /* 'Ÿ' */  '˙',  /* '⁄' */  '˚',  /* '€' */
  '¸',  /* '‹' */  '˝',  /* '›' */  '˛',  /* 'ﬁ' */  'ﬂ',  /* 'ﬂ' */
  '‡',  /* '‡' */  '·',  /* '·' */  '‚',  /* '‚' */  '„',  /* '„' */
  '‰',  /* '‰' */  'Â',  /* 'Â' */  'Ê',  /* 'Ê' */  'Á',  /* 'Á' */
  'Ë',  /* 'Ë' */  'È',  /* 'È' */  'Í',  /* 'Í' */  'Î',  /* 'Î' */
  'Ï',  /* 'Ï' */  'Ì',  /* 'Ì' */  'Ó',  /* 'Ó' */  'Ô',  /* 'Ô' */
  '',  /* '' */  'Ò',  /* 'Ò' */  'Ú',  /* 'Ú' */  'Û',  /* 'Û' */
  'Ù',  /* 'Ù' */  'ı',  /* 'ı' */  'ˆ',  /* 'ˆ' */  '˜',  /* '˜' */
  '¯',  /* '¯' */  '˘',  /* '˘' */  '˙',  /* '˙' */  '˚',  /* '˚' */
  '¸',  /* '¸' */  '˝',  /* '˝' */  '˛',  /* '˛' */  'ˇ',  /* 'ˇ' */
  };

//---------------------------------------------------------------------------

char TL_EXPORT tl_ToUpper
(
char c
)
  {
  return ac_Upper [(unsigned char) c];
  }

//---------------------------------------------------------------------------

wchar_t TL_EXPORT tl_ToUpper
(
wchar_t c
)
  {
  if (c < 256)
    return (unsigned char) ac_Upper [(unsigned short) c];
  else
    return c;
  }

//---------------------------------------------------------------------------

char TL_EXPORT tl_ToLower
(
char c
)
  {
  return ac_Lower [(unsigned char) c];
  }

//---------------------------------------------------------------------------

wchar_t TL_EXPORT tl_ToLower
(
wchar_t c
)
  {
  if (c < 256)
    return (unsigned char) ac_Lower [(unsigned short) c];
  else
    return c;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_ToUpper
(
char * pc_str
)
  {
  TL_ASSERT (ac_Upper [0] == '\0');
  TL_ASSERT (pc_str != 0);

  char * pc = pc_str;

  while ((* pc = ac_Upper [(unsigned char) * pc]) != '\0')
    pc ++;

  return true;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_ToUpper
(
wchar_t * pc_str
)
  {
  TL_ASSERT (ac_Upper [0] == '\0');
  TL_ASSERT (pc_str != 0);

  wchar_t * pc = pc_str;

  while ((* pc = tl_ToUpper (* pc)) != L'\0')
    pc ++;

  return true;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_ToLower
(
char * pc_str
)
  {
  TL_ASSERT (ac_Lower [0] == '\0');
  TL_ASSERT (pc_str != 0);

  char * pc = pc_str;

  while ((* pc = ac_Lower [(unsigned char) * pc]) != '\0')
    pc ++;

  return true;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_ToLower
(
wchar_t * pc_str
)
  {
  TL_ASSERT (ac_Lower [0] == '\0');
  TL_ASSERT (pc_str != 0);

  wchar_t * pc = pc_str;

  while ((* pc = tl_ToLower (* pc)) != L'\0')
    pc ++;

  return true;
  }

//---------------------------------------------------------------------------

wchar_t TL_EXPORT tl_ToUpper2
(
wchar_t c
)
  {
  #if defined TL_WINDOWS

    return (wchar_t) (size_t) CharUpperW ((wchar_t *) c);

  #else

    return towupper (c);

  #endif
  }

//---------------------------------------------------------------------------

wchar_t TL_EXPORT tl_ToLower2
(
wchar_t c
)
  {
  #if defined TL_WINDOWS

    return (wchar_t) (size_t) CharLowerW ((wchar_t *) c);

  #else

    return towlower (c);

  #endif
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_ToUpper2
(
wchar_t * pc_str
)
  {
  TL_ASSERT (pc_str != 0);

  #if defined TL_WINDOWS

    return CharUpperW (pc_str) == pc_str;

  #else

    wchar_t * pc = pc_str;

    while ((* pc = towupper (* pc)) != L'\0')
      pc ++;

    return true;

  #endif
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_ToLower2
(
wchar_t * pc_str
)
  {
  TL_ASSERT (pc_str != 0);

  #if defined TL_WINDOWS

    return CharLowerW (pc_str) == pc_str;

  #else

    wchar_t * pc = pc_str;

    while ((* pc = towlower (* pc)) != L'\0')
      pc ++;

    return true;

  #endif
  }

//---------------------------------------------------------------------------

typedef gct_CharBlock <gct_EmptyBaseMiniBlock <ct_Chn_Store>, wchar_t> t_TempWCharBlock;

bool ConvertCase
(
char * pc_str,
bool b_upper
)
  {
  TL_ASSERT (pc_str != 0);

  t_UInt u_len = tl_StringLength (pc_str);

  if (u_len == 0)
    return true;

  t_TempWCharBlock co_block;
  t_UInt u_wCount = tl_MbConvertCount ((wchar_t *) 0, pc_str);
  co_block. SetCharSize (u_wCount);

  if (! tl_MbConvert (co_block. GetCharAddr (), pc_str, u_wCount))
    return false;

  if (b_upper)
    {
    if (! tl_ToUpper2 (co_block. GetCharAddr ()))
      return false;
    }
  else
    {
    if (! tl_ToLower2 (co_block. GetCharAddr ()))
      return false;
    }

  t_UInt u_count = tl_MbConvertCount ((char *) 0, co_block. GetCharAddr ());

  if (u_count != u_len + 1)
    return false;

  return tl_MbConvert (pc_str, co_block. GetCharAddr (), u_count);
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_ToUpper2
(
char * pc_str
)
  {
  return ConvertCase (pc_str, true);
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_ToLower2
(
char * pc_str
)
  {
  return ConvertCase (pc_str, false);
  }

//---------------------------------------------------------------------------

t_UInt TL_EXPORT tl_StringLength
(
const char * pc
)
  {
  TL_ASSERT (pc != 0);

  return strlen (pc);
  }

//---------------------------------------------------------------------------

t_UInt TL_EXPORT tl_StringLength
(
const wchar_t * pc
)
  {
  TL_ASSERT (pc != 0);

  #if defined (TL_GNU) && (TL_WCHAR_SIZE == 2)

  const wchar_t * pc_next = pc;

  while (* pc_next != L'\0')
    pc_next ++;

  return pc_next - pc;

  #else

  return wcslen (pc);

  #endif
  }

//---------------------------------------------------------------------------

unsigned TL_EXPORT tl_StringHash
(
const char * pc,
t_UInt u_length
)
  {
  TL_ASSERT (pc != 0);

  const unsigned char * puc = (const unsigned char *) pc;

  switch (u_length)
    {
    case 0:
      return 0;

    case 1:
      return (unsigned) (puc [0]);

    case 2:
      return (unsigned)
        ((puc [0] << 8) +
          puc [1]);

    case 3:
      return (unsigned)
        ((puc [0] << 10) +
         (puc [1] << 5) +
          puc [2]);

    case 4:
      return (unsigned)
        ((puc [0] << 12) +
         (puc [1] << 8) +
         (puc [2] << 4) +
          puc [3]);

    default:
      t_UInt u_mid1 = (u_length >> 1) - (u_length >> 2);
      t_UInt u_mid2 = (u_length >> 1) + (u_length >> 2);

      return (unsigned)
        ((puc [0           ] << 14) +
         (puc [1           ] << 12) +
         (puc [u_mid1      ] << 10) +
         (puc [u_mid1 + 1  ] <<  8) +
         (puc [u_mid2      ] <<  6) +
         (puc [u_mid2 + 1  ] <<  4) +
         (puc [u_length - 2] <<  2) +
         (puc [u_length - 1]));
    }
  }

//---------------------------------------------------------------------------

unsigned TL_EXPORT tl_StringHash
(
const wchar_t * pc,
t_UInt u_length
)
  {
  TL_ASSERT (pc != 0);

  switch (u_length)
    {
    case 0:
      return 0;

    case 1:
      return (unsigned) (pc [0]);

    case 2:
      return (unsigned)
        ((pc [0] << 8) +
          pc [1]);

    case 3:
      return (unsigned)
        ((pc [0] << 10) +
         (pc [1] << 5) +
          pc [2]);

    case 4:
      return (unsigned)
        ((pc [0] << 12) +
         (pc [1] << 8) +
         (pc [2] << 4) +
          pc [3]);

    default:
      t_UInt u_mid1 = (u_length >> 1) - (u_length >> 2);
      t_UInt u_mid2 = (u_length >> 1) + (u_length >> 2);

      return (unsigned)
        ((pc [0           ] << 14) +
         (pc [1           ] << 12) +
         (pc [u_mid1      ] << 10) +
         (pc [u_mid1 + 1  ] <<  8) +
         (pc [u_mid2      ] <<  6) +
         (pc [u_mid2 + 1  ] <<  4) +
         (pc [u_length - 2] <<  2) +
         (pc [u_length - 1]));
    }
  }

//---------------------------------------------------------------------------

t_UInt TL_EXPORT tl_MbConvertCount
(
char *,
const char * pc_src
)
  {
  return tl_StringLength (pc_src) + 1;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_MbConvert
(
char * pc_dst,
const char * pc_src,
t_UInt u_count
)
  {
  TL_ASSERT (u_count > 0);

  tl_CopyMemory (pc_dst, pc_src, u_count);
  return true;
  }

//---------------------------------------------------------------------------

t_UInt TL_EXPORT tl_MbConvertCount
(
wchar_t *,
const wchar_t * pc_src
)
  {
  return tl_StringLength (pc_src) + 1;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_MbConvert
(
wchar_t * pc_dst,
const wchar_t * pc_src,
t_UInt u_count
)
  {
  TL_ASSERT (u_count > 0);

  tl_CopyMemory (pc_dst, pc_src, u_count);
  return true;
  }

//---------------------------------------------------------------------------

t_UInt TL_EXPORT tl_MbConvertCount
(
wchar_t *,
const char * pc_src
)
  {
  #if defined TL_WINDOWS

    return MultiByteToWideChar (CP_UTF8, MB_ERR_INVALID_CHARS, pc_src, -1, NULL, 0);

  #else

    return mbstowcs (0, pc_src, 0) + 1;

  #endif
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_MbConvert
(
wchar_t * pc_dst,
const char * pc_src,
t_UInt u_count
)
  {
  TL_ASSERT (u_count > 0);

  #if defined TL_WINDOWS

    TL_ASSERT (u_count == (int) u_count);
    return ((t_UInt) MultiByteToWideChar
      (CP_UTF8, MB_ERR_INVALID_CHARS, pc_src, -1, pc_dst, (int) u_count)) == u_count;

  #elif defined (TL_GNU) && (TL_WCHAR_SIZE == 2)

    wint_t * pc_dstBuffer = (wint_t *) tl_Alloc (u_count * sizeof (wint_t));
    wint_t * pc_dstBufferCp = pc_dstBuffer;
    bool b_ret = (mbstowcs ((wchar_t *) pc_dstBuffer, pc_src, u_count) + 1 == u_count);

    while (u_count -- != 0)
      * (pc_dst ++) = (wchar_t) * (pc_dstBufferCp ++);

    tl_Free (pc_dstBuffer);
    return b_ret;

  #else

    return mbstowcs (pc_dst, pc_src, u_count) + 1 == u_count;

  #endif
  }

//---------------------------------------------------------------------------

t_UInt TL_EXPORT tl_MbConvertCount
(
char *,
const wchar_t * pc_src
)
  {
  #if defined TL_WINDOWS

    return WideCharToMultiByte (CP_UTF8, 0, pc_src, -1, NULL, 0, NULL, NULL);

  #elif defined (TL_GNU) && (TL_WCHAR_SIZE == 2)

    t_UInt u_count = tl_StringLength (pc_src) + 1;
    wint_t * pc_srcBuffer = (wint_t *) tl_Alloc (u_count * sizeof (wint_t));
    wint_t * pc_srcBufferCp = pc_srcBuffer;

    while (u_count -- != 0)
      * (pc_srcBufferCp ++) = * (pc_src ++);

    size_t u_len = wcstombs (0, (wchar_t *) pc_srcBuffer, 0) + 1;
    tl_Free (pc_srcBuffer);
    return u_len;

  #else

    return wcstombs (0, pc_src, 0) + 1;

  #endif
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_MbConvert
(
char * pc_dst,
const wchar_t * pc_src,
t_UInt u_count
)
  {
  TL_ASSERT (u_count > 0);

  #if defined TL_WINDOWS

    TL_ASSERT (u_count == (int) u_count);
    return ((t_UInt) WideCharToMultiByte
      (CP_UTF8, 0, pc_src, -1, pc_dst, (int) u_count, NULL, NULL)) == u_count;

  #elif defined (TL_GNU) && (TL_WCHAR_SIZE == 2)

    wint_t * pc_srcBuffer = (wint_t *) tl_Alloc (u_count * sizeof (wint_t));
    wint_t * pc_srcBufferCp = pc_srcBuffer;
    t_UInt u_conv = u_count;

    while (u_conv -- != 0)
      * (pc_srcBufferCp ++) = * (pc_src ++);

    bool b_ret = (wcstombs (pc_dst, (wchar_t *) pc_srcBuffer, u_count) + 1 == u_count);
    tl_Free (pc_srcBuffer);
    return b_ret;

  #else

    return wcstombs (pc_dst, pc_src, u_count) + 1 == u_count;

  #endif
  }
