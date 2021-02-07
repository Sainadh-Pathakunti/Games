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

#include "GameScene.h"
#include "MainMenuScene.h"
#include "cocostudio/CocoStudio.h"
#include "cocostudio/SimpleAudioEngine.h"


USING_NS_CC;

Scene* GameScene::createScene()
{
    return GameScene::create();
}

bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    
    auto gameBg = Sprite::create("gamebg.png");
    gameBg->setPosition(Vec2(visibleSize.width/2 , visibleSize.height/2 ));
    this->addChild(gameBg);
    
    
    
    board = Sprite::create("board.png");
    board->setPosition(Vec2(visibleSize.width/2 , visibleSize.height * 0.65 ));
    this->addChild(board);
    
    auto layer = LayerColor::create(Color4B::GREEN);
    layer->setPosition(0,0);
    layer->changeWidthAndHeight(visibleSize.width, visibleSize.height * 0.2);
    this->addChild(layer);
    
    
    
    yellowLablel = Label::createWithSystemFont("0","fonts/arial.ttf", 300);
    yellowLablel->setPosition(Vec2(visibleSize.width * 0.3,visibleSize.height * 0.6));
    yellowLablel->setColor(Color3B::BLACK);
    yellowLablel->setName("yellowLablel");
    yellowLablel->setVisible(false);
    yellowLablel->setOpacity(50);
    yellowLablel->enableBold();
    this->addChild(yellowLablel);
    
    
    redLablel = Label::createWithSystemFont("0","fonts/arial.ttf", 300);
    redLablel->setPosition(Vec2(visibleSize.width * 0.7,visibleSize.height * 0.6));
    redLablel->setColor(Color3B::BLACK);
    
    redLablel->setName("redLablel");
    redLablel->setVisible(false);
    redLablel->setOpacity(50);
    redLablel->enableBold();
    this->addChild(redLablel);
    
    Paintwall = Label::createWithSystemFont("PAINT THIS WALL WITH THE BRUSHES BELOW  \n\n IN 2 : 3 RATIO (2 YELLOW'S AND 3 RED'S) TO OBTAIN RESULT COLOR \n\n ALL MATCHED CASES WILL SHOWN ON TOP","fonts/arial.ttf", 50);
    Paintwall->enableWrap(true);
    Paintwall->setAlignment(TextHAlignment::CENTER);
    Paintwall->setPosition(Vec2(visibleSize.width * 0.5,visibleSize.height * 0.6));
    Paintwall->setColor(Color3B::BLACK);
    Paintwall->setName("Paintwall");
    Paintwall->setOpacity(50);
    Paintwall->enableBold();
    this->addChild(Paintwall);
    
    
    auto topPlayer = LayerColor::create(Color4B::GRAY);
    topPlayer->setPosition(0, visibleSize.height * 0.8);
    topPlayer->setOpacity(95);
    topPlayer->changeWidthAndHeight(visibleSize.width, visibleSize.height * 0.2);
    this->addChild(topPlayer);
    
    
    
    Size scollFrameSize = Size(visibleSize.width, visibleSize.height * 0.2);
    scrollView = ScrollView::create();
    scrollView->setContentSize(scollFrameSize);
    scrollView->setPosition(topPlayer->getPosition());
    scrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    scrollView->setBounceEnabled(true);
    scrollView->setTouchEnabled(true);
    auto containerSize = Size(scollFrameSize.width, scollFrameSize.height);
    scrollView->setInnerContainerSize(containerSize);
    
    this->addChild(scrollView);
    

    
    auto yellowPaint = Sprite::create("yellowPaint.png");
    yellowPaint->setPosition(Vec2(visibleSize.width * 0.1 , visibleSize.height * 0.17 ));
    this->addChild(yellowPaint);
    
    
    auto Redpaint = Sprite::create("Redpaint.png");
    Redpaint->setPosition(Vec2(visibleSize.width * 0.9 , visibleSize.height * 0.17 ));
    this->addChild(Redpaint);
    
    yellowBrush = Sprite::create("YellowBrush.png");
    yellowBrush->setPosition(Vec2(visibleSize.width * 0.3 , visibleSize.height * 0.15 ));
    yellowBrush->setTag(1);
    yellowBrush->setName("YELLOW");
    this->addChild(yellowBrush);
    
    Redbrush = Sprite::create("Redbrush.png");
    Redbrush->setPosition(Vec2(visibleSize.width * 0.7 , visibleSize.height * 0.15 ));
    Redbrush->setTag(1);
    Redbrush->setName("RED");
    this->addChild(Redbrush);
    
    
    Button *reset_Btn = Button::create("btn.png","btn.png");
    reset_Btn->setPosition(Vec2(visibleSize.width/2 , visibleSize.height * 0.07));
    reset_Btn->setTag(1);
    this->addChild(reset_Btn);
    reset_Btn->addTouchEventListener(CC_CALLBACK_2(GameScene::buttonclicked, this));
    
    
    Button *back_Btn = Button::create("back.png","back.png");
    back_Btn->setPosition(Vec2(visibleSize.width * 0.04 , visibleSize.height * 0.9));
    back_Btn->setTag(2);
    this->addChild(back_Btn);
    back_Btn->addTouchEventListener(CC_CALLBACK_2(GameScene::buttonclicked, this));
    
    
    
    
    Label *label = Label::createWithTTF("Reset","fonts/arial.ttf", 45);
    label->setPosition(reset_Btn->getPosition());
    label->setColor(Color3B::BLACK);
    label->enableBold();
    this->addChild(label);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    listener->onTouchCancelled = CC_CALLBACK_2(GameScene::onTouchCancelled, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, yellowBrush);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), Redbrush);
    
    return true;
}


void GameScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
}


void GameScene::buttonclicked(Ref* pSender, Widget::TouchEventType eEventType)
{
    
    if(eEventType != ui::Widget::TouchEventType::ENDED) return;
   
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("table_clickbtn.mp3");
    
    int tag = ((Button*)(pSender))->getTag();
    
    if(tag == 1){
        Director::getInstance()->replaceScene(GameScene::createScene());
    }else{
        Director::getInstance()->replaceScene(MainMenu::createScene());
    }
    
    
}

bool GameScene::onTouchBegan(Touch* touch, Event* event)
{
    Sprite* targetSP = static_cast<Sprite*>(event->getCurrentTarget());
    Vec2 locationInNode = targetSP->convertToNodeSpace(touch->getLocation());
    Rect rect = Rect(0, 0, targetSP->getContentSize().width, targetSP->getContentSize().height);
    
    if (rect.containsPoint(locationInNode))
    {

        return true;
    }
    return false;
}

void GameScene::onTouchMoved(Touch* touch, Event* event)
{
    Sprite* targetSP = static_cast<Sprite*>(event->getCurrentTarget());
    
    Rect rect = board->getBoundingBox();
    
    targetSP->setPosition(targetSP->getPosition() + touch->getDelta());
    
        Color4B colr;
        if(targetSP->getName().compare("YELLOW") == 0){
            colr = Color4B(255,237,0,0);
        }
        if(targetSP->getName().compare("RED") == 0 ){
            colr = Color4B(255,0,0,0);
        }
    
        auto color = LayerColor::create(colr);
        color->setPosition(visibleSize/2);
        color->changeWidthAndHeight(100, 100);
        this->addChild(color,111);
    
}


void GameScene::onTouchEnded(Touch* touch, Event* event)
{
    Sprite* targetSP = static_cast<Sprite*>(event->getCurrentTarget());
    
    
    if (targetSP->getBoundingBox().intersectsRect(board->getBoundingBox()))
    {
            
        
        if(targetSP->getName().compare("YELLOW") == 0){
            
            targetSP->setPosition(Vec2(visibleSize.width * 0.3,visibleSize.height * 0.15));
            
        }else{
            targetSP->setPosition(Vec2(visibleSize.width * 0.7,visibleSize.height * 0.15));
        }
        
        
        board->setColor(getNewColor(targetSP));
        
    }
        
       

}

void GameScene::onTouchCancelled(Touch* touch, Event* event)
{
    Sprite* targetSP = static_cast<Sprite*>(event->getCurrentTarget());
    //    onChange(targetSP, ScaleTo::create(0.1, 1.0), preZOrder, targetInitPos);
}


