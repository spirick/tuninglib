
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
// File tuning/uuid.hpp

#ifndef TUNING_UUID_HPP
#define TUNING_UUID_HPP

#include "tuning/defs.hpp"

//---------------------------------------------------------------------------

typedef t_UInt8 t_UUID [16];

class TL_EXPORT ct_UUID
  {
  t_UUID               ao_UUID;

public:
                       ct_UUID ();
                       ct_UUID (const ct_UUID & co_init);
                       ct_UUID (const t_UUID & ao_init);
  ct_UUID &            operator = (const ct_UUID & co_asgn);

  bool                 IsEmpty () const;
  t_UInt               GetHash () const;
  const t_UUID &       GetUUID () const;
  void                 Clear ();
  bool                 Create ();
  bool                 ToStr (char * pc_dst, t_UInt u_len, bool b_upperCase) const;
  bool                 FromStr (const char * pc_src, t_UInt u_len);

  bool                 operator == (const ct_UUID & co_comp) const;
  bool                 operator != (const ct_UUID & co_comp) const;
  };

#endif
