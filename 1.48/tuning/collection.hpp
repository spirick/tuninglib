
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
// File tuning/collection.hpp

#ifndef TUNING_COLLECTION_HPP
#define TUNING_COLLECTION_HPP

#include "tuning/object.hpp"

//---------------------------------------------------------------------------

class TL_EXPORT ct_Collection: public ct_Object
  {
public:
  typedef t_UInt       t_Length;
  typedef t_UInt       t_Position;

  TL_CLASSID (ct_Collection)

  virtual bool         IsEmpty () const = 0;
  virtual t_Length     GetLen () const = 0;

  virtual t_Position   First () const = 0;
  virtual t_Position   Last () const = 0;
  virtual t_Position   Next (t_Position o_pos) const = 0;
  virtual t_Position   Prev (t_Position o_pos) const = 0;
  virtual t_Position   Nth (t_Length u_idx) const = 0;

  virtual ct_Object *  GetPtr (t_Position o_pos) const = 0;
  virtual ct_Object *  GetFirstPtr () const = 0;
  virtual ct_Object *  GetLastPtr () const = 0;
  virtual ct_Object *  GetNextPtr (t_Position o_pos) const = 0;
  virtual ct_Object *  GetPrevPtr (t_Position o_pos) const = 0;
  virtual ct_Object *  GetNthPtr (t_Length u_idx) const = 0;

  virtual t_Position   AddPtr (const ct_Object * po_obj) = 0;
  virtual t_Position   AddPtrBefore (t_Position o_pos,
                         const ct_Object * po_obj) = 0;
  virtual t_Position   AddPtrAfter (t_Position o_pos,
                         const ct_Object * po_obj) = 0;
  virtual t_Position   AddPtrBeforeFirst (const ct_Object * po_obj) = 0;
  virtual t_Position   AddPtrAfterLast (const ct_Object * po_obj) = 0;
  virtual t_Position   AddPtrBeforeNth (t_Length u_idx,
                         const ct_Object * po_obj) = 0;
  virtual t_Position   AddPtrAfterNth (t_Length u_idx,
                         const ct_Object * po_obj) = 0;

  virtual t_Position   DelPtr (t_Position o_pos) = 0;
  virtual t_Position   DelFirstPtr () = 0;
  virtual t_Position   DelLastPtr () = 0;
  virtual t_Position   DelNextPtr (t_Position o_pos) = 0;
  virtual t_Position   DelPrevPtr (t_Position o_pos) = 0;
  virtual t_Position   DelNthPtr (t_Length u_idx) = 0;
  virtual void         DelAllPtr () = 0;

  virtual t_Position   DelPtrAndObj (t_Position o_pos) = 0;
  virtual t_Position   DelFirstPtrAndObj () = 0;
  virtual t_Position   DelLastPtrAndObj () = 0;
  virtual t_Position   DelNextPtrAndObj (t_Position o_pos) = 0;
  virtual t_Position   DelPrevPtrAndObj (t_Position o_pos) = 0;
  virtual t_Position   DelNthPtrAndObj (t_Length u_idx) = 0;
  virtual void         DelAllPtrAndObj () = 0;

  virtual bool         ContainsPtr (const ct_Object * po_obj) const = 0;
  virtual t_Length     CountPtrs (const ct_Object * po_obj) const = 0;

  virtual t_Position   SearchFirstPtr (const ct_Object * po_obj) const = 0;
  virtual t_Position   SearchLastPtr (const ct_Object * po_obj) const = 0;
  virtual t_Position   SearchNextPtr (t_Position o_pos,
                         const ct_Object * po_obj) const = 0;
  virtual t_Position   SearchPrevPtr (t_Position o_pos,
                         const ct_Object * po_obj) const = 0;

  virtual t_Position   AddPtrCond (const ct_Object * po_obj) = 0;
  virtual t_Position   AddPtrBeforeFirstCond (const ct_Object * po_obj) = 0;
  virtual t_Position   AddPtrAfterLastCond (const ct_Object * po_obj) = 0;

  virtual t_Position   DelFirstEqualPtr (const ct_Object * po_obj) = 0;
  virtual t_Position   DelLastEqualPtr (const ct_Object * po_obj) = 0;
  virtual t_Position   DelFirstEqualPtrCond (const ct_Object * po_obj) = 0;
  virtual t_Position   DelLastEqualPtrCond (const ct_Object * po_obj) = 0;

  virtual t_Position   DelFirstEqualPtrAndObj (const ct_Object * po_obj) = 0;
  virtual t_Position   DelLastEqualPtrAndObj (const ct_Object * po_obj) = 0;
  virtual t_Position   DelFirstEqualPtrAndObjCond
                         (const ct_Object * po_obj) = 0;
  virtual t_Position   DelLastEqualPtrAndObjCond
                         (const ct_Object * po_obj) = 0;
  };

#endif
