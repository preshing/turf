/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_C_IMPL_ATOMIC_GCC_ARM_H
#define TURF_C_IMPL_ATOMIC_GCC_ARM_H

#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------
//  Atomic types
//-------------------------------------
// In compilers targeting the "old" ABI, 64-bit values are not guaranteed 8-byte
// aligned.
// These wrappers enforce the correct alignment.
// Warning: Local variables on the stack may still not be aligned using some
// compilers:
// http://gcc.gnu.org/bugzilla/show_bug.cgi?id=24691
// XCode 3.2.5 with iPhoneOS SDK 4.2 and GCC 4.2 demonstrates both problems.
typedef struct { volatile uint8_t nonatomic; } turf_atomic8_t;
typedef struct { volatile uint16_t nonatomic; } __attribute__((aligned(2))) turf_atomic16_t;
typedef struct { volatile uint32_t nonatomic; } __attribute__((aligned(4))) turf_atomic32_t;
typedef struct { volatile uint64_t nonatomic; } __attribute__((aligned(8))) turf_atomic64_t;
typedef struct { void* volatile nonatomic; } __attribute__((aligned(TURF_PTR_SIZE))) turf_atomicPtr_t;

//-------------------------------------
//  Fences
//-------------------------------------
// clang-format off
#define turf_signalFenceConsume() (0)
#define turf_signalFenceAcquire() asm volatile("" ::: "memory")
#define turf_signalFenceRelease() asm volatile("" ::: "memory")
#define turf_signalFenceSeqCst() asm volatile("" ::: "memory")

#define turf_threadFenceConsume() (0)
#if TURF_CPU_ARM_VERSION == 7
    #define turf_threadFenceAcquire() asm volatile("dmb ish" ::: "memory")
    #define turf_threadFenceRelease() asm volatile("dmb ish" ::: "memory")
    #define turf_threadFenceSeqCst() asm volatile("dmb ish" ::: "memory")
#elif TURF_CPU_ARM_VERSION == 6
    // dmb instruction does not exist in ARMv6 and lower.
    // mcr instruction can accomplish the same thing.
    #if TURF_CPU_ARM_THUMB
        // When compiling for ARMv6 in Thumb mode, the mcr instruction is not available.
        // We need to switch to ARM mode, by calling standalone functions, to use this instruction.
        // As non-inlined function calls, they function as compiler barriers as well.
        void turf_threadFenceAcquire();
        void turf_threadFenceRelease();
        void turf_threadFenceSeqCst();
    #else
        // Do we really need to set the source register to 0?
        #define turf_threadFenceAcquire() asm volatile("mcr p15, 0, %0, c7, c10, 5" :: "r"(0) : "memory")
        #define turf_threadFenceRelease() asm volatile("mcr p15, 0, %0, c7, c10, 5" :: "r"(0) : "memory")
        #define turf_threadFenceSeqCst() asm volatile("mcr p15, 0, %0, c7, c10, 5" :: "r"(0) : "memory")
    #endif
#endif
// clang-format on

//----------------------------------------------
//  8-bit atomic operations
//----------------------------------------------
TURF_C_INLINE uint8_t turf_load8Relaxed(const turf_atomic8_t* object) {
    return object->nonatomic;
}

TURF_C_INLINE void turf_store8Relaxed(turf_atomic8_t* object, uint8_t desired) {
    object->nonatomic = desired;
}

#if (TURF_CPU_ARM_VERSION == 6) && TURF_CPU_ARM_THUMB
// When compiling for ARMv6 in Thumb mode, the ldrex/strex instructions are not
// available.
// We need to switch to ARM mode, by calling standalone functions, to use these
// instructions.
uint8_t turf_compareExchange8Relaxed(turf_atomic8_t* object, uint8_t expected, uint8_t desired);
uint8_t turf_fetchAdd8Relaxed(turf_atomic8_t* object, int8_t operand);
uint8_t turf_fetchAnd8Relaxed(turf_atomic8_t* object, uint8_t operand);
uint8_t turf_fetchOr8Relaxed(turf_atomic8_t* object, uint8_t operand);

