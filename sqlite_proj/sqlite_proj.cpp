#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sqlite3.h>
#include "DatabaseConnection.h"
#include "Product.h"
#include "ProductView.h"

int main()
{
    DatabaseConnection db = DatabaseConnection();
    auto products = ProductView::getProducts();
    ProductView::printProducts(products);

}
