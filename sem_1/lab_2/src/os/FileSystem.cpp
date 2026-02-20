#include "../../include/os/FileSystem.h"
FileSystem::FileSystem(const std::string& t) : type(t) {}
void FileSystem::createFile(const std::string& name, size_t size) {
    files[name] = size;
}
void FileSystem::deleteFile(const std::string& name) {
    files.erase(name);
}
size_t FileSystem::getFileSize(const std::string& name) {
    auto it = files.find(name);
    return (it != files.end()) ? it->second : 0;
}
std::string FileSystem::getType() const { return type; }
