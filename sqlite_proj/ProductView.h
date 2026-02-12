#pragma once
#include <vector>
#include "Product.h"
class ProductView
{
public:
	static std::vector<Product> getProducts();
	static void printProducts(const std::vector<Product>& products);
};

