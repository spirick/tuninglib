
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
// File tuning/timedate.hpp

#ifndef TUNING_TIMEDATE_HPP
#define TUNING_TIMEDATE_HPP

#include "tuning/sys/ctimedate.hpp"

//---------------------------------------------------------------------------

class TL_EXPORT ct_TimeDate
  {
  t_UInt16             u_Year;        // 1970 - 2099
  t_UInt8              u_Month;       // 1 - 12
  t_UInt8              u_Day;         // 1 - 31
  t_UInt8              u_DayOfWeek;   // 0 - 6; 0: Sunday
  t_UInt8              u_Hour;        // 0 - 23
  t_UInt8              u_Minute;      // 0 - 59
  t_UInt8              u_Second;      // 0 - 59
  t_UInt32             u_MicroSecond; // 0 - 999999

public:
                       ct_TimeDate ();
                       ct_TimeDate (t_MicroTime i_time);

  void                 Clear ();
  t_MicroTime          GetTime () const;
  void                 SetTime (t_MicroTime i_time);

  void                 QueryUTCTime ();
  void                 QueryLocalTime ();

  inline unsigned      GetYear () const;
  inline unsigned      GetMonth () const;
  inline unsigned      GetDay () const;
  inline unsigned      GetDayOfWeek () const;
  inline unsigned      GetHour () const;
  inline unsigned      GetMinute () const;
  inline unsigned      GetSecond () const;
  inline unsigned      GetMicroSecond () const;

  inline void          SetYear (unsigned u);
  inline void          SetMonth (unsigned u);
  inline void          SetDay (unsigned u);
  inline void          SetDayOfWeek (unsigned u);
  inline void          SetHour (unsigned u);
  inline void          SetMinute (unsigned u);
  inline void          SetSecond (unsigned u);
  inline void          SetMicroSecond (unsigned u);

  inline bool          operator == (const ct_TimeDate & co_td) const;
  inline bool          operator != (const ct_TimeDate & co_td) const;
  inline bool          operator <  (const ct_TimeDate & co_td) const;
  inline bool          operator <= (const ct_TimeDate & co_td) const;
  inline bool          operator >  (const ct_TimeDate & co_td) const;
  inline bool          operator >= (const ct_TimeDate & co_td) const;
  };

//---------------------------------------------------------------------------

inline unsigned ct_TimeDate::GetYear () const
  { return u_Year; }

inline unsigned ct_TimeDate::GetMonth () const
  { return u_Month; }

inline unsigned ct_TimeDate::GetDay () const
  { return u_Day; }

inline unsigned ct_TimeDate::GetDayOfWeek () const
  { return u_DayOfWeek; }

inline unsigned ct_TimeDate::GetHour () const
  { return u_Hour; }

inline unsigned ct_TimeDate::GetMinute () const
  { return u_Minute; }

inline unsigned ct_TimeDate::GetSecond () const
  { return u_Second; }

inline unsigned ct_TimeDate::GetMicroSecond () const
  { return u_MicroSecond; }

//---------------------------------------------------------------------------

inline void ct_TimeDate::SetYear (unsigned u)
  { u_Year = (t_UInt16) u; }

inline void ct_TimeDate::SetMonth (unsigned u)
  { u_Month = (t_UInt8) u; }

inline void ct_TimeDate::SetDay (unsigned u)
  { u_Day = (t_UInt8) u; }

inline void ct_TimeDate::SetDayOfWeek (unsigned u)
  { u_DayOfWeek = (t_UInt8) u; }

inline void ct_TimeDate::SetHour (unsigned u)
  { u_Hour = (t_UInt8) u; }

inline void ct_TimeDate::SetMinute (unsigned u)
  { u_Minute = (t_UInt8) u; }

inline void ct_TimeDate::SetSecond (unsigned u)
  { u_Second = (t_UInt8) u; }

inline void ct_TimeDate::SetMicroSecond (unsigned u)
  { u_MicroSecond = u; }

//---------------------------------------------------------------------------

inline bool ct_TimeDate::operator == (const ct_TimeDate & co_td) const
  { return GetTime () == co_td. GetTime (); }

inline bool ct_TimeDate::operator != (const ct_TimeDate & co_td) const
  { return GetTime () != co_td. GetTime (); }

inline bool ct_TimeDate::operator <  (const ct_TimeDate & co_td) const
  { return GetTime () <  co_td. GetTime (); }

inline bool ct_TimeDate::operator <= (const ct_TimeDate & co_td) const
  { return GetTime () <= co_td. GetTime (); }

inline bool ct_TimeDate::operator >  (const ct_TimeDate & co_td) const
  { return GetTime () >  co_td. GetTime (); }

inline bool ct_TimeDate::operator >= (const ct_TimeDate & co_td) const
  { return GetTime () >= co_td. GetTime (); }

#endif
