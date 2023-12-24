#include <cstdint>

#include "doctest.h"
#include "ebl/bit.h"

enum class TestEnum : uint16_t { A = 0, B = 1, C = 2, D = 3 };

union TestUnion {
   uint16_t value;
   ebl::BitField<uint16_t, 0, 1> f0;
   ebl::BitField<uint16_t, 1, 7> f1;
   ebl::BitField<uint16_t, 0, 8> f2;
   ebl::BitField<uint16_t, 8, 8> f3;
   ebl::BitField<TestEnum, 8, 2> f4;
};

TEST_CASE("bitfield test") {
   TestUnion u;
   u.value = 0;
   u.f0 = 0b1;
   CHECK(u.value == 1);
   u.f1 = 0b111'1111;
   CHECK(u.value == 0b1111'1111);
   u.f2 = 0b0101'0101;
   CHECK(u.value == 0b0101'0101);
   CHECK(u.f0 == 0b1);
   CHECK(u.f1 == 0b0101'010);
   u.f2 = 0xf0;
   u.f3 = 0x82;
   CHECK(u.value == 0x82f0);
   u.f2 = 0;
   CHECK(!u.f1);
   CHECK(u.f3);
   CHECK((TestEnum)u.f4 == TestEnum::C);
   u.f4 = TestEnum::D;
   CHECK(u.value == 0x8300);
   u.f4 = TestEnum::A;
   CHECK(u.value == 0x8000);
}
