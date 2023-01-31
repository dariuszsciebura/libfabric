/*
 * Copyright (c) 2018-2023 Amazon.com, Inc. or its affiliates. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef EFA_AV_H
#define EFA_AV_H

#include <infiniband/verbs.h>
#include "rdm/rdm_proto_v4.h"
#include "rdm/efa_rdm_peer.h"

#define EFA_MIN_AV_SIZE (16384)
#define EFA_SHM_MAX_AV_COUNT       (256)

struct efa_ah {
	uint8_t		gid[EFA_GID_LEN]; /* efa device GID */
	struct ibv_ah	*ibv_ah; /* created by ibv_create_ah() using GID */
	uint16_t	ahn; /* adress handle number */
	int		refcnt; /* reference counter. Multiple efa_conn can share an efa_ah */
	UT_hash_handle	hh; /* hash map handle, link all efa_ah with efa_ep->ah_map */
};

struct efa_conn {
	struct efa_ah		*ah;
	struct efa_ep_addr	*ep_addr;
	/* for FI_AV_TABLE, fi_addr is same as util_av_fi_addr,
	 * for FI_AV_MAP, fi_addr is pointer to efa_conn; */
	fi_addr_t		fi_addr;
	fi_addr_t		util_av_fi_addr;
	struct efa_rdm_peer	rdm_peer;
};

struct efa_av_entry {
	uint8_t			ep_addr[EFA_EP_ADDR_LEN];
	struct efa_conn		conn;
};

struct efa_cur_reverse_av_key {
	uint16_t ahn;
	uint16_t qpn;
};

struct efa_cur_reverse_av {
	struct efa_cur_reverse_av_key key;
	struct efa_conn *conn;
	UT_hash_handle hh;
};

struct efa_prv_reverse_av_key {
	uint16_t ahn;
	uint16_t qpn;
	uint32_t connid;
};

struct efa_prv_reverse_av {
	struct efa_prv_reverse_av_key key;
	struct efa_conn *conn;
	UT_hash_handle hh;
};

struct efa_av {
	struct fid_av *shm_rdm_av;
	fi_addr_t shm_rdm_addr_map[EFA_SHM_MAX_AV_COUNT];
	struct efa_domain *domain;
	struct efa_base_ep *base_ep;
	size_t used;
	size_t shm_used;
	enum fi_av_type type;
	/* cur_reverse_av is a map from (ahn + qpn) to current (latest) efa_conn.
	 * prv_reverse_av is a map from (ahn + qpn + connid) to all previous efa_conns.
	 * cur_reverse_av is faster to search because its key size is smaller
	 */
	struct efa_cur_reverse_av *cur_reverse_av;
	struct efa_prv_reverse_av *prv_reverse_av;
	struct efa_ah *ah_map;
	struct util_av util_av;
	enum fi_ep_type ep_type;
};

int efa_av_open(struct fid_domain *domain_fid, struct fi_av_attr *attr,
		struct fid_av **av_fid, void *context);

int efa_av_insert_one(struct efa_av *av, struct efa_ep_addr *addr,
		      fi_addr_t *fi_addr, uint64_t flags, void *context);

struct efa_conn *efa_av_addr_to_conn(struct efa_av *av, fi_addr_t fi_addr);

fi_addr_t efa_av_reverse_lookup_rdm(struct efa_av *av, uint16_t ahn, uint16_t qpn, struct rxr_pkt_entry *pkt_entry);

fi_addr_t efa_av_reverse_lookup_dgram(struct efa_av *av, uint16_t ahn, uint16_t qpn);

#endif