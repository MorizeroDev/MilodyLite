#ifndef MILODYCLI_CONFIG_CONFIG_H
#define MILODYCLI_CONFIG_CONFIG_H

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace Config {
extern std::string file;
extern std::string file1;
extern std::string file2;
extern std::string fileSrc;
extern std::string fileDst;
extern std::string fileConfig;

extern std::string output;

extern std::vector<std::string> envList;
std::map<std::string, std::string> getEnv();
} // namespace Config

#endif //MILODYCLI_CONFIG_CONFIG_H
