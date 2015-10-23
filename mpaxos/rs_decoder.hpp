/**
 * rs_decoder.hpp
 * Author: Lijing Wang
 * Date: 9/20/2015
 */

#pragma once
#include "fec.h"
#include <stdio.h>
namespace mpaxos {

class RSDecoder {
 public:

  RSDecoder(node_id_t k, node_id_t n) 
    : k_(k), n_(n) {
//    LOG_INFO("Init Decoder k:%u n:%u", k_, n_);
    fec_ = fec_new(k_, n_);
    c_ = n_ - k_;
    decoded_data_ = "";
  }

  ~RSDecoder() {
    fec_free(fec_);
  }

  std::string rs_decode(std::vector<std::string> &blocks, std::vector<bool> &map, unsigned lost_num) {
    blk_size_ = blocks[0].size(); 
//    LOG_INFO("decoder block_size: %llu", blk_size_);
    unsigned char *in_recovery[k_];
    unsigned char *out_recovery[lost_num];
    unsigned char buf_recovery[lost_num * blk_size_];
    unsigned index[k_];
    slot_id_t r = k_;
    for (slot_id_t i = 0; i < k_; i++) {

      if (map[i]) {
        in_recovery[i] = (unsigned char *)blocks[i].c_str();
        index[i] = i;
      }
      else {
        while (!map[r]) r++;
        in_recovery[i] = (unsigned char *)blocks[r].c_str();
        index[i] = r++;
      }  

    }

    for (slot_id_t i = 0; i < lost_num; i++) {
      out_recovery[i] = buf_recovery + i * blk_size_;
    }

    fec_decode(fec_ , in_recovery, out_recovery, index, blk_size_);

    r = 0;
    for (slot_id_t i = 0; i < k_; i++) {
      if (map[i]) {
        decoded_data_.append(blocks[i]);
      } else {
        std::string my_string(reinterpret_cast<const char *>(out_recovery[r++]), blk_size_);
        decoded_data_.append(my_string);
      }
    }
    return decoded_data_;
  } 

 private:
  fec_t *fec_;
  size_t blk_size_;
  node_id_t k_;
  node_id_t n_;
  node_id_t c_;
  std::string decoded_data_;
};

} // namespace mpaxos
