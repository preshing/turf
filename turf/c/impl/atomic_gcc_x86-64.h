/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_C_IMPL_ATOMIC_GCC_X86_64_H
#define TURF_C_IMPL_ATOMIC_GCC_X86_64_H

#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------
//  Atomic types
//-------------------------------------
typedef struct { volatile uint8_t nonatomic; } turf_atomic8_t;
typedef struct { volatile uint16_t nonatomic; } __attribute__((aligned(2))) turf_atomic16_t;
typedef struct { volatile uint32_t nonatomic; } __attribute__((aligned(4))) turf_atomic32_t;
typedef struct { volatile uint64_t nonatomic; } __attribute__((aligned(8))) turf_atomic64_t;
typedef struct { void* volatile nonatomic; } __attribute__((aligned(TURF_PTR_SIZE))) turf_atomicPtr_t;

//-------------------------------------
//  Fences
//-------------------------------------
#define turf_signalFenceConsume() (0)
#define turf_signalFenceAcquire() asm volatile("" ::: "memory")
#define turf_signalFenceRelease() asm volatile("" ::: "memory")
#define turf_signalFenceSeqCst() asm volatile("" ::: "memory")

#define turf_threadFenceConsume() (0)
#define turf_threadFenceAcquire() asm volatile("" ::: "memory")
#define turf_threadFenceRelease() asm volatile("" ::: "memory")
#if TURF_CPU_X64
#define turf_threadFenceSeqCst() asm volatile("lock; orl $0, (%%rsp)" ::: "memory")
#else
#define turf_threadFenceSeqCst() asm volatile("lock; orl $0, (%%esp)" ::: "memory")
#endif

//----------------------------------------------
//  8-bit atomic operations
//----------------------------------------------
TURF_C_INLINE uint8_t turf_load8Relaxed(const turf_atomic8_t* object) {
    return object->nonatomic;
}

TURF_C_INLINE void turf_store8Relaxed(turf_atomic8_t* object, uint8_t desired) {
    object->nonatomic = desired;
}

TURF_C_INLINE uint8_t turf_compareExchange8Relaxed(turf_atomic8_t* object, uint8_t expected, uint8_t desired) {
    uint8_t previous;
    asm volatile("lock; cmpxchgb %2, %1" : "=a"(previous), "+m"(object->nonatomic) : "q"(desired), "0"(expected));
    return previous;
}

TURF_C_INLINE intreg_t turf_compareExchangeWeak8Relaxed(turf_atomic8_t* object, uint8_t* expected, uint8_t desired) {
    uint8_t e = *expected;
    uint8_t previous;
    intreg_t matched;
    asm volatile("lock; cmpxchgb %2, %1" : "=a"(previous), "+m"(object->nonatomic) : "q"(desired), "0"(e));
    matched = (previous == e);
    if (!matched)
        *expected = previous;
    return matched;
}

TURF_C_INLINE uint8_t turf_exchange8Relaxed(turf_atomic8_t* object, uint8_t desired) {
    uint8_t previous;
    asm volatile("xchgb %0, %1" : "=r"(previous), "+m"(object->nonatomic) : "0"(desired));
    return previous;
}

TURF_C_INLINE uint8_t turf_fetchAdd8Relaxed(turf_atomic8_t* object, int8_t operand) {
    uint8_t previous;
    asm volatile("lock; xaddb %0, %1" : "=r"(previous), "+m"(object->nonatomic) : "0"(operand));
    return previous;
}

TURF_C_INLINE uint8_t turf_fetchAnd8Relaxed(turf_atomic8_t* object, uint8_t operand) {
    uint8_t previous;
    uint8_t temp;
    asm volatile("1:     movb    %1, %0\n"
                 "       movb    %0, %2\n"
                 "       andb    %3, %2\n"
                 "       lock; cmpxchgb %2, %1\n"
                 "       jne     1b"
                 : "=&a"(previous), "+m"(object->nonatomic), "=&r"(temp)
                 : "r"(operand));
    return previous;
}

