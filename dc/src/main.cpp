#include <OgrePlatform.h>

// we needed to add this to solve a NACL compiler bug
#if (OGRE_PLATFORM == OGRE_PLATFORM_NACL) 
	#include "ppapi/utility/completion_callback_factory.h"
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	#define WIN32_LEAN_AND_MEAN
	#include "windows.h"
	#include "OgreString.h"
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE
	#include "platform/dc_OSX.hpp"
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
	#include "platform/dc_iOS.hpp"
#elif OGRE_PLATFORM == OGRE_PLATFORM_NACL
	#include "platform/dc_NaCl.hpp"
#elif OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
	#include "platform/dc_Android.hpp"

	SampleBrowser* OgreAndroidBridge::mBrowser = NULL;
	AndroidInputInjector* OgreAndroidBridge::mInputInjector = NULL;
	AndroidMultiTouch* OgreAndroidBridge::mTouch = NULL;
	AndroidKeyboard* OgreAndroidBridge::mKeyboard = NULL;
	Ogre::RenderWindow* OgreAndroidBridge::mRenderWnd = NULL;
	Ogre::Root* OgreAndroidBridge::mRoot = NULL;
	bool OgreAndroidBridge::mInit = false;
	StaticPluginLoader* OgreAndroidBridge::mStaticPluginLoader = NULL;
#endif

#include "dc_application.hpp"
#include <iostream>

#if OGRE_PLATFORM != OGRE_PLATFORM_NACL

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR cmdLine, INT) {
		int argc = __argc;
		char** argv = __argv;
#elif OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
	void android_main(struct android_app* state) {
#else
	int main(int argc, char *argv[]) {
#endif
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
		NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
		int retVal = UIApplicationMain(argc, argv, @"UIApplication", @"AppDelegate");
		[pool release];
		return retVal;
#elif (OGRE_PLATFORM == OGRE_PLATFORM_APPLE) && __LP64__
		NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];

		mAppDelegate = [[AppDelegate alloc] init];
		[[NSApplication sharedApplication] setDelegate:mAppDelegate];
		int retVal = NSApplicationMain(argc, (const char **)argv);

		[pool release];

		return retVal;
#elif OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
		// Make sure glue isn't stripped.
		app_dummy();

		OgreAndroidBridge::init(state);
		OgreAndroidBridge::go(state);
#else

		try
		{
			DC::dc_application dc;
			dc.go();
		}
		catch (Ogre::Exception& e)
		{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			MessageBoxA(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_ICONERROR | MB_TASKMODAL);
#else
			std::cerr << "An exception has occurred: " << e.getFullDescription().c_str() << std::endl;
#endif
		}
		return 0;
#endif
	}
#endif 
