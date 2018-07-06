#pragma once

#include <optional>
#include <string>

namespace mozjs::image
{

struct AsyncImageTaskResult
{
    std::unique_ptr<Gdiplus::Bitmap> bitmap;
    pfc::string8_fast imagePath;
};

uint32_t LoadImageAsync( HWND hWnd, const std::wstring& imagePath );

}
