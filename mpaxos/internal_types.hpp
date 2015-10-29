#pragma once
#include <cstdint>
#include "mpaxos.pb.h"
#include <mutex>
#include <chrono>

//#include "easylogging++.h"

namespace mpaxos {

// Regular
#define NRM      "\x1B[0m"
#define TXT_RED  "\x1B[31m"
#define TXT_GRN  "\x1B[32m"
#define TXT_YEL  "\x1B[33m"
#define TXT_BLU  "\x1B[34m"
#define TXT_MAG  "\x1B[35m"
#define TXT_CYN  "\x1B[36m"
#define TXT_WHT  "\x1B[37m"
// Bold
#define BLD_RED  "\e[1;31m"
#define BLD_GRN  "\e[1;32m"
#define BLD_YEL  "\e[1;33m"
#define BLD_BLU  "\e[1;34m"
#define BLD_MAG  "\e[1;35m"
#define BLD_CYN  "\e[1;36m"
// Underline
#define UND_RED  "\e[4;31m"
#define UND_GRN  "\e[4;32m"
#define UND_YEL  "\e[4;33m"
#define UND_BLU  "\e[4;34m"
#define UND_MAG  "\e[4;35m"
#define UND_CYN  "\e[4;36m"
// Background
#define BAK_RED  "\e[41m"
#define BAK_GRN  "\e[42m"
#define BAK_YEL  "\e[43m"
#define BAK_BLU  "\e[44m"
#define BAK_MAG  "\e[45m"
#define BAK_CYN  "\e[46m"

#if LOG_LEVEL >= 6 
#define LOG_TRACE(...)  printf("%s[TRACE]%s ", BLD_MAG, NRM); printf(__VA_ARGS__); printf("\n")
#define LOG_TRACE_PRO(...) printf("%s[TRACE]%s %sPROPOSER ", BLD_MAG, NRM, TXT_RED); printf(__VA_ARGS__); printf("%s\n", NRM)
#define LOG_TRACE_ACC(...) printf("%s[TRACE]%s %sACCEPTOR ", BLD_MAG, NRM, TXT_GRN); printf(__VA_ARGS__); printf("%s\n", NRM)
#define LOG_TRACE_CAP(...) printf("%s[TRACE]%s %sCAPTAIN ", BLD_MAG, NRM, TXT_CYN); printf(__VA_ARGS__); printf("%s\n", NRM)
#define LOG_TRACE_COM(...) printf("%s[TRACE]%s %sCOMMO ", BLD_MAG, NRM, TXT_YEL); printf(__VA_ARGS__); printf("%s\n", NRM)
#else
#define LOG_TRACE(...)
#define LOG_TRACE_PRO(...) 
#define LOG_TRACE_ACC(...) 
#define LOG_TRACE_CAP(...) 
#define LOG_TRACE_COM(...) 
#endif

#if LOG_LEVEL >= 5 
#define LOG_DEBUG(...) printf("%s[DEBUG]%s ", BAK_YEL, NRM); printf(__VA_ARGS__);  printf("\n")
#define LOG_DEBUG_PRO(...) printf("%s[DEBUG]%s %sPROPOSER ", BAK_YEL, NRM, TXT_RED); printf(__VA_ARGS__); printf("%s\n", NRM)
#define LOG_DEBUG_ACC(...) printf("%s[DEBUG]%s %sACCEPTOR ", BAK_YEL, NRM, TXT_GRN); printf(__VA_ARGS__); printf("%s\n", NRM)
#define LOG_DEBUG_CAP(...) printf("%s[DEBUG]%s %sCAPTAIN ", BAK_YEL, NRM, TXT_CYN); printf(__VA_ARGS__); printf("%s\n", NRM)
#define LOG_DEBUG_COM(...) printf("%s[DEBUG]%s %sCOMMO ", BAK_YEL, NRM, TXT_YEL); printf(__VA_ARGS__); printf("%s\n", NRM)
#else
#define LOG_DEBUG(...)
#define LOG_DEBUG_PRO(...) 
#define LOG_DEBUG_ACC(...) 
#define LOG_DEBUG_CAP(...) 
#define LOG_DEBUG_COM(...) 
#endif

#if LOG_LEVEL >= 4 
#define LOG_INFO(...) printf("%s[INFO]%s  ", UND_BLU, NRM); printf(__VA_ARGS__);  printf("\n")
#define LOG_INFO_PRO(...) printf("%s[INFO]%s  %sPROPOSER ", UND_BLU, NRM, TXT_RED); printf(__VA_ARGS__); printf("%s\n", NRM)
#define LOG_INFO_ACC(...) printf("%s[INFO]%s  %sACCEPTOR ", UND_BLU, NRM, TXT_GRN); printf(__VA_ARGS__); printf("%s\n", NRM)
#define LOG_INFO_CAP(...) printf("%s[INFO]%s  %sCAPTAIN ", UND_BLU, NRM, TXT_CYN); printf(__VA_ARGS__); printf("%s\n", NRM)
#define LOG_INFO_COM(...) printf("%s[INFO]%s  %sCOMMO ", UND_BLU, NRM, TXT_YEL); printf(__VA_ARGS__); printf("%s\n", NRM)
#else
#define LOG_INFO(...)
#define LOG_INFO_PRO(...) 
#define LOG_INFO_ACC(...) 
#define LOG_INFO_CAP(...) 
#define LOG_INFO_COM(...) 
#endif

enum AckType {
  DROP = 0,
  NOT_ENOUGH = 1,
  CONTINUE = 2,
  RESTART = 3,
  CHOOSE = 4
};

enum ProposerStatus {
  INIT = 0,
  PHASEI = 1,
  PHASEII = 2,
  CHOSEN = 3,
  DONE = 4,
  DIE = 5,
  EMPTY = 6
};

struct host_info_t {
  std::string name;  
  std::string addr;
  int32_t port;
  host_info_t() {};
  host_info_t(std::string init_name, std::string init_addr, int32_t init_port) 
    : name(init_name), addr(init_addr), port(init_port) {
  }; 
};

using node_id_t = uint16_t;
using slot_id_t = uint64_t;
using ballot_id_t = uint64_t;
using value_id_t = uint64_t;

using callback_t = std::function<void(slot_id_t, std::string&)>;
using callback_full_t = std::function<void(slot_id_t, PropValue&, node_id_t)>;
using callback_latency_t = std::function<void(slot_id_t, PropValue&, int)>;

}  // namespace mpaxos
