

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Image.h"

#include <QkTraits/TypeTraits.h>


namespace Mct {

    class TextureUtils {
    public:
        template <typename T>
        requires QkT::IsOneOf_V<T, uint8_t, uint16_t, float>
        [[nodiscard]] static Image GenerateGenericMissingPattern(int width, int height, int channels) {
            MCT_ASSERT(width != 0 && height != 0 && channels != 0);

            T maxVal;
            if constexpr (std::is_same_v<T, float>) maxVal = 1.0f;
            else maxVal = std::numeric_limits<T>::max(); // 255 or 65535

            std::vector<T> buffer;
            size_t totalElements = static_cast<size_t>(width) * static_cast<size_t>(height) * static_cast<size_t>(channels);
            buffer.resize(totalElements);

            // Fill with Solid Magenta (R=Max, G=0, B=Max, A=Max)
            for (size_t i = 0; i < totalElements; i += channels) {
                if (channels > 0) buffer[i + 0] = maxVal; // R
                if (channels > 1) buffer[i + 1] = 0;      // G
                if (channels > 2) buffer[i + 2] = maxVal; // B
                if (channels > 3) buffer[i + 3] = maxVal; // A
            }

            return Image::Create(std::move(buffer), width, height, channels);
        }

        [[nodiscard]] static const Image& GetMissingTexture() {
            static const Image img = GenerateGenericMissingPattern<uint8_t>(2, 2, 4);
            return img;
        }

        [[nodiscard]] static Image GetMissingTexture(int width, int height, int channels, ImgDataType type) {
            switch (type) {
                case ImgDataType::U8:  return GenerateGenericMissingPattern<uint8_t>(width, height, channels);
                case ImgDataType::U16: return GenerateGenericMissingPattern<uint16_t>(width, height, channels);
                case ImgDataType::F32: return GenerateGenericMissingPattern<float>(width, height, channels);
            }

            MCT_ERROR("TextureUtils: Unknown data type for fallback generation!");
            return GenerateGenericMissingPattern<uint8_t>(width, height, channels);
        }
    };

}
