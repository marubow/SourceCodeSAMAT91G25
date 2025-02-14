/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2012, Atmel Corporation

 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "common.h"
#include "hardware.h"
#include "pmc.h"
#include "dbgu.h"
#include "debug.h"
#include "ddramc.h"
#include "spi.h"
#include "gpio.h"
#include "slowclk.h"
#include "timer.h"
#include "watchdog.h"
#include "string.h"
#include "onewire_info.h"

#include "arch/at91_pmc.h"
#include "arch/at91_rstc.h"
#include "arch/at91sama5_smc.h"
#include "arch/at91_pio.h"
#include "arch/at91_ddrsdrc.h"
#include "at91sama5d3xek.h"

#ifdef CONFIG_USER_HW_INIT
extern void hw_init_hook(void);
#endif

static void at91_dbgu_hw_init(void)
{
	/* Configure DBGU pin */
	const struct pio_desc dbgu_pins[] = {
		{"RXD", AT91C_PIN_PB(30), 0, PIO_DEFAULT, PIO_PERIPH_A},
		{"TXD", AT91C_PIN_PB(31), 0, PIO_DEFAULT, PIO_PERIPH_A},
		{(char *)0, 0, 0, PIO_DEFAULT, PIO_PERIPH_A},
	};

	/*  Configure the dbgu pins */
	pio_configure(dbgu_pins);
	writel((1 << AT91C_ID_PIOB), (PMC_PCER + AT91C_BASE_PMC));

	/* Enable clock */
	writel(1 << AT91C_ID_DBGU, (PMC_PCER + AT91C_BASE_PMC));
}

static void initialize_dbgu(void)
{
	at91_dbgu_hw_init();
	dbgu_init(BAUDRATE(MASTER_CLOCK, 115200));
}

#ifdef CONFIG_DDR2
static void ddramc_reg_config(struct ddramc_register *ddramc_config)
{
	ddramc_config->mdr = (AT91C_DDRC2_DBW_32_BITS
				| AT91C_DDRC2_MD_DDR2_SDRAM);

	ddramc_config->cr = (AT91C_DDRC2_NC_DDR10_SDR9
				| AT91C_DDRC2_NR_14
				| AT91C_DDRC2_CAS_3
				| AT91C_DDRC2_DLL_RESET_DISABLED /* DLL not reset */
				| AT91C_DDRC2_DIS_DLL_DISABLED   /* DLL not disabled */
				| AT91C_DDRC2_ENRDM_ENABLE       /* Phase error correction is enabled */
				| AT91C_DDRC2_NB_BANKS_8
				| AT91C_DDRC2_NDQS_DISABLED      /* NDQS disabled (check on schematics) */
				| AT91C_DDRC2_DECOD_INTERLEAVED  /* Interleaved decoding */
				| AT91C_DDRC2_UNAL_SUPPORTED);   /* Unaligned access is supported */

	/*
	 * The DDR2-SDRAM device requires a refresh every 15.625 us or 7.81 us.
	 * With a 133 MHz frequency, the refresh timer count register must to be
	 * set with (15.625 x 133 MHz) ~ 2084 i.e. 0x824
	 * or (7.81 x 133 MHz) ~ 1040 i.e. 0x410.
	 */
	ddramc_config->rtr = 0x411;     /* Refresh timer: 7.8125us */

	/* One clock cycle @ 133 MHz = 7.5 ns */
	ddramc_config->t0pr = (AT91C_DDRC2_TRAS_6       /* 6 * 7.5 = 45 ns */
			| AT91C_DDRC2_TRCD_2            /* 2 * 7.5 = 22.5 ns */
			| AT91C_DDRC2_TWR_2             /* 2 * 7.5 = 15   ns */
			| AT91C_DDRC2_TRC_8             /* 8 * 7.5 = 75   ns */
			| AT91C_DDRC2_TRP_2             /* 2 * 7.5 = 15   ns */
			| AT91C_DDRC2_TRRD_2            /* 2 * 7.5 = 15   ns */
			| AT91C_DDRC2_TWTR_2            /* 2 clock cycles min */
			| AT91C_DDRC2_TMRD_2);          /* 2 clock cycles */

	ddramc_config->t1pr = (AT91C_DDRC2_TXP_2        /*  2 clock cycles */
			| AT91C_DDRC2_TXSRD_200         /* 200 clock cycles */
			| AT91C_DDRC2_TXSNR_28          /* 195 + 10 = 205ns ==> 28 * 7.5 = 210 ns*/
			| AT91C_DDRC2_TRFC_26);         /* 26 * 7.5 = 195 ns */

	ddramc_config->t2pr = (AT91C_DDRC2_TFAW_7       /* 7 * 7.5 = 52.5 ns */
			| AT91C_DDRC2_TRTP_2            /* 2 clock cycles min */
			| AT91C_DDRC2_TRPA_2            /* 2 * 7.5 = 15 ns */
			| AT91C_DDRC2_TXARDS_7          /* 7 clock cycles */
			| AT91C_DDRC2_TXARD_8);         /* MR12 = 1 : slow exit power down */
}

