
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
// File sourcedep/sd.cpp

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#include "tuning/chn/array.h"
#include "tuning/chn/sortedarray.h"
#include "tuning/file.hpp"
#include "tuning/timedate.hpp"

#include "sourcedep/project.hpp"
#include "sourcedep/dirmaps.hpp"
#include "sourcedep/filemaps.hpp"

#ifdef TL_UNIX
  #include <unistd.h>
  #define DEP_LINE " \\\n\t%s"
#else
  #define DEP_LINE " \\\n\t\"%s\""
#endif

//---------------------------------------------------------------------------

static int i_IncCount = 0;
static ct_CppProject * pco_Prj;
static gct_Chn32SortedArray <ct_String> co_Incs;
static ct_String co_Target;
static gct_Chn32Array <ct_String> co_Suffixes;

//---------------------------------------------------------------------------

static void AddIncs
(
ct_CppFileSemantic * pco_fsem
)
  {
  if (pco_fsem != 0)
    {
    for (int i_inc = pco_fsem-> FirstInclude ();
         i_inc != 0; i_inc = pco_fsem-> NextInclude (i_inc))
      {
      ct_CppFileSemantic * pco_inc = pco_fsem-> GetInclude (i_inc);

      if ((pco_inc != 0) &&
          (pco_inc-> GetIncCount () != i_IncCount))
        {
        pco_inc-> SetIncCount (i_IncCount);
        pco_inc-> GetFileMap ()-> SetInProject ();
        ct_FileName co_incName;
        pco_inc-> GetFileMap ()-> QueryAbsRelName (& co_incName,
          pco_Prj-> GetFileName ()-> GetDrivePath ());
        co_Incs. AddObj (& co_incName);
        AddIncs (pco_inc);
        }
      }
    }
  }

//---------------------------------------------------------------------------

static void CalcIncs
(
ct_CppFileSemantic * pco_fsem
)
  {
  pco_fsem-> GetFileMap ()-> GetDirMap ()-> SetInProject ();
  co_Incs. DelAll ();
  AddIncs (pco_fsem);

  for (unsigned u = 1; u <= co_Incs. GetLen (); u ++)
    co_Target. AppendF (DEP_LINE, co_Incs. GetNthObj (u)-> GetStr ());
  }

//---------------------------------------------------------------------------

static ct_String co_FileText;

// Search co_find at begin of line
static t_Int FindBol
(
ct_String co_find
)
  {
  if (co_FileText. CompSubStr (0, co_find) == 0)
    return 0;
  else
    {
    co_find. Insert (0, '\n');
    t_Int i_found = co_FileText. First (co_find);

    if (i_found >= 0)
      return i_found + 1;
    else
      return -1;
    }
  }

//---------------------------------------------------------------------------

bool IsIgnoreType
(
const ct_FileName & co_name
)
  {
  #ifdef TL_UNIX

    if (co_name. GetExt () == "a") return true;
    if (co_name. GetExt () == "so") return true;

  #else

    if (co_name. GetExt () == "dll") return true;
    if (co_name. GetExt () == "def") return true;
    if (co_name. GetExt () == "exe") return true;
    if (co_name. GetExt () == "exp") return true;
    if (co_name. GetExt () == "ilk") return true;
    if (co_name. GetExt () == "lib") return true;
    if (co_name. GetExt () == "ocx") return true;
    if (co_name. GetExt () == "pdb") return true;
    if (co_name. GetExt () == "res") return true;
    if (co_name. GetExt () == "rc") return true;

  #endif

  return false;
  }

//---------------------------------------------------------------------------

void SetSuffixes
(
const char * pc
)
  {
  ct_String co = pc;
  t_Int i_semi = co. First (';');
  co_Suffixes. DelAll ();

  while (i_semi >= 0)
    {
    ct_String co_sf (co. GetStr (), i_semi);
    co. Delete (0, i_semi + 1);
    i_semi = co. First (';');

    if (co_sf. GetLen () > 0)
      co_Suffixes. AddObj (& co_sf);
    }

  if (co. GetLen () > 0)
    co_Suffixes. AddObj (& co);
  }

