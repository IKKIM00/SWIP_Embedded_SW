/**********************************************************************************************************************
 * \file Cpu0_Main.c
 * \copyright Copyright (C) Infineon Technologies AG 2019
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG or its licensees. If and as long as no such terms of use
 * are agreed, use of this file is subject to following:
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and
 * accompanying documentation covered by this license (the "Software") to use, reproduce, display, distribute, execute,
 * and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the
 * Software is furnished to do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including the above license grant, this restriction
 * and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all
 * derivative works of the Software, unless such copies or derivative works are solely in the form of
 * machine-executable object code generated by a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *********************************************************************************************************************/
#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "ifxCcu6_reg.h"
#include<stdio.h>
#include "ifxVadc_reg.h"
#include "IfxGtm_reg.h"
#include "Ifx_Types.h"

IfxCpu_syncEvent g_cpuSyncEvent = 0;

// Port registers
#define PC0_BIT_LSB_IDX             3
#define PC1_BIT_LSB_IDX             11
#define PC2_BIT_LSB_IDX             19
#define PC3_BIT_LSB_IDX             27
#define PC4_BIT_LSB_IDX             3
#define PC5_BIT_LSB_IDX             11
#define PC6_BIT_LSB_IDX             19
#define PC7_BIT_LSB_IDX             27

#define P0_BIT_LSB_IDX              0
#define P1_BIT_LSB_IDX              1
#define P2_BIT_LSB_IDX              2
#define P3_BIT_LSB_IDX              3
#define P4_BIT_LSB_IDX              4
#define P5_BIT_LSB_IDX              5
#define P6_BIT_LSB_IDX              6
#define P7_BIT_LSB_IDX              7

// SCU registers
#define LCK_BIT_LSB_IDX             1
#define ENDINIT_BIT_LSB_IDX         0
#define EXIS0_BIT_LSB_IDX           4
#define EXIS1_BIT_LSB_IDX           20
#define FEN0_BIT_LSB_IDX            8
#define EIEN0_BIT_LSB_IDX           11
#define INP0_BIT_LSB_IDX            12
#define IGP0_BIT_LSB_IDX            14
#define REN0_BIT_LSB_IDX            9

// SRC registers
#define SRPN_BIT_LSB_IDX            0
#define TOS_BIT_LSB_IDX             11
#define SRE_BIT_LSB_IDX             10

// CCU60 registers
#define DISS_BIT_LSB_IDX            1
#define DISR_BIT_LSB_IDX            0
#define CTM_BIT_LSB_IDX             7
#define T12PRE_BIT_LSB_IDX          3
#define T12CLK_BIT_LSB_IDX          0
#define T12STR_BIT_LSB_IDX          6
#define T12RS_BIT_LSB_IDX           1
#define INPT12_BIT_LSB_IDX          10
#define ENT12PM_BIT_LSB_IDX         7

// VADC registers
#define DISS_BIT_LSB_IDX            1
#define DISR_BIT_LSB_IDX            0
#define ANONC_BIT_LSB_IDX           0
#define ASEN0_BIT_LSB_IDX           24
#define CSM0_BIT_LSB_IDX            3
#define PRIO0_BIT_LSB_IDX           0
#define CMS_BIT_LSB_IDX             8
#define FLUSH_BIT_LSB_IDX           10
#define TREV_BIT_LSB_IDX            9
#define ENGT_BIT_LSB_IDX            0
#define RESPOS_BIT_LSB_IDX          21
#define RESREG_BIT_LSB_IDX          16
#define ICLSEL_BIT_LSB_IDX          0
#define VF_BIT_LSB_IDX              31
#define RESULT_BIT_LSB_IDX          0
#define ASSCH7_BIT_LSB_IDX          7

// GTM registers
#define DISS_BIT_LSB_IDX            1
#define DISR_BIT_LSB_IDX            0
#define SEL7_BIT_LSB_IDX            14
#define EN_FXCLK_BIT_LSB_IDX        22
#define FXCLK_SEL_BIT_LSB_IDX       0

// GTM - TOM0 registers
#define UPEN_CTRL1_BIT_LSB_IDX      18
#define HOST_TRIG_BIT_LSB_IDX       0
#define ENDIS_CTRL1_BIT_LSB_IDX     2
#define OUTEN_CTRL1_BIT_LSB_IDX     2
#define CLK_SRC_SR_BIT_LSB_IDX      12
#define OSM_BIT_LSB_IDX             26
#define TRIGOUT_BIT_LSB_IDX         24
#define SL_BIT_LSB_IDX              11

