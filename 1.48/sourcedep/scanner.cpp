
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
// File sourcedep/scanner.cpp

#include "sourcedep/scanner.hpp"
#include "tuning/sys/cstring.hpp"
#include "tuning/sys/cmemory.hpp"
#include "tuning/sys/cprocess.hpp"
#include "sourcedep/filemap.hpp"

//---------------------------------------------------------------------------

static const char c_Alpha = 0x1;
static const char c_Num   = 0x2;
static const char c_Hex   = 0x4;
static const char c_Octal = 0x8;
static const char c_AlNum = c_Alpha | c_Num;

static const char ac_CharTypes [256] =
  {
  0, //   0
  0, //   1
  0, //   2
  0, //   3
  0, //   4
  0, //   5
  0, //   6
  0, //   7
  0, //   8
  0, //   9
  0, //   10
  0, //   11
  0, //   12
  0, //   13
  0, //   14
  0, //   15
  0, //   16
  0, //   17
  0, //   18
  0, //   19
  0, //   20
  0, //   21
  0, //   22
  0, //   23
  0, //   24
  0, //   25
  0, //   26
  0, //   27
  0, //   28
  0, //   29
  0, //   30
  0, //   31
  0, //   32
  0, // ! 33
  0, // " 34
  0, // # 35
  0, // $ 36
  0, // % 37
  0, // & 38
  0, //   39
  0, // ( 40
  0, // ) 41
  0, // * 42
  0, // + 43
  0, // , 44
  0, // - 45
  0, // . 46
  0, // / 47
  c_Num | c_Hex | c_Octal, // 0 48
  c_Num | c_Hex | c_Octal, // 1 49
  c_Num | c_Hex | c_Octal, // 2 50
  c_Num | c_Hex | c_Octal, // 3 51
  c_Num | c_Hex | c_Octal, // 4 52
  c_Num | c_Hex | c_Octal, // 5 53
  c_Num | c_Hex | c_Octal, // 6 54
  c_Num | c_Hex | c_Octal, // 7 55
  c_Num | c_Hex, // 8 56
  c_Num | c_Hex, // 9 57
  0, // : 58
  0, // ; 59
  0, // < 60
  0, // = 61
  0, // > 62
  0, // ? 63
  0, // @ 64
  c_Alpha | c_Hex, // A 65
  c_Alpha | c_Hex, // B 66
  c_Alpha | c_Hex, // C 67
  c_Alpha | c_Hex, // D 68
  c_Alpha | c_Hex, // E 69
  c_Alpha | c_Hex, // F 70
  c_Alpha, // G 71
  c_Alpha, // H 72
  c_Alpha, // I 73
  c_Alpha, // J 74
  c_Alpha, // K 75
  c_Alpha, // L 76
  c_Alpha, // M 77
  c_Alpha, // N 78
  c_Alpha, // O 79
  c_Alpha, // P 80
  c_Alpha, // Q 81
  c_Alpha, // R 82
  c_Alpha, // S 83
  c_Alpha, // T 84
  c_Alpha, // U 85
  c_Alpha, // V 86
  c_Alpha, // W 87
  c_Alpha, // X 88
  c_Alpha, // Y 89
  c_Alpha, // Z 90
  0, // [ 91
  0, // \ 92
  0, // ] 93
  0, // ^ 94
  c_Alpha, // _ 95
  0, // ` 96
  c_Alpha | c_Hex, // a 97
  c_Alpha | c_Hex, // b 98
  c_Alpha | c_Hex, // c 99
  c_Alpha | c_Hex, // d 100
  c_Alpha | c_Hex, // e 101
  c_Alpha | c_Hex, // f 102
  c_Alpha, // g 103
  c_Alpha, // h 104
  c_Alpha, // i 105
  c_Alpha, // j 106
  c_Alpha, // k 107
  c_Alpha, // l 108
  c_Alpha, // m 109
  c_Alpha, // n 110
  c_Alpha, // o 111
  c_Alpha, // p 112
  c_Alpha, // q 113
  c_Alpha, // r 114
  c_Alpha, // s 115
  c_Alpha, // t 116
  c_Alpha, // u 117
  c_Alpha, // v 118
  c_Alpha, // w 119
  c_Alpha, // x 120
  c_Alpha, // y 121
  c_Alpha, // z 122
  0, // { 123
  0, // | 124
  0, // } 125
  0, // ~ 126
  0, //  127
  0, // Ä 128
  0, // Å 129
  0, // Ç 130
  0, // É 131
  0, // Ñ 132
  0, // Ö 133
  0, // Ü 134
  0, // á 135
  0, // à 136
  0, // â 137
  0, // ä 138
  0, // ã 139
  0, // å 140
  0, // ç 141
  0, // é 142
  0, // è 143
  0, // ê 144
  0, // ë 145
  0, // í 146
  0, // ì 147
  0, // î 148
  0, // ï 149
  0, // ñ 150
  0, // ó 151
  0, // ò 152
  0, // ô 153
  0, // ö 154
  0, // õ 155
  0, // ú 156
  0, // ù 157
  0, // û 158
  0, // ü 159
  0, // † 160
  0, // ° 161
  0, // ¢ 162
  0, // £ 163
  0, // § 164
  0, // • 165
  0, // ¶ 166
  0, // ß 167
  0, // ® 168
  0, // © 169
  0, // ™ 170
  0, // ´ 171
  0, // ¨ 172
  0, // ≠ 173
  0, // Æ 174
  0, // Ø 175
  0, // ∞ 176
  0, // ± 177
  0, // ≤ 178
  0, // ≥ 179
  0, // ¥ 180
  0, // µ 181
  0, // ∂ 182
  0, // ∑ 183
  0, // ∏ 184
  0, // π 185
  0, // ∫ 186
  0, // ª 187
  0, // º 188
  0, // Ω 189
  0, // æ 190
  0, // ø 191
  0, // ¿ 192
  0, // ¡ 193
  0, // ¬ 194
  0, // √ 195
  0, // ƒ 196
  0, // ≈ 197
  0, // ∆ 198
  0, // « 199
  0, // » 200
  0, // … 201
  0, //   202
  0, // À 203
  0, // Ã 204
  0, // Õ 205
  0, // Œ 206
  0, // œ 207
  0, // – 208
  0, // — 209
  0, // “ 210
  0, // ” 211
  0, // ‘ 212
  0, // ’ 213
  0, // ÷ 214
  0, // ◊ 215
  0, // ÿ 216
  0, // Ÿ 217
  0, // ⁄ 218
  0, // € 219
  0, // ‹ 220
  0, // › 221
  0, // ﬁ 222
  0, // ﬂ 223
  0, // ‡ 224
  0, // · 225
  0, // ‚ 226
  0, // „ 227
  0, // ‰ 228
  0, // Â 229
  0, // Ê 230
  0, // Á 231
  0, // Ë 232
  0, // È 233
  0, // Í 234
  0, // Î 235
  0, // Ï 236
  0, // Ì 237
  0, // Ó 238
  0, // Ô 239
  0, //  240
  0, // Ò 241
  0, // Ú 242
  0, // Û 243
  0, // Ù 244
  0, // ı 245
  0, // ˆ 246
  0, // ˜ 247
  0, // ¯ 248
  0, // ˘ 249
  0, // ˙ 250
  0, // ˚ 251
  0, // ¸ 252
  0, // ˝ 253
  0, // ˛ 254
  0  // ˇ 255
  };