//---------------------------------------------------------------------------

const int i_SdVersion = 120;

int main (int argc, char * * argv)
  {
  bool b_clean = false;
  bool b_logo = true;
  bool b_warn = false;
  ct_String co_all;
  ct_FileName co_dspName;
  ct_String co_incStr;
  ct_String co_prefix;
  ct_String co_targetsVal;
  ct_String co_targetsMacro;
  ct_String co_sourcesVal;
  ct_String co_sourcesMacro;
  ct_File co_file ("makefile");
  gct_Chn32Array <ct_String> co_sources;
  gct_Chn32Array <ct_String> co_commands;
  pco_Prj = new ct_CppProject ("./*");
  ct_TimeDate co_td;
  co_td. QueryLocalTime ();

  ct_String co_sf;
  co_sf = "cpp"; co_Suffixes. AddObj (& co_sf);
  co_sf = "cxx"; co_Suffixes. AddObj (& co_sf);
  co_sf = "cc";  co_Suffixes. AddObj (& co_sf);
  co_sf = "C";   co_Suffixes. AddObj (& co_sf);
  co_sf = "c";   co_Suffixes. AddObj (& co_sf);

  #ifdef TL_UNIX
    ct_String co_ext ("o");
  #else
    bool b_unix = false;
    ct_String co_ext ("obj");
  #endif

  if (argc == 1)
    {
    printf ("Spirick Source Dependencies Utility   Version %.2f\n", i_SdVersion / 100.0);
    printf ("Copyright (C) 1996-%d Dietmar Deimling. All rights reserved.\n", co_td. GetYear ());
    printf ("Written with the \"Spirick Tuning\" Library: www.spirick.com\n\n");

    printf ("Usage: sd [options] files\n");
    printf ("    -all filename    all targets depends on \"filename\"\n");
    printf ("    -clean           clear dependencies\n");
    printf ("    -cmd text        append rule command \"text\"\n");
    printf ("                     \"~s\" means source name, \"~t\" means target name\n");
    printf ("    -dsp name        create visual studio project file \"name.dsp\"\n");
    printf ("    -ext name        use \"name\" as target file name extension\n");
    printf ("    -file filename   write dependencies to \"filename\"\n");
    printf ("                     default is \"makefile\"\n");
    printf ("    -Idirectory      add \"directory\" to include path\n");
    printf ("    -I directory     add \"directory\" to include path\n");
    printf ("    -incvar name     add environment variable \"name\" to include path\n");
    printf ("    -nologo          operate quietly\n");
    printf ("    -prefix text     prepend \"text\" to all targets\n");
    printf ("    -sources name    define makro \"name\" containing all sources\n");
    printf ("    -suffixes text   suffixes for sources, default is \"cpp;cxx;cc;C;c\"\n");
    printf ("    -targets name    define makro \"name\" containing all targets\n");
    printf ("    -unix            force unix output (slash in path)\n");
    printf ("    -warn            warn about parsing problems\n");
    return 0;
    }
  else
    {
    for (int i_arg = 1; i_arg < argc; i_arg ++)
      {
      ct_String co_arg (argv [i_arg]);

      if (co_arg == "-all")
        {
        i_arg ++;

        if (i_arg < argc)
          {
          co_all. Append (" ");
          co_all. Append (argv [i_arg]);
          }
        }
      else
      if (co_arg == "-clean")
        {
        b_clean = true;
        }
      else
      if (co_arg == "-cmd")
        {
        i_arg ++;

        if (i_arg < argc)
          {
          ct_String co_cmd (argv [i_arg]);
          co_commands. AddObj (& co_cmd);
          }
        }
      else
      if (co_arg == "-dsp")
        {
        i_arg ++;

        if (i_arg < argc)
          co_dspName. AssignAsName (argv [i_arg]);
        }
      else
      if (co_arg == "-ext")
        {
        i_arg ++;

        if (i_arg < argc)
          co_ext = argv [i_arg];
        }
      else
      if (co_arg == "-file")
        {
        i_arg ++;

        if (i_arg < argc)
          co_file = argv [i_arg];
        }
      else
      if (co_arg == "-I")
        {
        i_arg ++;

        if (i_arg < argc)
          {
          co_incStr. Append (";");
          co_incStr. Append (argv [i_arg]);
          }
        }
      else
      if (co_arg. CompSubStr (0, "-I") == 0)
        {
        co_arg. Delete (0, 2);
        co_incStr. Append (";");
        co_incStr. Append (co_arg);
        }
      else
      if (co_arg == "-incvar")
        {
        i_arg ++;

        if (i_arg < argc)
          {
          const char * pc = getenv (argv [i_arg]);

          if (pc != 0)
            {
            co_incStr. Append (";");
            co_incStr. Append (pc);
            }
          }
        }
      else
      if (co_arg == "-nologo")
        {
        b_logo = false;
        }
      else
      if (co_arg == "-prefix")
        {
        i_arg ++;

        if (i_arg < argc)
          co_prefix = argv [i_arg];
        }
      else
      if (co_arg == "-sources")
        {
        i_arg ++;

        if (i_arg < argc)
          {
          co_sourcesVal = argv [i_arg];
          co_sourcesVal. Append (" =");
          co_sourcesMacro = co_sourcesVal;
          }
        }
      else
      if (co_arg == "-suffixes")
        {
        i_arg ++;

        if (i_arg < argc)
          SetSuffixes (argv [i_arg]);
        }
      else
      if (co_arg == "-targets")
        {
        i_arg ++;

        if (i_arg < argc)
          {
          co_targetsVal = argv [i_arg];
          co_targetsVal. Append (" =");
          co_targetsMacro = co_targetsVal;
          }
        }
      else
      if (co_arg == "-unix")
        {
        #ifndef TL_UNIX
          b_unix = true;
        #endif
        }
      else
      if (co_arg == "-warn")
        {
        b_warn = true;
        }
      else
      if (co_arg. CompSubStr (0, "-") == 0)
        {
        fprintf (stderr, "sd: Unknown option, ignored \"%s\"\n", argv [i_arg]);
        }
      else
        {
        ct_String co_src = argv [i_arg];
        co_sources. AddObj (& co_src);
        }
      }
    }

  if (b_logo)
    {
    printf ("Spiric (R) Source Dependencies Utility   Version %.2f\n", i_SdVersion / 100.0);
    printf ("Copyright (C) Spiric GmbH 2000. All rights reserved.\n");
    printf ("Written with the \"Spiric (R) Tuning\" Library: www.spiric.com\n");
    }

  if (b_warn)
    pco_Prj-> SetMessages (true);

  co_file. Load (& co_FileText);
  co_FileText. ReplaceAll ("\r\n", "\n");

  for (unsigned u_src = 1; u_src <= co_sources. GetLen (); u_src ++)
    {
    ct_File co_src (co_sources. GetNthObj (u_src)-> GetStr ());

    if (co_src. GetExt () == co_ext)
      {
      // Search suffixes
      for (unsigned u_sf = co_Suffixes. First (); u_sf != 0; u_sf = co_Suffixes. Next (u_sf))
        {
        ct_File co_test (co_sources. GetNthObj (u_src)-> GetStr ());
        co_test. SetExt (co_Suffixes. GetObj (u_sf)-> GetStr ());

        if (co_test. Exists ())
          {
          co_src. SetExt (co_Suffixes. GetObj (u_sf)-> GetStr ());
          break;
          }
        }

      if (co_src. GetExt () == co_ext)
        {
        // Search in old dependencies
        ct_String co_tgt = co_src;

        #ifndef TL_UNIX
          co_tgt. ToLower ();
        #endif

        co_tgt. Append (":");
        t_Int i_targetBegin = FindBol (co_tgt);

        if (i_targetBegin >= 0)
          {
          t_Int i_srcBegin = co_FileText. First ("\n\t", i_targetBegin);

          if (i_srcBegin >= 0)
            {
            i_srcBegin += 2;
            t_UInt u_srcEnd = i_srcBegin;

            while ((u_srcEnd < co_FileText. GetLen ()) &&
                   (co_FileText [u_srcEnd] > ' '))
              u_srcEnd ++;

            ct_String co_srcName (co_FileText. GetStr (i_srcBegin), u_srcEnd - i_srcBegin);
            co_src = co_srcName. GetStr ();
            }
          }
        }
      }

    if (! IsIgnoreType (co_src))
      {
      if ((co_src. GetExt () == co_ext) || (! co_src. Exists ()))
        fprintf (stderr, "sd: Source file not found \"%s\"\n", co_src. GetAllStr ());
      else
        {
        pco_Prj-> AddModuleBefore (0, co_src. GetAllStr ());
        }
      }
    }

  pco_Prj-> SetIncludeStr (co_incStr. GetStr ());
  pco_Prj-> GetParser ()-> Reparse ();

  t_CppModulePtr o_mptr;

  for (o_mptr = pco_Prj-> FirstModulePtr ();
       o_mptr != 0; o_mptr = pco_Prj-> NextModulePtr (o_mptr))
    {
    ct_FileName co_sourceName = * pco_Prj-> GetUnifiedName (o_mptr);
    co_sourceName. ToRel (pco_Prj-> GetFileName ()-> GetDrivePath (). GetStr ());
    ct_FileName co_tmp = co_sourceName;
    co_tmp. SetExt (co_ext);
    ct_String co_targetName = co_tmp;
    co_targetName. Insert (0, co_prefix);
    co_Target = co_targetName;
    co_Target. Append (":");
    t_Int i_targetBegin = FindBol (co_Target);

    if (co_sourcesVal. GetLen () > 0)
      co_sourcesVal. AppendF (DEP_LINE, co_sourceName. GetStr ());

    if (co_targetsVal. GetLen () > 0)
      co_targetsVal. AppendF (DEP_LINE, co_targetName. GetStr ());

    if (i_targetBegin >= 0)
      {
      t_UInt u_targetEnd = i_targetBegin;
      bool b_rule = true;

      while (b_rule && (u_targetEnd < co_FileText. GetLen ()))
        {
        switch (co_FileText [u_targetEnd])
          {
        case '\\':
          u_targetEnd ++;

          if ((u_targetEnd < co_FileText. GetLen ()) &&
              (co_FileText [u_targetEnd] == '\n'))
            u_targetEnd ++;

          break;

        case '\n':
          u_targetEnd ++;

          if ((u_targetEnd < co_FileText. GetLen ()) &&
              (co_FileText [u_targetEnd] == '\t'))
            u_targetEnd ++;
          else
            b_rule = false;

          break;

        default:
          u_targetEnd ++;
          }
        }

      co_FileText. Delete (i_targetBegin, u_targetEnd - i_targetBegin);
      }

    if (! b_clean)
      {
      co_Target. Append (co_all);
      co_Target. AppendF (DEP_LINE, co_sourceName. GetAllStr ());
      t_FileMapPtr o_fmptr = pco_Prj-> GetFileMapPtr (o_mptr);

      if (o_fmptr != 0)
        {
        ct_CppFileSemantic * pco_fsem = pco_Prj-> QueryFileSemantic (o_fmptr);
        i_IncCount ++;
        CalcIncs (pco_fsem);
        }

      co_FileText. Append (co_Target);

      for (unsigned u_cmd = 1; u_cmd <= co_commands. GetLen (); u_cmd ++)
        {
        ct_String co_cmd = * co_commands. GetNthObj (u_cmd);
        co_cmd. ReplaceAll ("~s", co_sourceName);
        co_cmd. ReplaceAll ("~t", co_targetName);
        co_FileText. AppendF ("\n\t%s", co_cmd. GetStr ());
        }

      co_FileText. Append ("\n");
      }
    }

  if (co_sourcesVal. GetLen () > 0)
    {
    t_Int i_sourceBegin = FindBol (co_sourcesMacro);

    if (i_sourceBegin >= 0)
      {
      t_UInt u_sourceEnd = i_sourceBegin;
      bool b_rule = true;

      while (b_rule && (u_sourceEnd < co_FileText. GetLen ()))
        {
        switch (co_FileText [u_sourceEnd])
          {
        case '\\':
          u_sourceEnd ++;

          if ((u_sourceEnd < co_FileText. GetLen ()) &&
              (co_FileText [u_sourceEnd] == '\n'))
            u_sourceEnd ++;

          break;

        case '\n':
          u_sourceEnd ++;
          b_rule = false;
          break;

        default:
          u_sourceEnd ++;
          }
        }

      co_FileText. Delete (i_sourceBegin, u_sourceEnd - i_sourceBegin);
      }

    co_FileText. Append (co_sourcesVal);
    co_FileText. Append ("\n");
    }

  if (co_targetsVal. GetLen () > 0)
    {
    t_Int i_targetBegin = FindBol (co_targetsMacro);

    if (i_targetBegin >= 0)
      {
      t_UInt u_targetEnd = i_targetBegin;
      bool b_rule = true;

      while (b_rule && (u_targetEnd < co_FileText. GetLen ()))
        {
        switch (co_FileText [u_targetEnd])
          {
        case '\\':
          u_targetEnd ++;

          if ((u_targetEnd < co_FileText. GetLen ()) &&
              (co_FileText [u_targetEnd] == '\n'))
            u_targetEnd ++;

          break;

        case '\n':
          u_targetEnd ++;
          b_rule = false;
          break;

        default:
          u_targetEnd ++;
          }
        }

      co_FileText. Delete (i_targetBegin, u_targetEnd - i_targetBegin);
      }

    co_FileText. Append (co_targetsVal);
    co_FileText. Append ("\n");
    }

  #ifndef TL_UNIX
  if (b_unix)
    {
    co_FileText. ReplaceAll ("\\", "/");
    co_FileText. ReplaceAll ("/\n", "\\\n");
    }
  #endif

  // ct_File does not write "\r\n"
  FILE * pso_file = fopen (co_file. GetAllStr (), "wt");

  if (pso_file == 0)
    fprintf (stderr, "sd: Cannot create file \"%s\"\n", co_file. GetAllStr ());
  else
    {
    fwrite (co_FileText. GetStr (), 1, co_FileText. GetLen (), pso_file);
    fclose (pso_file);
    }

  //-------------------------------------------------------------------------

  if (co_dspName. GetLen () > 0)
    {
    co_dspName. SetDrivePath ("");
    co_dspName. SetExt ("dsp");
    ct_String co_dsp = co_dspName. GetName ();
    pso_file = fopen (co_dspName. GetStr (), "wt");

    if (pso_file == 0)
      fprintf (stderr, "sd: Cannot create dsp file \"%s\"\n", co_dspName. GetStr ());
    else
      {
      fprintf (pso_file, "# Microsoft Developer Studio Project File - Name=\"%s\" - Package Owner=<4>\n", co_dsp. GetStr ());
      fprintf (pso_file, "# Microsoft Developer Studio Generated Build File, Format Version 6.00\n");
      fprintf (pso_file, "# ** DO NOT EDIT **\n\n");
      fprintf (pso_file, "# TARGTYPE \"Win32 (x86) External Target\" 0x0106\n\n");
      fprintf (pso_file, "CFG=%s - Win32 Debug\n", co_dsp. GetStr ());
      fprintf (pso_file, "!MESSAGE This is not a valid makefile. To build this project using NMAKE,\n");
      fprintf (pso_file, "!MESSAGE use the Export Makefile command and run\n");
      fprintf (pso_file, "!MESSAGE \n");
      fprintf (pso_file, "!MESSAGE NMAKE /f \"%s.mak\".\n", co_dsp. GetStr ());
      fprintf (pso_file, "!MESSAGE \n");
      fprintf (pso_file, "!MESSAGE You can specify a configuration when running NMAKE\n");
      fprintf (pso_file, "!MESSAGE by defining the macro CFG on the command line. For example:\n");
      fprintf (pso_file, "!MESSAGE \n");
      fprintf (pso_file, "!MESSAGE NMAKE /f \"%s.mak\" CFG=\"%s - Win32 Debug\"\n", co_dsp. GetStr (), co_dsp. GetStr ());
      fprintf (pso_file, "!MESSAGE \n");
      fprintf (pso_file, "!MESSAGE Possible choices for configuration are:\n");
      fprintf (pso_file, "!MESSAGE \n");
      fprintf (pso_file, "!MESSAGE \"%s - Win32 Release\" (based on \"Win32 (x86) External Target\")\n", co_dsp. GetStr ());
      fprintf (pso_file, "!MESSAGE \"%s - Win32 Debug\" (based on \"Win32 (x86) External Target\")\n", co_dsp. GetStr ());
      fprintf (pso_file, "!MESSAGE \n\n");
      fprintf (pso_file, "# Begin Project\n");
      fprintf (pso_file, "# PROP AllowPerConfigDependencies 0\n");
      fprintf (pso_file, "# PROP Scc_ProjName \"\"\n");
      fprintf (pso_file, "# PROP Scc_LocalPath \"\"\n\n");
      fprintf (pso_file, "!IF  \"$(CFG)\" == \"%s - Win32 Release\"\n\n", co_dsp. GetStr ());
      fprintf (pso_file, "# PROP BASE Use_MFC\n");
      fprintf (pso_file, "# PROP BASE Use_Debug_Libraries 0\n");
      fprintf (pso_file, "# PROP BASE Output_Dir \".\"\n");
      fprintf (pso_file, "# PROP BASE Intermediate_Dir \".\"\n");
      fprintf (pso_file, "# PROP BASE Cmd_Line \"\"\n");
      fprintf (pso_file, "# PROP BASE Rebuild_Opt \"\"\n");
      fprintf (pso_file, "# PROP BASE Target_File \"\"\n");
      fprintf (pso_file, "# PROP BASE Bsc_Name \"\"\n");
      fprintf (pso_file, "# PROP BASE Target_Dir \"\"\n");
      fprintf (pso_file, "# PROP Use_MFC\n");
      fprintf (pso_file, "# PROP Use_Debug_Libraries 0\n");
      fprintf (pso_file, "# PROP Output_Dir \".\"\n");
      fprintf (pso_file, "# PROP Intermediate_Dir \".\"\n");
      fprintf (pso_file, "# PROP Cmd_Line \"nmake -nologo\"\n");
      fprintf (pso_file, "# PROP Rebuild_Opt \"-a\"\n");
      fprintf (pso_file, "# PROP Target_File \"\"\n");
      fprintf (pso_file, "# PROP Bsc_Name \"\"\n");
      fprintf (pso_file, "# PROP Target_Dir \"\"\n\n");
      fprintf (pso_file, "!ELSEIF  \"$(CFG)\" == \"%s - Win32 Debug\"\n\n", co_dsp. GetStr ());
      fprintf (pso_file, "# PROP BASE Use_MFC\n");
      fprintf (pso_file, "# PROP BASE Use_Debug_Libraries 1\n");
      fprintf (pso_file, "# PROP BASE Output_Dir \".\"\n");
      fprintf (pso_file, "# PROP BASE Intermediate_Dir \".\"\n");
      fprintf (pso_file, "# PROP BASE Cmd_Line \"\"\n");
      fprintf (pso_file, "# PROP BASE Rebuild_Opt \"\"\n");
      fprintf (pso_file, "# PROP BASE Target_File \"\"\n");
      fprintf (pso_file, "# PROP BASE Bsc_Name \"\"\n");
      fprintf (pso_file, "# PROP BASE Target_Dir \"\"\n");
      fprintf (pso_file, "# PROP Use_MFC\n");
      fprintf (pso_file, "# PROP Use_Debug_Libraries 1\n");
      fprintf (pso_file, "# PROP Output_Dir \"\"\n");
      fprintf (pso_file, "# PROP Intermediate_Dir \"\"\n");
      fprintf (pso_file, "# PROP Cmd_Line \"nmake -nologo\"\n");
      fprintf (pso_file, "# PROP Rebuild_Opt \"-a\"\n");
      fprintf (pso_file, "# PROP Target_File \"\"\n");
      fprintf (pso_file, "# PROP Bsc_Name \"\"\n");
      fprintf (pso_file, "# PROP Target_Dir \"\"\n\n");
      fprintf (pso_file, "!ENDIF \n\n");
      fprintf (pso_file, "# Begin Target\n\n");
      fprintf (pso_file, "# Name \"%s - Win32 Release\"\n", co_dsp. GetStr ());
      fprintf (pso_file, "# Name \"%s - Win32 Debug\"\n\n", co_dsp. GetStr ());
      fprintf (pso_file, "!IF  \"$(CFG)\" == \"%s - Win32 Release\"\n\n", co_dsp. GetStr ());
      fprintf (pso_file, "!ELSEIF  \"$(CFG)\" == \"%s - Win32 Debug\"\n\n", co_dsp. GetStr ());
      fprintf (pso_file, "!ENDIF \n\n");

      fprintf (pso_file, "# Begin Group \"Source Files\"\n\n");
      fprintf (pso_file, "# PROP Default_Filter \"cpp;c;cxx;rc;def;r;odl;idl;hpj;bat\"\n");

      for (o_mptr = pco_Prj-> FirstModulePtr ();
           o_mptr != 0; o_mptr = pco_Prj-> NextModulePtr (o_mptr))
        {
        ct_FileName co_sourceName = * pco_Prj-> GetUnifiedName (o_mptr);
        co_sourceName. ToRel (pco_Prj-> GetFileName ()-> GetDrivePath (). GetStr ());
        fprintf (pso_file, "# Begin Source File\n\n");
        fprintf (pso_file, "SOURCE=.\\%s\n", co_sourceName. GetAllStr ());
        fprintf (pso_file, "# End Source File\n");
        }

      fprintf (pso_file, "# End Group\n");
      fprintf (pso_file, "# Begin Group \"Header Files\"\n\n");
      fprintf (pso_file, "# PROP Default_Filter \"h;hpp;hxx;hm;inl\"\n");

      for (t_DirMapPtr o_dirmPtr = DirMaps ()-> GetDirMaps ()-> First ();
           o_dirmPtr != 0;
           o_dirmPtr = DirMaps ()-> GetDirMaps ()-> Next (o_dirmPtr))
        {
        ct_DirMap * pco_dirMap = DirMaps ()-> GetDirMap (o_dirmPtr);

        if (pco_dirMap-> InProject ())
          {
          const t_FileMapPtrs * pco_fmPtrs = pco_dirMap-> GetFileMapPtrs ();

          for (t_FileMapPtrs::t_Position o_pos = pco_fmPtrs-> First ();
               o_pos != 0;
               o_pos = pco_fmPtrs-> Next (o_pos))
            {
            ct_FileMap * pco_fileMap =
              FileMaps ()-> GetFileMap (* pco_fmPtrs-> GetObj (o_pos));

            if (pco_fileMap-> InProject ())
              {
              ct_FileName co_incName;
              pco_fileMap-> QueryAbsRelName (& co_incName,
                pco_Prj-> GetFileName ()-> GetDrivePath ());
              fprintf (pso_file, "# Begin Source File\n\n");
              fprintf (pso_file, "SOURCE=.\\%s\n", co_incName. GetAllStr ());
              fprintf (pso_file, "# End Source File\n");
              }
            }
          }
        }

      fprintf (pso_file, "# End Group\n");
      fprintf (pso_file, "# End Target\n");
      fprintf (pso_file, "# End Project\n");
      fclose (pso_file);
      }
    }

  return 0;
  }
