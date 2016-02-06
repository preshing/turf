template <>
class Atomic_Native<TURF_ATOMIC_INC_TYPE> {
private:
    turf_atomic32_t m_value;

    // Hide operator=
    TURF_ATOMIC_INC_TYPE operator=(TURF_ATOMIC_INC_TYPE value);

public:
    Atomic_Native() {
    }
    Atomic_Native(u32 value) {
        m_value.nonatomic = value;
    }
    TURF_ATOMIC_INC_TYPE loadNonatomic() const {
        return m_value.nonatomic;
    }
    TURF_ATOMIC_INC_TYPE load(MemoryOrder memoryOrder) const {
        TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Acquire);
        return turf_load32(&m_value, (turf_memoryOrder_t) memoryOrder);
    }
    void storeNonatomic(u32 value) {
        m_value.nonatomic = value;
    }
    void store(u32 value, MemoryOrder memoryOrder) {
        TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Release);
        turf_store32(&m_value, value, (turf_memoryOrder_t) memoryOrder);
    }
    TURF_ATOMIC_INC_TYPE compareExchange(u32 expected, u32 desired, MemoryOrder memoryOrder) {
        return turf_compareExchange32(&m_value, expected, desired, (turf_memoryOrder_t) memoryOrder);
    }
    bool compareExchangeStrong(TURF_ATOMIC_INC_TYPE& expected, u32 desired, MemoryOrder memoryOrder) {
        TURF_ATOMIC_INC_TYPE previous = (TURF_ATOMIC_INC_TYPE) turf_compareExchange32(&m_value, (uint32_t) expected, desired,
                                                                                      (turf_memoryOrder_t) memoryOrder);
        bool result = (previous == expected);
        if (!result)
            expected = previous;
        return result;
    }
    bool compareExchangeWeak(TURF_ATOMIC_INC_TYPE& expected, u32 desired, MemoryOrder success, MemoryOrder failure) {
        return !!turf_compareExchangeWeak32(&m_value, (uint32_t*) &expected, desired, (turf_memoryOrder_t) success,
                                            (turf_memoryOrder_t) failure);
    }
    TURF_ATOMIC_INC_TYPE exchange(u32 desired, MemoryOrder memoryOrder) {
        return turf_exchange32(&m_value, desired, (turf_memoryOrder_t) memoryOrder);
    }
    TURF_ATOMIC_INC_TYPE fetchAdd(s32 operand, MemoryOrder memoryOrder) {
        return turf_fetchAdd32(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
    }
    TURF_ATOMIC_INC_TYPE fetchSub(s32 operand, MemoryOrder memoryOrder) {
        return turf_fetchAdd32(&m_value, -operand, (turf_memoryOrder_t) memoryOrder);
    }
    TURF_ATOMIC_INC_TYPE fetchAnd(u32 operand, MemoryOrder memoryOrder) {
        return turf_fetchAnd32(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
    }
    TURF_ATOMIC_INC_TYPE fetchOr(u32 operand, MemoryOrder memoryOrder) {
        return turf_fetchOr32(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
    }
};
