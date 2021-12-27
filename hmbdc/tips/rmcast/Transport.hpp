#include "hmbdc/Copyright.hpp"
#pragma once

#include "hmbdc/tips/TypeTagSet.hpp"
#include "hmbdc/app/Config.hpp"
#include "hmbdc/comm/inet/Misc.hpp"
#include "hmbdc/Compile.hpp"

#include <memory>
#include <string>

namespace hmbdc { namespace tips { namespace rmcast {

namespace transport_detail {
struct Transport {
    using ptr = std::shared_ptr<Transport>;

    Transport(app::Config const& cfg)
    : config_(cfg) 
    , mtu_(config_.getExt<size_t>("mtu")) {
        mtu_ -= (8u + 20u);  // 8bytes udp header and 20bytes ip header
    }

    bool operator == (Transport const& other ) const {
        return &config_ == &other.config_;
    }

    bool operator < (Transport const& other ) const {
        return &config_ < &other.config_;
    }

    Transport (Transport const&) = delete;
    Transport& operator = (Transport const&) = delete;
    virtual ~Transport() = default;
    
protected:
    app::Config const config_;
    size_t mtu_;
};

struct EngineTransport : Transport {
    EngineTransport(app::Config const& cfg)
    : Transport(cfg) {
        cfg (hmbdcName_, "hmbdcName")
            (schedPolicy_, "schedPolicy")
            (schedPriority_, "schedPriority")
        ;
    }

protected:
    char const* hmbdcName() const { 
        return this->hmbdcName_.c_str();
    }
    std::string hmbdcName_;
    std::string schedPolicy_;
    int schedPriority_;
};
} //transport_detail

using Transport = transport_detail::Transport;
using EngineTransport = transport_detail::EngineTransport;
}}}
