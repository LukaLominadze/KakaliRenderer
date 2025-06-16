#pragma once

#include "Layer.h"

class ImGuiLayer : public Layer
{
public:
	void OnAttach() override;
	void OnEvent(Event& event) override;
	void OnDetach() override;

	void Begin();
	void End();
private:
	bool OnMouseOnUI(MouseButtonPressedEvent& event);
};
