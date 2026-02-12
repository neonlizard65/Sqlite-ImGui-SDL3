#include <iostream>
#include <sqlite3.h>
#include "ProductView.h"
#include "DatabaseConnection.h"

static std::vector<Product> products;

int callback(void* notUsed, int colCount, char** columns, char** colNames)
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

    auto searchResult = std::find_if(products.begin(), products.end(), [&](const Product& product) {return product.name.c_str() == productName; });
    if (searchResult == products.end()) {
        std::vector<std::string> tags;
        tags.reserve(8);
        tags.emplace_back(tagName);
        products.emplace_back(productName, tags);
    }
    else {
        auto& product = *searchResult;
        product.tags.emplace_back(tagName);
    }

    return 0;
}

std::vector<Product> ProductView::getProducts()
{
    std::string sql = "SELECT Product.Name AS ProductName, Product.Image AS ProductImage, Tag.Name AS TagName\
        FROM Product\
        INNER JOIN ProductTag ON Product.ID = ProductTag.ProductID\
        INNER JOIN Tag ON Tag.ID = ProductTag.TagID";

    char* messageError;
    int result = sqlite3_exec(DatabaseConnection::getContext(), sql.c_str(), callback, 0, &messageError);

    if (result != SQLITE_OK) {
        std::cerr << "Error during query" << *messageError << "\n";
        sqlite3_free(messageError);
    }

    return products;
}


void ProductView::printProducts(const std::vector<Product>& products) {
    if (products.empty()) {
        std::cout << "Empty product list passed\n";
        return;
    }
    for (const auto& product : products) {
        std::cout << "Name: " << product.name << "\nTags:\n";

        for (const auto& tag : product.tags) {
            std::cout << "\t" << tag << "\n";
        }
    }
    std::cout << "\n";
}