#else
TURF_C_INLINE uint8_t turf_compareExchange8Relaxed(turf_atomic8_t* object, uint8_t expected, uint8_t desired) {
    uintreg_t status;
    uint8_t previous;
    // The "Ir" constraint allows the compiler to pass "expected" to the cmp
    // instruction as an immediate
    // operand if it can. Otherwise, it falls back to loading it into a register
    // first.
    asm volatile("1:     ldrexb  %0, [%3]\n"
                 "       cmp     %0, %4\n"
                 "       bne     2f\n"
                 "       strexb  %1, %5, [%3]\n"
                 "       cmp     %1, #0\n"
                 "       bne     1b\n"
                 "2:     ;"
                 : "=&r"(previous), "=&r"(status), "+Qo"(object->nonatomic)
                 : "r"(object), "Ir"(expected), "r"(desired)
                 : "cc");
    return previous;
}

TURF_C_INLINE intreg_t turf_compareExchangeWeak8Relaxed(turf_atomic8_t* object, uint8_t* expected, uint8_t desired) {
    uintreg_t status = 1;
    uint8_t previous;
    // The "Ir" constraint allows the compiler to pass "expected" to the cmp
    // instruction as an immediate
    // operand if it can. Otherwise, it falls back to loading it into a register
    // first.
    asm volatile("1:     ldrexb  %0, [%3]\n"
                 "       cmp     %0, %4\n"
                 "       bne     2f\n"
                 "       strexb  %1, %5, [%3]\n"
                 "2:     ;"
                 : "=&r"(previous), "=&r"(status), "+Qo"(object->nonatomic)
                 : "r"(object), "Ir"(expected), "r"(desired)
                 : "cc");
    if (status)
        *expected = (uint8_t) previous;
    return !status;
}

TURF_C_INLINE uint32_t turf_exchange8Relaxed(turf_atomic8_t* object, uint8_t desired) {
    uintreg_t status;
    uint8_t previous;
    // The "Ir" constraint allows the compiler to pass "expected" to the cmp
    // instruction as an immediate
    // operand if it can. Otherwise, it falls back to loading it into a register
    // first.
    asm volatile("1:     ldrexb  %0, [%3]\n"
                 "       strexb  %1, %4, [%3]\n"
                 "       cmp     %1, #0\n"
                 "       bne     1b\n"
                 "2:     ;"
                 : "=&r"(previous), "=&r"(status), "+Qo"(object->nonatomic)
                 : "r"(object), "r"(desired)
                 : "cc");
    return previous;
}

TURF_C_INLINE uint8_t turf_fetchAdd8Relaxed(turf_atomic8_t* object, int8_t operand) {
    uintreg_t status;
    uint8_t previous, desired;
    asm volatile("1:     ldrexb  %0, [%4]\n"
                 "       mov     %3, %0\n"
                 "       add     %3, %5\n"
                 "       strexb  %1, %3, [%4]\n"
                 "       cmp     %1, #0\n"
                 "       bne     1b"
                 : "=&r"(previous), "=&r"(status), "+Qo"(object->nonatomic), "=&r"(desired)
                 : "r"(object), "Ir"(operand)
                 : "cc");
    return previous;
}

TURF_C_INLINE uint8_t turf_fetchAnd8Relaxed(turf_atomic8_t* object, uint8_t operand) {
    uintreg_t status;
    uint8_t previous, desired;
    asm volatile("1:     ldrexb  %0, [%4]\n"
                 "       mov     %3, %0\n"
                 "       and     %3, %5\n"
                 "       strexb  %1, %3, [%4]\n"
                 "       cmp     %1, #0\n"
                 "       bne     1b"
                 : "=&r"(previous), "=&r"(status), "+Qo"(object->nonatomic), "=&r"(desired)
                 : "r"(object), "Ir"(operand)
                 : "cc");
    return previous;
}

TURF_C_INLINE uint8_t turf_fetchOr8Relaxed(turf_atomic8_t* object, uint8_t operand) {
    uintreg_t status;
    uint8_t previous, desired;
    asm volatile("1:     ldrexb  %0, [%4]\n"
                 "       mov     %3, %0\n"
                 "       orr     %3, %5\n"
                 "       strexb  %1, %3, [%4]\n"
                 "       cmp     %1, #0\n"
                 "       bne     1b"
                 : "=&r"(previous), "=&r"(status), "+Qo"(object->nonatomic), "=&r"(desired)
                 : "r"(object), "Ir"(operand)
                 : "cc");
    return previous;
}
#endif

