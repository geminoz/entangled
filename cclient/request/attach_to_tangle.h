/*
 * Copyright (c) 2018 IOTA Stiftung
 * https://github.com/iotaledger/entangled
 *
 * Refer to the LICENSE file for licensing information
 */

#ifndef CCLIENT_REQUEST_ATTACH_TO_TANGLE_H
#define CCLIENT_REQUEST_ATTACH_TO_TANGLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "types/types.h"

#define ATTACH_TO_TANGLE_MAIN_MWM 14
#define ATTACH_TO_TANGLE_TEST_MWM 9

typedef struct {
  /**
   * Trunk transaction to approve
   */
  char_buffer_t* trunk;
  /**
   * branch transaction to approve
   */
  char_buffer_t* branch;
  /**
   * Min Weight Magnitude,Proof of Work intensity. Minimum value is 18
   */
  int32_t mwm;
  /**
   * List of trytes (raw transaction data) to attach to the tangle.
   */
  UT_array* trytes;

} attach_to_tangle_req_t;

attach_to_tangle_req_t* attach_to_tangle_req_new();
void attach_to_tangle_req_free(attach_to_tangle_req_t** req);
retcode_t attach_to_tangle_req_set_trunk(attach_to_tangle_req_t* req,
                                         const char* trunk);
retcode_t attach_to_tangle_req_set_branch(attach_to_tangle_req_t* req,
                                          const char* branch);
void attach_to_tangle_req_set_mwm(attach_to_tangle_req_t* req,
                                  const int32_t mwm);
void attach_to_tangle_req_add_trytes(attach_to_tangle_req_t* req,
                                     const char* trytes);

#ifdef __cplusplus
}
#endif

#endif  // CCLIENT_REQUEST_ATTACH_TO_TANGLE_H
