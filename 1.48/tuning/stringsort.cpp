
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
// File tuning/stringsort.cpp

#include "tuning/stringsort.hpp"
#include "tuning/chn/store.hpp"
#include "tuning/sys/cmemory.hpp"

//---------------------------------------------------------------------------

static const char ac_DefaultSortPage [256] =
  {
  '\000', '\001', '\002', '\003', '\004', '\005', '\006', '\007',
  '\010', '\011', '\012', '\013', '\014', '\015', '\016', '\017',
  '\020', '\021', '\022', '\023', '\024', '\025', '\026', '\027',
  '\030', '\031', '\032', '\033', '\034', '\035', '\036', '\037',
  '\040', '\041', '\042', '\043', '\044', '\045', '\046', '\047',
  '\050', '\051', '\052', '\053', '\054', '\055', '\056', '\057',
  '\060', '\061', '\062', '\063', '\064', '\065', '\066', '\067',
  '\070', '\071', '\072', '\073', '\074', '\075', '\076', '\077',
  '\100', '\101', '\102', '\103', '\104', '\105', '\106', '\107',
  '\110', '\111', '\112', '\113', '\114', '\115', '\116', '\117',
  '\120', '\121', '\122', '\123', '\124', '\125', '\126', '\127',
  '\130', '\131', '\132', '\133', '\134', '\135', '\136', '\137',
  '\140', '\141', '\142', '\143', '\144', '\145', '\146', '\147',
  '\150', '\151', '\152', '\153', '\154', '\155', '\156', '\157',
  '\160', '\161', '\162', '\163', '\164', '\165', '\166', '\167',
  '\170', '\171', '\172', '\173', '\174', '\175', '\176', '\177',
  '\200', '\201', '\202', '\203', '\204', '\205', '\206', '\207',
  '\210', '\211', '\212', '\213', '\214', '\215', '\216', '\217',
  '\220', '\221', '\222', '\223', '\224', '\225', '\226', '\227',
  '\230', '\231', '\232', '\233', '\234', '\235', '\236', '\237',
  '\240', '\241', '\242', '\243', '\244', '\245', '\246', '\247',
  '\250', '\251', '\252', '\253', '\254', '\255', '\256', '\257',
  '\260', '\261', '\262', '\263', '\264', '\265', '\266', '\267',
  '\270', '\271', '\272', '\273', '\274', '\275', '\276', '\277',
  '\300', '\301', '\302', '\303', '\304', '\305', '\306', '\307',
  '\310', '\311', '\312', '\313', '\314', '\315', '\316', '\317',
  '\320', '\321', '\322', '\323', '\324', '\325', '\326', '\327',
  '\330', '\331', '\332', '\333', '\334', '\335', '\336', '\337',
  '\340', '\341', '\342', '\343', '\344', '\345', '\346', '\347',
  '\350', '\351', '\352', '\353', '\354', '\355', '\356', '\357',
  '\360', '\361', '\362', '\363', '\364', '\365', '\366', '\367',
  '\370', '\371', '\372', '\373', '\374', '\375', '\376', '\377'
  };

//---------------------------------------------------------------------------

inline ct_StringSort::ct_SortArray::ct_SortArray ()
  {
  tl_FillMemory ((char *) ai_SortArray, sizeof (ai_SortArray), (char) -1);
  pco_Next = 0;
  }

//---------------------------------------------------------------------------

inline ct_StringSort::ct_SortArray::~ct_SortArray ()
  {
  delete pco_Next;
  }

//---------------------------------------------------------------------------

inline void * ct_StringSort::ct_SortArray::operator new
(
size_t u_size
)
  {
  return GetChnStore ()-> Alloc (u_size);
  }

//---------------------------------------------------------------------------

inline void ct_StringSort::ct_SortArray::operator delete
(
void * pv
)
  {
  GetChnStore ()-> Free (pv);
  }

//---------------------------------------------------------------------------

const char * ct_StringSort::GetDefaultSortPage ()
  {
  return ac_DefaultSortPage;
  }

//---------------------------------------------------------------------------

bool ct_StringSort::SubSort
(
ct_SortArray * pco_sortArray,
t_Int i_first,
t_Int i_idx
)
  {
  if (pco_sortArray-> pco_Next != 0)
    pco_sortArray = pco_sortArray-> pco_Next;
  else
    {
    if ((pco_sortArray = (pco_sortArray-> pco_Next = new ct_SortArray)) == 0)
      return false;
    }

  t_Int * pi_sortArray = pco_sortArray-> ai_SortArray;
  unsigned char c;
  unsigned char c_min = 255;
  unsigned char c_max = 1;
  t_Int i_tmp;

  while (i_first >= 0)
    {
    if ((c = pc_SortPage [ppc_Strings [i_first] [i_idx]]) == '\0')
      {
      * (pi_Result ++) = i_first;
      i_first = pi_Temp [i_first];
      }
    else
      {
      i_tmp = pi_Temp [i_first];

      if (c < c_min)
        c_min = c;

      if (c > c_max)
        c_max = c;

      pi_Temp [i_first] = pi_sortArray [c];
      pi_sortArray [c] = i_first;
      i_first = i_tmp;
      }
    }

  for (i_tmp = c_min; i_tmp <= c_max; i_tmp ++)
    {
    i_first = pi_sortArray [i_tmp];

    if (i_first >= 0)
      {
      pi_sortArray [i_tmp] = -1;

      if (pi_Temp [i_first] < 0)
        * (pi_Result ++) = i_first;
      else
        {
        if (! SubSort (pco_sortArray, i_first, i_idx + 1))
          return false;
        }
      }
    }

  return true;
  }

