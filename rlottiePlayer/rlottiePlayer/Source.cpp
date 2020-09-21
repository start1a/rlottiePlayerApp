#include "rlottie.h"
using namespace rlottie;

std::unique_ptr<Animation> anim;
uint32_t* buffer;
Surface surface;
size_t width = 500;
size_t height = 500;
size_t bytesPerLine = width * sizeof(uint32_t);

void initAnimation()
{
	buffer = (uint32_t*)calloc(bytesPerLine * height, sizeof(uint32_t));
}

void setAnimation(char* path, size_t w, size_t h)
{
	anim = Animation::loadFromFile(path);
	anim->size(w, h);
}

Surface* renderRLottieAnimation(unsigned int frameNum)
{
	surface = Surface(buffer, width, height, bytesPerLine);
	anim->renderSync(frameNum, surface);
	return &surface;
}

//unsigned int intervalAnimation()
//{
//	return anim->duration() / anim->frameRate() * 1000;
//}

size_t getTotalFrame()
{
	return anim->totalFrame();
}

bool isAnimNULL()
{
	return anim == NULL;
}