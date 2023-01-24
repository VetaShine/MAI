#ifndef ZMQ_STD_HPP
#define ZMQ_STD_HPP

#include <assert.h>
#include <errno.h>
#include <string.h>
#include <string>
#include "zmq.h"

const char* NODE_EXECUTABLE_NAME = "calculation";
const char SENTINEL = '$';
const int PORT_BASE = 8000;
const int WAIT_TIME = 1000;

enum actions {
  fail = 0,
  success = 1,
  create = 2,
  destroy = 3,
  bind = 4,
  ping = 5,
  exec = 6,
  info = 7,
  back = 8
};

struct NodeToken {
  actions action;
  long long ParentId, id;
};

namespace ZmqStandard {
  void InitPairSocket(void* &context, void* &socket) {
    int parameter;
    context = zmq_ctx_new();
    socket = zmq_socket(context, ZMQ_PAIR);
    parameter = zmq_setsockopt(socket, ZMQ_RCVTIMEO, &WAIT_TIME, sizeof(int));
    assert(parameter == 0);
    parameter = zmq_setsockopt(socket, ZMQ_SNDTIMEO, &WAIT_TIME, sizeof(int));
    assert(parameter == 0);
  }

  template <class Class>
  void RecieveMessage(Class &ReplyData, void* socket) {
    int parameter = 0;
    zmq_msg_t reply;
    zmq_msg_init(&reply);
    parameter = zmq_msg_recv(&reply, socket, 0);
    assert(parameter == sizeof(Class));
    ReplyData = *(Class*)zmq_msg_data(&reply);
    parameter = zmq_msg_close(&reply);
    assert(parameter == 0);
  }

  template <class Class>
  void SendMessage(Class* token, void* socket) {
    int parameter = 0;
    zmq_msg_t message;
    zmq_msg_init(&message);
    parameter = zmq_msg_init_size(&message, sizeof(Class));
    assert(parameter == 0);
    parameter = zmq_msg_init_data(&message, token, sizeof(Class), NULL, NULL);
    assert(parameter == 0);
    parameter = zmq_msg_send(&message, socket, 0);
    assert(parameter == sizeof(Class));
  }

  template <class Class>
  bool send_msg_dontwait(Class* token, void* socket) {
    int parameter;
    zmq_msg_t message;
    zmq_msg_init(&message);
    parameter = zmq_msg_init_size(&message, sizeof(Class));
    assert(parameter == 0);
    parameter = zmq_msg_init_data(&message, token, sizeof(Class), NULL, NULL);
    assert(parameter == 0);
    parameter = zmq_msg_send(&message, socket, ZMQ_DONTWAIT);

    if (parameter == -1) {
      zmq_msg_close(&message);
      return false;
    }
    
    assert(parameter == sizeof(Class));
    return true;
  }

  template <class Class>
  bool RecieveMessageWait(Class &ReplyData, void* socket) {
    int parameter = 0;
    zmq_msg_t reply;
    zmq_msg_init(&reply);
    parameter = zmq_msg_recv(&reply, socket, 0);
    
    if (parameter == -1) {
      zmq_msg_close(&reply);
      return false;
    }

    assert(parameter == sizeof(Class));
    ReplyData = *(Class*)zmq_msg_data(&reply);
    parameter = zmq_msg_close(&reply);
    assert(parameter == 0);
    return true;
  }

  template <class Class>
  bool SendMessageWait(Class* token, void* socket) {
    int parameter;
    zmq_msg_t message;
    zmq_msg_init(&message);
    parameter = zmq_msg_init_size(&message, sizeof(Class));
    assert(parameter == 0);
    parameter = zmq_msg_init_data(&message, token, sizeof(Class), NULL, NULL);
    assert(parameter == 0);
    parameter = zmq_msg_send(&message, socket, 0);

    if (parameter == -1) {
      zmq_msg_close(&message);
      return false;
    }

    assert(parameter == sizeof(Class));
    return true;
  }

  template <class Class>
  bool SendRecieveWait(Class* TokenSend, Class & TokenReply, void* socket) {
    if (SendMessageWait(TokenSend, socket)) {
      if (RecieveMessageWait(TokenReply, socket)) {
        return true;
      } 
      else {
        return false;
      }
    } 
    else {
      return false;
    }
  }
}

#endif
