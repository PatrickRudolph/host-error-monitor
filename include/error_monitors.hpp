/*
// Copyright (c) 2021 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
*/
#pragma once
#include <sdbusplus/asio/object_server.hpp>
#include <error_monitors/caterr_monitor.hpp>
#include <error_monitors/mcerr_monitor.hpp>
#include <error_monitors/err_pin_monitor.hpp>
#include <error_monitors/mem_thermtrip_monitor.hpp>
#include <error_monitors/memhot_monitor.hpp>
#include <error_monitors/cpu_thermtrip_monitor.hpp>
#include <error_monitors/mem_thermtrip_monitor.hpp>

// #include <error_monitors/smi_monitor.hpp>

#include <memory>

namespace host_error_monitor::error_monitors
{
// Error signals to monitor
// static std::unique_ptr<host_error_monitor::smi_monitor::SMIMonitor>
// smiMonitor;
static std::unique_ptr<host_error_monitor::err_pin_sample_monitor::CatErrMonitor>
caterrMonitor;
static std::unique_ptr<host_error_monitor::mcerr_monitor::MCERRMonitor>
mceerrMonitor;
static std::unique_ptr<host_error_monitor::err_pin_monitor::ErrPinMonitor>
errPin0Monitor;
static std::unique_ptr<host_error_monitor::err_pin_monitor::ErrPinMonitor>
errPin1Monitor;
static std::unique_ptr<host_error_monitor::err_pin_monitor::ErrPinMonitor>
errPin2Monitor;
static std::unique_ptr<host_error_monitor::memhot_monitor::MemhotMonitor>
memhot0Monitor;
static std::unique_ptr<host_error_monitor::memhot_monitor::MemhotMonitor>
memhot1Monitor;
static std::unique_ptr<host_error_monitor::memhot_monitor::MemhotMonitor>
memhot2Monitor;
static std::unique_ptr<host_error_monitor::memhot_monitor::MemhotMonitor>
memhot3Monitor;
#if 0
static std::unique_ptr<host_error_monitor::mem_thermtrip_monitor::MemThermtripMonitor>
memtrip0Monitor;
static std::unique_ptr<host_error_monitor::mem_thermtrip_monitor::MemThermtripMonitor>
memtrip1Monitor;
static std::unique_ptr<host_error_monitor::mem_thermtrip_monitor::MemThermtripMonitor>
memtrip2Monitor;
static std::unique_ptr<host_error_monitor::mem_thermtrip_monitor::MemThermtripMonitor>
memtrip3Monitor;

static std::unique_ptr<host_error_monitor::cpu_thermtrip_monitor::CPUThermtripMonitor>
cputrip0Monitor;
static std::unique_ptr<host_error_monitor::cpu_thermtrip_monitor::CPUThermtripMonitor>
cputrip1Monitor;
static std::unique_ptr<host_error_monitor::cpu_thermtrip_monitor::CPUThermtripMonitor>
cputrip2Monitor;
static std::unique_ptr<host_error_monitor::cpu_thermtrip_monitor::CPUThermtripMonitor>
cputrip3Monitor;
#endif

// Check if all the signal monitors started successfully
bool checkMonitors()
{
    bool ret = true;

    // ret &= smiMonitor->isValid();
    ret &= caterrMonitor->isValid();
    ret &= mceerrMonitor->isValid();
    ret &= errPin0Monitor->isValid();
    ret &= errPin1Monitor->isValid();
    ret &= errPin2Monitor->isValid();

    ret &= memhot0Monitor->isValid();
    ret &= memhot1Monitor->isValid();
    ret &= memhot2Monitor->isValid();
    ret &= memhot3Monitor->isValid();
#if 0
    ret &= memtrip0Monitor->isValid();
    ret &= memtrip1Monitor->isValid();
    ret &= memtrip2Monitor->isValid();
    ret &= memtrip3Monitor->isValid();


    ret &= cputrip0Monitor->isValid();
    ret &= cputrip1Monitor->isValid();
    ret &= cputrip2Monitor->isValid();
    ret &= cputrip3Monitor->isValid();
#endif
    return ret;
}

// Start the signal monitors
bool startMonitors(
    [[maybe_unused]] boost::asio::io_context& io,
    [[maybe_unused]] std::shared_ptr<sdbusplus::asio::connection> conn)
{
    caterrMonitor = std::make_unique<host_error_monitor::err_pin_sample_monitor::CatErrMonitor>(
         io, conn, "FM_CPU_CATERR_LVT3_N");

    mceerrMonitor = std::make_unique<host_error_monitor::mcerr_monitor::MCERRMonitor>(
         io, conn, "FM_CPU_RMCA_LVT3_N", host_error_monitor::base_gpio_monitor::AssertValue::lowAssert);

    errPin0Monitor = std::make_unique<host_error_monitor::err_pin_monitor::ErrPinMonitor>(
         io, conn, "FM_CPU_ERR0_LVT3_N", 0);

    errPin1Monitor = std::make_unique<host_error_monitor::err_pin_monitor::ErrPinMonitor>(
         io, conn, "FM_CPU_ERR1_LVT3_N", 1);

    errPin2Monitor = std::make_unique<host_error_monitor::err_pin_monitor::ErrPinMonitor>(
         io, conn, "FM_CPU_ERR2_LVT3_N", 2);

    memhot0Monitor = std::make_unique<host_error_monitor::memhot_monitor::MemhotMonitor>(
         io, conn, "H_LVT3_CPU0_MEMHOT_OUT_N", 0);

    memhot1Monitor = std::make_unique<host_error_monitor::memhot_monitor::MemhotMonitor>(
         io, conn, "H_LVT3_CPU1_MEMHOT_OUT_N", 1);

    memhot2Monitor = std::make_unique<host_error_monitor::memhot_monitor::MemhotMonitor>(
         io, conn, "H_LVT3_CPU2_MEMHOT_OUT_N", 2);

    memhot3Monitor = std::make_unique<host_error_monitor::memhot_monitor::MemhotMonitor>(
         io, conn, "H_LVT3_CPU3_MEMHOT_OUT_N", 3);
#if 0
    memtrip0Monitor = std::make_unique<host_error_monitor::mem_thermtrip_monitor::MemThermtripMonitor>(
         io, conn, "H_LVT3_CPU0_MEMTRIP_OUT_N", 0);

    memtrip1Monitor = std::make_unique<host_error_monitor::mem_thermtrip_monitor::MemThermtripMonitor>(
         io, conn, "H_LVT3_CPU1_MEMTRIP_OUT_N", 1);

    memtrip2Monitor = std::make_unique<host_error_monitor::mem_thermtrip_monitor::MemThermtripMonitor>(
         io, conn, "H_LVT3_CPU2_MEMTRIP_OUT_N", 2);

    memtrip3Monitor = std::make_unique<host_error_monitor::mem_thermtrip_monitor::MemThermtripMonitor>(
         io, conn, "H_LVT3_CPU3_MEMTRIP_OUT_N", 3);


    cputrip0Monitor = std::make_unique<host_error_monitor::cpu_thermtrip_monitor::CPUThermtripMonitor>(
         io, conn, "H_LVT3_CPU0_THERMTRIP_OUT_N", 0);

    cputrip1Monitor = std::make_unique<host_error_monitor::cpu_thermtrip_monitor::CPUThermtripMonitor>(
         io, conn, "H_LVT3_CPU1_THERMTRIP_OUT_N", 1);

    cputrip2Monitor = std::make_unique<host_error_monitor::cpu_thermtrip_monitor::CPUThermtripMonitor>(
         io, conn, "H_LVT3_CPU2_THERMTRIP_OUT_N", 2);

    cputrip3Monitor = std::make_unique<host_error_monitor::cpu_thermtrip_monitor::CPUThermtripMonitor>(
         io, conn, "H_LVT3_CPU3_THERMTRIP_OUT_N", 3);
#endif
    // std::make_unique<host_error_monitor::smi_monitor::SMIMonitor>(
    //     io, conn, "SMI");

    return checkMonitors();
}

// Notify the signal monitors of host on event
void sendHostOn()
{
    // smiMonitor->hostOn();
    caterrMonitor->hostOn();
    mceerrMonitor->hostOn();
    errPin0Monitor->hostOn();
    errPin1Monitor->hostOn();
    errPin2Monitor->hostOn();

    memhot0Monitor->hostOn();
    memhot1Monitor->hostOn();
    memhot2Monitor->hostOn();
    memhot3Monitor->hostOn();
#if 0
    memtrip0Monitor->hostOn();
    memtrip1Monitor->hostOn();
    memtrip2Monitor->hostOn();
    memtrip3Monitor->hostOn();

    cputrip0Monitor->hostOn();
    cputrip1Monitor->hostOn();
    cputrip2Monitor->hostOn();
    cputrip3Monitor->hostOn();
#endif
}

} // namespace host_error_monitor::error_monitors
