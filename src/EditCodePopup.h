#include "main.h"
#include "Dropdown.h"
using namespace geode::prelude;

class EditCodePopup : public geode::Popup<int> {
public:
    InputNode *inpNode;
    std::pair<std::string, std::string> getMsg(int value);
    Dropdown* dd;
    int value = 0;
protected:
    bool setup(int value) override {
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        m_closeBtn->setVisible(false);

        this->value = value;
        this->setTitle("Setup Logic");

        auto menu = CCMenu::create();
        menu->setPosition(winSize / 2);
        menu->setContentSize(m_size);
        menu->ignoreAnchorPointForPosition(false);

        std::stringstream ss;
        ss << value;

        auto label = CCLabelBMFont::create(ss.str().c_str(), "bigFont.fnt");
        label->setPosition(winSize / 2);
        //menu->addChild(label);        

        //auto a = InputNode::create(100, "", "bigFont.fnt");
        //a->setString(getMsg(value).first);
        //inpNode = a;
        //menu->addChild(a);

        auto closeSpr = ButtonSprite::create("Cancel");
        auto setSpr = ButtonSprite::create("Set");

        auto close = CCMenuItemSpriteExtra::create(closeSpr, this, menu_selector(EditCodePopup::Close));
        auto set = CCMenuItemSpriteExtra::create(setSpr, this, menu_selector(EditCodePopup::Set));

        close->m_baseScale = 0.75f;
        close->setScale(0.75f);
        set->m_baseScale = 0.75f;
        set->setScale(0.75f);

        close->setPosition({(menu->getContentSize().width / 2 - (close->getScaledContentSize().width / 2) - 5) + 20, 20});
        set->setPosition({(menu->getContentSize().width / 2 + (set->getScaledContentSize().width / 2) + 5) + 20, 20});

        std::vector<std::string> d = {Stuff::getPrettyString("{blank}"), Stuff::getPrettyString("{normal}"), Stuff::getPrettyString("{practice}")};

        auto b = Dropdown::create(d);
        b->menu->setPositionX(m_size.width / 2);
        b->menu->setPositionY(m_size.height - 50);
        b->lbl->setString(Stuff::getPrettyString(getMsg(value).first).c_str());
        dd = b;
        menu->addChild(b->menu);

        menu->addChild(close);
        menu->addChild(set);

        reinterpret_cast<CCLayer*>(this->getChildren()->objectAtIndex(0))->addChild(menu);
        this->show();
        return true;
    }

    void Close(CCObject* sender)
    {
        this->removeFromParentAndCleanup(true);
    }

    void Set(CCObject* sender)
    {
        //std::stringstream ss;

        //ss << "{stars}" << "==" << "10";

        Stuff::Set(dd->lbl->getString(), value);
        Stuff::UpdateSettings(false);

        this->removeFromParentAndCleanup(true);
    }

public:
    static EditCodePopup* create(int index) {
        auto ret = new EditCodePopup();
        if (ret && ret->init(300.f, 200.f, index)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};