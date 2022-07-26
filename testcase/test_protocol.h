/**
 * test_protocol.h
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFSIZE 1024
#define BUFSIZE_MAX 4096
#define RECORD_MAX 1000
#define DATA_END ".\n"

