
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
// File tuning/key.h

#ifndef TUNING_KEY_H
#define TUNING_KEY_H

#include "tuning/defs.hpp"

//---------------------------------------------------------------------------

template <class t_key>
  class TL_TM_EXPORT gct_Key
    {
  public:
    t_key                o_Key;

                         gct_Key ()
                           { TL_ASSERT (false);}
                         gct_Key (const gct_Key & co_copy):
                           o_Key (co_copy. o_Key) { }
                         gct_Key (t_key o_key):
                           o_Key (o_key) { }
    bool                 operator == (const gct_Key & co_comp) const
                           { return o_Key == co_comp. o_Key; }
    bool                 operator < (const gct_Key & co_comp) const
                           { return o_Key < co_comp. o_Key; }
    t_UInt               GetHash () const
                           { return (t_UInt) o_Key; }
    };

#endif
