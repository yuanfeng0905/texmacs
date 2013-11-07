
/******************************************************************************
* MODULE     : fast_alloc.hpp
* DESCRIPTION: see fast_alloc.cpp
* COPYRIGHT  : (C) 1999  Joris van der Hoeven
*******************************************************************************
* This software falls under the GNU general public license version 3 or later.
* It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
* in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
******************************************************************************/

#ifndef FAST_ALLOC_H
#define FAST_ALLOC_H
#include "tm_configure.hpp"
#include <stdlib.h>

#include "tm_ostream.hpp"

#define BLOCK_SIZE 65536 // should be >>> MAX_FAST

/******************************************************************************
* Globals
******************************************************************************/

extern char   alloc_table[MAX_FAST];
extern char*  alloc_mem;
extern size_t alloc_remains;
extern int    allocated;
extern int    large_uses;

#define alloc_ptr(i) (*((void **) (alloc_table+i)))
#define ind(ptr) (*((void **) ptr))

/******************************************************************************
* General purpose fast allocation routines
******************************************************************************/

extern void* safe_malloc (register size_t s);
extern void* enlarge_malloc (register size_t s);
extern void* fast_alloc (register size_t s);
extern void  fast_free (register void* ptr, register size_t s);
extern void* fast_new (register size_t s);
extern void  fast_delete (register void* ptr);

extern int   mem_used ();
extern void  mem_info ();

/******************************************************************************
* Fast new and delete
******************************************************************************/

#if (!defined(NO_FAST_ALLOC)) && (!defined(X11TEXMACS))

#ifdef OLD_GNU_COMPILER
inline void* operator new   (register size_t s, void* loc) { return loc; }
inline void* operator new[] (register size_t s, void* loc) { return loc; }
#else
#include <new>
#endif

template<typename C> inline C*
tm_new () {
  void* ptr= fast_new (sizeof (C));
  (void) new (ptr) C ();
  return (C*) ptr;
}