#define UPEN_CTRL3_BIT_LSB_IDX    22
#define ENDIS_CTRL3_BIT_LSB_IDX 6
#define OUTEN_CTRL3_BIT_LSB_IDX 6

#define SEL1_BIT_LSB_IDX        2
#define SEL3_BIT_LSB_IDX        6

#define MOTOR_RUN               1
#define MOTOR_STOP              0

void initLED(void);
void initButton(void);
void initERU(void);
void initCCU60(void);
void initRGBLED(void);
void initVADC(void);
void VADC_startConversion(void);
unsigned int VADC_readResult(void);
void initGTM(void);
void initUSonic(void);
void usonicTrigger(void);
void initCCU61(void);
void initBuzzer(void);
void initMotor(void);
void RunMotor(int Direction);
void activateBuzzer(int duty);
void deactivateBuzzer(void);
void GoMelody();

unsigned int range;
unsigned char range_valid_flag = 0;
unsigned int flag = 0;
unsigned int MotorStatus = 0;
unsigned int MotorCount = 0;
unsigned int MotorDirection = 0;

unsigned int SongCount = 0;
int Doremi[] = {130, 146, 164, 174, 195, 220, 246};
int Melody[] = {4,4,5,5,4,4,3,  4,4,3,3,2,   4,4,5,5,4,4,3,   4,3,2,3,1};

__interrupt(0x0A) __vector_table(0)
void ERU0_ISR(void)
{
    if ((P00_IN.U & (0x1 << P4_BIT_LSB_IDX)) != 0)   //rising edge of echo
    {
        CCU61_TCTR4.U = 0x1 << T12RS_BIT_LSB_IDX;    //start CCU61 T12 counter

    }                                                  // falling edge of echo
    else
    {
        CCU61_TCTR4.B.T12RR = 0x1;                  // stop CCU61 T12 counter

        range = ((CCU61_T12.B.T12CV * 1000000) / 48828) / 58;
        range_valid_flag = 1;

        CCU61_TCTR4.B.T12RES = 0x1; // reset CCU61_T12_counter
    }

}

__interrupt(0x0B) __vector_table(0)
void CCU60_T12_ISR(void)
{
    P02_OUT.U &= ~(0x1 << P6_BIT_LSB_IDX);
}

void RunMotor(int Direction)
{

    unsigned short duty = 0;
    unsigned int adcResult;
    VADC_startConversion();
    adcResult = VADC_readResult();
    duty = 12500*adcResult/4096;  // 12bit 0~4096

    P02_OUT.U |= 0x1 << P1_BIT_LSB_IDX;
    if(Direction)
    {
        P10_OUT.U |= 0x1 << P1_BIT_LSB_IDX; // 시계방향 모터 회전

    }
    else
    {
        P10_OUT.U &= ~(0x1 << P1_BIT_LSB_IDX);  // 반시계 모터 회전
    }

    GTM_TOM0_CH9_SR1.U = duty;
}