static void ddramc_init(void)
{
	struct ddramc_register ddramc_reg;
	unsigned int reg;

	ddramc_reg_config(&ddramc_reg);

	/* enable ddr2 clock */
	writel(1 << (AT91C_ID_MPDDRC - 32),  (PMC_PCER1 + AT91C_BASE_PMC));
	writel(AT91C_PMC_DDR, (PMC_SCER + AT91C_BASE_PMC));

	/* Init the special register for sama5d3x */
	/* MPDDRC DLL Slave Offset Register: DDR2 configuration */
	reg = AT91C_MPDDRC_S0OFF_1
		| AT91C_MPDDRC_S2OFF_1
		| AT91C_MPDDRC_S3OFF_1;
	writel(reg, (AT91C_BASE_MPDDRC + MPDDRC_DLL_SOR));

	/* MPDDRC DLL Master Offset Register */
	/* write master + clk90 offset */
	reg = AT91C_MPDDRC_MOFF_7
		| AT91C_MPDDRC_CLK90OFF_31
		| AT91C_MPDDRC_SELOFF_ENABLED | AT91C_MPDDRC_KEY;
	writel(reg, (AT91C_BASE_MPDDRC + MPDDRC_DLL_MOR));

	/* MPDDRC I/O Calibration Register */
	/* DDR2 RZQ = 50 Ohm */
	/* TZQIO = 4 */
	reg = AT91C_MPDDRC_RDIV_DDR2_RZQ_50
		| AT91C_MPDDRC_TZQIO_4;
	writel(reg, (AT91C_BASE_MPDDRC + MPDDRC_IO_CALIBR));

	/* DDRAM2 Controller initialize */
	ddram_initialize(AT91C_BASE_MPDDRC, AT91C_BASE_DDRCS, &ddramc_reg);
}
#endif /* #ifdef CONFIG_DDR2 */

static void one_wire_hw_init(void)
{
	const struct pio_desc one_wire_pio[] = {
		{"1-Wire", AT91C_PIN_PE(25), 1, PIO_DEFAULT, PIO_OUTPUT},
		{(char *)0, 0, 0, PIO_DEFAULT, PIO_PERIPH_A},
	};

	writel((1 << AT91C_ID_PIOE), (PMC_PCER + AT91C_BASE_PMC));
	pio_configure(one_wire_pio);
}

#ifdef CONFIG_BUZZ
static void buzz_hw_init(void)
{
	const struct pio_desc buzz_pio[] = {
		{"BUZZ", CONFIG_SYS_BUZZ_PIN, 1, PIO_PULLUP, PIO_OUTPUT},
		{(char *)0, 0, 0, PIO_DEFAULT, PIO_PERIPH_A},
	};

	writel((1 << AT91C_ID_PIOE), (PMC_PCER + AT91C_BASE_PMC));
	pio_configure(buzz_pio);
}

/* 0: on     1: off*/
void buzz_set_state(int state)
{
    pio_set_gpio_output(CONFIG_SYS_BUZZ_PIN, state);
}
#endif


#if defined(CONFIG_NANDFLASH_RECOVERY) || defined(CONFIG_DATAFLASH_RECOVERY)
static void recovery_buttons_hw_init(void)
{
	/* Configure recovery button PINs */
	const struct pio_desc recovery_button_pins[] = {
		{"RECOVERY_BUTTON", CONFIG_SYS_RECOVERY_BUTTON_PIN, 0, PIO_PULLUP, PIO_INPUT},
		{(char *)0, 0, 0, PIO_DEFAULT, PIO_PERIPH_A},
	};

	writel((1 << AT91C_ID_PIOE), PMC_PCER + AT91C_BASE_PMC);
	pio_configure(recovery_button_pins);
}
#endif /* #if defined(CONFIG_NANDFLASH_RECOVERY) || defined(CONFIG_DATAFLASH_RECOVERY) */

static void HDMI_Qt1070_workaround(void)
{
	/* For the HDMI and QT1070 shar the irq line
	 * if the HDMI does not initialize, the irq line is pulled down by HDMI,
	 * so, the irq line can not used by QT1070
	 */
	pio_set_gpio_output(AT91C_PIN_PC(31), 1);
	udelay(33000);
	pio_set_gpio_output(AT91C_PIN_PC(31), 0);
	udelay(33000);
	pio_set_gpio_output(AT91C_PIN_PC(31), 1);
}

