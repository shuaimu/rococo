/**
 * commo.hpp
 * Author: Lijing Wang
 * Date: 5/5/2015
 */

#pragma once

#include "view.hpp"
#include "threadpool.hpp" 
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
//#include "ThreadPool.h"

#include <zmq.hpp>
#include <unistd.h>
#include <google/protobuf/text_format.h>

using namespace boost::threadpool;
namespace mpaxos {
class Captain;
class Commo {
 public:
  Commo(std::vector<Captain *> &);
  Commo(Captain *captain, View &view, pool *pool);
  Commo(Captain *captain, View &view);
  ~Commo();
  void waiting_msg();
  void broadcast_msg(google::protobuf::Message *, MsgType);
  void send_one_msg(google::protobuf::Message *, MsgType, node_id_t);
//  void set_pool(ThreadPool *);
  void set_pool(pool *);
  void deal_msg(zmq::message_t &request);

 private:
  std::vector<Captain *> captains_;
  Captain * captain_;
  View *view_;
  pool *pool_;
  zmq::context_t context_;
  zmq::socket_t *receiver_;
//  zmq::socket_t sender_;
//  pool *self_pool_;
  std::vector<zmq::socket_t *> senders_;
  std::vector<std::string> senders_address_;
  std::vector<int> senders_state_;

  std::vector<boost::mutex> senders_mutexs_;
  boost::mutex receiver_mutex_;
};
} // namespace mpaxos
