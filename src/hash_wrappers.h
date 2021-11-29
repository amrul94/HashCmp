#ifndef THESIS_WORK_HASH_WRAPPERS_H
#define THESIS_WORK_HASH_WRAPPERS_H

#include <array>
#include <cstring>
#include <fstream>
#include <filesystem>
#include <functional>
#include <mutex>
#include <string>
#include <string_view>

#include <boost/multiprecision/cpp_int.hpp>

#include <rolling_hash/cyclichash.h>

#include "img.h"


// HFL = Hash function library
namespace hfl {

    using uint24_t = boost::multiprecision::number<boost::multiprecision::cpp_int_backend<24, 24,
            boost::multiprecision::unsigned_magnitude,
            boost::multiprecision::unchecked, void>>;


    class [[maybe_unused]] bitfield24 {
    private:
        uint32_t value : 24;
    };


    std::string ReadFile(std::ifstream& file);

    namespace detail {
        template<class Type>
        std::string WriteToString(Type source) {
            auto size = sizeof(Type);
            std::string str;
            str.resize(size);
            memcpy(str.data(), &source, size);
            return str;
        }

        template<typename HashType>
        class BaseHashWrapper {
        public:
            HashType operator()(std::string_view str) const {
                return Hash(str);
            }

            HashType operator()(std::ifstream& file) const {
                std::string binary_file = ReadFile(file);
                assert(!binary_file.empty());
                return Hash(binary_file);
            }

            HashType operator()(const img::Image& image) const {

                // Возможно стоит сделать так:
                // const char* bytes = reinterpret_cast<const char*>(image.GetLine(0);
                // const size_t size = image.GetHeight() * image.GetWidth();
                // const std::string_view str(bytes, size);
                // return Hash(str);

                const char* bytes = reinterpret_cast<const char*>(image.GetLine(0));
                return Hash(bytes);
            }

            HashType operator()(int8_t number) const {
                uint8_t number8 = number;
                uint64_t number64 = number8;
                return operator()(number64);
            }

            HashType operator()(int16_t number) const {
                uint64_t number64 = number;
                return operator()(number64);
            }

            HashType operator()(int32_t number) const {
                uint64_t number64 = number;
                return operator()(number64);
            }

            HashType operator()(int64_t number) const {
                uint64_t number64 = number;
                return operator()(number64);
            }


            HashType operator()(uint8_t number) const {
                uint64_t number64 = number;
                return operator()(number64);
            }

            HashType operator()(uint16_t number) const {
                uint64_t number64 = number;
                return operator()(number64);
            }

            HashType operator()(uint32_t number) const {
                uint64_t number64 = number;
                return operator()(number64);
            }

            HashType operator()(uint64_t number) const {
                std::string bytes = WriteToString<uint64_t>(number);
                assert(sizeof(bytes[0]) == 1);
                assert((&bytes[1] - &bytes[0]) == 1);
                return Hash(bytes);
            }

            virtual ~BaseHashWrapper() = default;

        private:
            [[nodiscard]] virtual HashType Hash(std::string_view str) const = 0;
        };
    }


    using BaseHash16Wrapper = detail::BaseHashWrapper<uint16_t>;
    using BaseHash24Wrapper = detail::BaseHashWrapper<uint24_t>;
    using BaseHash32Wrapper = detail::BaseHashWrapper<uint32_t>;
    using BaseHash64Wrapper = detail::BaseHashWrapper<uint64_t>;

//----- Bernstein's hash DJB2 ------

    class [[maybe_unused]] DJB2Hash16Wrapper : public BaseHash16Wrapper {
    private:
        [[nodiscard]] uint16_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] DJB2Hash24Wrapper : public BaseHash24Wrapper {
    private:
        [[nodiscard]] uint24_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] DJB2Hash32Wrapper : public BaseHash32Wrapper {
    private:
        [[nodiscard]] uint32_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] DJB2Hash64Wrapper : public BaseHash64Wrapper {
    private:
        [[nodiscard]] uint64_t Hash(std::string_view str) const override;
    };

//----- Rolling Hash (BuzHash) -----

    class [[maybe_unused]] BuzHash16Wrapper : public BaseHash16Wrapper {
    public:
        BuzHash16Wrapper() noexcept = default;

    private:
        uint16_t Hash(std::string_view str) const override;

        mutable std::mutex hash_mutex_;
        mutable CyclicHash<uint16_t, unsigned char> hasher_{1024, 16};
    };

    class [[maybe_unused]] BuzHash24Wrapper : public BaseHash24Wrapper {
    public:
        BuzHash24Wrapper() noexcept = default;

