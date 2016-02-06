template <>
class Atomic_Native<TURF_ATOMIC_INC_TYPE> {
private:
    turf_atomic64_t m_value;

    // Hide operator=
    TURF_ATOMIC_INC_TYPE operator=(TURF_ATOMIC_INC_TYPE value);

public:
    Atomic_Native() {
    }
    Atomic_Native(u64 value) {
        m_value.nonatomic = value;
    }
    TURF_ATOMIC_INC_TYPE loadNonatomic() const {
        return m_value.nonatomic;
    }
    TURF_ATOMIC_INC_TYPE load(MemoryOrder memoryOrder) const {
        TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Acquire);
        return turf_load64(&m_value, (turf_memoryOrder_t) memoryOrder);
    }
    void storeNonatomic(u64 value) {
        m_value.nonatomic = value;
    }
    void store(u64 value, MemoryOrder memoryOrder) {
        TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Release);
        turf_store64(&m_value, value, (turf_memoryOrder_t) memoryOrder);
    }
    TURF_ATOMIC_INC_TYPE compareExchange(u64 expected, u64 desired, MemoryOrder memoryOrder) {
        return turf_compareExchange64(&m_value, expected, desired, (turf_memoryOrder_t) memoryOrder);
    }
    bool compareExchangeStrong(TURF_ATOMIC_INC_TYPE& expected, u64 desired, MemoryOrder memoryOrder) {
        TURF_ATOMIC_INC_TYPE previous = (TURF_ATOMIC_INC_TYPE) turf_compareExchange64(&m_value, (uint64_t) expected, desired,
                                                                                      (turf_memoryOrder_t) memoryOrder);
        bool result = (previous == expected);
        if (!result)
            expected = previous;
        return result;
    }
    bool compareExchangeWeak(TURF_ATOMIC_INC_TYPE& expected, u64 desired, MemoryOrder success, MemoryOrder failure) {
        return !!turf_compareExchangeWeak64(&m_value, (uint64_t*) &expected, desired, (turf_memoryOrder_t) success,
                                            (turf_memoryOrder_t) failure);
    }
    TURF_ATOMIC_INC_TYPE exchange(u64 desired, MemoryOrder memoryOrder) {
        return turf_exchange64(&m_value, desired, (turf_memoryOrder_t) memoryOrder);
    }
    TURF_ATOMIC_INC_TYPE fetchAdd(s64 operand, MemoryOrder memoryOrder) {
        return turf_fetchAdd64(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
    }
    TURF_ATOMIC_INC_TYPE fetchSub(s64 operand, MemoryOrder memoryOrder) {
        return turf_fetchAdd64(&m_value, -operand, (turf_memoryOrder_t) memoryOrder);
    }
    TURF_ATOMIC_INC_TYPE fetchAnd(u64 operand, MemoryOrder memoryOrder) {
        return turf_fetchAnd64(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
    }
    TURF_ATOMIC_INC_TYPE fetchOr(u64 operand, MemoryOrder memoryOrder) {
        return turf_fetchOr64(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
    }
};
