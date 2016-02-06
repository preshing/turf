/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_C_ATOMIC_H
#define TURF_C_ATOMIC_H

#include "core.h"

// clang-format off

// Choose default implementation if not already configured by turf_userconfig.h:
#if !defined(TURF_C_IMPL_ATOMIC_PATH)
    #if TURF_COMPILER_MSVC
        #define TURF_C_IMPL_ATOMIC_PATH "turf/c/impl/atomic_msvc.h"
    #elif TURF_COMPILER_GCC && (TURF_CPU_X86 || TURF_CPU_X64)
        #define TURF_C_IMPL_ATOMIC_PATH "turf/c/impl/atomic_gcc_x86-64.h"
    #elif TURF_COMPILER_GCC && TURF_CPU_ARM
        #define TURF_C_IMPL_ATOMIC_PATH "turf/c/impl/atomic_gcc_arm.h"
    #else
        #define TURF_C_IMPL_ATOMIC_PATH "*** Unable to select a default Atomic implementation ***"
    #endif
#endif

#include TURF_C_IMPL_ATOMIC_PATH

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    TURF_MEMORY_ORDER_RELAXED,
    TURF_MEMORY_ORDER_ACQUIRE,
    TURF_MEMORY_ORDER_RELEASE,
    TURF_MEMORY_ORDER_ACQ_REL
} turf_memoryOrder_t;

