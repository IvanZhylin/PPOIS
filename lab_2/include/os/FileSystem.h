#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <map>
#include <string>
#include <cstddef>

class FileSystem {
private:
    std::map<std::string, size_t> files;
    std::string type;
public:
    FileSystem(const std::string& t);
    void createFile(const std::string& name, size_t size);
    void deleteFile(const std::string& name);
    size_t getFileSize(const std::string& name);
    std::string getType() const;
};
#endif
