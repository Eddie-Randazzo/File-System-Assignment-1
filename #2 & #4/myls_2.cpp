#include <iostream>
#include <string>
#include <iomanip>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <cstring>
#include <ctime>

using namespace std;

// Function to sort an array of strings in alphabetical order
void sort(string n[], int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (n[j] > n[j + 1]) {
                string temp = n[j];
                n[j] = n[j + 1];
                n[j + 1] = temp;
            }
        }
    }
}

// Formats permissions: e.g., rwxr-xr-x
string formatPermissions(mode_t mode) {
    string perms = "---------";
    perms[0] = (mode & S_IRUSR) ? 'r' : '-';
    perms[1] = (mode & S_IWUSR) ? 'w' : '-';
    perms[2] = (mode & S_IXUSR) ? 'x' : '-';
    perms[3] = (mode & S_IRGRP) ? 'r' : '-';
    perms[4] = (mode & S_IWGRP) ? 'w' : '-';
    perms[5] = (mode & S_IXGRP) ? 'x' : '-';
    perms[6] = (mode & S_IROTH) ? 'r' : '-';
    perms[7] = (mode & S_IWOTH) ? 'w' : '-';
    perms[8] = (mode & S_IXOTH) ? 'x' : '-';
    return perms;
}

// Identifies file type for the prefix
string fileType(mode_t mode) {
    if (S_ISDIR(mode))  return "[DIR]    ";
    if (S_ISLNK(mode))  return "[SYMLNK] ";
    if (S_ISREG(mode))  return "[FILE]   ";
    return "[OTHER]  ";
}

// Formats time as [Month] [Day] [Hour]:[Min]
string formatTime(time_t t) {
    struct tm *lt = localtime(&t);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%b %d %H:%M", lt);
    return string(buffer);
}

int main() {
    char cwd[256];
    string names[256];
    string user_cmd;

    while (true) {
        if (!getcwd(cwd, sizeof(cwd))) break;
        cout << endl << cwd << "> ";

        getline(cin, user_cmd);
        if (user_cmd == "exit") break;

        // Check for "ls -al" or just "ls"
        if (user_cmd.substr(0, 2) == "ls") {
            bool long_format = (user_cmd.find("-al") != string::npos);
            
            DIR * dir = opendir(cwd);
            if (!dir) {
                cout << "Error opening directory." << endl;
                continue;
            }

            struct dirent * dir_entry;
            int count = 0;
            while ((dir_entry = readdir(dir)) != nullptr && count < 256) {
                names[count++] = dir_entry->d_name;
            }
            closedir(dir);

            sort(names, count);

            for (int j = 0; j < count; j++) {
                struct stat f_stat;
                // Get file stats using the filename
                if (stat(names[j].c_str(), &f_stat) == -1) continue;

                if (long_format) {
                    // Get owner name
                    struct passwd *pw = getpwuid(f_stat.st_uid);
                    string owner = (pw) ? pw->pw_name : to_string(f_stat.st_uid);

                    // Print long format: [Type] [Perms] [Owner] [Size] [Time] [Name]
                    cout << fileType(f_stat.st_mode) << " "
                         << formatPermissions(f_stat.st_mode) << " "
                         << setw(8) << owner << " "
                         << setw(8) << f_stat.st_size << " "
                         << formatTime(f_stat.st_mtime) << " "
                         << names[j] << endl;
                } else {
                    // Standard ls (skip hidden files)
                    if (names[j][0] == '.') continue;
                    cout << names[j] << "  ";
                }
            }
            if (!long_format) cout << endl;
        }
    }
    return 0;
}