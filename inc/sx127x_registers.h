/**
 * @ Author: Morran Smith
 * @ Create Time: 2019-06-01 09:54:20
 * @ Modified by: Morran Smith
 * @ Modified time: 2019-06-02 09:23:52
 * @ Description:
 */

#ifndef SX127X_REGISTERS_H
#define SX127X_REGISTERS_H

#define RegFifo 0x00
#define RegOpMode 0x01
#define RegFrMsb 0x06
#define RegFrMid 0x07
#define RegFrLsb 0x08
#define RegPaConfig 0x09
#define RegPaRamp 0x0a
#define RegOcp 0x0b
#define RegLna 0x0c
#define RegFifoAddrPtr 0x0d
#define RegFifoTxBaseAddr 0x0e
#define RegFifoRxBaseAddr 0x0f
#define RegFifoRxCurrentAddr 0x10
#define RegIrqFlagsMask 0x11
#define RegIrqFlags 0x12
#define RegRxNbBytes 0x13
#define RegRxHeaderCntValueMsb 0x14
#define RegRxHeaderCntValueLsb 0x15
#define RegRxPacketCntValueMsb 0x16
#define RegRxPacketCntValueLsb 0x17
#define RegModemStat 0x18
#define RegPktSnrValue 0x19
#define RegPktRssiValue 0x1a
#define RegRssiValue 0x1b
#define RegHopChannel 0x1c
#define RegModemConfig1 0x1d
#define RegModemConfig2 0x1e
#define RegSymbTimeoutLsb 0x1f
#define RegPreambleMsb 0x20
#define RegPreambleLsb 0x21
#define RegPayloadLength 0x22
#define RegMaxPayloadLength 0x23
#define RegHopPeriod 0x24
#define RegFifoByteAddr 0x25
#define RegModemConfig3 0x26
#define RegFeiMsb 0x28
#define RegFeiMid 0x29
#define RegFeiLsb 0x2a
#define RegRssiWideband 0x2c
#define RegDetectOptimize 0x31
#define RegInvertIQ 0x33
#define RegDetectionThreshold 0x37
#define RegSyncWord 0x39
#define RegDioMapping1 0x40
#define RegDioMapping2 0x41
#define RegVersion 0x42
#define RegPllHop 0x44
#define RegTcxo 0x4B
#define RegPaDac 0x4D
#define RegFormerTemp 0x5B
#define RegAgcRef 0x61
#define RegAgcTresh1 0x62
#define RegAgcTresh2 0x63
#define RegAgcTresh3 0x64

#define FlagRxTimeout (1 << 7)
#define FlagRxDone (1 << 6)
#define FlagPayloadCrcError (1 << 5)
#define FlagValidHeader (1 << 4)
#define FlagTxDone (1 << 3)
#define FlagCadDone (1 << 2)
#define FlagFhssChangeChannel (1 << 1)
#define FlagCadDetected (1 << 0)

#define DIO_0_MAPPING 14
#define DIO_1_MAPPING 12
#define DIO_2_MAPPING 10
#define DIO_3_MAPPING 8
#define DIO_4_MAPPING 6
#define DIO_5_MAPPING 4

#define DIO_MODE_0 0
#define DIO_MODE_1 1
#define DIO_MODE_2 2
#define DIO_MODE_DISABLE 3

#define VERSION 0x12

#endif /* SX127X_REGISTERS_H */
