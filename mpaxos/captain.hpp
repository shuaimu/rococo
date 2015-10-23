/**
 * captain.hpp
 * Author: Lijing Wang
 * Date: 5/5/2015
 */

#pragma once
#include "proposer.hpp"
//#include "commo.hpp"
#include "acceptor.hpp"
#include <queue>
#include <unordered_map>
#include "threadpool.hpp" 
#include "ThreadPool.h"
#include <map>
//#include "include_all.h"
using namespace boost::threadpool;
namespace mpaxos {
// TODO [Loli] This module is responsible for locating the correct proposer and
// acceptor for a certain instance (an instance is identified by slot_id). 

struct proposer_info_t {

  int try_time;
  Proposer *curr_proposer;
  ProposerStatus proposer_status;

  proposer_info_t(int times) 
    : try_time(times), curr_proposer(NULL), proposer_status(EMPTY) {
  };
};


class Commo;
class Captain {
 public:

  Captain(View &);

  Captain(View &, pool *);

  // default everything constructor
  Captain(View &, pool *, callback_t&);

  Captain(View &, callback_t&);

  Captain(View &view, int window_size);

  ~Captain();


  /**
   * set_callback from outside
   */
  void set_callback(callback_t& cb);

  /**
   * set_callback from outside
   */
  void set_callback(callback_full_t& cb);

  void set_callback(callback_latency_t& cb);

  /**
   * warpper of commit_value and master lease 
   */
  void commit(std::string&);

  /**
   * warpper of commit_value and master lease 
   */
  void commit(PropValue *);

  /** 
   * client commits one value to captain
   */
  void commit_value(std::string&);

  /** 
  * client commits recover value to captain
  */
  void commit_recover();

  /**
   * captain starts a new paxos instance 
   */
//  void new_slot();
  /**
   * captain starts a new paxos instance with a slot_id
   */
  void new_slot(PropValue *, int try_time);

  /**
   * captain starts a new paxos instance with a slot_id
   */
  void new_slot(PropValue *, int try_time, slot_id_t old_slot);


  /** 
   * return node_id
   */
  node_id_t get_node_id(); 

  /**
   * set commo_handler 
   */
  void set_commo(Commo *); 

  /**
   * set set thread_pool handler 
   */
//  void set_thread_pool(ThreadPool *); 

  /**
   * handle message from commo, all kinds of message
   */
  void handle_msg(google::protobuf::Message *, MsgType);

  /**
   * Add a new chosen_value 
   */
  void add_chosen_value(slot_id_t, PropValue *);

  /**
   * Add a new learn_value 
   */
  void add_learn_value(slot_id_t, PropValue *, node_id_t);

  /**
   * Return Msg_header
   */
  MsgHeader *set_msg_header(MsgType, slot_id_t);

  /**
   * Return Decide Message
   */
  MsgDecide *msg_decide(slot_id_t);

  /**
   * Return Learn Message
   */
  MsgLearn *msg_learn(slot_id_t);

  /**
   * Return Teach Message
   */
  MsgTeach *msg_teach(slot_id_t);

  /**
   * Return Teach Message only for RS
   */
  MsgTeach *rs_msg_teach(slot_id_t, node_id_t);

  /**
   * Return Commit Message
   */
  MsgCommit *msg_commit(PropValue *);

  /**
   * Return Command Message
   */
  MsgCommand *msg_command();

  /** 
   * Callback function after commit_value  
   */
  void clean();

  void crash();

  void recover();

  bool get_status();

  void print_chosen_values();

  std::vector<PropValue *> get_chosen_values();

  bool if_recommit();

  void add_callback();

  void set_timer(int, PropValue *);

 private:

  View *view_;
  Commo *commo_;

  std::vector<Acceptor *> acceptors_;
  std::map<slot_id_t, proposer_info_t *> proposers_;
  std::vector<PropValue *> chosen_values_;
  std::queue<PropValue *> tocommit_values_;

  // max chosen instance/slot id 
  slot_id_t max_chosen_; 
  slot_id_t max_chosen_without_hole_; 
  slot_id_t max_slot_;

  // aim to supporting window
  value_id_t value_id_;
  slot_id_t window_size_;

  /** 
   * trigger this callback 
   * sequentially for each chosen value.
   */
  callback_t callback_;
  callback_full_t callback_full_;
  // when one value commited by this node is chosen, trigger this
  callback_latency_t callback_latency_;

  // pool no use now
  pool *pool_;

  // tag work 
  bool work_;

  boost::mutex max_chosen_mutex_;
  boost::mutex value_id_mutex_;

  boost::mutex tocommit_values_mutex_;
  boost::mutex acceptors_mutex_;
  boost::mutex proposers_mutex_;

  boost::mutex work_mutex_;

  std::map<value_id_t, boost::mutex> commit_mutexs_;
  std::map<value_id_t, boost::condition_variable> commit_conds_;
  std::map<value_id_t, bool> commit_readys_;

};

} //  namespace mpaxos
