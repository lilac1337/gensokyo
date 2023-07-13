#pragma once

#include "../../includes.hxx"

enum ObjectType : u32 {
	Button,
	Slider
};

class MenuButton {
public:
	 MenuButton(void* option, const std::string& name, ObjectType objType);
	
	void* Option;
	std::string Name;
	ObjectType ObjType;
};

namespace gensokyo::gui {
	inline bool open = true;

	inline Vector2<float> position = { 400.f, 200.f };
	inline Vector2<float> size = { 1300.f, 750.f };

	inline Vector2<float> buttonSize = { 15.f, 15.f };
	inline Vector2<float> sliderSize = { 100.f, 15.f };

	inline ImageTexture* mikuTex = nullptr;

	constexpr inline u16 mikuHeight = 312;
	constexpr inline u16 mikuWidth = 177;

	inline float test = .5f;

	inline std::vector<MenuButton*> guiButtons{};

	void style() noexcept;

	void reset() noexcept;
	void handleOpenClose() noexcept;

	void think() noexcept;

	void drawGui() noexcept;
	void init() noexcept;
};