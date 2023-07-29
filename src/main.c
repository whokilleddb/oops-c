#include "domaininfo.h"

// Resolve Domain name
int resolve(DomainInfo * domaininfo) {
    int status = 0;
    struct addrinfo hints, *result, *res;

    // Zero out memory
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    // AF_UNSPEC means it can return either IPv4 or IPv6 addresses
    hints.ai_socktype = SOCK_STREAM;

    // Get the address information for the domain name
    status = getaddrinfo(domaininfo->domainname, NULL, &hints, &result);
    if (status != 0) {
        fprintf(stderr, "[!] Function getaddrinfo() failed due to error: %s\n", gai_strerror(status));
        return -1;
    }

    // Loop through the linked list of address structures
    for (res = result; res != NULL; res = res->ai_next) {
        // IPv4
        if (res->ai_family == AF_INET) {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
            // Convert the IP address to a human-readable string
            inet_ntop(res->ai_family, &(ipv4->sin_addr), domaininfo->ipv4, INET_ADDRSTRLEN);
        }
        // IPv6
        else {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)res->ai_addr;
            // Convert the IP address to a human-readable string
            inet_ntop(res->ai_family, &(ipv6->sin6_addr), domaininfo->ipv6, INET6_ADDRSTRLEN);
        }
    }

    // Free the linked list when you're done with it
    freeaddrinfo(result);
    return 0;
}

// Print DomainInfo candidates
void display(DomainInfo * domaininfo) {
    printf("[i] Domain Name: %s\n", domaininfo->domainname);
    printf("[i] Function resolve(): %p\n", domaininfo->resolve);
    printf("[i] Function display(): %p\n", domaininfo->display);
    printf("[i] IPv4 Address: %s\n", domaininfo->ipv4);
    printf("[i] IPv6 Address: %s\n", domaininfo->ipv6);
}

// Initialize new struct
DomainInfo * init(char *domainname){
    size_t _size  = sizeof(DomainInfo);
    DomainInfo * _d_info = (DomainInfo *)calloc(1, _size);
    if (_d_info == NULL) {
        fprintf(stderr, "[!] Failed to allocate struct on the heap\n");
        return NULL;
    }
    _d_info->resolve = resolve;
    _d_info->display = display;
    strcpy(_d_info->domainname, domainname); // Yes, i know bofs exist, but eh :P
    return _d_info;
}

int main(int argc, char * argv[]) {
    printf("[i] Resolve IPv4 from Domain Name in C - OOPs style\n");
    
    // Check Command line arguments
    if (argc!=2) {
        fprintf(stderr, "[!] Usage: %s <domain name>\n", argv[0]);
        return -1;
    }

    // initialize new struct
    DomainInfo * d_info = init(argv[1]);
    if (d_info == NULL) {
        return -1;
    }
    // Resolve domain name
    int _res = d_info->resolve(d_info);
    // Check results
    if (_res != 0) {
        free(d_info);
        return -1;
    }
    // Display results
    d_info->display(d_info);
    // Free struct
    free(d_info);
    return 0;
}
