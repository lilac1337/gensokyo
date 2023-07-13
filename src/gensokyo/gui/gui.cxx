#include "../../includes.hxx"

MenuButton::MenuButton(void* option, const std::string& name, ObjectType objType) {
	this->Name = name;
	this->ObjType = objType;
	this->Option = option;

	gensokyo::gui::guiButtons.push_back(this);
}

void gensokyo::gui::style() noexcept {
	new MenuButton(&cheats::options::movement::bhopEnabled, "bhop", Button);
	new MenuButton(&cheats::options::visuals::radarEnabled, "radar", Button);
	new MenuButton(&cheats::options::visuals::alwaysShowCrosshair, "always show xhair", Button);
	new MenuButton(&cheats::options::movement::optiEnabled, "opti", Button);
	new MenuButton(&cheats::options::movement::optiPercent, "opti pwr", Slider);
	new MenuButton(&cheats::options::movement::alignedDir, "aligned dir", Button);
	new MenuButton(&cheats::options::movement::useAtan, "use arctan", Button);
	new MenuButton(&cheats::options::movement::useSidemoveDir, "sidemove dir", Button);
	new MenuButton(&cheats::options::movement::antiWall, "antiwall", Button);
	new MenuButton(&cheats::options::movement::forceLegalYaw, "force legal yaw values", Button);
}

void gensokyo::gui::reset() noexcept {
	for (const auto& button : guiButtons)
		delete (button);

	guiButtons.clear();

	renderer::cleanup();
}

void gensokyo::gui::handleOpenClose() noexcept {
	if (!open)
		drawGui();
	else {
		reset();

		csgo::interfaces::InputSystem->EnableInput(true);
	}
	
	open = !open;
}

void gensokyo::gui::think() noexcept {
	i32 cursorX, cursorY;
	csgo::interfaces::InputSystem->GetCursorPosition(&cursorX, &cursorY);

	if (GetAsyncKeyState(VK_LBUTTON) & 1) {
		for (size_t i = 0; i < guiButtons.size(); ++i) {
			const MenuButton* curButton = guiButtons.at(i);

			if (curButton->ObjType != Button)
				continue;

			const Vector2<float> buttonPos = { position.x + 20.f, position.y + (20.f * (i + 1)) };

			if ((cursorX > buttonPos.x && buttonPos.x + buttonSize.x > cursorX)
				&& (cursorY > buttonPos.y && buttonPos.y + buttonSize.y > cursorY)) {
				*static_cast<bool*>(curButton->Option) = !*static_cast<bool*>(curButton->Option);

				reset();
				drawGui();

				return;
			}
		}
	}

	if (GetAsyncKeyState(VK_LBUTTON)) {
		for (size_t i = 0; i < guiButtons.size(); ++i) {
			const MenuButton* curButton = guiButtons.at(i);

			if (curButton->ObjType != Slider)
				continue;

			const Vector2<float> buttonPos = { position.x + 20.f, position.y + (20.f * (i + 1)) };

			if ((cursorX > buttonPos.x - 1.f && buttonPos.x + sliderSize.x + 1.f > cursorX)
				&& (cursorY > buttonPos.y - 1.f && buttonPos.y + sliderSize.y + 1.f > cursorY)) {
				*static_cast<float*>(curButton->Option) = (cursorX - buttonPos.x) / sliderSize.x;

				reset();
				drawGui();

				return;
			}
		}
	}
}

// FIXME: certain combinations of buttons and sliders break this? wtf??
 void gensokyo::gui::drawGui() noexcept {
	csgo::interfaces::InputSystem->EnableInput(false);

	renderer::drawRectangleFilled(position, size, D3DCOLOR_XRGB(33, 33, 33), "gui");
	renderer::drawRectangleBordered(position + 10.f, size - 23.f, 3.f, D3DCOLOR_XRGB(75, 173, 176), "gui");

	constexpr Vector2<float> buttonPadding = { 1.f, -1.f };
	constexpr Vector2<float> textPadding = { 5.f, 2.5f };

	style();

	for (size_t i = 0; i < guiButtons.size(); ++i) {
		Vector2<float> buttonPos = { position.x + 20.f, position.y + (20.f * (i + 1)) };

		const MenuButton* curButton = guiButtons.at(i);

		switch (curButton->ObjType) {
		case Button: {
			renderer::drawRectangleBordered(buttonPos, buttonSize, 1.f, D3DCOLOR_XRGB(255, 255, 255), std::format("guiButton{}", i));

			if (*static_cast<bool*>(curButton->Option))
				renderer::drawRectangleFilled(buttonPos + buttonPadding.x, buttonSize + buttonPadding.y, D3DCOLOR_XRGB(75, 173, 176), std::format("guiButton{}", i));

			text buttonText = { curButton->Name, { buttonPos.x + buttonSize.x + textPadding.x, buttonPos.y + textPadding.y },  D3DCOLOR_XRGB(255, 255, 255) };
			renderer::texts.push_back(buttonText);

			break;
		}
		case Slider: {
			renderer::drawRectangleBordered(buttonPos, sliderSize, 1.f, D3DCOLOR_XRGB(255, 255, 255), "gui");

			renderer::drawRectangleFilled({ buttonPos.x + (85.f * *static_cast<float*>(curButton->Option)) + 1.f, buttonPos.y + 1.f }, {14.f, 14.f}, D3DCOLOR_XRGB(75, 173, 176), "gui");

			text buttonText = { std::format("{} : {:.0f}%", curButton->Name, *static_cast<float*>(curButton->Option) * 100.f), {buttonPos.x + sliderSize.x + 5.f, buttonPos.y + 2.5f},  D3DCOLOR_XRGB(255, 255, 255)};
			renderer::texts.push_back(buttonText);

			break;
		}
		}
		
	}

	LPDIRECT3DTEXTURE9 texture;
	LPD3DXSPRITE sprite;

	D3DXCreateTextureFromFileInMemory(renderer::device, &miku, sizeof(miku), &texture);
	D3DXCreateSprite(renderer::device, &sprite);

	Vector2<float> mikuPos = { position.x + (size.x / 2.f) - 128.f, position.y + size.y - 525.f };

	mikuTex = new ImageTexture(texture, sprite, mikuPos);

	renderer::images.push_back(mikuTex);
}

void gensokyo::gui::init() noexcept {
	drawGui();
}

