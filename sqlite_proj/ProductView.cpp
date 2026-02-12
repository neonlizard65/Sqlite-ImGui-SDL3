#include <iostream>
#include <sqlite3.h>
#include "ProductView.h"
#include "DatabaseConnection.h"

std::vector<Product> ProductView::products = std::vector<Product>();

int ProductView::callback(void* notUsed, int colCount, char** columns, char** colNames)
{
    std::string productName;
    std::string tagName;

    for (int i = 0; i < colCount; i++)
    {

        if (strcmp(colNames[i], "ProductName") == 0) {
            productName = columns[i];
        }
        if (strcmp(colNames[i], "TagName") == 0) {
            tagName = columns[i];
        }
    }

    auto searchResult = std::find_if(ProductView::products.begin(), ProductView::products.end(), [&](const Product& product) {return product.name.c_str() == productName; });
    if (searchResult == ProductView::products.end()) {
        std::vector<std::string> tags;
        tags.reserve(8);
        tags.emplace_back(tagName);
        ProductView::products.emplace_back(productName, tags);
    }
    else {
        auto& product = *searchResult;
        product.tags.emplace_back(tagName);
    }

    return 0;
}

std::vector<Product> ProductView::getProductsQuery()
{
    std::string sql = "SELECT Product.Name AS ProductName, Product.Image AS ProductImage, Tag.Name AS TagName\
        FROM Product\
        INNER JOIN ProductTag ON Product.ID = ProductTag.ProductID\
        INNER JOIN Tag ON Tag.ID = ProductTag.TagID";

    char* messageError = nullptr;
    int result = sqlite3_exec(DatabaseConnection::getContext(), sql.c_str(), ProductView::callback, 0, &messageError);

    if (messageError != nullptr && result != SQLITE_OK) {
        std::cerr << "Error during query" << *messageError << "\n";
        sqlite3_free(messageError);
    }

    return products;
}

std::vector<Product> ProductView::getProducts()
{
    if (products.empty()) {
        return ProductView::getProductsQuery();
    }
    return products;
}


void ProductView::printProducts() {
    if (ProductView::products.empty()) {
        std::cout << "Empty product list passed\n";
        return;
    }
    for (const auto& product : ProductView::products) {
        std::cout << "Name: " << product.name << "\nTags:\n";

        for (const auto& tag : product.tags) {
            std::cout << "\t" << tag << "\n";
        }
    }
    std::cout << "\n";
}