/*
 * Copyright (c) 2018 IOTA Stiftung
 * https://github.com/iotaledger/entangled
 *
 * Refer to the LICENSE file for licensing information
 */

#include <stdlib.h>
#include <string.h>

#include <unity/unity.h>

#include "common/model/milestone.h"
#include "common/model/transaction.h"
#include "common/storage/connection.h"
#include "common/storage/sql/defs.h"
#include "common/storage/storage.h"
#include "common/storage/tests/helpers/defs.h"
#include "utils/files.h"

#if defined(FLEX_TRIT_ENCODING_1_TRIT_PER_BYTE)
const flex_trit_t HASH[] = {
    1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1,
    0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0,
    0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,
    1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1,
    0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0,
    0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,
    1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1,
    0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0,
    0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,
    1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,
};
#elif defined(FLEX_TRIT_ENCODING_3_TRITS_PER_BYTE)
const flex_trit_t HASH[] = {
    65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
    65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
    65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
    65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
    65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
};
#elif defined(FLEX_TRIT_ENCODING_4_TRITS_PER_BYTE)
const flex_trit_t HASH[] = {
    65, 16, 4, 65, 16, 4, 65, 16, 4, 65, 16, 4, 65, 16, 4, 65, 16, 4, 65, 16, 4,
    65, 16, 4, 65, 16, 4, 65, 16, 4, 65, 16, 4, 65, 16, 4, 65, 16, 4, 65, 16, 4,
    65, 16, 4, 65, 16, 4, 65, 16, 4, 65, 16, 4, 65, 16, 4, 65, 16, 4, 1,
};
#elif defined(FLEX_TRIT_ENCODING_5_TRITS_PER_BYTE)
const flex_trit_t HASH[] = {
    28, 84, 9,  28, 84, 9,  28, 84, 9,  28, 84, 9,  28, 84, 9,  28, 84,
    9,  28, 84, 9,  28, 84, 9,  28, 84, 9,  28, 84, 9,  28, 84, 9,  28,
    84, 9,  28, 84, 9,  28, 84, 9,  28, 84, 9,  28, 84, 9,  1,
};
#endif

static connection_t conn;

void test_init_connection(void) {
  connection_config_t config;
  config.db_path = "common/storage/sql/sqlite3/tests/test.db";
  config.index_transaction_address = true;
  config.index_transaction_approvee = true;
  config.index_transaction_bundle = true;
  config.index_transaction_tag = true;
  config.index_transaction_hash = true;
  config.index_milestone_hash = true;
  TEST_ASSERT(init_connection(&conn, &config) == RC_OK);
}

void test_initialized_db_empty_transaction(void) {
  bool exist = false;

  TRIT_ARRAY_DECLARE(hash, NUM_TRITS_HASH);
  memcpy(hash.trits, HASH, FLEX_TRIT_SIZE_243);
  TEST_ASSERT(iota_stor_milestone_exist(&conn, MILESTONE_COL_HASH, &hash,
                                        &exist) == RC_OK);
  TEST_ASSERT(exist == false);
}

void test_initialized_db_empty_milestone(void) {
  bool exist = false;

  TRIT_ARRAY_DECLARE(hash, NUM_TRITS_HASH);
  memcpy(hash.trits, HASH, FLEX_TRIT_SIZE_243);
  TEST_ASSERT(iota_stor_transaction_exist(&conn, TRANSACTION_COL_HASH, &hash,
                                          &exist) == RC_OK);
  TEST_ASSERT(exist == false);
}

