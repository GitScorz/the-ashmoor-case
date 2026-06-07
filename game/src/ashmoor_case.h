#pragma once
#include <cineris/application.h>
#include <cineris/renderer/skybox.h>
#include "world/world.h"
#include "controllers/player_controller.h"

namespace renderer = cineris::renderer;

namespace ashmoor {
	class AshmoorCase final : public cineris::Application {
	public:
		AshmoorCase();
		~AshmoorCase();

	protected:
		auto onInit() -> void override;
		auto onUpdate(double deltaTime) -> void override;
		auto onRender() -> void override;
		auto onShutdown() -> void override;
	private:
		std::unique_ptr<World> m_pWorld;
		std::unique_ptr<PlayerController> m_pPlayerController;
		std::unique_ptr<WorldObject> m_pDebugPlayerObj;
		
		renderer::Skybox m_Skybox;

		std::string m_debugMessage;
		double m_debugMessageTimer = 0.0;
	private:
		auto showDebugMessage(const std::string& message, double duration) -> void;
	};
}