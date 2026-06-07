#include "ashmoor_case.h"
#include <cineris/resource_manager.h>
#include "world/level_loader.h"
#include "paths.h"
#include "debug.h"
#include <memory>

// TODO: Logging system


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
		std::cout << "Initialized!" << std::endl;

		m_pWorld = std::make_unique<World>();
		m_pPlayerController = std::make_unique<PlayerController>(camera(), input());

		auto* shader = resources().getShader(Paths::Shaders + "text");
		textRenderer().loadFont(Paths::Fonts + "palr45w.ttf", 48, shader);

		renderer::Mesh* playerMesh = resources().getCubeMesh();
		renderer::Shader* playerShader = resources().getShader(Paths::Shaders + "lightning");
		renderer::Texture* playerTexture = resources().getTexture(Paths::Textures + "black.png");

		m_pDebugPlayerObj = std::make_unique<WorldObject>(playerMesh, playerShader, playerTexture, glm::vec3(0.0f, 0.0f, 0.0f));
		m_pDebugPlayerObj->setObjectColor(glm::vec3(1.0f, 0.5f, 0.31f));

		auto level = LevelLoader::load("ashmoor_entrance");
		m_pWorld->loadLevel(m_pPlayerController.get(), level);

		// binds
		debug::registerDebugBindings(input());

		input().registerKeyBinding(GLFW_KEY_H, []() {
			cineris::ResourceManager::get().reloadShaders();
		});
	}

	auto AshmoorCase::onUpdate(double deltaTime) -> void {
		m_pPlayerController->update(deltaTime, m_pWorld.get());

		const float sensivity = input().getMouseSensitivity();

		camera().rotate(
			input().getMouseDeltaX() * sensivity,
			input().getMouseDeltaY() * sensivity
		);
	}

	auto AshmoorCase::onRender() -> void {
		float windowWidth = window().m_iWidth;
		float windowHeight = window().m_iHeight;

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
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


		glDisable(GL_DEPTH_TEST);
		textRenderer().renderText("this is a test", 25.f, 50.f, 0.5f, glm::vec3(1.f));
		glEnable(GL_DEPTH_TEST);
	}

	auto AshmoorCase::onShutdown() -> void {
		m_pWorld.reset();
		m_pPlayerController.reset();
	}
}