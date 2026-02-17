#pragma once
#include <vector>
#include <string>
#include <SDL3/SDL.h>
#include "Product.h"

class ProductView
{
private:
	static SDL_Texture* placeholder;
	static std::string pressedTag;
	static void RenderProducts(SDL_Renderer* renderer, std::vector<Product> products);
public:
	static void Show(SDL_Renderer* renderer);
};

