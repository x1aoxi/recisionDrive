//
// Created by taseny on 25-7-22.
//

#include "menu_c_wrapper.h"

#ifdef __cplusplus
#include "menu_navigator.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

void* menu_builder(void* mainMenu)
{
    const auto menu = new Menu::Navigator(static_cast<Menu::menuItem*>(mainMenu));
    return menu;
}

void menu_delete(void* navigator)
{
    delete static_cast<Menu::Navigator*>(navigator);
}

void menu_handle_input(void* navigator, uint8_t key_value)
{
    const auto menu = static_cast<Menu::Navigator*>(navigator);
    switch (key_value)
    {
    case UP:
        menu->handleInput(Menu::keyValue::UP);
        break;
    case DOWN:
        menu->handleInput(Menu::keyValue::DOWN);
        break;
    case LEFT:
        menu->handleInput(Menu::keyValue::LEFT);
        break;
    case RIGHT:
        menu->handleInput(Menu::keyValue::RIGHT);
        break;
    default:
        menu->handleInput(Menu::keyValue::NONE);
    }
}

void menu_refresh_display(void* navigator)
{
    const auto menu = static_cast<Menu::Navigator*>(navigator);
    menu->refreshDisplay();
}

char* menu_get_display_buffer(void* navigator)
{
    const auto menu = static_cast<Menu::Navigator*>(navigator);
    return menu->getDisplayBuffer();
}

uint8_t menu_get_app_mode(void* navigator)
{
    const auto menu = static_cast<Menu::Navigator*>(navigator);
    return menu->getAppMode();
}

void menu_set_app_mode(void* navigator, uint8_t mode)
{
    const auto menu = static_cast<Menu::Navigator*>(navigator);
    menu->setAppMode(static_cast<bool>(mode));
}

void menu_force_refresh_display(void* navigator)
{
    const auto menu = static_cast<Menu::Navigator*>(navigator);
    menu->forceRefreshDisplay();
}

void menu_exhibition_next_page(void* navigator)
{
    const auto menu = static_cast<Menu::Navigator*>(navigator);
    menu->exhibitionNextPage();
}

void menu_exhibition_prev_page(void* navigator)
{
    const auto menu = static_cast<Menu::Navigator*>(navigator);
    menu->exhibitionPrevPage();
}

void menu_exhibition_reset_to_first_page(void* navigator)
{
    const auto menu = static_cast<Menu::Navigator*>(navigator);
    menu->exhibitionResetToFirstPage();
}

uint8_t menu_exhibition_get_current_page(void* navigator)
{
    const auto menu = static_cast<Menu::Navigator*>(navigator);
    return menu->getExhibitionCurrentPage();
}

uint8_t menu_exhibition_get_total_pages(void* navigator)
{
    const auto menu = static_cast<Menu::Navigator*>(navigator);
    return menu->getExhibitionTotalPages();
}

uint8_t menu_exhibition_is_pageable(void* navigator)
{
    const auto menu = static_cast<Menu::Navigator*>(navigator);
    return menu->isExhibitionPageable() ? 1 : 0;
}

const char* menu_get_current_page_name(void* navigator)
{
    const auto menu = static_cast<Menu::Navigator*>(navigator);
    return menu->getCurrentPageName();
}

const char* menu_get_current_selected_item_name(void* navigator)
{
    const auto menu = static_cast<Menu::Navigator*>(navigator);
    return menu->getCurrentSelectedItemName();
}

uint8_t menu_is_in_exhibition_mode(void* navigator)
{
    const auto menu = static_cast<Menu::Navigator*>(navigator);
    return menu->isInExhibitionMode() ? 1 : 0;
}

#ifdef __cplusplus
}
#endif
