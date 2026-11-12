#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_i2c.h"
#include "fsl_debug_console.h"

#define MAX_I2C     I2C1
#define MAX_ADDR    87

static status_t max_write(uint8_t reg, uint8_t val)
{
    i2c_master_transfer_t x = {0};
    x.slaveAddress = MAX_ADDR;
    x.direction = kI2C_Write;
    x.subaddress = reg; x.subaddressSize = 1;
    x.data = &val; x.dataSize = 1;
    return I2C_MasterTransferBlocking(MAX_I2C, &x);
}

static status_t max_read(uint8_t reg, uint8_t *buf, size_t len)
{
    i2c_master_transfer_t x = {0};
    x.slaveAddress = MAX_ADDR;
    x.direction = kI2C_Read;
    x.subaddress = reg; x.subaddressSize = 1;
    x.data = buf; x.dataSize = len;
    return I2C_MasterTransferBlocking(MAX_I2C, &x);
}

int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitACCEL_I2CPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    i2c_master_config_t cfg;
    I2C_MasterGetDefaultConfig(&cfg);      // 100 kHz
    I2C_MasterInit(MAX_I2C, &cfg, CLOCK_GetBusClkFreq());

    uint8_t id = 0;
    status_t s = max_read(0xFF, &id, 1);
    PRINTF("status=%d  part ID=0x%02X (attendu 0x15)\r\n", s, id);

    while (1) {}
}
