
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
// File tuning/md5.cpp

#include "tuning/md5.hpp"
#include "tuning/sys/cmemory.hpp"

//---------------------------------------------------------------------------

static inline t_UInt32 RotLeft
(
t_UInt32 u_value,
unsigned u_rot
)
  {
  return (u_value << u_rot) | (u_value >> (32 - u_rot));
  }

static inline void StepF
(
t_UInt32 & u_a,
t_UInt32 u_b, t_UInt32 u_c, t_UInt32 u_d,
t_UInt32 u_bf, unsigned u_hex, unsigned u_rot
)
  {
  u_a = RotLeft (u_a + (u_d ^ (u_b & (u_c ^ u_d))) + u_bf + u_hex, u_rot) + u_b;
  }

static inline void StepG
(
t_UInt32 & u_a,
t_UInt32 u_b, t_UInt32 u_c, t_UInt32 u_d,
t_UInt32 u_bf, unsigned u_hex, unsigned u_rot
)
  {
  u_a = RotLeft (u_a + (u_c ^ (u_d & (u_b ^ u_c))) + u_bf + u_hex, u_rot) + u_b;
  }

static inline void StepH
(
t_UInt32 & u_a,
t_UInt32 u_b, t_UInt32 u_c, t_UInt32 u_d,
t_UInt32 u_bf, unsigned u_hex, unsigned u_rot
)
  {
  u_a = RotLeft (u_a + (u_b ^ u_c ^ u_d)           + u_bf + u_hex, u_rot) + u_b;
  }

static inline void StepI
(
t_UInt32 & u_a,
t_UInt32 u_b, t_UInt32 u_c, t_UInt32 u_d,
t_UInt32 u_bf, unsigned u_hex, unsigned u_rot
)
  {
  u_a = RotLeft (u_a + (u_c ^ (u_b | ~u_d))        + u_bf + u_hex, u_rot) + u_b;
  }

//---------------------------------------------------------------------------

void ct_MD5::Init ()
  {
  union
    {
    t_UInt32             u_int32;
    t_UInt8              ac_int8 [4];
    } uo_test;

  uo_test. u_int32 = 1;
  b_LittleEndian = (uo_test. ac_int8 [0] == 1);
  b_Finalized = false;
  b_ResultStr = false;
  u_ByteCount = 0;
  u_A = 0x67452301;
  u_B = 0xefcdab89;
  u_C = 0x98badcfe;
  u_D = 0x10325476;
  tl_FillMemory ((char *) ac_Result, sizeof (ac_Result), 0);
  }

//---------------------------------------------------------------------------

