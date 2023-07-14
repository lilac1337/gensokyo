#include "../../includes.hxx"

D3DState::D3DState(IDirect3DDevice9* d3dDevice) noexcept {
	this->device = d3dDevice;

	d3dDevice->CreateStateBlock(D3DSBT_ALL, &this->state);

	this->state->Capture();

	this->device->GetTransform(D3DTS_WORLD, &this->worldMat);
	this->device->GetTransform(D3DTS_VIEW, &this->viewMat);
	this->device->GetTransform(D3DTS_PROJECTION, &this->projMat);

	this->device->GetRenderState(D3DRS_SRGBWRITEENABLE, &this->srgb);
}

D3DState::~D3DState() noexcept {
	this->state->Release();
}

void D3DState::Restore() noexcept {
	this->device->SetTransform(D3DTS_WORLD, &this->worldMat);
	this->device->SetTransform(D3DTS_VIEW, &this->viewMat);
	this->device->SetTransform(D3DTS_PROJECTION, &this->projMat);

	this->device->SetRenderState(D3DRS_SRGBWRITEENABLE, this->srgb);

	this->state->Apply();
}

ImageTexture::ImageTexture(const LPDIRECT3DTEXTURE9 texture, const LPD3DXSPRITE sprite, const Vector2<float>& pos) noexcept {
	this->Texture = texture;
	this->Sprite = sprite;
	this->Pos = pos;
}

ImageTexture::~ImageTexture() noexcept {
	this->Sprite->Release();
	this->Texture->Release();
}

void ImageTexture::Draw() noexcept {
	this->Sprite->Begin(D3DXSPRITE_ALPHABLEND);

	D3DXVECTOR3 pos = D3DXVECTOR3(this->Pos.x, this->Pos.y, 0.0f);
	this->Sprite->Draw(this->Texture, nullptr, nullptr, &pos, 0xFFFFFFFF);

	this->Sprite->End();
}


void gensokyo::gui::renderer::drawText(std::string_view text, u16 x, u16 y, u16 width, u16 height, ID3DXFont* font, D3DCOLOR color, i32 flags) noexcept {
	RECT rect = { x, y, x + width, y + height };

	font->DrawTextA(nullptr, text.data(), -1, &rect, DT_NOCLIP | flags, color);
}

void gensokyo::gui::renderer::drawRectangleFilled(const Vector2<float>& pos, const Vector2<float>& size, const D3DCOLOR color) noexcept {
	vertices.push_back({ pos.x, pos.y, 0.5f, 1.f, color });
	vertices.push_back({ pos.x + size.x, pos.y, 0.5f, 1.f, color });
	vertices.push_back({ pos.x + size.x, pos.y + size.y, 0.5f, 1.f, color });
	vertices.push_back({ pos.x, pos.y + size.y, 0.5f, 1.f, color });

	indices.push_back(0u + vertCount);
	indices.push_back(1u + vertCount);
	indices.push_back(2u + vertCount);
	indices.push_back(2u + vertCount);
	indices.push_back(3u + vertCount);
	indices.push_back(0u + vertCount);

	vertCount += 4;
	idxCount += 6;

	++filledSquareCount;

	updateBuffers();
};

void gensokyo::gui::renderer::drawRectangleBordered(const Vector2<float>& pos, const Vector2<float>& size, const float thickness, const D3DCOLOR color) noexcept {
	drawRectangleFilled(pos, { size.x, thickness }, color);
	drawRectangleFilled(pos, { thickness, size.y }, color);
	drawRectangleFilled({ pos.x, pos.y + size.y }, { size.x + thickness, thickness }, color); // add thickness to size.x here or else there will be a gap in the bottom right
	drawRectangleFilled({ pos.x + size.x, pos.y }, { thickness, size.y }, color);
}

