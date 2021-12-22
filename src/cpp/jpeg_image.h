#ifndef THESIS_WORK_JPEG_IMAGE_H
#define THESIS_WORK_JPEG_IMAGE_H

#include "img.h"

#include <filesystem>

namespace img {
    using Path = std::filesystem::path;

    bool SaveJPEG(const Path& file, const Image& image);
    Image LoadJPEG(const Path& file);

} // of namespace img

#endif //THESIS_WORK_JPEG_IMAGE_H