void ct_MD5::CalcBuffer
(
const t_UInt8 * pc_buffer
)
  {
  t_UInt32 u_a = u_A;
  t_UInt32 u_b = u_B;
  t_UInt32 u_c = u_C;
  t_UInt32 u_d = u_D;
  t_UInt32 au_buffer [16];

  if (b_LittleEndian)
    tl_CopyMemory ((char *) au_buffer, (const char *) pc_buffer, 64);
  else
    {
    t_UInt32 * pu_buffer = au_buffer;

    for (unsigned u_src = 0; u_src < 64; u_src += 4)
      {
      * (pu_buffer ++) = pc_buffer [u_src] + (pc_buffer [u_src + 1] << 8) +
        (pc_buffer [u_src + 2] << 16) + (pc_buffer [u_src + 3] << 24);
      }
    }

  // Round 1
  StepF (u_a, u_b, u_c, u_d, au_buffer [ 0], 0xd76aa478,  7);
  StepF (u_d, u_a, u_b, u_c, au_buffer [ 1], 0xe8c7b756, 12);
  StepF (u_c, u_d, u_a, u_b, au_buffer [ 2], 0x242070db, 17);
  StepF (u_b, u_c, u_d, u_a, au_buffer [ 3], 0xc1bdceee, 22);
  StepF (u_a, u_b, u_c, u_d, au_buffer [ 4], 0xf57c0faf,  7);
  StepF (u_d, u_a, u_b, u_c, au_buffer [ 5], 0x4787c62a, 12);
  StepF (u_c, u_d, u_a, u_b, au_buffer [ 6], 0xa8304613, 17);
  StepF (u_b, u_c, u_d, u_a, au_buffer [ 7], 0xfd469501, 22);
  StepF (u_a, u_b, u_c, u_d, au_buffer [ 8], 0x698098d8,  7);
  StepF (u_d, u_a, u_b, u_c, au_buffer [ 9], 0x8b44f7af, 12);
  StepF (u_c, u_d, u_a, u_b, au_buffer [10], 0xffff5bb1, 17);
  StepF (u_b, u_c, u_d, u_a, au_buffer [11], 0x895cd7be, 22);
  StepF (u_a, u_b, u_c, u_d, au_buffer [12], 0x6b901122,  7);
  StepF (u_d, u_a, u_b, u_c, au_buffer [13], 0xfd987193, 12);
  StepF (u_c, u_d, u_a, u_b, au_buffer [14], 0xa679438e, 17);
  StepF (u_b, u_c, u_d, u_a, au_buffer [15], 0x49b40821, 22);

  // Round 2
  StepG (u_a, u_b, u_c, u_d, au_buffer [ 1], 0xf61e2562,  5);
  StepG (u_d, u_a, u_b, u_c, au_buffer [ 6], 0xc040b340,  9);
  StepG (u_c, u_d, u_a, u_b, au_buffer [11], 0x265e5a51, 14);
  StepG (u_b, u_c, u_d, u_a, au_buffer [ 0], 0xe9b6c7aa, 20);
  StepG (u_a, u_b, u_c, u_d, au_buffer [ 5], 0xd62f105d,  5);
  StepG (u_d, u_a, u_b, u_c, au_buffer [10], 0x02441453,  9);
  StepG (u_c, u_d, u_a, u_b, au_buffer [15], 0xd8a1e681, 14);
  StepG (u_b, u_c, u_d, u_a, au_buffer [ 4], 0xe7d3fbc8, 20);
  StepG (u_a, u_b, u_c, u_d, au_buffer [ 9], 0x21e1cde6,  5);
  StepG (u_d, u_a, u_b, u_c, au_buffer [14], 0xc33707d6,  9);
  StepG (u_c, u_d, u_a, u_b, au_buffer [ 3], 0xf4d50d87, 14);
  StepG (u_b, u_c, u_d, u_a, au_buffer [ 8], 0x455a14ed, 20);
  StepG (u_a, u_b, u_c, u_d, au_buffer [13], 0xa9e3e905,  5);
  StepG (u_d, u_a, u_b, u_c, au_buffer [ 2], 0xfcefa3f8,  9);
  StepG (u_c, u_d, u_a, u_b, au_buffer [ 7], 0x676f02d9, 14);
  StepG (u_b, u_c, u_d, u_a, au_buffer [12], 0x8d2a4c8a, 20);

  // Round 3
  StepH (u_a, u_b, u_c, u_d, au_buffer [ 5], 0xfffa3942,  4);
  StepH (u_d, u_a, u_b, u_c, au_buffer [ 8], 0x8771f681, 11);
  StepH (u_c, u_d, u_a, u_b, au_buffer [11], 0x6d9d6122, 16);
  StepH (u_b, u_c, u_d, u_a, au_buffer [14], 0xfde5380c, 23);
  StepH (u_a, u_b, u_c, u_d, au_buffer [ 1], 0xa4beea44,  4);
  StepH (u_d, u_a, u_b, u_c, au_buffer [ 4], 0x4bdecfa9, 11);
  StepH (u_c, u_d, u_a, u_b, au_buffer [ 7], 0xf6bb4b60, 16);
  StepH (u_b, u_c, u_d, u_a, au_buffer [10], 0xbebfbc70, 23);
  StepH (u_a, u_b, u_c, u_d, au_buffer [13], 0x289b7ec6,  4);
  StepH (u_d, u_a, u_b, u_c, au_buffer [ 0], 0xeaa127fa, 11);
  StepH (u_c, u_d, u_a, u_b, au_buffer [ 3], 0xd4ef3085, 16);
  StepH (u_b, u_c, u_d, u_a, au_buffer [ 6], 0x04881d05, 23);
  StepH (u_a, u_b, u_c, u_d, au_buffer [ 9], 0xd9d4d039,  4);
  StepH (u_d, u_a, u_b, u_c, au_buffer [12], 0xe6db99e5, 11);
  StepH (u_c, u_d, u_a, u_b, au_buffer [15], 0x1fa27cf8, 16);
  StepH (u_b, u_c, u_d, u_a, au_buffer [ 2], 0xc4ac5665, 23);

  // Round 4
  StepI (u_a, u_b, u_c, u_d, au_buffer [ 0], 0xf4292244,  6);
  StepI (u_d, u_a, u_b, u_c, au_buffer [ 7], 0x432aff97, 10);
  StepI (u_c, u_d, u_a, u_b, au_buffer [14], 0xab9423a7, 15);
  StepI (u_b, u_c, u_d, u_a, au_buffer [ 5], 0xfc93a039, 21);
  StepI (u_a, u_b, u_c, u_d, au_buffer [12], 0x655b59c3,  6);
  StepI (u_d, u_a, u_b, u_c, au_buffer [ 3], 0x8f0ccc92, 10);
  StepI (u_c, u_d, u_a, u_b, au_buffer [10], 0xffeff47d, 15);
  StepI (u_b, u_c, u_d, u_a, au_buffer [ 1], 0x85845dd1, 21);
  StepI (u_a, u_b, u_c, u_d, au_buffer [ 8], 0x6fa87e4f,  6);
  StepI (u_d, u_a, u_b, u_c, au_buffer [15], 0xfe2ce6e0, 10);
  StepI (u_c, u_d, u_a, u_b, au_buffer [ 6], 0xa3014314, 15);
  StepI (u_b, u_c, u_d, u_a, au_buffer [13], 0x4e0811a1, 21);
  StepI (u_a, u_b, u_c, u_d, au_buffer [ 4], 0xf7537e82,  6);
  StepI (u_d, u_a, u_b, u_c, au_buffer [11], 0xbd3af235, 10);
  StepI (u_c, u_d, u_a, u_b, au_buffer [ 2], 0x2ad7d2bb, 15);
  StepI (u_b, u_c, u_d, u_a, au_buffer [ 9], 0xeb86d391, 21);

  u_A += u_a;
  u_B += u_b;
  u_C += u_c;
  u_D += u_d;
  }

