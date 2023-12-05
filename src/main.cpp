#include "EditorScaling.h"
#include <Geode/Loader.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/OptionsLayer.hpp>
#include <Geode/modify/MoreOptionsLayer.hpp>
// #include <Geode/modify/CCLabelBMFont.hpp>
#include <Geode/modify/LevelCell.hpp>
#include <Geode/modify/LevelTools.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include <Geode/modify/LevelSelectLayer.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/CCSprite.hpp>

#include <Geode/loader/SettingNode.hpp>
#include <Geode/loader/ModJsonTest.hpp>

#include <map>

#include "EditCodePopup.h"
#include "WebhookSend.h"

#pragma region Hooks

class $modify (PlayLayer)
{
    void showNewBest(bool p0, int p1, int p2, bool p3, bool p4, bool p5)
    {
        PlayLayer::showNewBest(p0,p1,p2,p3,p4,p5);

        Webhook::SendDiscordMessage(Stuff::getMessage(Mod::get()->getSavedValue<std::string>("msgTab0")));
    }

    void levelComplete()
    {
        int pctg = 0;

        if (PlayLayer::get()->m_isPracticeMode)
        {
            pctg = PlayLayer::get()->m_level->m_practicePercent;
        }
        else
        {
            pctg = PlayLayer::get()->m_level->m_normalPercent;
        }

        PlayLayer::levelComplete();

        if (pctg != 100)
        {
            Webhook::SendDiscordMessage(Stuff::getMessage(Mod::get()->getSavedValue<std::string>("msgTab1")));
        }
    }
};

#pragma endregion

#pragma region Setting

enum class Icon {
    Steve,
    Mike,
    LazarithTheDestroyerOfForsakenSouls,
    Geoff,
};

class MySettingValue;

class MySettingValue : public SettingValue {
protected:
    Icon m_icon;

public:
    MySettingValue(std::string const& key, std::string const& modID, Icon icon)
      : SettingValue(key, modID), m_icon(icon) {}

    bool load(json::Value const& json) override {
        try {
            m_icon = static_cast<Icon>(json.as<int>());
            return true;
        } catch(...) {
            return false;
        }
    }
    bool save(json::Value& json) const override {
        json = static_cast<int>(m_icon);
        return true;
    }
    SettingNode* createNode(float width) override;

    void setIcon(Icon icon) {
        m_icon = icon;
    }
    Icon getIcon() const {
        return m_icon;
    }
};

class WebURLChanged : public TextInputDelegate
{
    public:
        static inline std::string startText = "";
        static inline std::string currentText = "";
    
    virtual void textChanged(CCTextInputNode* p0)
    {
        currentText = p0->getString();

        Update();
    }

    static void Update();
};

class MessageChanged : public TextInputDelegate
{
    public:
        std::string startText = "";
        std::string currentText = "";
        int id;
    
    virtual void textChanged(CCTextInputNode* p0)
    {
        id = p0->getTag();

        currentText = p0->getString();
        currentText = Webhook::replaceAll(currentText, ";", "");
        currentText = Webhook::replaceAll(currentText, ":", "");

        Update();
    }

    void Update();
};

class MySettingNode : public SettingNode {
    public:
        static inline MySettingNode* instance = nullptr;
        static inline InputNode* webUrlInp = nullptr;
        static inline CCLabelBMFont* visLbl = nullptr;
        static inline CCNode* nodeP = nullptr;
        static inline CCNode* nodeL = nullptr;
        static inline CCNode* nodeD = nullptr;
        static inline geode::ScrollLayer* scrlLayer = nullptr;
        static inline std::vector<TextArea *> textAreas = { };
        static inline std::vector<InputNode*> textInputs = { };
        static inline std::vector<std::pair<std::string, std::string>> messageMap = { std::make_pair<std::string, std::string>("test condition", "assssssssssssssssssssssssssf"), std::make_pair<std::string, std::string>("test condition", "jhfg"), std::make_pair<std::string, std::string>("test condition", "asd") };
        static inline int selected = 0;
        static inline std::vector<CCMenuItemToggler*> togglers = { };

