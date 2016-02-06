/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_C_IMPL_ATOMIC_MSVC_H
#define TURF_C_IMPL_ATOMIC_MSVC_H

#ifdef __cplusplus
extern "C" {
#endif

#if TURF_TARGET_XBOX_360 // Xbox 360
void _ReadWriteBarrier();
#pragma intrinsic(_ReadWriteBarrier)
#endif

//-------------------------------------
//  Atomic types
//-------------------------------------
// In MSVC, correct alignment of each type is already ensured.
// MSVC doesn't seem subject to out-of-thin-air stores like GCC, so volatile is
// omitted.
// (MS volatile implies acquire & release semantics, which may be expensive on
// ARM or Xbox 360.)
typedef struct { uint8_t nonatomic; } turf_atomic8_t;
typedef struct { uint16_t nonatomic; } turf_atomic16_t;
typedef struct { uint32_t nonatomic; } turf_atomic32_t;
typedef struct { uint64_t nonatomic; } turf_atomic64_t;
typedef struct { void* nonatomic; } turf_atomicPtr_t;

//-------------------------------------
//  Fences
//-------------------------------------
#define turf_signalFenceConsume() (0)
#define turf_signalFenceAcquire() _ReadWriteBarrier()
#define turf_signalFenceRelease() _ReadWriteBarrier()
#define turf_signalFenceSeqCst() _ReadWriteBarrier()

#if TURF_TARGET_XBOX_360 // Xbox 360
// According to
// http://msdn.microsoft.com/en-us/library/windows/desktop/ee418650.aspx,
// __lwsync() also acts a compiler barrier, unlike MemoryBarrier() on X360.
// Hoping __sync() acts as a compiler barrier too.
#define turf_threadFenceConsume() (0)
#define turf_threadFenceAcquire() __lwsync()
#define turf_threadFenceRelease() __lwsync()
#define turf_threadFenceSeqCst() __sync()
#else // Windows
#define turf_threadFenceConsume() (0)
#define turf_threadFenceAcquire() _ReadWriteBarrier()
#define turf_threadFenceRelease() _ReadWriteBarrier()
#define turf_threadFenceSeqCst() MemoryBarrier()
#endif

//----------------------------------------------
//  8-bit atomic operations
//----------------------------------------------
TURF_C_INLINE uint8_t turf_load8Relaxed(const turf_atomic8_t* object) {
    // Do a volatile load so that compiler doesn't duplicate loads, which makes
    // them nonatomic. (Happened in testing.)
    return ((volatile turf_atomic8_t*) object)->nonatomic;
}

TURF_C_INLINE void turf_store8Relaxed(turf_atomic8_t* object, uint8_t value) {
    ((volatile turf_atomic8_t*) object)->nonatomic = value;
}

TURF_C_INLINE uint8_t turf_compareExchange8Relaxed(turf_atomic8_t* object, uint8_t expected, uint8_t desired) {
    return _InterlockedCompareExchange8((char*) object, desired, expected);
}

TURF_C_INLINE intreg_t turf_compareExchangeWeak8Relaxed(turf_atomic8_t* object, uint8_t* expected, uint8_t desired) {
    uint8_t e = *expected;
    uint8_t previous = _InterlockedCompareExchange8((char*) object, desired, e);
    intreg_t matched = (previous == e);
    if (!matched)
        *expected = previous;
    return matched;
}

TURF_C_INLINE uint8_t turf_exchange8Relaxed(turf_atomic8_t* object, uint8_t desired) {
    return _InterlockedExchange8((char*) object, desired);
}

TURF_C_INLINE uint8_t turf_fetchAdd8Relaxed(turf_atomic8_t* object, int8_t operand) {
    return _InterlockedExchangeAdd8((char*) object, operand);
}

TURF_C_INLINE uint8_t turf_fetchAnd8Relaxed(turf_atomic8_t* object, uint8_t operand) {
    return _InterlockedAnd8((char*) object, operand);
}

TURF_C_INLINE uint8_t turf_fetchOr8Relaxed(turf_atomic8_t* object, uint8_t operand) {
    return _InterlockedOr8((char*) object, operand);
}

//----------------------------------------------
//  16-bit atomic operations
//----------------------------------------------
TURF_C_INLINE uint16_t turf_load16Relaxed(const turf_atomic16_t* object) {
    // Do a volatile load so that compiler doesn't duplicate loads, which makes
    // them nonatomic. (Happened in testing.)
    return ((volatile turf_atomic16_t*) object)->nonatomic;
}

TURF_C_INLINE void turf_store16Relaxed(turf_atomic16_t* object, uint16_t value) {
    ((volatile turf_atomic16_t*) object)->nonatomic = value;
}

TURF_C_INLINE uint16_t turf_compareExchange16Relaxed(turf_atomic16_t* object, uint16_t expected, uint16_t desired) {
    return _InterlockedCompareExchange16((short*) object, desired, expected);
}

TURF_C_INLINE intreg_t turf_compareExchangeWeak16Relaxed(turf_atomic16_t* object, uint16_t* expected, uint16_t desired) {
    uint16_t e = *expected;
    uint16_t previous = _InterlockedCompareExchange16((short*) object, desired, e);
    intreg_t matched = (previous == e);
    if (!matched)
        *expected = previous;
    return matched;
}

TURF_C_INLINE uint16_t turf_exchange16Relaxed(turf_atomic16_t* object, uint16_t desired) {
    return _InterlockedExchange16((short*) object, desired);
}

TURF_C_INLINE uint16_t turf_fetchAdd16Relaxed(turf_atomic16_t* object, int16_t operand) {
    return _InterlockedExchangeAdd16((short*) object, operand);
}

TURF_C_INLINE uint16_t turf_fetchAnd16Relaxed(turf_atomic16_t* object, uint16_t operand) {
    return _InterlockedAnd16((short*) object, operand);
}

TURF_C_INLINE uint16_t turf_fetchOr16Relaxed(turf_atomic16_t* object, uint16_t operand) {
    return _InterlockedOr16((short*) object, operand);
}

//----------------------------------------------
//  32-bit atomic operations
//----------------------------------------------
TURF_C_INLINE uint32_t turf_load32Relaxed(const turf_atomic32_t* object) {
    // Do a volatile load so that compiler doesn't duplicate loads, which makes
    // them nonatomic. (Happened in testing.)
    return ((volatile turf_atomic32_t*) object)->nonatomic;
}

TURF_C_INLINE void turf_store32Relaxed(turf_atomic32_t* object, uint32_t value) {
    ((volatile turf_atomic32_t*) object)->nonatomic = value;
}

TURF_C_INLINE uint32_t turf_compareExchange32Relaxed(turf_atomic32_t* object, uint32_t expected, uint32_t desired) {
    return _InterlockedCompareExchange((long*) object, desired, expected);
}

TURF_C_INLINE intreg_t turf_compareExchangeWeak32Relaxed(turf_atomic32_t* object, uint32_t* expected, uint32_t desired) {
    uint32_t e = *expected;
    uint32_t previous = _InterlockedCompareExchange((long*) object, desired, e);
    intreg_t matched = (previous == e);
    if (!matched)
        *expected = previous;
    return matched;
}

TURF_C_INLINE uint32_t turf_exchange32Relaxed(turf_atomic32_t* object, uint32_t desired) {
    return _InterlockedExchange((long*) object, desired);
}

TURF_C_INLINE uint32_t turf_fetchAdd32Relaxed(turf_atomic32_t* object, int32_t operand) {
    return _InterlockedExchangeAdd((long*) object, operand);
}

TURF_C_INLINE uint32_t turf_fetchAnd32Relaxed(turf_atomic32_t* object, uint32_t operand) {
    return _InterlockedAnd((long*) object, operand);
}

TURF_C_INLINE uint32_t turf_fetchOr32Relaxed(turf_atomic32_t* object, uint32_t operand) {
    return _InterlockedOr((long*) object, operand);
}

//----------------------------------------------
//  64-bit atomic operations
//----------------------------------------------
TURF_C_INLINE uint64_t turf_load64Relaxed(const turf_atomic64_t* object) {
#if (TURF_PTR_SIZE == 8) || TURF_TARGET_XBOX_360
    // Do a volatile load so that compiler doesn't duplicate loads, which makes
    // them nonatomic. (Happened in testing.)
    return ((volatile turf_atomic64_t*) object)->nonatomic;
#else
    // On 32-bit x86, the most compatible way to get an atomic 64-bit load is with
    // cmpxchg8b.
    // This essentially performs turf_compareExchange64Relaxed(object,
    // _dummyValue, _dummyValue).
    uint64_t result;
    __asm {
        mov esi, object;
        mov ebx, eax;
        mov ecx, edx;
        lock cmpxchg8b [esi];
        mov dword ptr result, eax;
        mov dword ptr result[4], edx;
    }
    return result;
#endif
}

TURF_C_INLINE void turf_store64Relaxed(turf_atomic64_t* object, uint64_t value) {
#if (TURF_PTR_SIZE == 8) || TURF_TARGET_XBOX_360
    ((volatile turf_atomic64_t*) object)->nonatomic = value;
#else
    // On 32-bit x86, the most compatible way to get an atomic 64-bit store is
    // with cmpxchg8b.
    // Essentially, we perform turf_compareExchange64Relaxed(object,
    // object->nonatomic, desired)
    // in a loop until it returns the previous value.
    // According to the Linux kernel (atomic64_cx8_32.S), we don't need the
    // "lock;" prefix
    // on cmpxchg8b since aligned 64-bit writes are already atomic on 586 and
    // newer.
    __asm {
        mov esi, object;
        mov ebx, dword ptr value;
        mov ecx, dword ptr value[4];
    retry:
        cmpxchg8b [esi];
        jne retry;
    }
#endif
}

TURF_C_INLINE uint64_t turf_compareExchange64Relaxed(turf_atomic64_t* object, uint64_t expected, uint64_t desired) {
    // Or make 2 versions of function? (Only if there's an advantage for GCC.)
    return _InterlockedCompareExchange64((LONGLONG*) object, desired, expected);
}

TURF_C_INLINE intreg_t turf_compareExchangeWeak64Relaxed(turf_atomic64_t* object, uint64_t* expected, uint64_t desired) {
    uint64_t e = *expected;
    uint64_t previous = _InterlockedCompareExchange64((LONGLONG*) object, desired, e);
    intreg_t matched = (previous == e);
    if (!matched)
        *expected = previous;
    return matched;
}

TURF_C_INLINE uint64_t turf_exchange64Relaxed(turf_atomic64_t* object, uint64_t desired) {
#if (TURF_PTR_SIZE == 8) || TURF_TARGET_XBOX_360
    return _InterlockedExchange64((LONGLONG*) object, desired);
#else
    // It would be cool to check the zero flag, which is set by lock cmpxchg8b, to
    // know whether the CAS succeeded,
    // but that would require an __asm block, which forces us to move the result
    // to a stack variable.
    // Let's just re-compare the result with the previous instead.
    uint64_t expected = object->nonatomic;
    for (;;) {
        uint64_t previous = _InterlockedCompareExchange64((LONGLONG*) object, desired, expected);
        if (previous == expected)
            return previous;
        expected = previous;
    }
#endif
}

TURF_C_INLINE uint64_t turf_fetchAdd64Relaxed(turf_atomic64_t* object, int64_t operand) {
#if (TURF_PTR_SIZE == 8) || TURF_TARGET_XBOX_360
    return _InterlockedExchangeAdd64((LONGLONG*) object, operand);
#else
    uint64_t expected = object->nonatomic;
    for (;;) {
        uint64_t previous = _InterlockedCompareExchange64((LONGLONG*) object, expected + operand, expected);
        if (previous == expected)
            return previous;
        expected = previous;
    }
#endif
}

TURF_C_INLINE uint64_t turf_fetchAnd64Relaxed(turf_atomic64_t* object, uint64_t operand) {
#if (TURF_PTR_SIZE == 8) || TURF_TARGET_XBOX_360
    return _InterlockedAnd64((LONGLONG*) object, operand);
#else
    uint64_t expected = object->nonatomic;
    for (;;) {
        uint64_t previous = _InterlockedCompareExchange64((LONGLONG*) object, expected & operand, expected);
        if (previous == expected)
            return previous;
        expected = previous;
    }
#endif
}

TURF_C_INLINE uint64_t turf_fetchOr64Relaxed(turf_atomic64_t* object, uint64_t operand) {
#if (TURF_PTR_SIZE == 8) || TURF_TARGET_XBOX_360
    return _InterlockedOr64((LONGLONG*) object, operand);
#else
    uint64_t expected = object->nonatomic;
    for (;;) {
        uint64_t previous = _InterlockedCompareExchange64((LONGLONG*) object, expected | operand, expected);
        if (previous == expected)
            return previous;
        expected = previous;
    }
#endif
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif // TURF_C_IMPL_ATOMIC_MSVC_H
