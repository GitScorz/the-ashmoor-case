#include <cineris/renderer/material.h>

namespace cineris::renderer {
	auto Material::bind() const -> void {
		if (!shader) return;

		shader->use();

		shader->setVec3("objectColor", color);
		shader->setFloat("material.shininess", shininess);

		if (albedo) {
			albedo->bind(0);
			shader->setInt("material.diffuse", 0);
		}
	}
}