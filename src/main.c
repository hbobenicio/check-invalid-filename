#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define ANSI_FG_RED "31"
#define ANSI_FG_GREEN "32"
#define ANSI_FG_YELLOW "33"

#define ANSI_FG_BRIGHT_RED "91"
#define ANSI_FG_BRIGHT_GREEN "92"
#define ANSI_FG_BRIGHT_YELLOW "93"

#define FILENAME_MAX_SIZE 4098

static void str_pop_last_char(char* const str, size_t* str_len);
static void str_print_binary(const char* str, size_t str_len);

static void set_color(const char* ansi_color_code);
static void set_fg_bright_green();
static void set_fg_bright_red();
static void set_fg_bright_yellow();

int main() {
    char line[FILENAME_MAX_SIZE] = {0};

    while (fgets(line, sizeof(line) - 1, stdin) != NULL) {
        size_t line_len = strlen(line);
        str_pop_last_char(line, &line_len);

        printf("%s: ", line);
        str_print_binary(line, line_len);
    }

    return EXIT_SUCCESS;
}

static void str_pop_last_char(char* const str, size_t* str_len) {
    assert(str != NULL);
    if (*str_len > 0) {
        str[*str_len - 1] = '\0';
        (*str_len)--;
    }
}

static void str_print_binary(const char* str, size_t str_len) {
    int has_non_ascii_chars = 0;
    int has_non_printable_ascii_chars = 0;

    for (size_t i = 0; i < str_len; i++) {
        unsigned char byte = (unsigned char) str[i];

        if (isprint(byte)) {
            set_fg_bright_green();
        } else if (isascii(byte)) {
            has_non_printable_ascii_chars = 1;
            set_fg_bright_yellow();
        } else {
            has_non_printable_ascii_chars = 1;
            has_non_ascii_chars = 1;
            set_fg_bright_red();
        }
        printf(" %02x\x1b[0m", byte);
    }

    printf("\x1b[91m");
    if (has_non_ascii_chars) {
        printf(" !ascii");
    }
    if (has_non_printable_ascii_chars) {
        printf(" !printable_ascii");
    }
    puts("\x1b[0m");
}

static void set_color(const char* ansi_color_code) {
    printf("\x1b[%sm", ansi_color_code);
}

static void set_fg_bright_green() {
    set_color(ANSI_FG_BRIGHT_GREEN);
}

static void set_fg_bright_red() {
    set_color(ANSI_FG_BRIGHT_RED);
}

static void set_fg_bright_yellow() {
    set_color(ANSI_FG_BRIGHT_YELLOW);
}
