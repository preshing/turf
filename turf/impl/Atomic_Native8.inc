template <>
class Atomic_Native<TURF_ATOMIC_INC_TYPE> {
private:
    turf_atomic8_t m_value;

    // Hide operator=
    TURF_ATOMIC_INC_TYPE operator=(TURF_ATOMIC_INC_TYPE value);

public:
    Atomic_Native() {
    }
    Atomic_Native(u8 value) {
        m_value.nonatomic = (uint8_t) value;
    }
    TURF_ATOMIC_INC_TYPE loadNonatomic() const {
        return (TURF_ATOMIC_INC_TYPE) m_value.nonatomic;
    }
    TURF_ATOMIC_INC_TYPE load(MemoryOrder memoryOrder) const {
        TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Acquire);
        return (TURF_ATOMIC_INC_TYPE) turf_load8(&m_value, (turf_memoryOrder_t) memoryOrder);
    }
    void storeNonatomic(u8 value) {
        m_value.nonatomic = value;
    }
    void store(u8 value, MemoryOrder memoryOrder) {
        TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Release);
        turf_store8(&m_value, value, (turf_memoryOrder_t) memoryOrder);
    }
    TURF_ATOMIC_INC_TYPE compareExchange(u8 expected, u8 desired, MemoryOrder memoryOrder) {
        return turf_compareExchange8(&m_value, expected, desired, (turf_memoryOrder_t) memoryOrder);
    }
    bool compareExchangeStrong(TURF_ATOMIC_INC_TYPE& expected, u8 desired, MemoryOrder memoryOrder) {
        TURF_ATOMIC_INC_TYPE previous =
            (TURF_ATOMIC_INC_TYPE) turf_compareExchange8(&m_value, (uint8_t) expected, desired, (turf_memoryOrder_t) memoryOrder);
        bool result = (previous == expected);
        if (!result)
            expected = previous;
        return result;
    }
    bool compareExchangeWeak(TURF_ATOMIC_INC_TYPE& expected, u8 desired, MemoryOrder success, MemoryOrder failure) {
        return !!turf_compareExchangeWeak8(&m_value, (uint8_t*) &expected, desired, (turf_memoryOrder_t) success,
                                           (turf_memoryOrder_t) failure);
    }
    TURF_ATOMIC_INC_TYPE exchange(u8 desired, MemoryOrder memoryOrder) {
        return turf_exchange8(&m_value, desired, (turf_memoryOrder_t) memoryOrder);
    }
    TURF_ATOMIC_INC_TYPE fetchAdd(s8 operand, MemoryOrder memoryOrder) {
        return turf_fetchAdd8(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
    }
    TURF_ATOMIC_INC_TYPE fetchSub(s8 operand, MemoryOrder memoryOrder) {
        return turf_fetchAdd8(&m_value, -operand, (turf_memoryOrder_t) memoryOrder);
    }
    TURF_ATOMIC_INC_TYPE fetchAnd(u8 operand, MemoryOrder memoryOrder) {
        return turf_fetchAnd8(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
    }
    TURF_ATOMIC_INC_TYPE fetchOr(u8 operand, MemoryOrder memoryOrder) {
        return turf_fetchOr8(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
    }
};
