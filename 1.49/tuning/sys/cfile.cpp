
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
// File tuning/sys/cfile.cpp

#define _FILE_OFFSET_BITS 64
#include "tuning/sys/cfile.hpp"

//===========================================================================

#if defined TL_WINDOWS

#include <windows.h>
#include "tuning/wstring.hpp"

//---------------------------------------------------------------------------

bool TL_EXPORT tl_OpenFile
(
const char * pc_name,
t_FileId & o_file,
bool b_readOnly,
bool b_sequential
)
  {
  if ((o_file != co_InvalidFileId) ||
      (pc_name == 0) || (* pc_name == '\0'))
    return false;

  ct_WString co_name;

  if (! co_name. MbConvert (pc_name))
    return false;

  o_file = CreateFileW
    (
    co_name. GetStr (),
    b_readOnly ? GENERIC_READ : (GENERIC_READ | GENERIC_WRITE),
    b_readOnly ? FILE_SHARE_READ : 0,
    NULL,
    OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL |
    (b_sequential ? FILE_FLAG_SEQUENTIAL_SCAN : FILE_FLAG_RANDOM_ACCESS),
    NULL
    );

  if (o_file != INVALID_HANDLE_VALUE)
    return true;
  else
    {
    o_file = co_InvalidFileId;
    return false;
    }
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_CreateFile
(
const char * pc_name,
t_FileId & o_file,
bool b_createNew
)
  {
  if ((o_file != co_InvalidFileId) ||
      (pc_name == 0) || (* pc_name == '\0'))
    return false;

  ct_WString co_name;

  if (! co_name. MbConvert (pc_name))
    return false;

  o_file = CreateFileW
    (
    co_name. GetStr (),
    GENERIC_READ | GENERIC_WRITE,
    0,
    NULL,
    b_createNew ? CREATE_NEW : CREATE_ALWAYS,
    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS,
    NULL
    );

  if (o_file != INVALID_HANDLE_VALUE)
    return true;
  else
    {
    o_file = co_InvalidFileId;
    return false;
    }
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_CloseFile
(
t_FileId & o_file
)
  {
  if (o_file == co_InvalidFileId)
    return false;

  bool b = (CloseHandle (o_file) != 0);
  o_file = co_InvalidFileId;
  return b;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_ExistsFile
(
const char * pc_name
)
  {
  if ((pc_name == 0) || (* pc_name == '\0'))
    return false;

  ct_WString co_name;

  if (! co_name. MbConvert (pc_name))
    return false;

  t_FileId o_file = CreateFileW
    (
    co_name. GetStr (),
    FILE_READ_ATTRIBUTES,
    FILE_SHARE_READ | FILE_SHARE_WRITE,
    NULL,
    OPEN_EXISTING,
    0,
    NULL
    );

  if (o_file != INVALID_HANDLE_VALUE)
    {
    CloseHandle (o_file);
    return true;
    }
  else
    return false;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_MoveFile
(
const char * pc_old,
const char * pc_new
)
  {
  if ((pc_old == 0) || (* pc_old == '\0') ||
      (pc_new == 0) || (* pc_new == '\0'))
    return false;

  ct_WString co_old;
  ct_WString co_new;

  if (! co_old. MbConvert (pc_old))
    return false;

  if (! co_new. MbConvert (pc_new))
    return false;

  return MoveFileW (co_old. GetStr (), co_new. GetStr ()) != 0;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_CopyFile
(
const char * pc_old,
const char * pc_new,
bool b_overwrite
)
  {
  if ((pc_old == 0) || (* pc_old == '\0') ||
      (pc_new == 0) || (* pc_new == '\0'))
    return false;

  ct_WString co_old;
  ct_WString co_new;

  if (! co_old. MbConvert (pc_old))
    return false;

  if (! co_new. MbConvert (pc_new))
    return false;

  return CopyFileW (co_old. GetStr (), co_new. GetStr (), ! b_overwrite) != 0;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_DeleteFile
(
const char * pc_name
)
  {
  if ((pc_name == 0) || (* pc_name == '\0'))
    return false;

  ct_WString co_name;

  if (! co_name. MbConvert (pc_name))
    return false;

  return DeleteFileW (co_name. GetStr ()) != 0;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_QuerySize
(
t_FileId o_file,
t_FileSize & o_size
)
  {
  if (o_file == co_InvalidFileId)
    {
    o_size = 0;
    return false;
    }

  LARGE_INTEGER uo_size;

  if (GetFileSizeEx (o_file, & uo_size) != 0)
    {
    o_size = uo_size. QuadPart;
    return true;
    }
  else
    {
    o_size = 0;
    return false;
    }
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_QueryPos
(
t_FileId o_file,
t_FileSize & o_pos
)
  {
  if (o_file == co_InvalidFileId)
    {
    o_pos = 0;
    return false;
    }

  LARGE_INTEGER uo_move;
  LARGE_INTEGER uo_pos;
  uo_move. QuadPart = 0;

  if (SetFilePointerEx (o_file, uo_move, & uo_pos, FILE_CURRENT) != 0)
    {
    o_pos = uo_pos. QuadPart;
    return true;
    }
  else
    {
    o_pos = 0;
    return false;
    }
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_SeekAbs
(
t_FileId o_file,
t_FileSize o_pos
)
  {
  if (o_file == co_InvalidFileId)
    return false;

  LARGE_INTEGER uo_move;
  uo_move. QuadPart = o_pos;
  return SetFilePointerEx (o_file, uo_move, 0, FILE_BEGIN) != 0;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_SeekRel
(
t_FileId o_file,
t_FileSize o_pos
)
  {
  if (o_file == co_InvalidFileId)
    return false;

  LARGE_INTEGER uo_move;
  uo_move. QuadPart = o_pos;
  return SetFilePointerEx (o_file, uo_move, 0, FILE_CURRENT) != 0;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_Truncate
(
t_FileId o_file,
t_FileSize o_size
)
  {
  if (o_file == co_InvalidFileId)
    return false;

  LARGE_INTEGER uo_move;
  uo_move. QuadPart = o_size;
  return (SetFilePointerEx (o_file, uo_move, 0, FILE_BEGIN) != 0) &&
    SetEndOfFile (o_file);
  }

//---------------------------------------------------------------------------

static bool tl_Read_
(
t_FileId o_file,
void * pv_dst,
unsigned u_len
)
  {
  if (u_len == 0)
    return true;
  else
    {
    DWORD l_readLen;

    return ReadFile (o_file, pv_dst, u_len, & l_readLen, NULL) &&
      (l_readLen == u_len);
    }
  }

//---------------------------------------------------------------------------

static bool tl_Write_
(
t_FileId o_file,
const void * pv_src,
unsigned u_len
)
  {
  if (u_len == 0)
    return true;
  else
    {
    DWORD l_writeLen;

    return WriteFile (o_file, pv_src, u_len, & l_writeLen, NULL) &&
      (l_writeLen == u_len);
    }
  }

//===========================================================================

#elif defined TL_UNIX

#define _FILE_OFFSET_BITS 64

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "tuning/sys/calloc.hpp"

//---------------------------------------------------------------------------

bool TL_EXPORT tl_OpenFile
(
const char * pc_name,
t_FileId & o_file,
bool b_readOnly,
bool /* b_sequential */
)
  {
  if ((o_file != co_InvalidFileId) ||
      (pc_name == 0) || (* pc_name == '\0'))
    return false;

  int i_flags = b_readOnly ? O_RDONLY : O_RDWR;
  #ifdef O_CLOEXEC
  i_flags |= O_CLOEXEC;
  #endif
  o_file = open (pc_name, i_flags);

  if (o_file == -1)
    {
    o_file = co_InvalidFileId;
    return false;
    }

  #ifndef O_CLOEXEC
  fcntl (o_file, F_SETFD, FD_CLOEXEC);
  #endif

  struct flock so_flock;
  so_flock. l_type = b_readOnly ? F_RDLCK : F_WRLCK;
  so_flock. l_whence = SEEK_SET;
  so_flock. l_start = 0;
  so_flock. l_len = 0;

  if (fcntl (o_file, F_SETLK, & so_flock) == -1)
    {
    close (o_file);
    o_file = co_InvalidFileId;
    return false;
    }

  return true;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_CreateFile
(
const char * pc_name,
t_FileId & o_file,
bool b_createNew
)
  {
  if ((o_file != co_InvalidFileId) ||
      (pc_name == 0) || (* pc_name == '\0'))
    return false;

  int i_flags = O_CREAT | O_RDWR;

  if (b_createNew)
    i_flags |= O_EXCL;

  #ifdef O_CLOEXEC
  i_flags |= O_CLOEXEC;
  #endif
  o_file = open (pc_name, i_flags, 0666);

  if (o_file == -1)
    {
    o_file = co_InvalidFileId;
    return false;
    }

  #ifndef O_CLOEXEC
  fcntl (o_file, F_SETFD, FD_CLOEXEC);
  #endif

  struct flock so_flock;
  so_flock. l_type = F_WRLCK;
  so_flock. l_whence = SEEK_SET;
  so_flock. l_start = 0;
  so_flock. l_len = 0;

  if (fcntl (o_file, F_SETLK, & so_flock) == -1)
    {
    close (o_file);
    o_file = co_InvalidFileId;
    return false;
    }

  if (ftruncate (o_file, 0) == -1)
    {
    so_flock. l_type = F_UNLCK;
    fcntl (o_file, F_SETLK, & so_flock);
    close (o_file);
    o_file = co_InvalidFileId;
    return false;
    }

  return true;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_CloseFile
(
t_FileId & o_file
)
  {
  if (o_file == co_InvalidFileId)
    return false;

  struct flock so_flock;
  so_flock. l_type = F_UNLCK;
  so_flock. l_whence = SEEK_SET;
  so_flock. l_start = 0;
  so_flock. l_len = 0;

  if (fcntl (o_file, F_SETLK, & so_flock) == -1)
    {
    close (o_file);
    o_file = co_InvalidFileId;
    return false;
    }

  bool b = (close (o_file) == 0);
  o_file = co_InvalidFileId;
  return b;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_ExistsFile
(
const char * pc_name
)
  {
  if ((pc_name == 0) || (* pc_name == '\0'))
    return false;

  return access (pc_name, F_OK) == 0;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_MoveFile
(
const char * pc_old,
const char * pc_new
)
  {
  if ((pc_old == 0) || (* pc_old == '\0') ||
      (pc_new == 0) || (* pc_new == '\0'))
    return false;

  return rename (pc_old, pc_new) == 0;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_CopyFile
(
const char * pc_old,
const char * pc_new,
bool b_overwrite
)
  {
  if ((pc_old == 0) || (* pc_old == '\0') ||
      (pc_new == 0) || (* pc_new == '\0'))
    return false;

  if ((! b_overwrite) && (access (pc_new, F_OK) == 0))
    return false;

  bool b_ret = false;
  t_FileId o_old = co_InvalidFileId;
  t_FileId o_new = co_InvalidFileId;

  if (tl_OpenFile (pc_old, o_old, true))
    {
    if (tl_CreateFile (pc_new, o_new, false))
      {
      t_FileSize o_oldSize;

      if (tl_QuerySize (o_old, o_oldSize))
        {
        const t_FileSize o_copySize = 0x1000; // 4 KB
        void * pv_buffer = tl_Alloc (o_copySize);

        if (pv_buffer != 0)
          {
          while (o_oldSize > 0)
            {
            t_FileSize o_len =
              (o_oldSize > o_copySize) ? o_copySize : o_oldSize;

            if (tl_Read (o_old, pv_buffer, o_len) &&
                tl_Write (o_new, pv_buffer, o_len))
              o_oldSize -= o_len;
            else
              break;
            }

          b_ret = (o_oldSize == 0);
          tl_Free (pv_buffer);
          }
        }

      tl_CloseFile (o_new);
      }

    tl_CloseFile (o_old);
    }

  return b_ret;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_DeleteFile
(
const char * pc_name
)
  {
  if ((pc_name == 0) || (* pc_name == '\0'))
    return false;

  return remove (pc_name) == 0;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_QuerySize
(
t_FileId o_file,
t_FileSize & o_size
)
  {
  if (o_file == co_InvalidFileId)
    {
    o_size = 0;
    return false;
    }

  struct stat so_stat;

  if (fstat (o_file, & so_stat) == 0)
    {
    o_size = so_stat. st_size;
    return true;
    }
  else
    {
    o_size = 0;
    return false;
    }
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_QueryPos
(
t_FileId o_file,
t_FileSize & o_pos
)
  {
  if (o_file == co_InvalidFileId)
    {
    o_pos = 0;
    return false;
    }

  o_pos = lseek (o_file, 0, SEEK_CUR);

  if (o_pos != (off_t) -1)
    return true;
  else
    {
    o_pos = 0;
    return false;
    }
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_SeekAbs
(
t_FileId o_file,
t_FileSize o_pos
)
  {
  if (o_file == co_InvalidFileId)
    return false;

  return lseek (o_file, o_pos, SEEK_SET) != -1;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_SeekRel
(
t_FileId o_file,
t_FileSize o_pos
)
  {
  if (o_file == co_InvalidFileId)
    return false;

  return lseek (o_file, o_pos, SEEK_CUR) != -1;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_Truncate
(
t_FileId o_file,
t_FileSize o_size
)
  {
  if (o_file == co_InvalidFileId)
    return false;

  return ftruncate (o_file, o_size) != -1;
  }

//---------------------------------------------------------------------------

static bool tl_Read_
(
t_FileId o_file,
void * pv_dst,
unsigned u_len
)
  {
  if (u_len == 0)
    return true;
  else
    return read (o_file, pv_dst, u_len) == (int) u_len;
  }

//---------------------------------------------------------------------------

static bool tl_Write_
(
t_FileId o_file,
const void * pv_src,
unsigned u_len
)
  {
  if (u_len == 0)
    return true;
  else
    return write (o_file, pv_src, u_len) == (int) u_len;
  }

#endif

// Avoid large file problems on network filesystems (support.microsoft.com Article ID: 899149)
static const int ci_FileIOSize = 0x100000; // 1 MB

//---------------------------------------------------------------------------

bool TL_EXPORT tl_Read
(
t_FileId o_file,
void * pv_dst,
t_FileSize o_len
)
  {
  if (o_file == co_InvalidFileId)
    return false;

  char * pc_dst = (char *) pv_dst;

  while (o_len > ci_FileIOSize)
    {
    if (! tl_Read_ (o_file, pc_dst, ci_FileIOSize))
      return false;

    o_len -= ci_FileIOSize;
    pc_dst += ci_FileIOSize;
    }

  return tl_Read_ (o_file, pc_dst, (int) o_len);
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_Write
(
t_FileId o_file,
const void * pv_src,
t_FileSize o_len
)
  {
  if (o_file == co_InvalidFileId)
    return false;

  const char * pc_src = (const char *) pv_src;

  while (o_len > ci_FileIOSize)
    {
    if (! tl_Write_ (o_file, pc_src, ci_FileIOSize))
      return false;

    o_len -= ci_FileIOSize;
    pc_src += ci_FileIOSize;
    }

  return tl_Write_ (o_file, pc_src, (int) o_len);
  }
