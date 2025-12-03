

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "Utils/Assert.h"
#include "Image.h"

#include <stb/stb_image.h>

#include <utility>


namespace Mct {

    [[nodiscard]] static inline bool LoadImageInto(Image& img, const char* path, bool flip, int forceChannels) noexcept {
        if (img) {
            img.FreeImage();
        }

        stbi_set_flip_vertically_on_load(static_cast<int>(flip));

        const bool isHDR   = stbi_is_hdr(path)    != 0;
        const bool is16bit = stbi_is_16_bit(path) != 0;

        int width = 0, height = 0, srcChannel = 0;

        if (isHDR) {
            float* p = stbi_loadf(path, &width, &height, &srcChannel, forceChannels);
            if (!p) return false;

            img.m_Data   = p;
            img.m_DataType = ImgDataType::F32;
        }
        else if (is16bit) {
            stbi_us* p = stbi_load_16(path, &width, &height, &srcChannel, forceChannels);
            if (!p) return false;

            img.m_Data   = reinterpret_cast<uint16_t*>(p);
            img.m_DataType = ImgDataType::U16;
        }
        else {
            stbi_uc* p = stbi_load(path, &width, &height, &srcChannel, forceChannels);
            if (!p) return false;

            img.m_Data   = static_cast<uint8_t*>(p);
            img.m_DataType = ImgDataType::U8;
        }

        img.m_Width    = width;
        img.m_Height   = height;
        img.m_Channels = (forceChannels > 0) ? forceChannels : srcChannel;

        return true;
    }

    std::optional<Image> Image::Create(const char* filepath, bool flipVertically, int forceChannels) noexcept {
        Image tmp;

        if (!LoadImageInto(tmp, filepath, flipVertically, forceChannels)) {
            return std::nullopt;
        }

        return std::optional<Image>(std::in_place, std::move(tmp));
    }

    Image::~Image() {
        FreeImage();
    }

    Image::Image(Image&& other) noexcept : 
            m_Data     ( std::exchange(other.m_Data,     std::monostate{}     ) ),
            m_Width    ( std::exchange(other.m_Width,    0                    ) ),
            m_Height   ( std::exchange(other.m_Height,   0                    ) ),
            m_Channels ( std::exchange(other.m_Channels, 0                    ) ),
            m_DataType ( std::exchange(other.m_DataType, ImgDataType::Invalid ) )
    {}

    Image& Image::operator=(Image&& other) noexcept {
        if (this != &other) {
            FreeImage();

            m_Data     = std::exchange(other.m_Data,     std::monostate{}     );
            m_Width    = std::exchange(other.m_Width,    0                    );
            m_Height   = std::exchange(other.m_Height,   0                    );
            m_Channels = std::exchange(other.m_Channels, 0                    );
            m_DataType = std::exchange(other.m_DataType, ImgDataType::Invalid );
        }

        return *this;
    }

    const void* Image::GetData() const noexcept {
        return std::visit([](auto&& data) noexcept -> const void* {
            using T = std::decay_t<decltype(data)>;

            if constexpr (std::is_same_v<T, std::monostate>) {
                return nullptr;
            }
            else if constexpr (std::is_pointer_v<T>) {
                return static_cast<const void*>(data);
            }
            else {
                return static_cast<const void*>(data.data());
            }
        }, m_Data);
    }

    void Image::FreeImage() noexcept {
        std::visit([](auto&& data) noexcept {
			using T = std::decay_t<decltype(data)>;

			if constexpr (!std::is_same_v<T, std::monostate> && std::is_pointer_v<T>) {
                if (data != nullptr) stbi_image_free(static_cast<void*>(data));
			}
		}, m_Data);

        m_Data     = std::monostate{};
        m_Width    = 0;
        m_Height   = 0;
        m_Channels = 0;
        m_DataType = ImgDataType::Invalid;
    }

}
