#define _POSIX_C_SOURCE 200112L
#include "focas_mock.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>

#define BACKLOG 8
#define BUFSIZE 512

static void trim_newline(char *s) {
    if (!s) return;
    size_t n = strlen(s);
    while (n > 0 && (s[n-1] == '\n' || s[n-1] == '\r')) { s[n-1] = '\0'; n--; }
}

/* respond with a line (adds newline) */
static ssize_t send_line(int fd, const char *line) {
    if (!line) return 0;
    size_t len = strlen(line);
    char tmp[1024];
    if (len + 2 >= sizeof(tmp)) {
        /* too long, send truncated */
        snprintf(tmp, sizeof(tmp)-2, "%.900s", line);
    } else {
        strcpy(tmp, line);
    }
    strcat(tmp, "\n");
    return send(fd, tmp, strlen(tmp), 0);
}

/* simple case-insensitive compare for prefix */
static int starts_with_ci(const char *s, const char *pref) {
    if (!s || !pref) return 0;
    while (*pref) {
        if (!*s) return 0;
        if (tolower((unsigned char)*s) != tolower((unsigned char)*pref)) return 0;
        s++; pref++;
    }
    return 1;
}

static void handle_client(int client_fd, cnc_state_t *state) {
    char buf[BUFSIZE];
    ssize_t n;
    /* simple loop: read line commands */
    while (1) {
        n = recv(client_fd, buf, sizeof(buf)-1, 0);
        if (n <= 0) break;
        buf[n] = '\0';
        trim_newline(buf);
        /* ignore empty */
        if (buf[0] == '\0') continue;

        printf("[focas-sim] RX: %s\n", buf);

        /* tick state before answering */
        cnc_state_tick(state);

        if (starts_with_ci(buf, "GET PART")) {
            int32_t v = cnc_state_get_part_count(state);
            char out[128];
            snprintf(out, sizeof(out), "PART=%d", v);
            send_line(client_fd, out);
        }
        else if (starts_with_ci(buf, "GET PROGRAM")) {
            const char *p = cnc_state_get_program(state);
            char out[128];
            snprintf(out, sizeof(out), "PROGRAM=%s", p);
            send_line(client_fd, out);
        }
        else if (starts_with_ci(buf, "GET STATUS")) {
            const char *st = cnc_state_get_status(state);
            char out[128];
            snprintf(out, sizeof(out), "STATUS=%s", st);
            send_line(client_fd, out);
        }
        else if (starts_with_ci(buf, "SET PART ")) {
            /* SET PART <number> */
            const char *arg = buf + 9;
            int v = atoi(arg);
            cnc_state_set_part_count(state, v);
            send_line(client_fd, "OK");
        }
        else if (starts_with_ci(buf, "SET PROGRAM ")) {
            const char *arg = buf + 12;
            cnc_state_set_program(state, arg);
            send_line(client_fd, "OK");
        }
        else if (starts_with_ci(buf, "RANDOM ON")) {
            cnc_state_set_random(state, 1);
            send_line(client_fd, "OK");
        }
        else if (starts_with_ci(buf, "RANDOM OFF")) {
            cnc_state_set_random(state, 0);
            send_line(client_fd, "OK");
        }
        else if (starts_with_ci(buf, "QUIT") || starts_with_ci(buf, "BYE")) {
            send_line(client_fd, "BYE");
            break;
        }
        else {
            send_line(client_fd, "ERR=UNKNOWN_CMD");
        }
    }
    close(client_fd);
    printf("[focas-sim] Client disconnected\n");
}

int focas_mock_server_run(const char *host, uint16_t port, cnc_state_t *state) {
    int server_fd = -1, client_fd = -1;
    struct sockaddr_in addr;
    int opt = 1;

    /* ignore SIGPIPE so send() won't kill the process on client disconnect */
    signal(SIGPIPE, SIG_IGN);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return -1;
    }
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (!host || strcmp(host, "0.0.0.0") == 0) {
        addr.sin_addr.s_addr = INADDR_ANY;
    } else {
        if (inet_pton(AF_INET, host, &addr.sin_addr) != 1) {
            fprintf(stderr, "Invalid bind address: %s\n", host);
            close(server_fd);
            return -1;
        }
    }

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(server_fd);
        return -1;
    }

    if (listen(server_fd, BACKLOG) < 0) {
        perror("listen");
        close(server_fd);
        return -1;
    }

    printf("[focas-sim] Listening on %s:%u\n", host ? host : "0.0.0.0", port);

    while (1) {
        struct sockaddr_in cliaddr;
        socklen_t clilen = sizeof(cliaddr);
        client_fd = accept(server_fd, (struct sockaddr*)&cliaddr, &clilen);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }
        printf("[focas-sim] Accepted connection from %s\n", inet_ntoa(cliaddr.sin_addr));
        /* handle in-process, sequentially (simple). You can extend to fork/pthreads. */
        handle_client(client_fd, state);
    }

    close(server_fd);
    return 0;
}