//----------------------------------------------
//  16-bit atomic operations
//----------------------------------------------
TURF_C_INLINE uint16_t turf_load16Relaxed(const turf_atomic16_t* object) {
    return object->nonatomic;
}

TURF_C_INLINE void turf_store16Relaxed(turf_atomic16_t* object, uint16_t desired) {
    object->nonatomic = desired;
}

#if (TURF_CPU_ARM_VERSION == 6) && TURF_CPU_ARM_THUMB
// When compiling for ARMv6 in Thumb mode, the ldrex/strex instructions are not
// available.
// We need to switch to ARM mode, by calling standalone functions, to use these
// instructions.
uint16_t turf_compareExchange16Relaxed(turf_atomic16_t* object, uint16_t expected, uint16_t desired);
uint16_t turf_fetchAdd16Relaxed(turf_atomic16_t* object, int16_t operand);
uint16_t turf_fetchAnd16Relaxed(turf_atomic16_t* object, uint16_t operand);
uint16_t turf_fetchOr16Relaxed(turf_atomic16_t* object, uint16_t operand);

#else
TURF_C_INLINE uint16_t turf_compareExchange16Relaxed(turf_atomic16_t* object, uint16_t expected, uint16_t desired) {
    uintreg_t status;
    uint16_t previous;
    // The "Ir" constraint allows the compiler to pass "expected" to the cmp
    // instruction as an immediate
    // operand if it can. Otherwise, it falls back to loading it into a register
    // first.
    asm volatile("1:     ldrexh  %0, [%3]\n"
                 "       cmp     %0, %4\n"
                 "       bne     2f\n"
                 "       strexh  %1, %5, [%3]\n"
                 "       cmp     %1, #0\n"
                 "       bne     1b\n"
                 "2:     ;"
                 : "=&r"(previous), "=&r"(status), "+Qo"(object->nonatomic)
                 : "r"(object), "Ir"(expected), "r"(desired)
                 : "cc");
    return previous;
}

TURF_C_INLINE intreg_t turf_compareExchangeWeak16Relaxed(turf_atomic16_t* object, uint16_t* expected, uint16_t desired) {
    uintreg_t status = 1;
    uint16_t previous;
    // The "Ir" constraint allows the compiler to pass "expected" to the cmp
    // instruction as an immediate
    // operand if it can. Otherwise, it falls back to loading it into a register
    // first.
    asm volatile("1:     ldrexh  %0, [%3]\n"
                 "       cmp     %0, %4\n"
                 "       bne     2f\n"
                 "       strexh  %1, %5, [%3]\n"
                 "2:     ;"
                 : "=&r"(previous), "=&r"(status), "+Qo"(object->nonatomic)
                 : "r"(object), "Ir"(expected), "r"(desired)
                 : "cc");
    if (status)
        *expected = (uint16_t) previous;
    return !status;
}

TURF_C_INLINE uint32_t turf_exchange16Relaxed(turf_atomic16_t* object, uint16_t desired) {
    uintreg_t status;
    uint16_t previous;
    // The "Ir" constraint allows the compiler to pass "expected" to the cmp
    // instruction as an immediate
    // operand if it can. Otherwise, it falls back to loading it into a register
    // first.
    asm volatile("1:     ldrexh  %0, [%3]\n"
                 "       strexh  %1, %4, [%3]\n"
                 "       cmp     %1, #0\n"
                 "       bne     1b\n"
                 "2:     ;"
                 : "=&r"(previous), "=&r"(status), "+Qo"(object->nonatomic)
                 : "r"(object), "r"(desired)
                 : "cc");
    return previous;
}

