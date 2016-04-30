#include "dc_application.hpp"

#include <OgreEntity.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OgreException.h>
#include <OgreTextureManager.h>
#include <OgreLogManager.h>
#include "resource.h"

dc_application::dc_application()
	: mRoot(0),
	mResourcesCfg(""),
	mPluginsCfg(""),
	mWindow(0),
	mSceneMgr(0),
	mCamera(0),
	mInputMgr(0),
	mMouse(0),
	mKeyboard(0)
{
}

dc_application::~dc_application()
{
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);

	windowClosed(mWindow);

	delete mRoot;
}

bool dc_application::go()
{
	mRoot = new Ogre::Root(Ogre::StringUtil::BLANK, 
						   Ogre::StringUtil::BLANK,
						   "dc.log");

#ifdef WIN32
	//Ogre::D3D11RenderSystem* d3d11 = new Ogre::D3D11RenderSystem();
	Ogre::RenderSystem* renderer = new Ogre::D3D9RenderSystem(GetModuleHandle(NULL));
#else
	Ogre::RenderSystem* renderer = new Ogre::GLRenderSystem();
#endif

	Ogre::Root::getSingleton().addRenderSystem(renderer);
	renderer->setConfigOption("Full Screen", "No");
	renderer->setConfigOption("Video Mode", "800 x 600 @ 32-bit colour");
	//renderer->setConfigOption("Display Frequency", "50 Hz");
	renderer->setConfigOption("FSAA", "16");
	renderer->setConfigOption("Fixed Pipeline Enabled", "Yes");
	//renderer->setConfigOption("RTT Preferred Mode", "FBO");
	//renderer->setConfigOption("VSync", "No");
	//renderer->setConfigOption("sRGB Gamma Conversion", "No");

	Ogre::Root::getSingleton().setRenderSystem(renderer); 

	if (!(mRoot->restoreConfig() || mRoot->showConfigDialog()))
		return false;

	mWindow = mRoot->initialise(true, "Decentralised");

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	HWND hwnd;
	mWindow->getCustomAttribute("WINDOW", (void*)&hwnd);
	LONG iconID = (LONG)LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON1));
	SetClassLong(hwnd, GCL_HICON, iconID);
#endif

	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);

	mCamera = mSceneMgr->createCamera("MainCam");
	mCamera->setPosition(0, 0, 80);
	mCamera->lookAt(0, 0, -300);
	mCamera->setNearClipDistance(5);

	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

	mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) /
		Ogre::Real(vp->getActualHeight()));

	// Create Scene
	//Ogre::Entity* ogreEntity = mSceneMgr->createEntity("ogrehead.mesh");

	Ogre::SceneNode* ogreNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	//ogreNode->attachObject(ogreEntity);

	mSceneMgr->setAmbientLight(Ogre::ColourValue(.5, .5, .5));

	Ogre::Light* light = mSceneMgr->createLight("MainLight");
	light->setPosition(20, 80, 50);

	// OIS
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");

	OIS::ParamList pl;
	size_t windowHandle = 0;
	std::ostringstream windowHandleStr;

	mWindow->getCustomAttribute("WINDOW", &windowHandle);
	windowHandleStr << windowHandle;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHandleStr.str()));

#if defined OIS_WIN32_PLATFORM
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
	pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
	pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
#elif defined OIS_LINUX_PLATFORM
	pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
	pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
	pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
	pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
#endif

	mInputMgr = OIS::InputManager::createInputSystem(pl);

	mKeyboard = static_cast<OIS::Keyboard*>(
		mInputMgr->createInputObject(OIS::OISKeyboard, false));
	mMouse = static_cast<OIS::Mouse*>(
		mInputMgr->createInputObject(OIS::OISMouse, false));

	windowResized(mWindow);
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	mRoot->addFrameListener(this);

	mRoot->startRendering();

	return true;
}

bool dc_application::frameRenderingQueued(const Ogre::FrameEvent& fe)
{
	if (mWindow->isClosed()) return false;

	mKeyboard->capture();
	mMouse->capture();

	if (mKeyboard->isKeyDown(OIS::KC_ESCAPE)) return false;

	return true;
}

void dc_application::windowResized(Ogre::RenderWindow* rw)
{
	int left, top;
	unsigned int width, height, depth;

	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState& ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

void dc_application::windowClosed(Ogre::RenderWindow* rw)
{
	if (rw == mWindow)
	{
		if (mInputMgr)
		{
			mInputMgr->destroyInputObject(mMouse);
			mInputMgr->destroyInputObject(mKeyboard);

			OIS::InputManager::destroyInputSystem(mInputMgr);
			mInputMgr = 0;
		}
	}
}