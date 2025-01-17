#pragma once
#include "src/util/memory.h"

enum EFontDrawType : std::int32_t
{
	FONT_DRAW_DEFAULT = 0,
	FONT_DRAW_NONADDITIVE,
	FONT_DRAW_ADDITIVE
};

enum EFontFlag
{
	FONT_FLAG_NONE,
	FONT_FLAG_ITALIC = 0x001,
	FONT_FLAG_UNDERLINE = 0x002,
	FONT_FLAG_STRIKEOUT = 0x004,
	FONT_FLAG_SYMBOL = 0x008,
	FONT_FLAG_ANTIALIAS = 0x010,
	FONT_FLAG_GAUSSIANBLUR = 0x020,
	FONT_FLAG_ROTARY = 0x040,
	FONT_FLAG_DROPSHADOW = 0x080,
	FONT_FLAG_ADDITIVE = 0x100,
	FONT_FLAG_OUTLINE = 0x200,
	FONT_FLAG_CUSTOM = 0x400,
	FONT_FLAG_BITMAP = 0x800,
};

class ISurface
{
public:
	constexpr void DrawSetColor(int r, int g, int b, int a = 255) noexcept
	{
		memory::Call<void>(this, 15, r, g, b, a);
	}

	constexpr void DrawFilledRect(int x, int y, int xx, int yy) noexcept
	{
		memory::Call<void>(this, 16, x, y, xx, yy);
	}

	constexpr void DrawOutlinedRect(int x, int y, int xx, int yy) noexcept
	{
		memory::Call<void>(this, 18, x, y, xx, yy);
	}

	constexpr void DrawLine(int x, int y, int xx, int yy) noexcept
	{
		memory::Call<void>(this, 19, x, y, xx, yy);
	}

	constexpr void DrawSetTextFont(unsigned long font) noexcept
	{
		memory::Call<void>(this, 23, font);
	}

	constexpr void DrawSetTextColor(int r, int g, int b, int a = 255) noexcept
	{
		memory::Call<void>(this, 25, r, g, b, a);
	}

	constexpr void DrawSetTextPos(int x, int y) noexcept
	{
		memory::Call<void>(this, 26, x, y);
	}

	constexpr void DrawPrintText(const wchar_t* text, int textLen, EFontDrawType drawType = FONT_DRAW_DEFAULT) noexcept
	{
		memory::Call<void>(this, 28, text, textLen, drawType);
	}

	constexpr void DrawSetTextureRGBA(int id, const unsigned char* rgba, int wide, int tall) noexcept
	{
		memory::Call<void>(this, 37, id, rgba, wide, tall);
	}

	constexpr void DrawSetTexture(int id) noexcept
	{
		memory::Call<void>(this, 38, id);
	}

	constexpr void DrawTexturedRect(int x, int y, int xx, int yy) noexcept
	{
		memory::Call<void>(this, 41, x, y, xx, yy);
	}

	constexpr void UnlockCursor() noexcept
	{
		memory::Call<void>(this, 66);
	}

	constexpr void LockCursor() noexcept
	{
		memory::Call<void>(this, 67);
	}

	constexpr void SetCursorAlwaysVisible(bool visible) noexcept
	{
		memory::Call<void>(this, 68, visible);
	}

	constexpr void SetFontGlyphSet(unsigned long font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags) noexcept
	{
		memory::Call<void>(this, 72, font, windowsFontName, tall, weight, blur, scanlines, flags);
	}

	constexpr unsigned long CreateFont() noexcept
	{
		return memory::Call<unsigned long>(this, 71);
	}

	constexpr void GetTextSize(unsigned long font, const wchar_t* text, int& wide, int& tall) noexcept
	{
		memory::Call<void>(this, 79, font, text, wide, tall);
	}

	constexpr void DrawOutlinedCircle(int x, int y, int radius, int segments) noexcept
	{
		memory::Call<void>(this, 103, x, y, radius, segments);
	}

	constexpr void DrawFilledCircle(int x, int y, int radius, int segments) noexcept
	{
		memory::Call<void>(this, 104, x, y, radius, segments);
	}

	constexpr void DrawFilledRectFade(int x, int y, int xx, int yy, int alpha0, int alpha1, bool horizontal) noexcept
	{
		memory::Call<void>(this, 123, x, y, xx, yy, alpha0, alpha1, horizontal);
	}
	
};