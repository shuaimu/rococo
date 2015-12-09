#include "marshal-value.h"
#include "coordinator.h"
#include "frame.h"
#include "constants.h"
#include "sharding.h"
#include "txn_req_factory.h"
#include "txn-chopper-factory.h"
#include "benchmark_control_rpc.h"

/**
 * What shoud we do to change this to asynchronous?
 * 1. Fisrt we need to have a queue to hold all transaction requests.
 * 2. pop next request, send start request for each piece until there is no
 *available left.
 *          in the callback, send the next piece of start request.
 *          if responses to all start requests are collected.
 *              send the finish request
 *                  in callback, remove it from queue.
 *
 *
 */

namespace rococo {
Coordinator::Coordinator(uint32_t coo_id,
                         std::vector<std::string> &addrs,
                         int32_t benchmark,
                         int32_t mode,
                         ClientControlServiceImpl *ccsi,
                         uint32_t thread_id,
                         bool batch_optimal) : coo_id_(coo_id),
                                               benchmark_(benchmark),
                                               mode_(mode),
                                               ccsi_(ccsi),
                                               thread_id_(thread_id),
                                               batch_optimal_(batch_optimal),
                                               site_prepare_(addrs.size(), 0),
                                               site_commit_(addrs.size(), 0),
                                               site_abort_(addrs.size(), 0),
                                               site_piece_(addrs.size(), 0),
                                               mtx_(),
                                               start_mtx_() {
  uint64_t k = coo_id_;
  k <<= 32;
  k++;
  commo_ = new Commo(addrs);
  this->next_pie_id_.store(k);
  this->next_txn_id_.store(k);
  recorder_ = NULL;
  retry_wait_ = Config::GetConfig()->retry_wait();
  verify(mode_ != 0);
}

Coordinator::~Coordinator() {
  for (int i = 0; i < site_prepare_.size(); i++) {
    Log_info("Coo: %u, Site: %d, piece: %d, "
                 "prepare: %d, commit: %d, abort: %d",
             coo_id_, i, site_piece_[i], site_prepare_[i],
             site_commit_[i], site_abort_[i]);
  }

  if (commo_) {
    delete commo_;
  }

  if (recorder_) delete recorder_;
#ifdef TXN_STAT

  for (auto& it : txn_stats_) {
        Log::info("TXN: %d", it.first);
        it.second.output();
      }
#endif /* ifdef TXN_STAT */

// TODO (shuai) destroy all the rpc clients and proxies.
}
} // namespace rococo
