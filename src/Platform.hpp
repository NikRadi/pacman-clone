#ifndef PACMAN_PLATFORM_HPP
#define PACMAN_PLATFORM_HPP


enum {
    KEY_NONE,
    KEY_A,
    KEY_D,
    KEY_S,
    KEY_W,

    KEY_COUNT
};

struct Input {
    s32 last_pressed_key;
    bool is_key_down[KEY_COUNT];
};

struct File {
    void *buffer;
};


File
PlatformReadFile(char *file_name);

void
PlatformFreeFile(File file);

#endif // PACMAN_PLATFORM_HPP
