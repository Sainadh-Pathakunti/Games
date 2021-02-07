/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
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
 ****************************************************************************/

#include "AppDelegate.h"
#include "MainMenuScene.h"

// #define USE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#endif

USING_NS_CC;
const int m_HDRCutoffWidth = 1280;
#if CC_TARGET_PLATFORM == CC_PLATFORM_WINRT
const int m_HDCutoffWidth = 960;
#else
const int m_HDCutoffWidth = 640;
#endif

const cocos2d::Size m_designResolutionSize = Size(2048, 1152);
const cocos2d::Size m_smallResolutionSize = Size(512, 288);
const cocos2d::Size m_mediumResolutionSize = Size(1024, 576);
const cocos2d::Size m_largeResolutionSize = Size(2048, 1152);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("ProjName", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("ProjName");
#endif
        director->setOpenGLView(glview);
    }

    float contentScaleFactor;
     
     glview->setDesignResolutionSize(m_designResolutionSize.width, m_designResolutionSize.height, ResolutionPolicy::FIXED_HEIGHT);
     const Size frameSize = glview->getFrameSize();
     // if the frame's height is larger than the height of medium size.
     
 #if CC_TARGET_PLATFORM != CC_PLATFORM_WINRT
     if (frameSize.width > m_HDRCutoffWidth)
     {
         contentScaleFactor = m_largeResolutionSize.height / m_designResolutionSize.height;
     }
     // if the frame's height is larger than the height of small size.
     else
 #endif
         if (frameSize.width > m_HDCutoffWidth)
         {
             contentScaleFactor = m_mediumResolutionSize.height / m_designResolutionSize.height;
            
         }
     // if the frame's height is smaller than the height of medium size.
         else
         {
             contentScaleFactor = m_smallResolutionSize.height / m_designResolutionSize.height;
            
         }
     
     director->setContentScaleFactor(contentScaleFactor);

    register_all_packages();

    // create a scene. it's an autorelease object
    auto scene = MainMenu::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#endif
}
