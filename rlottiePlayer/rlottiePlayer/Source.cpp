#include "rlottie.h"

void TestRlottie()
{
	auto animation = rlottie::Animation::loadFromFile("anubis.json");
	size_t w, h;
	animation->size(w, h);
}