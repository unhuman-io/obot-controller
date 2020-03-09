#include "parameter_api.h"
//#include <regex>
//#include <sstream>

static std::string trim(std::string s)
{
    auto first = s.find_first_not_of(' ');
    auto last = s.find_last_not_of(' ');
    return s.substr(first, (last-first+1));
}

void ParameterAPI::add_api_variable(std::string name, APIVariable *var) {
    variable_map_[name] = var;
}

void ParameterAPI::set_api_variable(std::string name, std::string value) {
    if (variable_map_.count(name))  {
        variable_map_[name]->set(value);
    }
}

std::string ParameterAPI::get_api_variable(std::string name) {
    std::string out;
    if (variable_map_.count(name)) {
        out = variable_map_[name]->get();
    }
    return out;
}

std::string ParameterAPI::parse_string(std::string s) {
    std::string out;
    // std::regex set(R"(\w+)\s*=\s*([\d.]+)");
    // std::smatch sm;
    // if (std::regex_match(s, sm, set)) {
    //     set_api_variable(sm[0], sm[1]);
    // } else {
    //std::istringstream iss(s);
    if (s.c_str()[0] == '.') {
        s = last_string_;
    }
    auto equal_pos = s.find("=");
    if (equal_pos != std::string::npos) {
        auto variable = trim(s.substr(0,equal_pos));
        auto value = trim(s.substr(equal_pos+1));
        set_api_variable(variable, value);
        out = variable + " set " + value;
    } else {
        out = get_api_variable(s);
    }
    last_string_ = s;
    return out;
}

void APIFloat::set(std::string s) {
    *value_ = std::stof(s);
}

void APIUint32::set(std::string s) {
    *value_ = std::stoi(s);
}