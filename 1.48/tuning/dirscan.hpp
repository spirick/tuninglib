
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
// File tuning/dirscan.hpp

#ifndef TUNING_DIRSCAN_HPP
#define TUNING_DIRSCAN_HPP

#include "tuning/dir.hpp"
#include "tuning/sys/cfile.hpp"
#include "tuning/sys/ctimedate.hpp"

#undef FindFirstFile
#undef FindNextFile

//---------------------------------------------------------------------------

typedef unsigned       t_FileAttributes;

const t_FileAttributes co_AttrArchive   = 0x01;
const t_FileAttributes co_AttrDirectory = 0x02;
const t_FileAttributes co_AttrHidden    = 0x04;
const t_FileAttributes co_AttrReadOnly  = 0x08;
const t_FileAttributes co_AttrSystem    = 0x10;

//---------------------------------------------------------------------------

const unsigned       cu_DirScanDataSize = 600; // Multiple of sizeof (t_UInt64)

class TL_EXPORT ct_DirScan: public ct_Directory
  {
  // Don't use char buffer because of alignment
  t_UInt64             au_Data [cu_DirScanDataSize / sizeof (t_UInt64)];
  t_MicroTime          o_CreationTime;
  t_MicroTime          o_LastAccessTime;
  t_MicroTime          o_LastWriteTime;
  t_FileSize           o_Size;
  t_FileAttributes     o_Attributes;
  bool                 b_Found;

  // Do not copy this object
                       ct_DirScan (const ct_DirScan &);
  ct_DirScan &         operator = (const ct_DirScan &);

  void                 Clear ();
  void                 ClearIntern ();
  void                 CopyResult ();

public:
                       ct_DirScan ();
                       ct_DirScan (const char * pc_init);
                       ct_DirScan (const ct_FileName & co_init);
                       ~ct_DirScan ();
  ct_DirScan &         operator = (const char * pc_asgn);
  ct_DirScan &         operator = (const ct_FileName & co_asgn);

  TL_CLASSID (ct_DirScan)

  bool                 FindOnce ();
  bool                 FindOnce (const char * pc_find);
  bool                 FindOnce (const ct_FileName & co_find);
  bool                 FindOncePath ();
  bool                 FindOncePath (const ct_FileName & co_find);

  bool                 FindFirst ();
  bool                 FindFirstFile ();
  bool                 FindFirstDirectory ();
  bool                 FindNext ();
  bool                 FindNextFile ();
  bool                 FindNextDirectory ();
  void                 AbortFind ();
  bool                 Found ();

  t_MicroTime          GetCreationTime () const;
  t_MicroTime          GetLastAccessTime () const;
  t_MicroTime          GetLastWriteTime () const;
  t_FileSize           GetSize () const;
  t_FileAttributes     GetAttributes () const;
  bool                 IsArchive () const;
  bool                 IsDirectory () const;
  bool                 IsHidden () const;
  bool                 IsReadOnly () const;
  bool                 IsSystem () const;
  };

//---------------------------------------------------------------------------

inline bool ct_DirScan::Found ()
  { return b_Found; }

inline t_MicroTime ct_DirScan::GetCreationTime () const
  { return o_CreationTime; }

inline t_MicroTime ct_DirScan::GetLastAccessTime () const
  { return o_LastAccessTime; }

inline t_MicroTime ct_DirScan::GetLastWriteTime () const
  { return o_LastWriteTime; }

inline t_FileSize ct_DirScan::GetSize () const
  { return o_Size; }

inline t_FileAttributes ct_DirScan::GetAttributes () const
  { return o_Attributes; }

inline bool ct_DirScan::IsArchive () const
  { return (o_Attributes & co_AttrArchive) != 0; }

inline bool ct_DirScan::IsDirectory () const
  { return (o_Attributes & co_AttrDirectory) != 0; }

inline bool ct_DirScan::IsHidden () const
  { return (o_Attributes & co_AttrHidden) != 0; }

inline bool ct_DirScan::IsReadOnly () const
  { return (o_Attributes & co_AttrReadOnly) != 0; }

inline bool ct_DirScan::IsSystem () const
  { return (o_Attributes & co_AttrSystem) != 0; }

#endif
