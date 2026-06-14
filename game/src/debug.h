#pragma once
#include <cineris/core/application.h>

namespace ashmoor::debug {
	extern bool bInWireframeMode;
	auto setupDebugConsole() -> void;
	auto registerDebugBindings(cineris::input::InputManager& pInput) -> void;
}
