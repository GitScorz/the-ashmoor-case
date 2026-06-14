#include "ashmoor_case.h"
#include <cineris/core/resource_manager.h>
#include "world/level_loader.h"
#include "paths.h"
#include "debug.h"
#include <memory>
#include <array>
#include <cineris/core/logger.h>

namespace ashmoor {
	AshmoorCase::AshmoorCase() 
		: cineris::Application("The Ashmoor Case") 
	{
		debug::setupDebugConsole();

		// Enable depth testing so terrain/objects render with correct occlusion
		glEnable(GL_DEPTH_TEST);
	}

	AshmoorCase::~AshmoorCase() = default;

	auto AshmoorCase::onInit() -> void {
		LOG_INFO(cineris::log::LogChannel::Game, "Application started!");

		m_pWorld = std::make_unique<World>();
		m_pPlayerController = std::make_unique<PlayerController>(camera(), input());

		auto* shader = resources().loadShader("main_text", Paths::Shaders + "text");
		textRenderer().loadFont(Paths::Fonts + "palr45w.ttf", 48, shader);

		auto* skyboxShader = resources().loadShader("skybox", Paths::Shaders + "skybox");

		std::array<std::string, 6> faces = {
			Paths::Textures + "skybox_faces/right.png",
			Paths::Textures + "skybox_faces/left.png",
			Paths::Textures + "skybox_faces/top.png",
			Paths::Textures + "skybox_faces/bottom.png",
			Paths::Textures + "skybox_faces/front.png",
			Paths::Textures + "skybox_faces/back.png"
		};

		m_Skybox.load(faces, skyboxShader);

		renderer::Mesh* playerMesh = resources().getCubeMesh();
		renderer::Material playerMaterial;
		playerMaterial.shader = resources().loadShader("lightning", Paths::Shaders + "lightning");
		playerMaterial.albedo = resources().loadTexture("black_texture", Paths::Textures + "black.png");
		playerMaterial.color = glm::vec3(1.0f, 0.5f, 0.31f);

		m_pDebugPlayerObj = std::make_unique<WorldObject>(playerMesh, playerMaterial, glm::vec3(0.0f));

		auto level = LevelLoader::load("ashmoor_entrance");
		m_pWorld->loadLevel(m_pPlayerController.get(), level);

		// binds
		debug::registerDebugBindings(input());

		input().registerKeyBinding(GLFW_KEY_H, [this]() {
			resources().reloadShaders();

			auto* textShader = resources().loadShader("main_text", Paths::Shaders + "text");
			textRenderer().setShader(textShader);

			showDebugMessage("Reloaded shaders", 2.0);
		});
	}

	auto AshmoorCase::onUpdate(double deltaTime) -> void {
		m_pPlayerController->update(deltaTime, m_pWorld.get());

		const float sensivity = input().getMouseSensitivity();

		camera().rotate(
			input().getMouseDeltaX() * sensivity,
			input().getMouseDeltaY() * sensivity
		);

		if (m_debugMessageTimer > 0.0) {
			m_debugMessageTimer -= deltaTime;
			if (m_debugMessageTimer <= 0.0) {
				m_debugMessage.clear();
				m_debugMessageTimer = 0.0;
			}
		}
	}

	auto AshmoorCase::onRender() -> void {
		float windowWidth = window().m_iWidth;
		float windowHeight = window().m_iHeight;

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view = camera().getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(70.0f), windowWidth / windowHeight, 0.1f, 100.0f);

		RenderContext context = {
		  view,
		  projection,
		  camera().getPosition(),
		  m_pWorld->getLightPositions(),
		  glm::vec3(1.0f, 1.0f, 1.0f),
		  FogSettings{
			true,
			glm::vec3(0.18f, 0.20f, 0.19f),
			0.08f,
			0.0f,
			25.0f
		  }
		};

		m_pDebugPlayerObj->setPosition(m_pPlayerController->getPosition());
		m_pWorld->draw(context);
		m_pDebugPlayerObj->draw(context);

		m_Skybox.draw(view, projection);

		// text rendering
		glDisable(GL_DEPTH_TEST);
		if (m_debugMessageTimer > 0.0 && !m_debugMessage.empty()) {
			textRenderer().renderText(m_debugMessage, 25.f, 50.f, 0.5f, glm::vec4(0.6f, 1.f, 0.6f, m_debugMessageTimer));
		}
		glEnable(GL_DEPTH_TEST);
	}

	auto AshmoorCase::onShutdown() -> void {
		m_pWorld.reset();
		m_pPlayerController.reset();
	}

	auto AshmoorCase::showDebugMessage(const std::string& message, double duration) -> void {
		m_debugMessage = message;
		m_debugMessageTimer = duration;
	}
}