static inline bool IsAlpha (char c)
  {
  return (ac_CharTypes [(unsigned char) c] & c_Alpha) != 0;
  }

static inline bool IsNum (char c)
  {
  return (ac_CharTypes [(unsigned char) c] & c_Num) != 0;
  }

static inline bool IsHex (char c)
  {
  return (ac_CharTypes [(unsigned char) c] & c_Hex) != 0;
  }

static inline bool IsOctal (char c)
  {
  return (ac_CharTypes [(unsigned char) c] & c_Octal) != 0;
  }

static inline bool IsAlNum (char c)
  {
  return (ac_CharTypes [(unsigned char) c] & c_AlNum) != 0;
  }

//---------------------------------------------------------------------------

inline bool ct_CppScanner::InText ()
  {
  return pc_Scan < pc_End;
  }

//---------------------------------------------------------------------------

inline bool ct_CppScanner::EndOfText ()
  {
  return pc_Scan >= pc_End;
  }

//---------------------------------------------------------------------------

inline const char * ct_CppScanner::FirstChar
(
char c
)
  {
  char * pc = (char *) tl_FirstChar (pc_Scan, pc_End - pc_Scan, c);

  if (pc != 0)
    return pc;
  else
    return pc_End;
  }

//---------------------------------------------------------------------------

