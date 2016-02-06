/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_BITFIELD_H
#define TURF_BITFIELD_H

#include <turf/Core.h>
#include <turf/Assert.h>

namespace turf {

//---------------------------------------------------------
// BitFieldMember<>: Used internally by TURF_ADD_BITFIELD_MEMBER macro.
//---------------------------------------------------------
template <typename T, int Offset, int Bits>
struct BitFieldMember {
    T value;

    TURF_STATIC_ASSERT(Offset + Bits <= (int) sizeof(T) * 8);
    TURF_STATIC_ASSERT(Bits < (int) sizeof(T) * 8);

    static const T Maximum = (T(1) << Bits) - 1;
    static const T Mask = Maximum << Offset;
    T maximum() const {
        return Maximum;
    }
    T one() const {
        return T(1) << Offset;
    }
    operator T() const {
        return (value >> Offset) & Maximum;
    }
    BitFieldMember& operator=(T v) {
        TURF_ASSERT(v <= Maximum); // v must fit inside the bitfield member
        value = (value & ~Mask) | (v << Offset);
        return *this;
    }
    BitFieldMember& operator=(const BitFieldMember& v) {
        return * this = T(v);
    }
    BitFieldMember& operator+=(T v) {
        TURF_ASSERT(T(*this) + v <= Maximum); // result must fit inside the bitfield member
        value += v << Offset;
        return *this;
    }
    BitFieldMember& operator-=(T v) {
        TURF_ASSERT(T(*this) >= v); // result must not underflow
        value -= v << Offset;
        return *this;
    }
    BitFieldMember& operator++() {
        return * this += 1;
    }
    BitFieldMember& operator++(int) {
        return * this += 1;
    } // postfix form
    BitFieldMember& operator--() {
        return * this -= 1;
    }
    BitFieldMember& operator--(int) {
        return * this -= 1;
    } // postfix form
};

//---------------------------------------------------------
// BitFieldArray<>: Used internally by TURF_ADD_BITFIELD_ARRAY macro.
// All members are public to simplify compliance with sections 9.0.7 and
// 9.5.1 of the C++11 standard, thereby avoiding undefined behavior.
//---------------------------------------------------------
template <typename T, int BaseOffset, int BitsPerItem, int NumItems>
struct BitFieldArray {
    T value;

    TURF_STATIC_ASSERT(BaseOffset + BitsPerItem * NumItems <= (int) sizeof(T) * 8);
    TURF_STATIC_ASSERT(BitsPerItem < (int) sizeof(T) * 8);

    static const T Maximum = (T(1) << BitsPerItem) - 1;
    T maximum() const {
        return Maximum;
    }
    int numItems() const {
        return NumItems;
    }

    class Element {
    private:
        T& value;
        int offset;

    public:
        Element(T& value, int offset) : value(value), offset(offset) {
        }
        T mask() const {
            return Maximum << offset;
        }
        operator T() const {
            return (value >> offset) & Maximum;
        }
        Element& operator=(T v) {
            TURF_ASSERT(v <= Maximum); // v must fit inside the bitfield member
            value = (value & ~mask()) | (v << offset);
            return *this;
        }
        Element& operator+=(T v) {
            TURF_ASSERT(T(*this) + v <= Maximum); // result must fit inside the bitfield member
            value += v << offset;
            return *this;
        }
        Element& operator-=(T v) {
            TURF_ASSERT(T(*this) >= v); // result must not underflow
            value -= v << offset;
            return *this;
        }
        Element& operator++() {
            return * this += 1;
        }
        Element& operator++(int) {
            return * this += 1;
        } // postfix form
        Element& operator--() {
            return * this -= 1;
        }
        Element& operator--(int) {
            return * this -= 1;
        } // postfix form
    };

    Element operator[](int i) {
        TURF_ASSERT(i >= 0 && i < NumItems); // array index must be in range
        return Element(value, BaseOffset + BitsPerItem * i);
    }
    const Element operator[](int i) const {
        TURF_ASSERT(i >= 0 && i < NumItems); // array index must be in range
        return Element(value, BaseOffset + BitsPerItem * i);
    }
};

//---------------------------------------------------------
// Bitfield definition macros.
//---------------------------------------------------------
// clang-format off
#define TURF_BEGIN_BITFIELD_TYPE(typeName, T) \
    union typeName { \
        struct Wrapper { T value; }; \
        Wrapper wrapper; \
        typeName(T v = 0) { wrapper.value = v; } \
        typeName& operator=(T v) { wrapper.value = v; return *this; } \
        operator T&() { return wrapper.value; } \
        operator T() const { return wrapper.value; } \
        typedef T StorageType;

#define TURF_ADD_BITFIELD_MEMBER(memberName, offset, bits) \
        BitFieldMember<StorageType, offset, bits> memberName;

#define TURF_ADD_BITFIELD_ARRAY(memberName, offset, bits, numItems) \
        BitFieldArray<StorageType, offset, bits, numItems> memberName;

#define TURF_END_BITFIELD_TYPE() \
    };
// clang-format on

} // namespace turf

#endif // TURF_BITFIELD_H
