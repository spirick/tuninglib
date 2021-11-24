
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
// File tuning/sys/cfile.hpp

#ifndef TUNING_SYS_CFILE_HPP
#define TUNING_SYS_CFILE_HPP

#include "tuning/defs.hpp"

//---------------------------------------------------------------------------

#if defined TL_WINDOWS
  typedef void *       t_FileId;
  const t_FileId       co_InvalidFileId = 0;
#elif defined TL_UNIX
  typedef int          t_FileId;
  const t_FileId       co_InvalidFileId = -1;
#endif

typedef t_Int64      t_FileSize;

//---------------------------------------------------------------------------

bool TL_EXPORT       tl_OpenFile (const char * pc_name, t_FileId & o_file,
                       bool b_readOnly = true, bool b_sequential = true);
bool TL_EXPORT       tl_CreateFile (const char * pc_name, t_FileId & o_file,
                       bool b_createNew = false);
bool TL_EXPORT       tl_CloseFile (t_FileId & o_file);

bool TL_EXPORT       tl_ExistsFile (const char * pc_name);
bool TL_EXPORT       tl_MoveFile (const char * pc_old, const char * pc_new);
bool TL_EXPORT       tl_CopyFile (const char * pc_old, const char * pc_new,
                       bool b_overwrite = true);
bool TL_EXPORT       tl_DeleteFile (const char * pc_name);

bool TL_EXPORT       tl_QuerySize (t_FileId o_file, t_FileSize & o_size);
bool TL_EXPORT       tl_QueryPos (t_FileId o_file, t_FileSize & o_pos);

bool TL_EXPORT       tl_SeekAbs (t_FileId o_file, t_FileSize o_pos);
bool TL_EXPORT       tl_SeekRel (t_FileId o_file, t_FileSize o_pos);
bool TL_EXPORT       tl_Truncate (t_FileId o_file, t_FileSize o_size);
bool TL_EXPORT       tl_Read (t_FileId o_file,
                       void * pv_dst, t_FileSize o_len);
bool TL_EXPORT       tl_Write (t_FileId o_file,
                       const void * pv_src, t_FileSize o_len);

#endif