        void updateToggled()
        {
            this->dispatchChanged();
        }

        bool init(MySettingValue* value, float width) {
            if (!SettingNode::init(value))
                return false;

            instance = this;
            togglers.clear();
            selected = 0;

            this->setContentSize({ width, 220.f });

            std::stringstream ss;
            ss << "msgTab" << MySettingNode::selected;

            MySettingNode::messageMap = MySettingNode::load(Mod::get()->getSavedValue<std::string>(ss.str()));

            auto menu = CCMenu::create();
            menu->setPosition(0, 0);

            auto webUrlLbl = CCLabelBMFont::create("Webhook URL:", "bigFont.fnt");
            webUrlLbl->setAnchorPoint({0, 1});
            webUrlLbl->setPosition(10, getContentSize().height - 10);
            webUrlLbl->setScale(0.35f);
            menu->addChild(webUrlLbl);

            webUrlInp = InputNode::create(width + (webUrlLbl->getScaledContentSize().width + 40) - 40, "discord.com/api/webhooks/...", "bigFont.fnt");
            webUrlInp->setAnchorPoint({0, 0.5f});
            webUrlInp->setScale(0.5f);
            webUrlInp->setPosition(webUrlLbl->getScaledContentSize().width + 15, getContentSize().height - 15);
            webUrlInp->getInput()->m_maxLabelLength = 69420;
            webUrlInp->getInput()->m_usePasswordChar = true;
            webUrlInp->setString(Mod::get()->getSavedValue<std::string>("webhook-url"));
            webUrlInp->getInput()->setDelegate(new WebURLChanged());
            webUrlInp->getInput()->setAllowedChars("QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm1234567890QWERTYUIOPASDFGHJKLZXCVBNM-=_+[]{}\\|/?><!@#$%^&*()~`.,;:");
            menu->addChild(webUrlInp);

            auto btnVis = CCMenuItemSpriteExtra::create(CCSprite::create("GJ_button_01.png"), this, menu_selector(MySettingNode::toggleVisibility));
            btnVis->setScale(0.5f);
            btnVis->m_baseScale = 0.5f;
            btnVis->setPosition(width - 17.5f, getContentSize().height - 15);

            visLbl = CCLabelBMFont::create("+", "goldFont.fnt");
            visLbl->setPosition((btnVis->getContentSize().width / 2) + 1, (btnVis->getContentSize().height / 2) + 4);
            visLbl->setScale(1.5f);
            btnVis->addChild(visLbl);

            menu->addChild(btnVis);

            WebURLChanged::startText = Mod::get()->getSavedValue<std::string>("webhook-url");
            WebURLChanged::currentText = WebURLChanged::startText;

            auto line = CCSprite::create("gravityLine_001.png");
            line->setRotation(90);
            line->setScaleX(4);
            line->setScaleY(width / line->getContentSize().height);
            line->setAnchorPoint({0, 0.5f});
            line->setPosition({width / 2, getContentSize().height - 30});
            menu->addChild(line);

            auto line2 = CCSprite::create("gravityLine_001.png");
            line2->setRotation(90);
            line2->setScaleX(4);
            line2->setScaleY(width / line->getContentSize().height);
            line2->setAnchorPoint({0, 0.5f});
            line2->setPosition({width / 2, getContentSize().height - 65});
            menu->addChild(line2);

            this->addChild(menu);

            auto tabMenu = CCMenu::create();

            tabMenu->setContentSize({ width * 2 - 20 * 2, 25 });
            tabMenu->setAnchorPoint(CCPoint(0, 1));
            tabMenu->setPosition({10, this->getContentSize().height - 50});
            tabMenu->setScale(0.5f);
            tabMenu->setLayout(
            RowLayout::create()
                ->setGap(10.f)
                ->setGrowCrossAxis(true)
                ->setAxisAlignment(AxisAlignment::Start)
                ->setAutoScale(true)
            );

            std::vector tabs = {"New Best", "Level Complete"};

            for (size_t i = 0; i < tabs.size(); i++)
            {

                auto label = CCLabelBMFont::create(tabs.at(i), "goldFont.fnt");
                label->setScale(0.7f);
                label->setAnchorPoint(CCPoint(0.5f, 0.5f));

                auto label2 = CCLabelBMFont::create(tabs.at(i), "goldFont.fnt");
                label2->setScale(0.7f);
                label2->setAnchorPoint(CCPoint(0.5f, 0.5f));


                auto grey = CCScale9Sprite::create("GJ_button_04.png");
                grey->setContentSize(CCSize(label->getScaledContentSize().width + 20, 25));

                auto blue = CCScale9Sprite::create("GJ_button_05.png");
                blue->setContentSize(CCSize(label->getScaledContentSize().width + 20, 25));

                auto btnGD = CCMenuItemToggler::create(grey, blue, this, menu_selector(MySettingNode::onSelectTab));

                btnGD->setTag(i);
                btnGD->toggle(i == 0);
                btnGD->m_notClickable = !(i == 0);


                label->setPosition(CCPoint(grey->getContentSize().width / 2, label->getContentSize().height / 2));
                label2->setPosition(CCPoint(grey->getContentSize().width / 2, label2->getContentSize().height / 2));

                (reinterpret_cast<CCNode*>(btnGD->getChildren()->objectAtIndex(0)))->addChild(label, 1);
                (reinterpret_cast<CCNode*>(btnGD->getChildren()->objectAtIndex(1)))->addChild(label2, 1);

                togglers.push_back(btnGD);

                tabMenu->addChild(btnGD);
            }

            auto labelAdd = CCLabelBMFont::create("Add condition", "bigFont.fnt");
            labelAdd->setScale(0.7f);
            labelAdd->setAnchorPoint(CCPoint(0.5f, 0.5f));

            auto purple = CCScale9Sprite::create("GJ_square04.png");
            purple->setContentSize(CCSize((labelAdd->getScaledContentSize().width + 20) / 0.5f, 25 / 0.5f));
            purple->setScale(0.5f);

            labelAdd->setPosition(purple->getScaledContentSize() / 2);

            auto btnGD = CCMenuItemSpriteExtra::create(purple, this, menu_selector(MySettingNode::onAddCondition));
            btnGD->setPosition({535, 44});
            btnGD->addChild(labelAdd);

            auto lblText = CCLabelBMFont::create("Messages for event", "bigFont.fnt");
            lblText->setScale(0.35f);
            lblText->setAnchorPoint({0, 0.5f});
            lblText->setPosition({10, this->getContentSize().height - 40});
            menu->addChild(lblText);

            this->addChild(tabMenu);
            tabMenu->updateLayout();
            tabMenu->setLayout(nullptr);

            auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
            infoSpr->setScale(1);

            auto infoBtn = CCMenuItemSpriteExtra::create(infoSpr, this, menu_selector(MySettingNode::onInfo));

            //i know this isn't the best but nothing else is working :(
            infoBtn->setPosition(645, 44);
            
            tabMenu->addChild(infoBtn);
            tabMenu->addChild(btnGD);

            auto scrollLayer = geode::ScrollLayer::create(ccp(width - 20, 128), true, true);
            scrollLayer->m_peekLimitTop = 0;
            scrollLayer->m_peekLimitBottom = 0;
            scrollLayer->setPosition(10, 10);
            scrlLayer = scrollLayer;
            nodeD = reinterpret_cast<CCNode*>(scrollLayer->getChildren()->objectAtIndex(2));

            auto m = CreateMenuTab(scrollLayer->getContentSize().width);
            nodeD->addChild(m);
            m->setVisible(true);

            menu->addChild(scrollLayer);

            nodeP = nullptr;
            nodeL = nullptr;
            this->scheduleUpdate();

            return true;
        }

