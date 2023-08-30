#pragma once
#include <stdint.h>
#include <ebl/type_traits.h>

namespace ebl {
    
    /**
     * @brief Implements platform-independent bitfields.
     * 
     * @tparam T 
     * @tparam Index 
     * @tparam Bits 
     */
    template<typename T, int Index, int Bits>
    struct BitField {
    private:
        static constexpr int Size = sizeof(T) * 8;
        // Underlying type of T is T2
        // TODO: This needs extra error checking.
        typedef
            ebl::conditional_t<Size == 0 , void,
            ebl::conditional_t<Size <= 8 , uint8_t,
            ebl::conditional_t<Size <= 16, uint16_t,
            ebl::conditional_t<Size <= 32, uint32_t,
            ebl::conditional_t<Size <= 64, uint64_t, void>>>>> T2;
        // Mask of Bits bits starting at Index
        static constexpr T2 Mask = (1u << Bits) - 1u;
    public:
        // Assignment operator from T to T2
        BitField operator=(T value) {
            value_ = (value_ & ~(Mask << Index)) | (((T2)value & Mask) << Index);
            return *this;
        }
        // Get value as T
        operator T() const { return (T) ((value_ >> Index) & Mask); }
        // Get value as bool
        explicit operator bool() const { return value_ & (Mask << Index); }
    private:
        // Bitfield underlying value T2
        T2 value_;
    };

} // namespace ebl
