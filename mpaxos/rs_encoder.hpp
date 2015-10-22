/**
 * rs_encoder.hpp
 * Author: Lijing Wang
 * Date: 9/20/2015
 */

#pragma once
#include "fec.h"
#include <stdio.h>
#include "internal_types.hpp"
#include <math.h>

namespace mpaxos {

class RSEncoder {
 public:

  RSEncoder(node_id_t k, node_id_t n) 
    : k_(k), n_(n) {
//    LOG_INFO("Init Encoder k:%u n:%u", k_, n_);
    fec_ = fec_new(k_, n_);
    c_ = n_ - k_;
  }

  ~RSEncoder() {
    fec_free(fec_);
  }

  std::vector<std::string> rs_encode(std::string &data) {
    size_t size = data.size();  
    blk_size_ = ceil(double(size) / k_);
    unsigned char *src[k_];
    unsigned char *fecs[c_];
    unsigned char buf[blk_size_ * c_];
    unsigned nums[c_];

//    LOG_INFO("encoder block_size: %llu", blk_size_);
    for (slot_id_t i = 0; i < k_; i++) {
      src[i] = (unsigned char *)data.c_str() + i * blk_size_;
      encoded_data_.push_back(data.substr(i * blk_size_, blk_size_));
    }

    for (slot_id_t i = 0; i < c_; i++) {
      nums[i] = k_ + i;
      fecs[i] = buf + i * blk_size_; 
    }
    
    fec_encode(fec_, src, fecs, nums, c_, blk_size_); 

    for (slot_id_t i = 0; i < c_; i++) {
      std::string my_string(reinterpret_cast<const char *>(fecs[i]), blk_size_);
      encoded_data_.push_back(my_string);
    }

    return encoded_data_;
  }

  size_t get_blk_size() {
    return blk_size_;
  }

 private:
  fec_t *fec_;
  size_t blk_size_;
  node_id_t k_;
  node_id_t n_;
  node_id_t c_;
  std::vector<std::string> encoded_data_;
};

} // namespace mpaxos
