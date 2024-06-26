#pragma once

#include <cstdint>

namespace sl
{

struct BackendInfo
{
	uint32_t m_maxTextureSize = 0;
	uint32_t m_maxFramebufferSize = 0;
	uint32_t m_maxFramebufferColorAttachmentCount = 0;
	uint32_t m_maxVertexUniformComponentCount = 0;
	uint32_t m_maxFragmentUniformComponentCount = 0;
	uint32_t m_maxUniformLocation = 0;
};

} // namespace sl