#pragma once
#include <vector>
#include "Product.h"
class ProductView
{
private:
	static std::vector<Product> products;
	static int callback(void* notUsed, int colCount, char** columns, char** colNames);
	static std::vector<Product> getProductsQuery();
public:
	static std::vector<Product> getProducts();
	static void printProducts();
};

