#include <Windows.h>
#include "Common.hpp"
#include "Game.hpp"
#include "OpenGL.hpp"
#include "Platform.hpp"


constexpr s32 WINDOW_WIDTH = 800;
constexpr s32 WINDOW_HEIGHT = WINDOW_WIDTH;
static bool is_window_open = true;


static s64
Win32GetWallClock() {
    LARGE_INTEGER time;
    QueryPerformanceCounter(&time);
    return time.QuadPart;
}

static HGLRC
Win32OpenGLGetRenderingContext(HDC device_context) {
    PIXELFORMATDESCRIPTOR desired_pixel_format = {};
    desired_pixel_format.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    desired_pixel_format.nVersion = 1;
    desired_pixel_format.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
    desired_pixel_format.cColorBits = 32;
    desired_pixel_format.cAlphaBits = 8;

    s32 suggested_pixel_format_idx = ChoosePixelFormat(device_context, &desired_pixel_format);
    PIXELFORMATDESCRIPTOR suggested_pixel_format;
    DescribePixelFormat(device_context, suggested_pixel_format_idx,
                        sizeof(suggested_pixel_format), &suggested_pixel_format);
    SetPixelFormat(device_context, suggested_pixel_format_idx, &suggested_pixel_format);
    return wglCreateContext(device_context);
}

static void
Win32ProcessMessages(Input *input) {
    MSG message = {};
    while (PeekMessageA(&message, 0, 0, 0, PM_REMOVE)) {
        switch (message.message) {
            case WM_KEYDOWN:
            case WM_KEYUP: {
                constexpr s32 IS_KEY_DOWN_MASK = 1 << 31;
                bool is_key_down = (message.lParam & IS_KEY_DOWN_MASK) == 0;
                u32 vk_code = static_cast<u32>(message.wParam);
                #define SET_KEY(key) input->is_key_down[key] = is_key_down; input->last_pressed_key = key
                switch (vk_code) {
                    case 'A': { SET_KEY(KEY_A); } break;
                    case 'D': { SET_KEY(KEY_D); } break;
                    case 'S': { SET_KEY(KEY_S); } break;
                    case 'W': { SET_KEY(KEY_W); } break;
                    case VK_ESCAPE: { is_window_open = false; } break;
                }
            } break;
            default: {
                TranslateMessage(&message);
                DispatchMessageA(&message);
            } break;
        }
    }
}

LRESULT CALLBACK
Win32WindowCallback(HWND window, UINT message, WPARAM w_param, LPARAM l_param) {
    LRESULT result = 0;
    switch (message) {
        // WM_CLOSE is different from WC_DESTROY, WC_NCDESTROY, and WC_QUIT.
        // It is called when the window is being closed, e.g., the 'X' button
        // is pressed, 'Close' is chosen from the window's menu, or
        // 'Alt+F4' is pressed.
        case WM_CLOSE: {
            is_window_open = false;
        } break;
        default: {
            result = DefWindowProc(window, message, w_param, l_param);
        } break;
    }

    return result;
}

static HWND
Win32CreateWindow(HINSTANCE instance) {
    WNDCLASS window_class = {};
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpfnWndProc = Win32WindowCallback;
    window_class.hInstance = instance;
    window_class.lpszClassName = "PacmanWindowClass";
    if (!RegisterClass(&window_class)) {
        PlatformShowErrorAndExit("Could not create window");
    }

    RECT client_rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    AdjustWindowRect(&client_rect, WS_OVERLAPPEDWINDOW, false);
    s32 window_width = client_rect.right - client_rect.left;
    s32 window_height = client_rect.bottom - client_rect.top;

    HWND window = CreateWindowEx(
        0, window_class.lpszClassName, "Pacman",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        0, 0,
        window_width, window_height,
        0, 0, instance, 0
    );

    if (!window) {
        PlatformShowErrorAndExit("Could not create window");
    }

    return window;
}

File
PlatformReadFile(char *file_name) {
    HANDLE file_handle = CreateFile(file_name, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    bool is_file_opened = true;
    if (file_handle == INVALID_HANDLE_VALUE) {
        is_file_opened = false;
    }

    LARGE_INTEGER file_size;
    if (!GetFileSizeEx(file_handle, &file_size)) {
        is_file_opened = false;
    }

    ASSERT(file_size.QuadPart <= 0xffffffff);
    u32 file_bytes = static_cast<u32>(file_size.QuadPart);
    void *buffer= VirtualAlloc(0, file_bytes, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!buffer) {
        is_file_opened = false;
    }

    DWORD bytes_read;
    if (!ReadFile(file_handle, buffer, file_bytes, &bytes_read, 0)) {
        is_file_opened = false;
    }

    if (bytes_read != file_bytes) {
        is_file_opened = false;
    }

    if (!is_file_opened) {
        PlatformShowErrorAndExit("Could not open file");
    }

    CloseHandle(file_handle);
    File file;
    file.buffer = buffer;
    return file;
}

void
PlatformFreeFile(File file) {
    ASSERT(file.buffer);
    VirtualFree(file.buffer, 0, MEM_RELEASE);
}

void
PlatformShowErrorAndExit(char *msg) {
    MessageBox(0, msg, "Error", MB_OK);
    is_window_open = false;
}

s32
WinMain(HINSTANCE instance, HINSTANCE, LPSTR, s32) {
    HWND window = Win32CreateWindow(instance);
    HDC device_context = GetDC(window);
    HGLRC gl_rendering_context = Win32OpenGLGetRenderingContext(device_context);
    if (!wglMakeCurrent(device_context, gl_rendering_context)) {
        PlatformShowErrorAndExit("OpenGL context error");
    }

    if (!gladLoadGL()) {
        NOT_IMPLEMENTED;
    }

    GameInit(WINDOW_WIDTH, WINDOW_HEIGHT);
    Input input = {};
    f32 delta_time = 0.016f;

    LARGE_INTEGER pf;
    QueryPerformanceFrequency(&pf);
    s64 performance_frequency = pf.QuadPart;
    s64 frame_time_start = Win32GetWallClock();
    while (is_window_open) {
        Win32ProcessMessages(&input);
        GameUpdate(delta_time, input);
        SwapBuffers(device_context);

        s64 frame_time_end = Win32GetWallClock();
        s64 frame_time_diff = frame_time_end - frame_time_start;
        delta_time = static_cast<f32>(frame_time_diff) / performance_frequency;
        frame_time_start = frame_time_end;
    }

    wglDeleteContext(gl_rendering_context);
    ReleaseDC(window, device_context);
    return 0;
}
