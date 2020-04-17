#include "../include/config.hpp"

#include <fstream>

config::config() : file_name("") { add_param("encoding", "UTF-8"); }
config::config(const std::string& name) { add_param("encoding", "UTF-8"); set_file_name(name); }

config::~config() { }

void config::set_file_name(const std::string& name) {
    file_name = name;

    std::ifstream fin(name);

<<<<<<< HEAD
    if(!fin.is_open()) return;
=======
    if (!fin.is_open()) return;
>>>>>>> ba6338c4f58f58d04c8d62a7452b5741ba11f432

    std::string tmp;

    while (!fin.eof()) {
        getline(fin, tmp);

        if (tmp.find("\"") != std::string::npos) {
            int i = 0;

            while (tmp.at(i) != '"') i++;

            tmp = tmp.substr(i + 1);

            i = 0;

            while (tmp.at(i) != '"') i++;

            std::string tmp_s = tmp.substr(0, i);

            tmp = tmp.substr(i + 1);

            i = 0;

            while (tmp.at(i) != '"') i++;

            tmp = tmp.substr(i + 1);

            i = 0;

            while (tmp.at(i) != '"') i++;

            tmp = tmp.substr(0, i);

            add_param(tmp_s, tmp);
        }
    }
}

std::string config::get_file_name() const {
    return file_name;
}

bool config::is_config() const {
    if (file_name == "") return false;

    std::ifstream fin(file_name);

    if (fin.is_open()) {
        fin.close();
        return true;
    }
    else {
        fin.close();
        return false;
    }
}

void config::add_param(const std::string& param, const std::string& value) {
    if (params.find(param) != params.end()) {
        params.erase(params.find(param));
    }

    params.insert(std::make_pair(param, value));
}

void config::write() {
    std::ofstream fout(file_name, std::ios_base::trunc);

    fout << "{\n";

    auto iter = params.begin();

    while (iter != params.end()) {
        fout << "\t\"" << iter->first << "\": \"" << iter->second << "\"";

        iter++;

        if (iter == params.end()) fout << "\n";
        else fout << ",\n";
    }

    fout << "}";

    fout.close();
}

std::string config::get_param(const std::string& param) {
    if (params.find(param) == params.end()) return "UNDEFINDED";

    return params.find(param)->second;
}
