/**
 * commo.cpp
 * Author: Lijing Wang
 * Date: 5/5/2015
 */

#include "commo.hpp"
#include "captain.hpp"
#include <iostream>

namespace mpaxos {
// Old Version
Commo::Commo(std::vector<Captain *> &captains) 
  : captains_(captains), context_(1) {
//  for (uint32_t i = 0; i < captains_.size(); i++) {
////    senders_.push_back(new zmq::socket_t(context_, ZMQ_DEALER));
//  }
//  pool_ = new pool(4);
}

Commo::Commo(Captain *captain, View &view, pool *pl) 
  : captain_(captain), view_(&view), pool_(pl), context_(1), 
    senders_mutexs_(view_->nodes_size()) {
}

Commo::Commo(Captain *captain, View &view) 
  : captain_(captain), view_(&view), context_(1), 
    senders_mutexs_(view_->nodes_size()) {
  

//  int sndhwm = 0;
  int immediate = 1;
  int handshake = 0;

  LOG_INFO_COM("%s Init START", view_->hostname().c_str());


  for (uint32_t i = 0; i < view_->nodes_size(); i++) {
    senders_.push_back(new zmq::socket_t(context_, ZMQ_DEALER));
    senders_address_.push_back("");
    senders_state_.push_back(-1);

//    int port = 44440 + i;
    std::string identity = view_->hostname();
    senders_[i]->setsockopt(ZMQ_IDENTITY, identity.c_str(), identity.size());
//    senders_[i]->setsockopt(ZMQ_SNDHWM, &sndhwm, sizeof(sndhwm));
//    senders_[i]->setsockopt(ZMQ_IMMEDIATE, &immediate, sizeof(immediate));
//    senders_[i]->setsockopt(ZMQ_HANDSHAKE_IVL, &handshake, sizeof(handshake));
//    senders_[i]->setsockopt(ZMQ_CONFLATE, &immediate, sizeof(immediate));

    std::string address = "tcp://" + view_->address(i) + ":" + std::to_string(view_->port(i));
    LOG_INFO_COM("Connect to address %s, host_name %s", address.c_str(), view_->hostname(i).c_str());
    senders_address_[i] = address;
    senders_[i]->connect(address.c_str());
//    senders_state_[i] = zmq_connect(senders_[i], address.c_str());
//    if (senders_state_[i] < 0)
//      LOG_INFO_COM("Node_ID: %lu does NOT connect!", i);
  }
//  self_pool_ = new pool(1);
//  self_pool_->schedule(boost::bind(&Commo::waiting_msg, this));
  receiver_ = new zmq::socket_t(context_, ZMQ_ROUTER);
//  receiver_->setsockopt(ZMQ_SNDHWM, &sndhwm, sizeof(sndhwm));
//  receiver_->setsockopt(ZMQ_IMMEDIATE, &immediate, sizeof(immediate));
  std::string address = "tcp://*:" + std::to_string(view_->port());
  LOG_INFO_COM("My address %s, host_name %s", address.c_str(), view_->hostname().c_str());
  receiver_->bind(address.c_str());
  boost::thread listen(boost::bind(&Commo::waiting_msg, this)); 
}

Commo::~Commo() {
}

void Commo::waiting_msg() {

//  zmq::pollitem_t items [] = {
//      { receiver, 0, ZMQ_POLLIN, 0 },
//      { subscriber, 0, ZMQ_POLLIN, 0 }
//  };

  std::vector<zmq::pollitem_t> items(1 + view_->nodes_size());
  items[0].socket = (void *)(*receiver_);
  items[0].events = ZMQ_POLLIN;

  for (int i = 0; i < view_->nodes_size(); i++) {
    items[i + 1].socket = (void *)(*senders_[i]);
    items[i + 1].events = ZMQ_POLLIN; 
  }

  while (true) {

    zmq::poll(&items[0], items.size(), -1);

    for (int i = 0; i <= view_->nodes_size(); i ++) {

      if (items[i].revents & ZMQ_POLLIN) {
          
        zmq::message_t identity;
        zmq::message_t request;
        //  Wait for next request from client
        if (i == 0) {
//          receiver_mutex_.lock();
          while (receiver_->recv(&identity, ZMQ_DONTWAIT) > 0) {

            receiver_->recv(&request);
//            receiver_mutex_.unlock();

            int size_id = identity.size();
            std::string data_id(static_cast<char*>(identity.data()), size_id);
            LOG_DEBUG_COM("receiver received from %s", data_id.c_str());
            deal_msg(request);
          }
        }
        else {
//          senders_mutexs_[i - 1].lock();
          while (senders_[i - 1]->recv(&request, ZMQ_DONTWAIT) > 0) {
//          senders_mutexs_[i - 1].unlock();
            LOG_DEBUG_COM("senders_[%d] received!", i - 1);
            deal_msg(request);
          }
        } 

      }
    }
  }
}

void Commo::deal_msg(zmq::message_t &request) {
  std::string msg_str(static_cast<char*>(request.data()), request.size());
  int type = int(msg_str.back() - '0');
  google::protobuf::Message *msg = nullptr;
//    LOG_DEBUG_COM("type %d", type);
  switch(type) {
    case PREPARE: {
      msg = new MsgPrepare();
      break;
    }
    case PROMISE: {
      msg = new MsgAckPrepare();
      break;
    }
    case ACCEPT: {
      msg = new MsgAccept();
      break;
    }
    case ACCEPTED: {
      msg = new MsgAckAccept();
      break;
    }
    case DECIDE: {
      msg = new MsgDecide();
      break;
    }
    case LEARN: {
      msg = new MsgLearn();
      break;
    }                        
    case TEACH: {
      msg = new MsgTeach();
      break;
    }
    case COMMIT: {
      msg = new MsgCommit();
      break;
    }
    case COMMAND: {
      msg = new MsgCommand();
      break;
    }
  }
  msg_str.pop_back();
  msg->ParseFromString(msg_str);
  std::string text_str;
  google::protobuf::TextFormat::PrintToString(*msg, &text_str);
  LOG_DEBUG_COM("Received %s", text_str.c_str());
//    pool_->schedule(boost::bind(&Captain::handle_msg, captain_, msg, static_cast<MsgType>(type)));
  captain_->handle_msg(msg, static_cast<MsgType>(type));
  LOG_DEBUG("Handle finish!");
}

//void Commo::set_pool(ThreadPool *pool) {
void Commo::set_pool(pool *pl) {
  pool_ = pl;
}

void Commo::broadcast_msg(google::protobuf::Message *msg, MsgType msg_type) {

  for (uint32_t i = 0; i < view_->nodes_size(); i++) {
    
    if (i == view_->whoami()) {
      continue;
    }

    std::string msg_str;
    msg->SerializeToString(&msg_str);
    msg_str.append(std::to_string(msg_type));
    // create a zmq message from the serialized string
    zmq::message_t request(msg_str.size());
    memcpy((void *)request.data(), msg_str.c_str(), msg_str.size());
    LOG_DEBUG_COM("Broadcast to --%s (msg_type):%d", view_->hostname(i).c_str(), msg_type);
    senders_mutexs_[i].lock(); 
    senders_[i]->send(request, ZMQ_DONTWAIT);
    senders_mutexs_[i].unlock(); 
    LOG_DEBUG_COM("Broadcast to --%s (msg_type):%d finished", view_->hostname(i).c_str(), msg_type);
  }

}

void Commo::send_one_msg(google::protobuf::Message *msg, MsgType msg_type, node_id_t node_id) {
//  std::cout << " --- Commo Send ONE to captain " << node_id << " MsgType: " << msg_type << std::endl;
  LOG_DEBUG_COM("Send ONE to --%s (msg_type):%d", view_->hostname(node_id).c_str(), msg_type);

  std::string msg_str;
  msg->SerializeToString(&msg_str);
  msg_str.append(std::to_string(msg_type));
  // create a zmq message from the serialized string
  zmq::message_t request(msg_str.size());
  memcpy((void *)request.data(), msg_str.c_str(), msg_str.size());

  if (msg_type == ACCEPT || ((msg_type == COMMIT) && (node_id == view_->master_id()))) {
    LOG_DEBUG_COM("senders[%d] send request", node_id);
    senders_mutexs_[node_id].lock(); 
    senders_[node_id]->send(request, ZMQ_DONTWAIT);
    senders_mutexs_[node_id].unlock(); 
    LOG_DEBUG_COM("senders[%d] send finish", node_id);
  }
  else { 
    std::string data_id = view_->hostname(node_id);
    zmq::message_t identity(data_id.size());
    memcpy((void *)identity.data(), data_id.c_str(), data_id.size());
    LOG_DEBUG_COM("receiver_ reply request to %s", view_->hostname(node_id).c_str());
    receiver_mutex_.lock();
    receiver_->send(identity, ZMQ_SNDMORE);
    receiver_->send(request, ZMQ_DONTWAIT);
    receiver_mutex_.unlock();
    LOG_DEBUG_COM("receiver_ reply request to %s finish", view_->hostname(node_id).c_str());
  }
}
} // namespace mpaxos
