#include "rlottie.h"

typedef std::unique_ptr<rlottie::Animation> Animation;

Animation anim;
float rValue = 0.0, gValue = 0.0, bValue = 0.0;

void setAnimation(char* path)
{
	anim = rlottie::Animation::loadFromFile(path);
	size_t w, h;
	anim->size(w, h);
}

void setColor(float r, float g, float b)
{
	if (!anim) return;
	rValue = r;
	gValue = g;
	bValue = b;
	anim->setValue<rlottie::Property::FillColor>("Layer1.Box 1.Fill1",
		[](const rlottie::FrameInfo& info) {
			return rlottie::Color(rValue, gValue, bValue);
		});
}