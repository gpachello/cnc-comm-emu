#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "focas_mock.h"
#include "../../core/cnc_state.h"

int main(int argc, char **argv)
{
    const char *bind_addr = "0.0.0.0";
    uint16_t port = 8193;

    if (argc >= 2) bind_addr = argv[1];
    if (argc >= 3) port = (uint16_t)atoi(argv[2]);

    cnc_state_t state;
    cnc_state_init(&state);

    printf("===== CNC Communications Emulator (textual mock) - Fanuc FOCAS =====\n");
    printf("Bind: %s  Port: %u\n", bind_addr, port);
    printf("Commands: GET PART, GET PROGRAM, GET STATUS, SET PART <n>, SET PROGRAM <name>, RANDOM ON/OFF, QUIT\n");

    return focas_mock_server_run(bind_addr, port, &state);
}