t_Token ct_CppScanner::ScanAsm ()
  {
  while (InText () && (* pc_Scan <= ' ') && (* pc_Scan != '\n'))
    pc_Scan ++;

  if (InText ())
    {
    if ((* pc_Scan == '{') || (* pc_Scan == '\n'))
      {
      int i_level = 1;
      bool b_first = true;

      do
        switch (* pc_Scan)
          {
          case '\n':
            pc_Scan ++; // NewLine
            break;

          case '"':
            pc_Scan ++;
            ScanString ('"', ec_CTokStringLit);
            break;

          case '\'':
            pc_Scan ++;
            ScanString ('\'', ec_CTokCharLit);
            break;

          case '/':
            pc_Scan ++;

            if (InText ())
              {
              if (* pc_Scan == '*')
                ScanCComment ();
              else
                {
                if (* pc_Scan == '/')
                  {
                  ScanCppComment ();
                  return ec_CTokSkippedText;
                  }
                }
              }
            break;

          case ';':
            pc_Scan = FirstChar ('\n');
            break;

          case '{':
            if (b_first)
              b_first = false;
            else
              i_level ++;

            pc_Scan ++;
            break;

          case '}':
            i_level --;
            pc_Scan ++;
            break;

          default:
            pc_Scan ++;
          }
      while (InText () && (i_level > 0));

      while (InText () && (* pc_Scan <= ' ') && (* pc_Scan != '\n'))
        pc_Scan ++;

      if (InText () && (* pc_Scan == ';'))
        pc_Scan ++;
      }
    else
      pc_Scan = FirstChar ('\n');
    }

  return ec_CTokAsmText;
  }

//---------------------------------------------------------------------------

inline t_Token ct_CppScanner::ScanCComment ()
  {
  TL_ASSERT (* pc_Scan == '*');

  u_LineToken --;
  pc_Scan ++; // '*'

  if (InText ())
    {
    pc_Scan ++;

    const char * pc_slash;
    const char * pc_nl = FirstChar ('\n');

    do
      {
      pc_slash = FirstChar ('/');

      while (pc_nl < pc_slash)
        {
        pc_Scan = pc_nl;
        pc_Scan ++; // NewLine
        pc_nl = FirstChar ('\n');
        }

      if (pc_slash == pc_End)
        {
        pc_Scan = pc_End;
        break;
        }
      else
        pc_Scan = pc_slash + 1;
      }
    while (pc_Scan [-2] != '*');
    }

  return ec_CTokCComment;
  }

//---------------------------------------------------------------------------

inline t_Token ct_CppScanner::ScanCppComment ()
  {
  pc_Scan = FirstChar ('\n');
  return ec_CTokCppComment;
  }

//-------------------------------------------------------------------------

inline void ct_CppScanner::TestUL ()
  {
  if (tl_ToUpper (* pc_Scan) == 'U')
    {
    pc_Scan ++;

    if (InText () && (tl_ToUpper (* pc_Scan) == 'L'))
      pc_Scan ++;
    }
  else
    {
    if (tl_ToUpper (* pc_Scan) == 'L')
      {
      pc_Scan ++;

      if (InText () && (tl_ToUpper (* pc_Scan) == 'U'))
        pc_Scan ++;
      }
    }
  }

