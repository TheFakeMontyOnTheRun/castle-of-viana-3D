//
// Created by monty on 08/12/16.
//
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include "Common.h"
#include "IFileLoaderDelegate.h"
#include "CPlainFileLoader.h"

namespace Knights {

    CPlainFileLoader::CPlainFileLoader() {
    }

    CPlainFileLoader::CPlainFileLoader(std::string prefix) : mPrefix(prefix) {
    }

    std::vector<char> CPlainFileLoader::loadBinaryFileFromPath(const std::string &path) {
        FILE *fd;

        fd = fopen((getFilePathPrefix() + mFilenameTransformation(path)).c_str(), "rb");

        if (fd == nullptr) {
            exit(0);
        }

        std::vector<char> toReturn = readToBuffer(fd);
        fclose(fd);

        return toReturn;
    }

    std::string CPlainFileLoader::loadFileFromPath(const std::string &path) {
        FILE *fd;

        fd = fopen(( getFilePathPrefix() + mFilenameTransformation( path ) ).c_str(), "r");

        if (fd == nullptr) {
            exit(0);
        }

        auto toReturn = readToString(fd);
        fclose(fd);

        return toReturn;
    }

    std::string CPlainFileLoader::getFilePathPrefix() {
        return mPrefix;
    }

    void CPlainFileLoader::setFilenameTransformation(std::function<std::string(std::string)> transform) {
        this->mFilenameTransformation = transform;
    }
}
