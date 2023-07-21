/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 * Copyright (C) 2021-2023 by Cornelis Networks.
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
#include <ofi.h>

#include "rdma/opx/fi_opx_domain.h"
#include "rdma/opx/fi_opx_endpoint.h"
#include "rdma/opx/fi_opx.h"
#include "rdma/opx/fi_opx_internal.h"

#include <ofi_enosys.h>

ssize_t fi_opx_sendmsg(struct fid_ep *ep, const struct fi_msg *msg,
			uint64_t flags)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "\n");

	struct fi_opx_ep * opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	const enum fi_threading threading = opx_ep->threading;
	const enum fi_av_type av_type = opx_ep->av_type;
	const enum ofi_reliability_kind reliability = opx_ep->reliability->state.kind;

	const uint64_t caps = opx_ep->tx->caps & (FI_LOCAL_COMM | FI_REMOTE_COMM);
	const int lock_required = fi_opx_threading_lock_required(threading, fi_opx_global.progress);

	return fi_opx_ep_tx_send(ep, msg->msg_iov, msg->iov_count,
		msg->desc, msg->addr, 0, msg->context, msg->data,
		lock_required,
		av_type,
		OPX_CONTIG_FALSE,
		OPX_FLAGS_OVERRIDE_TRUE,
		flags,
		caps | FI_MSG,
		reliability);
}

ssize_t fi_opx_sendv(struct fid_ep *ep, const struct iovec *iov,
			void **desc, size_t count, fi_addr_t dest_addr,
			void *context)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "\n");

	struct fi_opx_ep * opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	const enum fi_threading threading = opx_ep->threading;
	const enum fi_av_type av_type = opx_ep->av_type;
	const enum ofi_reliability_kind reliability = opx_ep->reliability->state.kind;

	const uint64_t caps = opx_ep->tx->caps & (FI_LOCAL_COMM | FI_REMOTE_COMM);
	const int lock_required = fi_opx_threading_lock_required(threading, fi_opx_global.progress);

	return fi_opx_ep_tx_send(ep, iov, count,
		desc, dest_addr, 0, context, 0,
		lock_required,
		av_type,
		OPX_CONTIG_FALSE,
		OPX_FLAGS_OVERRIDE_FALSE,
		0,	/* flags */
		caps | FI_MSG,
		reliability);
}


ssize_t fi_opx_senddata(struct fid_ep *ep, const void *buf, size_t len, void *desc,
			uint64_t data, void *context)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "\n");

	errno = FI_ENOSYS;
	return -errno;
}

/* FI_LOCAL_COMM | FI_REMOTE_COMM = 0x0018000000000000ull */
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0018000000000000ull, OFI_RELIABILITY_KIND_NONE)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0018000000000000ull, OFI_RELIABILITY_KIND_NONE)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0018000000000000ull, OFI_RELIABILITY_KIND_NONE)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0018000000000000ull, OFI_RELIABILITY_KIND_NONE)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0018000000000000ull, OFI_RELIABILITY_KIND_NONE)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0018000000000000ull, OFI_RELIABILITY_KIND_NONE)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0018000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0018000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0018000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0018000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0018000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0018000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0018000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0018000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0018000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0018000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0018000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0018000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)


/* FI_LOCAL_COMM = 0x0008000000000000ull */
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0008000000000000ull, OFI_RELIABILITY_KIND_NONE)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0008000000000000ull, OFI_RELIABILITY_KIND_NONE)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0008000000000000ull, OFI_RELIABILITY_KIND_NONE)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0008000000000000ull, OFI_RELIABILITY_KIND_NONE)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0008000000000000ull, OFI_RELIABILITY_KIND_NONE)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0008000000000000ull, OFI_RELIABILITY_KIND_NONE)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0008000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0008000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0008000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0008000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0008000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0008000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0008000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0008000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0008000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0008000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0008000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0008000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)


/* FI_REMOTE_COMM = 0x0010000000000000ull */
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0010000000000000ull, OFI_RELIABILITY_KIND_NONE)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0010000000000000ull, OFI_RELIABILITY_KIND_NONE)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0010000000000000ull, OFI_RELIABILITY_KIND_NONE)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0010000000000000ull, OFI_RELIABILITY_KIND_NONE)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0010000000000000ull, OFI_RELIABILITY_KIND_NONE)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0010000000000000ull, OFI_RELIABILITY_KIND_NONE)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0010000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0010000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0010000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0010000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0010000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0010000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0010000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0010000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0010000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0010000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0010000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)
FI_OPX_MSG_SPECIALIZED_FUNC(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0010000000000000ull, OFI_RELIABILITY_KIND_ONLOAD)



