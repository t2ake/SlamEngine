#pragma once

#include <imgui/imgui.h>

// From ImPlot
struct ScrollingBuffer
{
	ScrollingBuffer(int max_size = 1024)
	{
		m_maxSize = max_size;
		m_offset = 0;
		m_datas.reserve(m_maxSize);
	}
	void AddPoint(float x, float y)
	{
		if (m_datas.size() < m_maxSize)
		{
			m_datas.push_back(ImVec2(x, y));
		}
		else
		{
			m_datas[m_offset] = ImVec2(x, y);
			m_offset = (m_offset + 1) % m_maxSize;
		}
	}
	void Erase()
	{
		if (m_datas.size() > 0)
		{
			m_datas.shrink(0);
			m_offset = 0;
		}
	}

	int m_maxSize;
	int m_offset;
	ImVector<ImVec2> m_datas;
};
