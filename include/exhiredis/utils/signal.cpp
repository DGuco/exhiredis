//
// Created by dguco on 19-1-4.
//
#include "signal.h"

namespace exhiredis
{

void ignore_pipe()
{
    struct sigaction sig;

    sig.sa_handler = SIG_IGN;
    sig.sa_flags = 0;
    sigemptyset(&sig.sa_mask);
    sigaddset(&sig.sa_mask, SIGPIPE);
    if (pthread_sigmask(SIG_BLOCK, &sig.sa_mask, NULL) == -1) {
        perror("SIG_PIPE");
    }
}
}