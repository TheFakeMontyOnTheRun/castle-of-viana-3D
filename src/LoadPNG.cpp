//
// Created by monty on 06/10/16.
//
#include <memory>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>

#include "NativeBitmap.h"
#include "LoadPNG.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace odb {

    std::vector<char> readToBuffer(FILE *fileDescriptor) {
        const unsigned N = 1024;

        fseek(fileDescriptor, 0, SEEK_END);
        auto endPos = ftell(fileDescriptor);
        rewind(fileDescriptor);
        std::vector<char> total(endPos);
        auto writeHead = std::begin(total);

        for (int c = 0; c < endPos; ++c) {
            char buffer[N];
            size_t read = fread((void *) &buffer[0], 1, N, fileDescriptor);
            if (read) {
                for (int c = 0; c < read; ++c) {
                    *writeHead = (buffer[c]);
                    writeHead = std::next(writeHead);
                }
            }
            if (read < N) {
                break;
            }
        }

        return total;
    }

    std::vector<char> loadBinaryFileFromPath(const std::string &path) {
        FILE *fd;

        fd = fopen(path.c_str(), "rb");

        if (fd == nullptr) {
            exit(0);
        }

        std::vector<char> toReturn = readToBuffer(fd);
        fclose(fd);

        return toReturn;
    }

    std::shared_ptr<odb::NativeBitmap> loadPNG(const std::string path) {

        auto buffer = loadBinaryFileFromPath(path);
        int xSize;
        int ySize;
        int components;
        auto image = stbi_load_from_memory((const stbi_uc *) buffer.data(), buffer.size(), &xSize, &ySize, &components, 4);
        auto rawData = new int[xSize * ySize];
        std::memcpy( rawData, image, xSize * ySize * sizeof( int ) );
        stbi_image_free(image);


        return (std::make_shared<odb::NativeBitmap>(path, xSize, ySize, rawData));

    }
}