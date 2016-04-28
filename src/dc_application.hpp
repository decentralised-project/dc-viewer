#ifndef __DC_APPLICATION_H
#define __DC_APPLICATION_H

#include <OgreRoot.h>
#include <OgreWindowEventUtilities.h>
#include <OgreFrameListener.h>
#ifdef WIN32
#include <OgreD3D9RenderSystem.h>
#include <OgreD3D11RenderSystem.h>
#endif
#include <OgreGLRenderSystem.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

class dc_application : public Ogre::WindowEventListener, public Ogre::FrameListener
{
public:
	dc_application();
	virtual ~dc_application();

	bool go();

private:
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);

	virtual void windowResized(Ogre::RenderWindow* rw);
	virtual void windowClosed(Ogre::RenderWindow* rw);

	Ogre::Root* mRoot;
	Ogre::String mResourcesCfg;
	Ogre::String mPluginsCfg;
	Ogre::RenderWindow* mWindow;
	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;

	OIS::InputManager* mInputMgr;
	OIS::Keyboard* mKeyboard;
	OIS::Mouse* mMouse;
};

#endif