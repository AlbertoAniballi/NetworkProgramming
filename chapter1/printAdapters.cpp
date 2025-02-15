#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include "/home/alberto/Documents/CPP_BOOKS_NOTES/NetworkProgramming/chapter1/printAdapters.h"

using namespace std;

void printAdapters_CPP() {
    struct ifaddrs *addresses;
    if (getifaddrs(&addresses) == -1) {
        cerr << "getifaddrs call failed\n";
        return;
    }

    unique_ptr<struct ifaddrs, decltype(&freeifaddrs)> addr_list{addresses, freeifaddrs};

    for (auto address = addr_list.get(); address; address = address->ifa_next) {
        if (!address->ifa_addr) continue;

        const auto family = address->ifa_addr->sa_family;
        if (family == AF_INET || family == AF_INET6) {
            cout << address->ifa_name << "\t" << (family == AF_INET ? "IPv4" : "IPv6") << "\t";

            char ap[100];
            const auto family_size = (family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);

            if (getnameinfo(address->ifa_addr, family_size, ap, sizeof(ap), nullptr, 0, NI_NUMERICHOST) == 0) {
                cout << ap << "\n";
            } else {
                cerr << "Error getting address info\n";
            }
        }
    }
}

void printAdapters_C() {
    struct ifaddrs *addresses;
    if (getifaddrs(&addresses) == -1) {
        cout << "getifaddrs call failed\n";
        return;
    }

    struct ifaddrs *address = addresses;
    while (address) {
        if (address->ifa_addr == NULL) { 
            address = address->ifa_next;
            continue;
        }
        int family = address->ifa_addr->sa_family;
        if (family == AF_INET || family == AF_INET6) {
            cout << address->ifa_name << "\t" << (family == AF_INET ? "IPv4" : "IPv6") << "\t";

            char ap[100];
            const int family_size = (family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);

            if (getnameinfo(address->ifa_addr, family_size, ap, sizeof(ap), 0, 0, NI_NUMERICHOST) == 0) {
                cout << ap << "\n";
            } else {
                cerr << "Error getting address info\n";
            }
        }
        address = address->ifa_next;
    }

    freeifaddrs(addresses);
}
