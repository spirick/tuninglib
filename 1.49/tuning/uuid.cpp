
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
// File tuning/uuid.cpp

#include "tuning/uuid.hpp"
#include "tuning/sys/cmemory.hpp"

//---------------------------------------------------------------------------

ct_UUID::ct_UUID ()
  {
  Clear ();
  }

//---------------------------------------------------------------------------

ct_UUID::ct_UUID
(
const ct_UUID & co_init
)
  {
  tl_CopyMemory ((char *) ao_UUID, (const char *) co_init. ao_UUID, sizeof (ao_UUID));
  }

//---------------------------------------------------------------------------

ct_UUID::ct_UUID
(
const t_UUID & ao_init
)
  {
  tl_CopyMemory ((char *) ao_UUID, (const char *) ao_init, sizeof (ao_UUID));
  }

//---------------------------------------------------------------------------

ct_UUID & ct_UUID::operator =
(
const ct_UUID & co_asgn
)
  {
  if (& co_asgn != this)
    tl_CopyMemory ((char *) ao_UUID, (const char *) co_asgn. ao_UUID, sizeof (ao_UUID));

  return * this;
  }

//---------------------------------------------------------------------------

bool ct_UUID::IsEmpty () const
  {
  t_UUID ao_zero = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  return tl_CompareMemory ((const char *) ao_UUID,
    (const char *) ao_zero, sizeof (ao_UUID)) == 0;
  }

//---------------------------------------------------------------------------

t_UInt ct_UUID::GetHash () const
  {
  t_UInt32 au_data32 [4];
  tl_CopyMemory ((char *) au_data32, (const char *) ao_UUID, sizeof (ao_UUID));
  return au_data32 [0] + au_data32 [1] + au_data32 [2] + au_data32 [3]; 
  }

//---------------------------------------------------------------------------

const t_UUID & ct_UUID::GetUUID () const
  {
  return ao_UUID;
  }

//---------------------------------------------------------------------------

void ct_UUID::Clear ()
  {
  tl_FillMemory ((char *) ao_UUID, sizeof (ao_UUID), 0);
  }

//---------------------------------------------------------------------------

#if defined TL_WINDOWS

#include <Rpc.h>

bool ct_UUID::Create ()
  {
  UUID so_uuid;

  if (UuidCreate (& so_uuid) == RPC_S_OK)
    {
    ao_UUID [0] = (t_UInt8) (so_uuid. Data1 >> 24);
    ao_UUID [1] = (t_UInt8) (so_uuid. Data1 >> 16);
    ao_UUID [2] = (t_UInt8) (so_uuid. Data1 >> 8);
    ao_UUID [3] = (t_UInt8) (so_uuid. Data1);
    ao_UUID [4] = (t_UInt8) (so_uuid. Data2 >> 8);
    ao_UUID [5] = (t_UInt8) (so_uuid. Data2);
    ao_UUID [6] = (t_UInt8) (so_uuid. Data3 >> 8);
    ao_UUID [7] = (t_UInt8) (so_uuid. Data3);
    tl_CopyMemory ((char *) ao_UUID + 8, (const char *) so_uuid. Data4, 8);
    return true;
    }
  else
    return false;
  }

#else

#include <uuid/uuid.h>

bool ct_UUID::Create ()
  {
  uuid_generate (ao_UUID);
  return true;
  }

#endif

//---------------------------------------------------------------------------

static inline char UToHex
(
t_UInt8 u,
char c_a
)
  {
  u = u & 0xF;

  if (u < 10)
    return (char) (u + '0');
  else
    return (char) (u + c_a);
  }

//---------------------------------------------------------------------------

static void U8ToHex
(
t_UInt8 u,
char * & pc_dst,
char c_a
)
  {
  * (pc_dst ++) = UToHex (u >> 4, c_a);
  * (pc_dst ++) = UToHex (u, c_a);
  }

//---------------------------------------------------------------------------

static void UToHex
(
char * & pc_dst,
const t_UInt8 * & pu_src,
t_UInt u_len,
char c_a
)
  {
  for (t_UInt u = 0; u < u_len; u ++)
    U8ToHex (* (pu_src ++), pc_dst, c_a);
  }

//---------------------------------------------------------------------------

bool ct_UUID::ToStr
(
char * pc_dst,
t_UInt u_len,
bool b_upperCase
) const
  {
  if (u_len < 36)
    return false;

  char c_a = (b_upperCase ? 'A' : 'a') - 10;
  const t_UInt8 * pu_src = ao_UUID;
  UToHex (pc_dst, pu_src, 4, c_a);
  * (pc_dst ++) = '-';
  UToHex (pc_dst, pu_src, 2, c_a);
  * (pc_dst ++) = '-';
  UToHex (pc_dst, pu_src, 2, c_a);
  * (pc_dst ++) = '-';
  UToHex (pc_dst, pu_src, 2, c_a);
  * (pc_dst ++) = '-';
  UToHex (pc_dst, pu_src, 6, c_a);
  return true;
  }

//---------------------------------------------------------------------------

static inline t_UInt8 HexToU
(
char c
)
  {
  if (c >= 'a')
    return c - 'a' + 10;
  else
    {
    if (c >= 'A')
      return c - 'A' + 10;
    else
      return c - '0';
    }
  }

//---------------------------------------------------------------------------

static t_UInt8 HexToU8
(
const char * & pc_src
)
  {
  t_UInt8 u = HexToU (* (pc_src ++));
  return (u << 4) + HexToU (* (pc_src ++));
  }

//---------------------------------------------------------------------------

static void HexToU
(
const char * & pc_src,
t_UInt8 * & pu_dst,
t_UInt u_len
)
  {
  for (t_UInt u = 0; u < u_len; u ++)
    * (pu_dst ++) = HexToU8 (pc_src);
  }

//---------------------------------------------------------------------------

bool ct_UUID::FromStr
(
const char * pc_src,
t_UInt u_len
)
  {
  if (u_len < 36)
    return false;

  t_UInt8 * pu_dst = ao_UUID;
  HexToU (pc_src, pu_dst, 4);
  if (* (pc_src ++) != '-') return false;
  HexToU (pc_src, pu_dst, 2);
  if (* (pc_src ++) != '-') return false;
  HexToU (pc_src, pu_dst, 2);
  if (* (pc_src ++) != '-') return false;
  HexToU (pc_src, pu_dst, 2);
  if (* (pc_src ++) != '-') return false;
  HexToU (pc_src, pu_dst, 6);
  return true;
  }

//---------------------------------------------------------------------------

bool ct_UUID::operator ==
(
const ct_UUID & co_comp
) const
  {
  return tl_CompareMemory ((const char *) ao_UUID,
    (const char *) co_comp. ao_UUID, sizeof (ao_UUID)) == 0;
  }

//---------------------------------------------------------------------------

bool ct_UUID::operator !=
(
const ct_UUID & co_comp
) const
  {
  return tl_CompareMemory ((const char *) ao_UUID,
    (const char *) co_comp. ao_UUID, sizeof (ao_UUID)) != 0;
  }
