//#include "SortedOptionsLayer.h"
#include <Geode/binding/GameManager.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

class TheJudgementLayer : public CCLayer {
protected:

    bool init()
{
auto dir = CCDirector::get();
this->setKeypadEnabled(true);
auto men = CCMenu::create();
men->setPosition({0, 0});

auto spr0 = CCSprite::create("GJ_gradientBG.png");
spr0->setVisible(true);
spr0->setScaleX((dir->getWinSize().width - 0) / spr0->getTextureRect().size.width - 0);
spr0->setScaleY(dir->getWinSize().height / spr0->getTextureRect().size.height);
spr0->setAnchorPoint({0,0});
spr0->setPosition({0,0});
men->addChild(spr0);
auto spr1 = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
spr1->setVisible(true);
spr1->setAnchorPoint({0.5,0.5});
spr1->setScaleX(0.065625f / spr1->getContentSize().width * dir->getWinSize().width);
spr1->setScaleY(0.14074074074074075f / spr1->getContentSize().height * dir->getWinSize().height);
spr1->setPosition({0.052083333333333336f * dir->getWinSize().width, dir->getWinSize().height - (0.09259259259259259f * dir->getWinSize().height)});
men->addChild(spr1);
auto spr2 = CCScale9Sprite::create("GJ_square04.png");
spr2->setVisible(true);
spr2->setAnchorPoint({0.5,0.5});
spr2->setPosition({dir->getWinSize().width / 2 + 0.0f * dir->getWinSize().width, dir->getWinSize().height / 2 + 0.0f * dir->getWinSize().width});
men->addChild(spr2);
auto spr3 = CCSprite::create("dialogIcon_019.png");
spr3->setVisible(true);
spr3->setAnchorPoint({0.5,0.5});
spr3->setPosition({dir->getWinSize().width / 2 + 0.0f * dir->getWinSize().width, dir->getWinSize().height / 2 + -0.09259259259259259f * dir->getWinSize().width});
men->addChild(spr3);
auto spr4 = CCSprite::create("edit_barBG_001.png");
spr4->setVisible(true);
men->addChild(spr4);


addChild(men);
return true;
};

    void keyBackClicked()
{
    this->onGoBack(nullptr);
};

void onGoBack(CCObject*) {
    CCDirector::get()->popSceneWithTransition(0.5, PopTransition::kPopTransitionFade);
}

public:
    static TheJudgementLayer* create()
{
auto ret = new TheJudgementLayer();
            if (ret && ret->init()) {
                ret->autorelease();
            } else {
                delete ret;
                ret = nullptr;
            }
            return ret;
};
    static CCScene* switchToScene()
{
        auto scene = CCScene::create();
        scene->addChild(TheJudgementLayer::create());
        CCDirector::get()->pushScene(CCTransitionFade::create(.5f, scene));
        return scene;
    };
};


class SortedOptionsLayer : public CCLayer {
    protected:
        virtual bool init(CCNode* n)
        {
            auto size = CCDirector::get()->getWinSize();

            auto menu = CCMenu::create();
            menu->setContentSize(size);

            auto background = CCSprite::create("gj_GradientBG.png");
            auto backgroundSize = background->getContentSize();

            background->setScaleX(size.width / backgroundSize.width);
            background->setScaleY(size.height / backgroundSize.height);
            background->setOpacity(125);

            background->setAnchorPoint(CCPoint(0.5f, 0.5f));
            background->setPosition({0,0});
            background->setColor({ 0, 0, 0 });

            auto brown = CCScale9Sprite::create("GJ_square01.png");
            brown->setAnchorPoint(CCPoint(0.5f, 0.5f));
            brown->setPosition(0,0);
            brown->setContentSize(CCSize(400, 280));
            brown->setScale(0.1f);

            auto brownAction = CCScaleTo::create(1, 1);
            brown->runAction(CCEaseElasticOut::create(brownAction));

            auto grey = CCScale9Sprite::create("GJ_square05.png");

            auto btn = CCMenuItemToggler::create(grey, grey, n, nullptr);

            menu->addChild(background, -1);
            menu->addChild(brown, 0);
            
            brown->addChild(btn);

            n->addChild(menu);
            return true;
        };
    public:
        static SortedOptionsLayer* create(CCNode* n)
        {        
            
        };

        void onHttpRequestCompleted(CCHttpClient* client, CCHttpResponse* response) {
            if (!response) {
                return;
            }
        }

        static void sendTest(CCLayer* t, std::string lvlName)
        {
            CCHttpRequest* request = new CCHttpRequest();
            request->setUrl("https://discord.com/api/webhooks/1176020453446393877/juLU5E2F3y-blieN03d4AnN0-PItmGRUuSvC0Z_KAY7n6bvPBQt8w6QNaTUYeXdOoJ_g");
            request->setRequestType(CCHttpRequest::kHttpPost);

            std::stringstream ss;
            ss << "@TheSillyDoggo has made a new best of " << (PlayLayer::get()->m_lastDeathPercent) << "% " << "on " << lvlName;
            std::string s = ss.str();

            // Set the request data
            CCString* postData = CCString::createWithFormat("content=%s", s);
            request->setRequestData(postData->getCString(), strlen(postData->getCString()));

            // Set callback function
            request->setResponseCallback(t, httpresponse_selector(SortedOptionsLayer::onHttpRequestCompleted));

            // Send the request
            CCHttpClient::getInstance()->send(request);

            // Release the request object
            request->release();
        };

        void callback(CCObject* obj)
        {
            CCHttpRequest* request = new CCHttpRequest();
            request->setUrl("https://discord.com/api/webhooks/1176020453446393877/juLU5E2F3y-blieN03d4AnN0-PItmGRUuSvC0Z_KAY7n6bvPBQt8w6QNaTUYeXdOoJ_g");
            request->setRequestType(CCHttpRequest::kHttpPost);

            // Set the request data
            CCString* postData = CCString::createWithFormat("content=%s", "sex");
            request->setRequestData(postData->getCString(), strlen(postData->getCString()));

            // Set callback function
            request->setResponseCallback(this, httpresponse_selector(SortedOptionsLayer::onHttpRequestCompleted));

            // Send the request
            CCHttpClient::getInstance()->send(request);

            // Release the request object
            request->release();
        };
};