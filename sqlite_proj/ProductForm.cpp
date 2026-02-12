#include "imgui.h"
#include "ProductForm.h"
#include "ProductView.h"

void ProductForm::Show()
{
	for (const auto& product : ProductView::getProducts())
	{
		ImGui::Text("%s", product.name.c_str());
		for (const auto& tag : product.tags)
		{
			ImGui::Text("%s", tag.c_str());
			ImGui::SameLine();
		}
		ImGui::NewLine();
	}
}
