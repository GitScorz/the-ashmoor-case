#include "level_loader.h"
#include <paths.h>
#include <iostream>
#include <cineris/logger.h>

// maybe move this to engine
namespace ashmoor {

    auto LevelLoader::load(const std::string& fileName) -> std::vector<std::string> {
        std::string fullPath = Paths::Levels + fileName + ".level";
        std::ifstream file(fullPath);

        if (!file.is_open()) {
            LOG_ERROR(cineris::log::LogChannel::Game, "Failed to open level file: " + fullPath);
            return {};
        }

        std::vector<std::string> lines;
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty() && !line.starts_with("//")) // ignroe comments
                lines.push_back(line);
        }

        LOG_DEBUG(cineris::log::LogChannel::Game, "Loaded level: \"" + fileName + "\" successfully");

        return lines;
    }

}