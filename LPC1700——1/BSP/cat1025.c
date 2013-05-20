#include "cat1025.h"
#include "lpc17xx_i2c.h"
#include "lpc17xx_pinsel.h"
//#include "os.h"
#define I2CDEV   LPC_I2C2
#define SLVADDR  (0xa0)

// /*
// typedef struct
// {
//   uint32_t          sl_addr7bit;				/**< Slave address in 7bit mode */
//   __IO uint8_t*     tx_data;					/**< Pointer to Transmit data - NULL if data transmit
// 													  is not used */
//   uint32_t          tx_length;					/**< Transmit data length - 0 if data transmit
// 													  is not used*/
//   __IO uint32_t     tx_count;					/**< Current Transmit data counter */
//   __IO uint8_t*     rx_data;					/**< Pointer to Receive data - NULL if data receive
// 													  is not used */
//   uint32_t          rx_length;					/**< Receive data length - 0 if data receive is
// 													   not used */
//   __IO uint32_t     rx_count;					/**< Current Receive data counter */
//   uint32_t          retransmissions_max;		/**< Max Re-Transmission value */
//   uint32_t          retransmissions_count;		/**< Current Re-Transmission counter */
//   __IO uint32_t     status;						/**< Current status of I2C activity */
//   void 				(*callback)(void);			/**< Pointer to Call back function when transmission complete
// 													used in interrupt transfer mode */
// } I2C_M_SETUP_Type;

// */

I2C_M_SETUP_Type transferCfg;
uint8_t cat1025_sendbuf[5] = {0, 1, 2, 3, 4};
uint8_t cat1025_readbuf[5] ;
__IO FlagStatus complete;

void cat1025_send(void)
{
    complete = RESET;
    transferCfg.sl_addr7bit = SLVADDR;
	transferCfg.tx_data = (uint8_t *)cat1025_sendbuf;
	transferCfg.tx_length = sizeof(cat1025_sendbuf);
	transferCfg.rx_data = NULL;
	transferCfg.rx_length = 0;
	transferCfg.retransmissions_max = 1;
    
    //
    transferCfg.reg_addr          = 0x00;
    transferCfg.reg_addr_byte_nbr = 1;
    transferCfg.send_opt_type     = SEND_WR_ADR_OPT;
	I2C_MasterTransferData(I2CDEV, &transferCfg, I2C_TRANSFER_INTERRUPT);
    while (complete == RESET);          
}

void cat1025_read()
{
    complete = RESET;
    transferCfg.sl_addr7bit = (SLVADDR | 0x01);
	  transferCfg.tx_data   = NULL;
	  transferCfg.tx_length = 0;
	  transferCfg.rx_data   = (uint8_t *)cat1025_readbuf;
	  transferCfg.rx_length = sizeof(cat1025_sendbuf);
	  transferCfg.retransmissions_max = 1;
    //
    transferCfg.reg_addr          = 0x00;
    transferCfg.reg_addr_byte_nbr = 1;
    transferCfg.send_opt_type     = SEND_RD_ADR_OPT;
	  I2C_MasterTransferData(I2CDEV, &transferCfg, I2C_TRANSFER_INTERRUPT);  
    while (complete == RESET);     
}

void I2C2_IRQHandler(void)
{
    I2C_MasterHandler(I2CDEV);
	if (I2C_MasterTransferComplete(I2CDEV)){
		complete = SET;
	}
}

void cat1025_init(void)
{
    PINSEL_CFG_Type PinCfg;
    //
    
    PinCfg.Funcnum = 2;
	PinCfg.Pinnum = 10;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 11;
	PINSEL_ConfigPin(&PinCfg);
    
    I2C_Init(I2CDEV, 50000);
    I2C_Cmd(I2CDEV, I2C_MASTER_MODE, ENABLE);
    
    NVIC_SetPriority(I2C2_IRQn, 0x03);
    NVIC_EnableIRQ(I2C2_IRQn);  

    cat1025_send();
    cat1025_read();    
}