template<typename C, typename A1> inline C*
tm_new (const A1& a1) {
  void* ptr= fast_new (sizeof (C));
  (void) new (ptr) C (a1);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2> inline C*
tm_new (const A1& a1, const A2& a2) {
  void* ptr= fast_new (sizeof (C));
  (void) new (ptr) C (a1, a2);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2> inline C*
tm_new (const A1& a1, A2& a2) {
  void* ptr= fast_new (sizeof (C));
  (void) new (ptr) C (a1, a2);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2> inline C*
tm_new (A1& a1, const A2& a2) {
  void* ptr= fast_new (sizeof (C));
  (void) new (ptr) C (a1, a2);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2> inline C*
tm_new (A1& a1, A2& a2) {
  void* ptr= fast_new (sizeof (C));
  (void) new (ptr) C (a1, a2);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3) {
  void* ptr= fast_new (sizeof (C));
  (void) new (ptr) C (a1, a2, a3);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3> inline C*
tm_new (const A1& a1, A2& a2, A3& a3) {
  void* ptr= fast_new (sizeof (C));
  (void) new (ptr) C (a1, a2, a3);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3> inline C*
tm_new (A1& a1, A2& a2, const A3& a3) {
  void* ptr= fast_new (sizeof (C));
  (void) new (ptr) C (a1, a2, a3);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3> inline C*
tm_new (A1& a1, A2& a2, A3& a3) {
  void* ptr= fast_new (sizeof (C));
  (void) new (ptr) C (a1, a2, a3);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4) {
  void* ptr= fast_new (sizeof (C));
  (void) new (ptr) C (a1, a2, a3, a4);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4> inline C*
tm_new (const A1& a1, A2& a2, A3& a3, A4& a4) {
  void* ptr= fast_new (sizeof (C));
  (void) new (ptr) C (a1, a2, a3, a4);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4, const A5& a5) {
  void* ptr= fast_new (sizeof (C));
  (void) new (ptr) C (a1, a2, a3, a4, a5);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5> inline C*
tm_new (const A1& a1, A2& a2, A3& a3, A4& a4, A5& a5) {
  void* ptr= fast_new (sizeof (C));
  (void) new (ptr) C (a1, a2, a3, a4, a5);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5> inline C*
tm_new (A1& a1, A2& a2, A3& a3, A4& a4, A5& a5) {
  void* ptr= fast_new (sizeof (C));
  (void) new (ptr) C (a1, a2, a3, a4, a5);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4, const A5& a5, const A6& a6) {
  void* ptr= fast_new (sizeof (C));
  (void) new (ptr) C (a1, a2, a3, a4, a5, a6);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6> inline C*
tm_new (A1& a1, const A2& a2, A3& a3, A4& a4, A5& a5, A6& a6) {
  void* ptr= fast_new (sizeof (C));
  (void) new (ptr) C (a1, a2, a3, a4, a5, a6);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6, typename A7> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4, const A5& a5, const A6& a6,
	const A7& a7) {
  void* ptr= fast_new (sizeof (C));
  (void) new (ptr) C (a1, a2, a3, a4, a5, a6, a7);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6, typename A7> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	A4& a4, const A5& a5, const A6& a6,
	const A7& a7) {
  void* ptr= fast_new (sizeof (C));
  (void) new (ptr) C (a1, a2, a3, a4, a5, a6, a7);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6,
	 typename A7, typename A8> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4, const A5& a5, const A6& a6,
	const A7& a7, const A8& a8) {
  void* ptr= fast_new (sizeof (C));
  (void) new (ptr) C (a1, a2, a3, a4, a5, a6, a7, a8);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6,
	 typename A7, typename A8, typename A9> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4, const A5& a5, const A6& a6,
	const A7& a7, const A8& a8, const A9& a9) {
  void* ptr= fast_new (sizeof (C));
  (void) new (ptr) C (a1, a2, a3, a4, a5, a6, a7, a8, a9);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6,
	 typename A7, typename A8, typename A9,
	 typename A10> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4, const A5& a5, const A6& a6,
	const A7& a7, const A8& a8, const A9& a9,
	const A10& a10) {
  void* ptr= fast_new (sizeof (C));
  (void) new (ptr) C (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6,
	 typename A7, typename A8, typename A9,
	 typename A10, typename A11> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4, const A5& a5, const A6& a6,
	const A7& a7, const A8& a8, const A9& a9,
	const A10& a10, const A11& a11) {
  void* ptr= fast_new (sizeof (C));
  (void) new (ptr) C (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6,
	 typename A7, typename A8, typename A9,
	 typename A10, typename A11, typename A12> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4, const A5& a5, const A6& a6,
	const A7& a7, const A8& a8, const A9& a9,
	const A10& a10, const A11& a11, const A12& a12) {
  void* ptr= fast_new (sizeof (C));
  (void) new (ptr) C (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6,
	 typename A7, typename A8, typename A9,
	 typename A10, typename A11, typename A12,
	 typename A13> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4, const A5& a5, const A6& a6,
	const A7& a7, const A8& a8, const A9& a9,
	const A10& a10, const A11& a11, const A12& a12,
	const A13& a13) {
  void* ptr= fast_new (sizeof (C));
  (void) new (ptr) C (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10,
		      a11, a12, a13);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6,
	 typename A7, typename A8, typename A9,
	 typename A10, typename A11, typename A12,
	 typename A13, typename A14> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4, const A5& a5, const A6& a6,
	const A7& a7, const A8& a8, const A9& a9,
	const A10& a10, const A11& a11, const A12& a12,
	const A13& a13, const A14& a14) {
  void* ptr= fast_new (sizeof (C));
  (void) new (ptr) C (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10,
		      a11, a12, a13, a14);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6,
	 typename A7, typename A8, typename A9,
	 typename A10, typename A11, typename A12,
	 typename A13, typename A14, typename A15> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4, const A5& a5, const A6& a6,
	const A7& a7, const A8& a8, const A9& a9,
	const A10& a10, const A11& a11, const A12& a12,
	const A13& a13, const A14& a14, const A15& a15) {
  void* ptr= fast_new (sizeof (C));
  (void) new (ptr) C (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10,
		      a11, a12, a13, a14, a15);
  return (C*) ptr;
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6,
	 typename A7, typename A8, typename A9,
	 typename A10, typename A11, typename A12,
	 typename A13, typename A14, typename A15,
	 typename A16, typename A17> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4, const A5& a5, const A6& a6,
	const A7& a7, const A8& a8, const A9& a9,
	const A10& a10, const A11& a11, const A12& a12,
	const A13& a13, const A14& a14, const A15& a15,
	const A16& a16, const A17& a17) {
  void* ptr= fast_new (sizeof (C));
  (void) new (ptr) C (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10,
		      a11, a12, a13, a14, a15, a16, a17);
  return (C*) ptr;
}

template<typename C> inline void
tm_delete (C* ptr) {
  ptr -> ~C ();
  fast_delete ((void*) ptr);
}

template<typename C> inline C*
tm_new_array (int n) {
  void* ptr= fast_alloc (n * sizeof (C) + WORD_LENGTH);
  *((int*) ptr)= n;
  ptr= (void*) (((char*) ptr) + WORD_LENGTH);
  C* ctr= (C*) ptr;
  for (int i=0; i<n; i++, ctr++)
    (void) new ((void*) ctr) C ();
  return (C*) ptr;
}

template<typename C> inline void
tm_delete_array (C* Ptr) {
  void* ptr= (void*) Ptr;
  ptr= (void*) (((char*) ptr) - WORD_LENGTH);
  int n= *((int*) ptr);
  C* ctr= Ptr+n-1;
  for (int i=0; i<n; i++, ctr--) ctr -> ~C();
  fast_free (ptr, n * sizeof (C) + WORD_LENGTH);
}

#endif // (!defined(NO_FAST_ALLOC)) && (!defined(X11TEXMACS))

/******************************************************************************
* Slow new and delete
******************************************************************************/

#if defined(NO_FAST_ALLOC) || defined(X11TEXMACS)

#ifndef NO_FAST_ALLOC
#ifdef OS_IRIX
void* operator new (register size_t s) throw(std::bad_alloc);
void  operator delete (register void* ptr) throw();
void* operator new[] (register size_t s) throw(std::bad_alloc);
void  operator delete[] (register void* ptr) throw();
#else
void* operator new (register size_t s);
void  operator delete (register void* ptr);
void* operator new[] (register size_t s);
void  operator delete[] (register void* ptr);
#endif
#endif // not defined NO_FAST_ALLOC

template<typename C> inline C*
tm_new () {
  return new C ();
}

template<typename C, typename A1> inline C*
tm_new (const A1& a1) {
  return new C (a1);
}

template<typename C, typename A1, typename A2> inline C*
tm_new (const A1& a1, const A2& a2) {
  return new C (a1, a2);
}

template<typename C, typename A1, typename A2> inline C*
tm_new (const A1& a1, A2& a2) {
  return new C (a1, a2);
}

template<typename C, typename A1, typename A2> inline C*
tm_new (A1& a1, const A2& a2) {
  return new C (a1, a2);
}

template<typename C, typename A1, typename A2> inline C*
tm_new (A1& a1, A2& a2) {
  return new C (a1, a2);
}

template<typename C, typename A1, typename A2, typename A3> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3) {
  return new C (a1, a2, a3);
}

template<typename C, typename A1, typename A2, typename A3> inline C*
tm_new (const A1& a1, A2& a2, A3& a3) {
  return new C (a1, a2, a3);
}

template<typename C, typename A1, typename A2, typename A3> inline C*
tm_new (A1& a1, A2& a2, const A3& a3) {
  return new C (a1, a2, a3);
}

template<typename C, typename A1, typename A2, typename A3> inline C*
tm_new (A1& a1, A2& a2, A3& a3) {
  return new C (a1, a2, a3);
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4) {
  return new C (a1, a2, a3, a4);
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4> inline C*
tm_new (const A1& a1, A2& a2, A3& a3, A4& a4) {
  return new C (a1, a2, a3, a4);
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4, const A5& a5) {
  return new C (a1, a2, a3, a4, a5);
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5> inline C*
tm_new (const A1& a1, A2& a2, A3& a3, A4& a4, A5& a5) {
  return new C (a1, a2, a3, a4, a5);
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5> inline C*
tm_new (A1& a1, A2& a2, A3& a3, A4& a4, A5& a5) {
  return new C (a1, a2, a3, a4, a5);
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4, const A5& a5, const A6& a6) {
  return new C (a1, a2, a3, a4, a5, a6);
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6> inline C*
tm_new (A1& a1, const A2& a2, A3& a3, A4& a4, A5& a5, A6& a6) {
  return new C (a1, a2, a3, a4, a5, a6);
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6, typename A7> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4, const A5& a5, const A6& a6,
	const A7& a7) {
  return new C (a1, a2, a3, a4, a5, a6, a7);
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6, typename A7> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	A4& a4, const A5& a5, const A6& a6,
	const A7& a7) {
  return new C (a1, a2, a3, a4, a5, a6, a7);
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6,
	 typename A7, typename A8> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4, const A5& a5, const A6& a6,
	const A7& a7, const A8& a8) {
  return new C (a1, a2, a3, a4, a5, a6, a7, a8);
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6,
	 typename A7, typename A8, typename A9> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4, const A5& a5, const A6& a6,
	const A7& a7, const A8& a8, const A9& a9) {
  return new C (a1, a2, a3, a4, a5, a6, a7, a8, a9);
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6,
	 typename A7, typename A8, typename A9,
	 typename A10> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4, const A5& a5, const A6& a6,
	const A7& a7, const A8& a8, const A9& a9,
	const A10& a10) {
  return new C (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6,
	 typename A7, typename A8, typename A9,
	 typename A10, typename A11> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4, const A5& a5, const A6& a6,
	const A7& a7, const A8& a8, const A9& a9,
	const A10& a10, const A11& a11) {
  return new C (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6,
	 typename A7, typename A8, typename A9,
	 typename A10, typename A11, typename A12> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4, const A5& a5, const A6& a6,
	const A7& a7, const A8& a8, const A9& a9,
	const A10& a10, const A11& a11, const A12& a12) {
  return new C (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6,
	 typename A7, typename A8, typename A9,
	 typename A10, typename A11, typename A12,
	 typename A13> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4, const A5& a5, const A6& a6,
	const A7& a7, const A8& a8, const A9& a9,
	const A10& a10, const A11& a11, const A12& a12,
	const A13& a13) {
  return new C (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6,
	 typename A7, typename A8, typename A9,
	 typename A10, typename A11, typename A12,
	 typename A13, typename A14> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4, const A5& a5, const A6& a6,
	const A7& a7, const A8& a8, const A9& a9,
	const A10& a10, const A11& a11, const A12& a12,
	const A13& a13, const A14& a14) {
  return new C (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14);
}

template<typename C, typename A1, typename A2, typename A3,
	 typename A4, typename A5, typename A6,
	 typename A7, typename A8, typename A9,
	 typename A10, typename A11, typename A12,
	 typename A13, typename A14, typename A15> inline C*
tm_new (const A1& a1, const A2& a2, const A3& a3,
	const A4& a4, const A5& a5, const A6& a6,
	const A7& a7, const A8& a8, const A9& a9,
	const A10& a10, const A11& a11, const A12& a12,
	const A13& a13, const A14& a14, const A15& a15) {
  return new C (a1, a2, a3, a4, a5, a6, a7, a8, a9,
                a10, a11, a12, a13, a14, a15);
}

template<typename C> inline void
tm_delete (C* ptr) {
  delete ptr;
}

template<typename C> inline C*
tm_new_array (int n) {
  return new C[n];
}

template<typename C> inline void
tm_delete_array (C* Ptr) {
  delete[] Ptr;
}

#endif // defined(NO_FAST_ALLOC) || defined(X11TEXMACS)

#endif // defined FAST_ALLOC_H
