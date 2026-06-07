#pragma once
#include <cineris/include.h>
#include <cineris/camera.h>
#include <cineris/input/input_manager.h>
#include <cineris/math/aabb.h>
#include "world/world.h"

namespace ashmoor {

	class PlayerController
	{
	public:
		float m_fHealth = 100.f, m_fSpeed = 0.f;

		PlayerController(cineris::Camera& camera, cineris::input::InputManager& input);
		~PlayerController();

		auto update(float fDeltaTime, World* pWorld) -> void;
		auto getAABBAt(glm::vec3 vPosition) -> AABB;

		auto setPosition(glm::vec3 vPos) -> void { m_vPosition = vPos; }
		auto getPosition() const -> glm::vec3 { return m_vPosition; }

		auto getVelocity() const -> glm::vec3 { return m_vVelocity; }
		auto setVelocity(glm::vec3 vVel) -> void { m_vVelocity = vVel; }

	private:
		cineris::Camera& m_Camera;
		cineris::input::InputManager& m_Input;

		glm::vec3 m_vPosition{};
		glm::vec3 m_vVelocity{};
	};

}