__forceinline void gensokyo::gui::renderer::setState() noexcept {
	/* THANK YOU SO MUCH DEX */

	// Set up the viewport.
	D3DVIEWPORT9 vp{};
	vp.Width = 1920u;
	vp.Height = 1080u;
	vp.MaxZ = 0.f;
	device->SetViewport(&vp);

	// Set up the orthographic projection matrix.
	// If you're wondering why the "0.5f"... it's because we have to map texels to pixels.
	// And if you're wondering why I didn't use GLM here it's because GLM ortho gives different Z values that DirectX can't read (Z is in range of -1...1 rather than 0...1).
	D3DMATRIX identity = { 1, 0, 0, 0,
0, 1, 0, 0,
0, 0, 1, 0,
0, 0, 0, 1 };

	const auto l = 0.5f;
	const auto r = 1920.f + 0.5f;
	const auto t = 0.5f;
	const auto b = 1080.f + 0.5f;
	D3DMATRIX  ortho = identity;
	ortho.m[0][0] = 2.f / (r - l);
	ortho.m[1][1] = 2.f / (t - b);
	ortho.m[3][0] = (l + r) / (l - r);
	ortho.m[3][1] = (t + b) / (b - t);

	device->SetTransform(D3DTS_WORLD, &identity);
	device->SetTransform(D3DTS_VIEW, &identity);
	device->SetTransform(D3DTS_PROJECTION, &ortho);

	// Set the new render state.
	device->SetPixelShader(nullptr);
	device->SetVertexShader(nullptr);
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	device->SetRenderState(D3DRS_ZWRITEENABLE, false);
	device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	device->SetRenderState(D3DRS_ZENABLE, false);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, true);
	device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
	device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_SCISSORTESTENABLE, true);
	device->SetRenderState(D3DRS_FOGENABLE, false);
	device->SetRenderState(D3DRS_RANGEFOGENABLE, false);
	device->SetRenderState(D3DRS_SPECULARENABLE, false);
	device->SetRenderState(D3DRS_STENCILENABLE, false);
	device->SetRenderState(D3DRS_CLIPPING, true);
	device->SetRenderState(D3DRS_LIGHTING, false);
	device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
}


void gensokyo::gui::renderer::renderFrame() noexcept {
	if (!vertexBuffer)
		return;

	if (gui::open)
		gui::think();

	setState();

	device->SetFVF(CUSTOMFVF);

	// select the vertex and index buffers to use
	device->SetStreamSource(0, vertexBuffer, 0, sizeof(vertex));
	device->SetIndices(indexBuffer);

	// draw squares
	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, filledSquareCount * 4, 0, filledSquareCount * 2);

	// render text
	for (const auto& text : texts)
		drawText(text.textText, text.textPos.x, text.textPos.y, 100, 14, timesNewRoman, text.textColor, text.flags);

	for (const auto& image : images)
		image->Draw();
}

void gensokyo::gui::renderer::updateBuffers() noexcept {
	// release our old buffers
	if (vertexBuffer)
		vertexBuffer->Release();

	if (indexBuffer)
		indexBuffer->Release();

	std::vector<vertex> newVertexBuffer{};
	std::vector<u16> newIndexBuffer{};

	// grab all vertices and indices from the maps
	for (const auto& vtx : vertices)
		newVertexBuffer.push_back(vtx);

	for (const auto& idx : indices)
			newIndexBuffer.push_back(idx);

	vertCount = newVertexBuffer.size();
	idxCount = newIndexBuffer.size();
	
	D3DCAPS9 caps;
	device->GetDeviceCaps(&caps);

	//util::logger::log(GNSKY_INFO, std::format("filledSquareCount * 2 = {}, deviceCaps = {}", filledSquareCount * 2, caps.MaxPrimitiveCount));

	device->CreateVertexBuffer(newVertexBuffer.size() * sizeof(vertex), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, CUSTOMFVF, D3DPOOL_DEFAULT, &vertexBuffer, nullptr);
	device->CreateIndexBuffer(newIndexBuffer.size() * sizeof(u16), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &indexBuffer, nullptr);

	// copy our new vertex and index buffers into the video memory
	void* pVoid;

	vertexBuffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, newVertexBuffer.data(), newVertexBuffer.size() * sizeof(vertex));
	vertexBuffer->Unlock();

	indexBuffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, newIndexBuffer.data(), newIndexBuffer.size() * sizeof(u16));
	indexBuffer->Unlock();
}

void gensokyo::gui::renderer::init() noexcept {
	D3DXCreateFontA(device, 14, 0, FW_NORMAL, 4, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Times New Roman", &timesNewRoman);
}

void gensokyo::gui::renderer::cleanup() noexcept {
	indexBuffer->Release();
	vertexBuffer->Release();

	filledSquareCount = 0;

	vertCount = 0;
	idxCount = 0;

	texts.clear();

	for (const auto& image : images)
		delete image;

	images.clear();

	vertices.clear();
	indices.clear();
}
