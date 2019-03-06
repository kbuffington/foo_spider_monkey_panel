#pragma once

#include <utils/gdi_helpers.h>

#include <optional>

namespace smp::error
{

const char* GdiErrorCodeToText( Gdiplus::Status errorCode );
void CheckGdi( Gdiplus::Status gdiStatus, std::string_view functionName );

template <typename T, typename T_Parent = T>
void CheckGdiPlusObject( const std::unique_ptr<T>& obj, const T_Parent* pParentObj = nullptr ) noexcept( false )
{
    // GetLastStatus() resets status, so it needs to be saved here
    const auto status = [&obj, pParentObj]() -> std::optional<Gdiplus::Status>
    {
        if ( obj )
        {
            return obj->GetLastStatus();
        }

        if ( pParentObj )
        {
            return pParentObj->GetLastStatus();
        }

        return std::nullopt;
    }();

    if ( obj && status && Gdiplus::Status::Ok == status.value() )
    {
        return;
    }

    if ( status )
    {
        throw SmpException( fmt::format( "Failed to create GdiPlus object ({:#X}): {}", status.value(), GdiErrorCodeToText( status.value() ) ) );
    }
    else
    {
        throw SmpException( "Failed to create GdiPlus object" );
    }
}

} // namespace smp::error
