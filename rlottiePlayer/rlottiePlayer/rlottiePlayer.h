#pragma once

#include "resource.h"
#include "rlottie.h"
#include <Commdlg.h>                        // OPENFILENAME
#include "atlconv.h"                             // String cast. ex) LPWSTR <-> LPSTR
#include <gdiplus.h>
#include <CommCtrl.h>						 // slider handle
using namespace rlottie;

void setAnimation(char* path, size_t w, size_t h);
void setColor(float r, float g, float b);
Surface* renderRLottieAnimation(double pos);
size_t getTotalFrame();