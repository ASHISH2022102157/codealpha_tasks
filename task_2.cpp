//TASK 2-> BASIC FILE MANAGER
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unistd.h> // For chdir() function
#include <dirent.h> // For directory-related functions
#include <sys/stat.h> // For stat() function
#include <cstring> // For strerror() function

// Function to list files in the current directory
void listFilesInDirectory(const std::string& path) {
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(path.c_str())) != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            std::cout << entry->d_name << std::endl;
        }
        closedir(dir);
    } else {
        std::cerr << "Error: Could not open directory " << path << std::endl;
    }
}

// Function to create a directory
void createDirectory(const std::string& path, const std::string& dirname) {
    std::string fullpath = path + "/" + dirname;
    if (mkdir(fullpath.c_str()) == 0) {
        std::cout << "Directory '" << dirname << "' created successfully.\n";
    } else {
        std::cerr << "Error: Could not create directory " << dirname << ": " << strerror(errno) << std::endl;
    }
}

// Function to copy a file
void copyFile(const std::string& src, const std::string& dest) {
    std::ifstream source(src, std::ios::binary);
    std::ofstream destination(dest, std::ios::binary);
    destination << source.rdbuf();
    source.close();
    destination.close();
    std::cout << "File '" << src << "' copied to '" << dest << "' successfully.\n";
}

// Function to move a file
void moveFile(const std::string& src, const std::string& dest) {
    if (rename(src.c_str(), dest.c_str()) == 0) {
        std::cout << "File '" << src << "' moved to '" << dest << "' successfully.\n";
    } else {
        std::cerr << "Error: Could not move file: " << strerror(errno) << std::endl;
    }
}

int main() {
    std::string currentDir = ".";
    std::string command, arg1, arg2;

    while (true) {
        std::cout << "\nCurrent Directory: " << currentDir << '\n';
        std::cout << "Enter command (ls, mkdir <name>, cp <source> <destination>, mv <source> <destination>, cd <directory>, exit): ";
        std::getline(std::cin, command);

        std::istringstream iss(command);
        iss >> arg1 >> arg2;

        if (command == "ls") {
            listFilesInDirectory(currentDir);
        } else if (arg1 == "mkdir") {
            createDirectory(currentDir, arg2);
        } else if (arg1 == "cp") {
            copyFile(arg2, arg2);
        } else if (arg1 == "mv") {
            moveFile(arg1, arg2);
        } else if (arg1 == "cd") {
            if (arg2 == "..") {
                // Navigate up one directory
                size_t found = currentDir.find_last_of("/");
                if (found != std::string::npos) {
                    currentDir = currentDir.substr(0, found);
                }
            } else {
                // Navigate to a specific directory
                currentDir += "/" + arg2;
            }
            // Change the current working directory
            if (chdir(currentDir.c_str()) != 0) {
                std::cerr << "Error: Could not change directory to " << currentDir << ": " << strerror(errno) << std::endl;
            }
        } else if (command == "exit") {
            break;
        } else {
            std::cout << "Invalid command. Please try again.\n";
        }
    }

    return 0;
}
