#ifndef THESIS_WORK_HASHES_H
#define THESIS_WORK_HASHES_H

#include <concepts>
#include <utility>
#include <variant>

#include "hash_wrappers.h"

// HFL = Hash function library
namespace hfl {
    template<UnsignedIntegral UintT>
    class Hash {
        using BaseHashWrapper = wrappers::detail::BaseHashWrapper<UintT>;
    public:
        Hash(std::string name, std::unique_ptr<BaseHashWrapper>&& function_pointer)
                : name_(std::move(name))
                , hasher_impl_(std::move(function_pointer)) {
        }

        uint64_t operator()(const std::string& str) const {
            UintT hash = hasher_impl_->template Hash(str);
            return NumberToUint64(hash);
        }

        uint64_t operator()(std::ifstream& file) const {
            UintT hash = hasher_impl_->template Hash(file);
            return NumberToUint64(hash);
        }

        template<std::integral Number>
        uint64_t operator()(Number number) const {
            UintT hash = hasher_impl_->template Hash(number);
            return NumberToUint64(hash);
        }

        [[nodiscard]] std::string GetName() const {
            return name_;
        }

    private:
        uint64_t NumberToUint64(UintT number) const {
            return static_cast<uint64_t>(number);
        }

        std::string name_{};
        std::unique_ptr<BaseHashWrapper> hasher_impl_{};
    };

    //----------- BuildHashes ----------
    std::vector<Hash<uint16_t>> Build16bitsHashes();
    std::vector<Hash<uint24_t>> Build24bitsHashes();
    std::vector<Hash<uint32_t>> Build32bitsHashes();
    std::vector<Hash<uint48_t>> Build48bitsHashes();
    std::vector<Hash<uint64_t>> Build64bitsHashes();

    template<class> inline constexpr bool always_false_v = false;

    template<UnsignedIntegral UintT>
    std::vector<Hash<UintT>> BuildHashes() {
        if constexpr (std::is_same_v<UintT, uint16_t>)
            return Build16bitsHashes();
        else if constexpr (std::is_same_v<UintT, uint24_t>)
            return Build24bitsHashes();
        else if constexpr (std::is_same_v<UintT, uint32_t>)
            return Build32bitsHashes();
        else if constexpr (std::is_same_v<UintT, uint48_t>)
            return Build48bitsHashes();
        else if constexpr (std::is_same_v<UintT, uint64_t>)
            return Build64bitsHashes();
        else
            static_assert(always_false_v<UintT>, "non-exhaustive visitor!");
    }

    //uint64_t MaskShift(uint64_t src, uint16_t mask_bits, uint16_t shift = 0);
}

#endif //THESIS_WORK_HASHES_H