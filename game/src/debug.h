#pragma once
#include <cineris/application.h>

namespace ashmoor::debug {
	extern bool bInWireframeMode;
	auto setupDebugConsole() -> void;
	auto registerDebugBindings(cineris::input::InputManager& pInput) -> void;
}