static void lcd_disable(void)
{
    /* LCDDISP: PA25 */
    pio_set_gpio_output(AT91C_PIN_PA(25), 1);
}

#ifdef CONFIG_HW_INIT
void hw_init(void)
{
    /* Disable lcd */
    lcd_disable();

	/* Disable watchdog */
	at91_disable_wdt();

	/* At this stage the main oscillator is supposed to be enabled PCK = MCK = MOSC */
	writel(0x00, AT91C_BASE_PMC + PMC_PLLICPR);

	/* Configure PLLA = MOSC * (PLL_MULA + 1) / PLL_DIVA */
	pmc_cfg_plla(PLLA_SETTINGS, PLL_LOCK_TIMEOUT);

	/* PCK = PLLA/2 = 3 * MCK */
	pmc_cfg_mck(BOARD_PRESCALER_MAIN_CLOCK, PLL_LOCK_TIMEOUT);

	/* Switch MCK on PLLA output */
	pmc_cfg_mck(BOARD_PRESCALER_PLLA, PLL_LOCK_TIMEOUT);

	/* Enable External Reset */
	writel(AT91C_RSTC_KEY_UNLOCK | AT91C_RSTC_URSTEN, AT91C_BASE_RSTC + RSTC_RMR);

	/* Init timer */
	timer_init();

#ifdef CONFIG_SCLK
	slowclk_enable_osc32();
#endif

	/* initialize the dbgu */
	initialize_dbgu();

#ifdef CONFIG_DDR2
	/* Initialize MPDDR Controller */
	ddramc_init();
#endif
	/* load one wire information */
	one_wire_hw_init();

#ifdef CONFIG_BUZZ
	/* Initialize the buzz */
	buzz_hw_init();
#endif

#ifdef CONFIG_USER_HW_INIT
	hw_init_hook();
#endif
	HDMI_Qt1070_workaround();

#if defined(CONFIG_NANDFLASH_RECOVERY) || defined(CONFIG_DATAFLASH_RECOVERY)
	/* Init the recovery buttons pins */
	recovery_buttons_hw_init();
#endif
}
#endif /* #ifdef CONFIG_HW_INIT */

#ifdef CONFIG_DATAFLASH
void at91_spi0_hw_init(void)
{
	/* Configure PIN for SPI0 */
	const struct pio_desc spi0_pins[] = {
		{"MISO",	AT91C_PIN_PD(10), 0, PIO_DEFAULT, PIO_PERIPH_A},
		{"MOSI",	AT91C_PIN_PD(11), 0, PIO_DEFAULT, PIO_PERIPH_A},
		{"SPCK",	AT91C_PIN_PD(12), 0, PIO_DEFAULT, PIO_PERIPH_A},
		{"NPCS",	CONFIG_SYS_SPI_PCS, 1, PIO_DEFAULT, PIO_OUTPUT},
		{(char *)0, 0, 0, PIO_DEFAULT, PIO_PERIPH_A},
	};

	/* Configure the PIO controller */
	writel((1 << AT91C_ID_PIOD), (PMC_PCER + AT91C_BASE_PMC));
	pio_configure(spi0_pins);

	/* Enable the clock */
	writel((1 << AT91C_ID_SPI0), (PMC_PCER + AT91C_BASE_PMC));
}
#endif /* #ifdef CONFIG_DATAFLASH */

#ifdef CONFIG_SDCARD
static void sdcard_set_of_name_board(char *of_name)
{
	/* CPU TYPE*/
	switch (get_cm_sn()) {
	case BOARD_ID_SAMA5D31_CM:
		strcpy(of_name, "sama5d31ek");
		break;

	case BOARD_ID_SAMA5D33_CM:
		strcpy(of_name, "sama5d33ek");
		break;

	case BOARD_ID_SAMA5D34_CM:
		strcpy(of_name, "sama5d34ek");
		break;

	case BOARD_ID_SAMA5D35_CM:
		strcpy(of_name, "sama5d35ek");
		break;

	default:
		dbg_log(1, "WARNING: Not correct CPU board ID\n\r");
		break;
	}

	if (get_dm_sn() == BOARD_ID_PDA_DM)
		strcat(of_name, "_pda");

	strcat(of_name, ".dtb");
}

