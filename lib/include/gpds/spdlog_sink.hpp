#pragma once

#include "serialize.hpp"

#include <spdlog/details/null_mutex.h>
#include <spdlog/sinks/base_sink.h>

#include <mutex>

namespace gpds
{

    template<typename Mutex>
    class spdlog_sink :
        public gpds::serialize,
        public spdlog::sinks::base_sink<Mutex>
    {
    public:
        spdlog_sink() = default;
        ~spdlog_sink() override = default;

        [[nodiscard]]
        gpds::container to_container() const override
        {
            return m_container;
        }

        void from_container(const gpds::container& c) override
        {
            m_container = c;
        }

    protected:
        void sink_it_(const spdlog::details::log_msg& msg) override
        {
            std::string level_name{spdlog::level::to_string_view(msg.level).data(), spdlog::level::to_string_view(msg.level).size()};

            // Add to container
            gpds::container c;
            c.add_value("logger", std::string{msg.logger_name.data(), msg.logger_name.size()});
            c.add_value("level", static_cast<int>(msg.level)).add_attribute("name", level_name);
            c.add_value("thread_id", static_cast<int>(msg.thread_id));
            c.add_value("msg", std::string{msg.payload.data(), msg.payload.size()});

            m_container.add_value("message", c);
        }

        void flush_() override
        {
            // Nothing to do here
        }

    private:
        container m_container;
    };

    using spdlog_sink_mt = spdlog_sink<std::mutex>;
    using spdlog_sink_st = spdlog_sink<spdlog::details::null_mutex>;

}
