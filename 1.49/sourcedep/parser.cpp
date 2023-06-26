
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
// File sourcedep/parser.cpp

#include <stdio.h>

#ifdef TL_UNIX
  #include <unistd.h>
#endif

#include "tuning/filename.hpp"
#include "sourcedep/parser.hpp"
#include "sourcedep/filemaps.hpp"
#include "sourcedep/project.hpp"

//---------------------------------------------------------------------------

const char * apc_MessageStrings [] =
  {
  "Recursive inclusion ignored: %s", // ec_CMsgRecIncludeIgnored,
  "Self inclusion ignored: %s",  // ec_CMsgSelfIncludeIgnored,

  "Include file not found: %s",  // ec_CMsgIncludeFileNotFound
  "Prepocessor syntax error",    // ec_CMsgPPSyntaxError

  "Cannot open file: %s",        // ec_CMsgCannotOpenFile
  };

//---------------------------------------------------------------------------

void ct_CppParser::Message
(
et_Messages eo_msg,
const char * pc_str
)
  {
  if (pco_Project-> Messages ())
    {
    ct_FileName co_name;

    if (o_FileStack. GetLen () > 0)
      {
      ct_CppFileSemantic * pco_fsem = o_FileStack. GetObj (1)-> GetFileSemantic ();

      if (pco_fsem != 0)
        {
        ct_FileMap * pco_map = pco_fsem-> GetFileMap ();

        if (pco_map != 0)
          pco_map-> QueryName (& co_name);
        }
      }

    fprintf (stderr, "sd: ");

    if (co_name. GetLen () > 0)
      fprintf (stderr, "In %s: ", co_name. GetNameStr ());

    fprintf (stderr, apc_MessageStrings [eo_msg], pc_str);
    fprintf (stderr, "\n");
    }
  }

//---------------------------------------------------------------------------

void ct_CppParser::PushFile
(
ct_CppFileSemantic * pco_fileSemantic
)
  {
  TL_ASSERT (pco_fileSemantic != 0);
  TL_ASSERT (! pco_fileSemantic-> InStack ());

  o_FileStack. AddObjBeforeFirst ();
  o_FileStack. GetObj (1)-> SetFileSemantic (pco_fileSemantic);
  pco_fileSemantic-> SetInStack (true);

  if (pco_fileSemantic-> ParseOk ())
    {
    o_FileStack. GetObj (1)-> SetState (ec_CppFileStackCommand);
    }
  else
    {
    pco_fileSemantic-> Clear ();

    if (pco_fileSemantic-> GetFileMap ()-> IsEmpty ())
      {
      o_FileStack. GetObj (1)-> SetLoadOk (false);
      PopFile ();
      }
    else
      {
      ct_FileName co_fn;
      pco_fileSemantic-> GetFileMap ()-> QueryName (& co_fn);

      if (pco_fileSemantic-> GetFileMap ()-> IncLoadRef ())
        o_FileStack. GetObj (1)-> SetLoadOk (true);
      else
        {
        o_FileStack. GetObj (1)-> SetLoadOk (false);
        Message (ec_CMsgCannotOpenFile, co_fn. GetAllStr ());
        }

      o_FileStack. GetObj (1)-> GetScanner ()->
        Initialize (pco_fileSemantic-> GetFileMap ()-> GetTextBegin (),
                    pco_fileSemantic-> GetFileMap ()-> GetTextEnd ());
      o_FileStack. GetObj (1)-> SetState (ec_CppFileStackFirstScan);
      }
    }
  }

//---------------------------------------------------------------------------

void ct_CppParser::PopFile ()
  {
  ct_CppFileSemantic * pco_fileSemantic =
    o_FileStack. GetObj (1)-> GetFileSemantic ();

  if (! pco_fileSemantic-> ParseOk ())
    {
    if (u_FilesToParse > 0)
      {
      u_FilesToParse --;
      pco_fileSemantic-> SetParseOk ();
      }

    if (o_FileStack. GetObj (1)-> LoadOk ())
      pco_fileSemantic-> GetFileMap ()-> DecLoadRef ();
    }

  o_FileStack. DelObj (1);
  pco_fileSemantic-> SetInStack (false);
  }

//---------------------------------------------------------------------------

inline t_Token ct_CppParser::ScanToken ()
  {
  return o_Token = o_FileStack. GetObj (1)-> GetScanner ()-> ScanToken ();
  }

//---------------------------------------------------------------------------

t_Token ct_CppParser::ScanPPToken ()
  {
  switch (ScanToken ())
    {
    case ec_CTokLineConcat:
    case ec_CTokCComment:
    case ec_CTokCppComment:
      return ScanPPToken ();

    default:
      return o_Token;
    }
  }

