
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
// File samples/tsort.cpp

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tuning/sys/ctimedate.hpp"
#include "tuning/sys/cmemory.hpp"
#include "tuning/sys/cstring.hpp"
#include "tuning/stringsort.hpp"

const t_Int i_TestSize = 1000000;
char * apc_Str [i_TestSize];
t_UInt32 au_Int1 [i_TestSize];
t_UInt32 au_Int2 [i_TestSize];
t_Int ai_Res [i_TestSize];

//---------------------------------------------------------------------------

static const char ac_MySortPage [256] =
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
  '`',  /* '`' */  'a',  /* 'a' */  'b',  /* 'b' */  'c',  /* 'c' */
  'd',  /* 'd' */  'e',  /* 'e' */  'f',  /* 'f' */  'g',  /* 'g' */
  'h',  /* 'h' */  'i',  /* 'i' */  'j',  /* 'j' */  'k',  /* 'k' */
  'l',  /* 'l' */  'm',  /* 'm' */  'n',  /* 'n' */  'o',  /* 'o' */
  'p',  /* 'p' */  'q',  /* 'q' */  'r',  /* 'r' */  's',  /* 's' */
  't',  /* 't' */  'u',  /* 'u' */  'v',  /* 'v' */  'w',  /* 'w' */
  'x',  /* 'x' */  'y',  /* 'y' */  'z',  /* 'z' */  '{',  /* '{' */
  '|',  /* '|' */  '}',  /* '}' */  '~',  /* '~' */  '\177',  /* '\177' */
  '\200',  /* '\200' */  '\201',  /* '\201' */  '\202',  /* '\202' */  '\203',  /* '\203' */
  '\204',  /* '\204' */  '\205',  /* '\205' */  '\206',  /* '\206' */  '\207',  /* '\207' */
  '\210',  /* '\210' */  '\211',  /* '\211' */  '\212',  /* '\212' */  '\213',  /* '\213' */
  '\214',  /* '\214' */  '\215',  /* '\215' */  '\216',  /* '\216' */  '\217',  /* '\217' */
  '\220',  /* '\220' */  '\221',  /* '\221' */  '\222',  /* '\222' */  '\223',  /* '\223' */
  '\224',  /* '\224' */  '\225',  /* '\225' */  '\226',  /* '\226' */  '\227',  /* '\227' */
  '\230',  /* '\230' */  '\231',  /* '\231' */  '\232',  /* '\232' */  '\233',  /* '\233' */
  '\234',  /* '\234' */  '\235',  /* '\235' */  '\236',  /* '\236' */  '\237',  /* '\237' */
  '\240',  /* '\240' */  '¡',  /* '¡' */  '¢',  /* '¢' */  '£',  /* '£' */
  '¤',  /* '¤' */  '¥',  /* '¥' */  '¦',  /* '¦' */  '§',  /* '§' */
  '¨',  /* '¨' */  '©',  /* '©' */  'ª',  /* 'ª' */  '«',  /* '«' */
  '¬',  /* '¬' */  '­',  /* '­' */  '®',  /* '®' */  '¯',  /* '¯' */
  '°',  /* '°' */  '±',  /* '±' */  '²',  /* '²' */  '³',  /* '³' */
  '´',  /* '´' */  'µ',  /* 'µ' */  '¶',  /* '¶' */  '·',  /* '·' */
  '¸',  /* '¸' */  '¹',  /* '¹' */  'º',  /* 'º' */  '»',  /* '»' */
  '¼',  /* '¼' */  '½',  /* '½' */  '¾',  /* '¾' */  '¿',  /* '¿' */
  'À',  /* 'À' */  'Á',  /* 'Á' */  'Â',  /* 'Â' */  'Ã',  /* 'Ã' */
  'Ä',  /* 'Ä' */  'Å',  /* 'Å' */  'Æ',  /* 'Æ' */  'Ç',  /* 'Ç' */
  'È',  /* 'È' */  'É',  /* 'É' */  'Ê',  /* 'Ê' */  'Ë',  /* 'Ë' */
  'Ì',  /* 'Ì' */  'Í',  /* 'Í' */  'Î',  /* 'Î' */  'Ï',  /* 'Ï' */
  'Ð',  /* 'Ð' */  'Ñ',  /* 'Ñ' */  'Ò',  /* 'Ò' */  'Ó',  /* 'Ó' */
  'Ô',  /* 'Ô' */  'Õ',  /* 'Õ' */  'Ö',  /* 'Ö' */  '×',  /* '×' */
  'Ø',  /* 'Ø' */  'Ù',  /* 'Ù' */  'Ú',  /* 'Ú' */  'Û',  /* 'Û' */
  'Ü',  /* 'Ü' */  'Ý',  /* 'Ý' */  'Þ',  /* 'Þ' */  'ß',  /* 'ß' */
  'à',  /* 'à' */  'á',  /* 'á' */  'â',  /* 'â' */  'ã',  /* 'ã' */
  'ä',  /* 'ä' */  'å',  /* 'å' */  'æ',  /* 'æ' */  'ç',  /* 'ç' */
  'è',  /* 'è' */  'é',  /* 'é' */  'ê',  /* 'ê' */  'ë',  /* 'ë' */
  'ì',  /* 'ì' */  'í',  /* 'í' */  'î',  /* 'î' */  'ï',  /* 'ï' */
  'ð',  /* 'ð' */  'ñ',  /* 'ñ' */  'ò',  /* 'ò' */  'ó',  /* 'ó' */
  'ô',  /* 'ô' */  'õ',  /* 'õ' */  'ö',  /* 'ö' */  '÷',  /* '÷' */
  'ø',  /* 'ø' */  'ù',  /* 'ù' */  'ú',  /* 'ú' */  'û',  /* 'û' */
  'ü',  /* 'ü' */  'ý',  /* 'ý' */  'þ',  /* 'þ' */  '\377',  /* '\377' */
  };

