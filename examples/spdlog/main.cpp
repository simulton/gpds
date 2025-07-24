#include <gpds/spdlog_sink.hpp>
#include <gpds/archiver_xml.hpp>
#include <spdlog/spdlog.h>

#include <println>
#include <sstream>

int main()
{
    // Create spdlog gpds sink
    auto sink = std::make_shared<gpds::spdlog_sink_mt>();

    // Create spdlog logger
    spdlog::logger logger("", { sink });
    logger.info("Hello World: {}", 42);
    logger.warn("This is a warning");
    logger.error("Error during processing: {}", "something weird happened");
    logger.critical("Oh no! We got an exception: 0x{:x} - {}", 0x48, "generic exception");

    // Serialize
    std::stringstream ss;
    {
        const auto success  = gpds::to_stream<gpds::archiver_xml>(ss, *sink, "log");
        if (!success) {
            std::println("serializing log failed: {}", success.error().message());
            return EXIT_FAILURE;
        }
    }
    std::println("{}", ss.str());

    // Deserialize
    {
        gpds::spdlog_sink_mt loaded_sink;

        const auto success = gpds::from_stream<gpds::archiver_xml>(ss, loaded_sink, "log");
        if (!success) {
            std::println("deserializing log failed: {}", success.error().message());
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
