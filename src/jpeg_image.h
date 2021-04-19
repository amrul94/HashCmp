#pragma once
#include "img.h"

#include <filesystem>

namespace img {
    using Path = std::filesystem::path;

    bool SaveJPEG(const Path& file, const Image& image);
    Image LoadJPEG(const Path& file);

} // of namespace img