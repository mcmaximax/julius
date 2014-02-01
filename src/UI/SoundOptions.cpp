#include "AllWindows.h"
#include "Window.h"
#include "../Util.h"
/*
#include "../Graphics.h"
#include "../Data/Graphics.h"
*/

#include "../Widget.h"

#include "../Data/Constants.h"
#include "../Data/Mouse.h"
#include "../Data/Screen.h"
#include "../Data/Settings.h"


static void buttonToggle(int param1, int param2);
static void buttonOk(int param1, int param2);
static void buttonCancel(int param1, int param2);

static void arrowButtonMusic(int param1, int param2);
static void arrowButtonSpeech(int param1, int param2);
static void arrowButtonEffects(int param1, int param2);
static void arrowButtonCity(int param1, int param2);

static CustomButton buttons[6] = {
	{
		64, 162, 288, 182,
		buttonToggle,
		Widget_Button_doNothing,
		1, 1, 0
	},
	{
		64, 192, 288, 212,
		buttonToggle,
		Widget_Button_doNothing,
		1, 2, 0
	},
	{
		64, 222, 288, 242,
		buttonToggle,
		Widget_Button_doNothing,
		1, 3, 0
	},
	{
		64, 252, 288, 272,
		buttonToggle,
		Widget_Button_doNothing,
		1, 4, 0
	},
	{
		144, 296, 336, 316,
		buttonOk,
		Widget_Button_doNothing,
		1, 1, 0
	},
	{
		144, 296, 336, 346,
		buttonCancel,
		Widget_Button_doNothing,
		1, 1, 0
	},
};

static ArrowButton arrowButtons[8] = {
	{112, 100, 17, 24, arrowButtonMusic, 1, 0},
	{136, 100, 15, 24, arrowButtonMusic, 0, 0},
	{112, 130, 17, 24, arrowButtonSpeech, 1, 0},
	{136, 130, 15, 24, arrowButtonSpeech, 0, 0},
	{112, 160, 17, 24, arrowButtonEffects, 1, 0},
	{136, 160, 15, 24, arrowButtonEffects, 0, 0},
	{112, 190, 17, 24, arrowButtonCity, 1, 0},
	{136, 190, 15, 24, arrowButtonCity, 0, 0},
};

static char original_soundEffectsEnabled;
static char original_soundMusicEnabled;
static char original_soundSpeechEnabled;
static char original_soundCityEnabled;
static int original_soundEffectsPercentage;
static int original_soundMusicPercentage;
static int original_soundSpeechPercentage;
static int original_soundCityPercentage;

static int focusButtonId;

void UI_SoundOptions_init()
{
	original_soundEffectsEnabled = Data_Settings.soundEffectsEnabled;
	original_soundMusicEnabled = Data_Settings.soundMusicEnabled;
	original_soundSpeechEnabled = Data_Settings.soundSpeechEnabled;
	original_soundCityEnabled = Data_Settings.soundCityEnabled;
	original_soundEffectsPercentage = Data_Settings.soundEffectsPercentage;
	original_soundMusicPercentage = Data_Settings.soundMusicPercentage;
	original_soundSpeechPercentage = Data_Settings.soundSpeechPercentage;
	original_soundCityPercentage = Data_Settings.soundCityPercentage;
}

void UI_SoundOptions_drawForeground()
{
	int baseOffsetX = Data_Screen.offset640x480.x;
	int baseOffsetY = Data_Screen.offset640x480.y;
	
	Widget_Panel_drawOuterPanel(
		baseOffsetX + 48, baseOffsetY + 80,
		24, 18
	);
	
	Widget_Panel_drawSmallLabelButton(3,
		baseOffsetX + 64, baseOffsetY + 162,
		14, 0, focusButtonId == 1 ? 1 : 2
	);
	Widget_Panel_drawSmallLabelButton(3,
		baseOffsetX + 64, baseOffsetY + 192,
		14, 0, focusButtonId == 2 ? 1 : 2
	);
	Widget_Panel_drawSmallLabelButton(3,
		baseOffsetX + 64, baseOffsetY + 222,
		14, 0, focusButtonId == 3 ? 1 : 2
	);
	Widget_Panel_drawSmallLabelButton(3,
		baseOffsetX + 64, baseOffsetY + 252,
		14, 0, focusButtonId == 4 ? 1 : 2
	);
	Widget_Panel_drawSmallLabelButton(3,
		baseOffsetX + 144, baseOffsetY + 296,
		12, 0, focusButtonId == 5 ? 1 : 2
	);
	Widget_Panel_drawSmallLabelButton(3,
		baseOffsetX + 144, baseOffsetY + 326,
		12, 0, focusButtonId == 6 ? 1 : 2
	);
	
	Widget_GameText_drawCentered(46, 0,
		baseOffsetX + 96, baseOffsetY + 92,
		288, Font_LargeBlack
	);
	
	Widget_GameText_drawCentered(46, 12,
		baseOffsetX + 128, baseOffsetY + 300,
		224, Font_SmallBrown
	);
	Widget_GameText_drawCentered(46, 9,
		baseOffsetX + 128, baseOffsetY + 330,
		224, Font_SmallBrown
	);
	
	Widget_GameText_draw(46, 10,
		baseOffsetX + 112, baseOffsetY + 142,
		Font_SmallPlain
	);
	Widget_GameText_draw(46, 11,
		baseOffsetX + 336, baseOffsetY + 142,
		Font_SmallPlain
	);
	
	Widget_GameText_drawCentered(46, Data_Settings.soundMusicEnabled ? 2 : 1,
		baseOffsetX + 64, baseOffsetY + 166,
		224, Font_SmallBrown
	);
	Widget_Text_drawNumber(Data_Settings.soundMusicPercentage, 64, "%",
		baseOffsetX + 374, baseOffsetY + 166,
		Font_NormalPlain
	);
	
	Widget_GameText_drawCentered(46, Data_Settings.soundSpeechEnabled ? 4 : 3,
		baseOffsetX + 64, baseOffsetY + 196,
		224, Font_SmallBrown
	);
	Widget_Text_drawNumber(Data_Settings.soundSpeechPercentage, 64, "%",
		baseOffsetX + 374, baseOffsetY + 196,
		Font_NormalPlain
	);
	
	Widget_GameText_drawCentered(46, Data_Settings.soundEffectsEnabled ? 6 : 5,
		baseOffsetX + 64, baseOffsetY + 226,
		224, Font_SmallBrown
	);
	Widget_Text_drawNumber(Data_Settings.soundEffectsPercentage, 64, "%",
		baseOffsetX + 374, baseOffsetY + 226,
		Font_NormalPlain
	);
	
	Widget_GameText_drawCentered(46, Data_Settings.soundCityEnabled ? 8 : 7,
		baseOffsetX + 64, baseOffsetY + 256,
		224, Font_SmallBrown
	);
	Widget_Text_drawNumber(Data_Settings.soundCityPercentage, 64, "%",
		baseOffsetX + 374, baseOffsetY + 256,
		Font_NormalPlain
	);

	Widget_Button_drawArrowButtons(
		baseOffsetX + 208, baseOffsetY + 60,
		arrowButtons, 8
	);

	// TODO: reset clip region() ?
}

