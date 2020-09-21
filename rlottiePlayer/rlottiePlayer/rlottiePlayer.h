#pragma once

#include "resource.h"
#include "rlottie.h"
#include <Commdlg.h>                        // OPENFILENAME
#include "atlconv.h"                             // String cast. ex) LPWSTR <-> LPSTR
#include <gdiplus.h>
#include <CommCtrl.h>						 // slider handle
using namespace rlottie;

// interval
#define UI_INTERVAL 20

// length
#define WND_WIDTH 1000
#define WND_HEIGHT 800
#define BTN_WIDTH 100
#define BTN_HEIGHT 30
#define TEXT_HEIGHT 20
#define SLIDER_HEIGHT 25
#define RDOBTN_WIDTH 60
#define RDOBTN_HEIGHT 20

void setAnimation(char* path, size_t w, size_t h);
void setColor(float r, float g, float b);
Surface* renderRLottieAnimation(UINT frameNum);
size_t getTotalFrame();

typedef struct RlottieBitmap
{
	Gdiplus::Bitmap* image = NULL;
	int x = 0;
	int y = 0;
	unsigned int width = 0;
	unsigned int height = 0;
}RlottieBitmap;