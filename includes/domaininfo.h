#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>

// Domain Struct
struct _DomainInfo {
    char domainname[255];
    char ipv4[INET_ADDRSTRLEN];
    char ipv6[INET6_ADDRSTRLEN];
    int (*resolve)(struct _DomainInfo *);
    void (*display)(struct _DomainInfo *);
};
typedef struct _DomainInfo DomainInfo;

// function prototypes
int resolve(DomainInfo * domaininfo);
void display(DomainInfo *domaininfo);

// Function to initialize a DomainInfo struct
DomainInfo * init(char *domain);
#pragma once
