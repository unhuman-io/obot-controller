#ifndef PARAMETER_API_H
#define PARAMETER_API_H

#include <string>
#include <map>


class APIVariable {
 public:
   virtual std::string get() const = 0;
   virtual void set(std::string) = 0;
};

template<class T>
class APIVariable2 : public APIVariable {
 public:
   APIVariable2(T *value) : value_(value) {};
   virtual std::string get() const { return std::to_string(*value_); }
   virtual void set(std::string) = 0;
 protected:
   T *value_;
};

class APIFloat : public APIVariable2<float> {
 public:
   APIFloat(float *f) : APIVariable2(f) {}
   void set(std::string);
};

class APIUint32 : public APIVariable2<uint32_t> {
 public:
   APIUint32(uint32_t *u) : APIVariable2(u) {}
   void set(std::string);
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
    std::string last_string_;
};

#endif
