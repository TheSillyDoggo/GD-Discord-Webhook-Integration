using namespace geode::prelude;



class SortOptionsCallback
{
    public:
        static inline int selected = 0;

        void callback(CCObject* obj)
        {
            auto ob = reinterpret_cast<CCMenuItemToggler*>(obj);

            //ew disgusting code :(

            if (ob != nullptr)
            {
                auto pa = ob->getParent()->getChildren();

                if (pa != nullptr)
                {
                    for (size_t i = 0; i < pa->count(); i++)
                    {
                        auto a = reinterpret_cast<CCMenuItemToggler*>(pa->objectAtIndex(i));

                        if (a != nullptr)
                        {
                            a->toggle(!false);
                        
                            if (a->getTag() == i)
                            {
                                a->toggle(!true);
                            }

                            if (ob->getTag() == i)
                            {
                                selected = i;
                            }
                        }
                    }

                    stuff(ob);
                }
            }
        };

        static void stuff(CCNode* node)
        {
            auto objs = node->getParent()->getParent();

            for (size_t i = 0; i < 5; i++)
            {
                auto a =(reinterpret_cast<CCLayer*>(objs->getChildren()->objectAtIndex(3 + i)));
                
                if (i == selected)
                {
                    a->setVisible(true);
                }
                else
                {
                    a->setVisible(false);
                }
                a->setTag(i);
            }            
        };
};

std::vector<std::string> names;
std::vector<std::string> keys;
std::vector<std::string> infos;

void HideOldOptions(MoreOptionsLayer* layer)
{
    auto b = reinterpret_cast<CCNode*>((reinterpret_cast<CCNode*>(layer->getChildren()->objectAtIndex(0)))->getChildByTag(2))->getChildren();

    for (size_t i = 0; i < b->count() - 8; i++)
    {
        auto c = reinterpret_cast<CCNode*>(b->objectAtIndex(i));

        c->setScale(0);
    }
}

void AddNewOptions(MoreOptionsLayer* layer, std::vector<std::string> categories)
{
    auto men = reinterpret_cast<CCLayer*>(layer->getChildren()->objectAtIndex(0));

    for (size_t i = 0; i < categories.size(); i++)
    {
        CCMenu* menu = CCMenu::create();
        menu->ignoreAnchorPointForPosition(true);
        menu->setPosition(0,0);

        int y = 0;

        for (size_t e = 0; e < names.size(); e++)
        {
            auto lbl = CCLabelBMFont::create(names.at(e).c_str(), "bigFont.fnt");

            float x = 0;

            if ((e - 1) % 2 == 0)
            {
                x = 338.5f;
            }
            else
            {
                x = 146.5f;
            }

            lbl->setScale(0.5f);
            lbl->setAnchorPoint(CCPoint(0, 0.5f));

            lbl->setPosition(x, 240 - (y * 35));

            menu->addChild(lbl);

            if (x == 338.5f)
            {
                y++;
            }
        }

        menu->setID(categories.at(i));

        men->addChild(menu);
    }
    
}

void SortOptions(MoreOptionsLayer* layer)
{
    auto menu = CCMenu::create();
    menu->setLayout(
    RowLayout::create()
        ->setGap(10.f)
        ->setGrowCrossAxis(true)
        ->setAxisAlignment(AxisAlignment::Center)
        ->setAutoScale(false)
    );


    std::vector<std::string> categories = {"Gameplay", "Performance", "Accessibility", "Editor", "Misc"};
    int categoryCount = categories.size();
    std::vector<float> categorySize = {100, 130, 130, 80, 70};    
    SortOptionsCallback::selected = 0;


    menu->setContentSize({ 560, 25 });
    menu->setAnchorPoint(CCPoint(0.5f, 1));
    menu->setPositionY(265);
    menu->setScale(0.65f);


    for (size_t i = 0; i < categoryCount; i++)
    {
        auto grey = CCScale9Sprite::create("GJ_button_04.png");
        grey->setContentSize(CCSize(categorySize.at(i), 25));

        auto blue = CCScale9Sprite::create("GJ_button_05.png");
        blue->setContentSize(CCSize(categorySize.at(i), 25));
        auto btn = CCMenuItemToggler::create(grey, blue, layer, menu_selector(SortOptionsCallback::callback));
        btn->setID(categories.at(i).c_str());
        btn->setTag(i);
        btn->toggle(i == 0);

        auto label = CCLabelBMFont::create(categories.at(i).c_str(), "goldFont.fnt");
        label->setScale(0.7f);
        label->setAnchorPoint(CCPoint(0.5f, 0.5f));
        label->setPosition(CCPoint(grey->getContentSize().width / 2, label->getContentSize().height / 2));
        (reinterpret_cast<CCNode*>(btn->getChildren()->objectAtIndex(0)))->addChild(label, 1);

        auto label2 = CCLabelBMFont::create(categories.at(i).c_str(), "goldFont.fnt");
        label2->setScale(0.7f);
        label2->setAnchorPoint(CCPoint(0.5f, 0.5f));
        label2->setPosition(CCPoint(grey->getContentSize().width / 2, label2->getContentSize().height / 2));
        (reinterpret_cast<CCNode*>(btn->getChildren()->objectAtIndex(1)))->addChild(label2, 1);

        menu->addChild(btn, -1 * categoryCount + (i));
    }

    auto b = (reinterpret_cast<CCNode*>(layer->getChildren()->objectAtIndex(0))->getChildren());

    for (size_t i = 0; i < b->count(); i++)
    {
        auto c = reinterpret_cast<CCNode*>(b->objectAtIndex(i));

        c->setTag(i);
    }

    auto a = reinterpret_cast<CCNode*>(b->objectAtIndex(9));
    a->setParent(layer);

    menu->updateLayout();
    menu->setLayout(nullptr);

    auto men = reinterpret_cast<CCNode*>(layer->getChildren()->objectAtIndex(0));
    men->addChild(menu, 1);

    //HideOldOptions(layer); 
    //AddNewOptions(layer, categories);
}