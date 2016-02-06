/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_ATOMIC_NATIVE_H
#define TURF_IMPL_ATOMIC_NATIVE_H

#include <turf/Core.h>
#include <turf/c/atomic.h>
#include <turf/Assert.h>

namespace turf {

// clang-format off

inline void signalFenceConsume() { turf_signalFenceConsume(); }
inline void signalFenceAcquire() { turf_signalFenceAcquire(); }
inline void signalFenceRelease() { turf_signalFenceRelease(); }
inline void signalFenceSeqCst() { turf_signalFenceSeqCst(); }

inline void threadFenceConsume() { turf_threadFenceConsume(); }
inline void threadFenceAcquire() { turf_threadFenceAcquire(); }
inline void threadFenceRelease() { turf_threadFenceRelease(); }
inline void threadFenceSeqCst() { turf_threadFenceSeqCst(); }

enum MemoryOrder {
    Relaxed = TURF_MEMORY_ORDER_RELAXED,
    Consume = TURF_MEMORY_ORDER_ACQUIRE,
    Acquire = TURF_MEMORY_ORDER_ACQUIRE,
    Release = TURF_MEMORY_ORDER_RELEASE,
    ConsumeRelease = TURF_MEMORY_ORDER_ACQ_REL,
    AcquireRelease = TURF_MEMORY_ORDER_ACQ_REL,
};

template <typename T> class Atomic_Native;

#define TURF_ATOMIC_INC_TYPE char
#include "Atomic_Native8.inc"
#undef TURF_ATOMIC_INC_TYPE

#define TURF_ATOMIC_INC_TYPE unsigned char
#include "Atomic_Native8.inc"
#undef TURF_ATOMIC_INC_TYPE

#define TURF_ATOMIC_INC_TYPE short
#include "Atomic_Native16.inc"
#undef TURF_ATOMIC_INC_TYPE

#define TURF_ATOMIC_INC_TYPE unsigned short
#include "Atomic_Native16.inc"
#undef TURF_ATOMIC_INC_TYPE

#define TURF_ATOMIC_INC_TYPE int
#include "Atomic_Native32.inc"
#undef TURF_ATOMIC_INC_TYPE

#define TURF_ATOMIC_INC_TYPE unsigned int
#include "Atomic_Native32.inc"
#undef TURF_ATOMIC_INC_TYPE

#if TURF_LONG_SIZE == 4
    #define TURF_ATOMIC_INC_TYPE long
    #include "Atomic_Native32.inc"
    #undef TURF_ATOMIC_INC_TYPE

    #define TURF_ATOMIC_INC_TYPE unsigned long
    #include "Atomic_Native32.inc"
    #undef TURF_ATOMIC_INC_TYPE
#elif TURF_LONG_SIZE == 8
    #define TURF_ATOMIC_INC_TYPE long
    #include "Atomic_Native64.inc"
    #undef TURF_ATOMIC_INC_TYPE

    #define TURF_ATOMIC_INC_TYPE unsigned long
    #include "Atomic_Native64.inc"
    #undef TURF_ATOMIC_INC_TYPE
#else
    #error "TURF_LONG_SIZE not defined!"
#endif

#if TURF_HAS_LONG_LONG
    #define TURF_ATOMIC_INC_TYPE long long
    #include "Atomic_Native64.inc"
    #undef TURF_ATOMIC_INC_TYPE

    #define TURF_ATOMIC_INC_TYPE unsigned long long
    #include "Atomic_Native64.inc"
    #undef TURF_ATOMIC_INC_TYPE
#endif

// clang-format on

// Specialize for bool
template <>
class Atomic_Native<bool> {
private:
    turf_atomic8_t m_value;

