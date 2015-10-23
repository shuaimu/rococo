/**
 * detect_file.hpp
 * Author: Lijing Wang
 * Date: 7/02/2015
 */

#pragma once

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <vector>
#include <iostream>
#include "internal_types.hpp"

namespace mpaxos {

using namespace boost::filesystem;
using boost::filesystem::ofstream;
using boost::filesystem::ifstream;

class DetectionFile {
 public:

  DetectionFile(int total_times) 
    : node_nums_(0), total_times_(total_times), node_id_(0), longest_time_(0), longest_node_(0) {
    
    std::string dir_path = "tmp";
    if (!exists(dir_path)) {
      std::cout << "dir_path does NOT exist!!!";
    }

    directory_iterator end_itr; 
    for (directory_iterator itr(dir_path); itr != end_itr; ++itr) {
      std::cout << itr->path() << std::endl;
      std::vector<std::string> result;
      ifstream input(itr->path());
      int len = 0;
      for(std::string line; getline(input, line);) {
        len++;
        result.push_back(line);
      }
      names_.push_back(itr->path().string());
      times_.push_back(len);
      results_.push_back(result);
      if (longest_time_ < times_[node_nums_]) {
        longest_time_ = times_[node_nums_];
        longest_node_ = node_nums_; 
      }
      node_nums_++;
    }
    std::cout << "longest_node_ " << longest_node_ << std::endl;
    std::cout << "longgest_time_ " << longest_time_ << std::endl;

  }

  /**
   * Only for no node down situation
   */ 
  bool detect_size() {
    bool ret = true;
    for (int i = 0; i < node_nums_; i++) {
      if (times_[i] != total_times_) {
        LOG_INFO("Result ERROR Size not EQUAL! (NodeID): %d (chosen_values.size): %d (total_times): %d", i, times_[i], total_times_);
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
    for (int i = 0; i < node_nums_; i++) {
      for (int j = 0; j < times_[i]; j++) {
        if (results_[i][j].empty()) {
          LOG_INFO("Found EMPTY String! NodeID_%d slot_id: %d", i, j);
          node_id_ = i;
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
    for (int i = 0; i < total_times_; i++) {
      for (int j = 1; j < node_nums_; j++) {
        if (results_[j][i].compare(results_[0][i]) != 0) {
          LOG_INFO("Value Not EQUAL! slot_id: %d", i);
          LOG_INFO("NodeID_%d %s", j, results_[j][i].c_str());
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
    for (int i = 0; i < longest_time_; i++) {
      for (int j = 0; j < node_nums_; j++) {
        if (i > times_[j]) continue;
        if (results_[j][i].compare(results_[k][i]) != 0) {
          LOG_INFO("Value Not EQUAL! slot_id: %d", i);
          LOG_INFO("Value Not EQUAL! slot_id: %d", i);
          LOG_INFO("NodeID_%d %s", j, results_[j][i].c_str());
          LOG_INFO("NodeID_%d %s", k, results_[k][i].c_str());
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
    for (int i = 0; i < total_times_ - 1; i++) {
      for (int j = i + 1; j < total_times_; j++) {
        if (results_[0][i].compare(results_[0][j]) == 0) {
          LOG_INFO("Value Not UNIQUE! slot_id: %d & slot_id: %d", i, j);
          LOG_INFO("Value: %s", results_[0][j].c_str());
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
    for (int i = 0; i < times_[k] - 1; i++) {
      for (int j = i + 1; j < times_[k]; j++) {
        if (results_[k][i].compare(results_[k][j]) == 0) {
          node_id_ = k;
          LOG_INFO("Value Not UNIQUE! slot_id: %d & slot_id: %d", i, j);
          LOG_INFO("Value: %s", results_[k][j].c_str());
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
    for (int i = 0; i < node_nums_; i++) {
      std::cout << names_[i] << std::endl;
      for (int j = 0; j < times_[i]; j++) {
        std::cout << j << ": " << results_[i][j] << std::endl; 
      }
    }
  }

  void print_one() {
    std::cout << names_[node_id_] << std::endl;
    for (int j = 0; j < times_[node_id_]; j++) {
      std::cout << j << ": " << results_[node_id_][j] << std::endl; 
    }
  }

 private:
  std::vector<std::vector<std::string> > results_;
  int node_nums_;
  int total_times_;
  int node_id_;
  std::vector<int> times_;
  std::vector<std::string> names_;
  int longest_time_;
  int longest_node_;
};

} // namespace
