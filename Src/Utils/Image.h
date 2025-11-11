

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Utils/NonCopyable.h"

#include <string>
#include <cstdint>


namespace Mct {

    class Image : public NonCopyable {
    public:
        Image(const std::string& filepath, bool flipVertically = true, int forceChannels = 4);
        ~Image();

        Image(Image&& other)            noexcept;
        Image& operator=(Image&& other) noexcept;

        bool IsValid()                 const noexcept { return m_Data != nullptr; }
        int  GetWidth()                const noexcept { return m_Width;           }
        int  GetHeight()               const noexcept { return m_Height;          }
        int  GetChannels()             const noexcept { return m_Channels;        }
        const unsigned char* GetData() const noexcept { return m_Data;            }

    private:
        unsigned char* m_Data = nullptr;
        int m_Width           = 0;
        int m_Height          = 0;
        int m_Channels        = 0;
    };

}