TURF_C_INLINE uint16_t turf_fetchAdd16Relaxed(turf_atomic16_t* object, int16_t operand) {
    uintreg_t status;
    uint16_t previous, desired;
    asm volatile("1:     ldrexh  %0, [%4]\n"
                 "       mov     %3, %0\n"
                 "       add     %3, %5\n"
                 "       strexh  %1, %3, [%4]\n"
                 "       cmp     %1, #0\n"
                 "       bne     1b"
                 : "=&r"(previous), "=&r"(status), "+Qo"(object->nonatomic), "=&r"(desired)
                 : "r"(object), "Ir"(operand)
                 : "cc");
    return previous;
}

TURF_C_INLINE uint16_t turf_fetchAnd16Relaxed(turf_atomic16_t* object, uint16_t operand) {
    uintreg_t status;
    uint16_t previous, desired;
    asm volatile("1:     ldrexh  %0, [%4]\n"
                 "       mov     %3, %0\n"
                 "       and     %3, %5\n"
                 "       strexh  %1, %3, [%4]\n"
                 "       cmp     %1, #0\n"
                 "       bne     1b"
                 : "=&r"(previous), "=&r"(status), "+Qo"(object->nonatomic), "=&r"(desired)
                 : "r"(object), "Ir"(operand)
                 : "cc");
    return previous;
}

TURF_C_INLINE uint16_t turf_fetchOr16Relaxed(turf_atomic16_t* object, uint16_t operand) {
    uintreg_t status;
    uint16_t previous, desired;
    asm volatile("1:     ldrexh  %0, [%4]\n"
                 "       mov     %3, %0\n"
                 "       orr     %3, %5\n"
                 "       strexh  %1, %3, [%4]\n"
                 "       cmp     %1, #0\n"
                 "       bne     1b"
                 : "=&r"(previous), "=&r"(status), "+Qo"(object->nonatomic), "=&r"(desired)
                 : "r"(object), "Ir"(operand)
                 : "cc");
    return previous;
}
#endif

//----------------------------------------------
//  32-bit atomic operations
//----------------------------------------------
TURF_C_INLINE uint32_t turf_load32Relaxed(const turf_atomic32_t* object) {
    return object->nonatomic;
}

TURF_C_INLINE void turf_store32Relaxed(turf_atomic32_t* object, uint32_t desired) {
    object->nonatomic = desired;
}

#if (TURF_CPU_ARM_VERSION == 6) && TURF_CPU_ARM_THUMB
// When compiling for ARMv6 in Thumb mode, the ldrex/strex instructions are not
// available.
// We need to switch to ARM mode, by calling standalone functions, to use these
// instructions.
uint32_t turf_compareExchange32Relaxed(turf_atomic32_t* object, uint32_t expected, uint32_t desired);
uint32_t turf_fetchAdd32Relaxed(turf_atomic32_t* object, int32_t operand);
uint32_t turf_fetchAnd32Relaxed(turf_atomic32_t* object, uint32_t operand);
uint32_t turf_fetchOr32Relaxed(turf_atomic32_t* object, uint32_t operand);

#else
TURF_C_INLINE uint32_t turf_compareExchange32Relaxed(turf_atomic32_t* object, uint32_t expected, uint32_t desired) {
    uintreg_t status;
    uint32_t previous;
    // The "Ir" constraint allows the compiler to pass "expected" to the cmp
    // instruction as an immediate
    // operand if it can. Otherwise, it falls back to loading it into a register
    // first.
    asm volatile("1:     ldrex   %0, [%3]\n"
                 "       cmp     %0, %4\n"
                 "       bne     2f\n"
                 "       strex   %1, %5, [%3]\n"
                 "       cmp     %1, #0\n"
                 "       bne     1b\n"
                 "2:     ;"
                 : "=&r"(previous), "=&r"(status), "+Qo"(object->nonatomic)
                 : "r"(object), "Ir"(expected), "r"(desired)
                 : "cc");
    return previous;
}

TURF_C_INLINE intreg_t turf_compareExchangeWeak32Relaxed(turf_atomic32_t* object, uint32_t* expected, uint32_t desired) {
    uintreg_t status;
    uint32_t previous;
    uint32_t exp = *expected;
    // The "Ir" constraint allows the compiler to pass "expected" to the cmp
    // instruction as an immediate
    // operand if it can. Otherwise, it falls back to loading it into a register
    // first.
    asm volatile("       mov     %1, #1\n"
                 "       ldrex   %0, [%3]\n"
                 "       cmp     %0, %4\n"
                 "       bne     1f\n"
                 "       strex   %1, %5, [%3]\n"
                 "1:     ;"
                 : "=&r"(previous), "=&r"(status), "+Qo"(object->nonatomic)
                 : "r"(object), "Ir"(exp), "r"(desired)
                 : "cc");
    if (status)
        *expected = (uint32_t) previous;
    return !status;
}

