/*
-----------------------------------------------------------------------------
Decentralised Project
http://decentralised-project.org

Copyright (c) 2016 DC Foundation

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#include "dc_application.hpp"

namespace DC {
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
		//mRenderer = NULL;
	}

	dc_application::~dc_application()
	{
		Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);

		windowClosed(mWindow);

		delete mRoot;
	}

	bool dc_application::go()
	{
		// Create a new ogre root.
		mRoot = new Ogre::Root(Ogre::StringUtil::BLANK,
			Ogre::StringUtil::BLANK,
			"dc.log");

		// TODO: Make data directory configurable
		mRoot->addResourceLocation("./data", "FileSystem");

#ifdef WIN32
		//Ogre::D3D11RenderSystem* renderer = new Ogre::D3D11RenderSystem();
		Ogre::RenderSystem* renderer = new Ogre::D3D9RenderSystem(GetModuleHandle(NULL));
#else
		Ogre::RenderSystem* renderer = new Ogre::GLRenderSystem();
#endif

		Ogre::Root::getSingleton().addRenderSystem(renderer);
		renderer->setConfigOption("Full Screen", "No");
		renderer->setConfigOption("Video Mode", "800 x 600 @ 32-bit colour");
		//renderer->setConfigOption("Display Frequency", "50 Hz");
		renderer->setConfigOption("FSAA", "16");
		// renderer->setConfigOption("Fixed Pipeline Enabled", "Yes");
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

		mOverlaySystem = new Ogre::OverlaySystem();
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

		//Ogre::SceneNode* ogreNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
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

		mSceneMgr->addRenderQueueListener(mOverlaySystem);
		Ogre::OverlayManager& om = Ogre::OverlayManager::getSingleton();

		Ogre::Overlay* overlay = om.getByName("LoginOverlay");
		overlay->show();


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

	bool dc_application::keyPressed(const OIS::KeyEvent& evt)
	{
		// TODO
		return true;
	}

	bool dc_application::keyReleased(const OIS::KeyEvent& evt)
	{
		// TODO
		return true;
	}

	bool dc_application::mouseMoved(const OIS::MouseEvent& evt)
	{
		// TODO
		return true;
	}

	bool dc_application::mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
	{
		// TODO
		return true;
	}

	bool dc_application::mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
	{
		// TODO
		return true;
	}

#if OIS_WITH_MULTITOUCH
	bool dc_application::touchMoved(const OIS::MultiTouchEvent& evt)
	{
		// TODO
		return true;
	}

	bool dc_application::touchPressed(const OIS::MultiTouchEvent& evt)
	{
		// TODO
		return true;
	}

	bool dc_application::touchReleased(const OIS::MultiTouchEvent& evt)
	{
		// TODO
		return true;
	}

	bool dc_application::touchCancelled(const OIS::MultiTouchEvent &evt)
	{
		// TODO
		return true;
	}
#endif

}