#define FI_OPX_MSG_OPS_STRUCT_NAME(LOCK, AV, CAPS, RELIABILITY)					\
	FI_OPX_MSG_OPS_STRUCT_NAME_(LOCK, AV, CAPS, RELIABILITY)

#define FI_OPX_MSG_OPS_STRUCT_NAME_(LOCK, AV, CAPS, RELIABILITY)				\
	fi_opx_ops_msg_ ## LOCK ## _ ## AV ## _ ## CAPS ## _ ## RELIABILITY

#define FI_OPX_MSG_OPS_STRUCT(LOCK,AV,CAPS,RELIABILITY)						\
static struct fi_ops_msg									\
	FI_OPX_MSG_OPS_STRUCT_NAME(LOCK,AV,CAPS,RELIABILITY) __attribute__ ((unused)) = {		\
	.size		= sizeof(struct fi_ops_msg),						\
	.recv		= FI_OPX_MSG_SPECIALIZED_FUNC_NAME(recv, LOCK, AV, CAPS, RELIABILITY),	\
	.recvv		= fi_no_msg_recvv,							\
	.recvmsg	= FI_OPX_MSG_SPECIALIZED_FUNC_NAME(recvmsg, LOCK, AV, CAPS, RELIABILITY),	\
	.send		= FI_OPX_MSG_SPECIALIZED_FUNC_NAME(send, LOCK, AV, CAPS, RELIABILITY),	\
	.sendv		= fi_opx_sendv,							\
	.sendmsg	= fi_opx_sendmsg,							\
	.inject		= FI_OPX_MSG_SPECIALIZED_FUNC_NAME(inject, LOCK, AV, CAPS, RELIABILITY),	\
	.senddata	= FI_OPX_MSG_SPECIALIZED_FUNC_NAME(senddata, LOCK, AV, CAPS, RELIABILITY),	\
	.injectdata	= FI_OPX_MSG_SPECIALIZED_FUNC_NAME(injectdata, LOCK, AV, CAPS, RELIABILITY),\
}

/* FI_LOCAL_COMM | FI_REMOTE_COMM = 0x0018000000000000ull */
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0018000000000000ull, OFI_RELIABILITY_KIND_NONE);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0018000000000000ull, OFI_RELIABILITY_KIND_NONE);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0018000000000000ull, OFI_RELIABILITY_KIND_NONE);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0018000000000000ull, OFI_RELIABILITY_KIND_NONE);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0018000000000000ull, OFI_RELIABILITY_KIND_NONE);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0018000000000000ull, OFI_RELIABILITY_KIND_NONE);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0018000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0018000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0018000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0018000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0018000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0018000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0018000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0018000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0018000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0018000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0018000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0018000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);


/* FI_LOCAL_COMM = 0x0008000000000000ull */
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0008000000000000ull, OFI_RELIABILITY_KIND_NONE);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0008000000000000ull, OFI_RELIABILITY_KIND_NONE);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0008000000000000ull, OFI_RELIABILITY_KIND_NONE);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0008000000000000ull, OFI_RELIABILITY_KIND_NONE);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0008000000000000ull, OFI_RELIABILITY_KIND_NONE);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0008000000000000ull, OFI_RELIABILITY_KIND_NONE);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0008000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0008000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0008000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0008000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0008000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0008000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0008000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0008000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0008000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0008000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0008000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0008000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);


/* FI_REMOTE_COMM = 0x0010000000000000ull */
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0010000000000000ull, OFI_RELIABILITY_KIND_NONE);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0010000000000000ull, OFI_RELIABILITY_KIND_NONE);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0010000000000000ull, OFI_RELIABILITY_KIND_NONE);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0010000000000000ull, OFI_RELIABILITY_KIND_NONE);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0010000000000000ull, OFI_RELIABILITY_KIND_NONE);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0010000000000000ull, OFI_RELIABILITY_KIND_NONE);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0010000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0010000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0010000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0010000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0010000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0010000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0010000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0010000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0010000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0010000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_UNSPEC, 0x0010000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
FI_OPX_MSG_OPS_STRUCT(FI_OPX_LOCK_REQUIRED,FI_AV_UNSPEC, 0x0010000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);




