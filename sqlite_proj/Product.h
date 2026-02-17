#pragma once
#include <string>
#include <vector>
#include <SDL3/SDL_render.h>
class Product
{
private:
	static std::vector<Product> products;
	static std::vector<Product> getProductsQuery(SDL_Renderer* renderer);

public:
	std::string name;
	SDL_Texture* image = nullptr;
	std::vector<std::string> tags;

	Product() = default;
	//image = nullptr если без фотки. В этом случае в приложении будет использована заглушка
	Product(std::string& name, SDL_Texture* image, std::vector<std::string>& tags);

	//Рендерер нужен, так как фото товаров хранятся в блобе. Для преобразования блоба в фото нужен рендерер
	static std::vector<Product> getProducts(SDL_Renderer* renderer);
	static void printProducts();

};

