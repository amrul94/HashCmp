#ifndef THESIS_WORK_HASHES_H
#define THESIS_WORK_HASHES_H

#include <concepts>
#include <utility>

#include "hash_wrappers.h"

// HFL = Hash function library
namespace hfl {
    namespace detail {
        template<typename UintT>
        class Hash {
        public:
            Hash(std::string name, std::unique_ptr<BaseHashWrapper<UintT>>&& function_pointer)
                    : name_(std::move(name))
                    , hasher_impl_(std::move(function_pointer)) {
            }

            uint64_t operator()(const std::string& str) const {
                UintT hash = hasher_impl_->template operator()(str);
                return NumberToUint64(hash);
            }

            uint64_t operator()(std::ifstream& file) const {
                UintT hash = hasher_impl_->template operator()(file);
                return NumberToUint64(hash);
            }

            template<std::integral Number>
            uint64_t operator()(Number number) const {
                UintT hash = hasher_impl_->template operator()(number);
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
            std::unique_ptr<BaseHashWrapper<UintT>> hasher_impl_{};
        };
    }

    using Hash16 = detail::Hash<uint16_t>;
    using Hash24 = detail::Hash<uint24_t>;
    using Hash32 = detail::Hash<uint32_t>;
    using Hash48 = detail::Hash<uint48_t>;
    using Hash64 = detail::Hash<uint64_t>;

    //----------- BuildHashes ----------

    std::vector<Hash16> Build16bitsHashes();
    std::vector<Hash24> Build24bitsHashes();
    std::vector<Hash32> Build32bitsHashes();
    std::vector<Hash48> Build48bitsHashes();
    std::vector<Hash64> Build64bitsHashes();

    //uint64_t MaskShift(uint64_t src, uint16_t mask_bits, uint16_t shift = 0);
}

#endif //THESIS_WORK_HASHES_H