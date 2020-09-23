#include "rlottie.h"
using namespace rlottie;

std::unique_ptr<Animation> anim;
uint32_t* buffer;
size_t width, height;
size_t bytesPerLine;

void initAnimation(size_t w, size_t h)
{
	width = w;
	height = h;
	bytesPerLine = width * sizeof(uint32_t);
	buffer = (uint32_t*)calloc(bytesPerLine * height, sizeof(uint32_t));
}

void setAnimation(char* path, size_t w, size_t h)
{
	anim = Animation::loadFromFile(path);
	anim->size(w, h);
}

uint32_t* renderRLottieAnimation(uint32_t frameNum)
{
	static Surface surface = Surface(buffer, width, height, bytesPerLine);
	anim->renderSync(frameNum, surface);
	return buffer;
}

size_t getTotalFrame()
{
	return anim->totalFrame();
}

bool isAnimNULL()
{
	return anim == NULL;
}