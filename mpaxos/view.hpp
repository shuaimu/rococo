/*
 * view.hpp
 *
 * Created on: May 08, 2015
 * Author: Lijing 
 */

#pragma once
#include "internal_types.hpp"
#include "mpaxos.pb.h"
#include <set>
#include <vector>
#include <yaml-cpp/yaml.h>
#include <iostream>
namespace mpaxos {
class View {
 public:
//  View(set<node_id_t> &nodes);
  View(node_id_t node_id, std::string config_file);
  View(node_id_t node_id, std::set<node_id_t> &);
  View(node_id_t node_id);
  std::set<node_id_t> *get_nodes();
  std::vector<host_info_t> *get_host_nodes();
  node_id_t whoami();
  bool if_master();
  void set_master(node_id_t);

  std::string hostname();
  std::string address();
  uint32_t port();
  std::string hostname(node_id_t);
  std::string address(node_id_t);
  uint32_t port(node_id_t);
  node_id_t master_id();

  uint64_t nodes_size();
  uint32_t period();

  node_id_t rs_x();
  node_id_t rs_n();
  node_id_t rs_f();
  node_id_t rs_qr();
  node_id_t rs_qw();

  void print_host_nodes();

 private:
  node_id_t node_id_;
  // only node_id
  std::set<node_id_t> nodes_;
  // full host_info
  std::vector<host_info_t> host_nodes_;
  uint64_t size_;
  node_id_t master_id_;
  uint32_t period_;
  // related to Reed Solomon
  node_id_t rs_x_;
  node_id_t rs_n_;
  node_id_t rs_f_;
  node_id_t rs_qr_;
  node_id_t rs_qw_;
};
} // namespace mpaxos
