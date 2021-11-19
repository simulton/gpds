#include <gpds/spdlog_sink.hpp>
#include <gpds/archiver_xml.hpp>
#include <spdlog/spdlog.h>

#include <iostream>
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
    logger.critical("Oh no! We got an exception: {} - {}", 0x48, "generic exception");

    // Serialize
    std::stringstream ss;
    {
        gpds::archiver_xml ar;
        if (!ar.save(ss, *sink, "log")) {
            std::cerr << "serializing log failed." << std::endl;
            return EXIT_FAILURE;
        }
    }
    std::cout << ss.str() << std::endl;

    // Deserialize
    {
        gpds::spdlog_sink_mt loaded_sink;

        gpds::archiver_xml ar;
        if (!ar.load(ss, loaded_sink, "log")) {
            std::cerr << "deserializing log failed." << std::endl;
            return EXIT_FAILURE;
        }

        ar.save(std::cout, loaded_sink, "log_deserialized");
    }

    return EXIT_SUCCESS;
}
