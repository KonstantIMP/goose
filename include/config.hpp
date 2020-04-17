#ifndef CONFIG_HPP
#define CONFIG_HPP

#pragma once

#if defined(linux) || defined(__linux)
    #define CONFIGLIB_API
#else
    #ifdef CONFIGLIB_EXPORTS
    #define CONFIGLIB_API __declspec(dllexport)
    #else
    #define CONFIGLIB_API __declspec(dllimport)
    #endif
#endif

#include <string>
#include <map>

extern "C" {

    class config {
    public:
        CONFIGLIB_API config();
        CONFIGLIB_API config(const std::string &);

        CONFIGLIB_API ~config();

        CONFIGLIB_API void set_file_name(const std::string &);

        CONFIGLIB_API std::string get_file_name() const;

        CONFIGLIB_API bool is_config() const;

        CONFIGLIB_API void add_param(const std::string &, const std::string &);

        CONFIGLIB_API void write();

        CONFIGLIB_API std::string get_param(const std::string &);

    private:
        std::string file_name;

        std::map<std::string, std::string> params;
    };

}

#endif // CONFIG_HPP
