using namespace geode::prelude;

float scale = 0.7f;


void ScaleBar(CCNode* createBar, CCNode* catMenu, CCNode* catBar, CCNode* togMenu, CCNode* togBar)
    {
        auto ccDirector = CCDirector::get();

            if (createBar != nullptr)
            {
                auto buttons = reinterpret_cast<CCNode*>(createBar->getChildren()->objectAtIndex(0));
                buttons->setAnchorPoint(CCPoint(0.5f, 0));
                buttons->setScale(scale);

                auto pages = reinterpret_cast<CCMenu*>(createBar->getChildren()->objectAtIndex(1));
                if (pages != nullptr)
                {
                    pages->setPosition(CCPoint(ccDirector->getWinSize().width / 2, 92 * 0.5f * scale));
                    pages->setContentSize(CCSize(0, 0));
                    pages->setAnchorPoint(CCPoint(0.5f, 0.5f));
                    pages->setScale(scale);
                    
                    auto left = reinterpret_cast<CCNode*>(pages->getChildren()->objectAtIndex(0));
                    if (left != nullptr)
                    {
                        left->setPosition(CCPoint(-1 * ((ccDirector->getWinSize().width / 2 / scale) - (catMenu->getContentSize().width) - (catBar->getContentSize().width) - (left->getContentSize().width)), 0));
                    }

                    auto right = reinterpret_cast<CCNode*>(pages->getChildren()->objectAtIndex(1));
                    if (right != nullptr)
                    {
                        right->setPosition(CCPoint((((ccDirector->getWinSize().width / 2 / scale) - (togMenu->getContentSize().width) - (togBar->getContentSize().width) - (right->getContentSize().width))), 0));
                    }
                }
            }
    }

void ScaleInit(LevelEditorLayer* la)
{
    auto ccDirector = CCDirector::get();

    auto EditorUI = la->getChildByID("EditorUI");
    auto editorUI = LevelEditorLayer::get()->m_editorUI;


    auto catMenu = EditorUI->getChildByID("toolbar-categories-menu");
    catMenu->setAnchorPoint(CCPoint {0, 0});
    catMenu->setPosition(CCPoint {6 * scale, 0});
    catMenu->setScale(scale);

    auto togMenu = EditorUI->getChildByID("toolbar-toggles-menu");
    togMenu->setAnchorPoint(CCPoint {1, 0});
    togMenu->setPosition(CCPoint {ccDirector->getWinSize().width - 6 * scale, 0});
    togMenu->setScale(scale);

    auto catBar = reinterpret_cast<CCNode*>(EditorUI->getChildren()->objectAtIndex(4));
    if (catBar != nullptr)
    {
        catBar->setPosition(CCPoint(85 * scale, 46 * scale));
        catBar->setScale(scale);
    }

    auto togBar = reinterpret_cast<CCNode*>(EditorUI->getChildren()->objectAtIndex(5));
    if (togBar != nullptr)
    {
        togBar->setPosition(CCPoint(ccDirector->getWinSize().width - (90 +  11) * scale, 46 * scale));
        togBar->setScale(scale);
    }

    auto backgroundFade = reinterpret_cast<CCNode*>(EditorUI->getChildren()->objectAtIndex(1));
    if (backgroundFade != nullptr)
    {
        backgroundFade->setScaleY(scale);
    }

    auto tabMenu = EditorUI->getChildByID("build-tabs-menu");
    tabMenu->setPositionY((100 * scale) - 1);
    tabMenu->setScale(scale);

    for (size_t i = 0; i < editorUI->m_createButtonBars->count(); i++)
    {
        auto createBar = reinterpret_cast<CCNode*>(editorUI->m_createButtonBars->objectAtIndex(i));

        ScaleBar(createBar, catMenu, catBar, togMenu, togBar);
    }

    ScaleBar(editorUI->m_editButtonBar, catMenu, catBar, togMenu, togBar);
    
    auto delMenu = EditorUI->getChildByID("delete-category-menu");
    delMenu->setAnchorPoint(CCPoint {0, 0});
    delMenu->setPositionY(46 * scale);
    delMenu->setScale(scale);

    auto setMenu = EditorUI->getChildByID("settings-menu");
    setMenu->setAnchorPoint(CCPoint {1, 1});
    setMenu->setPosition(CCPoint(ccDirector->getWinSize().width - 6 * scale, ccDirector->getWinSize().height - 6 * scale));
    setMenu->setScale(scale);

    auto layMenu = EditorUI->getChildByID("layer-menu");
    layMenu->setAnchorPoint(CCPoint {1, 1});
    layMenu->setScale(scale);

    auto butMenu = EditorUI->getChildByID("editor-buttons-menu");
    butMenu->setAnchorPoint(CCPoint {1, 1});
    butMenu->setPosition(CCPoint(ccDirector->getWinSize().width - 6 * scale, ccDirector->getWinSize().height / 2 + (butMenu->getContentSize().height * scale) / 2 + (layMenu->getContentSize().height)));
    butMenu->setScale(scale);

    layMenu->setPosition(CCPoint(ccDirector->getWinSize().width - 6 * scale, butMenu->getPositionY() - (butMenu->getContentSize().height / 2 * scale) - (layMenu->getContentSize().height * scale * 2) - (12 * scale)));

    auto undoMenu = EditorUI->getChildByID("undo-menu");
    undoMenu->setAnchorPoint(CCPoint {0, 1});
    undoMenu->setPosition(CCPoint(6 * scale, ccDirector->getWinSize().height - 6 * scale));
    undoMenu->setScale(scale);

    auto playbackMenu = EditorUI->getChildByID("playback-menu");
    playbackMenu->setAnchorPoint(CCPoint {0, 1});
    playbackMenu->setPosition(CCPoint(6 * scale, ccDirector->getWinSize().height - (undoMenu->getContentSize().height * scale) - (6 * scale) - (6 * scale)));
    playbackMenu->setScale(scale);

    auto playtestMenu = EditorUI->getChildByID("playtest-menu");
    playtestMenu->setAnchorPoint(CCPoint {0, 1});
    playtestMenu->setPosition(CCPoint(6 * scale, ccDirector->getWinSize().height - (undoMenu->getContentSize().height * scale) - (playbackMenu->getContentSize().height * scale) - (6 * scale) - (6 * scale) - (6 * scale)));
    playtestMenu->setScale(scale);

    auto zoomMenu = EditorUI->getChildByID("zoom-menu");
    zoomMenu->setAnchorPoint(CCPoint {0, 1});
    zoomMenu->setPosition(CCPoint(6 * scale, ccDirector->getWinSize().height - (undoMenu->getContentSize().height * scale) - (playbackMenu->getContentSize().height * scale) - (54 * scale)));
    zoomMenu->setScale(scale);

    auto linkMenu = EditorUI->getChildByID("link-menu");
    linkMenu->setAnchorPoint(CCPoint {0, 1});
    linkMenu->setPosition(CCPoint((zoomMenu->getContentSize().width * scale) + (12 * scale), ccDirector->getWinSize().height - (undoMenu->getContentSize().height * scale) - (playbackMenu->getContentSize().height * scale) - (54 * scale)));
    linkMenu->setScale(scale);        
}