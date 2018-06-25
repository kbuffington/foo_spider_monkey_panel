#include <stdafx.h>

#include "gdi_graphics.h"

#include <js_engine/js_value_converter.h>
#include <js_engine/js_native_invoker.h>
#include <js_engine/js_error_reporter.h>
#include <js_objects/gdi_font.h>
#include <js_objects/gdi_error.h>
#include <js_utils/js_utils.h>

#include <helpers.h>


namespace
{

using namespace mozjs;

JSClassOps jsOps = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    JsFinalizeOp<JsGdiUtils>,
    nullptr,
    nullptr,
    nullptr,
    nullptr
};

JSClass jsClass = {
    "GdiGraphics",
    JSCLASS_HAS_PRIVATE | JSCLASS_FOREGROUND_FINALIZE,
    &jsOps
};

MJS_DEFINE_JS_TO_NATIVE_FN( JsGdiGraphics, CalcTextHeight )
MJS_DEFINE_JS_TO_NATIVE_FN( JsGdiGraphics, CalcTextWidth )
MJS_DEFINE_JS_TO_NATIVE_FN( JsGdiGraphics, DrawEllipse )
MJS_DEFINE_JS_TO_NATIVE_FN( JsGdiGraphics, DrawLine )
MJS_DEFINE_JS_TO_NATIVE_FN( JsGdiGraphics, DrawPolygon )
MJS_DEFINE_JS_TO_NATIVE_FN( JsGdiGraphics, DrawRect )
MJS_DEFINE_JS_TO_NATIVE_FN( JsGdiGraphics, DrawRoundRect )
MJS_DEFINE_JS_TO_NATIVE_FN_WITH_OPT( JsGdiGraphics, DrawString, DrawStringWithOpt, 1 )
MJS_DEFINE_JS_TO_NATIVE_FN( JsGdiGraphics, FillEllipse )
MJS_DEFINE_JS_TO_NATIVE_FN( JsGdiGraphics, FillGradRect )
MJS_DEFINE_JS_TO_NATIVE_FN( JsGdiGraphics, FillPolygon )
MJS_DEFINE_JS_TO_NATIVE_FN( JsGdiGraphics, FillRoundRect )
MJS_DEFINE_JS_TO_NATIVE_FN( JsGdiGraphics, FillSolidRect )
MJS_DEFINE_JS_TO_NATIVE_FN_WITH_OPT( JsGdiGraphics, SetInterpolationMode, SetInterpolationModeWithOpt, 1 )
MJS_DEFINE_JS_TO_NATIVE_FN_WITH_OPT( JsGdiGraphics, SetSmoothingMode, SetSmoothingModeWithOpt, 1 )
MJS_DEFINE_JS_TO_NATIVE_FN_WITH_OPT( JsGdiGraphics, SetTextRenderingHint, SetTextRenderingHintWithOpt, 1 )

const JSFunctionSpec jsFunctions[] = {
    JS_FN( "CalcTextHeight", CalcTextHeight, 2, 0 ),
    JS_FN( "CalcTextWidth", CalcTextWidth, 2, 0 ),
    JS_FN( "DrawEllipse", DrawEllipse, 6, 0 ),
    JS_FN( "DrawLine", DrawLine, 6, 0 ),
    JS_FN( "DrawPolygon", DrawPolygon, 3, 0 ),
    JS_FN( "DrawRect", DrawRect, 6, 0 ),
    JS_FN( "DrawRoundRect", DrawRoundRect, 8, 0 ),
    JS_FN( "DrawString", DrawString, 8, 0 ),
    JS_FN( "FillEllipse", FillEllipse, 5, 0 ),
    JS_FN( "FillGradRect", FillGradRect, 8, 0 ),
    JS_FN( "FillPolygon", DrawPolygon, 3, 0 ),
    JS_FN( "FillRoundRect", FillRoundRect, 7, 0 ),
    JS_FN( "FillSolidRect", FillSolidRect, 5, 0 ),
    JS_FN( "SetInterpolationMode", SetInterpolationMode, 1, 0 ),
    JS_FN( "SetSmoothingMode", SetSmoothingMode, 1, 0 ),
    JS_FN( "SetTextRenderingHint", SetTextRenderingHint, 1, 0 ),
    JS_FS_END
};

}

