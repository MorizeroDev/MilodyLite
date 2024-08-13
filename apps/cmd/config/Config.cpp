#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace Config {
std::string file;
std::string file1;
std::string file2;
std::string fileSrc;
std::string fileDst;
std::string fileConfig;

std::string output;

std::vector<std::string> envList;


std::map<std::string, std::string>* envCache = nullptr;

void generate() {
    auto ret = new std::map<std::string, std::string>();
    for (const auto& def: envList) {
        auto pos = def.find('=');
        if (pos != std::string::npos) {
            std::string key = def.substr(0, pos);
            std::string value = def.substr(pos + 1);
            ret->insert(std::pair(key, value));
        }
    }
    envCache = ret;
}

std::map<std::string, std::string> getEnv() {
    if (envCache == nullptr) {
        generate();
    }
    return *envCache;
}
} // namespace Config