        void update(float dt) override {
            if (nodeP == nullptr)
            {
                if (this->getParent() != nullptr)
                {
                    nodeP = this->getParent()->getParent()->getParent();

                    for (size_t i = 0; i < nodeP->getChildren()->count(); i++)
                    {
                        (reinterpret_cast<CCNode*>(nodeP->getChildren()->objectAtIndex(i)))->setTag(i);
                    }

                    for (size_t i = 0; i < nodeP->getChildByTag(1)->getChildren()->count(); i++)
                    {
                        (reinterpret_cast<CCNode*>(nodeP->getChildByTag(1)->getChildren()->objectAtIndex(i)))->setTag(i);
                    }

                    auto btn = nodeP->getChildByTag(1)->getChildByTag(1);
                    btn->setPositionX(-57.5);

                    auto spr = CCScale9Sprite::create("GJ_button_01.png");
                    spr->setContentSize({115, 30});

                    auto newBtn = CCMenuItemSpriteExtra::create(spr, nodeP->getChildByTag(1), menu_selector(MySettingNode::onSendTest));

                    auto lbl = CCLabelBMFont::create("Send Test", "goldFont.fnt");
                    lbl->setScale(0.7f);
                    lbl->setPosition(spr->getContentSize() / 2);
                    lbl->setPositionY(17);

                    newBtn->addChild(lbl);
                    newBtn->setPosition({57.5f / 2, btn->getPositionY()});
                    newBtn->m_baseScale = 0.7f;
                    newBtn->setScale(0.7f);

                    nodeP->getChildByTag(1)->addChild(newBtn);

                    for (size_t i = 0; i < nodeP->getChildByTag(4)->getChildren()->count(); i++)
                    {
                        (reinterpret_cast<CCNode*>(nodeP->getChildByTag(4)->getChildren()->objectAtIndex(i)))->setTag(i);
                    }

                    nodeL = (reinterpret_cast<CCNode*>(nodeP->getChildByTag(4)->getChildByTag(2)));
                }
            }
            else
            {
                nodeL->setPosition(0,0);
            }
        }

