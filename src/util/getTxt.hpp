#include <fstream>
#include <iostream>
#include <cstdlib>
#include <unistd.h>

std::string getFileContents(const char* filePath) {
    
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        return nullptr;
    }
    
    std::string contentsStr;
    std::string line;
    while (getline(file, line)) {
        contentsStr += line;
        contentsStr += "\n";
    }

    // Close the file
    file.close();

    std::cout << "___________________________________" << std::endl;
    std::cout << contentsStr << std::endl;
    std::cout << "___________________________________" << std::endl;

    return contentsStr;
}