    private:
        uint24_t Hash(std::string_view str) const override;

        mutable std::mutex hash_mutex_;
        mutable CyclicHash<uint32_t, unsigned char> hasher_{1024, 24};
    };

    class [[maybe_unused]] BuzHash32Wrapper : public BaseHash32Wrapper {
    public:
        BuzHash32Wrapper() noexcept = default;

    private:
        uint32_t Hash(std::string_view str) const override;

        mutable std::mutex hash_mutex_;
        mutable CyclicHash<uint32_t, unsigned char> hasher_{1024, 32};
    };

    class [[maybe_unused]] BuzHash64Wrapper : public BaseHash64Wrapper {
    public:
        BuzHash64Wrapper() noexcept = default;

    private:
        uint64_t Hash(std::string_view str) const override;

        mutable std::mutex hash_mutex_;
        mutable CyclicHash<uint64_t, unsigned char> hasher_{1024, 64};
    };

//----------- CityHashes ----------

    class [[maybe_unused]] CityHash32Wrapper : public BaseHash32Wrapper {
    private:
        [[nodiscard]] uint32_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] CityHash64Wrapper : public BaseHash64Wrapper {
    private:
        [[nodiscard]] uint64_t Hash(std::string_view str) const override;
    };

//----------- FarmHashes ----------

    class [[maybe_unused]] FarmHash32Wrapper : public BaseHash32Wrapper {
    private:
        [[nodiscard]] uint32_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] FarmHash64Wrapper : public BaseHash64Wrapper {
    private:
        [[nodiscard]] uint64_t Hash(std::string_view str) const override;
    };

//------------ FastHash ------------

    class [[maybe_unused]] FastHash16Wrapper : public BaseHash16Wrapper {
    private:
        [[nodiscard]] uint16_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] FastHash24Wrapper : public BaseHash24Wrapper {
    private:
        [[nodiscard]] uint24_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] FastHash32Wrapper : public BaseHash32Wrapper {
    private:
        [[nodiscard]] uint32_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] FastHash64Wrapper : public BaseHash64Wrapper {
    private:
        [[nodiscard]] uint64_t Hash(std::string_view str) const override;
    };

//---------- FNV-1a hash -----------

    class [[maybe_unused]] FNV1aHash16Wrapper : public BaseHash16Wrapper {
    private:
        [[nodiscard]] uint16_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] FNV1aHash24Wrapper : public BaseHash24Wrapper {
    private:
        [[nodiscard]] uint24_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] FNV1aHash32Wrapper : public BaseHash32Wrapper {
    private:
        [[nodiscard]] uint32_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] FNV1aHash64Wrapper : public BaseHash64Wrapper {
    private:
        [[nodiscard]] uint64_t Hash(std::string_view str) const override;
    };


//--------- Jenkins hash -----------

    class [[maybe_unused]] OneTimeHash16Wrapper : public BaseHash16Wrapper {
    private:
        [[nodiscard]] uint16_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] OneTimeHash24Wrapper : public BaseHash24Wrapper {
    private:
        [[nodiscard]] uint24_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] OneTimeHash32Wrapper : public BaseHash32Wrapper {
    private:
        [[nodiscard]] uint32_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] OneTimeHash64Wrapper : public BaseHash64Wrapper {
    private:
        [[nodiscard]] uint64_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] Lookup3LittleWrapper : public BaseHash32Wrapper {
    private:
        [[nodiscard]] uint32_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] Lookup3BigWrapper : public BaseHash32Wrapper {
    private:
        [[nodiscard]] uint32_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] SpookyHash16Wrapper : public BaseHash16Wrapper {
    private:
        [[nodiscard]] uint16_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] SpookyHash24Wrapper : public BaseHash24Wrapper {
    private:
        [[nodiscard]] uint24_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] SpookyHash32Wrapper : public BaseHash32Wrapper {
    private:
        [[nodiscard]] uint32_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] SpookyHash64Wrapper : public BaseHash64Wrapper {
    private:
        [[nodiscard]] uint64_t Hash(std::string_view str) const override;
    };


//------------ MetroHash -----------

    class [[maybe_unused]] MetroHash64_Wrapper : public BaseHash64Wrapper {
    private:
        [[nodiscard]] uint64_t Hash(std::string_view str) const override;
    };

//---------- MurmurHashes ---------