TURF_C_INLINE uint32_t turf_exchange32Relaxed(turf_atomic32_t* object, uint32_t desired) {
    uintreg_t status;
    uint32_t previous;
    // The "Ir" constraint allows the compiler to pass "expected" to the cmp
    // instruction as an immediate
    // operand if it can. Otherwise, it falls back to loading it into a register
    // first.
    asm volatile("1:     ldrex   %0, [%3]\n"
                 "       strex   %1, %4, [%3]\n"
                 "       cmp     %1, #0\n"
                 "       bne     1b\n"
                 "2:     ;"
                 : "=&r"(previous), "=&r"(status), "+Qo"(object->nonatomic)
                 : "r"(object), "r"(desired)
                 : "cc");
    return previous;
}

TURF_C_INLINE uint32_t turf_fetchAdd32Relaxed(turf_atomic32_t* object, int32_t operand) {
    uintreg_t status;
    uint32_t previous, desired;
    asm volatile("1:     ldrex   %0, [%4]\n"
                 "       mov     %3, %0\n"
                 "       add     %3, %5\n"
                 "       strex   %1, %3, [%4]\n"
                 "       cmp     %1, #0\n"
                 "       bne     1b"
                 : "=&r"(previous), "=&r"(status), "+Qo"(object->nonatomic), "=&r"(desired)
                 : "r"(object), "Ir"(operand)
                 : "cc");
    return previous;
}

TURF_C_INLINE uint32_t turf_fetchAnd32Relaxed(turf_atomic32_t* object, uint32_t operand) {
    uintreg_t status;
    uint32_t previous, desired;
    asm volatile("1:     ldrex   %0, [%4]\n"
                 "       mov     %3, %0\n"
                 "       and     %3, %5\n"
                 "       strex   %1, %3, [%4]\n"
                 "       cmp     %1, #0\n"
                 "       bne     1b"
                 : "=&r"(previous), "=&r"(status), "+Qo"(object->nonatomic), "=&r"(desired)
                 : "r"(object), "Ir"(operand)
                 : "cc");
    return previous;
}

TURF_C_INLINE uint32_t turf_fetchOr32Relaxed(turf_atomic32_t* object, uint32_t operand) {
    uintreg_t status;
    uint32_t previous, desired;
    asm volatile("1:     ldrex   %0, [%4]\n"
                 "       mov     %3, %0\n"
                 "       orr     %3, %5\n"
                 "       strex   %1, %3, [%4]\n"
                 "       cmp     %1, #0\n"
                 "       bne     1b"
                 : "=&r"(previous), "=&r"(status), "+Qo"(object->nonatomic), "=&r"(desired)
                 : "r"(object), "Ir"(operand)
                 : "cc");
    return previous;
}
#endif

//----------------------------------------------
//  64-bit atomic operations
//----------------------------------------------
uint64_t turf_load64Relaxed(const turf_atomic64_t* object);
void turf_store64Relaxed(turf_atomic64_t* object, uint64_t desired);
uint64_t turf_compareExchange64Relaxed(turf_atomic64_t* object, uint64_t expected, uint64_t desired);
uintptr_t turf_compareExchangeWeak64Relaxed(turf_atomic64_t* object, uint64_t* expected, uint64_t desired);
uint64_t turf_exchange64Relaxed(turf_atomic64_t* object, uint64_t desired);
uint64_t turf_fetchAdd64Relaxed(turf_atomic64_t* object, int64_t operand);
uint64_t turf_fetchAnd64Relaxed(turf_atomic64_t* object, uint64_t operand);
uint64_t turf_fetchOr64Relaxed(turf_atomic64_t* object, uint64_t operand);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // TURF_C_IMPL_ATOMIC_GCC_ARM_H
