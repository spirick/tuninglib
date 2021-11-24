
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
// File tuning/file.cpp

#include "tuning/file.hpp"
#include "tuning/dirscan.hpp"
#include "tuning/sys/cprocess.hpp"

//---------------------------------------------------------------------------

ct_File::ct_File ()
  {
  o_FileId = co_InvalidFileId;
  }

//---------------------------------------------------------------------------

ct_File::ct_File
(
const char * pc_init
)
  {
  AssignAsName (pc_init);
  o_FileId = co_InvalidFileId;
  }

//---------------------------------------------------------------------------

ct_File::ct_File
(
const ct_FileName & co_init
): ct_FileName (co_init)
  {
  o_FileId = co_InvalidFileId;
  }

//---------------------------------------------------------------------------

ct_File::~ct_File ()
  {
  Close ();
  }

//---------------------------------------------------------------------------

ct_File & ct_File::operator =
(
const char * pc_asgn
)
  {
  if (o_FileId == co_InvalidFileId)
    AssignAsName (pc_asgn);

  return * this;
  }

//---------------------------------------------------------------------------

ct_File & ct_File::operator =
(
const ct_FileName & co_asgn
)
  {
  if (o_FileId == co_InvalidFileId)
    ct_FileName::operator = (co_asgn);

  return * this;
  }

//---------------------------------------------------------------------------

TL_ISKINDOF (ct_File, ct_FileName)

//---------------------------------------------------------------------------

bool ct_File::TryOpen
(
bool b_readOnly,
bool b_sequential,
t_UInt32 u_milliSec
)
  {
  t_MicroTime i_time = tl_QueryPrecisionTime ();

  for (;;)
    {
    if (tl_OpenFile (GetAllStr (), o_FileId, b_readOnly, b_sequential))
      return true;

    int i_diff = (int) ((tl_QueryPrecisionTime () - i_time) / co_MilliSecondFactor);

    if (u_milliSec < (t_UInt32) i_diff)
      return false;

    tl_Delay (1);
    }
  }

//---------------------------------------------------------------------------

bool ct_File::Open
(
bool b_readOnly,
bool b_sequential
)
  {
  return tl_OpenFile (GetAllStr (), o_FileId, b_readOnly, b_sequential);
  }

//---------------------------------------------------------------------------

bool ct_File::Create
(
bool b_createNew
)
  {
  return tl_CreateFile (GetAllStr (), o_FileId, b_createNew);
  }

//---------------------------------------------------------------------------

bool ct_File::Close ()
  {
  return tl_CloseFile (o_FileId);
  }

//---------------------------------------------------------------------------

bool ct_File::Load
(
ct_String * pco_str
)
  {
  if (Open ())
    {
    t_FileSize o_size;

    if (QuerySize (o_size) &&
        (o_size < (t_FileSize) ct_String::t_StaticStore::MaxAlloc ()))
      {
      pco_str-> Assign (' ', (ct_String::t_Size) o_size);

      if (Read ((char *) pco_str-> GetStr (), o_size))
        return Close ();
      }

    Close ();
    }

  return false;
  }

//---------------------------------------------------------------------------

bool ct_File::Save
(
const ct_String * pco_str
)
  {
  if (Create ())
    {
    if (Write (pco_str-> GetStr (), pco_str-> GetLen ()))
      return Close ();

    Close ();
    }

  return false;
  }

//---------------------------------------------------------------------------

bool ct_File::Exists ()
  {
  if (o_FileId == co_InvalidFileId)
    return tl_ExistsFile (GetAllStr ());
  else
    return true;
  }

//---------------------------------------------------------------------------

bool ct_File::Move
(
const char * pc_new
)
  {
  if ((o_FileId == co_InvalidFileId) &&
      tl_MoveFile (GetAllStr (), pc_new))
    {
    AssignAsName (pc_new);
    return true;
    }
  else
    return false;
  }

//---------------------------------------------------------------------------

bool ct_File::Copy
(
const char * pc_new,
bool b_overwrite
)
  {
  if (o_FileId == co_InvalidFileId)
    return tl_CopyFile (GetAllStr (), pc_new, b_overwrite);
  else
    return false;
  }

//---------------------------------------------------------------------------

bool ct_File::Delete ()
  {
  if (o_FileId == co_InvalidFileId)
    return tl_DeleteFile (GetAllStr ());
  else
    return false;
  }

//---------------------------------------------------------------------------

bool ct_File::QuerySize
(
t_FileSize & o_size
) const
  {
  return tl_QuerySize (o_FileId, o_size);
  }

//---------------------------------------------------------------------------

bool ct_File::QueryPos
(
t_FileSize & o_pos
) const
  {
  return tl_QueryPos (o_FileId, o_pos);
  }

//---------------------------------------------------------------------------

bool ct_File::EndOfFile
(
bool & b_eof
) const
  {
  t_FileSize o_pos, o_size;

  if (tl_QueryPos (o_FileId, o_pos) && tl_QuerySize (o_FileId, o_size))
    {
    b_eof = (o_pos == o_size);
    return true;
    }
  else
    return false;
  }

//---------------------------------------------------------------------------

bool ct_File::SeekAbs
(
t_FileSize o_pos
) const
  {
  return tl_SeekAbs (o_FileId, o_pos);
  }

//---------------------------------------------------------------------------

bool ct_File::SeekRel
(
t_FileSize o_pos
) const
  {
  return tl_SeekRel (o_FileId, o_pos);
  }

//---------------------------------------------------------------------------

bool ct_File::Truncate
(
t_FileSize o_size
) const
  {
  return tl_Truncate (o_FileId, o_size);
  }

//---------------------------------------------------------------------------

bool ct_File::Read
(
void * pv_dst,
t_FileSize o_len
) const
  {
  return tl_Read (o_FileId, pv_dst, o_len);
  }

//---------------------------------------------------------------------------

bool ct_File::Write
(
const void * pv_src,
t_FileSize o_len
) const
  {
  return tl_Write (o_FileId, pv_src, o_len);
  }
