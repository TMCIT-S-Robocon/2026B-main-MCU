#include "xc.h"
#include "solenoid_kyou.hpp"
#include "config/default/peripheral/canfd/plib_canfd4.h"

void solenoid_kyou(uint8_t port, bool status){ // 個別ポート指定制御
    uint8_t data[2] = {0};
    data[0] = port;
    if(status == true){
        data[1] = 1;
    } else{
        data[1] = 0;
    }
    
    CAN4_MessageTransmit(0x623, 2, data, 1, CANFD_MODE_NORMAL, CANFD_MSG_TX_DATA_FRAME);
}
