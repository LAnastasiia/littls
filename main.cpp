#include <iostream>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <iomanip>
#include <cstring>
#include <cerrno>

// 4.
// Написати програму, яка виводить список всіх файлів та директорій у поточній директорії,
// разом із їх розмірами (для файлів), за допомогою системних викликів POSIX або стандартної бібліотеки С.

#define MAX_NAME_SIZE (256)


int main() {

    // Get CWD.
    char buff_cwd_name[MAX_NAME_SIZE];
    getcwd(buff_cwd_name,  MAX_NAME_SIZE);  // saves name into buff_cwd_name

    // Open CWD
    DIR *dir_p = opendir(buff_cwd_name);

    if (dir_p == nullptr)
        std::cerr << "Failed to open dir." << " Error code: " << errno << std::endl;

    // Iterate
    else {
        struct dirent *dir_entry = nullptr;
            // readdir() will assign the next dir entry to it
            // dir entry may be statically allocated -->  do not attempt to free() it.

        struct stat buf_stats{};

        while ( (dir_entry = readdir(dir_p)) != nullptr ) {

            // Try to get file size from stats.


            auto dir_entry_path = std::string() + buff_cwd_name + '/' + dir_entry->d_name;

            int stat_status = (dir_entry->d_type == DT_LNK) ?
                    lstat((dir_entry_path).c_str(), &buf_stats)
                    :
                    stat((dir_entry_path).c_str(), &buf_stats);

            if ( stat_status == EXIT_SUCCESS ) {
                std::cout
                        << std::left << std::setw(25)
                        << dir_entry->d_name
                        << std::setw(25)
                        << buf_stats.st_size
                        << std::endl;
            } else {
                std::cerr << "Unable to get stats." << " Error code: " << errno << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        if (errno != 0) std::cout << "Failed iterating directory. Error code: " << errno;
            // readdir() returns nullptr either if there's no entries left unread or if an error occurred while reading.
            // In latter case, it would also update errno.
    }

    closedir(dir_p);

    return 0;
}