        CCMenu* CreateMenuTab(float width)
        {
            textAreas.clear();
            textInputs.clear();

            auto menu = CCMenu::create();
            menu->setAnchorPoint({0, 1});

            menu->setPosition(0, messageMap.size() * 2 * 52.0f);
            menu->setContentSize({width, 0});

            scrlLayer->enableScrollWheel(true);
            scrlLayer->setTouchEnabled(!(messageMap.size() < 3));
            scrlLayer->setMouseEnabled(!(messageMap.size() < 3));

            if (messageMap.size() == 0 || messageMap == (std::vector<std::pair<std::string, std::string>>{}))
            {
                auto l = CCLabelBMFont::create("Nothing here yet :(", "bigFont.fnt");
                l->setScale(0.45f);
                l->setPosition(menu->getContentSize() / 2);
                l->setPositionY(75);

                menu->addChild(l);

                return menu;
            }

            try
            {
            
                int a = 0;
                for (size_t i = 0; i < messageMap.size(); i++)
                {
                    auto b = CCScale9Sprite::create("GJ_square01.png");
                    b->setScale(0.3f);
                    b->setContentSize({width / b->getScale(), 50 / b->getScale()});
                    b->setAnchorPoint({0, 1});
                    b->setPosition(0, menu->getContentSize().height + (-1 * 52 * a));
                    b->setColor({0, 0, 0});
                    b->setOpacity((a == roundf(a / 2) * 2) ? 50 : 0);

                    auto ifMsg = CCLabelBMFont::create("if (", "bigFont.fnt");
                    ifMsg->setAnchorPoint({0, 1});
                    ifMsg->setPosition(10, b->getContentSize().height - 10);
                    ifMsg->setScale(1.75f);

                    auto spr = CCScale9Sprite::create("GJ_square04.png");
                    auto spr2 = CCScale9Sprite::create("GJ_square04.png");
                    spr2->setColor({150, 150, 150});
                    
                    auto lbl = CCLabelBMFont::create(Stuff::getPrettyString(messageMap[i].first).c_str(), "bigFont.fnt");
                    lbl->setScale((1 / 0.3f) * 0.4f);

                    spr->setContentSize({lbl->getScaledContentSize().width + 20, lbl->getScaledContentSize().height + 10});
                    spr2->setContentSize({lbl->getScaledContentSize().width + 20, lbl->getScaledContentSize().height + 10});
                    
                    lbl->setPosition(spr->getContentSize() / 2);
                    
                    auto btn = CCMenuItemSprite::create(spr, spr2, this, menu_selector(MySettingNode::modifyCode));
                    btn->setAnchorPoint({0.5f, 0.5f});
                    btn->setScale(0.3f);
                    btn->setPosition({10 + 10 + 10 + 3.5f + btn->getScaledContentSize().width / 2, b->getPositionY() - ifMsg->getScaledContentSize().height + 44});
                    btn->addChild(lbl);
                    btn->setTag(i);

                    auto delSpr = CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png");
                    delSpr->setScale(0.45f);

                    auto delBtn = CCMenuItemSpriteExtra::create(delSpr, this, menu_selector(MySettingNode::onDelete));
                    delBtn->setAnchorPoint({0.5f, 0.5f});
                    delBtn->setPosition({b->getScaledContentSize().width - 10, b->getPositionY() - 10});
                    delBtn->setTag(i);

                    auto endIfMsg = CCLabelBMFont::create(")", "bigFont.fnt");
                    endIfMsg->setAnchorPoint({0, 1});
                    endIfMsg->setPosition((10 + 10 + 10 + 5 + btn->getScaledContentSize().width) / 0.3f, b->getContentSize().height - 10);
                    endIfMsg->setScale(1.75f);

                    auto sendMsg = CCLabelBMFont::create("Message:", "bigFont.fnt");
                    sendMsg->setScale(1.25f);
                    sendMsg->setAnchorPoint({0, 0.5f});
                    sendMsg->setPosition(65, b->getContentSize().height - 110);

                    auto txta = TextArea::create(messageMap[i].second, "chatFont.fnt", 1, 775 - 10, {0, 0}, 20, true);
                    txta->setAnchorPoint({0, 0});
                    txta->setContentSize({775 - 10, 70});
                    txta->setPosition({269 + 5 /*( ͡° ͜ʖ ͡°)*/, 15});

                    if ((reinterpret_cast<CCNode*>(txta->getChildren()->objectAtIndex(0)))->getChildrenCount() <= 4)
                    {
                        log::info("lower than required. i gotta do a thingy");

                        std::stringstream ss;
                        
                        ss << messageMap[i].second;

                        for (size_t i = 0; i < (4 - (reinterpret_cast<CCNode*>(txta->getChildren()->objectAtIndex(0)))->getChildrenCount()); i++)
                        {
                            ss << "\n";
                        }

                        txta->setString(ss.str());
                        txta->setContentSize({775 - 10, 70});
                    }

                    
                    textAreas.push_back(txta);

                    auto c = CCScale9Sprite::create("GJ_square01.png");
                    c->setContentSize({txta->getContentSize().width + 35, txta->getContentSize().height + 10});
                    c->setAnchorPoint({0, 0});
                    c->setPosition(-5, -5);
                    c->setColor({0, 0, 0});
                    c->setOpacity(50);
                    c->setID("background");
                    txta->addChild(c);

                    auto txti = geode::InputNode::create(c->getContentSize().width * 0.3f, "asdf", "bigFont.fnt");
                    txti->setString(messageMap[i].second);
                    txti->setOpacity(0);
                    txti->getInput()->getPlaceholderLabel()->setOpacity(0);
                    txti->getInput()->m_maxLabelScale = 0;
                    txti->getInput()->m_cursor->setOpacity(0);
                    txti->setPosition({200, b->getPositionY() - 36});
                    txti->setTag(i);
                    textInputs.push_back(txti);
                    txti->getInput()->setDelegate(new MessageChanged());
                    txti->getInput()->m_maxLabelLength = 2000;
                    txti->getInput()->setAllowedChars("qwertyuiopasdfghjklzxcvbnm1234567890QWERTYUIOPASDFGHJKLZXCVBNM-=_+[]{}\\|/?><!@#$%^&*()~` \"'.,");

                    b->addChild(ifMsg);
                    b->addChild(endIfMsg);
                    b->addChild(sendMsg);
                    b->addChild(txta);
                    menu->addChild(b);
                    menu->addChild(btn);
                    menu->addChild(delBtn);
                    menu->addChild(txti);
                    a++;
                }

                nodeD->setContentSize({nodeD->getContentSize().width, a * 52.0f});
                menu->setPositionY(nodeD->getContentSize().height);
                
                scrlLayer->moveToTop();

            }
            catch(const std::exception& e)
            {
                
            }
            
            return menu;
        }

