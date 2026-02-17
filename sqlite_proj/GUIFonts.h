#pragma once
#include <imgui_impl_sdlrenderer3.h>
class GUIFonts
{
public:
	static ImFont* fontMain;
	static ImFont* fontSecondary;
	static ImVec4 RGBtoImVec4(int r, int g, int b, int a);
};

