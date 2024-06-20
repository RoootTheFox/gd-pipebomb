#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/GJMessagePopup.hpp>
#include <Geode/modify/TextArea.hpp>

static std::string last_msg = "";

class $modify(TextArea) {
	static TextArea* create(gd::string str, char const* font, float scale, float width, cocos2d::CCPoint anchor, float lineHeight, bool disableColor) {
		auto ret = TextArea::create(str, font, scale, width, anchor, lineHeight, disableColor);
		last_msg = str;
		return ret;		
	}
};

class $modify(GJMessagePopup) {
	void loadFromGJMessage(GJUserMessage* msg) {
		log::info("load - msg {} last {}", msg->m_messageID, last_msg);
		GJMessagePopup::loadFromGJMessage(msg);

		auto text_area = getChildOfType<TextArea>(this->m_mainLayer, 0);
		if (!text_area) {
			log::error("text_area not found");
			return;
		}

		auto message = last_msg;
		if (message.find("pipebomb") != std::string::npos || message.find("pipe bomb") != std::string::npos) {
			log::info("pipe bomb :33");

			if (Mod::get()->getSettingValue<bool>("enable-sound")) {
				FMODAudioEngine::sharedEngine()->playEffect("rooot.pipebomb/pipebomb.ogg");
			}

			auto win_size = CCDirector::sharedDirector()->getWinSize();
			auto flashbang = CCLayerColor::create(ccColor4B{255, 255, 255, 255});

			flashbang->setZOrder(9999);
			flashbang->setContentSize(win_size);
			this->addChild(flashbang);

			flashbang->runAction(
				CCSequence::create(
					CCDelayTime::create(2.0f),
					CCFadeOut::create(2.f),
					CCCallFunc::create(flashbang, callfunc_selector(CCLayer::removeFromParent)),
					nullptr
				)
			);
		}
	}
};