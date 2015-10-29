/**
 * proposer.cpp
 * Created on April 27, 2015
 * Author: Lijing Wang
 */

#include "proposer.hpp"
#include <iostream>
#include <math.h>
namespace mpaxos {

Proposer::Proposer(View &view, PropValue &value) 
//                   std::function<void>(PropValue &value) &callback) 
  : view_(&view), init_value_(&value),// callback_(callback),
    curr_ballot_(0), curr_value_(NULL),
    max_ballot_(0), max_value_(NULL),
    qr_(ceil((view_->nodes_size() + 1) / 2.0)), qw_(qr_) {
  #if MODE_TYPE == 1
  qr_ = view_->rs_qr();
  qw_ = view_->rs_qw();
  rs_encoder_ = new RSEncoder(view_->rs_x(), view_->rs_n());
  rs_decoder_ = new RSDecoder(view_->rs_x(), view_->rs_n());
  #else
  #endif 
  LOG_TRACE_PRO("Init RS for Proposer! qr_:%u qw_:%u", qr_, qw_);
}

Proposer::Proposer() : max_ballot_(0), max_value_(NULL) {
}

Proposer::~Proposer() {
}

// start proposing, send prepare requests to at least a majority of acceptors. 
// Now just send to all acceptors according to ack_map;
MsgPrepare *Proposer::msg_prepare() {
  MsgPrepare *msg_pre = new MsgPrepare();
  MsgHeader *msg_header = new MsgHeader();
  msg_header->set_msg_type(MsgType::PREPARE);
  msg_header->set_node_id(view_->whoami());
//  msg_header->set_slot_id(0);
  // prepare the msg_prepare
  msg_pre->set_allocated_msg_header(msg_header);
  // gen_next_ballot will increase the curr_ballot_
  msg_pre->set_ballot_id(gen_next_ballot());

  LOG_TRACE_PRO("--> Phase I : <msg_prepare> (ballot_id):%llu", curr_ballot_);
  return msg_pre;
}

// choose a higher ballot id, retry the prepare phase.
// clean the map for the ack of prepare and accept
MsgPrepare *Proposer::restart_msg_prepare() {
  curr_value_ = NULL;
  // clear map
  msg_ack_prepare_.clear();
  msg_ack_accept_.clear();
  return msg_prepare();
}

// start accepting, send accept requests to acceptors who responsed.
MsgAccept *Proposer::msg_accept() {
  MsgAccept *msg_acc = new MsgAccept();
  MsgHeader *msg_header = new MsgHeader();
  msg_header->set_msg_type(MsgType::ACCEPT);
  msg_header->set_node_id(view_->whoami());
//  msg_header->set_slot_id(0);
  msg_acc->set_allocated_msg_header(msg_header);
  msg_acc->set_ballot_id(curr_ballot_);
  msg_acc->set_allocated_prop_value(curr_value_);
  return msg_acc;      
}

// start accepting, send accept requests to acceptors who responsed. only for RS
std::vector<MsgAccept *>Proposer::rs_msg_accept() {

  if (rs_values_.empty())
    rs_values_ = rs_encoder_->rs_encode(*curr_value_->mutable_data());

  std::vector<MsgAccept *> msg_accs;
  for (uint32_t i = 0; i < view_->nodes_size(); i++) {
    MsgAccept *msg_acc = new MsgAccept();
    MsgHeader *msg_header = new MsgHeader();
    msg_header->set_msg_type(MsgType::ACCEPT);
    msg_header->set_node_id(view_->whoami());
    msg_acc->set_allocated_msg_header(msg_header);
    msg_acc->set_ballot_id(curr_ballot_);

    LOG_TRACE_PRO("HERE!!! i:%lu", i);
    PropValue *rs_value = new PropValue();
    rs_value->set_id(curr_value_->id());
    LOG_TRACE_PRO("THERE!!!? i:%lu", i);
    rs_value->set_data(rs_values_[i]);
    LOG_TRACE_PRO("THERE!!!");
    msg_acc->set_allocated_prop_value(rs_value);
    msg_accs.push_back(msg_acc);
  }
  return msg_accs;      
}
// Temp return curr_value_ to captain
PropValue *Proposer::get_chosen_value() {
  return curr_value_;
}

/**
 * handle acks to the prepare requests.
 * if a majority of yes, decide use which value to do the accept requests.
 *   with a majority of empty, use the initial value I have. 
 *   with some already accepted value, choose one with the highest ballot
 * if a majority of no, 
 * restart with a higher ballot 
 */ 
AckType Proposer::handle_msg_promise(MsgAckPrepare *msg_ack_pre) {
//  std::lock_guard<std::mutex> lock(prepare_mutex_);
  //DROP Out of Date & Already enter Phase II
//  std::cout << "\tProposer handle_msg_promise start" << std::endl;
  if (msg_ack_pre->ballot_id() < curr_ballot_ || curr_value_) {
    LOG_TRACE_PRO("<handle_msg_promise> ---- DROP! --NodeID %u", msg_ack_pre->msg_header().node_id());
//    std::cout << "\tProposer promise ---- DROP! --NodeID" << msg_ack_pre->msg_header().node_id() << std::endl;
    return DROP; 
  }
  node_id_t node_id = (uint16_t)msg_ack_pre->msg_header().node_id();
//  std::cout << "Proposer handle_msg_promise from node_id: " << node_id << std::endl;
  msg_ack_prepare_[node_id] = msg_ack_pre;
  // NOT_ENOUGH
  if (msg_ack_prepare_.size() < qr_) {
    LOG_TRACE_PRO("<handle_msg_promise> ---- NOT_ENOUGH! --NodeID %u", msg_ack_pre->msg_header().node_id());
//    std::cout << "\tProposer promise ---- NOT_ENOUGH!" << std::endl;
    return NOT_ENOUGH; 
  }
  uint32_t true_counter = 0;
  std::map<node_id_t, MsgAckPrepare *>::iterator it;
  
  for (it = msg_ack_prepare_.begin(); it != msg_ack_prepare_.end(); it++) {
    if (it->second->reply()) {
      if (it->second->max_ballot_id() > max_ballot_) {
        max_ballot_ = it->second->max_ballot_id(); 
        max_value_ = it->second->mutable_max_prop_value();
      }
      true_counter++;
    }
  }

  if (true_counter >= qr_) {
    // CONTINUE
    if (max_value_) {

#if MODE_TYPE == 1
      // only for RS
      value_id_t max_value_id = max_value_->id();

      if (!curr_value_ || curr_value_->id() != max_value_id) {

        std::vector<std::string> encoded_data(view_->rs_n(), "");
        std::vector<bool> index(view_->rs_n(), false);
        unsigned lost_num = view_->rs_n();
        for (it = msg_ack_prepare_.begin(); it != msg_ack_prepare_.end(); it++) {
          if (it->second->reply() && (max_value_id == it->second->mutable_max_prop_value()->id())) {
            encoded_data[it->first] = it->second->mutable_max_prop_value()->data();
            index[it->first] = true;
            lost_num--;
          }
        }
        if (lost_num > view_->rs_f()) 
          return RESTART; 

        std::string decoded_data = rs_decoder_->rs_decode(encoded_data, index, lost_num);

        std::cout << "decoded_data: " << decoded_data << std::endl;

        curr_value_ = new PropValue();
        curr_value_->set_id(max_value_id);
        curr_value_->set_data(decoded_data);
        max_value_ = curr_value_;

        rs_values_ = rs_encoder_->rs_encode(decoded_data);
      }

#else
      curr_value_ = max_value_;      
#endif

    } else 
      curr_value_ = init_value_;

    LOG_TRACE_PRO("<handle_msg_promise> ---- CONTINUE into Phase II! --NodeID %u", msg_ack_pre->msg_header().node_id());
    return CONTINUE;

  } else {// RESTART 

    LOG_TRACE_PRO("<handle_msg_promise> ---- RESTART! --NodeID %u", msg_ack_pre->msg_header().node_id());
    return RESTART;
  }
}

/**
 * handle acks to the accept reqeusts;
 * if a majority of yes, the value is successfully chosen. 
 * if a majority of no, restart the first phase with a higher ballot.
 */
AckType Proposer::handle_msg_accepted(MsgAckAccept *msg_ack_acc) {
//  std::lock_guard<std::mutex> lock(accept_mutex_);
  //Drop Out of Date or Out of Order ACK
  if (msg_ack_acc->ballot_id() < curr_ballot_ || !curr_value_) {
//    std::cout << "\tProposer handle_msg_accepted --- DROP! from --NodeID" << msg_ack_acc->msg_header().node_id() << std::endl;
    LOG_TRACE_PRO("<handle_msg_accepted> ---- DROP!  --NodeID %u", msg_ack_acc->msg_header().node_id());
    return DROP;
  }
  node_id_t node_id = (uint16_t)msg_ack_acc->msg_header().node_id();
//  std::cout << "Inside handle_msg_accepted node_id: " << node_id << std::endl;
  msg_ack_accept_[node_id] = msg_ack_acc;

  // NOT_ENOUGH
  if (msg_ack_accept_.size() < qw_) {
//    std::cout << "\tProposer handle_msg_accepted --- NOT_ENOUGH! --NodeID" << msg_ack_acc->msg_header().node_id() << std::endl;
    LOG_TRACE_PRO("<handle_msg_accepted> ---- NOT_ENOUGH! --NodeID %u", msg_ack_acc->msg_header().node_id());
    return NOT_ENOUGH; 
  }

  uint32_t true_counter = 0;
  std::map<node_id_t, MsgAckAccept *>::iterator it;
  for (it = msg_ack_accept_.begin(); it != msg_ack_accept_.end(); it++) {
    if (it->second->reply()) {
      true_counter++;
    }
  }

  if (true_counter >= qw_) {
//    std::cout << "\tProposer handle_msg_accepted --- CHOOSE! from NodeID" << msg_ack_acc->msg_header().node_id() << std::endl;
    LOG_TRACE_PRO("<handl_msg_accepted> ---- CHOOSE! --NodeID %u", msg_ack_acc->msg_header().node_id());
    // ACCEPT 
    return CHOOSE;
  } else // RESTART 
//    std::cout << "\tProposer handle_msg_accepted --- RESTART! from NodeID" << msg_ack_acc->msg_header().node_id() << std::endl;
    LOG_TRACE_PRO("<handle_msg_accepted> ---- RESTART! --NodeID %u", msg_ack_acc->msg_header().node_id());
    // ACCEPT 
    return RESTART;    
}   

/**
 * ballot_id_t should be a 64-bit uint, high 48bit is self incremental counter,
 * low 16bit is the node id.
 */ 
ballot_id_t Proposer::gen_next_ballot() {
//  std::cout << "\t\tBefore gen_next_ballot curr_ballot_ " << curr_ballot_ << " NodeID " << view_->whoami() << std::endl;
  LOG_TRACE_PRO("Before <gen_next_ballot> (curr_ballot_):%llu --NodeID %u", curr_ballot_, view_->whoami());

  if (curr_ballot_ == 0)
    curr_ballot_ = (curr_ballot_ + (1 << 16)) + view_->whoami();
  else 
    curr_ballot_ = curr_ballot_ + (1 << 16);

//  std::cout << "\t\tAfter gen_next_ballot curr_ballot_ " << curr_ballot_ << std::endl;
  LOG_TRACE_PRO("After <gen_next_ballot> (curr_ballot_):%llu", curr_ballot_);
  return curr_ballot_;
}

/**
 * when one node is dead clean all except max_ballot_ & max_value
 */
void Proposer::die_clean() {
  curr_value_ = NULL;
  curr_ballot_ = 0;
  // clear map
  msg_ack_prepare_.clear();
  msg_ack_accept_.clear();
}

PropValue * Proposer::get_init_value() {
  return init_value_;
}

}
