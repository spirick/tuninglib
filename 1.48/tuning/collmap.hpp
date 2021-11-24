
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
// File tuning/collmap.hpp

#ifndef TUNING_COLLMAP_HPP
#define TUNING_COLLMAP_HPP

#include "tuning/collection.hpp"

//---------------------------------------------------------------------------

#define COLLMAP_DCL_(CollName, t_base)                                        \
  class TL_EXPORT ct_ ## CollName ## Container;                               \
  class TL_EXPORT ct_ ## CollName: public t_base                              \
    {                                                                         \
    typedef ct_ ## CollName ## Container t_Cont;                              \
    t_Cont *             pco_Cont;                                            \
  public:                                                                     \
                         ct_ ## CollName ();                                  \
                         ct_ ## CollName (const ct_ ## CollName & co_init);   \
                         ~ct_ ## CollName ();                                 \
    ct_ ## CollName &    operator = (const ct_ ## CollName & co_asgn);        \
    void                 Swap (ct_ ## CollName & co_swap);                    \
    TL_CLASSID (ct_ ## CollName)                                              \
    virtual bool         IsEmpty () const;                                    \
    virtual t_Length     GetLen () const;                                     \
    virtual t_Position   First () const;                                      \
    virtual t_Position   Last () const;                                       \
    virtual t_Position   Next (t_Position o_pos) const;                       \
    virtual t_Position   Prev (t_Position o_pos) const;                       \
    virtual t_Position   Nth (t_Length u_idx) const;                          \
    virtual ct_Object *  GetPtr (t_Position o_pos) const;                     \
    virtual ct_Object *  GetFirstPtr () const;                                \
    virtual ct_Object *  GetLastPtr () const;                                 \
    virtual ct_Object *  GetNextPtr (t_Position o_pos) const;                 \
    virtual ct_Object *  GetPrevPtr (t_Position o_pos) const;                 \
    virtual ct_Object *  GetNthPtr (t_Length u_idx) const;                    \
    virtual t_Position   AddPtr (const ct_Object * po_obj);                   \
    virtual t_Position   AddPtrBefore (t_Position o_pos,                      \
                           const ct_Object * po_obj);                         \
    virtual t_Position   AddPtrAfter (t_Position o_pos,                       \
                           const ct_Object * po_obj);                         \
    virtual t_Position   AddPtrBeforeFirst (const ct_Object * po_obj);        \
    virtual t_Position   AddPtrAfterLast (const ct_Object * po_obj);          \
    virtual t_Position   AddPtrBeforeNth (t_Length u_idx,                     \
                           const ct_Object * po_obj);                         \
    virtual t_Position   AddPtrAfterNth (t_Length u_idx,                      \
                           const ct_Object * po_obj);                         \
    virtual t_Position   DelPtr (t_Position o_pos);                           \
    virtual t_Position   DelFirstPtr ();                                      \
    virtual t_Position   DelLastPtr ();                                       \
    virtual t_Position   DelNextPtr (t_Position o_pos);                       \
    virtual t_Position   DelPrevPtr (t_Position o_pos);                       \
    virtual t_Position   DelNthPtr (t_Length u_idx);                          \
    virtual void         DelAllPtr ();                                        \
    virtual t_Position   DelPtrAndObj (t_Position o_pos);                     \
    virtual t_Position   DelFirstPtrAndObj ();                                \
    virtual t_Position   DelLastPtrAndObj ();                                 \
    virtual t_Position   DelNextPtrAndObj (t_Position o_pos);                 \
    virtual t_Position   DelPrevPtrAndObj (t_Position o_pos);                 \
    virtual t_Position   DelNthPtrAndObj (t_Length u_idx);                    \
    virtual void         DelAllPtrAndObj ();                                  \
    virtual bool         ContainsPtr (const ct_Object * po_obj) const;        \
    virtual t_Length     CountPtrs (const ct_Object * po_obj) const;          \
    virtual t_Position   SearchFirstPtr (const ct_Object * po_obj) const;     \
    virtual t_Position   SearchLastPtr (const ct_Object * po_obj) const;      \
    virtual t_Position   SearchNextPtr (t_Position o_pos,                     \
                           const ct_Object * po_obj) const;                   \
    virtual t_Position   SearchPrevPtr (t_Position o_pos,                     \
                           const ct_Object * po_obj) const;                   \
    virtual t_Position   AddPtrCond (const ct_Object * po_obj);               \
    virtual t_Position   AddPtrBeforeFirstCond (const ct_Object * po_obj);    \
    virtual t_Position   AddPtrAfterLastCond (const ct_Object * po_obj);      \
    virtual t_Position   DelFirstEqualPtr (const ct_Object * po_obj);         \
    virtual t_Position   DelLastEqualPtr (const ct_Object * po_obj);          \
    virtual t_Position   DelFirstEqualPtrCond (const ct_Object * po_obj);     \
    virtual t_Position   DelLastEqualPtrCond (const ct_Object * po_obj);      \
    virtual t_Position   DelFirstEqualPtrAndObj (const ct_Object * po_obj);   \
    virtual t_Position   DelLastEqualPtrAndObj (const ct_Object * po_obj);    \
    virtual t_Position   DelFirstEqualPtrAndObjCond                           \
                           (const ct_Object * po_obj);                        \
    virtual t_Position   DelLastEqualPtrAndObjCond                            \
                           (const ct_Object * po_obj);

#define COLLMAP_DCL(CollName)                                                 \
  COLLMAP_DCL_ (CollName, ct_Collection) };

//---------------------------------------------------------------------------

#define COLLMAP_DEF_(CollName, ContName, t_base)                              \
  TL_ISKINDOF (ct_ ## CollName, t_base)                                       \
  class TL_EXPORT ct_ ## CollName ## Container: public ContName <ct_Object> { }; \
  ct_ ## CollName::ct_ ## CollName ()                                         \
    {                                                                         \
    pco_Cont = new ct_ ## CollName ## Container;                              \
    }                                                                         \
  ct_ ## CollName::ct_ ## CollName (const ct_ ## CollName & co_init)          \
    {                                                                         \
    pco_Cont = new ct_ ## CollName ## Container (* co_init. pco_Cont);        \
    }                                                                         \
  ct_ ## CollName::~ct_ ## CollName ()                                        \
    {                                                                         \
    delete pco_Cont;                                                          \
    }                                                                         \
  ct_ ## CollName & ct_ ## CollName::operator =                               \
   (const ct_ ## CollName & co_asgn)                                          \
    {                                                                         \
    * pco_Cont = * co_asgn. pco_Cont;                                         \
    return * this;                                                            \
    }                                                                         \
  void ct_ ## CollName::Swap (ct_ ## CollName & co_swap)                      \
    {                                                                         \
    tl_SwapObj (pco_Cont, co_swap. pco_Cont);                                 \
    }                                                                         \
  bool ct_ ## CollName::IsEmpty () const                                      \
    {                                                                         \
    return pco_Cont-> IsEmpty ();                                             \
    }                                                                         \
  ct_Collection::t_Length ct_ ## CollName::GetLen () const                    \
    {                                                                         \
    return (t_Length) pco_Cont-> GetLen ();                                   \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::First () const                   \
    {                                                                         \
    return (t_Position) pco_Cont-> First ();                                  \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::Last () const                    \
    {                                                                         \
    return (t_Position) pco_Cont-> Last ();                                   \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::Next (t_Position o_pos) const    \
    {                                                                         \
    return (t_Position) pco_Cont-> Next ((t_Cont::t_Position) o_pos);         \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::Prev (t_Position o_pos) const    \
    {                                                                         \
    return (t_Position) pco_Cont-> Prev ((t_Cont::t_Position) o_pos);         \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::Nth (t_Length u_idx) const       \
    {                                                                         \
    return (t_Position) pco_Cont-> Nth (u_idx);                               \
    }                                                                         \
  ct_Object * ct_ ## CollName::GetPtr (t_Position o_pos) const                \
    {                                                                         \
    return pco_Cont-> GetPtr ((t_Cont::t_Position) o_pos);                    \
    }                                                                         \
  ct_Object * ct_ ## CollName::GetFirstPtr () const                           \
    {                                                                         \
    return pco_Cont-> GetFirstPtr ();                                         \
    }                                                                         \
  ct_Object * ct_ ## CollName::GetLastPtr () const                            \
    {                                                                         \
    return pco_Cont-> GetLastPtr ();                                          \
    }                                                                         \
  ct_Object * ct_ ## CollName::GetNextPtr (t_Position o_pos) const            \
    {                                                                         \
    return pco_Cont-> GetNextPtr ((t_Cont::t_Position) o_pos);                \
    }                                                                         \
  ct_Object * ct_ ## CollName::GetPrevPtr (t_Position o_pos) const            \
    {                                                                         \
    return pco_Cont-> GetPrevPtr ((t_Cont::t_Position) o_pos);                \
    }                                                                         \
  ct_Object * ct_ ## CollName::GetNthPtr (t_Length u_idx) const               \
    {                                                                         \
    return pco_Cont-> GetNthPtr (u_idx);                                      \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::AddPtr (const ct_Object * po_obj) \
    {                                                                         \
    return (t_Position) pco_Cont-> AddPtr (po_obj);                           \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::AddPtrBefore                     \
   (t_Position o_pos, const ct_Object * po_obj)                               \
    {                                                                         \
    return (t_Position) pco_Cont->                                            \
      AddPtrBefore ((t_Cont::t_Position) o_pos, po_obj);                      \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::AddPtrAfter                      \
   (t_Position o_pos, const ct_Object * po_obj)                               \
    {                                                                         \
    return (t_Position) pco_Cont->                                            \
      AddPtrAfter ((t_Cont::t_Position) o_pos, po_obj);                       \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::AddPtrBeforeFirst (const ct_Object * po_obj) \
    {                                                                         \
    return (t_Position) pco_Cont-> AddPtrBeforeFirst (po_obj);                \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::AddPtrAfterLast (const ct_Object * po_obj) \
    {                                                                         \
    return (t_Position) pco_Cont-> AddPtrAfterLast (po_obj);                  \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::AddPtrBeforeNth                  \
   (t_Length u_idx, const ct_Object * po_obj)                                 \
    {                                                                         \
    return (t_Position) pco_Cont-> AddPtrBeforeNth (u_idx, po_obj);           \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::AddPtrAfterNth                   \
   (t_Length u_idx, const ct_Object * po_obj)                                 \
    {                                                                         \
    return (t_Position) pco_Cont-> AddPtrAfterNth (u_idx, po_obj);            \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::DelPtr (t_Position o_pos)        \
    {                                                                         \
    return (t_Position) pco_Cont-> DelPtr ((t_Cont::t_Position) o_pos);       \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::DelFirstPtr ()                   \
    {                                                                         \
    return (t_Position) pco_Cont-> DelFirstPtr ();                            \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::DelLastPtr ()                    \
    {                                                                         \
    return (t_Position) pco_Cont-> DelLastPtr ();                             \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::DelNextPtr (t_Position o_pos)    \
    {                                                                         \
    return (t_Position) pco_Cont-> DelNextPtr ((t_Cont::t_Position) o_pos);   \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::DelPrevPtr (t_Position o_pos)    \
    {                                                                         \
    return (t_Position) pco_Cont-> DelPrevPtr ((t_Cont::t_Position) o_pos);   \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::DelNthPtr (t_Length u_idx)       \
    {                                                                         \
    return (t_Position) pco_Cont-> DelNthPtr (u_idx);                         \
    }                                                                         \
  void ct_ ## CollName::DelAllPtr ()                                          \
    {                                                                         \
    pco_Cont-> DelAllPtr ();                                                  \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::DelPtrAndObj (t_Position o_pos)  \
    {                                                                         \
    return (t_Position) pco_Cont-> DelPtrAndObj ((t_Cont::t_Position) o_pos); \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::DelFirstPtrAndObj ()             \
    {                                                                         \
    return (t_Position) pco_Cont-> DelFirstPtrAndObj ();                      \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::DelLastPtrAndObj ()              \
    {                                                                         \
    return (t_Position) pco_Cont-> DelLastPtrAndObj ();                       \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::DelNextPtrAndObj (t_Position o_pos) \
    {                                                                         \
    return (t_Position) pco_Cont-> DelNextPtrAndObj((t_Cont::t_Position)o_pos); \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::DelPrevPtrAndObj (t_Position o_pos) \
    {                                                                         \
    return (t_Position) pco_Cont-> DelPrevPtrAndObj((t_Cont::t_Position)o_pos); \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::DelNthPtrAndObj (t_Length u_idx) \
    {                                                                         \
    return (t_Position) pco_Cont-> DelNthPtrAndObj (u_idx);                   \
    }                                                                         \
  void ct_ ## CollName::DelAllPtrAndObj ()                                    \
    {                                                                         \
    pco_Cont-> DelAllPtrAndObj ();                                            \
    }                                                                         \
  bool ct_ ## CollName::ContainsPtr (const ct_Object * po_obj) const          \
    {                                                                         \
    return pco_Cont-> ContainsPtr (po_obj);                                   \
    }                                                                         \
  ct_Collection::t_Length ct_ ## CollName::CountPtrs (const ct_Object * po_obj) const \
    {                                                                         \
    return (t_Length) pco_Cont-> CountPtrs (po_obj);                          \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::SearchFirstPtr (const ct_Object * po_obj) const \
    {                                                                         \
    return (t_Position) pco_Cont-> SearchFirstPtr (po_obj);                   \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::SearchLastPtr (const ct_Object * po_obj) const \
    {                                                                         \
    return (t_Position) pco_Cont-> SearchLastPtr (po_obj);                    \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::SearchNextPtr                    \
   (t_Position o_pos, const ct_Object * po_obj) const                         \
    {                                                                         \
    return (t_Position) pco_Cont->                                            \
      SearchNextPtr ((t_Cont::t_Position) o_pos, po_obj);                     \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::SearchPrevPtr                    \
   (t_Position o_pos, const ct_Object * po_obj) const                         \
    {                                                                         \
    return (t_Position) pco_Cont->                                            \
      SearchPrevPtr ((t_Cont::t_Position) o_pos, po_obj);                     \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::AddPtrCond (const ct_Object * po_obj) \
    {                                                                         \
    return (t_Position) pco_Cont-> AddPtrCond (po_obj);                       \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::AddPtrBeforeFirstCond (const ct_Object * po_obj) \
    {                                                                         \
    return (t_Position) pco_Cont-> AddPtrBeforeFirstCond (po_obj);            \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::AddPtrAfterLastCond (const ct_Object * po_obj) \
    {                                                                         \
    return (t_Position) pco_Cont-> AddPtrAfterLastCond (po_obj);              \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::DelFirstEqualPtr (const ct_Object * po_obj) \
    {                                                                         \
    return (t_Position) pco_Cont-> DelFirstEqualPtr (po_obj);                 \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::DelLastEqualPtr (const ct_Object * po_obj) \
    {                                                                         \
    return (t_Position) pco_Cont-> DelLastEqualPtr (po_obj);                  \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::DelFirstEqualPtrCond (const ct_Object * po_obj) \
    {                                                                         \
    return (t_Position) pco_Cont-> DelFirstEqualPtrCond (po_obj);             \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::DelLastEqualPtrCond (const ct_Object * po_obj) \
    {                                                                         \
    return (t_Position) pco_Cont-> DelLastEqualPtrCond (po_obj);              \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::DelFirstEqualPtrAndObj           \
   (const ct_Object * po_obj)                                                 \
    {                                                                         \
    return (t_Position) pco_Cont-> DelFirstEqualPtrAndObj (po_obj);           \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::DelLastEqualPtrAndObj            \
   (const ct_Object * po_obj)                                                 \
    {                                                                         \
    return (t_Position) pco_Cont-> DelLastEqualPtrAndObj (po_obj);            \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::DelFirstEqualPtrAndObjCond       \
   (const ct_Object * po_obj)                                                 \
    {                                                                         \
    return (t_Position) pco_Cont-> DelFirstEqualPtrAndObjCond (po_obj);       \
    }                                                                         \
  ct_Collection::t_Position ct_ ## CollName::DelLastEqualPtrAndObjCond        \
   (const ct_Object * po_obj)                                                 \
    {                                                                         \
    return (t_Position) pco_Cont-> DelLastEqualPtrAndObjCond (po_obj);        \
    }

#define COLLMAP_DEF(CollName, ContName)                                       \
  COLLMAP_DEF_ (CollName, ContName, ct_Collection)

#endif
