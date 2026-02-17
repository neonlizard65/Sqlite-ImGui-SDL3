#include "GUIFonts.h"

ImFont* GUIFonts::fontMain = nullptr;
ImFont* GUIFonts::fontSecondary = nullptr;

ImVec4 GUIFonts::RGBtoImVec4(int r, int g, int b, int a) {
	return ImVec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}