        void modifyCode(CCObject* sender)
        {
            EditCodePopup::create((reinterpret_cast<CCNode*>(sender))->getTag());
        }

        void toggleVisibility(CCObject* sender)
        {
            webUrlInp->getInput()->m_usePasswordChar = !webUrlInp->getInput()->m_usePasswordChar;
            webUrlInp->setString(webUrlInp->getString());

            visLbl->setString(webUrlInp->getInput()->m_usePasswordChar ? "+" : "-");
        }

        void onDelete(CCObject* sender)
        {
            messageMap.erase(messageMap.begin() + (reinterpret_cast<CCNode*>(sender)->getTag()));
            Stuff::UpdateSettings(false);

            updateToggled();
        }

        void onSendTest(CCObject* sender) {
            auto popup = geode::createQuickPopup(
                "Send Test Message?",
                "This will send a <cl>message</c> to your <cb>Discord</c> server. If you get this message then you have setup your webhook link <cg>correctly</c>.\nThis will show a loading circle but this <cr>will not</c> show when sending a message in game.",
                "Cancel", "Send",
                [](auto, bool btn2) {
                    if (btn2) {
                        loading = LoadingCircle::create();
                        loading->setZOrder(69420);
                        loading->setParentLayer(reinterpret_cast<CCLayer*>(CCDirector::get()->getRunningScene()->getChildren()->objectAtIndex(2)));
                        loading->setFade(true);
                        loading->show();

                        Webhook::SendTestMessage();
                    }
                }
            );
        }

