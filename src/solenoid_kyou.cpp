#include "xc.h"
#include "solenoid_kyou.hpp"
#include "config/default/peripheral/canfd/plib_canfd4.h"
#include "delay.hpp"

void solenoid_kyou_individual(uint8_t port, bool status){ // 個別ポート指定制御
    uint8_t data[2] = {0};
    if(status == true){
        data[1] = 1;
    } else{
        data[1] = 0;
    }
    
    if(port > 8){
        port = port - 8;
        data[0] = port;
        CAN4_MessageTransmit(0x62d, 2, data, 1, CANFD_MODE_NORMAL, CANFD_MSG_TX_DATA_FRAME);
    } else{
        data[0] = port;
        CAN4_MessageTransmit(0x623, 2, data, 1, CANFD_MODE_NORMAL, CANFD_MSG_TX_DATA_FRAME);
    }
}

void solenoid_kyou_all(bool status){
    uint8_t data[1] = {0};
    data[0] = status;
    
    CAN4_MessageTransmit(0x622, 2, data, 1, CANFD_MODE_NORMAL, CANFD_MSG_TX_DATA_FRAME);
}