int core0_main(void)
{
    IfxCpu_enableInterrupts();
    
    /* !!WATCHDOG0 AND SAFETY WATCHDOG ARE DISABLED HERE!!
     * Enable the watchdogs and service them periodically if it is required
     */
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());
    
    /* Wait for CPU sync event */
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 1);
    //-----------------------------------------------------------------------------------------

    initLED();
    initButton();
    initMotor();
    initGTM();
    initRGBLED();
    initVADC();
    initBuzzer();
    initUSonic();
    initCCU60();
    initCCU61();
    initERU();

    // trigger update request signal
    GTM_TOM0_TGC0_GLB_CTRL.U |= 0x1 << HOST_TRIG_BIT_LSB_IDX;
    GTM_TOM0_TGC1_GLB_CTRL.U |= 0x1 << HOST_TRIG_BIT_LSB_IDX;

    int nMelodyCount = 0;
    while(1)
    {
        if(MotorStatus == MOTOR_STOP)
        {
            if((P02_IN.U & (0x1 << P0_BIT_LSB_IDX)) == 0) // check button status
            {
                MotorStatus = MOTOR_RUN;
                P10_OUT.U |= 0x1<<P2_BIT_LSB_IDX;   // 파란색 LED 발광
                MotorDirection ^= 1;
                RunMotor(MotorDirection);
            }
        }
        else // MOTOR_RUN
        {
            MotorCount++;
            if(MotorCount >= 100)
            {
                MotorStatus = MOTOR_STOP;
                P10_OUT.U &= ~(0x1 << P2_BIT_LSB_IDX); // 파란색 LED 끄기
                GTM_TOM0_CH9_SR1.U = 0;
                MotorCount = 0;
            }
        }

        usonicTrigger();

        while( range_valid_flag == 0);

        if(range >= 80)
        {
            P10_OUT.U &= ~(0x1 << P5_BIT_LSB_IDX); // Green
            P10_OUT.U &= ~(0x1 << P3_BIT_LSB_IDX); // Blue

            GTM_TOM0_CH11_SR0.B.SR0 = 0;
            GTM_TOM0_CH11_SR1.B.SR1 = 0;
            SongCount = 0;

        }
        else if (range >= 60)
        {
            P10_OUT.U |= (0x01) << P5_BIT_LSB_IDX; // Green On
            P10_OUT.U &= ~(0x1 << P3_BIT_LSB_IDX); // Blue

            GTM_TOM0_CH11_SR0.B.SR0 = 0;
            GTM_TOM0_CH11_SR1.B.SR1 = 0;
            SongCount = 0;

        }
        else if (range >= 20)
        {
            P10_OUT.U &= ~(0x1 << P5_BIT_LSB_IDX); // Green
            P10_OUT.U |= (0x01) << P3_BIT_LSB_IDX; // Blue On

            GTM_TOM0_CH11_SR0.B.SR0 = 0;
            GTM_TOM0_CH11_SR1.B.SR1 = 0;
            SongCount = 0;
        }
        else
        {
            P10_OUT.U |= (0x01) << P5_BIT_LSB_IDX; // Green On
            P10_OUT.U |= (0x01) << P3_BIT_LSB_IDX; // Blue On

            nMelodyCount++;
            if(nMelodyCount >= 20)
            {
                GoMelody();
                nMelodyCount = 0;
            }
        }

        // STOP 상태에서만 RUN으로 갈 수 있다.

    }
    return (1);
}


void initLED(void)
{
    P10_IOCR0.U &= ~(0x1F << PC1_BIT_LSB_IDX); // reset P10_IOCR0 PC1
    P10_IOCR0.U &= ~(0x1F << PC2_BIT_LSB_IDX); // reset P10_IOCR0 PC1

    //P10_IOCR0.U |= 0x10 << PC1_BIT_LSB_IDX; // set P10.1 push-pull general output
    //P10_IOCR0.U |= 0x11 << PC1_BIT_LSB_IDX; // set P10.1 GTM output
    P10_IOCR0.U |= 0x10 << PC2_BIT_LSB_IDX; // set P10.2 push-pull general output
}

void initButton(void)
{
    P02_IOCR0.U &= ~(0x1F << PC0_BIT_LSB_IDX); // reset P02_IOCR0 PC1

    P02_IOCR0.U |= 0x02 << PC0_BIT_LSB_IDX; // set P02.1  general input(pull-up connected)
}

void initERU(void){
    // ERU setting
    SCU_EICR1.U &= ~(0x7 << EXIS0_BIT_LSB_IDX);
    SCU_EICR1.U |= (0x2 <<  EXIS0_BIT_LSB_IDX);

    SCU_EICR1.U |= (0x1 << FEN0_BIT_LSB_IDX); //   FALLING ENABLE
    SCU_EICR1.U |= (0x1 << REN0_BIT_LSB_IDX); //   RISING ENABLE

    SCU_EICR1.U |= (0x1 << EIEN0_BIT_LSB_IDX);  // EXTERNAL REQUEST ENABLE (MASTER ENABLE)

    SCU_EICR1.U &=  ~(0x7 << INP0_BIT_LSB_IDX); // OGU0 Selection

    SCU_IGCR0.U &= ~(0x3 << IGP0_BIT_LSB_IDX);
    SCU_IGCR0.U |=  (0x1 << IGP0_BIT_LSB_IDX); //OGU0.IOUT selection | There are 2 options   IOUT / PDOUT


    // SRC Interrupt Setting
    SRC_SCU_SCU_ERU0.U &= ~(0xFF << SRPN_BIT_LSB_IDX);
    SRC_SCU_SCU_ERU0.U |=  (0x0A << SRPN_BIT_LSB_IDX); // Interrupt ID & Priority setting

    SRC_SCU_SCU_ERU0.U &= ~(0x3 << TOS_BIT_LSB_IDX); // CPU0 SELECTION
    SRC_SCU_SCU_ERU0.U |= 1 << SRE_BIT_LSB_IDX; //Service request enable
}


