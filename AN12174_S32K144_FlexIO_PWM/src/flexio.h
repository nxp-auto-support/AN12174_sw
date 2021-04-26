
#ifndef FLEXIO_H_
#define FLEXIO_H_

#include "S32K144.h"
#define FlexioPin0 0

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Shift clock polarity options   */
typedef enum
{
    FLEXIO_TIMER_POLARITY_POSEDGE = 0x00U,     /*!< Shift on positive edge of Shift clock */
    FLEXIO_TIMER_POLARITY_NEGEDGE = 0x01U,     /*!< Shift on negative edge of Shift clock */
} flexio_timer_polarity_t;

/* Pin polarity options   */
typedef enum
{
    FLEXIO_PIN_POLARITY_HIGH = 0x00U,          /*!< Pin is active high */
    FLEXIO_PIN_POLARITY_LOW  = 0x01U,          /*!< Pin is active low  */
} flexio_pin_polarity_t;

/* Pin configuration options   */
typedef enum
{
    FLEXIO_PIN_CONFIG_DISABLED     = 0x00U,    /*!< Shifter pin output disabled */
    FLEXIO_PIN_CONFIG_OPEN_DRAIN   = 0x01U,    /*!< Shifter pin open drain or bidirectional output enable */
    FLEXIO_PIN_CONFIG_BIDIR_OUTPUT = 0x02U,    /*!< Shifter pin bidirectional output data */
    FLEXIO_PIN_CONFIG_OUTPUT       = 0x03U,    /*!< Shifter pin output */
} flexio_pin_config_t;

/* Shifter mode options   */
typedef enum
{
    FLEXIO_SHIFTER_MODE_DISABLED          = 0x00U,  /*!< Shifter disabled */
    FLEXIO_SHIFTER_MODE_RECEIVE           = 0x01U,  /*!< Receive mode */
    FLEXIO_SHIFTER_MODE_TRANSMIT          = 0x02U,  /*!< Transmit mode */
    FLEXIO_SHIFTER_MODE_MATCH_STORE       = 0x04U,  /*!< Match Store mode */
    FLEXIO_SHIFTER_MODE_MATCH_CONTINUOUS  = 0x05U,  /*!< Match Continuous mode */
} flexio_shifter_mode_t;

/* Shifter input source options   */
typedef enum
{
    FLEXIO_SHIFTER_SOURCE_PIN        = 0x00U,  /*!< Input source is selected pin */
    FLEXIO_SHIFTER_SOURCE_SHIFTER    = 0x01U,  /*!< Input source is shifter N+1 output */
} flexio_shifter_source_t;

/* Read/Write mode for shifter buffer   */
typedef enum
{
    FLEXIO_SHIFTER_RW_MODE_NORMAL    = 0x00U,  /*!< Normal shifter buffer read/write */
    FLEXIO_SHIFTER_RW_MODE_BIT_SWAP  = 0x01U,  /*!< Data is bit-swapped */
    FLEXIO_SHIFTER_RW_MODE_BYTE_SWAP = 0x02U,  /*!< Data is byte-swapped */
    FLEXIO_SHIFTER_RW_MODE_BB_SWAP   = 0x03U,  /*!< Data in each byte is bit-swapped */
} flexio_shifter_buffer_mode_t;

/* Trigger polarity   */
typedef enum
{
    FLEXIO_TRIGGER_POLARITY_HIGH = 0x00U,          /*!< Trigger is active high */
    FLEXIO_TRIGGER_POLARITY_LOW  = 0x01U,          /*!< Trigger is active low  */
} flexio_trigger_polarity_t;

/* Trigger sources   */
typedef enum
{
    FLEXIO_TRIGGER_SOURCE_EXTERNAL  = 0x00U,  /*!< External trigger selected */
    FLEXIO_TRIGGER_SOURCE_INTERNAL  = 0x01U,  /*!< Internal trigger selected */
} flexio_trigger_source_t;

