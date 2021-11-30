#include "menu.h"
#include "SSD1306.h"
#include "relay.h"
#include "Button.h"
#include "flash.h"

#define FLASH_START 0x08080000

extern Settings set_flash[3], set_edit;
extern Relay relay[3];
extern MenuItem* selectedItem;
extern uint8_t pickRelayNum;
const uint8_t itemsPerScreen = 6;
extern ScreenID screen;
const char txtOff[] = " OFF";
const char txtOn[] = " ON";
extern uint32_t rtc_hours, rtc_minutes, rtc_seconds;


MenuItem::MenuItem(const char* name, MenuItem* prev, void* arg, uint32_t argMax, MenuItem* parent)
{
	name_ = name;

	if (arg)
		argument = arg;

	argumentMax = argMax;

	if (prev)
	{
		prevItem = prev;
		prevItem->nextItem = this;
	}

	if (parent)
	{
		parentItem = parent;
		if (!parent->childItem)
			parent->childItem = this;
	}
}

const char* MenuItem::name()
{
	return name_;
}

MenuItem* MenuItem::next()
{
	return nextItem;
}

MenuItem* MenuItem::prev()
{
	return prevItem;
}

MenuItem* MenuItem::getChild()
{
	return childItem;
}

MenuItem* MenuItem::getParent(bool firstParent)
{
	MenuItem* parent;
	if (firstParent)
		parent = selectedItem->parentItem;
	else
		parent = selectedItem;
	
	for (; parent->prev(); parent = parent->prev()) ;
	return parent->parentItem;
}

MenuItem* MenuItem::getNthChild(uint8_t n)
{
	if (childItem)
	{
		MenuItem* p = childItem;
		for (uint8_t i = 0; (i < n) && p->nextItem; i++) 
			p = p->nextItem;
		return p;
	}
	else
		return 0;
}

void* MenuItem::arg()
{
	return argument;
}

uint32_t MenuItem::argMax()
{
	return argumentMax;
}

void drawMenu(MenuItem& head, uint8_t selectedNum)
{
	if (head.getChild())
	{
		static uint8_t scrolls = 0;
		SSD1306_COLOR color = White;
		MenuItem* child = 0;
		
		child = head.getChild();
		SSD1306_Fill(Black);
		
		if (selectedNum == 0)
			selectedItem = head.getChild();
		if (selectedNum < scrolls)
			scrolls = selectedNum;
		else if (selectedNum >= (scrolls + itemsPerScreen))
			scrolls = selectedNum + 1 - itemsPerScreen;

		child = head.getNthChild(scrolls);
		
		for (uint8_t i = scrolls; i < (itemsPerScreen + scrolls); i++)
		{
			color = (i == selectedNum) ? Black : White;
			SSD1306_WriteString8Bit(0, 10 * (i - scrolls), (uint8_t*)child->name(), Font_7x9, color);
			if (child->next() == 0)
				break;
			child = child->next();
		}
	}
	
	SSD1306_UpdateScreen();
}

void drawStatus()
{
	bool out = relay[2].getOutputState();
	SSD1306_Fill(Black);
	SSD1306_WriteString8Bit(out ? 15 : 12, 5, (uint8_t*)(out ? txtOn : txtOff), Font_7x9, Black);
	SSD1306_WriteNum8Bit(0, 26, __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetHour(RTC)), 2, Font_7x9, White);
	SSD1306_WriteChar8Bit(':', Font_7x9, White);
	SSD1306_WriteNum8Bit(17, 26, __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetMinute(RTC)), 2, Font_7x9, White);
	SSD1306_WriteChar8Bit(':', Font_7x9, White);
	SSD1306_WriteNum8Bit(34, 26, __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetSecond(RTC)), 2, Font_7x9, White);
	SSD1306_UpdateScreen();
}

void drawValueEdit(bool isBool)
{
	SSD1306_Fill(Black);
	uint32_t val = false;
	MenuItem* item = selectedItem;

	if (item)
	{
		if (item->arg())
		{
			if (isBool)
				val = *(uint8_t*)item->arg();
			else
				val = *(uint32_t*)item->arg();
		}

		if (isBool)
			SSD1306_WriteString8Bit(val ? 15 : 12, 20, (uint8_t*)(val ? txtOn : txtOff), Font_7x9, Black);
		else
			SSD1306_WriteNum16Bit(0, 15, val, 3, Font_16x26, Black);
	}

	SSD1306_UpdateScreen();
}

void returnToStatus()
{
	screen = STATUS;
	drawStatus();
}

void editValueCallback(bool increase, bool isBool)
{
	if (selectedItem)
	{
		if (isBool)
		{
			*(uint8_t*)selectedItem->arg() ^= 1;
		}
		else
		{
			uint32_t val = *(uint32_t*)selectedItem->arg();
			if ((val == 0) && !increase)
				val = selectedItem->argMax();
			else
				val = (val + increase * 2 - 1);
			if (val > selectedItem->argMax())
				val = 0;
			*(uint32_t*)selectedItem->arg() = val;
		}
		drawValueEdit(selectedItem->argMax() < 2);
	}
}

void callback10Hz()
{
//	set_flash[pickRelayNum] = set_edit;
//	FlashWrite(FLASH_START + sizeof(set_flash[0]) * pickRelayNum, (uint8_t*)&set_flash[pickRelayNum], sizeof(set_flash[0]));
	drawStatus();
	screen = STATUS;
}

void scrollMenu(MenuItem& head, uint32_t btNum)
{
	if (btNum ? (selectedItem->next() == 0) : (selectedItem == head.getChild()))
	{
		btNum ? selectedItem = head.getChild() : selectedItem = head.getNthChild(getLastItemNum(head));
		btNum ? drawMenu(head, 0) : drawMenu(head, getLastItemNum(head));
	}
	else
	{
		btNum ? (selectedItem = selectedItem->next()) : (selectedItem = selectedItem->prev());
		drawMenu(head, getCurrentItemNum());
	}
}

uint8_t getLastItemNum(MenuItem& head)
{
	uint8_t num = 0;
	MenuItem* selectedItem_ = selectedItem;
	for (selectedItem_ = head.getChild(); selectedItem_->next() != 0; num++)
	{
		selectedItem_ = selectedItem_->next();
	}
	return num;
}

uint8_t getCurrentItemNum()
{
	uint8_t num = 0;
	MenuItem* selectedItem_ = selectedItem;
	for (num = 0; selectedItem_->prev() != 0; num++)
	{
		selectedItem_ = selectedItem_->prev();
	}
	return num;
}

