#include <algorithm>
#include <vector>
#include <string>
#include <imgui.h>
#include <SDL3_image/SDL_image.h>
#include "Product.h"
#include "ProductController.h"
#include "ProductView.h"
#include "GUIFonts.h"
#include <iterator>

SDL_Texture* ProductView::placeholder = nullptr;
std::string ProductView::pressedTag = "";

//Генерировать текст tag0, tag1, tag2... для того, чтобы у кнопок (тегов) были уникальные ID
static void generateImGuiID(char* elementName, int elementNameSize, int elementCount) {
	char elementID[10];
	strcpy_s(elementName, elementNameSize, "tag");
	sprintf_s(elementID, "%d", elementCount);
	strcat_s(elementName, elementNameSize, elementID);
}

void ProductView::RenderProducts(SDL_Renderer* renderer, std::vector<Product> products) {
	//Очистить выбранный тег
	if (!pressedTag.empty()) {
		ImGui::PushFont(GUIFonts::fontMain);
		ImGui::Text("Selected tag: %s", pressedTag.c_str());
		ImGui::PopFont();
		ImGui::PushFont(GUIFonts::fontSecondary);
		ImGui::PushStyleColor(ImGuiCol_Button, GUIFonts::RGBtoImVec4(214, 40, 40, 255));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, GUIFonts::RGBtoImVec4(247, 127, 0, 255));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, GUIFonts::RGBtoImVec4(252, 191, 73, 255));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 14.0f);
		if (ImGui::Button("Clear selected tag", ImVec2(300, 60))) {
			pressedTag = "";
		}
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopFont();
	}
	//Счетчики для ID элементов
	int imageCount = 0;
	int tagCount = 0;
	if (!ProductView::placeholder) {
		ProductView::placeholder = IMG_LoadTexture(renderer, "./placeholder.png");
	}
	for (const auto& product : products)
	{
		ImGui::PushFont(GUIFonts::fontMain);
		ImGui::TextColored(GUIFonts::RGBtoImVec4(255, 255, 255, 255), "%s", product.name.c_str());
		ImGui::PopFont();
		char imageIDText[16];
		ImGui::PushID(imageIDText);
		if (product.image) {
			ImGui::Image((ImTextureID)(intptr_t)product.image, ImVec2(800, 450));
		}
		else {
			ImGui::Image((ImTextureID)(intptr_t)placeholder, ImVec2(800, 450));
		}
		ImGui::PopID();
		char tagIDText[14];
		for (const auto& tag : product.tags)
		{
			generateImGuiID(tagIDText, 14, tagCount++); //4 для "tag\0" + 10 для числа
			ImGui::PushFont(GUIFonts::fontSecondary);
			ImGui::PushStyleColor(ImGuiCol_Button, GUIFonts::RGBtoImVec4(214, 40, 40, 255));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, GUIFonts::RGBtoImVec4(247, 127, 0, 255));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, GUIFonts::RGBtoImVec4(252, 191, 73, 255));
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 7.0f);
			ImGui::PushID(tagIDText);
			if (ImGui::Button(tag.c_str(), ImVec2(200, 40))) {
				pressedTag = tag.c_str();
			}
			ImGui::SetItemTooltip("Press to filter items with this tag");
			ImGui::PopID();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::SameLine();
			ImGui::PopFont();
		}
		ImGui::NewLine();
	}

}

void ProductView::Show(SDL_Renderer *renderer)
{
	std::vector<Product> products = Product::getProducts(renderer);
	if (pressedTag.empty()) {
		ProductView::RenderProducts(renderer, products);
	}
	else {
		std::vector<Product> result;
		//Лямбда в лямбде - вкусно
		auto productList = std::copy_if(products.begin(), products.end(), std::back_inserter(result), [](const Product& product) {
			return std::find_if(product.tags.begin(), product.tags.end(), [](const std::string& tag) {
				return tag == ProductView::pressedTag; }) != product.tags.end();
			});
		ProductView::RenderProducts(renderer, result);
	}

}
