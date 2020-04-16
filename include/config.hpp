#ifndef CONFIG_HPP
#define CONFIG_HPP

#pragma once

#include <string>
#include <map>

class config {
public:
    config();
    config(const std::string &);

    ~config();

    void set_file_name(const std::string &);

    std::string get_file_name() const;

    bool is_config() const;

    void add_param(const std::string &, const std::string &);

    void write();

    std::string get_param(const std::string &);

private:
    std::string file_name;

    std::map<std::string, std::string> params;
};

#endif // CONFIG_HPP
