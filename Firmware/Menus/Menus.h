#ifndef __MENUS_H
#define __MENUS_H

#include <stdint.h>

#define MENU_TYPE_NONE      -1
#define MENU_TYPE_PARENT    0
#define MENU_TYPE_FUNCTION  1

typedef struct
{
	char * MenuText;          //Text to display for the specific item
	int8_t MenuItemType;      //MENU_TYPE_PARENT   : This item has child items, pointer then points to next menu level
	                          //MENU_TYPE_FUNCTION : This item directly calls a function to do something, pointer is function pointer
	                          //MENU_TYPE_NONE     : This item marks the end of a list
	void * MenuActionPointer; //Pointer to child menu list or pointer to function to call for lowest level entries
	void *Parameter;          //Parameter to pass to function or NULL if no parameter
} MenuItemDescriptor;

extern const char *VSYS_Setting[4];

extern const MenuItemDescriptor TopMenu[];
extern const MenuItemDescriptor SetPowerMenu[];

void Menus_ShowMenu(MenuItemDescriptor *CurrentMenu);

#endif
