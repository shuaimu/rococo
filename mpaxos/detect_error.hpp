/**
 * detect_error.hpp
 * Author: Lijing Wang
 * Date: 5/26/2015
 */

#pragma once

#include "captain.hpp"
namespace mpaxos {

class Detection {
 public:
  Detection(std::vector<Captain *> &captains, uint64_t total_times) 
    : captains_(&captains), total_times_(total_times), node_id_(0), longest_time_(0), longest_node_(0) {
    node_nums_ = captains.size();
    for (int i = 0; i < node_nums_; i++) {
      std::vector<PropValue *> tmp;
      results_.push_back(tmp);
//      results_.push_back(captains[i]->get_chosen_values());
      for (int j = 0; j < captains[i]->get_chosen_values().size(); j++) {
        if (captains[i]->get_chosen_values()[j] && captains[i]->get_chosen_values().at(j)->id() == 0) {
          continue;
        }
        results_[i].push_back(captains[i]->get_chosen_values().at(j));
      }

      times_.push_back(results_[i].size() - 1);
      if (longest_time_ < times_[i]) {
        longest_time_ = times_[i];
        longest_node_ = i; 
      }

    } 
  }

  Detection(std::vector<std::vector<PropValue *> > &results, uint64_t total_times) 
    : results_(results), total_times_(total_times), node_id_(0), longest_time_(0), longest_node_(0) {
    node_nums_ = results_.size();
    for (int i = 0; i < node_nums_; i++) {
      times_.push_back(results_[i].size() - 1);
      if (longest_time_ < times_[i]) {
        longest_time_ = times_[i];
        longest_node_ = i; 
      }
    } 
  }
//  ~Detection();

  /**
   * Only for no node down situation
   */ 
  bool detect_size() {
    bool ret = true;
    for (int i = 0; i < node_nums_; i++) {
      if (times_[i] != total_times_) {
        LOG_INFO("Result ERROR Size not EQUAL! (NodeID): %d (chosen_values.size): %llu (total_times): %llu", i, times_[i], total_times_);
        node_id_ = i;
        ret = false;
      }
    } 
    return ret;
  }
  
  /**
   * For both
   */
  bool detect_null_both() {
    bool ret = true;
    for (int j = 0; j < node_nums_; j++) {
      for (int i = 1; i <= times_[j]; i++) {
        if (results_[j][i] == NULL) {
          LOG_INFO("Found NULL! NodeID_%d slot_id: %d", j, i);
          node_id_ = j;
          ret = false;
        }
      }
    }
    return ret;
  }

  /**
   * TO test if the same slot has EQUAL value
   * only for No Node down
   */
  bool detect_equal() {
    bool ret = true;
    for (int i = 1; i <= total_times_; i++) {
      for (int j = 1; j < node_nums_; j++) {
        if (results_[j][i]->id() != results_[0][i]->id()) {
          LOG_INFO("Value Not EQUAL! slot_id: %d", i);
          LOG_INFO("NodeID_%d %s", j, results_[j][i]->data().c_str());
          node_id_ = j;
          ret = false;
        }
      }
    }
    return ret;
  }

  /**
   * TO test if the same slot has EQUAL value
   */
  bool detect_equal_down() {
    bool ret = true;
    int k = longest_node_;
    for (int i = 1; i <= longest_time_; i++) {
      for (int j = 0; j < node_nums_; j++) {
        if (i > times_[j]) continue;
        if (results_[j][i]->id() != results_[k][i]->id()) {
          LOG_INFO("Value Not EQUAL! slot_id: %d", i);
          LOG_INFO("NodeID_%d %s", j, results_[j][i]->data().c_str());
          LOG_INFO("NodeID_%d %s", k, results_[k][i]->data().c_str());
          node_id_ = j;
          ret = false;
        }
      }
    }
    return ret;
  }

