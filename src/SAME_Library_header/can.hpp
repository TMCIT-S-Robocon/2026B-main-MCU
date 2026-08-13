/*******************************************************************************
 *  File:        can.hpp
 *  Module:      CAN Abstraction Layer
 *
 *  Summary:
 *    Hardware-independent CAN interface for embedded firmware and ROS 2.
 *
 *  Description:
 *    This header defines the base CAN interface and platform-specific
 *    implementations. It is designed to share identical application-layer
 *    behavior between bare-metal firmware and ROS 2 environments.
 *
 *  Attention:
 *    This file is part of taku-256's personal library or SAME library.
 *    "SAME" does not mean "similar"; in Japanese it means "shark".
 *    The actual pronunciation may differ from what you expect.
 *
 *******************************************************************************/

/*******************************************************************************
 *  Copyright (C) 2025 taku-256
 *
 *  License:
 *    This file is part of the SAME library.
 *    The license of this file follows the license of SAME library.
 *    Refer to the top-level LICENSE file for details.
 *
 *  This software is provided "AS IS", without warranty of any kind.
 *  The author shall not be held liable for any damages arising from the use
 *  of this software.
 ******************************************************************************/

#ifndef SAME_LIBRARY__CAN_HPP_
#define SAME_LIBRARY__CAN_HPP_

#include <stdint.h>

#include <vector>

class CAN_base {
public:
    virtual void Transmit() = 0;
    struct {
        uint32_t id;
        uint8_t dlc;
        std::vector<uint8_t> data;
    } tx;
    struct {
        uint32_t id;
        uint8_t dlc;
        std::vector<uint8_t> data;
    } rx;
};

#if defined(__has_include)
#if __has_include("rclcpp/rclcpp.hpp")
#include "rclcpp/rclcpp.hpp"
#endif
#if __has_include("same_interface/msg/can_msg.hpp")
#include "same_interface/msg/can_msg.hpp"
#endif
#if __has_include("definitions.h")
#include "definitions.h"
#endif
#endif

// ROS2 Humble CAN
#if defined(RCLCPP__RCLCPP_HPP_) && defined(SAME_INTERFACE__MSG__CAN_MSG_HPP_)
using namespace std::chrono_literals;
using namespace std::placeholders;
class ROS_CAN : public CAN_base {
public:
    std::function<void()> user_callback = nullptr;
    void init(rclcpp::Node* node, const std::string& topic_path, std::function<void()> callback = nullptr, bool sub = false) {
        tx.data.resize(8);
        rx.data.resize(8);
        publisher_ = node->create_publisher<same_interface::msg::CanMsg>(topic_path + "/can_tx", 20);
        if (!callback && !sub) {
            return;
        }
        user_callback = callback;
        subscription_ = node->create_subscription<same_interface::msg::CanMsg>(topic_path + "/can_rx", 1, [this](const same_interface::msg::CanMsg::SharedPtr msg) {
            rx.id = msg->id;
            rx.dlc = msg->dlc;
            for (uint8_t i = 0; i < ((msg->dlc) < (8) ? (msg->dlc) : (8)); i++) {
                rx.data[i] = msg->data[i];
            }

            if (this->user_callback) {
                this->user_callback();
            }
        });
    }
    void Transmit() override {
        auto tx_message = std::make_shared<same_interface::msg::CanMsg>();
        tx_message->id = tx.id;
        tx_message->dlc = tx.dlc;
        tx_message->data.resize(tx.dlc);
        for (uint8_t i = 0; i < tx.dlc; i++) {
            tx_message->data[i] = tx.data[i];
        }
        publisher_->publish(*tx_message);
    }
    void set_callback(std::function<void()> callback) { user_callback = callback; }

private:
    rclcpp::Publisher<same_interface::msg::CanMsg>::SharedPtr publisher_;
    rclcpp::Subscription<same_interface::msg::CanMsg>::SharedPtr subscription_;
};
#endif

#if defined(PLIB_CANFD1_H)

class CAN1_class : public CAN_base {
public:
    void init();
    void Transmit();
    uintptr_t not_use = 1;
    CANFD_MSG_RX_ATTRIBUTE msgAttr;
};

extern CAN1_class CAN1;

#endif

#if defined(PLIB_CANFD2_H)

class CAN2_class : public CAN_base {
public:
    void init();
    void Transmit();
    uintptr_t not_use = 1;
    CANFD_MSG_RX_ATTRIBUTE msgAttr;
};

extern CAN2_class CAN2;

#endif

#if defined(PLIB_CANFD3_H)

class CAN3_class : public CAN_base {
public:
    void init();
    void Transmit();
    uintptr_t not_use = 1;
    CANFD_MSG_RX_ATTRIBUTE msgAttr;
};

extern CAN3_class CAN3;

#endif

#if defined(PLIB_CANFD4_H)

class CAN4_class : public CAN_base {
public:
    void init();
    void Transmit();
    uintptr_t not_use = 1;
    CANFD_MSG_RX_ATTRIBUTE msgAttr;
};

extern CAN4_class CAN4;

#endif

#endif /*SAME_LIBRARY__CAN_HPP_*/

/*******************************************************************************
 End of File
*/
