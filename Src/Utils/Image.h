

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "NonCopyable.h"
#include "Traits.h"
#include "Assert.h"

#include <string>
#include <optional>
#include <variant>
#include <vector>


namespace Mct {

    enum class ImgDataType { 
        Invalid,
        U8, 
        U16, 
        F32
    };

    [[nodiscard]] constexpr uint32_t GetImgDataTypeSize(ImgDataType type) noexcept {
        switch (type) {
            case ImgDataType::Invalid:  return 0;
            case ImgDataType::U8:       return 1;
            case ImgDataType::U16:      return 2;
            case ImgDataType::F32:      return 4;
        }

        MCT_ASSERT("Invalid ImgDataType provided!");
        return 0;
    }

    using ImageData = std::variant<
        std::monostate, 
        uint8_t*, uint16_t*, float*,                                    // Owned by manual free() (stbi_load)
        std::vector<uint8_t>, std::vector<uint16_t>, std::vector<float> // Owned by RAII
    >;

    class Image : public NonCopyable {
        friend bool LoadImageInto(Image& img, const char* path, bool flip, int forceChannels) noexcept;

    public:
        [[nodiscard]] static std::optional<Image> Create(const char* filepath,
                                                         bool        flipVertically = true, 
                                                         int         forceChannels  = 0) noexcept;

        [[nodiscard]] static std::optional<Image> Create(const std::string& filepath,
                                                         bool               flipVertically = true, 
                                                         int                forceChannels  = 0) noexcept {
            return Create(filepath.c_str(), flipVertically, forceChannels);
        }

        template<typename T>
        requires IsOneOf_V<std::decay_t<T>, std::vector<uint8_t>, std::vector<uint16_t>, std::vector<float>>
        [[nodiscard]] static Image Create(T&& data, int width, int height, int channels) {
            MCT_ASSERT(data.size() == static_cast<size_t>(width) * static_cast<size_t>(height) * static_cast<size_t>(channels),
                       "Vector size does not match image dimensions!");

            using ScalarType = typename std::decay_t<T>::value_type;

            ImgDataType dataType = ImgDataType::Invalid;

            if constexpr      (std::is_same_v<ScalarType,  uint8_t>) dataType = ImgDataType::U8;
            else if constexpr (std::is_same_v<ScalarType, uint16_t>) dataType = ImgDataType::U16;
            else                                                     dataType = ImgDataType::F32;

            return Image(std::forward<T>(data), width, height, channels, dataType);
        }

    public:
        ~Image();

        Image(Image&& other)            noexcept;
        Image& operator=(Image&& other) noexcept;

        [[nodiscard]] explicit operator bool() const noexcept {
            return !std::holds_alternative<std::monostate>(m_Data);
        }

        [[nodiscard]] int         GetWidth()    const noexcept { return m_Width;    }
        [[nodiscard]] int         GetHeight()   const noexcept { return m_Height;   }
        [[nodiscard]] int         GetChannels() const noexcept { return m_Channels; }
        [[nodiscard]] ImgDataType GetDataType() const noexcept { return m_DataType; }

        [[nodiscard]] const ImageData& GetVariantData() const noexcept { return m_Data; }

        [[nodiscard]] const void* GetData() const noexcept;

    private:
        Image() noexcept = default;

        Image(ImageData data, int width, int height, int channels, ImgDataType dataType) noexcept :
                m_Data     ( std::move(data) ),
                m_Width    ( width           ),
                m_Height   ( height          ),
                m_Channels ( channels        ),
                m_DataType ( dataType        )
        {}

        void FreeImage() noexcept;

    private:
        ImageData m_Data = std::monostate{};

        int m_Width    = 0;
        int m_Height   = 0;
        int m_Channels = 0;

        ImgDataType m_DataType = ImgDataType::Invalid;
    };

}
