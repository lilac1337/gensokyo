#pragma once

#include "../../includes.hxx"

#define CUSTOMFVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

// backing up our state
class D3DState {
public:
	 D3DState(IDirect3DDevice9* d3dDevice) noexcept;
	 ~D3DState() noexcept;

	 void Restore() noexcept;

private:
	IDirect3DDevice9* device;
	IDirect3DStateBlock9* state;
	D3DMATRIX             worldMat;
	D3DMATRIX             viewMat;
	D3DMATRIX             projMat;
	DWORD srgb;
};

class ImageTexture {
public:
	 ImageTexture(const LPDIRECT3DTEXTURE9 texture, const LPD3DXSPRITE sprite, const Vector2<float>& pos) noexcept;
	 ~ImageTexture() noexcept;

	 void Draw() noexcept;

private:
	LPDIRECT3DTEXTURE9 Texture;
	LPD3DXSPRITE Sprite;
	Vector2<float> Pos;
};

struct vertex {
	FLOAT x, y, z, rhw;    // from the D3DFVF_XYZRHW flag
	D3DCOLOR color;    // from the D3DFVF_DIFFUSE flag
};

struct text {
	std::string textText;
	Vector2<u16> textPos;
	D3DCOLOR textColor;
};

namespace gensokyo::gui::renderer {
	inline IDirect3DDevice9* device = **reinterpret_cast<IDirect3DDevice9***>(gensokyo::util::findPattern("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 0x1);

	inline ID3DXFont* timesNewRoman;

	inline LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
	inline LPDIRECT3DINDEXBUFFER9 indexBuffer;

	inline D3DState* prevState = nullptr;

	inline int menuImageWidth{};
	inline int menuImageHeight{};

	inline std::unordered_map <std::string, std::vector<vertex>> vertices;
	inline std::unordered_map <std::string, std::vector<std::size_t>> indices;

	inline std::vector<text> texts;
	inline std::vector<ImageTexture*> images;

	inline std::size_t vertCount{};
	inline std::size_t idxCount{};

	inline u8 filledSquareCount{};

	void drawText(std::string_view text, u16 x, u16 y, u16 width, u16 height, ID3DXFont* font, D3DCOLOR color) noexcept;
	void drawRectangleFilled(const Vector2<float>& pos, const Vector2<float>& size, const D3DCOLOR color, const std::string& key) noexcept;
	void drawRectangleBordered(const Vector2<float>& pos, const Vector2<float>& size, const float thickness, const D3DCOLOR color, const std::string& key) noexcept;

	__forceinline void setState() noexcept;
	void renderFrame() noexcept;

	void updateBuffers() noexcept;

	void init() noexcept;
	void cleanup() noexcept;
};
