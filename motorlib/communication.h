#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "messages.h"

class Communication {
 public:
    virtual void init() = 0;
    virtual int receive_data(ReceiveData * const data) = 0;
    virtual void send_data(const SendData &data) = 0;
};

#endif
