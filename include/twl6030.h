#include <common.h>
#include <i2c.h>

#define TWL6030_CHIP_PM		0x48

#define TWL6030_CHIP_USB	0x49
#define TWL6030_CHIP_ADC	0x49
#define TWL6030_CHIP_CHARGER	0x49
#define TWL6030_CHIP_PWM	0x49

#define CONTROLLER_INT_MASK	0xE0
#define CONTROLLER_CTRL1	0xE1
#define CONTROLLER_WDG		0xE2
#define CONTROLLER_STAT1	0xE3
#define CHARGERUSB_INT_STATUS	0xE4
#define CHARGERUSB_INT_MASK	0xE5
#define CHARGERUSB_STATUS_INT1	0xE6
#define CHARGERUSB_STATUS_INT2	0xE7
#define CHARGERUSB_CTRL1	0xE8
#define CHARGERUSB_CTRL2	0xE9
#define CHARGERUSB_CTRL3	0xEA
#define CHARGERUSB_STAT1	0xEB
#define CHARGERUSB_VOREG	0xEC
#define CHARGERUSB_VICHRG	0xED
#define CHARGERUSB_CINLIMIT	0xEE
#define CHARGERUSB_CTRLLIMIT1	0xEF

#define VAUX2_CFG_GRP		0x88
#define VAUX2_CFG_TRANS		0x89
#define VAUX2_CFG_STATE		0x8A
#define VAUX2_CFG_VOLTAGE	0x8B

#define VAUX3_CFG_GRP		0x8C
#define VAUX3_CFG_TRANS		0x8D
#define VAUX3_CFG_STATE		0x8E
#define VAUX3_CFG_VOLTAGE	0x8F

#define VUSIM_CFG_GRP		0xA4
#define VUSIM_CFG_TRANS		0xA5
#define VUSIM_CFG_STATE		0xA6
#define VUSIM_CFG_VOLTAGE	0xA7

#define REGEN1_CFG_GRP		0xAD
#define REGEN1_CFG_TRANS	0xAE
#define REGEN1_CFG_STATE	0xAF

#define CHARGERUSB_VICHRG_500		0x4
#define CHARGERUSB_VICHRG_1500		0xE

#define CHARGERUSB_CIN_LIMIT_100	0x1
#define CHARGERUSB_CIN_LIMIT_300	0x5
#define CHARGERUSB_CIN_LIMIT_500	0x9
#define CHARGERUSB_CIN_LIMIT_NONE	0xF

#define MVAC_FAULT		(1 << 6)
#define MAC_EOC			(1 << 5)
#define MBAT_REMOVED		(1 << 4)
#define MFAULT_WDG		(1 << 3)
#define MBAT_TEMP		(1 << 2)
#define MVBUS_DET		(1 << 1)
#define MVAC_DET		(1 << 0)

#define MASK_MCURRENT_TERM		(1 << 3)
#define MASK_MCHARGERUSB_STAT		(1 << 2)
#define MASK_MCHARGERUSB_THMREG		(1 << 1)
#define MASK_MCHARGERUSB_FAULT		(1 << 0)

#define CHARGERUSB_VOREG_3P52		0x01
#define CHARGERUSB_VOREG_4P0		0x19
#define CHARGERUSB_VOREG_4P2		0x23
#define CHARGERUSB_VOREG_4P76		0x3F

#define CHARGERUSB_CTRL2_VITERM_50	(0 << 5)
#define CHARGERUSB_CTRL2_VITERM_100	(1 << 5)
#define CHARGERUSB_CTRL2_VITERM_150	(2 << 5)

#define CONTROLLER_CTRL1_EN_CHARGER	(1 << 4)
#define CONTROLLER_CTRL1_SEL_CHARGER	(1 << 3)

#define VUSB_CFG_STATE		0xA2
#define MISC2			0xE5

#define MISC1			0xE4
#define VBAT_MEAS		(1 << 1)

#define CTRL_P1 0x33
	#define SP1		(1 << 3)
	#define EOCP1	(1 << 1)
	#define BUSY	(1 << 0)

#define GPADC_CTRL 0x2E
	#define GPADC_ISOURCE_EN	(1 << 7)
	#define GPADC_CTRL_SCALER_EN    (1 << 2)	
	#define GPADC_CTRL_SCALER_DIV4  (1 << 3)
	
#define ADC_LSB_MASK 0xC0

#define GPCH0 0x57 

#define TOGGLE1 0x90
	#define GPADC_START_POLARITY (1 << 3)
	#define GPADCS (1 << 1)
	#define GPADCR (1 << 0)

#define mdelay(n) ({ unsigned long msec = (n); while (msec--) udelay(1000); })

#define PHOENIX_DEV_ON				0x25

#define SW_RESET					(1 << 6)
#define MOD_DEVON					(1 << 5)
#define CON_DEVON					(1 << 4)
#define APP_DEVON					(1 << 3)
#define MOD_DEVOFF					(1 << 2)
#define CON_DEVOFF					(1 << 1)
#define APP_DEVOFF					(1 << 0)

#define PHOENIX_START_CONDITION		0x1F

#define RESTART_BB					(1 << 6)
#define FIST_BAT_INS				(1 << 5)
#define STRT_ON_RTC					(1 << 4)
#define STRT_ON_PLUG_DET			(1 << 3)
#define STRT_ON_USB_ID				(1 << 2)
#define STRT_ON_RPWRON				(1 << 1)
#define STRT_ON_PWRON				(1 << 0)

#define STS_HW_CONDITIONS			0x21

#define STS_PREQ3					(1 << 7)
#define STS_PREQ2					(1 << 6)
#define STS_PREQ1					(1 << 5)
#define STS_VBAT_MIN_HI				(1 << 4)
#define STS_PLUG_DET				(1 << 3)
#define STS_USB_ID					(1 << 2)
#define STS_RPWRON					(1 << 1)
#define STS_PWRON					(1 << 0)

#define PHOENIX_LAST_TURNOFF_STS			0x22
#define DEVOFF_SHORT					(1 << 5)
#define DEVOFF_WDT					(1 << 4)
#define DEVOFF_TSHUT					(1 << 3)
#define DEVOFF_BCK					(1 << 2)
#define DEVOFF_LPK					(1 << 1)
#define OSC_RC32K					(1 << 0)

#define PH_CFG_VBATLOW					0x28

#define BB_SEL						(1 << 7)
#define BB_MSK						(1 << 6)
#define VBAT_DELAY_MASK					(0x3F << 0)

int twl6030_init_battery_charging(const int charger_type);
void twl6030_usb_device_settings(void);
void twl6030_set_battery_bounce_setting(void);
