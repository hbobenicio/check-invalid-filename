/**
 * cc -Wall -pedantic -o check-invalid-filenames check-invalid-filenames.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <dirent.h>

static void usage_print();
static int check_invalid_filenames(const char* dir_path, size_t dir_path_len);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "error: nenhum argumento informado.\n");
        puts("");
        usage_print(argv[0]);
        exit(EXIT_FAILURE);
    }

    int failed = 0;
    for (int i = 1; i < argc; i++) {
        const char* dir_path = argv[i];
        size_t dir_path_len = strlen(dir_path);

        if (check_invalid_filenames(dir_path, dir_path_len) != 0) {
            failed = 1;
        }
    }

    return failed;
}

static void usage_print(const char* progname) {
    printf("usage: %s <DIR> [DIR [...]]\n\n", progname);
}

static int check_invalid_filenames(const char* dir_path, size_t dir_path_len) {
    DIR* dir = opendir(dir_path);
    if (dir == NULL) {
        int err_code = errno;
        const char* err_msg = strerror(err_code);
        fprintf(stderr, "error: opendir: error while opening dir '%s': [%d] %s\n", dir_path, err_code, err_msg);
        return 1;
    }

    struct dirent *dir_entry = NULL;
    while (1) {
        errno = 0;
        dir_entry = readdir(dir);
        if (dir_entry == NULL) {
            // no errors. it's just the end of the dir stream
            if (errno == 0) {
                break;
            }
            int err_code = errno;
            const char* err_msg = strerror(err_code);
            fprintf(stderr, "error: readdir: error while reading dir '%s': [%d] %s\n", dir_path, err_code, err_msg);
            return 1;
        }

        const char* entry_name = dir_entry->d_name;
        if (entry_name == NULL || entry_name[0] == '\0' || entry_name[0] == '.') {
            continue;
        }

        printf("'%s'", entry_name);

        size_t entry_name_len = strlen(entry_name);
        int has_non_ascii_chars = 0;
        int has_non_printabe_ascii_chars = 0;

        printf("\x1b[33m");
        for (size_t i = 0; i < entry_name_len; i++) {
            unsigned char byte = (unsigned char) entry_name[i];
            printf(" %02x", byte);

            if (!isascii(byte)) {
                has_non_ascii_chars = 1;
            }
            if (!isprint(byte)) {
                has_non_printabe_ascii_chars = 1;
            }
        }
        printf("\x1b[0m");

        printf("\x1b[91m");
        if (has_non_ascii_chars) {
            printf(" !ascii");
        }
        if (has_non_printabe_ascii_chars) {
            printf(" !printable_ascii");
        }
        puts("\x1b[0m");
    }
    return 0;
}

