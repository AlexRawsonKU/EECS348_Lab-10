#include <iostream>
#include <fstream>

#include "calculator.hpp"

int main(int argc, char *argv[])
{
    std::string filename;
    std::cout << "Enter a filename: ";
    std::cin >> filename;

    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file." << std::endl;
        return 1;
    }

    std::string line;
    while (!std::getline(file, line).eof())
    {
        if (validate_double(line))
        {
            std::cout << line << " is valid double" << std::endl;
        }
        else
        {
            std::cout << line << " is not valid double" << std::endl;
        }
    }
}
