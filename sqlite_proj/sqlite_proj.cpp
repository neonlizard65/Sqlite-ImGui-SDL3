#define SDL_MAIN_USE_CALLBACKS 1
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sqlite3.h>
#include <Windows.h>
#include <clocale>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include "DatabaseConnection.h"
#include "Product.h"
#include "ProductController.h"
#include "ProductView.h"
#include "GUIFonts.h"

#pragma execution_character_set("utf-8") //C C++20 обязательно для кириллицы

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

static int windowWidth = 1920;
static int windowHeight = 1080;

// Инициализация
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    SetConsoleOutputCP(CP_UTF8); //Для отображения русского языка в консоли
    SDL_SetAppMetadata("myStore", "0.0.1", "com.mystore");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("myStore", windowWidth, windowHeight, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    //??
    SDL_SetRenderLogicalPresentation(renderer, windowWidth, windowHeight, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    auto products = Product::getProducts(renderer);
    Product::printProducts();

    //ImGui
    IMGUI_CHECKVERSION(); //Проверка версии
    ImGui::CreateContext(); //Начинаем работу с ImGUI
    ImGui::StyleColorsDark(); //Задаем темный стиль по умолчанию

    //Привязка к SDL3
    if (!ImGui_ImplSDL3_InitForSDLRenderer(window, renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to initialize ImGui SDL3 backend, Error: %s", SDL_GetError());
        exit(1);
    }

    //Привязка к SDL3 Renderer
    if (!ImGui_ImplSDLRenderer3_Init(renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to initialize ImGui SDLRenderer3 backend, Error: %s", SDL_GetError());
        exit(1);
    }

    //Не забываем получить ввод/вывод для imgui
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; //Клавиатура
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  //Геймпад

    //Загрузите кастомный TTF файл
    io.Fonts->AddFontDefault();
    GUIFonts::fontMain = io.Fonts->AddFontFromFileTTF("TTSans.ttf", 48.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic()); // Шрифт, англ + русский, 48
    GUIFonts::fontSecondary = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 32.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic()); // Шрифт, англ + русский, 48

    //Если не удалось загрузить - используем умолчанию
    if (!GUIFonts::fontMain || !GUIFonts::fontSecondary) {
        std::cout << "Не удалось загрузить шрифт. Использую шрифт по умолчанию..." << std::endl;
    }
    // После добавления всех шрифтов, перед началом основного цикла:
    io.Fonts->Build(); // Перестраиваем атлас
    // Если используете SDL_Renderer, нужно заново создать текстуру:
    ImGui_ImplSDLRenderer3_CreateFontsTexture();

    return SDL_APP_CONTINUE; 
}

//Цикл событий
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    ImGui_ImplSDL3_ProcessEvent(event);
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS; //Успешное завершение программы
    }
    return SDL_APP_CONTINUE; 
}

//Цикл приложения
SDL_AppResult SDL_AppIterate(void* appstate)
{

    SDL_RenderClear(renderer);
    //Создание кадра
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_WindowBg] = GUIFonts::RGBtoImVec4(0, 48, 73, 255); // dark blue

    //Вставить сюда код для графического интерфейса ImGUI
    //Начинаем работать над окном ImGui
    static ImVec2 pos = ImVec2(0, 0); //Задаем позицию окна
    ImGui::SetNextWindowPos(pos);
    ImGui::SetNextWindowSize(ImGui::GetMainViewport()->Size); //Автоматически меняет размер окна под размер окна SDL
    ImGui::Begin("myStore", nullptr,
        ImGuiWindowFlags_NoCollapse |    // Убрать кнопку сворачивания
        ImGuiWindowFlags_NoDecoration |  // Убрать заголовок и рамку (опционально)
        ImGuiWindowFlags_NoMove |        // Запретить перемещение (ImGUI зафиксировано внутри окна SDL и перемещается с ним)
        ImGuiWindowFlags_NoResize);      // Пользователь не сможет поменять размер окна. 

    ProductView::Show(renderer);

    //Завершение работы над окном
    ImGui::End();

    //Рендер 
    ImGui::Render();
    SDL_RenderClear(renderer);
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

//Закрытие
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}