TURF_C_INLINE uint8_t turf_fetchOr8Relaxed(turf_atomic8_t* object, uint8_t operand) {
    uint8_t previous;
    uint8_t temp;
    asm volatile("1:     movb    %1, %0\n"
                 "       movb    %0, %2\n"
                 "       orb     %3, %2\n"
                 "       lock; cmpxchgb %2, %1\n"
                 "       jne     1b"
                 : "=&a"(previous), "+m"(object->nonatomic), "=&r"(temp)
                 : "r"(operand));
    return previous;
}

//----------------------------------------------
//  16-bit atomic operations
//----------------------------------------------
TURF_C_INLINE uint16_t turf_load16Relaxed(const turf_atomic16_t* object) {
    return object->nonatomic;
}

TURF_C_INLINE void turf_store16Relaxed(turf_atomic16_t* object, uint16_t desired) {
    object->nonatomic = desired;
}

TURF_C_INLINE uint16_t turf_compareExchange16Relaxed(turf_atomic16_t* object, uint16_t expected, uint16_t desired) {
    uint16_t previous;
    asm volatile("lock; cmpxchgw %2, %1" : "=a"(previous), "+m"(object->nonatomic) : "q"(desired), "0"(expected));
    return previous;
}

TURF_C_INLINE intreg_t turf_compareExchangeWeak16Relaxed(turf_atomic16_t* object, uint16_t* expected, uint16_t desired) {
    uint16_t e = *expected;
    uint16_t previous;
    intreg_t matched;
    asm volatile("lock; cmpxchgw %2, %1" : "=a"(previous), "+m"(object->nonatomic) : "q"(desired), "0"(e));
    matched = (previous == e);
    if (!matched)
        *expected = previous;
    return matched;
}

TURF_C_INLINE uint16_t turf_exchange16Relaxed(turf_atomic16_t* object, uint16_t desired) {
    uint16_t previous;
    asm volatile("xchgw %0, %1" : "=r"(previous), "+m"(object->nonatomic) : "0"(desired));
    return previous;
}

TURF_C_INLINE uint16_t turf_fetchAdd16Relaxed(turf_atomic16_t* object, int16_t operand) {
    uint16_t previous;
    asm volatile("lock; xaddw %0, %1" : "=r"(previous), "+m"(object->nonatomic) : "0"(operand));
    return previous;
}

TURF_C_INLINE uint16_t turf_fetchAnd16Relaxed(turf_atomic16_t* object, uint16_t operand) {
    uint16_t previous;
    uint16_t temp;
    asm volatile("1:     movw    %1, %0\n"
                 "       movw    %0, %2\n"
                 "       andw    %3, %2\n"
                 "       lock; cmpxchgw %2, %1\n"
                 "       jne     1b"
                 : "=&a"(previous), "+m"(object->nonatomic), "=&r"(temp)
                 : "r"(operand));
    return previous;
}

TURF_C_INLINE uint16_t turf_fetchOr16Relaxed(turf_atomic16_t* object, uint16_t operand) {
    uint16_t previous;
    uint16_t temp;
    asm volatile("1:     movw    %1, %0\n"
                 "       movw    %0, %2\n"
                 "       orw     %3, %2\n"
                 "       lock; cmpxchgw %2, %1\n"
                 "       jne     1b"
                 : "=&a"(previous), "+m"(object->nonatomic), "=&r"(temp)
                 : "r"(operand));
    return previous;
}

//----------------------------------------------
//  32-bit atomic operations
//----------------------------------------------
TURF_C_INLINE uint32_t turf_load32Relaxed(const turf_atomic32_t* object) {
    return object->nonatomic;
}

TURF_C_INLINE void turf_store32Relaxed(turf_atomic32_t* object, uint32_t desired) {
    object->nonatomic = desired;
}

