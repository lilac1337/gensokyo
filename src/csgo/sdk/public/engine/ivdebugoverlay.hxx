#pragma once

#include "../../../../includes.hxx"
//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
//
//  cdll_int.h
//
// 4-23-98  
// JOHN:  client dll interface declarations
//

#ifndef IVDEBUGOVERLAY_H
#define IVDEBUGOVERLAY_H

#ifdef _WIN32
#pragma once
#endif


#define VDEBUG_OVERLAY_INTERFACE_VERSION	"VDebugOverlay004"

// When used as a duration by a server-side NDebugOverlay:: call, 
// causes the overlay to persist until the next server update.
#define NDEBUG_PERSIST_TILL_NEXT_SERVER (0.01023f)

class OverlayText_t;

class IVDebugOverlay
{
public:
	virtual void AddEntityTextOverlay(int ent_index, int line_offset, float duration, int r, int g, int b, int a, PRINTF_FORMAT_STRING const char* format, ...) = 0;
	virtual void AddBoxOverlay(const Vector3<float>& origin, const Vector3<float>& mins, const Vector3<float>& max, Vector3<float> const& orientation, int r, int g, int b, int a, float duration) = 0;
	virtual void AddSphereOverlay(const Vector3<float>& vOrigin, float flRadius, int nTheta, int nPhi, int r, int g, int b, int a, float flDuration) = 0;
	virtual void AddTriangleOverlay(const Vector3<float>& p1, const Vector3<float>& p2, const Vector3<float>& p3, int r, int g, int b, int a, bool noDepthTest, float duration) = 0;
	virtual void AddLineOverlay(const Vector3<float>& origin, const Vector3<float>& dest, int r, int g, int b,bool noDepthTest, float duration) = 0;
	virtual void AddTextOverlay(const Vector3<float>& origin, float duration, PRINTF_FORMAT_STRING const char* format, ...) FMTFUNCTION(4, 5) = 0;
	virtual void AddTextOverlay(const Vector3<float>& origin, int line_offset, float duration, PRINTF_FORMAT_STRING const char* format, ...) FMTFUNCTION(5, 6) = 0;
	virtual void AddScreenTextOverlay(float flXPos, float flYPos,float flDuration, int r, int g, int b, int a, const char* text) = 0;
	virtual void AddSweptBoxOverlay(const Vector3<float>& start, const Vector3<float>& end, const Vector3<float>& mins, const Vector3<float>& max, const Vector3<float>& angles, int r, int g, int b, int a, float flDuration) = 0;
	virtual void AddGridOverlay(const Vector3<float>& origin) = 0;
	virtual void AddCoordFrameOverlay(const void* frame, float flScale, int vColorTable[3][3] = NULL) = 0;

	virtual int ScreenPosition(const Vector3<float>& point, Vector3<float>& screen) = 0;
	virtual int ScreenPosition(float flXPos, float flYPos, Vector3<float>& screen) = 0;

	virtual OverlayText_t* GetFirst(void) = 0;
	virtual OverlayText_t* GetNext(OverlayText_t* current) = 0;
	virtual void ClearDeadOverlays(void) = 0;
	virtual void ClearAllOverlays() = 0;

	virtual void AddTextOverlayRGB(const Vector3<float>& origin, int line_offset, float duration, float r, float g, float b, float alpha, PRINTF_FORMAT_STRING const char* format, ...) FMTFUNCTION(9, 10) = 0;
	virtual void AddTextOverlayRGB(const Vector3<float>& origin, int line_offset, float duration, int r, int g, int b, int a, PRINTF_FORMAT_STRING const char* format, ...) FMTFUNCTION(9, 10) = 0;

	virtual void AddLineOverlayAlpha(const Vector3<float>& origin, const Vector3<float>& dest, int r, int g, int b, int a, bool noDepthTest, float duration) = 0;
	virtual void AddBoxOverlay2(const Vector3<float>& origin, const Vector3<float>& mins, const Vector3<float>& max, Vector3<float> const& orientation, const Color& faceColor, const Color& edgeColor, float duration) = 0;

	virtual void AddLineOverlay(const Vector3<float>& origin, const Vector3<float>& dest, int r, int g, int b, int a, float thickness, float duration) = 0;

	virtual void PurgeTextOverlays() = 0;

	virtual void AddCapsuleOverlay(const Vector3<float>& vStart, const Vector3<float>& vEnd, const float& flRadius, int r, int g, int b, int a, float flDuration) = 0;

private:
	inline void AddTextOverlay(const Vector3<float>& origin, int line_offset, float duration, int r, int g, int b, int a, PRINTF_FORMAT_STRING const char* format, ...) {} /* catch improper use of bad interface. Needed because '0' duration can be resolved by compiler to NULL format string (i.e., compiles but calls wrong function) */
};


#endif // IVDEBUGOVERLAY_H