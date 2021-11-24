
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
// File sourcedep/token.hpp

#ifndef SOURCEDEP_TOKEN_HPP
#define SOURCEDEP_TOKEN_HPP

#include "tuning/defs.hpp"

//---------------------------------------------------------------------------

typedef unsigned long t_Token; // Only 24 bits are used

const t_Token o_Literals    = 0x00010000l;
const t_Token o_Identifiers = 0x00020000l;

enum et_Tokens
  {
  ec_TokNoToken = 0,

  ec_CTokInclude,
  ec_CTokPreproc,
  ec_CTokLastKeyword,   // Delimiter

  // Symbolic C/C++ tokens

  ec_CTokIdf,           // Identifier
  ec_CTokNumLit,        // 1
  ec_CTokCharLit,       // 'a'
  ec_CTokWideCharLit,   // L'a'
  ec_CTokStringLit,     // "abc"
  ec_CTokWideStringLit, // L"abc"
  ec_CTokPPBegin,       // "#" at line begin
  ec_CTokStringize,     // #
  ec_CTokCharize,       // #@
  ec_CTokTokenConcat,   // ##
  ec_CTokSrcFileName,   // "filename"
  ec_CTokStdFileName,   // <filename>
  ec_CTokSkippedText,   // skipped text (len >= 0, excl. '\n')
  ec_CTokAsmText,       // asm move ax,bx
  ec_CTokLineConcat,    // '\\' at line end (incl. '\n')
  ec_CTokCComment,      // "/*" comment
  ec_CTokCppComment,    // "//" comment (excl. '\n')
  ec_CTokLogNot,        // !
  ec_CTokMod,           // %
  ec_CTokAmpers,        // &
  ec_CTokOpenPar,       // (
  ec_CTokClosePar,      // )
  ec_CTokStar,          // *
  ec_CTokPlus,          // +
  ec_CTokComma,         // ,
  ec_CTokMinus,         // -
  ec_CTokDot,           // .
  ec_CTokDiv,           // /
  ec_CTokColon,         // :
  ec_CTokSemicolon,     // ;
  ec_CTokLower,         // <
  ec_CTokAssign,        // =
  ec_CTokGreater,       // >
  ec_CTokLogIf,         // ?
  ec_CTokOpenBracket,   // [
  ec_CTokCloseBracket,  // ]
  ec_CTokXor,           // ^
  ec_CTokOpenBrace,     // {
  ec_CTokBitOr,         // |
  ec_CTokCloseBrace,    // }
  ec_CTokTilde,         // ~
  ec_CTokNotEqual,      // !=
  ec_CTokModAssign,     // %=
  ec_CTokLogAnd,        // &&
  ec_CTokAndAssign,     // &=
  ec_CTokMultAssign,    // *=
  ec_CTokIncrement,     // ++
  ec_CTokPlusAssign,    // +=
  ec_CTokDecrement,     // --
  ec_CTokMinusAssign,   // -=
  ec_CTokArrow,         // ->
  ec_CTokArrowStar,     // ->*
  ec_CTokDotStar,       // .*
  ec_CTokEllipsis,      // ...
  ec_CTokDivAssign,     // /=
  ec_CTokScopeReso,     // ::
  ec_CTokLeftShift,     // <<
  ec_CTokLsAssign,      // <<=
  ec_CTokLowerEqual,    // <=
  ec_CTokEqual,         // ==
  ec_CTokGreaterEqual,  // >=
  ec_CTokRightShift,    // >>
  ec_CTokRsAssign,      // >>=
  ec_CTokXorAssign,     // ^=
  ec_CTokOrAssign,      // |=
  ec_CTokLogOr,         // ||
  ec_CTokScanError
  };

#endif
