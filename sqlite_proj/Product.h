#pragma once
#include <string>
#include <vector>
class Product
{
public:
	std::string name;
	//image
	std::vector<std::string> tags;
	Product() = default;
	Product(std::string& name, std::vector<std::string>& tags);


};