Color3B GameScene::getNewColor(Sprite* targetSP) {
    
    Color3B newcolor;
    int rtag = Redbrush->getTag();
    int ytag = yellowBrush->getTag();
   
    Paintwall->setVisible(false);
    
    if(targetSP->getName().compare("RED") == 0){
       
        int rlwtag;
        if( ytag == 1){
            rlwtag = rtag;
            rtag = 1;
            ytag = 0;
        }else{
            rlwtag = rtag;
            ytag--;
        }
        
        float val = (float)1/(rtag + ytag);
        int percent =  (val  * 100) *  rtag;
        
        rtag = rlwtag + 1;
        
        redLablel->setVisible(true);
        redLablel->setString(StringUtils::format("%d",rtag - 1));
                
        Redbrush->setTag(rtag);
        
        // match case
        if (percent == 60){
            
            auto matchBtn = Sprite::create("btn.png");
            matchBtn->setPosition( matchBtn->getContentSize().width * scrollView->getChildren().size() +  matchBtn->getContentSize().width  ,matchBtn->getContentSize().height);
            
            Label *label = Label::createWithTTF(StringUtils::format("%d : %d",ytag, rtag - 1),"fonts/arial.ttf", 45);
            label->setPosition(matchBtn->getContentSize()/2);
            label->setColor(Color3B::BLACK);
            label->enableBold();
            matchBtn->addChild(label);
            
            scrollView->setInnerContainerSize(Size(matchBtn->getContentSize().width * scrollView->getChildren().size() +  matchBtn->getContentSize().width * 1.5,matchBtn->getContentSize().height));
            scrollView->addChild(matchBtn);
            
            return Color3B(255,95,0);
        }
        if (percent  < 10){
            return Color3B(255,237,0);
        }
        if (percent  < 20){
            return Color3B(255,190,0);
        }
        if (percent  < 30){
            return Color3B(255,167,0);
        }
        if (percent  < 40){
            return Color3B(255,158,0);
        }
        if (percent  <= 50){
            return Color3B(255,124,0);
        }
        if (percent  < 60){
            return Color3B(255,85,0);
            
        }
        if (percent  < 70){
            return Color3B(255,79,0);
        }
        if (percent  < 80){
            return Color3B(255,68,0);
        }
        if (percent  < 90){
            return Color3B(255,43,0);
        }
        if (percent  < 100){
            return Color3B(255,24,0);
        }
        if (percent == 100){
            return Color3B(255,0,0);
        }
        
    }
    
    if(targetSP->getName().compare("YELLOW") == 0){
    
        int ylwtag;
        if( rtag == 1){
            rtag = 0;
            ylwtag = ytag;
            ytag = 1;
        }else{
            ylwtag = ytag;
            rtag--;
        }
        
        float val = (float)1/(rtag + ytag);
        int percent =  (val  * 100) *  ytag;
        
        
        
        ytag = ylwtag + 1;
        yellowLablel->setVisible(true);
        yellowLablel->setString(StringUtils::format("%d",ytag - 1));
        
        
        yellowBrush->setTag(ytag);
        
     // match case
        if (percent == 40){
            
            auto matchBtn = Sprite::create("btn.png");
            matchBtn->setPosition( matchBtn->getContentSize().width * scrollView->getChildren().size() +  matchBtn->getContentSize().width  ,matchBtn->getContentSize().height);
            scrollView->setInnerContainerSize(Size(matchBtn->getContentSize().width * scrollView->getChildren().size() +  matchBtn->getContentSize().width * 1.5,matchBtn->getContentSize().height));
            
            Label *label = Label::createWithTTF(StringUtils::format("%d : %d",ytag -1,rtag),"fonts/arial.ttf", 45);
            label->setPosition(matchBtn->getContentSize()/2);
            label->setColor(Color3B::BLACK);
            label->enableBold();
            matchBtn->addChild(label);
            
            scrollView->addChild(matchBtn);
            
            return Color3B(255,95,0);
        }
        
        
        if (percent < 10){
            return Color3B(255,21,0);
        }
        if (percent < 20){
            return Color3B(255,46,0);
        }
        
        if (percent < 30){
            return Color3B(255,69,0);

        }
        if (percent < 40){
            return Color3B(255,92,0);

        }
        if (percent <= 50){
            return Color3B(255,124,0);
        }
        if (percent < 60){
            return Color3B(255,139,0);
        }
        if (percent < 70){
            return Color3B(255,158,0);
        }
        if (percent < 80){
            return Color3B(255,182,0);
        }
        if (percent < 90){
            return Color3B(255,198,0);
        }
        if (percent < 100){
            return Color3B(255,207,0);
        }
        if (percent == 100){
            return Color3B(255,237,0);

        }
    
    }
   
}