//--------------------------------------------------------------
//  Wrappers for 8-bit operations with built-in ordering constraints
//--------------------------------------------------------------
TURF_C_INLINE uint8_t turf_load8(const turf_atomic8_t* object, turf_memoryOrder_t memoryOrder) {
    uint8_t result = turf_load8Relaxed(object);
    if (memoryOrder == TURF_MEMORY_ORDER_ACQUIRE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL) // a little forgiving
        turf_threadFenceAcquire();
    return result;
}
TURF_C_INLINE void turf_store8(turf_atomic8_t* object, uint8_t desired, turf_memoryOrder_t memoryOrder) {
    if (memoryOrder == TURF_MEMORY_ORDER_RELEASE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL) // a little forgiving
        turf_threadFenceRelease();
    turf_store8Relaxed(object, desired);
}
TURF_C_INLINE uint8_t turf_compareExchange8(turf_atomic8_t* object, uint8_t expected, uint8_t desired, turf_memoryOrder_t memoryOrder) {
    if (memoryOrder == TURF_MEMORY_ORDER_RELEASE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceRelease();
    uint8_t result = turf_compareExchange8Relaxed(object, expected, desired);
    if (memoryOrder == TURF_MEMORY_ORDER_ACQUIRE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceAcquire();
    return result;
}
TURF_C_INLINE intreg_t turf_compareExchangeWeak8(turf_atomic8_t* object, uint8_t* expected, uint8_t desired, int success, int failure) {
    if ((success == TURF_MEMORY_ORDER_RELEASE || success == TURF_MEMORY_ORDER_ACQ_REL) ||
        (failure == TURF_MEMORY_ORDER_RELEASE || failure == TURF_MEMORY_ORDER_ACQ_REL))
        turf_threadFenceRelease();
    intreg_t result = turf_compareExchangeWeak8Relaxed(object, expected, desired);
    if (result) {
        if (success == TURF_MEMORY_ORDER_ACQUIRE || success == TURF_MEMORY_ORDER_ACQ_REL)
            turf_threadFenceAcquire();
    } else {
        if (failure == TURF_MEMORY_ORDER_ACQUIRE || failure == TURF_MEMORY_ORDER_ACQ_REL)
            turf_threadFenceAcquire();
    }
    return result;
}
TURF_C_INLINE uint8_t turf_exchange8(turf_atomic8_t* object, uint8_t desired, turf_memoryOrder_t memoryOrder) {
    if (memoryOrder == TURF_MEMORY_ORDER_RELEASE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceRelease();
    uint8_t result = turf_exchange8Relaxed(object, desired);
    if (memoryOrder == TURF_MEMORY_ORDER_ACQUIRE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceAcquire();
    return result;
}
TURF_C_INLINE uint8_t turf_fetchAdd8(turf_atomic8_t* object, int8_t operand, turf_memoryOrder_t memoryOrder) {
    if (memoryOrder == TURF_MEMORY_ORDER_RELEASE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceRelease();
    uint8_t result = turf_fetchAdd8Relaxed(object, operand);
    if (memoryOrder == TURF_MEMORY_ORDER_ACQUIRE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceAcquire();
    return result;
}
TURF_C_INLINE uint8_t turf_fetchAnd8(turf_atomic8_t* object, uint8_t operand, turf_memoryOrder_t memoryOrder) {
    if (memoryOrder == TURF_MEMORY_ORDER_RELEASE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceRelease();
    uint8_t result = turf_fetchAnd8Relaxed(object, operand);
    if (memoryOrder == TURF_MEMORY_ORDER_ACQUIRE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceAcquire();
    return result;
}
TURF_C_INLINE uint8_t turf_fetchOr8(turf_atomic8_t* object, uint8_t operand, turf_memoryOrder_t memoryOrder) {
    if (memoryOrder == TURF_MEMORY_ORDER_RELEASE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceRelease();
    uint8_t result = turf_fetchOr8Relaxed(object, operand);
    if (memoryOrder == TURF_MEMORY_ORDER_ACQUIRE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceAcquire();
    return result;
}

//--------------------------------------------------------------
//  Wrappers for 16-bit operations with built-in ordering constraints
//--------------------------------------------------------------
TURF_C_INLINE uint16_t turf_load16(const turf_atomic16_t* object, turf_memoryOrder_t memoryOrder) {
    uint16_t result = turf_load16Relaxed(object);
    if (memoryOrder == TURF_MEMORY_ORDER_ACQUIRE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL) // a little forgiving
        turf_threadFenceAcquire();
    return result;
}
TURF_C_INLINE void turf_store16(turf_atomic16_t* object, uint16_t desired, turf_memoryOrder_t memoryOrder) {
    if (memoryOrder == TURF_MEMORY_ORDER_RELEASE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL) // a little forgiving
        turf_threadFenceRelease();
    turf_store16Relaxed(object, desired);
}
TURF_C_INLINE uint16_t turf_compareExchange16(turf_atomic16_t* object, uint16_t expected, uint16_t desired, turf_memoryOrder_t memoryOrder) {
    if (memoryOrder == TURF_MEMORY_ORDER_RELEASE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceRelease();
    uint16_t result = turf_compareExchange16Relaxed(object, expected, desired);
    if (memoryOrder == TURF_MEMORY_ORDER_ACQUIRE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceAcquire();
    return result;
}
TURF_C_INLINE intreg_t turf_compareExchangeWeak16(turf_atomic16_t* object, uint16_t* expected, uint16_t desired, int success, int failure) {
    if ((success == TURF_MEMORY_ORDER_RELEASE || success == TURF_MEMORY_ORDER_ACQ_REL) ||
        (failure == TURF_MEMORY_ORDER_RELEASE || failure == TURF_MEMORY_ORDER_ACQ_REL))
        turf_threadFenceRelease();
    intreg_t result = turf_compareExchangeWeak16Relaxed(object, expected, desired);
    if (result) {
        if (success == TURF_MEMORY_ORDER_ACQUIRE || success == TURF_MEMORY_ORDER_ACQ_REL)
            turf_threadFenceAcquire();
    } else {
        if (failure == TURF_MEMORY_ORDER_ACQUIRE || failure == TURF_MEMORY_ORDER_ACQ_REL)
            turf_threadFenceAcquire();
    }
    return result;
}
TURF_C_INLINE uint16_t turf_exchange16(turf_atomic16_t* object, uint16_t desired, turf_memoryOrder_t memoryOrder) {
    if (memoryOrder == TURF_MEMORY_ORDER_RELEASE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceRelease();
    uint16_t result = turf_exchange16Relaxed(object, desired);
    if (memoryOrder == TURF_MEMORY_ORDER_ACQUIRE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceAcquire();
    return result;
}
TURF_C_INLINE uint16_t turf_fetchAdd16(turf_atomic16_t* object, int16_t operand, turf_memoryOrder_t memoryOrder) {
    if (memoryOrder == TURF_MEMORY_ORDER_RELEASE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceRelease();
    uint16_t result = turf_fetchAdd16Relaxed(object, operand);
    if (memoryOrder == TURF_MEMORY_ORDER_ACQUIRE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceAcquire();
    return result;
}
TURF_C_INLINE uint16_t turf_fetchAnd16(turf_atomic16_t* object, uint16_t operand, turf_memoryOrder_t memoryOrder) {
    if (memoryOrder == TURF_MEMORY_ORDER_RELEASE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceRelease();
    uint16_t result = turf_fetchAnd16Relaxed(object, operand);
    if (memoryOrder == TURF_MEMORY_ORDER_ACQUIRE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceAcquire();
    return result;
}
TURF_C_INLINE uint16_t turf_fetchOr16(turf_atomic16_t* object, uint16_t operand, turf_memoryOrder_t memoryOrder) {
    if (memoryOrder == TURF_MEMORY_ORDER_RELEASE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceRelease();
    uint16_t result = turf_fetchOr16Relaxed(object, operand);
    if (memoryOrder == TURF_MEMORY_ORDER_ACQUIRE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceAcquire();
    return result;
}

//--------------------------------------------------------------
//  Wrappers for 32-bit operations with built-in ordering constraints
//--------------------------------------------------------------
TURF_C_INLINE uint32_t turf_load32(const turf_atomic32_t* object, turf_memoryOrder_t memoryOrder) {
    uint32_t result = turf_load32Relaxed(object);
    if (memoryOrder == TURF_MEMORY_ORDER_ACQUIRE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL) // a little forgiving
        turf_threadFenceAcquire();
    return result;
}
TURF_C_INLINE void turf_store32(turf_atomic32_t* object, uint32_t desired, turf_memoryOrder_t memoryOrder) {
    if (memoryOrder == TURF_MEMORY_ORDER_RELEASE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL) // a little forgiving
        turf_threadFenceRelease();
    turf_store32Relaxed(object, desired);
}
TURF_C_INLINE uint32_t turf_compareExchange32(turf_atomic32_t* object, uint32_t expected, uint32_t desired, turf_memoryOrder_t memoryOrder) {
    if (memoryOrder == TURF_MEMORY_ORDER_RELEASE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceRelease();
    uint32_t result = turf_compareExchange32Relaxed(object, expected, desired);
    if (memoryOrder == TURF_MEMORY_ORDER_ACQUIRE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceAcquire();
    return result;
}
TURF_C_INLINE intreg_t turf_compareExchangeWeak32(turf_atomic32_t* object, uint32_t* expected, uint32_t desired, int success, int failure) {
    if ((success == TURF_MEMORY_ORDER_RELEASE || success == TURF_MEMORY_ORDER_ACQ_REL) ||
        (failure == TURF_MEMORY_ORDER_RELEASE || failure == TURF_MEMORY_ORDER_ACQ_REL))
        turf_threadFenceRelease();
    intreg_t result = turf_compareExchangeWeak32Relaxed(object, expected, desired);
    if (result) {
        if (success == TURF_MEMORY_ORDER_ACQUIRE || success == TURF_MEMORY_ORDER_ACQ_REL)
            turf_threadFenceAcquire();
    } else {
        if (failure == TURF_MEMORY_ORDER_ACQUIRE || failure == TURF_MEMORY_ORDER_ACQ_REL)
            turf_threadFenceAcquire();
    }
    return result;
}
TURF_C_INLINE uint32_t turf_exchange32(turf_atomic32_t* object, uint32_t desired, turf_memoryOrder_t memoryOrder) {
    if (memoryOrder == TURF_MEMORY_ORDER_RELEASE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceRelease();
    uint32_t result = turf_exchange32Relaxed(object, desired);
    if (memoryOrder == TURF_MEMORY_ORDER_ACQUIRE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceAcquire();
    return result;
}
TURF_C_INLINE uint32_t turf_fetchAdd32(turf_atomic32_t* object, int32_t operand, turf_memoryOrder_t memoryOrder) {
    if (memoryOrder == TURF_MEMORY_ORDER_RELEASE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceRelease();
    uint32_t result = turf_fetchAdd32Relaxed(object, operand);
    if (memoryOrder == TURF_MEMORY_ORDER_ACQUIRE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceAcquire();
    return result;
}
TURF_C_INLINE uint32_t turf_fetchAnd32(turf_atomic32_t* object, uint32_t operand, turf_memoryOrder_t memoryOrder) {
    if (memoryOrder == TURF_MEMORY_ORDER_RELEASE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceRelease();
    uint32_t result = turf_fetchAnd32Relaxed(object, operand);
    if (memoryOrder == TURF_MEMORY_ORDER_ACQUIRE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceAcquire();
    return result;
}
TURF_C_INLINE uint32_t turf_fetchOr32(turf_atomic32_t* object, uint32_t operand, turf_memoryOrder_t memoryOrder) {
    if (memoryOrder == TURF_MEMORY_ORDER_RELEASE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceRelease();
    uint32_t result = turf_fetchOr32Relaxed(object, operand);
    if (memoryOrder == TURF_MEMORY_ORDER_ACQUIRE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceAcquire();
    return result;
}

//--------------------------------------------------------------
//  Wrappers for 64-bit operations with built-in ordering constraints
//--------------------------------------------------------------
TURF_C_INLINE uint64_t turf_load64(const turf_atomic64_t* object, turf_memoryOrder_t memoryOrder) {
    uint64_t result = turf_load64Relaxed(object);
    if (memoryOrder == TURF_MEMORY_ORDER_ACQUIRE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL) // a little forgiving
        turf_threadFenceAcquire();
    return result;
}
TURF_C_INLINE void turf_store64(turf_atomic64_t* object, uint64_t desired, turf_memoryOrder_t memoryOrder) {
    if (memoryOrder == TURF_MEMORY_ORDER_RELEASE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL) // a little forgiving
        turf_threadFenceRelease();
    turf_store64Relaxed(object, desired);
}
TURF_C_INLINE uint64_t turf_compareExchange64(turf_atomic64_t* object, uint64_t expected, uint64_t desired, turf_memoryOrder_t memoryOrder) {
    if (memoryOrder == TURF_MEMORY_ORDER_RELEASE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceRelease();
    uint64_t result = turf_compareExchange64Relaxed(object, expected, desired);
    if (memoryOrder == TURF_MEMORY_ORDER_ACQUIRE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceAcquire();
    return result;
}
TURF_C_INLINE intreg_t turf_compareExchangeWeak64(turf_atomic64_t* object, uint64_t* expected, uint64_t desired, int success, int failure) {
    if ((success == TURF_MEMORY_ORDER_RELEASE || success == TURF_MEMORY_ORDER_ACQ_REL) ||
        (failure == TURF_MEMORY_ORDER_RELEASE || failure == TURF_MEMORY_ORDER_ACQ_REL))
        turf_threadFenceRelease();
    intreg_t result = turf_compareExchangeWeak64Relaxed(object, expected, desired);
    if (result) {
        if (success == TURF_MEMORY_ORDER_ACQUIRE || success == TURF_MEMORY_ORDER_ACQ_REL)
            turf_threadFenceAcquire();
    } else {
        if (failure == TURF_MEMORY_ORDER_ACQUIRE || failure == TURF_MEMORY_ORDER_ACQ_REL)
            turf_threadFenceAcquire();
    }
    return result;
}
TURF_C_INLINE uint64_t turf_exchange64(turf_atomic64_t* object, uint64_t desired, turf_memoryOrder_t memoryOrder) {
    if (memoryOrder == TURF_MEMORY_ORDER_RELEASE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceRelease();
    uint64_t result = turf_exchange64Relaxed(object, desired);
    if (memoryOrder == TURF_MEMORY_ORDER_ACQUIRE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceAcquire();
    return result;
}
TURF_C_INLINE uint64_t turf_fetchAdd64(turf_atomic64_t* object, int64_t operand, turf_memoryOrder_t memoryOrder) {
    if (memoryOrder == TURF_MEMORY_ORDER_RELEASE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceRelease();
    uint64_t result = turf_fetchAdd64Relaxed(object, operand);
    if (memoryOrder == TURF_MEMORY_ORDER_ACQUIRE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceAcquire();
    return result;
}
TURF_C_INLINE uint64_t turf_fetchAnd64(turf_atomic64_t* object, uint64_t operand, turf_memoryOrder_t memoryOrder) {
    if (memoryOrder == TURF_MEMORY_ORDER_RELEASE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceRelease();
    uint64_t result = turf_fetchAnd64Relaxed(object, operand);
    if (memoryOrder == TURF_MEMORY_ORDER_ACQUIRE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceAcquire();
    return result;
}
TURF_C_INLINE uint64_t turf_fetchOr64(turf_atomic64_t* object, uint64_t operand, turf_memoryOrder_t memoryOrder) {
    if (memoryOrder == TURF_MEMORY_ORDER_RELEASE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceRelease();
    uint64_t result = turf_fetchOr64Relaxed(object, operand);
    if (memoryOrder == TURF_MEMORY_ORDER_ACQUIRE || memoryOrder == TURF_MEMORY_ORDER_ACQ_REL)
        turf_threadFenceAcquire();
    return result;
}

//--------------------------------------------------------------
//  Pointer-sized atomic operations
//--------------------------------------------------------------
#if TURF_PTR_SIZE == 4
    TURF_C_INLINE void* turf_loadPtrRelaxed(const turf_atomicPtr_t* object) {
        return (void*) turf_load32Relaxed((const turf_atomic32_t*) object);
    }
    TURF_C_INLINE void turf_storePtrRelaxed(turf_atomicPtr_t* object, void* desired) {
        turf_store32Relaxed((turf_atomic32_t*) object, (size_t) desired);
    }
    TURF_C_INLINE void* turf_compareExchangePtrRelaxed(turf_atomicPtr_t* object, void* expected, void* desired) {
        return (void*) turf_compareExchange32Relaxed((turf_atomic32_t*) object, (size_t) expected, (size_t) desired);
    }
    TURF_C_INLINE intreg_t turf_compareExchangeWeakPtrRelaxed(turf_atomicPtr_t* object, void** expected, void* desired) {
        return turf_compareExchangeWeak32Relaxed((turf_atomic32_t*) object, (uint32_t*) expected, (size_t) desired);
    }
    TURF_C_INLINE void* turf_exchangePtrRelaxed(turf_atomicPtr_t* object, void* desired) {
        return (void*) turf_exchange32Relaxed((turf_atomic32_t*) object, (size_t) desired);
    }
    TURF_C_INLINE void* turf_fetchAddPtrRelaxed(turf_atomicPtr_t* object, ptrdiff_t operand) {
        return (void*) turf_fetchAdd32Relaxed((turf_atomic32_t*) object, operand);
    }
    TURF_C_INLINE void* turf_fetchAndPtrRelaxed(turf_atomicPtr_t* object, size_t operand) {
        return (void*) turf_fetchAnd32Relaxed((turf_atomic32_t*) object, operand);
    }
    TURF_C_INLINE void* turf_fetchOrPtrRelaxed(turf_atomicPtr_t* object, size_t operand) {
        return (void*) turf_fetchOr32Relaxed((turf_atomic32_t*) object, operand);
    }
    TURF_C_INLINE void* turf_loadPtr(const turf_atomicPtr_t* object, turf_memoryOrder_t memoryOrder) {
        return (void*) turf_load32((const turf_atomic32_t*) object, memoryOrder);
    }
    TURF_C_INLINE void turf_storePtr(turf_atomicPtr_t* object, void* desired, turf_memoryOrder_t memoryOrder) {
        turf_store32((turf_atomic32_t*) object, (size_t) desired, memoryOrder);
    }
    TURF_C_INLINE void* turf_compareExchangePtr(turf_atomicPtr_t* object, void* expected, void* desired, turf_memoryOrder_t memoryOrder) {
        return (void*) turf_compareExchange32((turf_atomic32_t*) object, (size_t) expected, (size_t) desired, memoryOrder);
    }
    TURF_C_INLINE intreg_t turf_compareExchangeWeakPtr(turf_atomicPtr_t* object, void** expected, void* desired, int success, int failure) {
        return turf_compareExchangeWeak32((turf_atomic32_t*) object, (uint32_t*) expected, (size_t) desired, success, failure);
    }
    TURF_C_INLINE void* turf_exchangePtr(turf_atomicPtr_t* object, void* desired, turf_memoryOrder_t memoryOrder) {
        return (void*) turf_exchange32((turf_atomic32_t*) object, (size_t) desired, memoryOrder);
    }
    TURF_C_INLINE void* turf_fetchAddPtr(turf_atomicPtr_t* object, ptrdiff_t operand, turf_memoryOrder_t memoryOrder) {
        return (void*) turf_fetchAdd32((turf_atomic32_t*) object, operand, memoryOrder);
    }
    TURF_C_INLINE void* turf_fetchAndPtr(turf_atomicPtr_t* object, size_t operand, turf_memoryOrder_t memoryOrder) {
        return (void*) turf_fetchAnd32((turf_atomic32_t*) object, operand, memoryOrder);
    }
    TURF_C_INLINE void* turf_fetchOrPtr(turf_atomicPtr_t* object, size_t operand, turf_memoryOrder_t memoryOrder) {
        return (void*) turf_fetchOr32((turf_atomic32_t*) object, operand, memoryOrder);
    }
#elif TURF_PTR_SIZE == 8
    TURF_C_INLINE void* turf_loadPtrRelaxed(const turf_atomicPtr_t* object) {
        return (void*) turf_load64Relaxed((const turf_atomic64_t*) object);
    }
    TURF_C_INLINE void turf_storePtrRelaxed(turf_atomicPtr_t* object, void* desired) {
        turf_store64Relaxed((turf_atomic64_t*) object, (size_t) desired);
    }
    TURF_C_INLINE void* turf_compareExchangePtrRelaxed(turf_atomicPtr_t* object, void* expected, void* desired) {
        return (void*) turf_compareExchange64Relaxed((turf_atomic64_t*) object, (size_t) expected, (size_t) desired);
    }
    TURF_C_INLINE intreg_t turf_compareExchangeWeakPtrRelaxed(turf_atomicPtr_t* object, void** expected, void* desired) {
        return turf_compareExchangeWeak64Relaxed((turf_atomic64_t*) object, (uint64_t*) expected, (size_t) desired);
    }
    TURF_C_INLINE void* turf_exchangePtrRelaxed(turf_atomicPtr_t* object, void* desired) {
        return (void*) turf_exchange64Relaxed((turf_atomic64_t*) object, (size_t) desired);
    }
    TURF_C_INLINE void* turf_fetchAddPtrRelaxed(turf_atomicPtr_t* object, ptrdiff_t operand) {
        return (void*) turf_fetchAdd64Relaxed((turf_atomic64_t*) object, operand);
    }
    TURF_C_INLINE void* turf_fetchAndPtrRelaxed(turf_atomicPtr_t* object, size_t operand) {
        return (void*) turf_fetchAnd64Relaxed((turf_atomic64_t*) object, operand);
    }
    TURF_C_INLINE void* turf_fetchOrPtrRelaxed(turf_atomicPtr_t* object, size_t operand) {
        return (void*) turf_fetchOr64Relaxed((turf_atomic64_t*) object, operand);
    }
    TURF_C_INLINE void* turf_loadPtr(const turf_atomicPtr_t* object, turf_memoryOrder_t memoryOrder) {
        return (void*) turf_load64((const turf_atomic64_t*) object, memoryOrder);
    }
    TURF_C_INLINE void turf_storePtr(turf_atomicPtr_t* object, void* desired, turf_memoryOrder_t memoryOrder) {
        turf_store64((turf_atomic64_t*) object, (size_t) desired, memoryOrder);
    }
    TURF_C_INLINE void* turf_compareExchangePtr(turf_atomicPtr_t* object, void* expected, void* desired, turf_memoryOrder_t memoryOrder) {
        return (void*) turf_compareExchange64((turf_atomic64_t*) object, (size_t) expected, (size_t) desired, memoryOrder);
    }
    TURF_C_INLINE intreg_t turf_compareExchangeWeakPtr(turf_atomicPtr_t* object, void** expected, void* desired, int success, int failure) {
        return turf_compareExchangeWeak64((turf_atomic64_t*) object, (uint64_t*) expected, (size_t) desired, success, failure);
    }
    TURF_C_INLINE void* turf_exchangePtr(turf_atomicPtr_t* object, void* desired, turf_memoryOrder_t memoryOrder) {
        return (void*) turf_exchange64((turf_atomic64_t*) object, (size_t) desired, memoryOrder);
    }
    TURF_C_INLINE void* turf_fetchAddPtr(turf_atomicPtr_t* object, ptrdiff_t operand, turf_memoryOrder_t memoryOrder) {
        return (void*) turf_fetchAdd64((turf_atomic64_t*) object, operand, memoryOrder);
    }
    TURF_C_INLINE void* turf_fetchAndPtr(turf_atomicPtr_t* object, size_t operand, turf_memoryOrder_t memoryOrder) {
        return (void*) turf_fetchAnd64((turf_atomic64_t*) object, operand, memoryOrder);
    }
    TURF_C_INLINE void* turf_fetchOrPtr(turf_atomicPtr_t* object, size_t operand, turf_memoryOrder_t memoryOrder) {
        return (void*) turf_fetchOr64((turf_atomic64_t*) object, operand, memoryOrder);
    }
#else
    #error "TURF_PTR_SIZE not set!"
#endif

// clang-format on

#ifdef __cplusplus
} // extern "C"
#endif

#endif // TURF_C_ATOMIC_H
