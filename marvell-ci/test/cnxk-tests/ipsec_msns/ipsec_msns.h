/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(C) 2022 Marvell.
 */

#ifndef _IPSEC_MSNS_H_
#define _IPSEC_MSNS_H_

#define MAX_PKT_LEN  1500

struct ipsec_session_data {
	uint32_t spi;
	struct {
		uint8_t data[32];
	} key;
	struct rte_security_ipsec_xform ipsec_xform;
	bool aead;
	union {
		struct {
			struct rte_crypto_sym_xform cipher;
			struct rte_crypto_sym_xform auth;
		} chain;
		struct rte_crypto_sym_xform aead;
	} xform;
};

struct ipsec_test_packet {
	uint32_t len;
	uint8_t data[MAX_PKT_LEN];
};

struct test_ipsec_vector {
	struct ipsec_session_data *sa_data;
	struct ipsec_test_packet *full_pkt;
	struct ipsec_test_packet *frags;
};

struct ipsec_test_packet pkt_ipv4_plain = {
	.len = 76,
	.data = {
		/* ETH */
		0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1,
		0xf2, 0xf2, 0xf2, 0xf2, 0xf2, 0xf2, 0x08, 0x00,

		/* IP */
		0x45, 0x00, 0x00, 0x3e, 0x69, 0x8f, 0x00, 0x00,
		0x80, 0x11, 0x4d, 0xcc, 0xc0, 0xa8, 0x01, 0x02,
		0xc0, 0xa8, 0x01, 0x01,

		/* UDP */
		0x0a, 0x98, 0x00, 0x35, 0x00, 0x2a, 0x23, 0x43,
		0xb2, 0xd0, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x03, 0x73, 0x69, 0x70,
		0x09, 0x63, 0x79, 0x62, 0x65, 0x72, 0x63, 0x69,
		0x74, 0x79, 0x02, 0x64, 0x6b, 0x00, 0x00, 0x01,
		0x00, 0x01,
	},
};

struct ipsec_test_packet pkt_ipv4_gcm128_cipher = {
	.len = 130,
	.data = {
		/* ETH */
		0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1,
		0xf2, 0xf2, 0xf2, 0xf2, 0xf2, 0xf2, 0x08, 0x00,

		/* IP - outer header */
		0x45, 0x00, 0x00, 0x74, 0x00, 0x01, 0x00, 0x00,
		0x40, 0x32, 0xf7, 0x03, 0xc0, 0xa8, 0x01, 0x02,
		0xc0, 0xa8, 0x01, 0x01,

		/* ESP */
		0x00, 0x00, 0xa5, 0xf8, 0x00, 0x00, 0x00, 0x01,

		/* IV */
		0xfa, 0xce, 0xdb, 0xad, 0xde, 0xca, 0xf8, 0x88,

		/* Data */
		0xde, 0xb2, 0x2c, 0xd9, 0xb0, 0x7c, 0x72, 0xc1,
		0x6e, 0x3a, 0x65, 0xbe, 0xeb, 0x8d, 0xf3, 0x04,
		0xa5, 0xa5, 0x89, 0x7d, 0x33, 0xae, 0x53, 0x0f,
		0x1b, 0xa7, 0x6d, 0x5d, 0x11, 0x4d, 0x2a, 0x5c,
		0x3d, 0xe8, 0x18, 0x27, 0xc1, 0x0e, 0x9a, 0x4f,
		0x51, 0x33, 0x0d, 0x0e, 0xec, 0x41, 0x66, 0x42,
		0xcf, 0xbb, 0x85, 0xa5, 0xb4, 0x7e, 0x48, 0xa4,
		0xec, 0x3b, 0x9b, 0xa9, 0x5d, 0x91, 0x8b, 0xd4,
		0x29, 0xc7, 0x37, 0x57, 0x9f, 0xf1, 0x9e, 0x58,
		0xcf, 0xfc, 0x60, 0x7a, 0x3b, 0xce, 0x89, 0x94,
	},
};