void at91_mci0_hw_init(void)
{
	const struct pio_desc mci_pins[] = {
		{"MCCK", AT91C_PIN_PD(9), 0, PIO_DEFAULT, PIO_PERIPH_A},
		{"MCCDA", AT91C_PIN_PD(0), 0, PIO_DEFAULT, PIO_PERIPH_A},

		{"MCDA0", AT91C_PIN_PD(1), 0, PIO_DEFAULT, PIO_PERIPH_A},
		{"MCDA1", AT91C_PIN_PD(2), 0, PIO_DEFAULT, PIO_PERIPH_A},
		{"MCDA2", AT91C_PIN_PD(3), 0, PIO_DEFAULT, PIO_PERIPH_A},
		{"MCDA3", AT91C_PIN_PD(4), 0, PIO_DEFAULT, PIO_PERIPH_A},
		{"MCDA4", AT91C_PIN_PD(5), 0, PIO_DEFAULT, PIO_PERIPH_A},
		{"MCDA5", AT91C_PIN_PD(6), 0, PIO_DEFAULT, PIO_PERIPH_A},
		{"MCDA6", AT91C_PIN_PD(7), 0, PIO_DEFAULT, PIO_PERIPH_A},
		{"MCDA7", AT91C_PIN_PD(8), 0, PIO_DEFAULT, PIO_PERIPH_A},
		{(char *)0, 0, 0, PIO_DEFAULT, PIO_PERIPH_A},
	};

	/* Configure the PIO controller */
	writel((1 << AT91C_ID_PIOD), (PMC_PCER + AT91C_BASE_PMC));
	pio_configure(mci_pins);

	/* Enable the clock */
	writel((1 << AT91C_ID_HSMCI0), (PMC_PCER + AT91C_BASE_PMC));

	/* Set of name function pointer */
	sdcard_set_of_name = &sdcard_set_of_name_board;
}
#endif /* #ifdef CONFIG_SDCARD */

#ifdef CONFIG_NANDFLASH
void nandflash_hw_init(void)
{
	/* Configure nand pins */
	const struct pio_desc nand_pins[] = {
		{"NANDALE", AT91C_PIN_PE(21), 0, PIO_PULLUP, PIO_PERIPH_A},
		{"NANDCLE", AT91C_PIN_PE(22), 0, PIO_PULLUP, PIO_PERIPH_A},
		{(char *)0, 0, 0, PIO_DEFAULT, PIO_PERIPH_A},
	};

	/* Configure the nand controller pins*/
	pio_configure(nand_pins);
	writel((1 << AT91C_ID_PIOE), (PMC_PCER + AT91C_BASE_PMC));

	/* Enable the clock */
	writel(1 << AT91C_ID_SMC, (PMC_PCER + AT91C_BASE_PMC));

	/* Configure SMC CS3 for NAND/SmartMedia */
	writel(AT91C_SMC_SETUP_NWE(1)
		| AT91C_SMC_SETUP_NCS_WR(1) 
		| AT91C_SMC_SETUP_NRD(2) 
		| AT91C_SMC_SETUP_NCS_RD(1), 
		(ATMEL_BASE_SMC + SMC_SETUP3));

	writel(AT91C_SMC_PULSE_NWE(5)
		| AT91C_SMC_PULSE_NCS_WR(7)
		| AT91C_SMC_PULSE_NRD(5)
		| AT91C_SMC_PULSE_NCS_RD(7), 
	 	(ATMEL_BASE_SMC + SMC_PULSE3));

	writel(AT91C_SMC_CYCLE_NWE(8)
		| AT91C_SMC_CYCLE_NRD(9), 
		(ATMEL_BASE_SMC + SMC_CYCLE3));

	writel(AT91C_SMC_TIMINGS_TCLR(3)
		| AT91C_SMC_TIMINGS_TADL(10)
		| AT91C_SMC_TIMINGS_TAR(3)
		| AT91C_SMC_TIMINGS_TRR(4)
		| AT91C_SMC_TIMINGS_TWB(5)
		| AT91C_SMC_TIMINGS_RBNSEL(3)
		| AT91C_SMC_TIMINGS_NFSEL,
		(ATMEL_BASE_SMC + SMC_TIMINGS3));

	writel(AT91C_SMC_MODE_READMODE_NRD_CTRL
		| AT91C_SMC_MODE_WRITEMODE_NWE_CTRL
		| AT91C_SMC_MODE_EXNWMODE_DISABLED
		| AT91C_SMC_MODE_DBW_8
		| AT91C_SMC_MODE_TDF_CYCLES(1),
		(ATMEL_BASE_SMC + SMC_MODE3));
}

void nandflash_config_buswidth(unsigned char buswidth)
{
	unsigned int mode; 

	mode = readl(ATMEL_BASE_SMC + SMC_MODE3);

	mode &= ~AT91C_SMC_MODE_DBW;
	if (buswidth == 0)	/* 8 bits bus */
		mode |= AT91C_SMC_MODE_DBW_8;	
	else 
		mode |= AT91C_SMC_MODE_DBW_16;

	writel(mode, (ATMEL_BASE_SMC + SMC_MODE3));
}
#endif /* #ifdef CONFIG_NANDFLASH */