TURF_C_INLINE uint32_t turf_compareExchange32Relaxed(turf_atomic32_t* object, uint32_t expected, uint32_t desired) {
    // CMPXCHG is written cmpxchgl because GCC (and Clang) uses AT&T assembler
    // syntax.
    // Also due to AT&T syntax, the operands are swapped: %1 is the destination.
    // (This is the opposite of how Intel syntax lists the operands, where the
    // destination comes first.)
    // "=a"(previous) means the asm block outputs EAX to previous, because CMPXCHG
    // puts the old value in EAX.
    // "+m"(object->nonatomic) is the memory address that is read/written. This
    // becomes %1.
    // "q"(desired) puts desired into any of EBX, ECX or EDX before the block.
    // This becomes %2.
    // "0"(expected) puts expected in the same register as "=a"(previous), which
    // is EAX, before the block.
    // Not putting "memory" in the clobber list because the operation is relaxed.
    // It's OK for the compiler
    // to reorder this atomic followed by a load, for example. If the programmer
    // wants to enforce ordering,
    // they will use an explicit fence.
    // http://www.ibiblio.org/gferg/ldp/GCC-Inline-Assembly-HOWTO.html
    // http://gcc.gnu.org/onlinedocs/gcc/Simple-Constraints.html#Simple-Constraints
    // http://gcc.gnu.org/onlinedocs/gcc/Modifiers.html#Modifiers
    // http://gcc.gnu.org/onlinedocs/gcc/Machine-Constraints.html#Machine-Constraints
    // http://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html
    // http://download.intel.com/products/processor/manual/325383.pdf
    uint32_t previous;
    asm volatile("lock; cmpxchgl %2, %1" : "=a"(previous), "+m"(object->nonatomic) : "q"(desired), "0"(expected));
    return previous;
}

TURF_C_INLINE intreg_t turf_compareExchangeWeak32Relaxed(turf_atomic32_t* object, uint32_t* expected, uint32_t desired) {
    uint32_t e = *expected;
    uint32_t previous;
    intreg_t matched;
    asm volatile("lock; cmpxchgl %2, %1" : "=a"(previous), "+m"(object->nonatomic) : "q"(desired), "0"(e));
    matched = (previous == e);
    if (!matched)
        *expected = previous;
    return matched;
}

TURF_C_INLINE uint32_t turf_exchange32Relaxed(turf_atomic32_t* object, uint32_t desired) {
    // No lock prefix is necessary for XCHG.
    // See turf_fetchAdd32Relaxed for explanation of constraints.
    uint32_t previous;
    asm volatile("xchgl %0, %1" : "=r"(previous), "+m"(object->nonatomic) : "0"(desired));
    return previous;
}

TURF_C_INLINE uint32_t turf_fetchAdd32Relaxed(turf_atomic32_t* object, int32_t operand) {
    // "=r"(previous) chooses any general register, makes that %0, and outputs
    // this register to previous after the
    // block.
    // "+m"(object->nonatomic) is the memory address that is read/written. This
    // becomes %1.
    // "0"(operand) puts operand into the same register as %0 before the block.
    // volatile is required. Otherwise, if the return value (previous) is unused,
    // the asm block
    // may be deleted. ("+m" is apparently not enough hint to the compiler that
    // the asm
    // block has side effects on memory.)
    uint32_t previous;
    asm volatile("lock; xaddl %0, %1" : "=r"(previous), "+m"(object->nonatomic) : "0"(operand));
    return previous;
}

TURF_C_INLINE uint32_t turf_fetchAnd32Relaxed(turf_atomic32_t* object, uint32_t operand) {
    // The & in "=&a"(previous) makes eax an earlyclobber operand.
    // If we don't specify &, the compiler may assign eax to input operand %3 as
    // well.
    uint32_t previous;
    uint32_t temp;
    asm volatile("1:     movl    %1, %0\n"
                 "       movl    %0, %2\n"
                 "       andl    %3, %2\n"
                 "       lock; cmpxchgl %2, %1\n"
                 "       jne     1b"
                 : "=&a"(previous), "+m"(object->nonatomic), "=&r"(temp)
                 : "r"(operand));
    return previous;
}

TURF_C_INLINE uint32_t turf_fetchOr32Relaxed(turf_atomic32_t* object, uint32_t operand) {
    uint32_t previous;
    uint32_t temp;
    asm volatile("1:     movl    %1, %0\n"
                 "       movl    %0, %2\n"
                 "       orl     %3, %2\n"
                 "       lock; cmpxchgl %2, %1\n"
                 "       jne     1b"
                 : "=&a"(previous), "+m"(object->nonatomic), "=&r"(temp)
                 : "r"(operand));
    return previous;
}

#if TURF_CPU_X64
//------------------------------------------------------------------------
//  64-bit atomic operations on 64-bit processor (x64)
//------------------------------------------------------------------------