//---------------------------------------------------------------------------

bool ct_CppParser::TestText
(
t_FileMapPtr * po_fmptr,
const ct_FileName * pco_fileName,
const char * pc_path,
t_UInt u_pathLen,
bool b_rel
)
  {
  ct_FileName co_name = * pco_fileName;

  co_name. InsertDrivePath (pc_path, u_pathLen);
  co_name. CompressPath ();
  * po_fmptr = FileMaps ()-> QueryCreateSerialFile (& co_name, b_rel);
  return * po_fmptr != 0;
  }

//---------------------------------------------------------------------------

bool ct_CppParser::SearchSrcFile
(
t_FileMapPtr * po_fmptr,
const ct_FileName * pco_fileName
)
  {
  if ((pco_fileName-> IsAbs ()) || (pco_fileName-> HasDrive ()))
    {
    ct_FileName co_name = * pco_fileName;
    pco_Project-> UnifyFileName (& co_name);
    * po_fmptr = FileMaps ()-> QueryCreateSerialFile (& co_name, false);
    return * po_fmptr != 0;
    }
  else
    if (TestText (po_fmptr, pco_fileName,
        pco_Project-> GetFileName ()-> GetDriveStr (),
        pco_Project-> GetFileName ()-> GetDrivePathLen (),
        true))
      return true;
    else
      {
      ct_FileName co_name;
      const ct_DirMap * pco_lastDirMap = 0;

      for (unsigned u_stk = o_FileStack. First ();
           u_stk != 0; u_stk = o_FileStack. Next (u_stk))
        {
        const ct_DirMap * pco_dirMap = o_FileStack. GetObj (u_stk)->
          GetFileSemantic ()-> GetFileMap ()-> GetDirMap ();

        if (pco_dirMap != pco_lastDirMap)
          {
          pco_lastDirMap = pco_dirMap;

          if (TestText (po_fmptr, pco_fileName,
              pco_dirMap-> GetPath (), pco_dirMap-> GetPathLen (),
              pco_dirMap-> IsRel ()))
            return true;
          }
        }

      return false;
      }
  }

//---------------------------------------------------------------------------

bool ct_CppParser::SearchStdFile
(
t_FileMapPtr * po_fmptr,
const ct_FileName * pco_fileName
)
  {
  if ((pco_fileName-> IsRel ()) && (! pco_fileName-> HasDrive ()))
    {
    for (t_IncludePtr o_incPtr = pco_Project-> FirstIncludeDir ();
         o_incPtr != 0; o_incPtr = pco_Project-> NextIncludeDir (o_incPtr))
      if (TestText (po_fmptr, pco_fileName,
          pco_Project-> GetIncludeDir (o_incPtr),
          pco_Project-> GetIncludeDirLen (o_incPtr),
          pco_Project-> GetIncludeRel (o_incPtr)))
        return true;
    }

  return false;
  }

//---------------------------------------------------------------------------

