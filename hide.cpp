#include <sys/stat.h>

#include <filesystem>
#include <iostream>
#include <string>

void hide_file(const std::filesystem::path& filepath);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    };
    std::filesystem::path filepath = argv[1];
    if (!std::filesystem::exists(filepath)) {
        std::cerr << "File doesn't exist\n";
        return 1;
    };
    hide_file(filepath);
    std::cout << "OK\n";
};

void hide_file(const std::filesystem::path& filepath) {
    std::filesystem::path shadow_dir = "./shadow_dir";
    if (!std::filesystem::exists(shadow_dir)) {
        std::filesystem::create_directory(shadow_dir);
        chmod(shadow_dir.c_str(), S_IWUSR | S_IXUSR | S_IXGRP | S_IXOTH);
    };
    std::filesystem::rename(filepath, shadow_dir / filepath);
};
