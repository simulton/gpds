#include <gpds/spdlog_sink.hpp>
#include <gpds/archiver_xml.hpp>
#include <spdlog/spdlog.h>

#include <iostream>

int main()
{
    // Create spdlog gpds sink
    auto sink = std::make_shared<gpds::spdlog_sink_mt>();

    // Create spdlog logger
    spdlog::logger logger("", { sink });
    logger.info("Hello World: {}", 42);

    // Show output
    gpds::archiver_xml ar;
    ar.save(std::cout, *sink, "log");

    return 0;
}
