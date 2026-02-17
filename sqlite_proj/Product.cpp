#include <iostream>
#include <sqlite3.h>
#include <vector>
#include <string>
#include <algorithm>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "DatabaseConnection.h"
#include "Product.h"

std::vector<Product> Product::products{};

std::vector<Product> Product::getProductsQuery(SDL_Renderer* renderer)
{
    products.clear();
    std::string sql = "SELECT Product.Name AS ProductName, Product.Image AS ProductImage, Tag.Name AS TagName\
        FROM Product\
        INNER JOIN ProductTag ON Product.ID = ProductTag.ProductID\
        INNER JOIN Tag ON Tag.ID = ProductTag.TagID";

    sqlite3_stmt* stmt;
    int prepareResult = sqlite3_prepare_v2(DatabaseConnection::getContext(), sql.c_str(), -1, &stmt, NULL);

    if (prepareResult == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string productName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));            //Loading blob
            SDL_Texture* productImage = nullptr;
            std::string productTag = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            auto productExists = std::find_if(products.begin(), products.end(), [&productName](const Product& product) { return product.name == productName; });
            if (productExists == products.end()) {
                const void* productImageBlob = sqlite3_column_blob(stmt, 1);
                int blobSize = sqlite3_column_bytes(stmt, 1);
                SDL_IOStream* io = SDL_IOFromConstMem(productImageBlob, blobSize);
                if (io) {
                    productImage = IMG_LoadTexture_IO(renderer, io, 1);
                }
                if (!productImage) {
                    SDL_Log("Failed to load texture from blob: %s", SDL_GetError());
                }
                std::vector<std::string> tags{ productTag };
                products.emplace_back(productName, productImage, tags);
            }
            else {
                (*productExists).tags.emplace_back(productTag);
            }

        }
    }
    else {
        std::cerr << "Error during query" << sqlite3_errmsg(DatabaseConnection::getContext()) << "\n";
    }

    sqlite3_finalize(stmt);
    return products;
}

std::vector<Product> Product::getProducts(SDL_Renderer* renderer)
{
    if (products.empty()) {
        return Product::getProductsQuery(renderer);
    }
    return products;
}


void Product::printProducts() {
    if (Product::products.empty()) {
        std::cout << "Empty product list passed\n";
        return;
    }
    for (const auto& product : Product::products) {
        std::cout << "Name: " << product.name << "\nTags:\n";

        for (const auto& tag : product.tags) {
            std::cout << "\t" << tag << "\n";
        }
    }
    std::cout << "\n";
}
Product::Product(std::string& name, SDL_Texture* image, std::vector<std::string>& tags) : name(name), tags(tags), image(image)
{
}