void test_stored_transaction(void) {
  TEST_ASSERT(iota_stor_transaction_store(
                  &conn, (iota_transaction_t)&TEST_TRANSACTION) == RC_OK);
  // Test primary key constraint violation
  TEST_ASSERT(iota_stor_transaction_store(
                  &conn, (iota_transaction_t)&TEST_TRANSACTION) ==
              RC_SQLITE3_FAILED_STEP);
  bool exist = false;

  TRIT_ARRAY_DECLARE(hash, NUM_TRITS_HASH);
  memcpy(hash.trits, TEST_TRANSACTION.hash, FLEX_TRIT_SIZE_243);
  TEST_ASSERT(iota_stor_transaction_exist(&conn, NULL, NULL, &exist) == RC_OK);
  TEST_ASSERT(exist == true);
  TEST_ASSERT(iota_stor_transaction_exist(&conn, TRANSACTION_COL_HASH, &hash,
                                          &exist) == RC_OK);
  TEST_ASSERT(exist == true);

  TEST_ASSERT(iota_stor_transaction_exist(&conn, NULL, NULL, &exist) == RC_OK);
  TEST_ASSERT(exist == true);

  iota_stor_pack_t pack;
  iota_transaction_t txs[5];
  pack.models = (void **)txs;
  pack.num_loaded = 0;
  pack.capacity = 5;

  for (int i = 0; i < 5; ++i) {
    pack.models[i] = transaction_new();
  }

  TEST_ASSERT(iota_stor_transaction_load(&conn, TRANSACTION_COL_HASH, &hash,
                                         &pack) == RC_OK);
  TEST_ASSERT_EQUAL_INT(1, pack.num_loaded);

  TEST_ASSERT_EQUAL_MEMORY(txs[0]->nonce, TEST_TRANSACTION.nonce,
                           FLEX_TRIT_SIZE_81);
  TEST_ASSERT_EQUAL_MEMORY(txs[0]->signature_or_message,
                           TEST_TRANSACTION.signature_or_message,
                           FLEX_TRIT_SIZE_6561);
  TEST_ASSERT_EQUAL_MEMORY(txs[0]->address, TEST_TRANSACTION.address,
                           FLEX_TRIT_SIZE_243);
  TEST_ASSERT_EQUAL_MEMORY(txs[0]->branch, TEST_TRANSACTION.branch,
                           FLEX_TRIT_SIZE_243);
  TEST_ASSERT_EQUAL_MEMORY(txs[0]->trunk, TEST_TRANSACTION.trunk,
                           FLEX_TRIT_SIZE_243);
  TEST_ASSERT_EQUAL_MEMORY(txs[0]->bundle, TEST_TRANSACTION.bundle,
                           FLEX_TRIT_SIZE_243);
  TEST_ASSERT_EQUAL_INT(txs[0]->value, TEST_TRANSACTION.value);
  TEST_ASSERT_EQUAL_INT(txs[0]->attachment_timestamp,
                        TEST_TRANSACTION.attachment_timestamp);
  TEST_ASSERT_EQUAL_INT(txs[0]->attachment_timestamp_upper,
                        TEST_TRANSACTION.attachment_timestamp_upper);
  TEST_ASSERT_EQUAL_INT(txs[0]->attachment_timestamp_lower,
                        TEST_TRANSACTION.attachment_timestamp_lower);
  TEST_ASSERT_EQUAL_INT(txs[0]->timestamp, TEST_TRANSACTION.timestamp);
  TEST_ASSERT_EQUAL_INT(txs[0]->current_index, TEST_TRANSACTION.current_index);
  TEST_ASSERT_EQUAL_INT(txs[0]->last_index, TEST_TRANSACTION.last_index);
  TEST_ASSERT_EQUAL_MEMORY(txs[0]->hash, TEST_TRANSACTION.hash,
                           FLEX_TRIT_SIZE_243);

  for (int i = 0; i < 5; ++i) {
    transaction_free(pack.models[i]);
  }
}

