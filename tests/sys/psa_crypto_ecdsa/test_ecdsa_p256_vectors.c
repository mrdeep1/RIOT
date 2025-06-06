/*
 * Copyright (C) 2024 Lars Pfau <lars.pfau@haw-hamburg.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @ingroup     tests
 * @{
 *
 * @file
 * @brief       Tests PSA ECDSA with test vectors
 *
 * @author      Lars Pfau <lars.pfau@haw-hamburg.de>
 *
 * @}
 */
#include <stdio.h>
#include <stdint.h>

#include "compiler_hints.h"
#include "psa/crypto.h"

/*
 * RFC6979 test vector for P-256, SHA256 [1].
 *
 * [1] https://www.rfc-editor.org/rfc/rfc6979#appendix-A.2.5
 */
static const psa_algorithm_t algo = PSA_ALG_ECDSA(PSA_ALG_SHA_256);
static const psa_key_type_t type = PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1);

static const uint8_t private_key[] = {0xC9, 0xAF, 0xA9, 0xD8, 0x45, 0xBA, 0x75, 0x16, 0x6B, 0x5C,
    0x21, 0x57, 0x67, 0xB1, 0xD6, 0x93, 0x4E, 0x50, 0xC3, 0xDB, 0x36, 0xE8, 0x9B, 0x12, 0x7B, 0x8A,
    0x62, 0x2B, 0x12, 0x0F, 0x67, 0x21};

static const uint8_t public_key[] = {0x04, 0x60, 0xFE, 0xD4, 0xBA, 0x25, 0x5A, 0x9D, 0x31, 0xC9,
    0x61, 0xEB, 0x74, 0xC6, 0x35, 0x6D, 0x68, 0xC0, 0x49, 0xB8, 0x92, 0x3B, 0x61, 0xFA, 0x6C, 0xE6,
    0x69, 0x62, 0x2E, 0x60, 0xF2, 0x9F, 0xB6, 0x79, 0x03, 0xFE, 0x10, 0x08, 0xB8, 0xBC, 0x99, 0xA4,
    0x1A, 0xE9, 0xE9, 0x56, 0x28, 0xBC, 0x64, 0xF2, 0xF1, 0xB2, 0x0C, 0x2D, 0x7E, 0x9F, 0x51, 0x77,
    0xA3, 0xC2, 0x94, 0xD4, 0x46, 0x22, 0x99};

/* certain PSA backends require the data to be in RAM rather than ROM
 * so these values cannot be `const` */
NONSTRING
static uint8_t message[6] = "sample";
static uint8_t signature[] = {0xEF, 0xD4, 0x8B, 0x2A, 0xAC, 0xB6, 0xA8, 0xFD, 0x11, 0x40,
    0xDD, 0x9C, 0xD4, 0x5E, 0x81, 0xD6, 0x9D, 0x2C, 0x87, 0x7B, 0x56, 0xAA, 0xF9, 0x91, 0xC3, 0x4D,
    0x0E, 0xA8, 0x4E, 0xAF, 0x37, 0x16, 0xF7, 0xCB, 0x1C, 0x94, 0x2D, 0x65, 0x7C, 0x41, 0xD4, 0x36,
    0xC7, 0xA1, 0xB6, 0xE2, 0x9F, 0x65, 0xF3, 0xE9, 0x00, 0xDB, 0xB9, 0xAF, 0xF4, 0x06, 0x4D, 0xC4,
    0xAB, 0x2F, 0x84, 0x3A, 0xCD, 0xA8};

/**
 * @brief Verify a sample NIST P-256 signature from a message using SHA256
 *
 * @return psa_status_t
 */
psa_status_t test_ecdsa_p256_vectors(void) {
    psa_key_attributes_t key_attr = psa_key_attributes_init();
    psa_key_id_t key_id;

    psa_set_key_usage_flags(&key_attr, PSA_KEY_USAGE_VERIFY_MESSAGE | PSA_KEY_USAGE_EXPORT);
    psa_set_key_algorithm(&key_attr, algo);
    psa_set_key_bits(&key_attr, PSA_BYTES_TO_BITS(sizeof(private_key)));
    psa_set_key_type(&key_attr, type);

    psa_status_t status;
    status = psa_import_key(&key_attr, private_key, sizeof(private_key), &key_id);
    if (status != PSA_SUCCESS) {
        return status;
    }

    /* testing public key derivation */
    uint8_t exp_public_key[sizeof(public_key)];
    size_t length;
    status = psa_export_public_key(key_id, exp_public_key, sizeof(public_key), &length);
    if (status != PSA_SUCCESS) {
        return status;
    }

    if (0 != memcmp(public_key, exp_public_key, sizeof(public_key))) {
        return -1;
    }

    status = psa_verify_message(key_id, algo, message, sizeof(message), signature, sizeof(signature));

    psa_destroy_key(key_id);

    return status;
}
