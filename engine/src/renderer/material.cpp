#include <cineris/renderer/material.h>

namespace cineris::renderer {
	auto Material::bind() const -> void {
		if (!shader) return;

		shader->setVec3("objectColor", color);
		shader->setFloat("material.shininess", shininess);
		shader->setVec3("material.diffuse", color);

		if (albedo) {
			albedo->bind(0);
			shader->setInt("uTexture", 0);
		}
	}
}