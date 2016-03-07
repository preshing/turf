/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_IMPL_ATOMIC_BOOST_H
#define TURF_IMPL_ATOMIC_BOOST_H

#include <turf/Core.h>
#include <boost/atomic/atomic.hpp>

namespace turf {

// clang-format off
inline void signalFenceConsume() { boost::atomic_signal_fence(boost::memory_order_acquire); }
inline void signalFenceAcquire() { boost::atomic_signal_fence(boost::memory_order_acquire); }
inline void signalFenceRelease() { boost::atomic_signal_fence(boost::memory_order_release); }
inline void signalFenceSeqCst() { boost::atomic_signal_fence(boost::memory_order_seq_cst); }

inline void threadFenceConsume() { boost::atomic_thread_fence(boost::memory_order_acquire); }
inline void threadFenceAcquire() { boost::atomic_thread_fence(boost::memory_order_acquire); }
inline void threadFenceRelease() { boost::atomic_thread_fence(boost::memory_order_release); }
inline void threadFenceSeqCst() { boost::atomic_thread_fence(boost::memory_order_seq_cst); }
// clang-format on

enum MemoryOrder {
    Relaxed = boost::memory_order_relaxed,
    Consume = boost::memory_order_acquire,
    Acquire = boost::memory_order_acquire,
    Release = boost::memory_order_release,
    ConsumeRelease = boost::memory_order_acq_rel,
    AcquireRelease = boost::memory_order_acq_rel,
};

template <typename T>
class Atomic_Boost : protected boost::atomic<T> {
private:
    // Hide operator=
    T operator=(T value);

public:
    Atomic_Boost() {
    }
    Atomic_Boost(T value) : boost::atomic<T>(0) {
    }
    T loadNonatomic() const {
        return boost::atomic<T>::load(boost::memory_order_relaxed);
    }
    T load(MemoryOrder memoryOrder) const {
        return boost::atomic<T>::load((boost::memory_order) memoryOrder);
    }
    void storeNonatomic(T value) {
        return boost::atomic<T>::store(value, boost::memory_order_relaxed);
    }
    void store(T value, MemoryOrder memoryOrder) {
        return boost::atomic<T>::store(value, (boost::memory_order) memoryOrder);
    }
    T compareExchange(T expected, T desired, MemoryOrder memoryOrder) {
        boost::atomic<T>::compare_exchange_strong(expected, desired, (boost::memory_order) memoryOrder);
        return expected; // modified by reference by compare_exchange_strong
    }
    bool compareExchangeStrong(T& expected, T desired, MemoryOrder memoryOrder) {
        return boost::atomic<T>::compare_exchange_strong(expected, desired, (boost::memory_order) memoryOrder);
    }
    bool compareExchangeWeak(T& expected, T desired, MemoryOrder success, MemoryOrder failure) {
        return boost::atomic<T>::compare_exchange_weak(expected, desired, (boost::memory_order) success,
                                                       (boost::memory_order) failure);
    }
    T exchange(T desired, MemoryOrder memoryOrder) {
        return boost::atomic<T>::exchange(desired, (boost::memory_order) memoryOrder);
    }
    T fetchAdd(T operand, MemoryOrder memoryOrder) {
        return boost::atomic<T>::fetch_add(operand, (boost::memory_order) memoryOrder);
    }
    T fetchSub(T operand, MemoryOrder memoryOrder) {
        return boost::atomic<T>::fetch_sub(operand, (boost::memory_order) memoryOrder);
    }
    T fetchAnd(T operand, MemoryOrder memoryOrder) {
        return boost::atomic<T>::fetch_and(operand, (boost::memory_order) memoryOrder);
    }
    T fetchOr(T operand, MemoryOrder memoryOrder) {
        return boost::atomic<T>::fetch_or(operand, (boost::memory_order) memoryOrder);
    }
};

} // namespace turf

#endif // TURF_IMPL_ATOMIC_BOOST_H
