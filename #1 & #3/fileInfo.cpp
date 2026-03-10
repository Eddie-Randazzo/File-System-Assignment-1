#include <iostream>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <ctime>
#include <iomanip>

void printFilePermissions(mode_t mode) {
    std::cout << ((S_ISDIR(mode)) ? "d" : (S_ISLNK(mode)) ? "l" : "-");
    std::cout << ((mode & S_IRUSR) ? "r" : "-");
    std::cout << ((mode & S_IWUSR) ? "w" : "-");
    std::cout << ((mode & S_IXUSR) ? "x" : "-");
    std::cout << ((mode & S_IRGRP) ? "r" : "-");
    std::cout << ((mode & S_IWGRP) ? "w" : "-");
    std::cout << ((mode & S_IXGRP) ? "x" : "-");
    std::cout << ((mode & S_IROTH) ? "r" : "-");
    std::cout << ((mode & S_IWOTH) ? "w" : "-");
    std::cout << ((mode & S_IXOTH) ? "x" : "-");
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    struct stat sb;


    if (lstat(argv[1], &sb) == -1) {
        perror("lstat");
        return 1;
    }
    
    printFilePermissions(sb.st_mode);
    std::cout << " ";

    std::cout << sb.st_nlink << " ";

    struct passwd *pw = getpwuid(sb.st_uid);
    std::cout << (pw ? pw->pw_name : "unknown") << " ";

    struct group  *gr = getgrgid(sb.st_gid);
    std::cout << (gr ? gr->gr_name : "unknown") << " ";

    std::cout << std::setw(8) << sb.st_size << " ";

    char dateStr[20];
    strftime(dateStr, sizeof(dateStr), "%b %d %H:%M", localtime(&sb.st_mtime));
    std::cout << dateStr << " ";

    std::cout << argv[1] << std::endl;

    std::cout << "Inode: " << sb.st_ino << std::endl;

    return 0;
}