    class [[maybe_unused]] MurmurHash1Wrapper : public BaseHash32Wrapper {
    private:
        [[nodiscard]] uint32_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] MurmurHash2Wrapper : public BaseHash32Wrapper {
    private:
        [[nodiscard]] uint32_t Hash(std::string_view str) const override;
    };
    class [[maybe_unused]] MurmurHash2AWrapper : public BaseHash32Wrapper {
    private:
        [[nodiscard]] uint32_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] MurmurHash64AWrapper: public BaseHash64Wrapper  {
    private:
        [[nodiscard]] uint64_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] MurmurHash3Wrapper: public BaseHash32Wrapper  {
    private:
        [[nodiscard]] uint32_t Hash(std::string_view str) const override;
    };

//--- Paul Hsieh's SuperFastHash ---

    class [[maybe_unused]] SuperFastHashWrapper : public BaseHash32Wrapper {
    private:
        [[nodiscard]] uint32_t Hash(std::string_view str) const override;
    };

//---------- PearsonHashes ---------

    class [[maybe_unused]] PearsonHash16Wrapper : public BaseHash16Wrapper {
    public:
        PearsonHash16Wrapper() noexcept;

    private:
        void PearsonHashInit() const;
        [[nodiscard]] uint16_t Hash(std::string_view str) const override;

        mutable std::vector<uint16_t> t16;
        mutable std::once_flag init_flag;
    };

    class [[maybe_unused]] PearsonHash24Wrapper : public BaseHash24Wrapper {
    public:
        PearsonHash24Wrapper() noexcept;

    private:
        void PearsonHashInit() const;
        [[nodiscard]] uint24_t Hash(std::string_view str) const override;

        mutable std::vector<uint32_t> t24;
        mutable std::once_flag init_flag;
    };
    class [[maybe_unused]] PearsonHash32Wrapper : public BaseHash32Wrapper {
    private:
        [[nodiscard]] uint32_t Hash(std::string_view str) const override;

        mutable std::once_flag init_flag;
    };

    class [[maybe_unused]] PearsonHash64Wrapper : public BaseHash64Wrapper {
    private:
        [[nodiscard]] uint64_t Hash(std::string_view str) const override;

        mutable std::once_flag init_flag;
    };

//------------ PJW Hash ------------

    class [[maybe_unused]] PJWHash16Wrapper : public BaseHash16Wrapper {
    private:
        [[nodiscard]] uint16_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] PJWHash24Wrapper : public BaseHash24Wrapper {
    private:
        [[nodiscard]] uint24_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] PJWHash32Wrapper : public BaseHash32Wrapper {
    private:
        [[nodiscard]] uint32_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] PJWHash64Wrapper : public BaseHash64Wrapper {
    private:
        [[nodiscard]] uint64_t Hash(std::string_view str) const override;
    };

//-------------- SDBM --------------

    class [[maybe_unused]] SDBMHash16Wrapper : public BaseHash16Wrapper {
    private:
        [[nodiscard]] uint16_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] SDBMHash24Wrapper : public BaseHash24Wrapper {
    private:
        [[nodiscard]] uint24_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] SDBMHash32Wrapper : public BaseHash32Wrapper {
    private:
        [[nodiscard]] uint32_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] SDBMHash64Wrapper : public BaseHash64Wrapper {
    private:
        [[nodiscard]] uint64_t Hash(std::string_view str) const override;
    };

//-------------- T1HA --------------

    class [[maybe_unused]] T1HA0_32leWrapper : public BaseHash32Wrapper {
    private:
        [[nodiscard]] uint32_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] T1HA0_32beWrapper : public BaseHash32Wrapper {
    private:
        [[nodiscard]] uint32_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] T1HA1LeWrapper : public BaseHash64Wrapper {
    private:
        [[nodiscard]] uint64_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] T1HA1BeWrapper : public BaseHash64Wrapper {
    private:
        [[nodiscard]] uint64_t Hash(std::string_view str) const override;
    };


    class [[maybe_unused]] T1HA2AtonceWrapper : public BaseHash64Wrapper {
    private:
        [[nodiscard]] uint64_t Hash(std::string_view str) const override;
    };

//------------ xxHashes -----------

    class [[maybe_unused]] xxHash32Wrapper : public BaseHash32Wrapper {
    private:
        [[nodiscard]] uint32_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] xxHash64Wrapper : public BaseHash64Wrapper {
    private:
        [[nodiscard]] uint64_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] XXH3_64BitsWrapper : public BaseHash64Wrapper {
    private:
        [[nodiscard]] uint64_t Hash(std::string_view str) const override;
    };

    class [[maybe_unused]] XXH3_64BitsWithSeedWrapper : public BaseHash64Wrapper {
    private:
        [[nodiscard]] uint64_t Hash(std::string_view str) const override;
    };
}

#endif //THESIS_WORK_HASH_WRAPPERS_H
