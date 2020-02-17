#include <stdio.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <arpa/inet.h>


int main(int argc, char ** argv){

    char** pp;
    struct in_addr addr;
    struct hostent* hostp;

    if (argc < 2){
        printf("USAGE: ./hostinfo (domain name or dotted decimal)\n");
        return 0;
    }

    // get host entry
    if (inet_aton(argv[1], &addr) != 0){
        hostp = gethostbyaddr((const char*)&addr, sizeof(addr), AF_INET);  // if given as dotted string
    } else {
        hostp = gethostbyname(argv[1]);     // if given as host name
    }

    printf("Official domain name: %s\n", hostp->h_name);

    // print list of domain aliases

    printf("Aliases------------\n");
    for (pp = hostp->h_aliases; *pp != NULL; pp++){
        printf("alias: %s\n", *pp);
    }

    // address list
    printf("\nAddress List------------\n");
    for (pp = hostp->h_addr_list; *pp != NULL; pp++) {
        addr.s_addr = ((struct in_addr *)*pp)->s_addr;
        printf("%s\n", inet_ntoa(addr));
    }

    return 0;
}
