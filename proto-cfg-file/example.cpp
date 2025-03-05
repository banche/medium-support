#include "google/protobuf/text_format.h"
#include "google/protobuf/util/json_util.h"
#include <google/protobuf/io/tokenizer.h>

#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string_view>

// generated files
#include "config.pb.h"

class Reader
{
public:
    template <typename T>
    static bool parseFromFile(std::string_view filename, T* msg);

private:
    static std::string readFile(std::string_view filename);

    template <typename T>
    static bool parseFromTextFile(std::string_view filename, T* msg);

    template <typename T>
    static bool parseFromJsonFile(std::string_view filename, T* msg);

    class ErrorCollector : public google::protobuf::io::ErrorCollector
    {
    public:
        ErrorCollector(std::string_view filename)
            : m_filename(filename)
        {
        }

        ~ErrorCollector() override = default;

        void AddError(int line, google::protobuf::io::ColumnNumber column, const std::string& msg) override
        {
            m_hasErrors = true;
            std::cerr << std::format("Error when parsing '{}' at {}:{} details '{}", m_filename, line, column, msg)
                      << '\n';
        }

        void AddWarning(int line, google::protobuf::io::ColumnNumber column, const std::string& msg) override
        {
            m_hasErrors = true;
            std::cerr << std::format("Warning when parsing '{}' at {}:{} details '{}", m_filename, line, column, msg)
                      << '\n';
        }

        bool hasErrors() const { return m_hasErrors; }

    private:
        std::string_view m_filename;
        bool m_hasErrors{false};
    };
};

template <typename T>
bool Reader::parseFromTextFile(std::string_view filename, T* msg)
{
    const std::string cfgMsg = readFile(filename);

    google::protobuf::TextFormat::Parser parser;
    ErrorCollector errorCollector(filename);
    parser.RecordErrorsTo(&errorCollector);
    return parser.ParseFromString(cfgMsg, msg);
}

template <typename T>
bool Reader::parseFromJsonFile(std::string_view filename, T* msg)
{
    const std::string cfgMsg = readFile(filename);

    const auto status = google::protobuf::util::JsonStringToMessage(cfgMsg, msg);
    if (not status.ok())
    {
        std::cerr << "Failed to parse '" << filename << "' details '" << status.ToString() << "'\n";
    }
    return status.ok();
}

template <typename T>
bool Reader::parseFromFile(std::string_view filename, T* msg)
{
    using std::operator""sv;
    constexpr auto delim{"."sv};
    constexpr auto textPart{"txtpb"sv};
    constexpr auto jsonPart{"json"sv};

    bool isText = false;
    bool isJson = false;
    for (const auto part : std::views::split(filename, delim))
    {
        std::string_view view(std::ranges::data(part), std::ranges::size(part));
        if (textPart == view)
        {
            isText = true;
        }
        else if (jsonPart == view)
        {
            isJson = true;
        }
    }

    if (not(isText xor isJson))
    {
        std::cout << std::format(
            "configFilename '{}' has an invalid name. Expecting a single txtpb or json extension\n", filename);
        return false;
    }

    if (isText)
    {
        return parseFromTextFile(filename, msg);
    }
    else
    {
        return parseFromJsonFile(filename, msg);
    }
}

std::string Reader::readFile(std::string_view filename)
{
    if (not std::filesystem::exists(filename))
    {
        std::cout << std::format("Error file '{}' does not exist\n", filename);
        return {};
    }

    std::ifstream input(filename.data(), std::ios::in);

    if (not input.is_open())
    {
        std::cout << std::format("Error could not open file '{}'\n", filename);
        return {};
    }

    std::stringstream buffer;
    buffer << input.rdbuf();

    return buffer.str();
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "please supply a configuration filename\n";
        return 1;
    }

    const std::string filename = argv[1];

    config::Config cfg;
    Reader::parseFromFile(filename, &cfg);

    std::cout << cfg.DebugString() << '\n';

    return 0;
}