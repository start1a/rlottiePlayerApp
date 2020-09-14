#include "rlottie.h"
#include "rlottiePlayer.h"

void TestRlottie(LPSTR path)
{
	auto animation = rlottie::Animation::loadFromFile(path);
	size_t w, h;
	animation->size(w, h);
}