static struct fi_ops_msg fi_opx_no_msg_ops = {
	.size		= sizeof(struct fi_ops_msg),
	.recv		= fi_no_msg_recv,
	.recvv		= fi_no_msg_recvv,
	.recvmsg	= fi_no_msg_recvmsg,
	.send		= fi_no_msg_send,
	.sendv		= fi_no_msg_sendv,
	.sendmsg	= fi_no_msg_sendmsg,
	.inject		= fi_no_msg_inject,
	.senddata	= fi_no_msg_senddata,
	.injectdata	= fi_no_msg_injectdata
};

int fi_opx_init_msg_ops(struct fid_ep *ep, struct fi_info *info)
{
	struct fi_opx_ep * opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	if (!info || !opx_ep) {
		errno = FI_EINVAL;
		goto err;
	}
	if (info->caps & FI_MSG ||
			(info->tx_attr &&
			 (info->tx_attr->caps & FI_MSG))) {

       opx_ep->rx->min_multi_recv = sizeof(union fi_opx_hfi1_packet_payload);
	}

	return 0;

err:
	return -errno;
}

int fi_opx_enable_msg_ops(struct fid_ep *ep)
{
	struct fi_opx_ep * opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	if (!opx_ep || !opx_ep->domain)
		return -FI_EINVAL;

	if (!(opx_ep->tx->caps & FI_MSG) || !(opx_ep->rx->caps & FI_MSG)) {
		/* Messaging ops not enabled on this endpoint */
		opx_ep->ep_fid.msg = &fi_opx_no_msg_ops;
		return 0;
	}

	if ((opx_ep->tx->caps & (FI_LOCAL_COMM | FI_REMOTE_COMM)) !=
		(opx_ep->rx->caps & (FI_LOCAL_COMM | FI_REMOTE_COMM))) {
		/* rx/tx cpability mismatch */
		opx_ep->ep_fid.msg = &fi_opx_no_msg_ops;
		return 0;
	}

	const enum fi_threading threading = opx_ep->domain->threading;
	if (OFI_UNLIKELY(fi_opx_threading_unknown(threading))) {
		return -FI_EINVAL;
	}

	uint64_t comm_caps = opx_ep->rx->caps & (FI_LOCAL_COMM | FI_REMOTE_COMM);
	if (comm_caps == 0)
		comm_caps = FI_LOCAL_COMM | FI_REMOTE_COMM;

	const int lock_required = fi_opx_threading_lock_required(threading, fi_opx_global.progress);
	const enum ofi_reliability_kind reliability = opx_ep->reliability->state.kind;

	if (!lock_required) {
		if (opx_ep->av->type == FI_AV_TABLE) {
			if (comm_caps == FI_LOCAL_COMM) {

				if (reliability == OFI_RELIABILITY_KIND_NONE)
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0008000000000000ull, OFI_RELIABILITY_KIND_NONE);
				else if (reliability == OFI_RELIABILITY_KIND_ONLOAD)
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0008000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
				else
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0008000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);

			} else if (comm_caps == FI_REMOTE_COMM) {

				if (reliability == OFI_RELIABILITY_KIND_NONE)
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0010000000000000ull, OFI_RELIABILITY_KIND_NONE);
				else if (reliability == OFI_RELIABILITY_KIND_ONLOAD)
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0010000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
				else
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0010000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);

			} else {	/* comm_caps == (FI_LOCAL_COMM | FI_REMOTE_COMM) */

				if (reliability == OFI_RELIABILITY_KIND_NONE)
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0018000000000000ull, OFI_RELIABILITY_KIND_NONE);
				else if (reliability == OFI_RELIABILITY_KIND_ONLOAD)
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0018000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
				else
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_TABLE, 0x0018000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
			}

		} else  if (opx_ep->av->type == FI_AV_MAP) {

			if (comm_caps == FI_LOCAL_COMM) {

				if (reliability == OFI_RELIABILITY_KIND_NONE)
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0008000000000000ull, OFI_RELIABILITY_KIND_NONE);
				else if (reliability == OFI_RELIABILITY_KIND_ONLOAD)
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0008000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
				else
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0008000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);

			} else if (comm_caps == FI_REMOTE_COMM) {

				if (reliability == OFI_RELIABILITY_KIND_NONE)
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0010000000000000ull, OFI_RELIABILITY_KIND_NONE);
				else if (reliability == OFI_RELIABILITY_KIND_ONLOAD)
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0010000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
				else
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0010000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);

			} else {	/* comm_caps == (FI_LOCAL_COMM | FI_REMOTE_COMM) */

				if (reliability == OFI_RELIABILITY_KIND_NONE)
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0018000000000000ull, OFI_RELIABILITY_KIND_NONE);
				else if (reliability == OFI_RELIABILITY_KIND_ONLOAD)
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0018000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
				else
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_NOT_REQUIRED,FI_AV_MAP, 0x0018000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);

			}
		}  else {
			/* FI_AV_UNSPEC is not a runtime value in the address vector so FI_OPX_MSG_OPS_STRUCT_NAME is not used here.  It is used in FABRIC_DIRECT */
			assert((opx_ep->av->type==FI_AV_TABLE)||(opx_ep->av->type==FI_AV_MAP));
		}
	} else {
		if (opx_ep->av->type == FI_AV_TABLE) {

			if (comm_caps == FI_LOCAL_COMM) {

				if (reliability == OFI_RELIABILITY_KIND_NONE)
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0008000000000000ull, OFI_RELIABILITY_KIND_NONE);
				else if (reliability == OFI_RELIABILITY_KIND_ONLOAD)
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0008000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
				else
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0008000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);

			} else if (comm_caps == FI_REMOTE_COMM) {

				if (reliability == OFI_RELIABILITY_KIND_NONE)
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0010000000000000ull, OFI_RELIABILITY_KIND_NONE);
				else if (reliability == OFI_RELIABILITY_KIND_ONLOAD)
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0010000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
				else
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0010000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);

			} else {	/* comm_caps == (FI_LOCAL_COMM | FI_REMOTE_COMM) */

				if (reliability == OFI_RELIABILITY_KIND_NONE)
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0018000000000000ull, OFI_RELIABILITY_KIND_NONE);
				else if (reliability == OFI_RELIABILITY_KIND_ONLOAD)
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0018000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
				else
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_REQUIRED,FI_AV_TABLE, 0x0018000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);
			}
		} else if (opx_ep->av->type == FI_AV_MAP) {

			if (comm_caps == FI_LOCAL_COMM) {

				if (reliability == OFI_RELIABILITY_KIND_NONE)
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0008000000000000ull, OFI_RELIABILITY_KIND_NONE);
				else if (reliability == OFI_RELIABILITY_KIND_ONLOAD)
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0008000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
				else
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0008000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);



			} else if (comm_caps == FI_REMOTE_COMM) {

				if (reliability == OFI_RELIABILITY_KIND_NONE)
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0010000000000000ull, OFI_RELIABILITY_KIND_NONE);
				else if (reliability == OFI_RELIABILITY_KIND_ONLOAD)
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0010000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
				else
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0010000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);

			} else {	/* comm_caps == (FI_LOCAL_COMM | FI_REMOTE_COMM) */

				if (reliability == OFI_RELIABILITY_KIND_NONE)
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0018000000000000ull, OFI_RELIABILITY_KIND_NONE);
				else if (reliability == OFI_RELIABILITY_KIND_ONLOAD)
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0018000000000000ull, OFI_RELIABILITY_KIND_ONLOAD);
				else
					opx_ep->ep_fid.msg = &FI_OPX_MSG_OPS_STRUCT_NAME(FI_OPX_LOCK_REQUIRED,FI_AV_MAP, 0x0018000000000000ull, OFI_RELIABILITY_KIND_OFFLOAD);

			}
		}  else {
			/* FI_AV_UNSPEC is not a runtime value in the address vector so FI_OPX_MSG_OPS_STRUCT_NAME is not used here.  It is used in FABRIC_DIRECT */
			assert((opx_ep->av->type==FI_AV_TABLE)||(opx_ep->av->type==FI_AV_MAP));
		}
	}

	return 0;
}

int fi_opx_finalize_msg_ops(struct fid_ep *ep)
{
	return 0;
}