    // Hide operator=
    bool operator=(bool value);

public:
    Atomic_Native() {
    }
    Atomic_Native(bool value) {
        m_value.nonatomic = (uint8_t) value;
    }
    bool loadNonatomic() const {
        return !!m_value.nonatomic;
    }
    bool load(MemoryOrder memoryOrder) const {
        TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Acquire);
        return !!turf_load8(&m_value, (turf_memoryOrder_t) memoryOrder);
    }
    void storeNonatomic(bool value) {
        m_value.nonatomic = (uint8_t) value;
    }
    void store(bool value, MemoryOrder memoryOrder) {
        TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Release);
        turf_store8(&m_value, (uint8_t) value, (turf_memoryOrder_t) memoryOrder);
    }
    bool compareExchange(bool expected, bool desired, MemoryOrder memoryOrder) {
        return !!turf_compareExchange8(&m_value, expected, desired, (turf_memoryOrder_t) memoryOrder);
    }
    bool compareExchangeStrong(bool& expected, bool desired, MemoryOrder memoryOrder) {
        uint8_t previous =
            turf_compareExchange8(&m_value, (uint8_t) expected, (uint8_t) desired, (turf_memoryOrder_t) memoryOrder);
        bool result = (previous == (uint8_t) expected);
        if (!result)
            expected = !!previous;
        return result;
    }
    bool compareExchangeWeak(bool& expected, u8 desired, MemoryOrder success, MemoryOrder failure) {
        uint8_t expected8 = (uint8_t) expected;
        bool result = !!turf_compareExchangeWeak8(&m_value, &expected8, (uint8_t) desired, (turf_memoryOrder_t) success,
                                                  (turf_memoryOrder_t) failure);
        if (!result)
            expected = !!expected8;
        return result;
    }
    bool exchange(bool desired, MemoryOrder memoryOrder) {
        return !!turf_exchange8(&m_value, (uint8_t) desired, (turf_memoryOrder_t) memoryOrder);
    }
};

// Specialize for pointers
template <typename T>
class Atomic_Native<T*> {
private:
    turf_atomicPtr_t m_value;

    // Hide operator=
    T* operator=(T* value);

public:
    Atomic_Native() {
    }
    Atomic_Native(T* value) {
        m_value.nonatomic = value;
    }
    T* loadNonatomic() const {
        return (T*) m_value.nonatomic;
    }
    T* load(MemoryOrder memoryOrder) const {
        TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Acquire);
        return (T*) turf_loadPtr(&m_value, (turf_memoryOrder_t) memoryOrder);
    }
    void storeNonatomic(T* value) {
        m_value.nonatomic = value;
    }
    void store(T* value, MemoryOrder memoryOrder) {
        TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Release);
        turf_storePtr(&m_value, value, (turf_memoryOrder_t) memoryOrder);
    }
    T* compareExchange(T* expected, T* desired, MemoryOrder memoryOrder) {
        return (T*) turf_compareExchangePtr(&m_value, expected, desired, (turf_memoryOrder_t) memoryOrder);
    }
    bool compareExchangeStrong(T*& expected, T* desired, MemoryOrder memoryOrder) {
        T* previous = (T*) turf_compareExchangePtr(&m_value, expected, desired, (turf_memoryOrder_t) memoryOrder);
        bool result = (previous == expected);
        if (!result)
            expected = previous;
        return result;
    }
    bool compareExchangeWeak(T*& expected, T* desired, MemoryOrder success, MemoryOrder failure) {
        return !!turf_compareExchangeWeakPtr(&m_value, (void**) &expected, desired, (turf_memoryOrder_t) success,
                                             (turf_memoryOrder_t) failure);
    }
    T* exchange(T* desired, MemoryOrder memoryOrder) {
        return (T*) turf_exchangePtr(&m_value, desired, (turf_memoryOrder_t) memoryOrder);
    }
    // If you need other RMW operations on an atomic pointer, use
    // turf::Atomic<uptr> and cast argument/return values by hand
};

} // namespace turf

#endif // TURF_IMPL_ATOMIC_NATIVE_H
