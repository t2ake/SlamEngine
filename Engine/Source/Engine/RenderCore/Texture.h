#pragma once

#include <cstdint>
#include <string>

namespace sl
{

class Texture
{
public:
	virtual ~Texture() = default;
	
	virtual uint32_t GetHeight() const = 0;
	virtual uint32_t GetWidth() const = 0;

	virtual void Bind(uint32_t slot) const = 0;
};

class Texture2D : public Texture
{
public:
	static Texture2D *Create(std::string path);

public:

};

class Texture3D : public Texture
{

};

class TextureCube : public Texture
{

};

} // namespace sl
