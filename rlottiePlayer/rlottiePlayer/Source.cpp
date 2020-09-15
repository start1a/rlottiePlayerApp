#include "rlottie.h"

typedef std::unique_ptr<rlottie::Animation> Animation;
Animation anim;

void setAnimation(char* path)
{
	anim = rlottie::Animation::loadFromFile(path);
	size_t w, h;
	anim->size(w, h);
}