#ifndef THESIS_WORK_HASHES_H
#define THESIS_WORK_HASHES_H

#include <memory>

#include "hash_wrappers.h"

// HFL = Hash function library
namespace hfl {

    // Класс обертка над BaseHashWrapper
    template<UnsignedIntegral UintT>
    class Hash {
        using BaseHashWrapper = wrappers::detail::BaseHashWrapper<UintT>;
    public:
        Hash(std::string name, std::unique_ptr<BaseHashWrapper>&& function_pointer)
                : hash_name_(std::move(name))
                , hash_impl_(std::move(function_pointer)) {
        }

        // Метод для хеширования строки
        uint64_t operator()(const std::string& str) const {
            UintT hash = hash_impl_->template Hash(str);
            return NumberToUint64(hash);
        }

        // Метод для хеширования бинарного файла
        uint64_t operator()(std::ifstream& file) const {
            UintT hash = hash_impl_->template Hash(file);
            return NumberToUint64(hash);
        }

        // Метод для хеширования чисел
        uint64_t operator()(std::integral auto number) const {
            UintT hash = hash_impl_->template Hash(number);
            return NumberToUint64(hash);
        }

        [[nodiscard]] std::string GetName() const {
            return hash_name_;
        }

    private:
        // Метод, для приведения UintT к uint64_t
        [[nodiscard]] uint64_t NumberToUint64(UintT number) const {
            return static_cast<uint64_t>(number);
        }

        // Название хеш функции
        std::string hash_name_{};
        // Указатель на хеш функцию
        std::unique_ptr<BaseHashWrapper> hash_impl_{};
    };

    //----------- BuildHashes ----------
    // Функции для конструирования хеш функций
    std::vector<Hash<uint16_t>> Build16bitsHashes();
    std::vector<Hash<uint24_t>> Build24bitsHashes();
    std::vector<Hash<uint32_t>> Build32bitsHashes();
    std::vector<Hash<uint48_t>> Build48bitsHashes();
    std::vector<Hash<uint64_t>> Build64bitsHashes();

    template<class> inline constexpr bool always_false_v = false;

    // Универсальная функция для конструирования хеш функций
    // заданной битности
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
}

#endif //THESIS_WORK_HASHES_H