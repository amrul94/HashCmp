#ifndef THESIS_WORK_IMG_H
#define THESIS_WORK_IMG_H

#include <array>
#include <cassert>
#include <cstddef>
#include <vector>

namespace img {

    struct Size {
        int width;
        int height;
    };

    struct Color {
        static Color Black() {
            return {std::byte{0}, std::byte{0}, std::byte{0}, std::byte{255}};
        }

        std::byte r, g, b, a;
    };

    class Image {
    public:
        Image() = default;
        Image(int w, int h, Color fill);

        [[nodiscard]] Color GetPixel(int x, int y) const {
            return const_cast<Image*>(this)->GetPixel(x, y);
        }
        Color& GetPixel(int x, int y) {
            assert(x < GetWidth() && y < GetHeight() && x >= 0 && y >= 0);
            return GetLine(y)[x];
        }

        // геттер для заданной строки изображения
        Color* GetLine(int y);
        [[nodiscard]] const Color* GetLine(int y) const;

        [[nodiscard]] int GetWidth() const;
        [[nodiscard]] int GetHeight() const;
        [[nodiscard]] int GetStep() const;

        explicit operator bool() const {
            return GetWidth() > 0 && GetHeight() > 0;
        }

        bool operator!() const {
            return !operator bool();
        }

    private:
        int width_ = 0;
        int height_ = 0;
        int step_ = 0;

        std::vector<Color> pixels_;
    };

}  // namespace img

#endif // THESIS_WORK_IMG_H