//---------------------------------------------------------------------------

int StrCmp
(
const void * pv1,
const void * pv2
)
  {
  return strcmp (apc_Str [* (const t_Int *) pv1], apc_Str [* (const t_Int *) pv2]);
  }

//---------------------------------------------------------------------------

int StrICmp
(
const void * pv1,
const void * pv2
)
  {
  #if defined TL_UNIX
    return strcmp (apc_Str [* (const t_Int *) pv1], apc_Str [* (const t_Int *) pv2]);
  #else
    return _stricmp (apc_Str [* (const t_Int *) pv1], apc_Str [* (const t_Int *) pv2]);
  #endif
  }

//---------------------------------------------------------------------------

int IntCmp
(
const void * pv1,
const void * pv2
)
  {
  return (int) (* (const t_UInt32 *) pv1 - * (const t_UInt32 *) pv2);
  }

//---------------------------------------------------------------------------

int main ()
  {
  t_Int i;
  char ac [52];
  srand (1);

  for (i = 0; i < i_TestSize / 40; i ++)
    {
    ac [0] = '\n';
    ac [1] = '\0';

    for (t_Int j = 0; j < 40; j ++)
      {
      tl_MoveMemory (ac + 1, ac, 51);
      ac [0] = (char) (rand () % 52);

      if (ac [0] < 26)
        ac [0] += 'A';
      else
        ac [0] += 'a' - 26;

      apc_Str [i * 40 + j] = new char [tl_StringLength (ac) + 1];
      tl_CopyMemory (apc_Str [i * 40 + j], ac, tl_StringLength (ac) + 1);
      }
    }

  for (i = 0; i < i_TestSize; i ++)
    {
    au_Int1 [i] =
    au_Int2 [i] = rand ();
    }

//  for (i = 0; i < i_TestSize; i ++)
//    printf ("%s", apc_Str [i]);

  for (i = 0; i < i_TestSize; i ++)
    ai_Res [i] = i;

  t_MicroTime i_time = tl_QueryPrecisionTime ();
  qsort (ai_Res, i_TestSize, sizeof (t_Int), StrCmp);
  printf ("StrCmp %d\n", (int) ((tl_QueryPrecisionTime () - i_time) / co_MilliSecondFactor));

  for (i = 0; i < i_TestSize; i ++)
    ai_Res [i] = i;

  i_time = tl_QueryPrecisionTime ();
  qsort (ai_Res, i_TestSize, sizeof (t_Int), StrICmp);
  printf ("StrICmp %d\n", (int) ((tl_QueryPrecisionTime () - i_time) / co_MilliSecondFactor));

  i_time = tl_QueryPrecisionTime ();
  ct_StringSort co_stringSort;
  co_stringSort. Sort ((const char * *) apc_Str, ai_Res, i_TestSize);
  printf ("StringSort %d\n", (int) ((tl_QueryPrecisionTime () - i_time) / co_MilliSecondFactor));

  i_time = tl_QueryPrecisionTime ();
  qsort (au_Int1, i_TestSize, sizeof (t_UInt32), IntCmp);
  printf ("IntCmp %d\n", (int) ((tl_QueryPrecisionTime () - i_time) / co_MilliSecondFactor));

  i_time = tl_QueryPrecisionTime ();
  ct_UInt32Sort co_intSort;
  co_intSort. Sort (au_Int2, ai_Res, i_TestSize);
  printf ("IntSort %d\n", (int) ((tl_QueryPrecisionTime () - i_time) / co_MilliSecondFactor));

  for (i = 0; i < i_TestSize; i ++)
    {
    if (au_Int1 [i] != au_Int2 [ai_Res [i]])
      printf ("Error: %d %d\n", au_Int1 [i], au_Int2 [ai_Res [i]]);
    }

  t_UInt32 au_ints [16] =
    {
    0x00000003,
    0x00000002,
    0x00000001,
    0x00000204,
    0x00000307,
    0x00000105,
    0x00000000,
    0x00000001,
    0x00030009,
    0x0001000a,
    0x00030009,
    0x0002000f,
    0x00030008,
    0x0200000c,
    0x01000002,
    0x03000001,
    };
  t_Int ai_intRes [16];
  co_intSort. Sort (au_ints, ai_intRes, 16);

  for (i = 0; i < 16; i ++)
    printf ("%3d %08x\n", (int) ai_intRes [i], au_ints [ai_intRes [i]]);

  return 0;
  }