TURF_C_INLINE uint64_t turf_load64Relaxed(const turf_atomic64_t* object) {
    // On x64, aligned 64-bit loads are already atomic.
    return object->nonatomic;
}

TURF_C_INLINE void turf_store64Relaxed(turf_atomic64_t* object, uint64_t desired) {
    // On x64, aligned 64-bit stores are already atomic.
    object->nonatomic = desired;
}

TURF_C_INLINE uint64_t turf_compareExchange64Relaxed(turf_atomic64_t* object, uint64_t expected, uint64_t desired) {
    // On x64, we can work with 64-bit values directly.
    // It's basically the same as the 32-bit versions except for the q suffix on
    // opcodes.
    uint64_t previous;
    asm volatile("lock; cmpxchgq %2, %1" : "=a"(previous), "+m"(object->nonatomic) : "q"(desired), "0"(expected));
    return previous;
}

TURF_C_INLINE intreg_t turf_compareExchangeWeak64Relaxed(turf_atomic64_t* object, uint64_t* expected, uint64_t desired) {
    uint64_t e = *expected;
    uint64_t previous;
    intreg_t matched;
    asm volatile("lock; cmpxchgq %2, %1" : "=a"(previous), "+m"(object->nonatomic) : "q"(desired), "0"(e));
    matched = (previous == e);
    if (!matched)
        *expected = previous;
    return matched;
}

TURF_C_INLINE uint64_t turf_exchange64Relaxed(turf_atomic64_t* object, uint64_t desired) {
    uint64_t previous;
    asm volatile("xchgq %0, %1" : "=r"(previous), "+m"(object->nonatomic) : "0"(desired));
    return previous;
}

TURF_C_INLINE uint64_t turf_fetchAdd64Relaxed(turf_atomic64_t* object, int64_t operand) {
    uint64_t previous;
    asm volatile("lock; xaddq %0, %1" : "=r"(previous), "+m"(object->nonatomic) : "0"(operand));
    return previous;
}

TURF_C_INLINE uint64_t turf_fetchAnd64Relaxed(turf_atomic64_t* object, uint64_t operand) {
    uint64_t previous;
    uint64_t temp;
    asm volatile("1:     movq    %1, %0\n"
                 "       movq    %0, %2\n"
                 "       andq    %3, %2\n"
                 "       lock; cmpxchgq %2, %1\n"
                 "       jne     1b"
                 : "=&a"(previous), "+m"(object->nonatomic), "=&r"(temp)
                 : "r"(operand));
    return previous;
}

TURF_C_INLINE uint64_t turf_fetchOr64Relaxed(turf_atomic64_t* object, uint64_t operand) {
    uint64_t previous;
    uint64_t temp;
    asm volatile("1:     movq    %1, %0\n"
                 "       movq    %0, %2\n"
                 "       orq     %3, %2\n"
                 "       lock; cmpxchgq %2, %1\n"
                 "       jne     1b"
                 : "=&a"(previous), "+m"(object->nonatomic), "=&r"(temp)
                 : "r"(operand));
    return previous;
}

#elif TURF_CPU_X86
//------------------------------------------------------------------------
//  64-bit atomic operations on 32-bit processor (x86)
//------------------------------------------------------------------------

TURF_C_INLINE uint64_t turf_load64Relaxed(const turf_atomic64_t* object) {
    // On 32-bit x86, the most compatible way to get an atomic 64-bit load is with
    // cmpxchg8b.
    // Essentially, we perform turf_compareExchange64Relaxed(object, _dummyValue,
    // _dummyValue).
    // "=&A"(previous) outputs EAX:EDX to previous after the block, while telling
    // the compiler that
    // these registers are clobbered before %1 is used, so don't use EAX or EDX
    // for %1.
    // "m"(object->nonatomic) loads object's address into a register, which
    // becomes %1, before the block.
    // No other registers are modified.
    uint64_t previous;
    asm volatile("movl %%ebx, %%eax\n"
                 "movl %%ecx, %%edx\n"
                 "lock; cmpxchg8b %1"
                 : "=&A"(previous)
                 : "m"(object->nonatomic));
    return previous;
}

