#include <XE/XE.h>

#if PLATFORM_OS & OS_ANDROID

#include <android/log.h>
#include <android/asset_manager.h>
#include <android/native_activity.h>
#include <android_native_app_glue.h>


void on_android_cmd( struct android_app * app, XE::int32 cmd )
{
	switch ( cmd )
    {
    case APP_CMD_INIT_WINDOW:
    {
		
    }
    break;
    case APP_CMD_GAINED_FOCUS:
    {
    }
    break;
    case APP_CMD_LOST_FOCUS:
    {
    }
    break;
    }
}

void on_android_input( struct android_app * app, AInputEvent * event )
{
    int32_t type = AInputEvent_getType( event );
    if (type == AINPUT_EVENT_TYPE_KEY)
	{
        int32_t key = AKeyEvent_getKeyCode( event );
		switch ( key )
        {
        case AKEYCODE_Z:
            break;
        default:
            break;
        }
	}
}

void android_main( struct android_app * app )
{
	app->onAppCmd = on_android_cmd;
	app->onInputEvent = on_android_input;

	XE::CoreFramework framework;

	framework.Exec( XE::MakeShared< XE::Window >( XE::HandleCast< XE::Window >( reinterpret_cast<XE::uint64>( app->window ) ), app ) );
}

#endif