struct ipsec_session_data conf_aes_128_gcm = {
	.key = {
		.data = {
			0xfe, 0xff, 0xe9, 0x92, 0x86, 0x65, 0x73, 0x1c,
			0x6d, 0x6a, 0x8f, 0x94, 0x67, 0x30, 0x83,
			0x08
		},
	},
	.ipsec_xform = {
		.spi = 0xa5f8,
		.salt = 0xbebafeca,
		.options.esn = 0,
		.options.udp_encap = 0,
		.options.copy_dscp = 0,
		.options.copy_flabel = 0,
		.options.copy_df = 0,
		.options.dec_ttl = 0,
		.options.ecn = 0,
		.options.stats = 0,
		.options.tunnel_hdr_verify = 0,
		.options.ip_csum_enable = 0,
		.options.l4_csum_enable = 0,
		.direction = RTE_SECURITY_IPSEC_SA_DIR_EGRESS,
		.proto = RTE_SECURITY_IPSEC_SA_PROTO_ESP,
		.mode = RTE_SECURITY_IPSEC_SA_MODE_TUNNEL,
		.tunnel.type = RTE_SECURITY_IPSEC_TUNNEL_IPV4,
		.replay_win_sz = 0,
	},

	.aead = true,

	.xform = {
		.aead = {
			.next = NULL,
			.type = RTE_CRYPTO_SYM_XFORM_AEAD,
			.aead = {
				.op = RTE_CRYPTO_AEAD_OP_ENCRYPT,
				.algo = RTE_CRYPTO_AEAD_AES_GCM,
				.key.length = 16,
				.iv.length = 12,
				.iv.offset = 0,
				.digest_length = 16,
				.aad_length = 12,
			},
		},
	},
};

struct ipsec_session_data conf_aes_256_gcm = {
	.key = {
		.data = {
			0xfe, 0xff, 0xe9, 0x92, 0x86, 0x65, 0x73, 0x1c,
			0x6d, 0x6a, 0x8f, 0x94, 0x67, 0x30, 0x83,
			0x08,
			0xfe, 0xff, 0xe9, 0x92, 0x86, 0x65, 0x73, 0x1c,
			0x6d, 0x6a, 0x8f, 0x94, 0x67, 0x30, 0x83,
			0x08,
		},
	},
	.ipsec_xform = {
		.spi = 0xa5f8,
		.salt = 0xbebafeca,
		.options.esn = 0,
		.options.udp_encap = 0,
		.options.copy_dscp = 0,
		.options.copy_flabel = 0,
		.options.copy_df = 0,
		.options.dec_ttl = 0,
		.options.ecn = 0,
		.options.stats = 0,
		.options.tunnel_hdr_verify = 0,
		.options.ip_csum_enable = 0,
		.options.l4_csum_enable = 0,
		.direction = RTE_SECURITY_IPSEC_SA_DIR_EGRESS,
		.proto = RTE_SECURITY_IPSEC_SA_PROTO_ESP,
		.mode = RTE_SECURITY_IPSEC_SA_MODE_TUNNEL,
		.tunnel.type = RTE_SECURITY_IPSEC_TUNNEL_IPV4,
		.replay_win_sz = 0,
	},

	.aead = true,

	.xform = {
		.aead = {
			.next = NULL,
			.type = RTE_CRYPTO_SYM_XFORM_AEAD,
			.aead = {
				.op = RTE_CRYPTO_AEAD_OP_ENCRYPT,
				.algo = RTE_CRYPTO_AEAD_AES_GCM,
				.key.length = 32,
				.iv.length = 12,
				.iv.offset = 0,
				.digest_length = 16,
				.aad_length = 12,
			},
		},
	},
};

enum pkt_type {
	PKT_TYPE_PLAIN_IPV4 = 1,
	PKT_TYPE_IPSEC_IPV4,
	PKT_TYPE_PLAIN_IPV6,
	PKT_TYPE_IPSEC_IPV6,
	PKT_TYPE_INVALID
};

#endif
