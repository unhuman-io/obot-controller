#ifndef PARAMETER_API_H
#define PARAMETER_API_H

#include <string>
#include <map>

class APIVariable {
 public:
    virtual std::string get() const = 0;
    virtual void set(std::string) = 0;
};

class APIFloat : public APIVariable {
 public:
    APIFloat(float *f) : f_(f) {};
    std::string get() const;
    void set(std::string);
 private:
    float *f_;
};

// allows for setting variables through text commands
class ParameterAPI {
 public:
    // type is used by scanf to parse the string
    void add_api_variable(std::string name, APIVariable *variable);
    void set_api_variable(std::string name, std::string value);
    std::string get_api_variable(std::string name);
    std::string parse_string(std::string);
 private:
    std::map<std::string, APIVariable *> variable_map_;
};

#endif
