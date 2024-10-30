#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>

constexpr char MARKER[] = "DEFDE0C";
constexpr size_t MARKER_SIZE = sizeof(MARKER) - 1;

void add_message(const std::string& filename, const std::string& message);
std::string read_message(const std::string& filename);
void delete_message(const std::string& filename);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Not enough arguments. Use help for usage information.\n\n";
        return 1;
    }
    std::string option = argv[1];

    if (option == "help") {
        std::cout << "Options:\n"
              << "add <filename> <message>    Add a message to a JPEG file\n"
              << "read <filename>             Read a message from a JPEG file\n"
              << "delete <filename>           Delete a message from a JPEG file\n"
              << "help                        Show help message\n";
    } else if (option == "add") {
        if (argc != 4) {
            std::cerr << "Not enough arguments. Use help for usage information.\n";
            return 1;
        }
        add_message(argv[2], argv[3]);
    } else if (option == "read") {
        if (argc != 3) {
            std::cerr << "Not enough arguments. Use help for usage information.\n";
            return 1;
        }
        std::string message = read_message(argv[2]);
        std::cout << "Hidden message: " << message << std::endl;
    } else if (option == "delete") {
        if (argc != 3) {
            std::cerr << "Not enough arguments. Use help for usage information.\n";
            return 1;
        }
        delete_message(argv[2]);
    } else {
        std::cerr << "Unknown option. Use help for usage information.\n";
        return 1;
    }

    return 0;
}

void add_message(const std::string& filename, const std::string& message) {
    std::ofstream file(filename, std::ios::binary | std::ios::app);
    if (!file) return;

    size_t message_size = message.size();
    file.write(message.data(), message.size());
    file.write(reinterpret_cast<const char*>(&message_size), sizeof(size_t));
    file.write(MARKER, MARKER_SIZE);
}

std::string read_message(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) return "";

    file.seekg(-static_cast<std::streamoff>(MARKER_SIZE), std::ios::end);
    char marker[MARKER_SIZE];
    file.read(marker, MARKER_SIZE);
    if (std::string_view(marker, MARKER_SIZE) != MARKER) return "";

    file.seekg(-static_cast<std::streamoff>(MARKER_SIZE + sizeof(size_t)), std::ios::end);
    size_t message_len = 0;
    file.read(reinterpret_cast<char*>(&message_len), sizeof(size_t));

    file.seekg(-static_cast<std::streamoff>(MARKER_SIZE + sizeof(size_t) + message_len), std::ios::end);
    std::string message(message_len, '\0');
    file.read(&message[0], message_len);

    return message;
}

void delete_message(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);

    file.seekg(-static_cast<std::streamoff>(MARKER_SIZE), std::ios::end);
    char marker[MARKER_SIZE];
    file.read(marker, MARKER_SIZE);
    if (std::string_view(marker, MARKER_SIZE) != MARKER) return;

    file.seekg(-static_cast<std::streamoff>(MARKER_SIZE + sizeof(size_t)), std::ios::end);
    size_t message_len = 0;
    file.read(reinterpret_cast<char*>(&message_len), sizeof(size_t));

    file.seekg(0, std::ios::end);
    std::streamoff file_size = file.tellg();
    std::streamoff new_size = file_size - static_cast<std::streamoff>(message_len + sizeof(size_t) + MARKER_SIZE);
    std::filesystem::resize_file(filename, new_size);
}
