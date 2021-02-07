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

#include "MainMenuScene.h"
#include "cocostudio/CocoStudio.h"
#include "cocostudio/SimpleAudioEngine.h"


using namespace std;
using namespace cocos2d;
using namespace ui;

USING_NS_CC;

Scene* MainMenu::createScene()
{
    return MainMenu::create();
}

bool MainMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    if(!CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()){
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("table_bgm.mp3");
        CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1);
    }
 

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    auto bg_SP = Sprite::create("bg.png");
    bg_SP->setPosition(Vec2(visibleSize.width/2 , visibleSize.height/2 ));
    this->addChild(bg_SP);
    
    auto tittle_SP = Sprite::create("tittle.png");
    tittle_SP->setPosition(Vec2(visibleSize.width/2 , visibleSize.height * 0.75 ));
    tittle_SP->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(1, 1), ScaleTo::create(1, 0.97), NULL)));
    

    this->addChild(tittle_SP);
    
    Button *play_Btn = Button::create("play_btn.png","play_btn.png");
    play_Btn->setPosition(Vec2(visibleSize.width/2 , visibleSize.height * 0.25));
    play_Btn->setTag(1);
    play_Btn->setScale(2);
    this->addChild(play_Btn);
    play_Btn->runAction(RepeatForever::create(Sequence::create(RotateTo::create(1, -10), RotateTo::create(1, 10), NULL)));
        
    play_Btn->addTouchEventListener(CC_CALLBACK_2(MainMenu::buttonclicked, this));
  
    return true;
}

void MainMenu::buttonclicked(Ref* pSender, Widget::TouchEventType eEventType)
{
    
    if(eEventType != ui::Widget::TouchEventType::ENDED) return;
   
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("table_clickbtn.mp3");
    
    Director::getInstance()->replaceScene(GameScene::createScene());
    
}

void MainMenu::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