//---------------------------------------------------------------------------

bool ct_StringSort::Sort
(
const char * * ppc_strings, // Input of type char * [i_numOfStrings]
t_Int * pi_sortedIndex,     // Result of type t_Int [i_numOfStrings]
t_Int i_numOfStrings,       // Number of strings to sort
const char * pc_sortPage    // Sort page like ac_DefaultSortPage
)
  {
  if (pc_sortPage [0] != '\0')
    return false;

  pi_Temp = (t_Int *) GetChnStore ()-> Alloc (sizeof (t_Int) * i_numOfStrings);

  if (pi_Temp == 0)
    return false;

  ct_SortArray co_sortArray;
  unsigned char c;
  t_Int i_tmp;
  bool b_result = true;
  t_Int * pi_sortArray = co_sortArray. ai_SortArray;
  ppc_Strings = (const unsigned char * *) ppc_strings;
  pi_Result = pi_sortedIndex;
  pc_SortPage = (const unsigned char *) pc_sortPage;

  for (i_tmp = 0; i_tmp < i_numOfStrings; i_tmp ++)
    {
    if ((c = pc_SortPage [* ppc_Strings [i_tmp]]) == '\0')
      * (pi_Result ++) = i_tmp;
    else
      {
      pi_Temp [i_tmp] = pi_sortArray [c];
      pi_sortArray [c] = i_tmp;
      }
    }

  for (i_tmp = 1; b_result && (i_tmp <= 255); i_tmp ++)
    {
    t_Int i_first = pi_sortArray [i_tmp];

    if (i_first >= 0)
      {
      if (pi_Temp [i_first] < 0)
        * (pi_Result ++) = i_first;
      else
        b_result = SubSort (& co_sortArray, i_first, 1);
      }
    }

  GetChnStore ()-> Free (pi_Temp);
  return b_result;
  }

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

inline ct_UInt32Sort::ct_SortArray::ct_SortArray ()
  {
  tl_FillMemory ((char *) ai_SortArray, sizeof (ai_SortArray), (char) -1);
  pco_Next = 0;
  }

//---------------------------------------------------------------------------

void ct_UInt32Sort::SubSort
(
ct_SortArray * pco_sortArray,
t_Int i_first,
t_Int i_idx
)
  {
  pco_sortArray = pco_sortArray-> pco_Next;
  t_Int * pi_sortArray = pco_sortArray-> ai_SortArray;
  unsigned char c;
  unsigned char c_min = 255;
  unsigned char c_max = 0;
  t_Int i_tmp;

  while (i_first >= 0)
    {
    c = pu_Ints [i_first] [i_idx];
    i_tmp = pi_Temp [i_first];

    if (c < c_min)
      c_min = c;

    if (c > c_max)
      c_max = c;

    pi_Temp [i_first] = pi_sortArray [c];
    pi_sortArray [c] = i_first;
    i_first = i_tmp;
    }

  for (i_tmp = c_min; i_tmp <= c_max; i_tmp ++)
    {
    i_first = pi_sortArray [i_tmp];

    if (i_first >= 0)
      {
      pi_sortArray [i_tmp] = -1;

      if (pi_Temp [i_first] < 0)
        * (pi_Result ++) = i_first;
      else
        {
        if (i_idx == 0)
          {
          while (i_first >= 0)
            {
            * (pi_Result ++) = i_first;
            i_first = pi_Temp [i_first];
            }
          }
        else
          SubSort (pco_sortArray, i_first, i_idx - 1);
        }
      }
    }
  }

//---------------------------------------------------------------------------

bool ct_UInt32Sort::Sort
(
const t_UInt32 * pu_ints, // Input of type t_UInt32 [i_numOfInts]
t_Int * pi_sortedIndex,   // Result of type t_Int [i_numOfInts]
t_Int i_numOfInts         // Number of ints to sort
)
  {
  pi_Temp = (t_Int *) GetChnStore ()-> Alloc (sizeof (t_Int) * i_numOfInts);

  if (pi_Temp == 0)
    return false;

  ct_SortArray aco_sortArray [4];
  aco_sortArray [0]. pco_Next = aco_sortArray + 1;
  aco_sortArray [1]. pco_Next = aco_sortArray + 2;
  aco_sortArray [2]. pco_Next = aco_sortArray + 3;
  unsigned char c;
  t_Int i_tmp;
  t_Int * pi_sortArray = aco_sortArray [0]. ai_SortArray;
  pu_Ints = (const t_Char4 *) pu_ints;
  pi_Result = pi_sortedIndex;

  for (i_tmp = 0; i_tmp < i_numOfInts; i_tmp ++)
    {
    c = pu_Ints [i_tmp] [3];
    pi_Temp [i_tmp] = pi_sortArray [c];
    pi_sortArray [c] = i_tmp;
    }

  for (i_tmp = 0; i_tmp <= 255; i_tmp ++)
    {
    t_Int i_first = pi_sortArray [i_tmp];

    if (i_first >= 0)
      {
      if (pi_Temp [i_first] < 0)
        * (pi_Result ++) = i_first;
      else
        SubSort (aco_sortArray, i_first, 2);
      }
    }

  GetChnStore ()-> Free (pi_Temp);
  return true;
  }
