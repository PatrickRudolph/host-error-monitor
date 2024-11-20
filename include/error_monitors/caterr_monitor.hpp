/*
// Copyright (c) 2024 9elements GmbH
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
#include <systemd/sd-journal.h>
#include <boost/asio/steady_timer.hpp>

#include <error_monitors/base_gpio_monitor.hpp>
#include <host_error_monitor.hpp>
#include <sdbusplus/asio/object_server.hpp>

#include <bitset>

namespace host_error_monitor::err_pin_sample_monitor
{
static constexpr bool debug = false;

class ErrPinLatchMonitor :
    public host_error_monitor::base_gpio_monitor::BaseGPIOMonitor
{
    const static host_error_monitor::base_gpio_monitor::AssertValue
        assertValue =
            host_error_monitor::base_gpio_monitor::AssertValue::lowAssert;

    void assertHandler() override
    {
        asserted = true;
    }

    void deassertHandler() override
    {
    }

  public:
    bool asserted;
    const std::string& signalName;

    ErrPinLatchMonitor(boost::asio::io_context& io,
                       std::shared_ptr<sdbusplus::asio::connection> conn,
                       const std::string& signalName) :
        BaseGPIOMonitor(io, conn, signalName, assertValue), asserted(false),
        signalName(signalName)
    {
        if (valid)
        {
            startMonitoring();
        }
    }

    void reset()
    {
        asserted = false;
    }

};

class CatErrMonitor :
    public host_error_monitor::base_gpio_monitor::BaseGPIOMonitor
{
    const static host_error_monitor::base_gpio_monitor::AssertValue
        assertValue =
            host_error_monitor::base_gpio_monitor::AssertValue::lowAssert;
    std::vector<std::unique_ptr<ErrPinLatchMonitor>> errPins;
    boost::asio::steady_timer pollingTimer;

    size_t pollingTimeMs;
    int cpuNum;

    void catErrLog(std::string err)
    {
        std::string msg = "CATERR on CPU " + std::to_string(cpuNum) + " " + err;

        log_message(LOG_INFO, msg, "OpenBMC.0.1.CPUError", msg);
    }

    void poll()
    {
        std::string msg;

        for (auto& pin : errPins) {
            if (msg.length())
                msg += ", ";
            msg += pin->signalName + "=" + (pin->asserted ? "0" : "1");
            pin->reset();
        }

        catErrLog(msg);
    }

    void assertHandler() override
    {
        pollingTimer.expires_after(std::chrono::milliseconds(pollingTimeMs));
        pollingTimer.async_wait([this](const boost::system::error_code ec) {
            if (ec)
            {
                // operation_aborted is expected if timer is canceled before
                // completion.
                if (ec != boost::asio::error::operation_aborted)
                {
                    std::cerr << signalName << " polling async_wait failed: "
                              << ec.message() << "\n";
                }
                return;
            }
            poll();
        });
    }

    void deassertHandler() override
    {
    }

  public:
    CatErrMonitor(boost::asio::io_context& io,
                  std::shared_ptr<sdbusplus::asio::connection> conn,
                  std::vector<std::string>& errSignalNames,
                  const std::string& catErrSignalName,
                  const int cpuNum) :
        BaseGPIOMonitor(io, conn, catErrSignalName, assertValue), pollingTimer(io),
        pollingTimeMs(100), cpuNum(cpuNum)
    {
        for (auto& name : errSignalNames) {
            errPins.emplace_back(std::make_unique<ErrPinLatchMonitor>(io, conn, name));
        }

        if (valid)
        {
            startMonitoring();
        }
    }

};
} // namespace host_error_monitor::err_pin_sample_monitor