TURF_C_INLINE void turf_store64Relaxed(turf_atomic64_t* object, uint64_t desired) {
    // On 32-bit x86, the most compatible way to get an atomic 64-bit store is
    // with cmpxchg8b.
    // Essentially, we perform turf_compareExchange64Relaxed(object,
    // object->nonatomic, desired)
    // in a loop until it returns the previous value.
    // According to the Linux kernel (atomic64_cx8_32.S), we don't need the
    // "lock;" prefix
    // on cmpxchg8b since aligned 64-bit writes are already atomic on 586 and
    // newer.
    // "=m"(object->nonatomic) loads object's address into a register, which
    // becomes %0, before the block,
    // and tells the compiler the variable at address will be modified by the
    // block.
    // "b" and "c" move desired to ECX:EBX before the block.
    // "A"(expected) loads the previous value of object->nonatomic into EAX:EDX
    // before the block.
    uint64_t expected = object->nonatomic;
    asm volatile("1:    cmpxchg8b %0\n"
                 "      jne 1b"
                 : "=m"(object->nonatomic)
                 : "b"((uint32_t) desired), "c"((uint32_t)(desired >> 32)), "A"(expected));
}

TURF_C_INLINE uint64_t turf_compareExchange64Relaxed(turf_atomic64_t* object, uint64_t expected, uint64_t desired) {
    // cmpxchg8b is the only way to do 64-bit RMW operations on 32-bit x86.
    // "=A"(previous) outputs EAX:EDX to previous after the block.
    // "+m"(object->nonatomic) is the memory address that is read/written. This
    // becomes %1.
    // "b" and "c" move desired to ECX:EBX before the block.
    // "0"(expected) puts expected in the same registers as "=a"(previous), which
    // are EAX:EDX, before the block.
    uint64_t previous;
    asm volatile("lock; cmpxchg8b %1"
                 : "=A"(previous), "+m"(object->nonatomic)
                 : "b"((uint32_t) desired), "c"((uint32_t)(desired >> 32)), "0"(expected));
    return previous;
}

TURF_C_INLINE intreg_t turf_compareExchangeWeak64Relaxed(turf_atomic64_t* object, uint64_t* expected, uint64_t desired) {
    uint64_t e = *expected;
    uint64_t previous;
    intreg_t matched;
    asm volatile("lock; cmpxchg8b %1"
                 : "=A"(previous), "+m"(object->nonatomic)
                 : "b"((uint32_t) desired), "c"((uint32_t)(desired >> 32)), "0"(e));
    matched = (previous == e);
    if (!matched)
        *expected = previous;
    return matched;
}

TURF_C_INLINE uint64_t turf_exchange64Relaxed(turf_atomic64_t* object, uint64_t desired) {
    uint64_t previous = object->nonatomic;
    for (;;) {
        uint64_t probed = turf_compareExchange64Relaxed(object, previous, desired);
        if (previous == probed)
            return previous;
        previous = probed;
    }
}

TURF_C_INLINE uint64_t turf_fetchAdd64Relaxed(turf_atomic64_t* object, int64_t operand) {
    // This implementation generates an unnecessary cmp instruction after the
    // cmpxchg8b.
    // Could be optimized further using inline assembly.
    for (;;) {
        uint64_t previous = object->nonatomic;
        if (turf_compareExchange64Relaxed(object, previous, previous + operand) == previous)
            return previous;
    }
}

TURF_C_INLINE uint64_t turf_fetchAnd64Relaxed(turf_atomic64_t* object, uint64_t operand) {
    // This implementation generates an unnecessary cmp instruction after the
    // cmpxchg8b.
    // Could be optimized further using inline assembly.
    for (;;) {
        uint64_t previous = object->nonatomic;
        if (turf_compareExchange64Relaxed(object, previous, previous & operand) == previous)
            return previous;
    }
}

TURF_C_INLINE uint64_t turf_fetchOr64Relaxed(turf_atomic64_t* object, uint64_t operand) {
    // This implementation generates an unnecessary cmp instruction after the
    // cmpxchg8b.
    // Could be optimized further using inline assembly.
    for (;;) {
        uint64_t previous = object->nonatomic;
        if (turf_compareExchange64Relaxed(object, previous, previous | operand) == previous)
            return previous;
    }
}

#else
#error "Unrecognized target CPU!"
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif // TURF_C_IMPL_ATOMIC_GCC_X86_64_H
