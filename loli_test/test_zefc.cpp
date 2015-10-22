/**
 * test_zefc.cpp
 * Author: Shuai & Lijing
 * Date: 9/20/2015
 * State: Work Well
 */
#include "fec.h"
#include <stdio.h>

namespace mpaxos {

int main(int argc, char** argv) {

  fec_t *fec = NULL;
  fec = fec_new(2, 3);
  unsigned char buf0[10] = "hello ";
  unsigned char buf1[10] = "zfec!";
  unsigned char buf2[10];
  unsigned char *src[2];
  unsigned char *dst[1];
  src[0] = buf0;
  src[1] = buf1;
  dst[0] = buf2;

  unsigned nums[1];
  nums[0] = 2;

  fec_encode(fec, src, dst, nums, 1, 10); 

  unsigned char out1[10];
  unsigned char* out[1];
  unsigned char* inp[2];
  unsigned index[2];

  inp[0] = buf0;
  inp[1] = buf2;
  index[0] = 0;
  index[1] = 2;
  out[0] = out1;
  fec_decode(fec, inp, out, index, 10);  
  printf("%s%s\n", buf0, out[0]);

  fec_free(fec);
  return 0;
}

} // namespace mpaxos

int main(int argc, char** argv) {
  return mpaxos::main(argc, argv);
}