void ct_CppParser::PPDirective ()
  {
  if (ScanPPToken () == ec_CTokInclude)
    {
    switch (ScanPPToken ())
      {
      case ec_CTokSrcFileName:
      case ec_CTokStdFileName:
        {
        ct_String co_string
         (o_FileStack. GetObj (1)-> GetScanner ()-> GetTokenString (),
          o_FileStack. GetObj (1)-> GetScanner ()-> GetTokenLength ());

        if (o_Token == ec_CTokSrcFileName)
          {
          if ((co_string. GetLen () > 0) &&
              (co_string. GetChar (0) == '"'))
            co_string. Delete (0, 1);

          if ((co_string. GetLen () > 0) &&
              (co_string. GetChar (co_string. GetLen () - 1) == '"'))
            co_string. Delete (co_string. GetLen () - 1, 1);
          }
        else
          {
          if ((co_string. GetLen () > 0) &&
              (co_string. GetChar (0) == '<'))
            co_string. Delete (0, 1);

          if ((co_string. GetLen () > 0) &&
              (co_string. GetChar (co_string. GetLen () - 1) == '>'))
            co_string. Delete (co_string. GetLen () - 1, 1);
          }

        while ((co_string. GetLen () > 0) &&
               (co_string. GetChar (0) <= ' '))
          co_string. Delete (0, 1);

        while ((co_string. GetLen () > 0) &&
               (co_string. GetChar (co_string. GetLen () - 1) <= ' '))
          co_string. Delete (co_string. GetLen () - 1, 1);

        ct_FileName co_fileName;
        t_FileMapPtr o_fileMapPtr;
        ct_CppFileSemantic * pco_fileSemantic = 0;

        co_fileName. AssignAsName (co_string. GetStr ());

        #ifndef TL_UNIX
          co_fileName. ToLower ();
        #endif

        if (o_Token == ec_CTokSrcFileName)
          {
          if (! SearchSrcFile (& o_fileMapPtr, & co_fileName))
            SearchStdFile (& o_fileMapPtr, & co_fileName);
          }
        else
          {
          if (! SearchStdFile (& o_fileMapPtr, & co_fileName))
            SearchSrcFile (& o_fileMapPtr, & co_fileName);
          }

        if (o_fileMapPtr != 0)
          {
          pco_fileSemantic = pco_Project-> QueryFileSemantic (o_fileMapPtr);

          if (pco_fileSemantic == 0)
            {
            u_FilesToParse ++;
            pco_fileSemantic =
              pco_Project-> QueryCreateFileSemantic (o_fileMapPtr);
            }
          }

        while (ScanPPToken () != ec_CTokSkippedText);

        if (pco_fileSemantic != 0)
          {
          if (pco_fileSemantic ==
              o_FileStack. GetObj (1)-> GetFileSemantic ())
            {
            ct_FileName co_fn;

            pco_fileSemantic-> GetFileMap ()-> QueryName (& co_fn);
            Message (ec_CMsgSelfIncludeIgnored, co_fn. GetNameStr ());
            }
          else
            {
            o_FileStack. GetObj (1)->
              GetFileSemantic ()-> AddInclude (pco_fileSemantic);

            if (pco_fileSemantic-> InStack ())
              {
              ct_FileName co_fn;

              pco_fileSemantic-> GetFileMap ()-> QueryName (& co_fn);
              Message (ec_CMsgRecIncludeIgnored, co_fn. GetNameStr ());
              pco_fileSemantic->
                AddIncluding (o_FileStack. GetObj (1)-> GetFileSemantic ());
              }
            else
              {
              pco_fileSemantic->
                AddIncluding (o_FileStack. GetObj (1)-> GetFileSemantic ());
              PushFile (pco_fileSemantic);
              }
            }
          }
        else
          Message (ec_CMsgIncludeFileNotFound, co_string. GetStr ());

        break;
        }

      default:
        Message (ec_CMsgPPSyntaxError);
        while (ScanPPToken () != ec_CTokSkippedText);
      }
    }
  else
    while (ScanPPToken () != ec_CTokSkippedText);
  }

//---------------------------------------------------------------------------

t_Token ct_CppParser::NextToken ()
  {
  while ((o_FileStack. GetLen () > 0) && (u_FilesToParse > 0))
    switch (o_FileStack. GetObj (1)-> GetState ())
      {
      case ec_CppFileStackFirstScan:
        switch (ScanToken ())
          {
          case ec_TokNoToken:
            PopFile ();
            break;

          case ec_CTokSkippedText:
          case ec_CTokAsmText:
          case ec_CTokLineConcat:
          case ec_CTokCComment:
          case ec_CTokCppComment:
            break;

          case ec_CTokPPBegin:
            PPDirective ();
            break;

          default:
            return o_Token;
          }
        break;

      case ec_CppFileStackCommand:
        PopFile ();
        break;

      default:
        TL_ASSERT (false);
        break;
      }

  return ec_TokNoToken;
  }

//---------------------------------------------------------------------------

ct_CppParser::ct_CppParser ()
  {
  pco_Project = 0;
  }

//---------------------------------------------------------------------------

void ct_CppParser::Init
(
ct_CppProject * pco_project
)
  {
  pco_Project = pco_project;
  }

//---------------------------------------------------------------------------

bool ct_CppParser::Reparse ()
  {
  t_CppModulePtr o_mptr;
  u_FilesToParse = 0;

  for (o_mptr = pco_Project-> FirstModulePtr ();
       o_mptr != 0; o_mptr = pco_Project-> NextModulePtr (o_mptr))
    {
    TL_ASSERT (pco_Project-> GetFileMapPtr (o_mptr) == 0);
    t_FileMapPtr o_moduleFileMapPtr = FileMaps ()->
      QueryCreateSerialFile (pco_Project-> GetUnifiedName (o_mptr), true);
    TL_ASSERT (o_moduleFileMapPtr != 0);
    pco_Project-> SetFileMapPtr (o_mptr, o_moduleFileMapPtr);
    u_FilesToParse ++;
    ct_CppFileSemantic * pco_fileSemantic =
      pco_Project-> QueryFileSemantic (pco_Project-> GetFileMapPtr (o_mptr));
    PushFile (pco_fileSemantic);
    while (NextToken ());
    }

  TL_ASSERT (u_FilesToParse == 0);

  while (! o_FileStack. IsEmpty ())
    PopFile ();

  return true;
  }
