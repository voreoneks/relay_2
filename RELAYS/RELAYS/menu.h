#pragma once

#include "stdint.h"

class MenuItem
{
public:
	MenuItem(const char* name = 0, MenuItem* prev = 0, void* arg = 0, uint32_t argMax = 0, MenuItem* parent = 0);
	const char* name();
	MenuItem* next();
	MenuItem* prev();
	MenuItem* getChild();
	MenuItem* getParent(bool firstParent);
	MenuItem* getNthChild(uint8_t n);
	void* arg();
	uint32_t argMax();


private:
	const char* name_;
	uint8_t id = 0;
	bool isFolder = false;
	MenuItem* nextItem = 0, * prevItem = 0, * parentItem = 0, * childItem = 0;
	void* argument = 0;
	uint32_t argumentMax = 0;
};



	uint8_t getCurrentItemNum();
	uint8_t getLastItemNum(MenuItem& head);
	void scrollMenu(MenuItem& head, uint32_t btNum);
	void drawMenu(MenuItem& head, uint8_t selectedNum);
	void drawStatus();
	void drawValueEdit(bool isBool);
	void returnToStatus();
	void editValueCallback(bool increase, bool isBool);
	void callback10Hz();


	