//---------------------------------------------------------------------------

ct_MD5::ct_MD5 ()
  {
  Init ();
  }

//---------------------------------------------------------------------------

ct_MD5::ct_MD5
(
const t_MD5Result & ac_init
)
  {
  b_Finalized = true;
  b_ResultStr = false;
  tl_CopyMemory ((char *) ac_Result, (const char *) ac_init, sizeof (ac_Result));
  }

//---------------------------------------------------------------------------

ct_MD5::ct_MD5
(
const void * pv_data,
t_UInt u_len
)
  {
  Init ();
  Update (pv_data, u_len);
  Finalize ();
  }

//---------------------------------------------------------------------------

void ct_MD5::Update
(
const void * pv_data,
t_UInt u_len
)
  {
  if (b_Finalized || (u_len == 0))
    return;

  const t_UInt8 * pc_data = (const t_UInt8 *) pv_data;
  t_UInt u_bfPos = ((t_UInt) u_ByteCount) & 0x3f;
  u_ByteCount += u_len;
  t_UInt u_bfFree = 64 - u_bfPos;

  if (u_bfFree <= u_len)
    {
    tl_CopyMemory ((char *) ac_Buffer + u_bfPos, (const char *) pc_data, u_bfFree);
    CalcBuffer (ac_Buffer);
    const t_UInt8 * pc_end = pc_data + u_len - 64;
    pc_data += u_bfFree;

    while (pc_data <= pc_end)
      {
      CalcBuffer (pc_data);
      pc_data += 64;
      }

    tl_CopyMemory ((char *) ac_Buffer, (const char *) pc_data, (pc_end + 64) - pc_data);
    }
  else
    tl_CopyMemory ((char *) ac_Buffer + u_bfPos, (const char *) pc_data, u_len);
  }

