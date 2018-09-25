#include <stdafx.h>
#include "native_to_js_invoker.h"

#pragma warning( push )  
#pragma warning( disable : 4100 ) // unused variable
#pragma warning( disable : 4251 ) // dll interface warning
#pragma warning( disable : 4324 ) // structure was padded due to alignment specifier
#pragma warning( disable : 4996 ) // C++17 deprecation warning
#   include <js/Conversions.h>
#pragma warning( pop ) 


namespace mozjs
{
bool InvokeJsCallback_Impl( JSContext* cx,
                            JS::HandleObject globalObject,
                            JS::HandleValue functionValue,
                            const JS::HandleValueArray& args,
                            JS::MutableHandleValue rval )
{      
    JS::RootedFunction func( cx, JS_ValueToFunction( cx, functionValue ) );
    if ( !func )
    {// Reports
        return false;
    }

    return JS::Call( cx, globalObject, func, args, rval ); // reports    
}

}
