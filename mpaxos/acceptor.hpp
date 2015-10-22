/*
 * acceptor.hpp
 *
 * Created on: April 30, 2015
 * Author: Lijing 
 */

#pragma once
#include "view.hpp"
#include <mutex>

namespace mpaxos {

class Acceptor {
 public:
  Acceptor(View &view);
  ~Acceptor();

  /** 
   * handle prepare requests from proposers:
   * if ballot_id > max_proposed_ballot_, 
   *   update max_proposed_ballot_,
   *   reply yes with max_value_;
   * else 
   *   reply no.
   * return MsgAckPrepare *
   */ 

  MsgAckPrepare *handle_msg_prepare(MsgPrepare *);
  /**
   * handle accept requests from proposers:
   * if ballot_id > max_proposed_ballot_, accept this value
   *   update max_proposed_ballot_, max_accepted_ballot_ and max_value_,
   *   reply yes;
   * else
   *   reply no.
   * return MsgAckAccept *
   */
  MsgAckAccept *handle_msg_accept(MsgAccept *);

  PropValue *get_max_value();

 private:
  View *view_;
  // the max proposed ballot id I have ever seen, initial 0. 
  ballot_id_t max_proposed_ballot_;
  // the max accepted ballot id I have accepted, initial 0.
  ballot_id_t max_accepted_ballot_;
  // the the max PropValue I have ever seen, initialy null.
  PropValue *max_value_; 
  // thread safe
  std::mutex mutex_;
};
}  // namespace mpaxos
