/*
 * Copyright (C) 2019 Inria
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#pragma once

/**
 * @ingroup   boards_stm32f723e-disco
 * @{
 *
 * @file
 * @brief     Board specific configuration of direct mapped GPIOs
 *
 * @author    Alexandre Abadie <alexandre.abadie@inria.fr>
 */

#include "board.h"
#include "saul/periph.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief    GPIO pin configuration
 */
static const  saul_gpio_params_t saul_gpio_params[] =
{
#ifdef MODULE_PERIPH_INIT_LED0
    {
        .name = "LD1 (blue)",
        .pin = LED0_PIN,
        .mode = GPIO_OUT
    },
#endif
    {
        .name = "LD5 (red)",
        .pin = LED1_PIN,
        .mode = GPIO_OUT
    },
    {
        .name = "LD6 (green)",
        .pin = LED2_PIN,
        .mode = GPIO_OUT
    },
    {
        .name = "B1",
        .pin  = BTN0_PIN,
        .mode = BTN0_MODE
    },
};

#ifdef __cplusplus
}
#endif

/** @} */