void initRGBLED(void)
{
    // reset Port IOCR register
    P02_IOCR4.U &= ~(0x1F << PC7_BIT_LSB_IDX);
    P10_IOCR4.U &= ~(0x1F << PC5_BIT_LSB_IDX);
    P10_IOCR0.U &= ~(0x1F << PC3_BIT_LSB_IDX);
    // set Port as general purpose output (push-pull)
    P02_IOCR4.U |= (0x10 << PC7_BIT_LSB_IDX);
    P10_IOCR4.U |= (0x10 << PC5_BIT_LSB_IDX);
    P10_IOCR0.U |= (0x10 << PC3_BIT_LSB_IDX);
}

void initVADC(void)
{
    // Password Access to unlock SCU_WDTSCON0
    SCU_WDTCPU0_CON0.U = ((SCU_WDTCPU0_CON0.U ^ 0xFC) & ~(1<< LCK_BIT_LSB_IDX)) | (1<<ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1<<LCK_BIT_LSB_IDX)) !=0); //wait until unlocked

    //Modify Access to clear ENDINIT
    SCU_WDTCPU0_CON0.U = ((SCU_WDTCPU0_CON0.U ^ 0xFC) | (1<< LCK_BIT_LSB_IDX)) & ~(1<<ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1<<LCK_BIT_LSB_IDX)) ==0); //wait until locked

    VADC_CLC.U &= ~(1<<DISR_BIT_LSB_IDX); // enable CCU

    // Password Access to unlock SCU_WDTSCON0
    SCU_WDTCPU0_CON0.U = ((SCU_WDTCPU0_CON0.U ^ 0xFC) & ~(1<< LCK_BIT_LSB_IDX)) | (1<<ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1<<LCK_BIT_LSB_IDX)) !=0); //wait until unlocked

        //Modify Access to set ENDINIT
    SCU_WDTCPU0_CON0.U = ((SCU_WDTCPU0_CON0.U ^ 0xFC) | (1<< LCK_BIT_LSB_IDX)) | (1<<ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1<<LCK_BIT_LSB_IDX)) ==0); //wait until locked

    while((VADC_CLC.U & (1<<DISS_BIT_LSB_IDX)) != 0);// wait until VADC_CLC module clock enabled

    VADC_G4_ARBPR.U |= 0x3 << PRIO0_BIT_LSB_IDX;    // highest priority for Requesst Source 0
    VADC_G4_ARBPR.U &= ~(0x1 << CSM0_BIT_LSB_IDX);  // Wait-for-Start Mode
    VADC_G4_ARBPR.U |= 0x1 << ASEN0_BIT_LSB_IDX;    // Arbitraction Source Input 0 Enable

    VADC_G4_QMR0.U &= ~(0x3 << ENGT_BIT_LSB_IDX);
    VADC_G4_QMR0.U |= (0x1 << ENGT_BIT_LSB_IDX);   // enable conversion resquest
    VADC_G4_QMR0.U |= (0x1 << FLUSH_BIT_LSB_IDX);   // clear ADC queue

    VADC_G4_ARBCFG.U |= 0x3 << ANONC_BIT_LSB_IDX;

    VADC_G4_ICLASS0.U &= ~(0x7 << CMS_BIT_LSB_IDX);

    // VADC Group 4 Channel 7 configuration
    VADC_G4_CHCTR7.U |= 0x1 << RESPOS_BIT_LSB_IDX;  // result right-aligned
    VADC_G4_CHCTR7.U &= ~(0xF << RESREG_BIT_LSB_IDX);   // store result @ Result Register G4RES0
    VADC_G4_CHCTR7.U &= ~(0x3 << ICLSEL_BIT_LSB_IDX);   // Class 0

    VADC_G4_CHASS.U |= 0x1 << ASSCH7_BIT_LSB_IDX;
}

void VADC_startConversion(void)
{
    VADC_G4_QINR0.U |= 0x07;    // no. of Request Channel = 7
    VADC_G4_QMR0.U |= 0x1 << TREV_BIT_LSB_IDX;  // Generate Conversion Start Trigger Event
}

unsigned int VADC_readResult(void)
{
    unsigned int result;

    while( (VADC_G4_RES0.U & (0x1 << VF_BIT_LSB_IDX)) == 0);    // wait until read available
    result = VADC_G4_RES0.U & (0xFFF << RESULT_BIT_LSB_IDX);    // read ADC value

    return result;
}

