#pragma once

#include <utility>

namespace Input
{
	inline std::pair<float, float> MouseStartPos = { 0, 0 };
	inline std::pair<float, float> MousePosDelta = { 0, 0 };

	bool IsKeyPressed(int keycode);
	bool IsKeyReleased(int keycode);
	bool IsMouseButtonPressed(int button);
	std::pair<float, float> GetMousePosition();
	inline std::pair<float, float> GetMousePositionDelta() { return MousePosDelta; }
	float GetMouseX();
	float GetMouseY();
	void OnUpdate();
}