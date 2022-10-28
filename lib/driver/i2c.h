#ifndef __I2C_H__
#define __I2C_H__

#define I2C_0		(0x01C27000)
#define I2C_1		(0x01C27400)
#define I2C_2		(0x01C27800)

#define I2C_CLK_LOW_10KHZ		(10000)
#define I2C_CLK_STD_100KHZ		(100000)
#define I2C_CLK_FAST_400KHZ		(400000)

typedef enum{
    I2CMode_Master = 0,
    I2CMode_Slave,
}I2CMode;

typedef enum{
	StartConditionError = -2,
	WriteDataError = -1,
	Success = 0,
}TransmissionStatus;

typedef struct{
	uint8_t mode;
	uint8_t slaveAddr;
	uint8_t slaveExAddr;
	uint8_t enGeneralCall;
}I2CConfig;

void i2c_init(uint32_t unit, uint32_t clk, I2CConfig cfg);
void i2c_startTransmission(uint32_t unit, uint8_t addr);
void i2c_write(uint32_t unit, uint8_t data);
TransmissionStatus i2c_endTransmission(uint32_t unit, uint8_t stopBit);
TransmissionStatus i2c_probe(uint32_t unit, uint8_t addr);
void i2c_exit(uint32_t unit);

#endif /* __I2C_H__ */