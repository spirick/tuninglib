
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
// File tuning/sys/csharedres.hpp

#ifndef TUNING_SYS_CSHAREDRES_HPP
#define TUNING_SYS_CSHAREDRES_HPP

#include "tuning/filename.hpp"
#include "tuning/sys/creserror.hpp"

//---------------------------------------------------------------------------
// Shared resource

class TL_EXPORT ct_SharedResource
  {
protected:
  bool                 b_InitSuccess;
  ct_String            co_Key;

  #if defined TL_WINDOWS

    void *               pv_Id;

  #elif defined TL_UNIX

    ct_FileName          co_TempName;
    int                  o_FileId;
    int                  i_Id;

    void                 CloseDeleteFile ();
    et_ResError          OpenKey (int & i_key);
    et_ResError          CreateKey (int & i_key, bool b_createNew);
    et_ResError          OpenSem ();
    et_ResError          CreateSem (t_Int32 i_initValue, bool b_createNew);
    et_ResError          CloseSem ();

  #endif

private:
  // Do not copy this object
                       ct_SharedResource (const ct_SharedResource &);
  ct_SharedResource &  operator = (const ct_SharedResource &);

public:
                       ct_SharedResource ();
                       ct_SharedResource (const char * pc_key);
                       ct_SharedResource (const char * pc_key, unsigned u_idx);
  virtual              ~ct_SharedResource () { }

  bool                 GetInitSuccess () const;
  const char *         GetKey () const;
  et_ResError          SetKey (const char * pc_key);
  et_ResError          SetKey (const char * pc_key, unsigned u_idx);
  };

#endif
