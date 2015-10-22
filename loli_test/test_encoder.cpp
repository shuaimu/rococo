/**
 * test_encoder.cpp
 * Author: Lijing
 * Date: 9/20/2015
 */
#include "rs_encoder.hpp"
#include "rs_decoder.hpp"
#include <iostream>

namespace mpaxos {

int main(int argc, char** argv) {

  slot_id_t k = 3;
  slot_id_t n = 5;
  RSEncoder encoder(k, n);
  std::string data = "Hi my name is Lijing Wang, I'm a PhD Student!";
  std::cout << "data_size: " << data.size() <<  std::endl;
  std::vector<std::string> encoded_data;
  encoded_data = encoder.rs_encode(data);

  RSDecoder decoder(k, n);
  std::vector<bool> map(n, true); 
  map[1] = false;
  map[2] = false;
  encoded_data[1] = "";
  encoded_data[2] = "";

  unsigned lost_num = 2;

  std::string decoded_data = decoder.rs_decode(encoded_data, map, lost_num);

  std::cout << "original " << data << std::endl;
  std::cout << "decoded  " << decoded_data << std::endl;

  return 0;
}

} // namespace mpaxos

int main(int argc, char** argv) {
  return mpaxos::main(argc, argv);
}
