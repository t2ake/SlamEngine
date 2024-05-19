#pragma once

#include "Core/Defines.h"

namespace sl
{

class Texture2D;

struct Attachment
{
	Texture2D *pTexture;
	uint32_t point;
};

} // namespace sl
