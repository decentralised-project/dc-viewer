#ifndef __DC_APPLICATION_H
#define __DC_APPLICATION_H

#include <Ogre.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE || OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
	#include "macUtils.h"
#endif

#define ENABLE_SHADERS_CACHE_SAVE 0
#define ENABLE_SHADERS_CACHE_LOAD 0

#if OGRE_PLATFORM == OGRE_PLATFORM_WINRT
	#include <sdkddkver.h>
	#if defined(_WIN32_WINNT) && _WIN32_WINNT == _WIN32_WINNT_WIN8
		// For WinRT 8.0 we only support running from the cache file.
		#undef ENABLE_SHADERS_CACHE_LOAD
		#define ENABLE_SHADERS_CACHE_LOAD 1
	#endif
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
	#include <android_native_app_glue.h>
	#include "Android/OgreAPKFileSystemArchive.h"
	#include "Android/OgreAPKZipArchive.h"
#endif

//#include <OgreRoot.h>
#include <OgreWindowEventUtilities.h>
#include <OgreFrameListener.h>
#include <Overlay/OgreOverlay.h>
#include <Overlay/OgreOverlaySystem.h>
#include <Overlay/OgreOverlayManager.h>
#include <Overlay/OgreOverlayContainer.h>
#include "resource.h"
#ifdef WIN32
#include <OgreD3D9RenderSystem.h>
#include <OgreD3D11RenderSystem.h>
#endif
#include <OgreGLRenderSystem.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#ifdef INCLUDE_RTSHADER_SYSTEM
	// Remove the comment below in order to make the RTSS use valid path for writing down the generated shaders.
	// If cache path is not set - all shaders are generated to system memory.
	//#define _RTSS_WRITE_SHADERS_TO_DISK
	#include "ShaderGeneratorTechniqueResolverListener.h"
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
	#ifdef __OBJC__
		#import <UIKit/UIKit.h>
	#endif

	namespace DC
	{
		class dc_application;
	}

	@interface DcApplicationGestureView : UIView
	{
		DC::dc_application *mApp;
	}
	@property (assign) DC::dc_application *mApp;
	@end
#endif

	namespace DC
	{
		class dc_application : 
			public Ogre::WindowEventListener, 
			public Ogre::FrameListener,
			public OIS::KeyListener,
			public OIS::MouseListener
#if OIS_WITH_MULTITOUCH
			, public OIS::MultiTouchListener  // note: we will send events either to MouseListener or to MultiTouchListener, depending on platform
#endif
		{
		public:
			dc_application();
			virtual ~dc_application();

			bool go();

		private:
			virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);

			virtual void windowResized(Ogre::RenderWindow* rw);
			virtual void windowClosed(Ogre::RenderWindow* rw);

			virtual bool keyPressed(const OIS::KeyEvent& evt);
			virtual bool keyReleased(const OIS::KeyEvent& evt);

			virtual bool mouseMoved(const OIS::MouseEvent& evt);
			virtual bool mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
			virtual bool mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
#if OIS_WITH_MULTITOUCH
			virtual bool touchMoved(const OIS::MultiTouchEvent& evt);
			virtual bool touchPressed(const OIS::MultiTouchEvent& evt);
			virtual bool touchReleased(const OIS::MultiTouchEvent& evt);
			virtual bool touchCancelled(const OIS::MultiTouchEvent &evt);
#endif

			Ogre::Root* mRoot;
			Ogre::String mResourcesCfg;
			Ogre::String mPluginsCfg;
			Ogre::RenderWindow* mWindow;
			Ogre::SceneManager* mSceneMgr;
			Ogre::Camera* mCamera;

			OIS::InputManager* mInputMgr;
			OIS::Keyboard* mKeyboard;
			OIS::Mouse* mMouse;

			Ogre::OverlaySystem* mOverlaySystem;
		};
	}
#endif