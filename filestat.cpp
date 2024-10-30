#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_map>

std::unordered_map<std::string, int> filestat();

int main() {
    std::unordered_map<std::string, int> stats = filestat();
    for (const auto& [ftype, count] : stats) {
        std::cout << ftype << ": " << count << std::endl;
    };
};

std::unordered_map<std::string, int> filestat() {
    std::unordered_map<std::string, int> stats;
    const auto& work_dir = std::filesystem::current_path();
    for (const auto& dir_entry : std::filesystem::directory_iterator{work_dir}) {
        std::string ftype;
        switch (dir_entry.symlink_status().type()) {
            case std::filesystem::file_type::regular:
                ftype = "regular file";
                break;
            case std::filesystem::file_type::directory:
                ftype = "directory";
                break;
            case std::filesystem::file_type::symlink:
                ftype = "symlink";
                break;
            case std::filesystem::file_type::block:
                ftype = "block device";
                break;
            case std::filesystem::file_type::character:
                ftype = "character device";
                break;
            case std::filesystem::file_type::fifo:
                ftype = "named IPC pipe";
                break;
            case std::filesystem::file_type::socket:
                ftype = "named IPC socket";
                break;
            case std::filesystem::file_type::unknown:
                ftype = "unknown";
                break;
            default:
                ftype = "none";
                break;
        };
        ++stats[ftype];
    };
    return stats;
};
