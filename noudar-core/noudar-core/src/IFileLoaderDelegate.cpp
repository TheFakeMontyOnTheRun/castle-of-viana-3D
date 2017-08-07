//
// Created by monty on 09/12/16.
//
#include <vector>
#include <string>
#include "IFileLoaderDelegate.h"

namespace Knights {
    std::string filterLineBreaks(std::string input) {
        auto position = input.find('\n');

        while (position != std::string::npos) {
            input.replace(position, 1, "");
            position = input.find('\n', position + 1);
        }

        return input;
    }
}

