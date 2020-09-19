#pragma once

#include "resource.h"
#include <stdint.h>

void setAnimation(char* path, size_t w, size_t h);
void setColor(float r, float g, float b);
uint32_t* renderAnimation(double pos);