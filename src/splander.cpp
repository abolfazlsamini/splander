#include <cstdint>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "gui.h"
#include "steam/steam_api.h"
#include "utils.h"
#if defined(_WIN32) || defined(_WIN64)
#define WIN32_LEAN_AND_MEAN
// #define NOMINMAX // Prevent Windows from defining min/max macros
#define NOGDI	 // Exclude GDI symbols (e.g., Rectangle)
#define NOUSER	 // Exclude User symbols (e.g., ShowCursor)
#include <winsock2.h>
#include <ws2tcpip.h>
#include <C:\raylib\raylib\src\raylib.h>
#include <iostream>
#else
#include "raylib.h"
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define SOCKET_ERROR -1
#endif
// #pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib

#define SERVER_IP "127.0.0.1"
#define PORT 6969
#define BUFFER_SIZE 1024

typedef struct
{
    Vector2 pos;
    Vector2 size;
} Player;

typedef struct
{
    Player player;
    Player player2;
    Gui gui;
    bool is_paused;
    bool should_close;
} GameState;

// Update and draw one frame
static void UpdateDrawFrame(GameState *gs);
#if defined(_WIN32) || defined(_WIN64)
SOCKET client_socket;
#else
typedef int SOCKET;
SOCKET client_socket;
#endif
struct sockaddr_in server;
char buffer[BUFFER_SIZE];
char NAME[] = "Sam"; //TODO when testing, one client should be "Sam" but the other one can be anything

void init_socket()
{
#if defined(_WIN32) || defined(_WIN64)
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Failed to initialize Winsock. Error: %d\n", WSAGetLastError());
        exit(1);
    }

    // Create a socket
    if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
    {
        printf("Failed to create socket. Error: %d\n", WSAGetLastError());
        WSACleanup();
        exit(1);
    }
#else
    // Create a socket
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if(client_socket < 0){
        printf("Error while creating socket\n");
        exit(1);
    }
    printf("Socket created successfully\n");
#endif
}

#if defined(_WIN32) || defined(_WIN64)
void close_socket(SOCKET socket)
{
    closesocket(client_socket);
    WSACleanup();
}
#else
void close_socket(int c_socket)
{
    close(c_socket);
}
#endif


int main()
{
    // Initialize sock
    init_socket();
    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_port = htons(PORT);

    SetRandomSeed(0x69420);
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Splander");
    SetExitKey(KEY_F1);
    SetTargetFPS(60);
    
    SteamAPI_Init();

    GameState gs = {0};
    GUIInit(&gs.gui);
    gs.player.size.x = 50;
    gs.player.size.y = 50;
    gs.player.pos.x = GetScreenWidth() / 2.0f - (gs.player.size.x / 2) - 30.0f;
    gs.player.pos.y = GetScreenHeight() / 2.0f - (gs.player.size.y / 2);

    gs.player2.size.x = 50;
    gs.player2.size.y = 50;
    gs.player2.pos.x = GetScreenWidth() / 2.0f - (gs.player2.size.x / 2) + 30.0f;
    gs.player2.pos.y = GetScreenHeight() / 2.0f - (gs.player2.size.y / 2);
    gs.is_paused = true;

    while (!WindowShouldClose() && !gs.should_close)
    {
        UpdateDrawFrame(&gs);
    }

    CloseWindow();

    return 0;
}

void GUIFunctionEval(GameState *gs)
{
    if (gs->gui.sel_page == 0)
    {
        switch (gs->gui.sel_button)
        {
            case 0:
                gs->is_paused = !gs->is_paused;
                char new_name[16];
                strcpy(new_name, "name:");
                strcat(new_name, NAME);
                if (sendto(client_socket, new_name, 8, 0, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
                {
                    printf("sendto() failed. Error\n");
                    close_socket(client_socket);
                }
                break;
            case 1:
            case 2:
                printf("[Splander LOG] Button no Function!\n");
                break;
            case 3:
                gs->should_close = true;
                break;
            default:
                printf("[Splander ERROR] button has no function assigned! \n");
                break;
        }
    }
}


static void UpdateServer(GameState *gs)
{
    char send_data[240];
    // name,time,position
    sprintf(send_data, "%s,%ld,%.2f:%.2f", NAME, GetTimeMs64(), gs->player.pos.x, gs->player.pos.y);
    printf("data sent: %s\n", send_data);

    // put all syncy data here  V
    if (sendto(client_socket, send_data, (int)strlen(send_data), 0, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
    {
        // printf("sendto() failed. Error\n");
        close_socket(client_socket);
    }
}
long long prev_time_from_server = 0;
static void UpdateClient(GameState *gs)
{
    int recv_len;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    if ((recv_len = recvfrom(client_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_len)) <= 0)
    {
        // printf("recvfrom() failed. Error\n");
    }
    else
    {
        buffer[recv_len] = '\0'; // Null-terminate the string
        printf("Recieved: %s\n", buffer);
    }

    char name[100], position[100];
    long long time;
    char name2[100], position2[100];
    long long time2;
    sscanf(buffer, "%[^,],%lld,%[^;];%[^,],%lld,%s", name, &time, position, name2, &time2, position2);
    float x, y;
    if (prev_time_from_server <= 0)
    {
        if (strcmp(name, NAME) != 0)
        {
            sscanf(position, "%f:%f", &x, &y);
            gs->player2.pos.x = x;
            gs->player2.pos.y = y;
        }
        else
        {
            sscanf(position2, "%f:%f", &x, &y);
            gs->player2.pos.x = x;
            gs->player2.pos.y = y;
        }
    }
}
// Update and draw game frame
int counter = 0;
static void UpdateDrawFrame(GameState *gs)
{
    if (counter % 10 == 0)
    {
        UpdateServer(gs);
        UpdateClient(gs);
    }
    counter++;
    if (!gs->is_paused)
    {
        if (IsKeyDown(KEY_RIGHT))
            gs->player.pos.x += 2.0f;
        if (IsKeyDown(KEY_LEFT))
            gs->player.pos.x -= 2.0f;
        if (IsKeyDown(KEY_UP))
            gs->player.pos.y -= 2.0f;
        if (IsKeyDown(KEY_DOWN))
            gs->player.pos.y += 2.0f;
    }

    else
    {
        if (IsKeyPressed(KEY_UP))
        {
            gs->gui.sel_button -= 1;
            if (gs->gui.sel_button < 0)
            {
                gs->gui.sel_button = gs->gui.items[gs->gui.sel_page].buttons.size() - 1;
            }
        }
        if (IsKeyPressed(KEY_DOWN))
        {
            gs->gui.sel_button += 1;
            if ((uint64_t)gs->gui.sel_button >= gs->gui.items[gs->gui.sel_page].buttons.size())
            {
                gs->gui.sel_button = 0;
            }
        }
        if (IsKeyPressed(KEY_ENTER))
        {
            GUIFunctionEval(gs);
        }
    }

    if (IsKeyPressed(KEY_ESCAPE))
        gs->is_paused = !gs->is_paused;

    BeginDrawing();
    ClearBackground(GetColor(0x1E1E1EFF));

    DrawRectangleV(gs->player.pos, gs->player.size, GetColor(0xffffff));
    DrawRectangleV(gs->player2.pos, gs->player2.size, GetColor(0xffaaff));

    DrawFPS(10, 10);

    if (gs->is_paused)
    {
        GUIDraw(gs->gui);
    }

    EndDrawing();
}
