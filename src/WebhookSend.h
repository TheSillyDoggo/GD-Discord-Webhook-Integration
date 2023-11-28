using namespace geode::prelude;

static inline LoadingCircle* loading = nullptr;

class Webhook
{
    public: 
        static inline gd::string webUrl = "";
        static inline geode::Notification* notif = nullptr;

        #pragma region testMSG

        static void SendTestMessage()
        {
            webUrl = Mod::get()->getSavedValue<std::string>("webhook-url");

            CCHttpRequest* request = new CCHttpRequest();
            request->setUrl(webUrl.c_str());
            request->setRequestType(CCHttpRequest::kHttpPost);

            //todo: get the mod on the geode sdk
            CCString* postData = CCString::createWithFormat("content=%s", "This is a test message for the Geometry Dash Discord webhook integration mod by TheSillyDoggo.\nThe mod is available for download at https://geode-sdk.org/mods/TheSillyDoggo.DiscordWebhook/");
            request->setRequestData(postData->getCString(), strlen(postData->getCString()));

            // Set callback function
            request->setResponseCallback(CCDirector::get()->getRunningScene(), httpresponse_selector(Webhook::onHttpRequestCompletedTest));

            // Send the request
            CCHttpClient::getInstance()->send(request);

            // Release the request object
            request->release();
        }

        void onHttpRequestCompletedTest(CCHttpClient* client, CCHttpResponse* response) {
            if (!response)
            {
                loading->fadeAndRemove();

                geode::createQuickPopup(
                    "Failed to send Test Message",
                    "<cr>Failure</c> sending the test message.\nYou can see the error msg by pressing the button",
                    "OK", nullptr, nullptr);

                return;
            }

            geode::createQuickPopup(
                "Sent Test Message",
                "<cg>Success</c> sending the test message, I <cl>think</c>?\nidk I can't tell go check yourself",
                "OK", nullptr, nullptr);

            loading->fadeAndRemove();
        }

        #pragma endregion

        static void SendDiscordMessage(gd::string msg)
        {
            std::string s = msg;

            if (s == "")
            {
                return;
            }

            webUrl = Mod::get()->getSavedValue<std::string>("webhook-url");

            if (PlayLayer::get() != nullptr)
            {
                s = replaceAll(s, "{name}", GetLevelName(PlayLayer::get()->m_level));
                s = replaceAll(s, "{creator}", GetLevelCreator(PlayLayer::get()->m_level));
                s = replaceAll(s, "{id}", GetLevelID(PlayLayer::get()->m_level));
                s = replaceAll(s, "{attempts}", GetLevelAttempts(PlayLayer::get()->m_level));
                s = replaceAll(s, "{difficulty}", GetLevelDifficulty(PlayLayer::get()->m_level));
                s = replaceAll(s, "{percentage}", GetPlayerPercentage());
            }
            //log::info(s);

            notif = geode::Notification::create("Sending message to Discord", NotificationIcon::Loading, 10);
            notif->show();

            CCHttpRequest* request = new CCHttpRequest();
            request->setUrl(webUrl.c_str());
            request->setRequestType(CCHttpRequest::kHttpPost);

            // Set the request data
            CCString* postData = CCString::createWithFormat("content=%s", s.c_str());
            request->setRequestData(postData->getCString(), strlen(postData->getCString()));

            // Set callback function
            request->setResponseCallback(CCDirector::get()->getRunningScene(), httpresponse_selector(Webhook::onHttpRequestCompleted));

            // Send the request
            CCHttpClient::getInstance()->send(request);

            // Release the request object
            request->release();
        }
            
        void onHttpRequestCompleted(CCHttpClient* client, CCHttpResponse* response) {
            if (!response)
            {
                notif->hide();
                notif = geode::Notification::create("Error sending message to Discord", NotificationIcon::Error, 0.5f);
                notif->show();

                return;
            }

            notif->hide();
            notif = geode::Notification::create("Successfully sent message to Discord", NotificationIcon::Success, 0.5f);
            notif->show();
        }

        static std::string replaceAll(std::string& str, const std::string& from, const std::string& to) {
            std::string str2 = str;

            if (str.find(to))
            {
                if(from.empty())
                    return str2;
                size_t start_pos = 0;
                while((start_pos = str2.find(from, start_pos)) != std::string::npos) {
                    str2.replace(start_pos, from.length(), to);
                    start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
                }
            }

            return str2;
        }

        static std::string GetLevelName(GJGameLevel* level)
        {
            return level->m_levelName;
        }

        static std::string GetLevelCreator(GJGameLevel* level)
        {
            if (std::strcmp(level->m_creatorName.c_str(), ""))
            {
                return "RobTop";
            }

            return level->m_creatorName;
        }

        static std::string GetLevelID(GJGameLevel* level)
        {
            std::stringstream st;
            st << "" << level->m_levelID.value();

            return st.str();
        }

        static std::string GetLevelAttempts(GJGameLevel* level)
        {
            std::stringstream st;
            st << "" << level->m_attempts.value();

            return st.str();
        }

        static std::string GetLevelDifficulty(GJGameLevel* level)
        {
            std::string str = "NA";

            switch (level->m_difficulty)
            {
                case GJDifficulty::Auto:
                    if (level->m_stars > 0)
                    {
                        str = "Auto";
                    }
                    else
                    {
                        str = "NA";
                    }
                    break;
                case GJDifficulty::Easy:
                    str = "Easy";
                    break;
                case GJDifficulty::Normal:
                    str = "Normal";
                    break;
                case GJDifficulty::Hard:
                    str = "Hard";
                    break;
                case GJDifficulty::Harder:
                    str = "Harder";
                    break;
                case GJDifficulty::Insane:
                    str = "Insane";
                    break;
                case GJDifficulty::Demon:
                    str = "Hard Demon";
                    break;
                case GJDifficulty::DemonEasy:
                    str = "Easy Demon";
                    break;
                case GJDifficulty::DemonMedium:
                    str = "Medium Demon";
                    break;
                case GJDifficulty::DemonInsane:
                    str = "Insane Demon";
                    break;
                case GJDifficulty::DemonExtreme:
                    str = "Extreme Demon";
                    break;
                
                default:
                    break;
            }

            return str;
        }

        static std::string GetPlayerPercentage()
        {
            std::stringstream st;
            st << "" << PlayLayer::get()->m_lastDeathPercent;

            return st.str();
        }
};