#ifndef FOCAS_MOCK_H
#define FOCAS_MOCK_H

#include "../core/cnc_state.h"

/* Start the fanuc mock server (blocking). 
   host: bind address, pass NULL or "0.0.0.0" for all interfaces
   port: TCP port to listen (use 8193)
*/
int focas_mock_server_run(const char *host, uint16_t port, cnc_state_t *state);

#endif /* FOCAS_MOCK_H */
