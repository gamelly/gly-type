#include <iostream>
#include <fstream>
#include <regex>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        return 1;
    }

    std::string line;
    std::ifstream file(argv[1]);
    std::stringstream sourcecode; 
    std::regex pattern_source("@sourcecode");
    std::regex pattern_comments("^(\\/\\*|\\*\\/|\\s\\*)");

    if (!file.is_open()) {
        return 2;
    }

    while (std::getline(file, line)) {
        if (!std::regex_search(line, pattern_comments)) {
            sourcecode << line << std::endl;
        }
    }

    file.clear();
    file.seekg(0, std::ios::beg);

    while (std::getline(file, line)) {
        if (std::regex_search(line, pattern_source)) {
            std::cout << "@code\n" << sourcecode.str() << "\n@endcode" << std::endl;
        } else {
            std::cout << line << std::endl;
        }
    }

    file.close();
    return 0;
}
