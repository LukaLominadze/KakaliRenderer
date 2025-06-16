#pragma once

#include <cstdint>
#include "Layer.h"
#include <vector>

class LayerStack
{
public:
	LayerStack() = default;
	~LayerStack() = default;

	bool StartUp();
	void ShutDown();

	void PushLayer(Layer* layer);
	void PushOverlay(Layer* layer);
	void PopLayer(Layer* layer);
	void PopOverlay(Layer* layer);

	inline std::vector<Layer*>::iterator begin() { return m_layerStack.begin(); }
	inline std::vector<Layer*>::iterator end() { return m_layerStack.end(); }
private:
	std::vector<Layer*> m_layerStack;

	uint32_t m_layerInsertIndex = 0;
};