/* Timer mode options   */
typedef enum
{
    FLEXIO_TIMER_MODE_DISABLED    = 0x00U,  /*!< Timer Disabled. */
    FLEXIO_TIMER_MODE_8BIT_BAUD   = 0x01U,  /*!< Dual 8-bit counters baud/bit mode. */
    FLEXIO_TIMER_MODE_8BIT_PWM    = 0x02U,  /*!< Dual 8-bit counters PWM mode. */
    FLEXIO_TIMER_MODE_16BIT       = 0x03U,  /*!< Single 16-bit counter mode. */
} flexio_timer_mode_t;

/* Timer initial output options   */
typedef enum
{
    FLEXIO_TIMER_INITOUT_ONE          = 0x00U,  /*!< Timer output is logic one when enabled, unaffected by timer reset. */
    FLEXIO_TIMER_INITOUT_ZERO         = 0x01U,  /*!< Timer output is logic zero when enabled, unaffected by timer reset. */
    FLEXIO_TIMER_INITOUT_ONE_RESET    = 0x02U,  /*!< Timer output is logic one when enabled and on timer reset. */
    FLEXIO_TIMER_INITOUT_ZERO_RESET   = 0x03U,  /*!< Timer output is logic zero when enabled and on timer reset. */
} flexio_timer_output_t;

/* Timer decrement options   */
typedef enum
{
    FLEXIO_TIMER_DECREMENT_CLK_SHIFT_TMR      = 0x00U,  /*!< Decrement counter on FlexIO clock, Shift clock equals Timer output. */
    FLEXIO_TIMER_DECREMENT_TRG_SHIFT_TMR      = 0x01U,  /*!< Decrement counter on Trigger input (both edges), Shift clock equals Timer output. */
    FLEXIO_TIMER_DECREMENT_PIN_SHIFT_PIN      = 0x02U,  /*!< Decrement counter on Pin input (both edges), Shift clock equals Pin input. */
    FLEXIO_TIMER_DECREMENT_TRG_SHIFT_TRG      = 0x03U,  /*!< Decrement counter on Trigger input (both edges), Shift clock equals Trigger input. */
} flexio_timer_decrement_t;

/* Timer reset options   */
typedef enum
{
    FLEXIO_TIMER_RESET_NEVER                  = 0x00U,  /*!< Timer never reset. */
    FLEXIO_TIMER_RESET_PIN_OUT                = 0x02U,  /*!< Timer reset on Timer Pin equal to Timer Output. */
    FLEXIO_TIMER_RESET_TRG_OUT                = 0x03U,  /*!< Timer reset on Timer Trigger equal to Timer Output. */
    FLEXIO_TIMER_RESET_PIN_RISING             = 0x04U,  /*!< Timer reset on Timer Pin rising edge. */
    FLEXIO_TIMER_RESET_TRG_RISING             = 0x06U,  /*!< Timer reset on Trigger rising edge. */
    FLEXIO_TIMER_RESET_TRG_BOTH               = 0x07U,  /*!< Timer reset on Trigger rising or falling edge. */
} flexio_timer_reset_t;

/* Timer disable options   */
typedef enum
{
    FLEXIO_TIMER_DISABLE_NEVER                = 0x00U,  /*!< Timer never disabled. */
    FLEXIO_TIMER_DISABLE_TIM_DISABLE          = 0x01U,  /*!< Timer disabled on Timer N-1 disable. */
    FLEXIO_TIMER_DISABLE_TIM_CMP              = 0x02U,  /*!< Timer disabled on Timer compare. */
    FLEXIO_TIMER_DISABLE_TIM_CMP_TRG_LOW      = 0x03U,  /*!< Timer disabled on Timer compare and Trigger Low. */
    FLEXIO_TIMER_DISABLE_PIN                  = 0x04U,  /*!< Timer disabled on Pin rising or falling edge. */
    FLEXIO_TIMER_DISABLE_PIN_TRG_HIGH         = 0x05U,  /*!< Timer disabled on Pin rising or falling edge provided Trigger is high. */
    FLEXIO_TIMER_DISABLE_TRG                  = 0x06U,  /*!< Timer disabled on Trigger falling edge. */
} flexio_timer_disable_t;

