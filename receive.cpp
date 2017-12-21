#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <linux/can.h>
#include <linux/can/raw.h>
using namespace std;
int
main(void)
{

    int s1,s2;
    int nbytes;
    struct sockaddr_can addr;
    struct can_frame frame;
    struct ifreq ifr;

    const char *ifname = "can0";

    //port1
    if((s1 = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        perror("Error while opening socket \n "
               "try: sudo ip link set can0 up txqueuelen 1000 type can bitrate 1000000");
        return -1;
    }
    else{
        printf("socket at index %d\n", s1);
    }

    strcpy(ifr.ifr_name, ifname);
    ioctl(s1, SIOCGIFINDEX, &ifr);

    addr.can_family  = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    printf("%s at index %d\n", ifname, ifr.ifr_ifindex);

    if(bind(s1, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Error in socket bind");
        return -2;
    }
    //filters
    can_filter rfilter1[1];
    rfilter1[0].can_id   = 0x122;
    rfilter1[0].can_mask = 0x7F;
    setsockopt(s1, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter1, sizeof(rfilter1));



while (1)//(std::cin.get() != 'q')
{
    nbytes = read(s1, &frame, sizeof(struct can_frame));
    cout<< hex ;
    cout << "can_id " <<  frame.can_id << std::endl;
    cout << "can_dlc " << int(frame.can_dlc) << std::endl;
    cout << "data[0] " << int(frame.data[0]) << std::endl;
    cout << "data[1] " << int(frame.data[1]) << std::endl;
    cout<< dec ;
    printf("Read %d bytes\n", nbytes);



}
    return 0;
}
