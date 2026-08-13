/*
 * File:   can.cpp
 * Author: takum
 *
 * Created on May 19, 2025, 10:18 AM
 */

#include "../SAME_Library_header/can.hpp"
#include "../SAME_Library_header/shotacon.hpp"

extern Shotacon controller;

#if defined(PLIB_CANFD1_H)
CAN1_class CAN1;

void can1_tx_callback(uintptr_t){}

void can1_rx_callback(uintptr_t){
    CAN1_MessageReceive(&CAN1.rx.id, &CAN1.rx.dlc, CAN1.rx.data.data(), 0, 2, &CAN1.msgAttr);
    switch(CAN1.rx.id){
        case 0x602:
            controller.readCAN();
            break;
        default:
		    break;
    }
}

void CAN1_class::init(){
    for(int i=0;i<8;i++){
        tx.data.push_back(0);
        rx.data.push_back(0);
    }
    CAN1_CallbackRegister(can1_tx_callback,not_use,1);
    CAN1_CallbackRegister(can1_rx_callback,not_use,2);
    CAN1_MessageReceive(&rx.id, &rx.dlc, rx.data.data(), 0, 2, &msgAttr);
}

void CAN1_class::Transmit(){
    CAN1_MessageTransmit(tx.id,tx.dlc,tx.data.data(),1,CANFD_MODE_NORMAL,CANFD_MSG_TX_DATA_FRAME);
}
#endif

#if defined(PLIB_CANFD2_H)
CAN2_class CAN2;

void can2_tx_callback(uintptr_t){}

void can2_rx_callback(uintptr_t){
    CAN2_MessageReceive(&CAN2.rx.id, &CAN2.rx.dlc, CAN2.rx.data.data(), 0, 2, &CAN2.msgAttr);
    switch(CAN2.rx.id){
		default:
			break;
    }
}

void CAN2_class::init(){
    for(int i=0;i<8;i++){
        tx.data.push_back(0);
        rx.data.push_back(0);
    }
    CAN2_CallbackRegister(can2_tx_callback,not_use,1);
    CAN2_CallbackRegister(can2_rx_callback,not_use,2);
    CAN2_MessageReceive(&rx.id, &rx.dlc, rx.data.data(), 0, 2, &msgAttr);
}

void CAN2_class::Transmit(){
    CAN2_MessageTransmit(tx.id,tx.dlc,tx.data.data(),1,CANFD_MODE_NORMAL,CANFD_MSG_TX_DATA_FRAME);
}

#endif

#if defined(PLIB_CANFD3_H)

CAN3_class CAN3;

void can3_tx_callback(uintptr_t){}

void can3_rx_callback(uintptr_t){
    CAN3_MessageReceive(&CAN3.rx.id, &CAN3.rx.dlc, CAN3.rx.data.data(), 0, 2, &CAN3.msgAttr);
    switch(CAN3.rx.id){
        default:
            break;
    }
}

void CAN3_class::init(){
    for(int i=0;i<8;i++){
        tx.data.push_back(0);
        rx.data.push_back(0);
    }
    CAN3_MessageReceive(&rx.id, &rx.dlc, rx.data.data(), 0, 2, &msgAttr);
    CAN3_CallbackRegister(can3_tx_callback,not_use,1);
    CAN3_CallbackRegister(can3_rx_callback,not_use,2);
}

void CAN3_class::Transmit(){
    CAN3_MessageTransmit(tx.id,tx.dlc,tx.data.data(),1,CANFD_MODE_NORMAL,CANFD_MSG_TX_DATA_FRAME);
}

#endif

#if defined(PLIB_CANFD4_H)

CAN4_class CAN4;

void can4_tx_callback(uintptr_t){}

void can4_rx_callback(uintptr_t){
    CAN4_MessageReceive(&CAN4.rx.id, &CAN4.rx.dlc, CAN4.rx.data.data(), 0, 2, &CAN4.msgAttr);
    switch(CAN4.rx.id){
        case 0x602:
            controller.readCAN();
            break;
        default:
		    break;
    }
}

void CAN4_class::init(){
    for(int i=0;i<8;i++){
        tx.data.push_back(0);
        rx.data.push_back(0);
    }
    CAN4_MessageReceive(&rx.id, &rx.dlc, rx.data.data(), 0, 2, &msgAttr);
    CAN4_CallbackRegister(can4_tx_callback,not_use,1);
    CAN4_CallbackRegister(can4_rx_callback,not_use,2);
}

void CAN4_class::Transmit(){
    CAN4_MessageTransmit(tx.id,tx.dlc,tx.data.data(),1,CANFD_MODE_NORMAL,CANFD_MSG_TX_DATA_FRAME);
}

#endif