//---------------------------------------------------------------------------

t_Token ct_CppScanner::ScanFloat ()
  {
  if (* pc_Scan == '.')
    {
    do
      {
      pc_Scan ++;

      if (EndOfText ())
        return ec_CTokNumLit; // FloatLit
      }
    while (IsNum (* pc_Scan));
    }

  if (tl_ToUpper (* pc_Scan) == 'E')
    {
    pc_Scan ++;

    if (EndOfText ())
      return ec_CTokNumLit; // FloatLit

    if (IsNum (* pc_Scan) || (* pc_Scan == '-') || (* pc_Scan == '+'))
      {
      do
        {
        pc_Scan ++;

        if (EndOfText ())
          return ec_CTokNumLit; // FloatLit
        }
      while (IsNum (* pc_Scan));
      }
    }

  if ((tl_ToUpper (* pc_Scan) == 'F') || (tl_ToUpper (* pc_Scan) == 'L'))
    pc_Scan ++;

  return ec_CTokNumLit; // FloatLit
  }

//---------------------------------------------------------------------------

inline t_Token ct_CppScanner::ScanNull ()
  {
  if (InText ())
    {
    if (tl_ToUpper (* pc_Scan) == 'X')
      {
      do
        {
        pc_Scan ++;

        if (EndOfText ())
          return ec_CTokNumLit; // HexLit
        }
      while (IsHex (* pc_Scan));

      TestUL ();
      return ec_CTokNumLit; // HexLit
      }
    else
      {
      if (IsOctal (* pc_Scan))
        {
        do
          {
          pc_Scan ++;

          if (EndOfText ())
            return ec_CTokNumLit; // OctalLit
          }
        while (IsOctal (* pc_Scan));

        TestUL ();
        return ec_CTokNumLit; // OctalLit
        }
      else
        {
        if ((* pc_Scan == '.') || (tl_ToUpper (* pc_Scan) == 'E'))
          return ScanFloat ();
        else
          TestUL ();
        }
      }
    }

  return ec_CTokNumLit; // Pure 0, IntLit
  }

//---------------------------------------------------------------------------

inline t_Token ct_CppScanner::ScanNumber ()
  {
  if (InText ())
    {
    while (IsNum (* pc_Scan))
      {
      pc_Scan ++;

      if (EndOfText ())
        return ec_CTokNumLit; // IntLit
      }

    if ((* pc_Scan == '.') || (tl_ToUpper (* pc_Scan) == 'E'))
      return ScanFloat ();
    else
      TestUL ();
    }

  return ec_CTokNumLit; // IntLit
  }

//---------------------------------------------------------------------------

t_Token ct_CppScanner::ScanString
(
char c_end,
t_Token o_ret
)
  {
  while (InText ())
    {
    if (* pc_Scan == '\\')
      {
      pc_Scan ++;

      if (InText ())
        {
        if (* pc_Scan == 'x')
          {
          pc_Scan ++;

          while (InText () && IsHex (* pc_Scan))
            pc_Scan ++;
          }
        else
          {
          if (IsOctal (* pc_Scan))
            {
            pc_Scan ++;

            if (InText () && IsOctal (* pc_Scan))
              {
              pc_Scan ++;

              if (InText () && IsOctal (* pc_Scan))
                pc_Scan ++;
              }
            }
          else
            {
            if (* pc_Scan == '\r')
              {
              pc_Scan ++;

              if (InText () && (* pc_Scan == '\n'))
                pc_Scan ++; // NewLine
              }
            else
              pc_Scan ++;
            }
          }
        }
      }
    else
      {
      if (* pc_Scan == '\n')
        return ec_CTokScanError;
      else
        {
        if (* (pc_Scan ++) == c_end)
          return o_ret;
        }
      }
    }

  return ec_CTokScanError;
  }