//---------------------------------------------------------------------------

static inline void UIntToChar
(
t_UInt8 * pc_dst,
t_UInt32 u_src
)
  {
  * (pc_dst ++) = (t_UInt8) u_src;
  * (pc_dst ++) = (t_UInt8) (u_src >> 8);
  * (pc_dst ++) = (t_UInt8) (u_src >> 16);
  * (pc_dst ++) = (t_UInt8) (u_src >> 24);
  }

//---------------------------------------------------------------------------

void ct_MD5::Finalize ()
  {
  if (! b_Finalized)
    {
    t_UInt32 u_bitCountLo = (t_UInt32) (u_ByteCount << 3);
    t_UInt32 u_bitCountHi = (t_UInt32) (u_ByteCount >> 29);
    t_UInt u_bfPos = ((t_UInt) u_ByteCount) & 0x3f;
    ac_Buffer [u_bfPos ++] = 0x80;
    t_UInt u_bfFree = 64 - u_bfPos;

    if (u_bfFree < 8)
      {
      tl_FillMemory ((char *) ac_Buffer + u_bfPos, u_bfFree, 0);
      CalcBuffer (ac_Buffer);
      u_bfPos = 0;
      u_bfFree = 64;
      }

    tl_FillMemory ((char *) ac_Buffer + u_bfPos, u_bfFree - 8, 0);
    UIntToChar (ac_Buffer + 56, u_bitCountLo);
    UIntToChar (ac_Buffer + 60, u_bitCountHi);
    CalcBuffer (ac_Buffer);
    UIntToChar (ac_Result +  0, u_A);
    UIntToChar (ac_Result +  4, u_B);
    UIntToChar (ac_Result +  8, u_C);
    UIntToChar (ac_Result + 12, u_D);
    u_ByteCount = 0;
    tl_FillMemory ((char *) ac_Buffer, 64, 0);
    b_Finalized = true;
    }
  }

//---------------------------------------------------------------------------

const t_MD5Result & ct_MD5::GetResult () const
  {
  return ac_Result;
  }

//---------------------------------------------------------------------------

static inline char UToHex
(
unsigned u
)
  {
  if (u < 10)
    return (char) (u + '0');
  else
    return (char) (u - 10 + 'a');
  }

//---------------------------------------------------------------------------

const char * ct_MD5::GetResultStr ()
  {
  if (! b_Finalized)
    return 0;

  if (! b_ResultStr)
    {
    for (unsigned u = 0; u < 16; u ++)
      {
      ac_ResultStr [2 * u    ] = UToHex (ac_Result [u] >> 4);
      ac_ResultStr [2 * u + 1] = UToHex (ac_Result [u] & 0xf);
      }

    ac_ResultStr [32] = 0;
    b_ResultStr = true;
    }

  return ac_ResultStr;
  }

//---------------------------------------------------------------------------

bool ct_MD5::operator ==
(
const ct_MD5 & co_comp
) const
  {
  return tl_CompareMemory ((const char *) ac_Result,
    (const char *) co_comp. ac_Result, sizeof (ac_Result)) == 0;
  }
