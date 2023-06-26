
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
// File tuning/md5.hpp

#ifndef TUNING_MD5_HPP
#define TUNING_MD5_HPP

#include "tuning/defs.hpp"

//---------------------------------------------------------------------------

typedef t_UInt8 t_MD5Result [16];

class TL_EXPORT ct_MD5
  {
  bool                 b_LittleEndian;
  bool                 b_Finalized;
  bool                 b_ResultStr;
  t_UInt64             u_ByteCount;
  t_UInt32             u_A, u_B, u_C, u_D;
  t_UInt8              ac_Buffer [64];
  t_MD5Result          ac_Result;
  char                 ac_ResultStr [33];

  // Do not copy this object
                       ct_MD5 (const ct_MD5 &);
  ct_MD5 &             operator = (const ct_MD5 &);

  void                 Init ();
  void                 CalcBuffer (const t_UInt8 * pc_buffer);

public:
                       ct_MD5 ();
                       ct_MD5 (const t_MD5Result & ac_init);
                       ct_MD5 (const void * pv_data, t_UInt u_len);

  void                 Update (const void * pv_data, t_UInt u_len);
  void                 Finalize ();
  const t_MD5Result &  GetResult () const;
  const char *         GetResultStr ();
  bool                 operator == (const ct_MD5 & co_comp) const;
  };

#endif