//---------------------------------------------------------------------------

t_Token ct_CppScanner::ScanIdf ()
  {
  const char * pc_scan = pc_Scan;
  const char * pc_end = pc_End;

  while ((pc_scan < pc_end) && IsAlNum (* pc_scan))
    pc_scan ++;

  pc_Scan = pc_scan;
  ct_String co_keyword (pc_Token, pc_Scan - pc_Token);

  if (eo_Language >= ec_LangPrep)
    {
    if (eo_Language == ec_LangPrepBegin)
      {
      if (u_LineToken == 1)
        {
        if (co_keyword == "include")
          {
          eo_Language = ec_LangInclude;
          return ec_CTokInclude;
          }
        else
          {
          eo_Language = ec_LangPrep;
          return ec_CTokPreproc;
          }
        }

      eo_Language = ec_LangSkipPrep;
      return ec_CTokIdf;
      }
    }
  else
    {
    if ((co_keyword == "asm") ||
        (co_keyword == "_asm") ||
        (co_keyword == "__asm"))
      return ScanAsm ();
    }

  return ec_CTokIdf;
  }

//---------------------------------------------------------------------------

inline t_Token ct_CppScanner::PrepEnd ()
  {
  eo_Language = eo_OldLanguage;
  pc_Token = pc_Scan;
  return ec_CTokSkippedText;
  }

//---------------------------------------------------------------------------

void ct_CppScanner::Initialize
(
const char * pc_begin,
const char * pc_end
)
  {
  eo_Language = ec_LangCpp;
  pc_Begin = pc_begin;
  pc_End = pc_end;
  pc_Scan = pc_begin;
  u_LineToken = unsigned (-1);
  pc_Token = 0;
  }

//---------------------------------------------------------------------------

