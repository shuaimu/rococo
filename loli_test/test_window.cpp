/**
 * evaluation_window.cpp 
 * Author: Lijing Wang
 * Date: 8/2/2015
 */

#include "view.hpp"
#include "captain.hpp"
#include "detect_file.hpp"
#include <fstream>
#include <boost/filesystem.hpp>
#include <chrono>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>

namespace mpaxos {

using namespace std;
using namespace boost::filesystem;

std::string my_name;
node_id_t my_id;

Captain *captain;
std::string value;

int total = 70000;
//int total = 5000;
slot_id_t commit_counter = 0;
int global_counter = 0;
int self_counter = 0;
int send_counter = 0;
int control_sig = 0;
boost::mutex counter_mut;
boost::mutex global_mut;
boost::mutex exe_counter_mut;

std::vector<uint64_t> periods;
std::vector<uint64_t> exe_periods;
std::vector<int> trytimes;
std::vector<std::chrono::high_resolution_clock::time_point> starts(total);

std::chrono::high_resolution_clock::time_point start;

void generate_res() {
}

void do_sth(slot_id_t slot_id, std::string& data) {
//  LOG_INFO("exe_callback slot_id:%llu value:%s", slot_id, data.c_str());
//  global_mut.lock();
//  global_counter++;
//  global_mut.unlock();
}

void count_exe_latency(slot_id_t slot_id, PropValue& prop_value, node_id_t node_id) {

  exe_counter_mut.lock();

  LOG_INFO("exe_latency slot_id:%llu value_id:%llu value:%s", slot_id, prop_value.id(), prop_value.data().c_str());

  if (node_id == my_id && !prop_value.has_cmd_type()) {
    value_id_t value_id = prop_value.id() >> 16;
    auto finish = std::chrono::high_resolution_clock::now();
    exe_periods.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-starts[value_id % total]).count());
  }  

  exe_counter_mut.unlock();
}

void count_latency(slot_id_t slot_id, PropValue& prop_value, int try_time) {

  if (prop_value.has_cmd_type()) {
    counter_mut.lock();
    commit_counter++;
    counter_mut.unlock();
    LOG_INFO("count_latency triggered! but this is a command slot_id : %llu commit_counter : %llu ", slot_id, commit_counter);
    return;
  }
//  LOG_INFO("count_latency triggered! slot_id : %llu", slot_id);
  auto finish = std::chrono::high_resolution_clock::now();
  counter_mut.lock();
  commit_counter++;
  value_id_t value_id = prop_value.id() >> 16;
  periods.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-starts[value_id % total]).count());
  trytimes.push_back(try_time);

//  LOG_INFO("periods[%d] = %llu ns trytime = %d value_id : %llu slot_id: %llu commit_counter: %llu", self_counter, periods[self_counter], trytimes[self_counter], value_id, slot_id, commit_counter);

  self_counter++;

  std::string value = "Commiting Value Time_" + std::to_string(commit_counter) + " from " + my_name;
  starts[commit_counter % total] = std::chrono::high_resolution_clock::now();
  slot_id_t counter_tmp = commit_counter;
  counter_mut.unlock();

  if (counter_tmp <= total * 1) {
//    LOG_INFO("++++ I just Commit Value: %s ++++", value.c_str());
    if (counter_tmp % 10000 == 0) {
      auto finish = std::chrono::high_resolution_clock::now();
      uint64_t period = std::chrono::duration_cast<std::chrono::milliseconds>(finish-start).count();
      start = std::chrono::high_resolution_clock::now();
      int throughput = 10000 * 1000 / period;
      LOG_INFO("Last_commit -- counter:%d milliseconds:%llu throughput:%d", counter_tmp, period, throughput);
    }
    captain->commit(value);
  }
}

static void sig_int(int num) {
  std::cout << "Control + C triggered! " << std::endl;
  exit(num);
}  

int main(int argc, char** argv) {
  signal(SIGINT, sig_int);
 

  std::string config_file;
  if (argc < 5) {
    std::cout << "Usage: Node_ID Node_Num Value_Size Window_Size" << std::endl;
    return 0;
  }

  my_id = stoul(argv[1]); 
  int node_num = stoi(argv[2]);
  int value_size = stoi(argv[3]);
  int win_size = stoi(argv[4]);

  config_file = "config/localhost-" + to_string(node_num) + ".yaml";

  // init view for one captain
  View view(my_id, config_file);
  view.print_host_nodes();
  
  my_name = view.hostname();

  // init thread_pool 
  // pool tp(1);

  // init callback
  callback_t callback = do_sth;

  callback_latency_t call_latency = count_latency;

  callback_full_t callback_full = count_exe_latency;

  // init one specific captain

  captain = new Captain(view, win_size);

  captain->set_callback(call_latency);
  captain->set_callback(callback);
  captain->set_callback(callback_full);


  control_sig = 1;

  start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < win_size * 2; i++) {
    counter_mut.lock();
    commit_counter++;
    starts[commit_counter] = std::chrono::high_resolution_clock::now(); 
    counter_mut.unlock();
    std::string value = "Commiting Value Time_" + std::to_string(commit_counter) + " from " + view.hostname();
    LOG_INFO(" +++++++++++ Init Commit Value: %s +++++++++++", value.c_str());
    captain->commit(value);

//    LOG_INFO("***********************************************************************");
  }


  LOG_INFO("I'm sleeping");

  sleep(100000);

  return 0;
}

} // namespace mpaxos

int main(int argc, char** argv) {
  return mpaxos::main(argc, argv);
}
