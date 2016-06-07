#pragma once

#include <cstdint>
#include <ostream>

class int48_t final {
    public:
        // default constructors
        int48_t() : _s0(0), _s1(0), _s2(0) {}
        int48_t(const int48_t& obj) = default;
        int48_t(int48_t&& obj) noexcept = default;

        // converter constructors
        int48_t(const std::int64_t i) {
            int64_to_int48(i, *this);
        }

        int48_t(const std::uint64_t i) {
            int64_to_int48(static_cast<std::int64_t>(i), *this);
        }

        // default assigments
        int48_t& operator=(const int48_t& obj) = default;
        int48_t& operator=(int48_t&& obj) noexcept = default;

        // converter assigments
        int48_t& operator=(std::int64_t i) {
            int64_to_int48(i, *this);
            return *this;
        }

        int48_t& operator=(std::uint64_t i) {
            int64_to_int48(static_cast<std::int64_t>(i), *this);
            return *this;
        }

        // cast operators
        operator std::int64_t() const {
            return int48_to_int64(*this);
        }

        operator std::uint64_t() const {
            return static_cast<std::uint64_t>(int48_to_int64(*this));
        }

        // comparison
        bool operator==(const int48_t& obj) const {
            return (_s0 == obj._s0) && (_s1 == obj._s1) && (_s2 == obj._s2);
        }

        bool operator!=(const int48_t& obj) const {
            return (_s0 != obj._s0) || (_s1 != obj._s1) || (_s2 != obj._s2);
        }

        bool operator==(const std::int64_t i) const {
            // XXX: implement fast path
            return i == int48_to_int64(*this);
        }

        bool operator!=(const std::int64_t i) const {
            // XXX: implement fast path
            return i != int48_to_int64(*this);
        }

        // printing
        friend std::ostream& operator<<(std::ostream& stream, const int48_t& obj);

    private:
        union converter64 {
            std::uint64_t u;
            std::int64_t  i;
        };

        std::uint16_t _s0;  // least significant byte
        std::uint16_t _s1;  // middle byte
        std::uint16_t _s2;  // most significant 7 bit + sign bit (which is the most significant bit)


        // converter code
        static void int64_to_int48(const std::int64_t i64, int48_t& i48) {
            converter64 c;
            c.i = i64;

            i48._s0 = static_cast<std::uint16_t>(
                  ((c.u      ) & static_cast<std::uint64_t>(0xffff))
            );

            i48._s1 = static_cast<std::uint16_t>(
                  ((c.u >> 16) & static_cast<std::uint64_t>(0xffff))
            );

            i48._s2 = static_cast<std::uint16_t>(
                  ((c.u >> 32) & static_cast<std::uint64_t>(0x7fff))  // 7 bit content
                | ((c.u >> 48) & static_cast<std::uint64_t>(0x8000))  // sign bit
            );
        }

        static std::int64_t int48_to_int64(const int48_t& i48) {
            converter64 c;
            c.u = 0;

            c.u |= static_cast<std::uint64_t>(i48._s0);

            c.u |= static_cast<std::uint64_t>(i48._s1) << 16;

            c.u |= (static_cast<std::uint64_t>(i48._s2) & static_cast<std::uint64_t>(0x7fff)) << 32;

            // recover sign bit
            if (static_cast<std::uint64_t>(i48._s2) & static_cast<std::uint64_t>(0x8000)) {
                // fill remaining bits
                c.u |= 0xffff800000000000;
            }

            return c.i;
        }
};

static_assert(sizeof(int48_t) == 6, "size of int48_t is not 48bit, check your compiler!");

std::ostream& operator<<(std::ostream& stream, const int48_t& obj) {
    stream << "int48_t(s0=" << obj._s0 << ", s1=" << obj._s1 << ", s2=" << obj._s2 << ")";
    return stream;
}