t_Token ct_CppScanner::ScanToken ()
  {
  if (eo_Language == ec_LangSkipPrep)
    {
    while (InText () && (* pc_Scan <= ' ') && (* pc_Scan != '\n'))
      pc_Scan ++;

    PrepEnd ();

    while (InText ())
      switch (* pc_Scan)
        {
        case '\n':
          return ec_CTokSkippedText;

        case '"':
          pc_Scan ++;
          ScanString ('"', ec_CTokStringLit);
          break;

        case '\'':
          pc_Scan ++;
          ScanString ('\'', ec_CTokCharLit);
          break;

        case '/':
          pc_Scan ++;

          if (InText ())
            {
            if (* pc_Scan == '*')
              ScanCComment ();
            else
              {
              if (* pc_Scan == '/')
                {
                ScanCppComment ();
                return ec_CTokSkippedText;
                }
              }
            }
          break;

        case '\\':
          pc_Scan ++;

          while (InText () && (* pc_Scan <= ' '))
            {
            if (* pc_Scan == '\n')
              {
              pc_Scan ++; // NewLine
              break;
              }
            else
              pc_Scan ++;
            }

          break;

        default:
          pc_Scan ++;
        }

    return ec_CTokSkippedText;
    }

  if (EndOfText ())
    {
    if (eo_Language >= ec_LangPrep)
      return PrepEnd ();
    else
      return 0;
    }

  while (* pc_Scan <= ' ')
    {
    if (* pc_Scan == '\n')
      {
      if (eo_Language >= ec_LangPrep)
        return PrepEnd ();
      else
        {
        pc_Scan ++; // NewLine
        u_LineToken = unsigned (-1);
        }
      }
    else
      {
      const char * pc_scan = pc_Scan + 1;
      const char * pc_end = pc_End;

      while ((pc_scan < pc_end) && (* pc_scan <= ' ') && (* pc_scan != '\n'))
        pc_scan ++;

      pc_Scan = pc_scan;
      }

    if (EndOfText ())
      {
      if (eo_Language >= ec_LangPrep)
        return PrepEnd ();
      else
        return 0;
      }
    }

  u_LineToken ++;
  pc_Token = pc_Scan;

  switch (* (pc_Scan ++))
    {
    case '!':
      if (InText () && (* pc_Scan == '='))
        {
        pc_Scan ++;
        return ec_CTokNotEqual;
        }
      else
        return ec_CTokLogNot;

    case '"':
      if (eo_Language == ec_LangInclude)
        {
        while (InText () && (* pc_Scan != '\n') && (* (pc_Scan ++) != '"'));

        eo_Language = ec_LangSkipPrep;
        return ec_CTokSrcFileName;
        }
      else
        return ScanString ('"', ec_CTokStringLit);

    case '#':
      if (eo_Language >= ec_LangPrep)
        {
        if (InText ())
          {
          if (* pc_Scan == '#')
            {
            pc_Scan ++;
            return ec_CTokTokenConcat;
            }
          else
            {
            if (* pc_Scan == '@')
              {
              pc_Scan ++;
              return ec_CTokCharize;
              }
            }
          }

        return ec_CTokStringize;
        }
      else
        if (u_LineToken == 0)
          {
          eo_OldLanguage = eo_Language;
          eo_Language = ec_LangPrepBegin;
          return ec_CTokPPBegin;
          }
        else
          return ec_CTokScanError;

    case '$':
      return ec_CTokScanError;

    case '%':
      if (InText () && (* pc_Scan == '='))
        {
        pc_Scan ++;
        return ec_CTokModAssign;
        }
      else
        return ec_CTokMod;

    case '&':
      if (InText ())
        {
        if (* pc_Scan == '&')
          {
          pc_Scan ++;
          return ec_CTokLogAnd;
          }
        else
          {
          if (* pc_Scan == '=')
            {
            pc_Scan ++;
            return ec_CTokAndAssign;
            }
          }
        }

      return ec_CTokAmpers;

    case '\'':
      return ScanString ('\'', ec_CTokCharLit);

    case '(':
      return ec_CTokOpenPar;

    case ')':
      return ec_CTokClosePar;

    case '*':
      if (InText () && (* pc_Scan == '='))
        {
        pc_Scan ++;
        return ec_CTokMultAssign;
        }
      else
        return ec_CTokStar;

    case '+':
      if (InText ())
        {
        if (* pc_Scan == '+')
          {
          pc_Scan ++;
          return ec_CTokIncrement;
          }
        else
          {
          if (* pc_Scan == '=')
            {
            pc_Scan ++;
            return ec_CTokPlusAssign;
            }
          }
        }

      return ec_CTokPlus;

    case ',':
      return ec_CTokComma;

    case '-':
      if (InText ())
        {
        if (* pc_Scan == '-')
          {
          pc_Scan ++;
          return ec_CTokDecrement;
          }
        else
          {
          if (* pc_Scan == '=')
            {
            pc_Scan ++;
            return ec_CTokMinusAssign;
            }
          else
            {
            if (* pc_Scan == '>')
              {
              pc_Scan ++;

              if (InText () && (* pc_Scan == '*'))
                {
                pc_Scan ++;
                return ec_CTokArrowStar;
                }
              else
                return ec_CTokArrow;
              }
            }
          }
        }

      return ec_CTokMinus;

    case '.':
      if (InText ())
        {
        if (* pc_Scan == '*')
          {
          pc_Scan ++;
          return ec_CTokDotStar;
          }
        else
          {
          if (* pc_Scan == '.')
            {
            pc_Scan ++;

            if (InText () && (* pc_Scan == '.'))
              {
              pc_Scan ++;
              return ec_CTokEllipsis;
              }
            else
              return ec_CTokScanError;
            }
          else
            {
            if (IsNum (* pc_Scan))
              {
              pc_Scan --;
              return ScanFloat ();
              }
            }
          }
        }

      return ec_CTokDot;

    case '/':
      if (InText ())
        {
        if (* pc_Scan == '*')
          return ScanCComment ();
        else
          {
          if (* pc_Scan == '/')
            return ScanCppComment ();
          else
            {
            if (* pc_Scan == '=')
              {
              pc_Scan ++;
              return ec_CTokDivAssign;
              }
            }
          }
        }

      return ec_CTokDiv;

    case '0':
      return ScanNull ();

    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      return ScanNumber ();

    case ':':
      if (InText () && (* pc_Scan == ':'))
        {
        pc_Scan ++;
        return ec_CTokScopeReso;
        }
      else
        return ec_CTokColon;

    case ';':
      return ec_CTokSemicolon;

    case '<':
      if (eo_Language == ec_LangInclude)
        {
        while (InText () && (* pc_Scan != '\n') && (* (pc_Scan ++) != '>'));

        eo_Language = ec_LangSkipPrep;
        return ec_CTokStdFileName;
        }
      else
        {
        if (InText ())
          {
          if (* pc_Scan == '=')
            {
            pc_Scan ++;
            return ec_CTokLowerEqual;
            }
          else
            {
            if (* pc_Scan == '<')
              {
              pc_Scan ++;

              if (InText () && (* pc_Scan == '='))
                {
                pc_Scan ++;
                return ec_CTokLsAssign;
                }
              else
                return ec_CTokLeftShift;
              }
            }
          }
        }

      return ec_CTokLower;

    case '=':
      if (InText () && (* pc_Scan == '='))
        {
        pc_Scan ++;
        return ec_CTokEqual;
        }
      else
        return ec_CTokAssign;

    case '>':
      if (InText ())
        {
        if (* pc_Scan == '=')
          {
          pc_Scan ++;
          return ec_CTokGreaterEqual;
          }
        else
          {
          if (* pc_Scan == '>')
            {
            pc_Scan ++;

            if (InText () && (* pc_Scan == '='))
              {
              pc_Scan ++;
              return ec_CTokRsAssign;
              }
            else
              return ec_CTokRightShift;
            }
          }
        }

      return ec_CTokGreater;

    case '?':
      return ec_CTokLogIf;

    case '@':
      return ec_CTokScanError;

    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
      return ScanIdf ();

    case 'L':
      if (InText ())
        {
        if (* pc_Scan == '"')
          {
          pc_Scan ++;
          return ScanString ('"', ec_CTokWideStringLit);
          }
        else
          {
          if (* pc_Scan == '\'')
            {
            pc_Scan ++;
            return ScanString ('\'', ec_CTokWideCharLit);
            }
          }
        }

    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
      return ScanIdf ();

    case '[':
      return ec_CTokOpenBracket;

    case '\\':
      while (InText () && (* pc_Scan <= ' '))
        {
        if (* pc_Scan == '\n')
          {
          u_LineToken --;
          pc_Scan ++; // NewLine
          return ec_CTokLineConcat;
          }
        else
          pc_Scan ++;
        }

      return ec_CTokScanError;

    case ']':
      return ec_CTokCloseBracket;

    case '^':
      if (InText () && (* pc_Scan == '='))
        {
        pc_Scan ++;
        return ec_CTokXorAssign;
        }
      else
        return ec_CTokXor;

    case '_':
      return ScanIdf ();

    case '`':
      return ec_CTokScanError;

    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
    case 's':
    case 't':
    case 'u':
    case 'v':
    case 'w':
    case 'x':
    case 'y':
    case 'z':
      return ScanIdf ();

    case '{':
      return ec_CTokOpenBrace;

    case '|':
      if (InText ())
        {
        if (* pc_Scan == '|')
          {
          pc_Scan ++;
          return ec_CTokLogOr;
          }
        else
          {
          if (* pc_Scan == '=')
            {
            pc_Scan ++;
            return ec_CTokOrAssign;
            }
          }
        }

      return ec_CTokBitOr;

    case '}':
      return ec_CTokCloseBrace;

    case '~':
      return ec_CTokTilde;

    default:
      return ec_CTokScanError;
    }
  }
