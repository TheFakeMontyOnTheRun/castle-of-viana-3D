//
// Created by monty on 26/09/16.
//
#include <functional>
#include <string>
#include <iterator>
#include <iostream>
#include <sstream>
#include <vector>

#include "Common.h"

namespace Knights {
	float floatFrom(std::string str) {
		std::istringstream buffer(str);
		float temp;
		buffer >> temp;

		return temp;
	}

	int intFrom(std::string str) {
		std::istringstream buffer(str);
		int temp;
		buffer >> temp;

		return temp;
	}

	std::string filterComments(std::string input) {
		bool reading = true;
		std::stringstream output;

		for (auto &character : input) {

			if (character == '/') {
				reading = false;
			} else if (character == '\n') {
				reading = true;
			}

			if (reading) {
				output << character;
			}
		}

		return output.str();
	}

	std::vector<char> readToBuffer(FILE *fileDescriptor) {
		const unsigned N = 1024;

		fseek(fileDescriptor, 0, SEEK_END);
		auto endPos = ftell( fileDescriptor );
		rewind(fileDescriptor);
		std::vector<char> total(endPos);
		auto writeHead = std::begin( total );

		for ( int c = 0; c < endPos; ++c ) {
			char buffer[N];
			size_t read = fread((void *) &buffer[0], 1, N, fileDescriptor);
			if (read) {
				for (unsigned int c = 0; c < read; ++c) {
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

	std::string readToString(FILE *fileDescriptor) {

		fseek(fileDescriptor, 0, SEEK_END);
		long fileSize = ftell(fileDescriptor);
		rewind(fileDescriptor);
		std::string total;
		total.assign(fileSize, 0);

		for (auto pos = 0; pos < fileSize; ++pos) {
			char buffer[1];
			size_t read = fread((void *) &buffer[0], 1, 1, fileDescriptor);
			if (read) {
				for (unsigned int c = 0; c < read; ++c) {
					total[pos] = buffer[c];
				}
			}
			if (read < 1) {
				return "total not met";
			}
		}

		return total;
	}
}