void initGTM(void)
{
    // Password Access to unlock SCU_WDTSCON0
    SCU_WDTCPU0_CON0.U = ((SCU_WDTCPU0_CON0.U ^ 0xFC) & ~(1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1<<LCK_BIT_LSB_IDX)) !=0); //wait until unlocked

    //Modify Access to clear ENDINIT
    SCU_WDTCPU0_CON0.U = ((SCU_WDTCPU0_CON0.U ^ 0xFC) | (1 << LCK_BIT_LSB_IDX)) & ~(1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1<<LCK_BIT_LSB_IDX)) ==0); //wait until locked

    GTM_CLC.U &= ~(1<<DISR_BIT_LSB_IDX); // enable CCU

    // Password Access to unlock SCU_WDTSCON0
    SCU_WDTCPU0_CON0.U = ((SCU_WDTCPU0_CON0.U ^ 0xFC) & ~(1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1<<LCK_BIT_LSB_IDX)) !=0); //wait until unlocked

    //Modify Access to set ENDINIT
    SCU_WDTCPU0_CON0.U = ((SCU_WDTCPU0_CON0.U ^ 0xFC) | (1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1<<LCK_BIT_LSB_IDX)) ==0); //wait until locked

    while((GTM_CLC.U & (1<<DISS_BIT_LSB_IDX)) != 0);// wait until GTM module clock enabled

    // GTM clock configuration

    GTM_CMU_FXCLK_CTRL.U &= ~(0xF << FXCLK_SEL_BIT_LSB_IDX);    // input clock of CMU_FXCLK --> CMI_GCLK_EN


    GTM_CMU_CLK_EN.U |= 0x2 << EN_FXCLK_BIT_LSB_IDX;            // enable all CMU_FXCLK

    // GTM TOM0 PWM configuration
    GTM_TOM0_TGC0_GLB_CTRL.U |= 0x2 << UPEN_CTRL1_BIT_LSB_IDX;  // TOM0 channel 1 update enable
    GTM_TOM0_TGC0_ENDIS_CTRL.U |= 0x2 << ENDIS_CTRL1_BIT_LSB_IDX;   // enable channel 1 on update trigger
    GTM_TOM0_TGC0_OUTEN_CTRL.U |= 0x2 << OUTEN_CTRL1_BIT_LSB_IDX;   // enable channel 1 on output on update trigger

    // GTM TOM0 PWM configuration
    GTM_TOM0_TGC1_GLB_CTRL.B.UPEN_CTRL1 |= 0x2; //<< UPEN_CTRL1_BIT_LSB_IDX; // TOM0 channel 1 update enable
    GTM_TOM0_TGC1_ENDIS_CTRL.B.ENDIS_CTRL1 |= 0x2; //<< ENDIS_CTRL1_BIT_LSB_IDX; // enable channel 1 on update trigger
    GTM_TOM0_TGC1_OUTEN_CTRL.B.OUTEN_CTRL1 |= 0x2; //<< OUTEN_CTRL1_BIT_LSB_IDX; // enable channel 1 output on update trigger


    GTM_TOM0_CH1_CTRL.U |= 0x1 << SL_BIT_LSB_IDX;                   // high signal level for duty cycle
    GTM_TOM0_CH1_CTRL.U |= 0x1 << CLK_SRC_SR_BIT_LSB_IDX;           // clock source --> CMU_FXCLK(1) = 6250kHz
    GTM_TOM0_CH1_CTRL.U &= ~(0x1 << OSM_BIT_LSB_IDX);               // continuous mode enables
    GTM_TOM0_CH1_CTRL.U &= ~(0x1 << TRIGOUT_BIT_LSB_IDX);

    GTM_TOM0_CH1_SR0.U = 12500 -1;                                  // PWM freq. = 12500 / 6250 kHz = 2ms
    GTM_TOM0_CH1_SR1.U = 1250 - 1;                                  // duty cycle = 1250 / 12500 = 10%

    GTM_TOM0_CH9_CTRL.B.SL |= 0x1;
    GTM_TOM0_CH9_CTRL.B.CLK_SRC_SR |= 0x1;
    GTM_TOM0_CH9_SR0.U = 12500 - 1; // PWM freq = 12500/6250 kHz = 2ms

    GTM_TOUTSEL6.U &= ~(0x3 << SEL7_BIT_LSB_IDX);   // TOUT103 --> TOM0 channel 1           103 = 16 * 6 + 7
    GTM_TOUTSEL0.U &= ~(0x3 << SEL1_BIT_LSB_IDX);
}

