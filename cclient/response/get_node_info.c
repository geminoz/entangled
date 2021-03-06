/*
 * Copyright (c) 2018 IOTA Stiftung
 * https://github.com/iotaledger/entangled
 *
 * Refer to the LICENSE file for licensing information
 */

#include "get_node_info.h"

get_node_info_res_t* get_node_info_res_new() {
  get_node_info_res_t* res =
      (get_node_info_res_t*)malloc(sizeof(get_node_info_res_t));

  res->app_name = char_buffer_new();
  res->app_version = char_buffer_new();
  res->latest_milestone = char_buffer_new();
  res->latest_solid_subtangle_milestone = char_buffer_new();

  return res;
}

void get_node_info_res_free(get_node_info_res_t* res) {
  if (res) {
    char_buffer_free(res->app_name);
    char_buffer_free(res->app_version);
    char_buffer_free(res->latest_milestone);
    char_buffer_free(res->latest_solid_subtangle_milestone);
    free(res);
    res = NULL;
  }
}