namespace mozjs
{


JsGdiGraphics::JsGdiGraphics( JSContext* cx )
    : pJsCtx_( cx )
    , pGdi_( nullptr )
{
}


JsGdiGraphics::~JsGdiGraphics()
{
}

JSObject* JsGdiGraphics::Create( JSContext* cx )
{
    JS::RootedObject jsObj( cx,
                            JS_NewObject( cx, &jsClass ) );
    if ( !jsObj )
    {
        return nullptr;
    }

    if ( !JS_DefineFunctions( cx, jsObj, jsFunctions ) )
    {
        return nullptr;
    }

    JS_SetPrivate( jsObj, new JsGdiGraphics( cx ) );

    return jsObj;
}

const JSClass& JsGdiGraphics::GetClass()
{
    return jsClass;
}

Gdiplus::Graphics* JsGdiGraphics::GetGraphicsObject() const
{
    return pGdi_;
}

void JsGdiGraphics::SetGraphicsObject( Gdiplus::Graphics* graphics )
{
    pGdi_ = graphics;
}

std::optional<uint32_t>
JsGdiGraphics::CalcTextHeight( std::wstring str, JS::HandleValue font )
{
    if ( !pGdi_ )
    {
        JS_ReportErrorASCII( pJsCtx_, "Internal error: Gdiplus::Graphics object is null" );
        return std::nullopt;
    }

    JS::RootedObject jsObject( pJsCtx_, GetJsObjectFromValue( pJsCtx_, font ) );
    if ( !jsObject )
    {
        JS_ReportErrorASCII( pJsCtx_, "font argument is not a JS object" );
        return std::nullopt;
    }

    JsGdiFont* pJsFont = GetNativeFromJsObject<JsGdiFont>( pJsCtx_, jsObject );
    if ( !pJsFont )
    {
        JS_ReportErrorASCII( pJsCtx_, "font argument is not a GdiFont object" );
        return std::nullopt;
    }

    HFONT hFont = pJsFont->HFont();
    HDC dc = pGdi_->GetHDC();
    HFONT oldfont = SelectFont( dc, hFont );

    uint32_t textH = helpers::get_text_height( dc, str.c_str(), str.length() );

    SelectFont( dc, oldfont );
    pGdi_->ReleaseHDC( dc );

    return textH;
}

std::optional<uint32_t>
JsGdiGraphics::CalcTextWidth( std::wstring str, JS::HandleValue font )
{
    if ( !pGdi_ )
    {
        JS_ReportErrorASCII( pJsCtx_, "Internal error: Gdiplus::Graphics object is null" );
        return std::nullopt;
    }

    JS::RootedObject jsObject( pJsCtx_, GetJsObjectFromValue( pJsCtx_, font ) );
    if ( !jsObject )
    {
        JS_ReportErrorASCII( pJsCtx_, "font argument is not a JS object" );
        return std::nullopt;
    }

    JsGdiFont* pJsFont = GetNativeFromJsObject<JsGdiFont>( pJsCtx_, jsObject );
    if ( !pJsFont )
    {
        JS_ReportErrorASCII( pJsCtx_, "font argument is not a GdiFont object" );
        return std::nullopt;
    }

    HFONT hFont = pJsFont->HFont();
    HDC dc = pGdi_->GetHDC();
    HFONT oldfont = SelectFont( dc, hFont );

    uint32_t textW = helpers::get_text_width( dc, str.c_str(), str.length() );

    SelectFont( dc, oldfont );
    pGdi_->ReleaseHDC( dc );

    return textW;
}

std::optional<std::nullptr_t>
JsGdiGraphics::DrawEllipse( float x, float y, float w, float h, float line_width, uint32_t colour )
{
    if ( !pGdi_ )
    {
        JS_ReportErrorASCII( pJsCtx_, "Internal error: Gdiplus::Graphics object is null" );
        return std::nullopt;
    }

    Gdiplus::Pen pen( colour, line_width );
    Gdiplus::Status gdiRet = pGdi_->DrawEllipse( &pen, x, y, w, h );
    IF_GDI_FAILED_RETURN_WITH_REPORT( pJsCtx_, gdiRet, std::nullopt, DrawEllipse );

    return nullptr;
}

std::optional<std::nullptr_t>
JsGdiGraphics::DrawLine( float x1, float y1, float x2, float y2, float line_width, uint32_t colour )
{
    if ( !pGdi_ )
    {
        JS_ReportErrorASCII( pJsCtx_, "Internal error: Gdiplus::Graphics object is null" );
        return std::nullopt;
    }

    Gdiplus::Pen pen( colour, line_width );
    Gdiplus::Status gdiRet = pGdi_->DrawLine( &pen, x1, y1, x2, y2 );
    IF_GDI_FAILED_RETURN_WITH_REPORT( pJsCtx_, gdiRet, std::nullopt, DrawLine );

    return nullptr;
}

std::optional<std::nullptr_t> JsGdiGraphics::DrawPolygon( uint32_t colour, float line_width, JS::HandleValue points )
{
    if ( !pGdi_ )
    {
        JS_ReportErrorASCII( pJsCtx_, "Internal error: Gdiplus::Graphics object is null" );
        return std::nullopt;
    }

    std::vector<Gdiplus::PointF> gdiPoints;
    if ( !ParsePoints( points, gdiPoints ) )
    {// Report in ParsePoints
        return std::nullopt;
    }

    Gdiplus::Pen pen( colour, line_width );
    Gdiplus::Status gdiRet = pGdi_->DrawPolygon( &pen, gdiPoints.data(), gdiPoints.size() );
    IF_GDI_FAILED_RETURN_WITH_REPORT( pJsCtx_, gdiRet, std::nullopt, DrawPolygon );

    return nullptr;
}

std::optional<std::nullptr_t>
JsGdiGraphics::DrawRect( float x, float y, float w, float h, float line_width, uint32_t colour )
{
    if ( !pGdi_ )
    {
        JS_ReportErrorASCII( pJsCtx_, "Internal error: Gdiplus::Graphics object is null" );
        return std::nullopt;
    }

    Gdiplus::Pen pen( colour, line_width );
    Gdiplus::Status gdiRet = pGdi_->DrawRectangle( &pen, x, y, w, h );
    IF_GDI_FAILED_RETURN_WITH_REPORT( pJsCtx_, gdiRet, std::nullopt, DrawRectangle );

    return nullptr;
}

std::optional<std::nullptr_t>
JsGdiGraphics::DrawRoundRect( float x, float y, float w, float h, float arc_width, float arc_height, float line_width, uint32_t colour )
{
    if ( !pGdi_ )
    {
        JS_ReportErrorASCII( pJsCtx_, "Internal error: Gdiplus::Graphics object is null" );
        return std::nullopt;
    }

    if ( 2 * arc_width > w || 2 * arc_height > h )
    {
        JS_ReportErrorASCII( pJsCtx_, "Arc argument has invalid value" );
        return std::nullopt;
    }

    Gdiplus::Pen pen( colour, line_width );
    Gdiplus::GraphicsPath gp;
    Gdiplus::RectF rect( x, y, w, h );
    Gdiplus::Status gdiRet = ( Gdiplus::Status )GetRoundRectPath( gp, rect, arc_width, arc_height );
    if ( gdiRet > 0 )
    {// Report in GetRoundRectPath
        return std::nullopt;
    }

    gdiRet = pen.SetStartCap( Gdiplus::LineCapRound );
    IF_GDI_FAILED_RETURN_WITH_REPORT( pJsCtx_, gdiRet, std::nullopt, SetStartCap );

    gdiRet = pen.SetEndCap( Gdiplus::LineCapRound );
    IF_GDI_FAILED_RETURN_WITH_REPORT( pJsCtx_, gdiRet, std::nullopt, SetEndCap );

    gdiRet = pGdi_->DrawPath( &pen, &gp );
    IF_GDI_FAILED_RETURN_WITH_REPORT( pJsCtx_, gdiRet, std::nullopt, DrawPath );

    return nullptr;
}

std::optional<std::nullptr_t>
JsGdiGraphics::DrawString( std::wstring str, JS::HandleValue font, uint32_t colour, float x, float y, float w, float h, uint32_t flags )
{
    if ( !pGdi_ )
    {
        JS_ReportErrorASCII( pJsCtx_, "Internal error: Gdiplus::Graphics object is null" );
        return std::nullopt;
    }

    JS::RootedObject jsObject( pJsCtx_, GetJsObjectFromValue( pJsCtx_, font ) );
    if ( !jsObject )
    {
        JS_ReportErrorASCII( pJsCtx_, "font argument is not a JS object" );
        return std::nullopt;
    }

    JsGdiFont* pJsFont = GetNativeFromJsObject<JsGdiFont>( pJsCtx_, jsObject );
    if ( !pJsFont )
    {
        JS_ReportErrorASCII( pJsCtx_, "font argument is not a GdiFont object" );
        return std::nullopt;
    }

    Gdiplus::Font* pGdiFont = pJsFont->GdiFont();
    if ( !pGdiFont )
    {
        JS_ReportErrorASCII( pJsCtx_, "Internal error: GdiFont is null" );
        return std::nullopt;
    }

    Gdiplus::SolidBrush br( colour );
    Gdiplus::StringFormat fmt( Gdiplus::StringFormat::GenericTypographic() );

    if ( flags != 0 )
    {
        Gdiplus::Status gdiRet = fmt.SetAlignment( ( Gdiplus::StringAlignment )( ( flags >> 28 ) & 0x3 ) ); //0xf0000000
        IF_GDI_FAILED_RETURN_WITH_REPORT( pJsCtx_, gdiRet, std::nullopt, SetAlignment );

        gdiRet = fmt.SetLineAlignment( ( Gdiplus::StringAlignment )( ( flags >> 24 ) & 0x3 ) ); //0x0f000000
        IF_GDI_FAILED_RETURN_WITH_REPORT( pJsCtx_, gdiRet, std::nullopt, SetLineAlignment );

        gdiRet = fmt.SetTrimming( ( Gdiplus::StringTrimming )( ( flags >> 20 ) & 0x7 ) ); //0x00f00000
        IF_GDI_FAILED_RETURN_WITH_REPORT( pJsCtx_, gdiRet, std::nullopt, SetTrimming );

        gdiRet = fmt.SetFormatFlags( ( Gdiplus::StringFormatFlags )( flags & 0x7FFF ) ); //0x0000ffff
        IF_GDI_FAILED_RETURN_WITH_REPORT( pJsCtx_, gdiRet, std::nullopt, SetFormatFlags );
    }

    Gdiplus::Status gdiRet = pGdi_->DrawString( str.c_str(), -1, pGdiFont, Gdiplus::RectF( x, y, w, h ), &fmt, &br );
    IF_GDI_FAILED_RETURN_WITH_REPORT( pJsCtx_, gdiRet, std::nullopt, DrawString );

    return nullptr;
}

std::optional<std::nullptr_t>
JsGdiGraphics::DrawStringWithOpt( size_t optArgCount, std::wstring str, JS::HandleValue font, uint32_t colour, float x, float y, float w, float h, uint32_t flags )
{
    if ( optArgCount > 1 )
    {
        JS_ReportErrorASCII( pJsCtx_, "Internal error: invalid number of optional arguments specified: %d", optArgCount );
        return std::nullopt;
    }

    if ( optArgCount == 1 )
    {
        return DrawString( str, font, colour, x, y, w, h, 0 );
    }

    return DrawString( str, font, colour, x, y, w, h, flags );
}

std::optional<std::nullptr_t>
JsGdiGraphics::FillEllipse( float x, float y, float w, float h, uint32_t colour )
{
    if ( !pGdi_ )
    {
        JS_ReportErrorASCII( pJsCtx_, "Internal error: Gdiplus::Graphics object is null" );
        return std::nullopt;
    }

    Gdiplus::SolidBrush br( colour );
    Gdiplus::Status gdiRet = pGdi_->FillEllipse( &br, x, y, w, h );
    IF_GDI_FAILED_RETURN_WITH_REPORT( pJsCtx_, gdiRet, std::nullopt, FillEllipse );

    return nullptr;
}

std::optional<std::nullptr_t>
JsGdiGraphics::FillGradRect( float x, float y, float w, float h, float angle, uint32_t colour1, uint32_t colour2, float focus )
{
    if ( !pGdi_ )
    {
        JS_ReportErrorASCII( pJsCtx_, "Internal error: Gdiplus::Graphics object is null" );
        return std::nullopt;
    }

    Gdiplus::RectF rect( x, y, w, h );
    Gdiplus::LinearGradientBrush brush( rect, colour1, colour2, angle, TRUE );
    Gdiplus::Status gdiRet = brush.SetBlendTriangularShape( focus );
    IF_GDI_FAILED_RETURN_WITH_REPORT( pJsCtx_, gdiRet, std::nullopt, SetBlendTriangularShape );

    gdiRet = pGdi_->FillRectangle( &brush, rect );
    IF_GDI_FAILED_RETURN_WITH_REPORT( pJsCtx_, gdiRet, std::nullopt, FillRectangle );

    return nullptr;
}

std::optional<std::nullptr_t>
JsGdiGraphics::FillPolygon( uint32_t colour, uint32_t fillmode, JS::HandleValue points )
{
    if ( !pGdi_ )
    {
        JS_ReportErrorASCII( pJsCtx_, "Internal error: Gdiplus::Graphics object is null" );
        return std::nullopt;
    }

    std::vector<Gdiplus::PointF> gdiPoints;
    if ( !ParsePoints( points, gdiPoints ) )
    {// Report in ParsePoints
        return std::nullopt;
    }

    Gdiplus::SolidBrush br( colour );
    Gdiplus::Status gdiRet = pGdi_->FillPolygon( &br, gdiPoints.data(), gdiPoints.size(), ( Gdiplus::FillMode )fillmode );
    IF_GDI_FAILED_RETURN_WITH_REPORT( pJsCtx_, gdiRet, std::nullopt, FillPolygon );

    return nullptr;
}

std::optional<std::nullptr_t>
JsGdiGraphics::FillRoundRect( float x, float y, float w, float h, float arc_width, float arc_height, uint32_t colour )
{
    if ( !pGdi_ )
    {
        JS_ReportErrorASCII( pJsCtx_, "Internal error: Gdiplus::Graphics object is null" );
        return std::nullopt;
    }

    if ( 2 * arc_width > w || 2 * arc_height > h )
    {
        JS_ReportErrorASCII( pJsCtx_, "Arc argument has invalid value" );
        return std::nullopt;
    }

    Gdiplus::SolidBrush br( colour );
    Gdiplus::GraphicsPath gp;
    Gdiplus::RectF rect( x, y, w, h );
    Gdiplus::Status gdiRet = ( Gdiplus::Status )GetRoundRectPath( gp, rect, arc_width, arc_height );
    if ( gdiRet > 0 )
    {// Report in GetRoundRectPath
        return std::nullopt;
    }

    gdiRet = pGdi_->FillPath( &br, &gp );
    IF_GDI_FAILED_RETURN_WITH_REPORT( pJsCtx_, gdiRet, std::nullopt, FillPath );

    return nullptr;
}

std::optional<std::nullptr_t>
JsGdiGraphics::FillSolidRect( float x, float y, float w, float h, uint32_t colour )
{
    if ( !pGdi_ )
    {
        JS_ReportErrorASCII( pJsCtx_, "Internal error: Gdiplus::Graphics object is null" );
        return std::nullopt;
    }

    Gdiplus::SolidBrush brush( colour );
    Gdiplus::Status gdiRet = pGdi_->FillRectangle( &brush, x, y, w, h );
    IF_GDI_FAILED_RETURN_WITH_REPORT( pJsCtx_, gdiRet, std::nullopt, FillRectangle );

    return nullptr;
}

std::optional<std::nullptr_t> JsGdiGraphics::SetInterpolationMode( uint32_t mode )
{
    if ( !pGdi_ )
    {
        JS_ReportErrorASCII( pJsCtx_, "Internal error: Gdiplus::Graphics object is null" );
        return std::nullopt;
    }

    Gdiplus::Status gdiRet = pGdi_->SetInterpolationMode( ( Gdiplus::InterpolationMode )mode );
    IF_GDI_FAILED_RETURN_WITH_REPORT( pJsCtx_, gdiRet, std::nullopt, SetInterpolationMode );

    return nullptr;
}

std::optional<std::nullptr_t> JsGdiGraphics::SetInterpolationModeWithOpt( size_t optArgCount, uint32_t mode )
{
    if ( optArgCount > 1 )
    {
        JS_ReportErrorASCII( pJsCtx_, "Internal error: invalid number of optional arguments specified: %d", optArgCount );
        return std::nullopt;
    }

    if ( optArgCount == 1 )
    {
        return SetInterpolationMode( 0 );
    }

    return SetInterpolationMode( mode );
}

std::optional<std::nullptr_t> JsGdiGraphics::SetSmoothingMode( uint32_t mode )
{
    if ( !pGdi_ )
    {
        JS_ReportErrorASCII( pJsCtx_, "Internal error: Gdiplus::Graphics object is null" );
        return std::nullopt;
    }

    Gdiplus::Status gdiRet = pGdi_->SetSmoothingMode( ( Gdiplus::SmoothingMode )mode );
    IF_GDI_FAILED_RETURN_WITH_REPORT( pJsCtx_, gdiRet, std::nullopt, SetSmoothingMode );

    return nullptr;
}

std::optional<std::nullptr_t> JsGdiGraphics::SetSmoothingModeWithOpt( size_t optArgCount, uint32_t mode )
{
    if ( optArgCount > 1 )
    {
        JS_ReportErrorASCII( pJsCtx_, "Internal error: invalid number of optional arguments specified: %d", optArgCount );
        return std::nullopt;
    }

    if ( optArgCount == 1 )
    {
        return SetSmoothingMode( 0 );
    }

    return SetSmoothingMode( mode );
}

std::optional<std::nullptr_t> JsGdiGraphics::SetTextRenderingHint( uint32_t mode )
{
    if ( !pGdi_ )
    {
        JS_ReportErrorASCII( pJsCtx_, "Internal error: Gdiplus::Graphics object is null" );
        return std::nullopt;
    }

    Gdiplus::Status gdiRet = pGdi_->SetTextRenderingHint( ( Gdiplus::TextRenderingHint )mode );
    IF_GDI_FAILED_RETURN_WITH_REPORT( pJsCtx_, gdiRet, std::nullopt, SetTextRenderingHint );

    return nullptr;
}

std::optional<std::nullptr_t> JsGdiGraphics::SetTextRenderingHintWithOpt( size_t optArgCount, uint32_t mode )
{
    if ( optArgCount > 1 )
    {
        JS_ReportErrorASCII( pJsCtx_, "Internal error: invalid number of optional arguments specified: %d", optArgCount );
        return std::nullopt;
    }

    if ( optArgCount == 1 )
    {
        return SetTextRenderingHint( 0 );
    }

    return SetTextRenderingHint( mode );
}

bool JsGdiGraphics::GetRoundRectPath( Gdiplus::GraphicsPath& gp, Gdiplus::RectF& rect, float arc_width, float arc_height )
{
    float arc_dia_w = arc_width * 2;
    float arc_dia_h = arc_height * 2;
    Gdiplus::RectF corner( rect.X, rect.Y, arc_dia_w, arc_dia_h );

    Gdiplus::Status gdiRet = gp.Reset();
    IF_GDI_FAILED_RETURN_WITH_REPORT( pJsCtx_, gdiRet, false, Reset );

    // top left
    gdiRet = gp.AddArc( corner, 180, 90 );
    IF_GDI_FAILED_RETURN_WITH_REPORT( pJsCtx_, gdiRet, false, AddArc );

    // top right
    corner.X += ( rect.Width - arc_dia_w );
    gdiRet = gp.AddArc( corner, 270, 90 );
    IF_GDI_FAILED_RETURN_WITH_REPORT( pJsCtx_, gdiRet, false, AddArc );

    // bottom right
    corner.Y += ( rect.Height - arc_dia_h );
    gdiRet = gp.AddArc( corner, 0, 90 );
    IF_GDI_FAILED_RETURN_WITH_REPORT( pJsCtx_, gdiRet, false, AddArc );

    // bottom left
    corner.X -= ( rect.Width - arc_dia_w );
    gdiRet = gp.AddArc( corner, 90, 90 );
    IF_GDI_FAILED_RETURN_WITH_REPORT( pJsCtx_, gdiRet, false, AddArc );

    gdiRet = gp.CloseFigure();
    IF_GDI_FAILED_RETURN_WITH_REPORT( pJsCtx_, gdiRet, false, CloseFigure );

    return true;
}

bool JsGdiGraphics::ParsePoints( JS::HandleValue jsValue, std::vector<Gdiplus::PointF> &gdiPoints )
{
    gdiPoints.clear();

    JS::RootedObject jsObject( pJsCtx_, GetJsObjectFromValue( pJsCtx_, jsValue ) );
    if ( !jsObject )
    {
        JS_ReportErrorASCII( pJsCtx_, "Points argument is not a JS object" );
        return false;
    }

    bool is;
    if ( !JS_IsArrayObject( pJsCtx_, jsObject, &is ) )
    {
        JS_ReportErrorASCII( pJsCtx_, "Points argument is an array" );
        return false;
    }

    uint32_t arraySize;
    if ( !JS_GetArrayLength( pJsCtx_, jsObject, &arraySize ) )
    {
        JS_ReportErrorASCII( pJsCtx_, "Failed to get points argument array length" );
        return false;
    }

    if ( arraySize % 2 > 0 )
    {
        JS_ReportErrorASCII( pJsCtx_, "Points count must be multiple of two" );
        return false;
    }

    JS::RootedValue arrayElement( pJsCtx_ );
    JS::RootedValue jsX( pJsCtx_ ), jsY( pJsCtx_ );
    for ( uint32_t i = 0; i < arraySize; ++i )
    {
        if ( !JS_GetElement( pJsCtx_, jsObject, i, &arrayElement ) )
        {
            JS_ReportErrorASCII( pJsCtx_, "Failed to get points[%d]", i );
            return false;
        }

        JS::RootedObject curElement( pJsCtx_, GetJsObjectFromValue( pJsCtx_, arrayElement ) );
        if ( !curElement )
        {
            JS_ReportErrorASCII( pJsCtx_, "points[%d] is not an object", i );
            return false;
        }

        if ( !JS_GetProperty( pJsCtx_, curElement, "x", &jsX ) )
        {
            JS_ReportErrorASCII( pJsCtx_, "Failed to get 'x' property of point" );
            return false;
        }
        if ( !JS_GetProperty( pJsCtx_, curElement, "y", &jsY ) )
        {
            JS_ReportErrorASCII( pJsCtx_, "Failed to get 'y' property of point" );
            return false;
        }

        if ( !JsToNative<float>::IsValid( pJsCtx_, jsX ) )
        {
            JS_ReportErrorASCII( pJsCtx_, "'x' property of point is of wrong type" );
            return false;
        }
        if ( !JsToNative<float>::IsValid( pJsCtx_, jsX ) )
        {
            JS_ReportErrorASCII( pJsCtx_, "'y' property of point is of wrong type" );
            return false;
        }

        gdiPoints.emplace_back( Gdiplus::PointF(
            JsToNative<float>::Convert( pJsCtx_, jsX ),
            JsToNative<float>::Convert( pJsCtx_, jsY ) )
        );
    }

    return true;
}

}