void initCCU60(void){

    //PASSWORD ACCESS TO UNLOCK SCU_WDTCON0

    //PASSWORD TOGGLE, LOCK CLEAR AND END INIT SET
    SCU_WDTCPU0_CON0.U = ( (SCU_WDTCPU0_CON0.U ^ 0xFC) & ~(1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    // wait until unlocked
    while( ((SCU_WDTCPU0_CON0.U) & (1 << LCK_BIT_LSB_IDX) ) != 0 );


    // Modify Access to clear ENDINIT
    //PASSWORD TOGGLE, LOCK SET AND END INIT CLEAR

    SCU_WDTCPU0_CON0.U = ( (SCU_WDTCPU0_CON0.U ^ 0xFC) | (1 << LCK_BIT_LSB_IDX)) & ~(1 << ENDINIT_BIT_LSB_IDX);
    // wait until unlocked
    while( ((SCU_WDTCPU0_CON0.U) & (1 << LCK_BIT_LSB_IDX) ) == 0 );


    // THE ABOVE CODE IS DONE TO ACCESS CCU6 CLK REG

    CCU60_CLC.U &= ~(1 << DISR_BIT_LSB_IDX); // ENABLE CCU60 clock enable


    SCU_WDTCPU0_CON0.U = ( (SCU_WDTCPU0_CON0.U ^ 0xFC) & ~(1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    // wait until unlocked
    while( ((SCU_WDTCPU0_CON0.U) & (1 << LCK_BIT_LSB_IDX) ) != 0 );


    //SET ENDINIT BIT

    SCU_WDTCPU0_CON0.U = ( (SCU_WDTCPU0_CON0.U ^ 0xFC) | (1 << LCK_BIT_LSB_IDX))  | (1 << ENDINIT_BIT_LSB_IDX);
    // wait until locked
    while( ((SCU_WDTCPU0_CON0.U) & (1 << LCK_BIT_LSB_IDX) ) == 0 );


    // CCU60 T12 configuration
    while((CCU60_CLC.U & (1<<DISS_BIT_LSB_IDX)) != 0); // wait until CCU60 module enabled

    CCU60_TCTR0.U &= ~(0x7 << T12CLK_BIT_LSB_IDX); // f_T12 = f_CCU6 / prescaler
    CCU60_TCTR0.U |=  (0x2 << T12CLK_BIT_LSB_IDX); // f_CCU6 = 50 Mhz, prescaler = 50MHz / 2^2  = 12.5
    //CCU60_TCTR0.U |=  (0x1 << T12PRE_BIT_LSB_IDX); // f_T12 = 48,828 Hz <- 12.5MHz

    CCU60_TCTR0.U &= ~(0x1 << CTM_BIT_LSB_IDX); //T12 counter register (C/R) auto clear when Period Match (PM) occur

    CCU60_T12PR.U  = 125 -1; // PM interrupt freq. = f T12 / (T12PR + 1)
 //   CCU60_T12PR.U  = 24414 -1; // PM interrupt freq. = f T12 / (T12PR + 1)
    CCU60_TCTR4.U |= 0x1 << T12STR_BIT_LSB_IDX; //(load T12PR  from shadow register)

    CCU60_TCTR2.B.T12SSC = 0x1; // single shot control

    CCU60_T12.U = 0; // clear T12 counter register



    // CCU60 T12 PM interrupt setting
    CCU60_IEN.U |= 0x1 << ENT12PM_BIT_LSB_IDX; // enable T12 PM interrupt
    CCU60_INP.U &= ~(0x3 << INPT12_BIT_LSB_IDX); // service request output SR0 selected


    //SRC setting for CCU60

    SRC_CCU6_CCU60_SR0.U &= ~(0xFF << SRPN_BIT_LSB_IDX);
    SRC_CCU6_CCU60_SR0.U |= 0x0B << SRPN_BIT_LSB_IDX; //set priority 0x0B

    SRC_CCU6_CCU60_SR0.U &= ~(0x3 << TOS_BIT_LSB_IDX); // CPU0 service T12 PM interrupt
    SRC_CCU6_CCU60_SR0.U |= (0x1 << SRE_BIT_LSB_IDX); // SR0 enabled

}

void initCCU61(void){

    //PASSWORD ACCESS TO UNLOCK SCU_WDTCON0

    //PASSWORD TOGGLE, LOCK CLEAR AND END INIT SET
    SCU_WDTCPU0_CON0.U = ( (SCU_WDTCPU0_CON0.U ^ 0xFC) & ~(1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    // wait until unlocked
    while( ((SCU_WDTCPU0_CON0.U) & (1 << LCK_BIT_LSB_IDX) ) != 0 );


    // Modify Access to clear ENDINIT
    //PASSWORD TOGGLE, LOCK SET AND END INIT CLEAR

    SCU_WDTCPU0_CON0.U = ( (SCU_WDTCPU0_CON0.U ^ 0xFC) | (1 << LCK_BIT_LSB_IDX)) & ~(1 << ENDINIT_BIT_LSB_IDX);
    // wait until unlocked
    while( ((SCU_WDTCPU0_CON0.U) & (1 << LCK_BIT_LSB_IDX) ) == 0 );


    // THE ABOVE CODE IS DONE TO ACCESS CCU6 CLK REG

    CCU61_CLC.U &= ~(1 << DISR_BIT_LSB_IDX); // ENABLE CCU60 clock enable


    SCU_WDTCPU0_CON0.U = ( (SCU_WDTCPU0_CON0.U ^ 0xFC) & ~(1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    // wait until unlocked
    while( ((SCU_WDTCPU0_CON0.U) & (1 << LCK_BIT_LSB_IDX) ) != 0 );


    //SET ENDINIT BIT

    SCU_WDTCPU0_CON0.U = ( (SCU_WDTCPU0_CON0.U ^ 0xFC) | (1 << LCK_BIT_LSB_IDX))  | (1 << ENDINIT_BIT_LSB_IDX);
    // wait until locked
    while( ((SCU_WDTCPU0_CON0.U) & (1 << LCK_BIT_LSB_IDX) ) == 0 );


    // CCU60 T12 configuration
    while((CCU61_CLC.U & (1<<DISS_BIT_LSB_IDX)) != 0); // wait until CCU60 module enabled

    CCU61_TCTR0.U &= ~(0x7 << T12CLK_BIT_LSB_IDX); // f_T12 = f_CCU6 / prescaler
    CCU61_TCTR0.U |=  (0x2 << T12CLK_BIT_LSB_IDX); // f_CCU6 = 50 Mhz, prescaler = 50MHz / 2^2  = 12.5
    CCU61_TCTR0.U |=  (0x1 << T12PRE_BIT_LSB_IDX); // f_T12 = 48,828 Hz <- 12.5MHz

    //CCU61_TCTR0.U &= ~(0x1 << CTM_BIT_LSB_IDX); //T12 counter register (C/R) auto clear when Period Match (PM) occur

    CCU61_T12PR.U  = 100000 - 1; // PM interrupt freq. = f T12 / (T12PR + 1)
//    CCU60_T12PR.U  = 24414 -1; // PM interrupt freq. = f T12 / (T12PR + 1)
    CCU61_TCTR4.U |= 0x1 << T12STR_BIT_LSB_IDX; //(load T12PR  from shadow register)

    // CCU61_TCTR2.B.T12SSC = 0x1; // single shot control

    CCU61_T12.U = 0; // clear T12 counter register



    // CCU60 T12 PM interrupt setting
    CCU60_IEN.U |= 0x1 << ENT12PM_BIT_LSB_IDX; // enable T12 PM interrupt
    CCU60_INP.U &= ~(0x3 << INPT12_BIT_LSB_IDX); // service request output SR0 selected


    //SRC setting for CCU60

    SRC_CCU6_CCU60_SR0.U &= ~(0xFF << SRPN_BIT_LSB_IDX);
    SRC_CCU6_CCU60_SR0.U |= 0x0B << SRPN_BIT_LSB_IDX; //set priority 0x0B

    SRC_CCU6_CCU60_SR0.U &= ~(0x3 << TOS_BIT_LSB_IDX); // CPU0 service T12 PM interrupt
    SRC_CCU6_CCU60_SR0.U |= (0x1 << SRE_BIT_LSB_IDX); // SR0 enabled

}

void initUSonic(void){
    P02_IOCR4.U &= ~(0x1F << PC6_BIT_LSB_IDX);
    P00_IOCR4.U &= ~(0x1F << PC4_BIT_LSB_IDX);

    P00_IOCR4.U |= (0x01 << PC4_BIT_LSB_IDX); // echo
    P02_IOCR4.U |= (0x10 << PC6_BIT_LSB_IDX); // trigger

    P02_OUT.U &= ~(0x1 << P6_BIT_LSB_IDX); // trigger만 초기화
}


void usonicTrigger(void){ // trigger pulse 생성
    P02_OUT.U |= 0x1 << P6_BIT_LSB_IDX;
    range_valid_flag = 0;
    CCU60_TCTR4.U = 0x1 << T12RS_BIT_LSB_IDX;
}

void initBuzzer(void)
{
    P02_IOCR0.U &= ~(0X1F << PC3_BIT_LSB_IDX);
    P02_IOCR0.U |= (0X11 << PC3_BIT_LSB_IDX);

    // GTM clock configuration
    GTM_CMU_FXCLK_CTRL.U &= ~(0xF << FXCLK_SEL_BIT_LSB_IDX);    // input clock of CUM FXCLK --> CMU_GCLK_EN

    GTM_CMU_CLK_EN.U |= 0x2 << EN_FXCLK_BIT_LSB_IDX;

    // GTM TOM0 PWM configuration
    GTM_TOM0_TGC1_GLB_CTRL.U |= 0x2 << UPEN_CTRL3_BIT_LSB_IDX;      //  TOM channel 1 update enable
    GTM_TOM0_TGC1_ENDIS_CTRL.U |= 0x2 << ENDIS_CTRL3_BIT_LSB_IDX;   // enable channel 1 on update trigger
    GTM_TOM0_TGC1_OUTEN_CTRL.U |= 0x2 << OUTEN_CTRL3_BIT_LSB_IDX;   // enable channel 1 output on trigger

    GTM_TOM0_CH11_CTRL.U |= (0x1 << SL_BIT_LSB_IDX);            // high signal level for duty cycle
    GTM_TOM0_CH11_CTRL.U |= (0x1 << CLK_SRC_SR_BIT_LSB_IDX);    // clock source --> MU FXCLK(1) = 6250kHz
    GTM_TOM0_CH11_CTRL.U &= ~(0x1 << OSM_BIT_LSB_IDX);          // continuous mode enable
    GTM_TOM0_CH11_CTRL.U &= ~(0x1 << TRIGOUT_BIT_LSB_IDX);       // TRIG[x] = TRIG[x-1]

    GTM_TOM0_CH11_SR0.U = 12500 - 1;                             // PWM freq. = 6250kHz / 12500 = 500 Hz
    GTM_TOM0_CH11_SR1.U = 0;                              // duty cycle = 1250 / 12500 = 10 % (temporary)

    GTM_TOUTSEL0.U &= ~(0x3 << SEL1_BIT_LSB_IDX);
}

void initMotor(void)  // TOUT105, TOM2_11, TGC 1, CTRL 3
{
    // Direction: P10.2
    // Speed(PWM): P10.3
    // Break: P02.6
    P10_IOCR0.U &= ~(0x1F << PC1_BIT_LSB_IDX);
    P02_IOCR0.U &= ~(0x1F << PC1_BIT_LSB_IDX);
    P02_IOCR4.U &= ~(0x1F << PC7_BIT_LSB_IDX);

    P10_IOCR0.U |= (0x10 << PC1_BIT_LSB_IDX);
    P02_IOCR0.U |= (0x11 << PC1_BIT_LSB_IDX);
    P02_IOCR4.U |= (0x10 << PC7_BIT_LSB_IDX);
}

void activateBuzzer(int duty)
{
    GTM_TOM0_CH11_SR0.B.SR0 = 6250000 / duty;
    GTM_TOM0_CH11_SR1.B.SR1 = 3125000 / duty;
}

void deactivateBuzzer(void)
{
    GTM_TOM0_CH11_SR0.B.SR0 = 0;
    GTM_TOM0_CH11_SR1.B.SR1 = 0;
}

void GoMelody()
{
    const int interval = 20000000;
    const int size_Melody = sizeof(Melody) / sizeof(int);
    if(SongCount >= size_Melody)
        SongCount = 0;

    activateBuzzer(Doremi[Melody[SongCount++]]);
    for(int i = 0; i < interval; ++i);
    if(SongCount == 7 || SongCount == 12 || SongCount == 19)
        for(int i = 0; i < interval; ++i);

    if(SongCount == size_Melody)
        for(int i = 0; i < interval*2; ++i);
    deactivateBuzzer();
}