  /**
   * TO test if the different slot has DIFFERENT value
   * only for no node down
   */
  bool detect_unique() {
    bool ret = true;
    for (int i = 1; i < total_times_; i++) {
      for (int j = i + 1; j <= total_times_; j++) {
        if (results_[0][i]->id() == results_[0][j]->id()) {
          LOG_INFO("Value Not UNIQUE! slot_id: %d & slot_id: %d", i, j);
          LOG_INFO("Value: %s", results_[0][j]->data().c_str());
          ret = false;
        }
      }
    }
   return ret; 
  }

  /**
   * TO test if the different slot has DIFFERENT value
   * only for down
   */
  bool detect_unique_down() {
    bool ret = true;
    int k = longest_node_;
    for (int i = 1; i < times_[k]; i++) {
      for (int j = i + 1; j <= times_[k]; j++) {
        if (results_[k][i]->id() == results_[k][j]->id()) {
          node_id_ = k;
          LOG_INFO("Value Not UNIQUE! slot_id: %d & slot_id: %d", i, j);
          LOG_INFO("Value: %s", results_[k][j]->data().c_str());
          ret = false;
        }
      }
    }
    return ret; 
  }

  bool detect_all() {
    bool ret = true;
    if (detect_size()) {

      LOG_INFO("%sSIZE TEST PASS%s", BLD_GRN, NRM);

      if (detect_null_both()) {

        LOG_INFO("%sNO_NULL TEST PASS%s", BLD_GRN, NRM);

        if (detect_equal()) {

          LOG_INFO("%sEQUAL TEST PASS%s", BLD_GRN, NRM);

          if (detect_unique()) {

            LOG_INFO("%sUNIQUE TEST PASS%s", BLD_GRN, NRM);
            LOG_INFO("%sALL PASS ^_^%s", BLD_GRN, NRM);

          } else {

            LOG_INFO("%sERROR! NONUNIQUE!%s", BLD_RED, NRM);
            ret = false;
          }
        } else {

          LOG_INFO("%sERROR! UNEQUAL!%s", BLD_RED, NRM);
          ret = false;
        }
      } else {

        LOG_INFO("%sERROR! NULL! %s", BLD_RED, NRM);
        ret = false;

      }
    } else {

      LOG_INFO("%sERROR! SIZE!%s", BLD_RED, NRM);
      if (detect_null_both()) {
//        detect_equal_down();
        detect_unique_down();
      } else {
        LOG_INFO("%sERROR! NULL!%s", BLD_RED, NRM);
      }
      ret = false;

    }
    return ret;
  }

  bool detect_down() {
    bool ret = true;
    if (longest_time_ == 0) {
      LOG_INFO("%sALL EMPTY NO TEST Requested%s", BLD_GRN, NRM);
      return true;
    }

    if (detect_null_both()) {

      LOG_INFO("%sNO_NULL TEST PASS%s", BLD_GRN, NRM);

      if (detect_equal_down()) {
         LOG_INFO("%sEQUAL TEST PASS%s", BLD_GRN, NRM);

         if (detect_unique_down()) {
            LOG_INFO("%sUNIQUE TEST PASS%s", BLD_GRN, NRM);
            LOG_INFO("%sALL PASS ^_^%s", BLD_GRN, NRM);
         } else {
            LOG_INFO("%sERROR! NONUNIQUE!%s", BLD_RED, NRM);
            ret = false;
         }

      } else {

        LOG_INFO("%sERROR! UNEQUAL!%s", BLD_RED, NRM);
        ret = false;

      }
    } else {

      LOG_INFO("%sERROR! NULL! %s", BLD_RED, NRM);
      ret = false;

    }

    return ret;
  }

  void print_all() {
    for (int i = 0; i < node_nums_; i++) 
      captains_->at(i)->print_chosen_values();
  }

  void print_one() {
    captains_->at(node_id_)->print_chosen_values();
  }
 
 private:
  std::vector<Captain *> *captains_;
  std::vector<std::vector<PropValue *> > results_;
  int node_nums_;
  uint64_t total_times_;
  int node_id_;
  std::vector<uint64_t> times_;
  int longest_time_;
  int longest_node_;
};

} // namespace