        void onInfo(CCObject* sender) {
            auto popup = geode::createQuickPopup(
                "Logic Help",
                "Leaving the message area blank acts as a <cr>‘return’</c> command so this stops it from sending the message.\nThis can be used to <cr>prevent</c> stuff from sending in a <cl>practice</c> completion",
                "OK", nullptr,
                nullptr
            );
        }

        void onSelectTab(CCObject* sender)
        {
            Mod::get()->setSavedValue<std::string>("webhook-url", WebURLChanged::currentText);
            WebURLChanged::startText = WebURLChanged::currentText;

            std::stringstream ss;
            ss << "msgTab" << MySettingNode::selected;
            Mod::get()->setSavedValue<std::string>(ss.str(), MySettingNode::save(MySettingNode::messageMap));

            MySettingNode::instance->updateToggled();

            int s = selected;
            selected = reinterpret_cast<CCNode*>(sender)->getTag();

            for (size_t i = 0; i < togglers.size(); i++)
            {
                togglers[i]->toggle(i == selected);
                togglers[i]->setEnabled(!(i == selected));
            }

            if (s != selected)
            {
                Stuff::UpdateSettings(true);
            }
        }

        void onAddCondition(CCObject* sender)
        {
            messageMap.push_back(std::make_pair<std::string, std::string>("", ""));

            Stuff::UpdateSettings(false);
            MySettingNode::instance->updateToggled();
        }

