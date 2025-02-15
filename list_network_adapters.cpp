#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    struct ifaddrs *addresses;

    if (getifaddrs(&addresses) == -1) {
        cerr << "getifaddrs call failed\n";
        return -1;
    }

    std::unique_ptr<struct ifaddrs, decltype(&freeifaddrs)> addr_list{addresses,freeifaddrs};

    for (auto address = addr_list.get(); address; address = address->ifa_next) {
        if (!address->ifa_addr) continue;

        const auto family = address->ifa_addr->sa_family;
        if (family == AF_INET || family == AF_INET6) {
            std::cout << address->ifa_name << "\t"
                      << (family == AF_INET ? "IPv4" : "IPv6") << "\t";

            char ap[100];
            const auto family_size = (family == AF_INET) 
                                      ? sizeof(struct sockaddr_in) 
                                      : sizeof(struct sockaddr_in6);

            if (getnameinfo(address->ifa_addr, family_size, ap, sizeof(ap), nullptr, 0, NI_NUMERICHOST) == 0) {
                std::cout << ap << "\n";
            } else {
                std::cerr << "Error getting address info\n";
            }
        }
    }
    
    return EXIT_SUCCESS;
}
