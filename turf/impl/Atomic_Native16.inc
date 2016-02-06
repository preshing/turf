template <>
class Atomic_Native<TURF_ATOMIC_INC_TYPE> {
private:
    turf_atomic16_t m_value;

    // Hide operator=
    TURF_ATOMIC_INC_TYPE operator=(TURF_ATOMIC_INC_TYPE value);

public:
    Atomic_Native() {
    }
    Atomic_Native(u16 value) {
        m_value.nonatomic = value;
    }
    TURF_ATOMIC_INC_TYPE loadNonatomic() const {
        return m_value.nonatomic;
    }
    TURF_ATOMIC_INC_TYPE load(MemoryOrder memoryOrder) const {
        TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Acquire);
        return turf_load16(&m_value, (turf_memoryOrder_t) memoryOrder);
    }
    void storeNonatomic(u16 value) {
        m_value.nonatomic = value;
    }
    void store(u16 value, MemoryOrder memoryOrder) {
        TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Release);
        turf_store16(&m_value, value, (turf_memoryOrder_t) memoryOrder);
    }
    TURF_ATOMIC_INC_TYPE compareExchange(u16 expected, u16 desired, MemoryOrder memoryOrder) {
        return turf_compareExchange16(&m_value, expected, desired, (turf_memoryOrder_t) memoryOrder);
    }
    bool compareExchangeStrong(TURF_ATOMIC_INC_TYPE& expected, u16 desired, MemoryOrder memoryOrder) {
        TURF_ATOMIC_INC_TYPE previous = (TURF_ATOMIC_INC_TYPE) turf_compareExchange16(&m_value, (uint16_t) expected, desired,
                                                                                      (turf_memoryOrder_t) memoryOrder);
        bool result = (previous == expected);
        if (!result)
            expected = previous;
        return result;
    }
    bool compareExchangeWeak(TURF_ATOMIC_INC_TYPE& expected, u16 desired, MemoryOrder success, MemoryOrder failure) {
        return !!turf_compareExchangeWeak16(&m_value, (uint16_t*) &expected, desired, (turf_memoryOrder_t) success,
                                            (turf_memoryOrder_t) failure);
    }
    TURF_ATOMIC_INC_TYPE exchange(u16 desired, MemoryOrder memoryOrder) {
        return turf_exchange16(&m_value, desired, (turf_memoryOrder_t) memoryOrder);
    }
    TURF_ATOMIC_INC_TYPE fetchAdd(s16 operand, MemoryOrder memoryOrder) {
        return turf_fetchAdd16(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
    }
    TURF_ATOMIC_INC_TYPE fetchSub(s16 operand, MemoryOrder memoryOrder) {
        return turf_fetchAdd16(&m_value, -operand, (turf_memoryOrder_t) memoryOrder);
    }
    TURF_ATOMIC_INC_TYPE fetchAnd(u16 operand, MemoryOrder memoryOrder) {
        return turf_fetchAnd16(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
    }
    TURF_ATOMIC_INC_TYPE fetchOr(u16 operand, MemoryOrder memoryOrder) {
        return turf_fetchOr16(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
    }
};