        static std::vector<std::string> split(const std::string& s, char seperator)
        {
            log::info("split");
            
            std::vector<std::string> output;

            if (s.find(seperator) == std::string::npos)
            {
                output.push_back(s);
                return output;
            }

            std::string::size_type prev_pos = 0, pos = 0;

            while((pos = s.find(seperator, pos)) != std::string::npos)
            {
                std::string substring( s.substr(prev_pos, pos-prev_pos) );

                output.push_back(substring);

                prev_pos = ++pos;
            }

            output.push_back(s.substr(prev_pos, pos-prev_pos)); // Last word

            return output;
        }

        static std::string save(std::vector<std::pair<std::string, std::string>> arr)
        {
            std::stringstream ss;

            for (size_t i = 0; i < arr.size(); i++)
            {
                ss << arr[i].first << ":" << arr[i].second << ";";
            }

            return ss.str();
        }

        static std::vector<std::pair<std::string, std::string>> load(std::string str)
        {
            std::vector<std::pair<std::string, std::string>> arr = {};

            if (!str.find(";"))
                return arr;

            try
            {
                auto a = split(str, ';');

                for (size_t i = 0; i < a.size(); i++)
                {
                    std::string item = a[i];
                    log::info(item);

                    if (item.size() < 1)
                    {
                        continue;
                    }

                    std::vector<std::string> b = split(item, ':');

                    arr.push_back(std::make_pair<std::string, std::string>(b[0] + "", b[1] + ""));
                }
            }
            catch(const std::exception& e)
            {
                arr.clear();
            }            

            return arr;
        }

    public:
        void commit() override {
            Mod::get()->setSavedValue<std::string>("webhook-url", WebURLChanged::currentText);
            WebURLChanged::startText = WebURLChanged::currentText;

            std::stringstream ss;
            ss << "msgTab" << MySettingNode::selected;
            Mod::get()->setSavedValue<std::string>(ss.str(), MySettingNode::save(MySettingNode::messageMap));

            this->dispatchCommitted();
        }
        bool hasUncommittedChanges() override {
            bool hUnc = false;

            if (WebURLChanged::currentText != WebURLChanged::startText)
                hUnc = true;

            std::stringstream ss;
            ss << "msgTab" << MySettingNode::selected;

            if (messageMap != load(Mod::get()->getSavedValue<std::string>(ss.str())))
                hUnc = true;

            return hUnc;
        }
        bool hasNonDefaultValue() override {
            return (WebURLChanged::currentText != "");
        }
        void resetToDefault() override {
            Mod::get()->setSavedValue<std::string>("webhook-url", "");
            webUrlInp->setString("");
        }

        static MySettingNode* create(MySettingValue* value, float width) {
            auto ret = new MySettingNode;
            if (ret && ret->init(value, width)) {
                ret->autorelease();
                return ret;
            }
            CC_SAFE_DELETE(ret);
            return nullptr;
        }
};

SettingNode* MySettingValue::createNode(float width) {
    return MySettingNode::create(this, width);
}

void WebURLChanged::Update()
{
    MySettingNode::instance->updateToggled();
}

void MessageChanged::Update()
{
    for (size_t i = 0; i < MySettingNode::textInputs.size(); i++)
    {
        MySettingNode::textAreas[i]->setString(MySettingNode::textInputs[i]->getString());
        MySettingNode::messageMap[i].second = MySettingNode::textInputs[i]->getString();

        if ((reinterpret_cast<CCNode*>(MySettingNode::textAreas[i]->getChildren()->objectAtIndex(0)))->getChildrenCount() <= 4)
        {
            std::stringstream ss;
            
            ss << MySettingNode::textInputs[i]->getString();

            int c = (reinterpret_cast<CCNode*>(MySettingNode::textAreas[i]->getChildren()->objectAtIndex(0)))->getChildrenCount();

            for (size_t i = 0; i < (4 - c); i++)
            {
                ss << "\n";
            }

            MySettingNode::textAreas[i]->setString(ss.str());
            MySettingNode::textAreas[i]->setContentSize({775 - 10, 70});
        }
    }

    MySettingNode::instance->updateToggled();
}