void test_stored_milestone(void) {
  TRIT_ARRAY_DECLARE(hash, NUM_TRITS_HASH);
  memcpy(hash.trits, HASH, FLEX_TRIT_SIZE_243);
  iota_milestone_t milestone;
  milestone.index = 42;
  memcpy(milestone.hash, HASH, FLEX_TRIT_SIZE_243);

  TEST_ASSERT(iota_stor_milestone_store(&conn, &milestone) == RC_OK);
  // Test id primary key constraint violation
  TEST_ASSERT(iota_stor_milestone_store(&conn, &milestone) ==
              RC_SQLITE3_FAILED_STEP);
  // Test hash unique constraint violation
  milestone.index++;
  TEST_ASSERT(iota_stor_milestone_store(&conn, &milestone) ==
              RC_SQLITE3_FAILED_STEP);
  milestone.index--;

  bool exist = false;
  TEST_ASSERT(iota_stor_milestone_exist(&conn, NULL, NULL, &exist) == RC_OK);
  TEST_ASSERT(exist == true);
  TEST_ASSERT(iota_stor_milestone_exist(&conn, MILESTONE_COL_HASH, &hash,
                                        &exist) == RC_OK);
  TEST_ASSERT(exist == true);
  TEST_ASSERT(iota_stor_milestone_exist(&conn, NULL, NULL, &exist) == RC_OK);
  TEST_ASSERT(exist == true);

  iota_stor_pack_t pack;
  iota_milestone_t *milestones[5];
  pack.models = (void **)milestones;
  pack.num_loaded = 0;
  pack.capacity = 5;

  for (int i = 0; i < 5; ++i) {
    pack.models[i] = malloc(sizeof(iota_milestone_t));
  }
  TEST_ASSERT(iota_stor_milestone_load(&conn, MILESTONE_COL_HASH, &hash,
                                       &pack) == RC_OK);
  TEST_ASSERT_EQUAL_INT(1, pack.num_loaded);
  TEST_ASSERT_EQUAL_INT(milestones[0]->index, 42);
  TEST_ASSERT_EQUAL_MEMORY(milestones[0]->hash, hash.trits, FLEX_TRIT_SIZE_243);
  for (int i = 0; i < 5; ++i) {
    free(pack.models[i]);
  }
}

void test_stored_load_hashes_by_address(void) {
  trit_array_p hashes[5];
  iota_stor_pack_t pack;
  pack.models = (void **)hashes;
  pack.num_loaded = 0;
  pack.capacity = 5;
  for (int i = 0; i < pack.capacity; ++i) {
    pack.models[i] = trit_array_new(NUM_TRITS_ADDRESS);
  }
  pack.capacity = 5;
  TRIT_ARRAY_DECLARE(key, NUM_TRITS_HASH);
  memcpy(key.trits, TEST_TRANSACTION.address, FLEX_TRIT_SIZE_243);
  TEST_ASSERT(iota_stor_transaction_load_hashes(&conn, TRANSACTION_COL_ADDRESS,
                                                &key, &pack) == RC_OK);
  TEST_ASSERT_EQUAL_INT(1, pack.num_loaded);
  TEST_ASSERT_EQUAL_MEMORY(TEST_TRANSACTION.hash,
                           ((trit_array_p)pack.models[0])->trits,
                           FLEX_TRIT_SIZE_243);

  for (int i = 0; i < pack.capacity; ++i) {
    trit_array_free(pack.models[i]);
  }
}

void test_stored_load_hashes_of_approvers(void) {
  trit_array_p hashes[5];
  iota_stor_pack_t pack;
  pack.models = (void **)hashes;
  pack.num_loaded = 0;
  pack.capacity = 5;
  for (int i = 0; i < pack.capacity; ++i) {
    pack.models[i] = trit_array_new(NUM_TRITS_HASH);
  }
  pack.capacity = 5;
  TRIT_ARRAY_DECLARE(key, NUM_TRITS_HASH);
  memcpy(key.trits, TEST_TRANSACTION.address, FLEX_TRIT_SIZE_243);
  TEST_ASSERT(iota_stor_transaction_load_hashes_of_approvers(&conn, &key,
                                                             &pack) == RC_OK);
  TEST_ASSERT_EQUAL_INT(0, pack.num_loaded);

  for (int i = 0; i < pack.capacity; ++i) {
    trit_array_free(pack.models[i]);
  }
}

int main(void) {
  UNITY_BEGIN();

  copy_file("common/storage/sql/sqlite3/tests/test.db",
            "common/storage/sql/sqlite3/tests/ciri.db");

  RUN_TEST(test_init_connection);
  RUN_TEST(test_initialized_db_empty_transaction);
  RUN_TEST(test_initialized_db_empty_milestone);
  RUN_TEST(test_stored_transaction);
  RUN_TEST(test_stored_milestone);
  RUN_TEST(test_stored_load_hashes_by_address);
  RUN_TEST(test_stored_load_hashes_of_approvers);

  return UNITY_END();
}