void UI_SoundOptions_handleMouse()
{
	if (Data_Mouse.isRightClick) {
		// cancel dialog
		UI_Window_goTo(Window_City);
	} else {
		int baseOffsetX = Data_Screen.offset640x480.x;
		int baseOffsetY = Data_Screen.offset640x480.y;
		if (!Widget_Button_handleCustomButtons(
				baseOffsetX, baseOffsetY, buttons, 6, &focusButtonId)) {
			Widget_Button_handleArrowButtons(
				baseOffsetX + 208, baseOffsetY + 60, arrowButtons, 8);
		}
	}
}

static void buttonToggle(int param1, int param2)
{
	switch (param1) {
		case 1: // music
			Data_Settings.soundMusicEnabled = Data_Settings.soundMusicEnabled ? 0 : 1;
			if (Data_Settings.soundMusicEnabled) {
				// TODO start background music
			} else {
				// TODO stop background music
			}
			break;

		case 2: // speech
			Data_Settings.soundSpeechEnabled = Data_Settings.soundSpeechEnabled ? 0 : 1;
			if (!Data_Settings.soundSpeechEnabled) {
				// TODO stop talking persons
			}
			break;

		case 3: // effects
			Data_Settings.soundEffectsEnabled = Data_Settings.soundEffectsEnabled ? 0 : 1;
			break;

		case 4: // city
			Data_Settings.soundCityEnabled = Data_Settings.soundCityEnabled ? 0 : 1;
			break;
	}
}

static void buttonOk(int param1, int param2)
{
	UI_Window_goTo(Window_City);
}

static void buttonCancel(int param1, int param2)
{
	Data_Settings.soundEffectsEnabled = original_soundEffectsEnabled;
	Data_Settings.soundMusicEnabled = original_soundMusicEnabled;
	Data_Settings.soundSpeechEnabled = original_soundSpeechEnabled;
	Data_Settings.soundCityEnabled = original_soundCityEnabled;
	Data_Settings.soundEffectsPercentage = original_soundEffectsPercentage;
	Data_Settings.soundMusicPercentage = original_soundMusicPercentage;
	Data_Settings.soundSpeechPercentage = original_soundSpeechPercentage;
	Data_Settings.soundCityPercentage = original_soundCityPercentage;
	if (Data_Settings.soundMusicEnabled) {
		// TODO start background music
	} else {
		// TODO stop background music
	}
	// TODO set channel percentages

	UI_Window_goTo(Window_City);
}

static void arrowButtonMusic(int param1, int param2)
{
	if (param1 == 1) {
		Data_Settings.soundMusicPercentage--;
	} else if (param1 == 0) {
		Data_Settings.soundMusicPercentage++;
	}
	BOUND(Data_Settings.soundMusicPercentage, 0, 100);
	// TODO set channel percentage

	UI_Window_requestRefresh();
}

static void arrowButtonSpeech(int param1, int param2)
{
	if (param1 == 1) {
		Data_Settings.soundSpeechPercentage--;
	} else if (param1 == 0) {
		Data_Settings.soundSpeechPercentage++;
	}
	BOUND(Data_Settings.soundSpeechPercentage, 0, 100);
	// TODO set channel percentage

	UI_Window_requestRefresh();
}

static void arrowButtonEffects(int param1, int param2)
{
	if (param1 == 1) {
		Data_Settings.soundEffectsPercentage--;
	} else if (param1 == 0) {
		Data_Settings.soundEffectsPercentage++;
	}
	BOUND(Data_Settings.soundEffectsPercentage, 0, 100);
	// TODO set channel percentage

	UI_Window_requestRefresh();
}

static void arrowButtonCity(int param1, int param2)
{
	if (param1 == 1) {
		Data_Settings.soundCityPercentage--;
	} else if (param1 == 0) {
		Data_Settings.soundCityPercentage++;
	}
	BOUND(Data_Settings.soundCityPercentage, 0, 100);
	// TODO set channel percentage

	UI_Window_requestRefresh();
}