void Stuff::UpdateSettings(bool update)
{
    log::info("Updating layout");
    if (update)
    {
        std::stringstream ss;
        ss << "msgTab" << MySettingNode::selected;
        MySettingNode::messageMap = MySettingNode::load(Mod::get()->getSavedValue<std::string>(ss.str()));
    }

    float width = reinterpret_cast<CCNode*>(MySettingNode::nodeD->getChildren()->objectAtIndex(0))->getContentSize().width;

    reinterpret_cast<CCNode*>(MySettingNode::nodeD->getChildren()->objectAtIndex(0))->removeFromParentAndCleanup(true);

    auto a = MySettingNode::instance->CreateMenuTab(width);
    MySettingNode::nodeD->addChild(a);
    MySettingNode::scrlLayer->moveToTop();

    a->setVisible(true);
}

void Stuff::Set(std::string str, int value)
{
    MySettingNode::messageMap[value].first = str;

    MySettingNode::instance->updateToggled();
}

std::string Stuff::getPrettyString(std::string str)
{
    std::string s = str;

    if (s == "")
    {
        s = "True";
        return s;
    }

    s = Webhook::replaceAll(s, "{difficulty}", "Difficulty");
    s = Webhook::replaceAll(s, "{percentage}", "Percentage");
    s = Webhook::replaceAll(s, "{practice}", "Is Practice Mode?");
    s = Webhook::replaceAll(s, "{normal}", "Is Normal Mode?");
    s = Webhook::replaceAll(s, "{blank}", "True");

    if (s.find("="))
        s = Webhook::replaceAll(s, "=", " = ");
    else if (s.find(">"))
        s = Webhook::replaceAll(s, ">", " > ");
    else if (s.find("<"))
        s = Webhook::replaceAll(s, "<", " < ");

    return s;
}

std::pair<std::string, std::string> EditCodePopup::getMsg(int value)
{
    return MySettingNode::messageMap[value];
}

std::string Stuff::getMessage(std::string str)
{
    std::string s = "";
    auto arr = MySettingNode::load(str);

    for (size_t i = 0; i < arr.size(); i++)
    {
        std::string left;
        std::string right;
        
        if (arr[i].first.size() > 0)
        {
            if (arr[0].first == "{practice}")
            {
                if (PlayLayer::get()->m_isPracticeMode)
                {
                    return arr[i].second;
                }
            }
            else if (arr[0].first == "{normal}")
            {
                if (!PlayLayer::get()->m_isPracticeMode)
                {
                    return arr[i].second;
                }
            }
            else if (arr[0].first == "{blank}")
            {
                return arr[i].second;
            }

            //left = MySettingNode::split(arr[i].first, getSeperator(arr[i].first).c_str()[0])[0];
            //right = MySettingNode::split(arr[i].first, getSeperator(arr[i].first).c_str()[0])[1];

            
        }

        std::string msg = arr[i].second;

        log::info(msg);
        s = msg;

        return s;
    }

    return s;
}

std::string Stuff::getSeperator(std::string str)
{
    if (str.find("="))
        return "=";
    if (str.find(">"))
        return ">";
    if (str.find("<"))
        return "<";

    return "";
}

char Stuff::getSeperatorChar(std::string str)
{
    if (str.find("="))
        return '=';
    if (str.find(">"))
        return '>';
    if (str.find("<"))
        return '<';

    return '=';
}

$on_mod(Loaded) {
    Mod::get()->addCustomSetting<MySettingValue>("webhook-messages", Icon::LazarithTheDestroyerOfForsakenSouls);
}

#pragma endregion