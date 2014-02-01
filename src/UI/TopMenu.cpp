#include "TopMenu.h"
#include "Window.h"
#include "../Graphics.h"
#include "../Widget.h"
#include "../Data/CityInfo.h"
#include "../Data/Constants.h"
#include "../Data/Graphics.h"
#include "../Data/Screen.h"

static void menuFile_newGame(int param);
static void menuFile_replayMap(int param);
static void menuFile_loadGame(int param);
static void menuFile_saveGame(int param);
static void menuFile_deleteGame(int param);
static void menuFile_exitGame(int param);

static MenuItem menuFile[] = {
	{0, 1, menuFile_newGame, 0},
	{20, 2, menuFile_replayMap, 0},
	{40, 3, menuFile_loadGame, 0},
	{60, 4, menuFile_saveGame, 0},
	{80, 6, menuFile_deleteGame, 0},
	{100, 5, menuFile_exitGame, 0},
};

static MenuBarItem menu[] = {
	{10, 6, 1, menuFile, 6},
};

static int offsetFunds;
static int offsetPopulation;
static int offsetDate;

static void drawBackground();

void UI_TopMenu_draw()
{
	drawBackground();
	Widget_Menu_drawMenuBar(menu, 1);

	int width;
	Color treasureColor = Color_White;
	if (Data_CityInfo.treasury < 0) {
		treasureColor = Color_Red;
	}
	if (Data_Screen.width < 800) {
		offsetFunds = 338;
		offsetPopulation = 453;
		offsetDate = 547;
		
		width = Widget_GameText_drawColored(6, 0, 350, 5, Font_NormalPlain, treasureColor);
		Widget_Text_drawNumberColored(Data_CityInfo.treasury, '@', " ", 346 + width, 5, Font_NormalPlain, treasureColor);

		width = Widget_GameText_draw(6, 1, 458, 5, Font_SmallBrown);
		Widget_Text_drawNumber(Data_CityInfo.population, '@', " ", 450 + width, 5, Font_SmallBrown);

		width = Widget_GameText_draw(25, Data_CityInfo_Extra.gameTimeMonth, 552, 5, Font_SmallBrown);
		Widget_GameText_drawYearNoSpacing(Data_CityInfo_Extra.gameTimeYear, 541 + width, 5, Font_SmallBrown);
	} else if (Data_Screen.width < 1024) {
		offsetFunds = 338;
		offsetPopulation = 458;
		offsetDate = 652;
		
		width = Widget_GameText_drawColored(6, 0, 350, 5, Font_NormalPlain, treasureColor);
		Widget_Text_drawNumberColored(Data_CityInfo.treasury, '@', " ", 346 + width, 5, Font_NormalPlain, treasureColor);

		width = Widget_GameText_drawColored(6, 1, 470, 5, Font_NormalPlain, Color_White);
		Widget_Text_drawNumberColored(Data_CityInfo.population, '@', " ", 466 + width, 5, Font_NormalPlain, Color_White);

		width = Widget_GameText_drawColored(25, Data_CityInfo_Extra.gameTimeMonth, 655, 5, Font_NormalPlain, Color_Yellow);
		Widget_GameText_drawYearColored(Data_CityInfo_Extra.gameTimeYear, 655 + width, 5, Font_NormalPlain, Color_Yellow);
	} else {
		offsetFunds = 493;
		offsetPopulation = 637;
		offsetDate = 852;
		
		width = Widget_GameText_drawColored(6, 0, 495, 5, Font_NormalPlain, treasureColor);
		Widget_Text_drawNumberColored(Data_CityInfo.treasury, '@', " ", 501 + width, 5, Font_NormalPlain, treasureColor);

		width = Widget_GameText_drawColored(6, 1, 645, 5, Font_NormalPlain, Color_White);
		Widget_Text_drawNumberColored(Data_CityInfo.population, '@', " ", 651 + width, 5, Font_NormalPlain, Color_White);

		width = Widget_GameText_drawColored(25, Data_CityInfo_Extra.gameTimeMonth, 850, 5, Font_NormalPlain, Color_Yellow);
		Widget_GameText_drawYearColored(Data_CityInfo_Extra.gameTimeYear, 850 + width, 5, Font_NormalPlain, Color_Yellow);
	}
}

static void drawBackground()
{
	int blockWidth = 24;
	int graphicBase = GraphicId(ID_Graphic_TopMenuSidebar);
	for (int i = 0; i * blockWidth < Data_Screen.width; i++) {
		Graphics_drawImage(graphicBase + i % 8, i * blockWidth, 0);
	}
	// black panels for funds/pop/time
	if (Data_Screen.width < 800) {
		Graphics_drawImage(graphicBase + 14, 336, 0);
	} else if (Data_Screen.width < 1024) {
		Graphics_drawImage(graphicBase + 14, 336, 0);
		Graphics_drawImage(graphicBase + 14, 456, 0);
		Graphics_drawImage(graphicBase + 14, 648, 0);
	} else {
		Graphics_drawImage(graphicBase + 14, 480, 0);
		Graphics_drawImage(graphicBase + 14, 624, 0);
		Graphics_drawImage(graphicBase + 14, 840, 0);
	}
}

void UI_TopMenu_handleMouse()
{

}


static void menuFile_newGame(int param) {}
static void menuFile_replayMap(int param) {}
static void menuFile_loadGame(int param) {}
static void menuFile_saveGame(int param) {}
static void menuFile_deleteGame(int param) {}
static void menuFile_exitGame(int param) {}