/* Timer disable options   */
typedef enum
{
    FLEXIO_TIMER_ENABLE_ALWAYS                  = 0x00U,  /*!< Timer always enabled. */
    FLEXIO_TIMER_ENABLE_TIM_ENABLE              = 0x01U,  /*!< Timer enabled on Timer N-1 enable. */
    FLEXIO_TIMER_ENABLE_TRG_HIGH                = 0x02U,  /*!< Timer enabled on Trigger high. */
    FLEXIO_TIMER_ENABLE_TRG_PIN_HIGH            = 0x03U,  /*!< Timer enabled on Trigger high and Pin high. */
    FLEXIO_TIMER_ENABLE_PIN_POSEDGE             = 0x04U,  /*!< Timer enabled on Pin rising edge. */
    FLEXIO_TIMER_ENABLE_PIN_POSEDGE_TRG_HIGH    = 0x05U,  /*!< Timer enabled on Pin rising edge and Trigger high. */
    FLEXIO_TIMER_ENABLE_TRG_POSEDGE             = 0x06U,  /*!< Timer enabled on Trigger rising edge. */
    FLEXIO_TIMER_ENABLE_TRG_EDGE                = 0x07U,  /*!< Timer enabled on Trigger rising or falling edge. */
} flexio_timer_enable_t;

/* Timer stop bit options   */
typedef enum
{
    FLEXIO_TIMER_STOP_BIT_DISABLED              = 0x00U,  /*!< Stop bit disabled. */
    FLEXIO_TIMER_STOP_BIT_TIM_CMP               = 0x01U,  /*!< Stop bit is enabled on timer compare. */
    FLEXIO_TIMER_STOP_BIT_TIM_DIS               = 0x02U,  /*!< Stop bit is enabled on timer disable. */
    FLEXIO_TIMER_STOP_BIT_TIM_CMP_DIS           = 0x03U,  /*!< Stop bit is enabled on timer compare and disable. */
} flexio_timer_stop_t;

/* Timer stop bit options - for Transmit, Receive or Match Store modes only   */
typedef enum
{
    FLEXIO_SHIFTER_STOP_BIT_DISABLED            = 0x00U,  /*!< Stop bit disabled. */
    FLEXIO_SHIFTER_STOP_BIT_0                   = 0x02U,  /*!< Transmit/expect stop bit value 0 */
    FLEXIO_SHIFTER_STOP_BIT_1                   = 0x03U,  /*!< Transmit/expect stop bit value 1 */
} flexio_shifter_stop_t;

/* Timer start bit options - for Transmit, Receive or Match Store modes only   */
typedef enum
{
    FLEXIO_SHIFTER_START_BIT_DISABLED       = 0x00U,  /*!< Start bit disabled, transmitter loads data on enable */
    FLEXIO_SHIFTER_START_BIT_DISABLED_SH    = 0x01U,  /*!< Start bit disabled, transmitter loads data on first shift */
    FLEXIO_SHIFTER_START_BIT_0              = 0x02U,  /*!< Transmit/expect start bit value 0 */
    FLEXIO_SHIFTER_START_BIT_1              = 0x03U,  /*!< Transmit/expect start bit value 1 */
} flexio_shifter_start_t;

/* Timer start bit options   */
typedef enum
{
    FLEXIO_TIMER_START_BIT_DISABLED              = 0x00U,  /*!< Start bit disabled. */
    FLEXIO_TIMER_START_BIT_ENABLED               = 0x01U,  /*!< Start bit enabled. */
} flexio_timer_start_t;

/* FlexIO module version number  */
typedef struct
{
    uint8_t  majorNumber;       /**< Major Version Number */
    uint8_t  minorNumber;       /**< Minor Version Number */
    uint16_t featureNumber;     /**< Feature Specification Number */
} flexio_version_info_t;


#endif /* FLEXIO_H_ */
