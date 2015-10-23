/*
 * proposer.hpp
 * Author: Lijing Wang
 */

#pragma once

#include "view.hpp"
#include <map>
#include <mutex>
#include "rs_encoder.hpp"
#include "rs_decoder.hpp"

namespace mpaxos {

class Proposer {
 public:
  Proposer();
  Proposer(View &view, PropValue &value); 
  ~Proposer();


  
  // start proposing, send prepare requests to at least a majority of acceptors.
  MsgPrepare *msg_prepare();

  // choose a higher ballot id, retry the prepare phase.
  // clean the map for the ack of prepare and accept
  MsgPrepare *restart_msg_prepare();

  // start accepting, send accept requests to acceptors who responsed.
  MsgAccept *msg_accept();

  // start accepting, send accept requests to acceptors who responsed. especially for RS
  std::vector<MsgAccept *>rs_msg_accept();

  // Temp return curr_value_ to captain
  PropValue *get_chosen_value();

  /**
   * handle acks to the prepare requests:
   * if a majority of yes, decide use which value to do the accept requests.
   *   with a majority of empty, use the initial value I have. 
   *   with some already accepted value, choose one with the highest ballot
   * if a majority of no, 
   *   restart with a higher ballot.
   * return the status 
   */ 
  AckType handle_msg_promise(MsgAckPrepare *);

  /**
   * handle acks to the accept reqeusts:
   * if a majority of yes, the value is successfully chosen. 
   * if a majority of no, restart the first phase with a higher ballot.
   * return the status 
   */
  AckType handle_msg_accepted(MsgAckAccept *); 

  /**
   * ballot_id_t should be a 64-bit uint, high 48bit is self incremental counter,
   * low 16bit is the node id.
   */ 
  ballot_id_t gen_next_ballot() ;

  /**
   * when one node is dead clean all except max_ballot_ & max_value
   */
  void die_clean();

  /**
   * Return the current Phase 
   * if curr_value_ == NULL return Phase I
   * if curr_value_ != NULL && chosen_ == false return Phase II
   * if curr_value_ != NULL && chosen_ == true return CHOSEN
   * else return ERROR
   */
  ProposerStatus get_curr_status();

  /**
   * get init value
   */
  PropValue *get_init_value();

 private:
  View *view_; 

  // the value that client wants me to commit.
  PropValue *init_value_; 

  // the ballot I am currently using.
  ballot_id_t curr_ballot_; 
  
  // the value I am currently proposing.
  PropValue *curr_value_;  
  
  // the max ballot I have ever seen, initialy 0.
  ballot_id_t max_ballot_;  

  // the max PropValue I have ever seen, initialy null.
  PropValue *max_value_;

  // the callback after I finish this instance.  
//  std::function<void>(PropValue &value) callback_;

  // the acks from all nodes to the current prepare.  
  // remember to clean for every next ballot!!!
  std::map<node_id_t, MsgAckPrepare *> msg_ack_prepare_;

  // the ack from all nodes to the cuurent accept. 
  // remember to clean for every next ballot!!!
  std::map<node_id_t, MsgAckAccept *> msg_ack_accept_;

  // lock to maintain thread_safe, now one proposer share one lock ADDED by Lijing
  std::mutex prepare_mutex_;
  std::mutex accept_mutex_;

  // added because of RS 
  node_id_t qr_;
  node_id_t qw_;  
  RSEncoder *rs_encoder_;
  RSDecoder *rs_decoder_;
  std::vector<std::string> rs_values_;
};
}  // namespace mpaxos
