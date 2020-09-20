#include "rlottie.h"
using namespace rlottie;

std::unique_ptr<Animation> anim;
float rValue = 0.0, gValue = 0.0, bValue = 0.0;

void setAnimation(char* path, size_t w, size_t h)
{
	anim = Animation::loadFromFile(path);
	anim->size(w, h);
}

Surface* renderRLottieAnimation(double pos)
{
	auto frameNum = anim->frameAtPos(pos);
	size_t width = 500;
	size_t height = 500;
	size_t bytesPerLine = width * sizeof(uint32_t);
	uint32_t* buffer = (uint32_t*)calloc(bytesPerLine * height, sizeof(uint32_t));
	rlottie::Surface surface(buffer, width, height, bytesPerLine);

	anim->renderSync(frameNum, surface);
	return &surface;
}

void setColor(float r, float g, float b)
{
	if (!anim) return;
	rValue = r;
	gValue = g;
	bValue = b;
	anim->setValue<Property::FillColor>("Layer1.Box 1.Fill1",
		[](const FrameInfo& info) {
			return Color(rValue, gValue, bValue);
		});
}