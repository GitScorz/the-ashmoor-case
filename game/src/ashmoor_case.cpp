#include "ashmoor_case.h"
#include <cineris/core/resource_manager.h>
#include <cineris/core/level_loader.h>
#include "paths.h"
#include "debug.h"
#include <memory>
#include <array>
#include <cineris/core/logger.h>

namespace ashmoor {
	AshmoorCase::AshmoorCase() 
		: cineris::Application("Ashmoor Case") 
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
		playerMaterial.shader = resources().loadShader("lit_textured", Paths::Shaders + "lit_textured");
		playerMaterial.albedo = resources().loadTexture("black_texture", Paths::Textures + "black.png");
		playerMaterial.color = glm::vec3(1.0f, 0.5f, 0.31f);

		m_pDebugPlayerObj = std::make_unique<WorldObject>(playerMesh, playerMaterial, glm::vec3(0.0f));

		cineris::Scene scene;
		cineris::LevelLoader loader;

		const char* levelName = "test_level.json";
		if (!loader.load(Paths::Levels, levelName, scene)) {
			LOG_ERROR(cineris::log::LogChannel::Game, "Couldn't load level {}", levelName);
		}
		else {
			m_pWorld->loadScene(m_pPlayerController.get(), scene);
		}

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

		FogSettings fogSettings;
		fogSettings.color = glm::vec3(0.07f, 0.08f, 0.09f);
		fogSettings.density = 0.045f;
		fogSettings.start = 0.0f;
		fogSettings.end = 25.0f;

		RenderContext context;
		context.view = view;
		context.projection = projection;
		context.viewPos = camera().getPosition();
		context.lightPositions = m_pWorld->getLightPositions();
		context.lightColors = m_pWorld->getLightColors();
		context.lightColor = glm::vec3(1.0f, 0.98f, 0.19f);
		context.fog = fogSettings;
		context.ambientColor = glm::vec3(0.20f, 0.24f, 0.30f);
		context.ambientStrength = 0.18f;

		static bool logged = false;
		if (!logged && !context.lightPositions.empty()) {
			LOG_INFO(cineris::log::LogChannel::Game, "Lights loaded: {}", context.lightPositions.size());
			for (size_t i = 0; i < context.lightPositions.size(); ++i) {
				auto& pos = context.lightPositions[i];
				auto& col = context.lightColors[i];
				LOG_INFO(cineris::log::LogChannel::Game, "  Light {}: pos({}, {}, {}) color({}, {}, {})", 
					i, pos.x, pos.y, pos.z, col.r, col.g, col.b);
			}
			logged = true;
		}

		m_Skybox.draw(view, projection);

		m_pDebugPlayerObj->setPosition(m_pPlayerController->getPosition());
		m_pWorld->draw(context);
		m_pDebugPlayerObj->draw(context);

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