///////////////////////////////////////////////////////////////////////////////
//
// IAR ANSI C/C++ Compiler V7.20.5.7591/W32 for ARM       07/Feb/2019  15:37:24
// Copyright 1999-2014 IAR Systems AB.
//
//    Cpu mode     =  thumb
//    Endian       =  little
//    Source file  =  D:\2019.IAR\lib\LPLD\HW\HW_CAN.c
//    Command line =  
//        D:\2019.IAR\lib\LPLD\HW\HW_CAN.c -D LPLD_K60 -lCN
//        "D:\2019.IAR\project\001-My NaoKe\iar\FLASH\List\" -lB
//        "D:\2019.IAR\project\001-My NaoKe\iar\FLASH\List\" -o
//        "D:\2019.IAR\project\001-My NaoKe\iar\FLASH\Obj\" --no_cse
//        --no_unroll --no_inline --no_code_motion --no_tbaa --no_clustering
//        --no_scheduling --debug --endian=little --cpu=Cortex-M4 -e --fpu=None
//        --dlib_config "D:\Embedded Workbench
//        7.0\arm\INC\c\DLib_Config_Normal.h" -I "D:\2019.IAR\project\001-My
//        NaoKe\iar\..\app\" -I "D:\2019.IAR\project\001-My
//        NaoKe\iar\..\..\..\lib\CPU\" -I "D:\2019.IAR\project\001-My
//        NaoKe\iar\..\..\..\lib\common\" -I "D:\2019.IAR\project\001-My
//        NaoKe\iar\..\..\..\lib\LPLD\" -I "D:\2019.IAR\project\001-My
//        NaoKe\iar\..\..\..\lib\LPLD\HW\" -I "D:\2019.IAR\project\001-My
//        NaoKe\iar\..\..\..\lib\LPLD\DEV\" -I "D:\2019.IAR\project\001-My
//        NaoKe\iar\..\..\..\lib\uCOS-II\Ports\" -I "D:\2019.IAR\project\001-My
//        NaoKe\iar\..\..\..\lib\uCOS-II\Source\" -I
//        "D:\2019.IAR\project\001-My NaoKe\iar\..\..\..\lib\FatFs\" -I
//        "D:\2019.IAR\project\001-My NaoKe\iar\..\..\..\lib\FatFs\option\" -I
//        "D:\2019.IAR\project\001-My NaoKe\iar\..\..\..\lib\USB\common\" -I
//        "D:\2019.IAR\project\001-My NaoKe\iar\..\..\..\lib\USB\driver\" -I
//        "D:\2019.IAR\project\001-My NaoKe\iar\..\..\..\lib\USB\descriptor\"
//        -I "D:\2019.IAR\project\001-My NaoKe\iar\..\..\..\lib\USB\class\" -Ol
//        -I "D:\Embedded Workbench 7.0\arm\CMSIS\Include\" -D ARM_MATH_CM4
//    List file    =  D:\2019.IAR\project\001-My NaoKe\iar\FLASH\List\HW_CAN.s
//
///////////////////////////////////////////////////////////////////////////////

        #define SHT_PROGBITS 0x1

        EXTERN assert_failed
        EXTERN g_bus_clock
        EXTERN memcpy

        PUBLIC CAN0_BUS_OFF_IRQHandler
        PUBLIC CAN0_ERR_IRQHandler
        PUBLIC CAN0_IMEU_IRQHandler
        PUBLIC CAN0_ISR
        PUBLIC CAN0_LR_IRQHandler
        PUBLIC CAN0_MESS_IRQHandler
        PUBLIC CAN0_RW_IRQHandler
        PUBLIC CAN0_TW_IRQHandler
        PUBLIC CAN0_WAKE_UP_IRQHandler
        PUBLIC CAN1_BUS_OFF_IRQHandler
        PUBLIC CAN1_ERR_IRQHandler
        PUBLIC CAN1_IMEU_IRQHandler
        PUBLIC CAN1_ISR
        PUBLIC CAN1_LR_IRQHandler
        PUBLIC CAN1_MESS_IRQHandler
        PUBLIC CAN1_RW_IRQHandler
        PUBLIC CAN1_TW_IRQHandler
        PUBLIC CAN1_WAKE_UP_IRQHandler
        PUBLIC LPLD_CAN_Deinit
        PUBLIC LPLD_CAN_DisableIrq
        PUBLIC LPLD_CAN_EnableIrq
        PUBLIC LPLD_CAN_Init
        PUBLIC LPLD_CAN_InitMessageObject
        PUBLIC LPLD_CAN_ReceivedMessage
        PUBLIC LPLD_CAN_TransmitMessage
        PUBLIC flexcan_msgobj_param_t

// D:\2019.IAR\lib\LPLD\HW\HW_CAN.c
//    1 /**
//    2  * @file HW_CAN.c
//    3  * @version 3.0[By LPLD]
//    4  * @date 2013-06-18
//    5  * @brief CAN�ײ�ģ����غ���
//    6  *
//    7  * ���Ľ���:�������޸�
//    8  *
//    9  * ��Ȩ����:�����������µ��Ӽ������޹�˾
//   10  * http://www.lpld.cn
//   11  * mail:support@lpld.cn
//   12  *
//   13  * @par
//   14  * ����������������[LPLD]������ά������������ʹ���߿���Դ���롣
//   15  * �����߿���������ʹ�û��Դ���롣�����μ�����ע��Ӧ���Ա�����
//   16  * ���ø��Ļ�ɾ��ԭ��Ȩ���������������ο����߿��Լ�ע���ΰ�Ȩ�����ߡ�
//   17  * ��Ӧ�����ش�Э��Ļ����ϣ�����Դ���롢���ó��۴��뱾��
//   18  * �������²���������ʹ�ñ��������������κ��¹ʡ��������λ���ز���Ӱ�졣
//   19  * ����������������͡�˵��������ľ���ԭ�����ܡ�ʵ�ַ�����
//   20  * ������������[LPLD]��Ȩ�������߲��ý�������������ҵ��Ʒ��
//   21  */
//   22 
//   23 #include "common.h"

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
// static __interwork __softfp void NVIC_EnableIRQ(IRQn_Type)
NVIC_EnableIRQ:
        MOVS     R1,#+1
        ANDS     R2,R0,#0x1F
        LSLS     R1,R1,R2
        LDR.W    R2,??DataTable12  ;; 0xe000e100
        SXTB     R0,R0            ;; SignExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+5
        STR      R1,[R2, R0, LSL #+2]
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
// static __interwork __softfp void NVIC_DisableIRQ(IRQn_Type)
NVIC_DisableIRQ:
        MOVS     R1,#+1
        ANDS     R2,R0,#0x1F
        LSLS     R1,R1,R2
        LDR.W    R2,??DataTable12_1  ;; 0xe000e180
        SXTB     R0,R0            ;; SignExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+5
        STR      R1,[R2, R0, LSL #+2]
        BX       LR               ;; return
//   24 #include "HW_CAN.h"
//   25 
//   26 //FlexCANģ�����ò����ʺ���
//   27 static void LPLD_CAN_SetBaudRate(CAN_Type *,uint32 );
//   28 //FlexCAN ����жϱ�־λ����
//   29 static void LPLD_CAN_Interrupt_ClearPending(CAN_Type *, uint8 );
//   30 //FlexCAN ����жϱ�־λ����
//   31 static uint8 LPLD_CAN_Interrupt_GetFlag(CAN_Type *, uint8 );
//   32 //FlexCAN ����Msg����������
//   33 static uint16 LPLD_CAN_UnlockMsg(CAN_Type *);
//   34 //FlexCAN ʹ��Msg�������жϺ���
//   35 static void LPLD_CAN_EnableMsgInterrupt(CAN_Type *, uint8 );
//   36 //FlexCAN ����Msg��������CODE
//   37 static void LPLD_CAN_SetMsgCode(CAN_Type *, uint8 , uint8 );
//   38 //FlexCAN ���Msg��������CODE
//   39 static uint32 LPLD_CAN_GetMsgCode(CAN_Type *, uint8 );
//   40 //FlexCAN ���Msg��������TIMESTAMP
//   41 static uint16 LPLD_CAN_GetMsgTimeStamp(CAN_Type *, uint8 );
//   42 //FlexCAN ����Msg��������ID
//   43 void LPLD_CAN_SetMsgID(CAN_Type *, uint8 ,uint32 );
//   44 //FlexCAN ���Msg��������ID
//   45 static uint32 LPLD_CAN_GetMsgID(CAN_Type *, uint8 );
//   46 //FlexCAN ����Msg�������ĳ���
//   47 static void LPLD_CAN_SetMsgLength(CAN_Type *, uint8 ,uint8 );
//   48 //FlexCAN ���Msg�������ĳ���
//   49 uint8 LPLD_CAN_GetMsgLength(CAN_Type *canx, uint8 MSG_NUM);
//   50 //FlexCAN ���Msg������������
//   51 static void LPLD_CAN_GetData(CAN_Type *canx, uint8 , uint8 ,  uint8 *);
//   52 //FlexCAN дMsg������������
//   53 static void LPLD_CAN_WriteData(CAN_Type *, uint8 , uint8, uint8 * );
//   54 //FlexCAN �����жϺ���
//   55 void LPLD_CAN_Transmit_Interrupt(CAN_Type *, uint8 );
//   56 //FlexCAN �����жϺ���
//   57 static void LPLD_CAN_Receive_Interrupt(CAN_Type *, uint8 );
//   58 //FlexCAN CAN0���жϴ�����
//   59 static void LPLD_CAN_CAN0_Interrupt_Handler(CAN_MSGOBJ_InitTypeDef *);
//   60 //FlexCAN CAN1���жϴ�����
//   61 static void LPLD_CAN_CAN1_Interrupt_Handler(CAN_MSGOBJ_InitTypeDef *);
//   62 
//   63 //����16��Msg�Ľṹ��

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   64 CAN_MSGOBJ_InitTypeDef  flexcan_msgobj_param_t[MSG_MAX_NO] = {0};
flexcan_msgobj_param_t:
        DS8 256
//   65 //����CANx Msg���жϻص�����

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   66 CAN_ISR_CALLBACK CAN0_ISR[MSG_MAX_NO];
CAN0_ISR:
        DS8 64

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   67 CAN_ISR_CALLBACK CAN1_ISR[MSG_MAX_NO];
CAN1_ISR:
        DS8 64
//   68 
//   69 /*
//   70  * LPLD_CAN_Init
//   71  * �ڸú�����ʹ��CANx������CANx�Ĳ����ʣ�ѡ����Ӧ�Ĺ���ģʽ
//   72  * ͨ��CAN_InitTypeDef�ṹ���CAN���г�ʼ����
//   73  * 
//   74  * ����:
//   75  *    CAN_InitTypeDef--can_init_structure
//   76  *                     ���嶨���HW_CAN.h can_init_structure
//   77  *
//   78  * ���:
//   79  *    ��
//   80  *
//   81  */

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//   82 void LPLD_CAN_Init(CAN_InitTypeDef can_init_structure)
//   83 {
LPLD_CAN_Init:
        PUSH     {R0-R2,R4-R8,LR}
        SUB      SP,SP,#+4
//   84   uint8 i;
//   85   CAN_Type *canx = can_init_structure.CAN_Canx;
        LDR      R4,[SP, #+4]
//   86   uint32 baud = can_init_structure.CAN_BaudRate;
        LDR      R5,[SP, #+8]
//   87   uint8  mask_mode = can_init_structure.CAN_RxMaskMode;
        LDRB     R6,[SP, #+12]
//   88   PortPinsEnum_Type tx_pin = can_init_structure.CAN_TxPin;
        LDRB     R7,[SP, #+13]
//   89   PortPinsEnum_Type rx_pin = can_init_structure.CAN_RxPin;
        LDRB     R8,[SP, #+14]
//   90 
//   91   ASSERT(mask_mode <= CAN_MSGOBJ_GLOBAL_MASKING);
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+2
        BLT.N    ??LPLD_CAN_Init_0
        MOVS     R1,#+91
        LDR.W    R0,??DataTable12_2
        BL       assert_failed
//   92   ASSERT(baud <= CAN_BAUD_RATE_1MBPS);
??LPLD_CAN_Init_0:
        LDR.W    R0,??DataTable12_3  ;; 0xf4241
        CMP      R5,R0
        BCC.N    ??LPLD_CAN_Init_1
        MOVS     R1,#+92
        LDR.W    R0,??DataTable12_2
        BL       assert_failed
//   93     
//   94   OSC->CR |= OSC_CR_ERCLKEN_MASK | OSC_CR_EREFSTEN_MASK;
??LPLD_CAN_Init_1:
        LDR.W    R0,??DataTable12_4  ;; 0x40065000
        LDRB     R0,[R0, #+0]
        ORRS     R0,R0,#0xA0
        LDR.W    R1,??DataTable12_4  ;; 0x40065000
        STRB     R0,[R1, #+0]
//   95   
//   96   if(canx == CAN0)
        LDR.W    R0,??DataTable12_5  ;; 0x40024000
        CMP      R4,R0
        BNE.N    ??LPLD_CAN_Init_2
//   97     SIM->SCGC6 |= SIM_SCGC6_FLEXCAN0_MASK;
        LDR.W    R0,??DataTable12_6  ;; 0x4004803c
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x10
        LDR.W    R1,??DataTable12_6  ;; 0x4004803c
        STR      R0,[R1, #+0]
        B.N      ??LPLD_CAN_Init_3
//   98   else
//   99     SIM->SCGC3 |= SIM_SCGC3_FLEXCAN1_MASK;
??LPLD_CAN_Init_2:
        LDR.W    R0,??DataTable12_7  ;; 0x40048030
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x10
        LDR.W    R1,??DataTable12_7  ;; 0x40048030
        STR      R0,[R1, #+0]
//  100   
//  101   if(canx == CAN0)
??LPLD_CAN_Init_3:
        LDR.W    R0,??DataTable12_5  ;; 0x40024000
        CMP      R4,R0
        BNE.N    ??LPLD_CAN_Init_4
//  102   {
//  103     if(tx_pin == PTB18)//CAN0_TX
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,#+50
        BNE.N    ??LPLD_CAN_Init_5
//  104     {
//  105       PORTB->PCR[18] = PORT_PCR_MUX(2)| PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; 
        MOVW     R0,#+515
        LDR.W    R1,??DataTable12_8  ;; 0x4004a048
        STR      R0,[R1, #+0]
        B.N      ??LPLD_CAN_Init_6
//  106     }
//  107     else
//  108     {
//  109       PORTA->PCR[12] = PORT_PCR_MUX(2); 
??LPLD_CAN_Init_5:
        MOV      R0,#+512
        LDR.W    R1,??DataTable12_9  ;; 0x40049030
        STR      R0,[R1, #+0]
//  110     }
//  111 
//  112     if(rx_pin == PTB19)//CAN0_RX
??LPLD_CAN_Init_6:
        UXTB     R8,R8            ;; ZeroExt  R8,R8,#+24,#+24
        CMP      R8,#+51
        BNE.N    ??LPLD_CAN_Init_7
//  113     {
//  114       PORTB->PCR[19] = PORT_PCR_MUX(2)| PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; 
        MOVW     R0,#+515
        LDR.W    R1,??DataTable12_10  ;; 0x4004a04c
        STR      R0,[R1, #+0]
        B.N      ??LPLD_CAN_Init_8
//  115     }
//  116     else
//  117     {
//  118       PORTA->PCR[13] = PORT_PCR_MUX(2); 
??LPLD_CAN_Init_7:
        MOV      R0,#+512
        LDR.W    R1,??DataTable12_11  ;; 0x40049034
        STR      R0,[R1, #+0]
        B.N      ??LPLD_CAN_Init_8
//  119     }
//  120   }
//  121   else
//  122   {
//  123     if(tx_pin == PTC17)//CAN1_TX
??LPLD_CAN_Init_4:
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,#+77
        BNE.N    ??LPLD_CAN_Init_9
//  124     {
//  125       PORTC->PCR[17] = PORT_PCR_MUX(2)| PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; 
        MOVW     R0,#+515
        LDR.W    R1,??DataTable12_12  ;; 0x4004b044
        STR      R0,[R1, #+0]
        B.N      ??LPLD_CAN_Init_10
//  126     }
//  127     else
//  128     {
//  129       PORTE->PCR[24] = PORT_PCR_MUX(2)| PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
??LPLD_CAN_Init_9:
        MOVW     R0,#+515
        LDR.W    R1,??DataTable12_13  ;; 0x4004d060
        STR      R0,[R1, #+0]
//  130     }
//  131 
//  132     if(rx_pin == PTC16)//CAN1_RX
??LPLD_CAN_Init_10:
        UXTB     R8,R8            ;; ZeroExt  R8,R8,#+24,#+24
        CMP      R8,#+76
        BNE.N    ??LPLD_CAN_Init_11
//  133     {
//  134       PORTC->PCR[16] = PORT_PCR_MUX(2)| PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; 
        MOVW     R0,#+515
        LDR.W    R1,??DataTable12_14  ;; 0x4004b040
        STR      R0,[R1, #+0]
        B.N      ??LPLD_CAN_Init_8
//  135     }
//  136     else
//  137     {
//  138       PORTE->PCR[25] = PORT_PCR_MUX(2)| PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
??LPLD_CAN_Init_11:
        MOVW     R0,#+515
        LDR.W    R1,??DataTable12_15  ;; 0x4004d064
        STR      R0,[R1, #+0]
//  139     }
//  140   } 	 
//  141   //��ֹCAN����
//  142   canx->MCR   |= CAN_MCR_MDIS_MASK;
??LPLD_CAN_Init_8:
        LDR      R0,[R4, #+0]
        ORRS     R0,R0,#0x80000000
        STR      R0,[R4, #+0]
//  143   //ֻ����LPM_ACK = 1����²ſ���ѡ����Դ
//  144   canx->CTRL1 |= CAN_CTRL1_CLKSRC_MASK;  //ѡ��peripheral clock��ΪCAN�����ʱ��Դ
        LDR      R0,[R4, #+4]
        ORRS     R0,R0,#0x2000
        STR      R0,[R4, #+4]
//  145                                          //���ô�λ������CANֹͣģʽ��
//  146   //��ʱ�ӳ�ʼ����Ϻ�CAN����ʹ����Ϻ�
//  147   //��Ƭ���Զ����붳��ģʽ
//  148   //ֻ���ڶ���ģʽ�²������ô����CAN���߼Ĵ���
//  149   //ʹ�ܶ���ģʽ
//  150   canx->MCR |= CAN_MCR_HALT_MASK; 
        LDR      R0,[R4, #+0]
        ORRS     R0,R0,#0x10000000
        STR      R0,[R4, #+0]
//  151   canx->MCR |= CAN_MCR_FRZ_MASK;
        LDR      R0,[R4, #+0]
        ORRS     R0,R0,#0x40000000
        STR      R0,[R4, #+0]
//  152   canx->MCR &= ~CAN_MCR_MDIS_MASK; 
        LDR      R0,[R4, #+0]
        LSLS     R0,R0,#+1        ;; ZeroExtS R0,R0,#+1,#+1
        LSRS     R0,R0,#+1
        STR      R0,[R4, #+0]
//  153   
//  154   while(!(canx->MCR & CAN_MCR_LPMACK_MASK));	
??LPLD_CAN_Init_12:
        LDR      R0,[R4, #+0]
        LSLS     R0,R0,#+11
        BPL.N    ??LPLD_CAN_Init_12
//  155 
//  156   //���������λ
//  157   canx->MCR ^= CAN_MCR_SOFTRST_MASK;
        LDR      R0,[R4, #+0]
        EORS     R0,R0,#0x2000000
        STR      R0,[R4, #+0]
//  158   while(canx->MCR & CAN_MCR_SOFTRST_MASK);
??LPLD_CAN_Init_13:
        LDR      R0,[R4, #+0]
        LSLS     R0,R0,#+6
        BMI.N    ??LPLD_CAN_Init_13
//  159             
//  160   //�ȴ����붳��ģʽ 
//  161   while(!(canx->MCR & CAN_MCR_FRZACK_MASK));
??LPLD_CAN_Init_14:
        LDR      R0,[R4, #+0]
        LSLS     R0,R0,#+7
        BPL.N    ??LPLD_CAN_Init_14
//  162   
//  163   if(mask_mode == CAN_MSGOBJ_GLOBAL_MASKING)
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+1
        BNE.N    ??LPLD_CAN_Init_15
//  164   {
//  165     canx->MCR &= ~CAN_MCR_IRMQ_MASK;   //ʹ��ȫ��ƥ��Ĵ�����ֹ����ƥ�� 
        LDR      R0,[R4, #+0]
        BICS     R0,R0,#0x10000
        STR      R0,[R4, #+0]
        B.N      ??LPLD_CAN_Init_16
//  166   }
//  167   else
//  168   {
//  169     canx->MCR |= CAN_MCR_IRMQ_MASK;   //ʹ��Msg����ƥ�� 
??LPLD_CAN_Init_15:
        LDR      R0,[R4, #+0]
        ORRS     R0,R0,#0x10000
        STR      R0,[R4, #+0]
//  170   }
//  171   
//  172   canx->MCR |= CAN_MCR_SUPV_MASK ;   //���óɹ���ģʽ 
??LPLD_CAN_Init_16:
        LDR      R0,[R4, #+0]
        ORRS     R0,R0,#0x800000
        STR      R0,[R4, #+0]
//  173   canx->MCR |= CAN_MCR_SRXDIS_MASK ; //��ֹ���ҽ���
        LDR      R0,[R4, #+0]
        ORRS     R0,R0,#0x20000
        STR      R0,[R4, #+0]
//  174   canx->MCR &= ~CAN_MCR_WRNEN_MASK;  //�����������ж�
        LDR      R0,[R4, #+0]
        BICS     R0,R0,#0x200000
        STR      R0,[R4, #+0]
//  175   canx->MCR &= ~CAN_MCR_RFEN_MASK ;  //��ֹ����FIFO
        LDR      R0,[R4, #+0]
        BICS     R0,R0,#0x20000000
        STR      R0,[R4, #+0]
//  176   canx->MCR &= ~CAN_MCR_AEN_MASK;    //��ֹAbort
        LDR      R0,[R4, #+0]
        BICS     R0,R0,#0x1000
        STR      R0,[R4, #+0]
//  177   canx->MCR &= ~CAN_MCR_LPRIOEN_MASK;//��ֹ��������
        LDR      R0,[R4, #+0]
        BICS     R0,R0,#0x2000
        STR      R0,[R4, #+0]
//  178   
//  179   //������صļĴ���
//  180   canx->CTRL2 |= CAN_CTRL2_EACEN_MASK; //�����������IDEƥ�䣬RTR��ƥ��
        LDR      R0,[R4, #+52]
        ORRS     R0,R0,#0x10000
        STR      R0,[R4, #+52]
//  181   canx->CTRL2 &= ~CAN_CTRL2_RRS_MASK;  //���Զ�����Զ������֡����
        LDR      R0,[R4, #+52]
        BICS     R0,R0,#0x20000
        STR      R0,[R4, #+52]
//  182   canx->CTRL2 |= CAN_CTRL2_MRP_MASK;   //ID���ȴ�������ƥ��
        LDR      R0,[R4, #+52]
        ORRS     R0,R0,#0x40000
        STR      R0,[R4, #+52]
//  183 
//  184   //canx->CTRL1 |= CAN_CTRL1_LBUF_MASK;  //���͵�ʱ��ӵ�Msg��ʼ
//  185   canx->CTRL1 &= ~CAN_CTRL1_LBUF_MASK;   //���͵�ʱ��ӵ����ȼ�����
        LDR      R0,[R4, #+4]
        BICS     R0,R0,#0x10
        STR      R0,[R4, #+4]
//  186                              
//  187   //canx->CTRL1 |= CAN_CTRL1_LPB_MASK;  //loop ģʽ,���ڲ���
//  188   canx->CTRL1 &= ~CAN_CTRL1_LPB_MASK;   //ʹ������ģʽ
        LDR      R0,[R4, #+4]
        BICS     R0,R0,#0x1000
        STR      R0,[R4, #+4]
//  189   //����CAN����ͨ�ŵĲ�����
//  190   LPLD_CAN_SetBaudRate(canx,baud);
        MOVS     R1,R5
        MOVS     R0,R4
        BL       LPLD_CAN_SetBaudRate
//  191   //���CAN���ɼ����� 
//  192   canx->TIMER = 0x0000; 
        MOVS     R0,#+0
        STR      R0,[R4, #+8]
//  193   //��MB_MAX�����仺����������0
//  194   for(i = 0;i < MSG_MAX_NO; i++)
        MOVS     R0,#+0
        B.N      ??LPLD_CAN_Init_17
//  195   {
//  196     canx->MB[i].CS    = 0x00000000;
??LPLD_CAN_Init_18:
        MOVS     R1,#+0
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        ADDS     R2,R4,R0, LSL #+4
        STR      R1,[R2, #+128]
//  197     canx->MB[i].ID    = 0x00000000;
        MOVS     R1,#+0
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        ADDS     R2,R4,R0, LSL #+4
        STR      R1,[R2, #+132]
//  198     canx->MB[i].WORD0 = 0x00000000;
        MOVS     R1,#+0
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        ADDS     R2,R4,R0, LSL #+4
        STR      R1,[R2, #+136]
//  199     canx->MB[i].WORD1 = 0x00000000;        
        MOVS     R1,#+0
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        ADDS     R2,R4,R0, LSL #+4
        STR      R1,[R2, #+140]
//  200   }
        ADDS     R0,R0,#+1
??LPLD_CAN_Init_17:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+16
        BLT.N    ??LPLD_CAN_Init_18
//  201   //���64������ı�־λ
//  202   canx->IFLAG1 = 0xFFFFFFFF;
        MOVS     R0,#-1
        STR      R0,[R4, #+48]
//  203   canx->IFLAG2 = 0xFFFFFFFF;
        MOVS     R0,#-1
        STR      R0,[R4, #+44]
//  204   //��ֹ�����ж�
//  205   canx->IMASK1 = 0x00000000;
        MOVS     R0,#+0
        STR      R0,[R4, #+40]
//  206   canx->IMASK2 = 0x00000000;
        MOVS     R0,#+0
        STR      R0,[R4, #+36]
//  207    /*
//  208   ���MCR��IRMQΪ1����ʾÿ�����䶼���Ե�������IDƥ��
//  209   ע�⣺�����low cost MCUs�������õ�MCU����û�е���ƥ������ܡ�
//  210           1\IDƥ���ѡ��free to receive״̬��������Ϊʤ�ߣ�
//  211           ��ʤ��������Դӽ��ջ������ڽ����յ�֡ Move in ��MB�У�
//  212           2\�������ID����ͬ�����䣬�����ȴ���ŵ͵��������ƥ�䣬���
//  213           �����������non free to receive״̬����ôƥ����̽������
//  214           ��һ������ֱ���ҵ�free to receive״̬��ID��ͬ�����䣻
//  215           3\���ID����ͬ�����䶼����non free to receive״̬����ôƥ�����
//  216           ��ֹͣ�����һ������ͬID������λ�ã���֡Move in ��MB�в��ҽ�����
//  217           ���״̬���ó�OVERRUN��
//  218           RXIMR[i]�洢�ڵ�Ƭ����RAM�У�ֻ����CAN����ģʽ�¿�д��
//  219           ��ʱCANx_RXMGMASK��CANx_RX14MASK��CANx_RX15MASK��������
//  220   */
//  221 
//  222   for(i = 0;i < MSG_MAX_NO; i++)
        MOVS     R0,#+0
        B.N      ??LPLD_CAN_Init_19
//  223   {
//  224     canx->RXIMR[i] = 0x1FFFFFFF;//��������29λȫ������
??LPLD_CAN_Init_20:
        MVNS     R1,#-536870912
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        ADDS     R2,R4,R0, LSL #+2
        STR      R1,[R2, #+2176]
//  225   }
        ADDS     R0,R0,#+1
??LPLD_CAN_Init_19:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+16
        BLT.N    ??LPLD_CAN_Init_20
//  226  /*
//  227   ���MCR��IRMQΪ0����ʾʹ��ȫ��ƥ��Ĵ�������ƥ�䣬
//  228           ��ʱCANx_RXMGMASK��CANx_RX14MASK��CANx_RX15MASK�����ã�
//  229           CANx_RXMGMASK�����CANx_RX14MASK��CANx_RX15MASK���������Msg��
//  230           IDƥ����̻�ֹͣ���ҵ��ĵ�һ��ID��ͬ������λ�ã����ܸ������Ƿ���
//  231           free to receive״̬��
//  232   */
//  233 
//  234   canx->RXMGMASK = 0x1FFFFFFF; //29λIDȫ��ƥ��            
        MVNS     R0,#-536870912
        STR      R0,[R4, #+16]
//  235   canx->RX14MASK = 0x1FFFFFFF;
        MVNS     R0,#-536870912
        STR      R0,[R4, #+20]
//  236   canx->RX15MASK = 0x1FFFFFFF; 
        MVNS     R0,#-536870912
        STR      R0,[R4, #+24]
//  237 
//  238   canx->MCR &= ~CAN_MCR_FRZ_MASK; //�˳�����ģʽ
        LDR      R0,[R4, #+0]
        BICS     R0,R0,#0x40000000
        STR      R0,[R4, #+0]
//  239   //�ȴ�ֱ���˳�����ģʽ
//  240   while( canx->MCR & CAN_MCR_FRZACK_MASK); 
??LPLD_CAN_Init_21:
        LDR      R0,[R4, #+0]
        LSLS     R0,R0,#+7
        BMI.N    ??LPLD_CAN_Init_21
//  241   //ֻ���ڶ���ģʽ�²������ã��������Ƴ�����ģʽ
//  242   canx->MCR &= ~(CAN_MCR_HALT_MASK); 
        LDR      R0,[R4, #+0]
        BICS     R0,R0,#0x10000000
        STR      R0,[R4, #+0]
//  243   //�ȵ����ڶ���ģʽ������ģʽ����ֹͣģʽ
//  244   while( canx->MCR & CAN_MCR_NOTRDY_MASK);
??LPLD_CAN_Init_22:
        LDR      R0,[R4, #+0]
        LSLS     R0,R0,#+4
        BMI.N    ??LPLD_CAN_Init_22
//  245   //ʹ��CAN����
//  246   canx->MCR &= ~CAN_MCR_MDIS_MASK;
        LDR      R0,[R4, #+0]
        LSLS     R0,R0,#+1        ;; ZeroExtS R0,R0,#+1,#+1
        LSRS     R0,R0,#+1
        STR      R0,[R4, #+0]
//  247 }
        POP      {R0-R8,PC}       ;; return
//  248 /*
//  249  * LPLD_CAN_SetBaudRate
//  250  * ����CAN������
//  251  * ����:
//  252  *    CAN_Type *canx
//  253  *      |___CAN0 --CAN0��ģ��
//  254  *      |___CAN1 --CAN0��ģ��
//  255  *    baud ������
//  256  *      |__CAN_BAUD_RATE_50KBPS  --������50KBPS
//  257  *      |__CAN_BAUD_RATE_100KBPS --������100KBPS
//  258  *      |__CAN_BAUD_RATE_250KBPS --������250KBPS
//  259  *      |__CAN_BAUD_RATE_500KBPS --������500KBPS
//  260  *      |__CAN_BAUD_RATE_1MBPS   --������1MBPS
//  261  *  �����
//  262  *    ��
//  263 */

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//  264 static void LPLD_CAN_SetBaudRate(CAN_Type *canx,uint32 baud)
//  265 {
LPLD_CAN_SetBaudRate:
        PUSH     {R3-R5,LR}
        MOVS     R4,R0
        MOVS     R5,R1
//  266   uint8 prescale;
//  267   //1��CAN����λ��ʱ�����ӣ�Time Quanta = SYNC_SEG + (PROP_SEG + PSEG1 + 2) + (PSEG2 + 1)��
//  268   //SYNC_SEG =  1 ���̶�ֵ��
//  269   ASSERT(baud <= CAN_BAUD_RATE_1MBPS);
        LDR.W    R0,??DataTable12_3  ;; 0xf4241
        CMP      R5,R0
        BCC.N    ??LPLD_CAN_SetBaudRate_0
        MOVW     R1,#+269
        LDR.W    R0,??DataTable12_2
        BL       assert_failed
//  270   switch(baud)
??LPLD_CAN_SetBaudRate_0:
        MOVS     R0,R5
        MOVW     R1,#+50000
        CMP      R0,R1
        BEQ.N    ??LPLD_CAN_SetBaudRate_1
        LDR.W    R1,??DataTable12_16  ;; 0x186a0
        CMP      R0,R1
        BEQ.N    ??LPLD_CAN_SetBaudRate_1
        LDR.W    R1,??DataTable12_17  ;; 0x3d090
        CMP      R0,R1
        BEQ.N    ??LPLD_CAN_SetBaudRate_1
        LDR.W    R1,??DataTable12_18  ;; 0x7a120
        CMP      R0,R1
        BEQ.N    ??LPLD_CAN_SetBaudRate_1
        LDR.W    R1,??DataTable12_19  ;; 0xf4240
        CMP      R0,R1
        BNE.N    ??LPLD_CAN_SetBaudRate_2
//  271   {
//  272     case CAN_BAUD_RATE_1MBPS:
//  273       {
//  274         prescale = CAN_GET_PRESCALE(g_bus_clock,baud,10); 
??LPLD_CAN_SetBaudRate_3:
        LDR.W    R0,??DataTable12_20
        LDR      R0,[R0, #+0]
        MOVS     R1,#+10
        MUL      R1,R1,R5
        UDIV     R0,R0,R1
        SUBS     R0,R0,#+1
//  275         //����CAN����ͨ�ŵĲ�����
//  276         canx->CTRL1 |= CAN_CTRL1_RJW(2) 
//  277                  | CAN_CTRL1_PROPSEG(2)  
//  278                  | CAN_CTRL1_PSEG1(2) 
//  279                  | CAN_CTRL1_PSEG2(2)
//  280                  | CAN_CTRL1_PRESDIV(prescale); 
        LDR      R1,[R4, #+4]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSLS     R0,R0,#+24
        ORR      R0,R0,#0x920000
        ORRS     R0,R0,#0x2
        ORRS     R0,R0,R1
        STR      R0,[R4, #+4]
//  281         break;
        B.N      ??LPLD_CAN_SetBaudRate_4
//  282       }
//  283     case CAN_BAUD_RATE_500KBPS:
//  284     case CAN_BAUD_RATE_250KBPS:
//  285     case CAN_BAUD_RATE_100KBPS:
//  286     case CAN_BAUD_RATE_50KBPS:
//  287       {
//  288         prescale = CAN_GET_PRESCALE(g_bus_clock,baud,20); 
??LPLD_CAN_SetBaudRate_1:
        LDR.W    R0,??DataTable12_20
        LDR      R0,[R0, #+0]
        MOVS     R1,#+20
        MUL      R1,R1,R5
        UDIV     R0,R0,R1
        SUBS     R0,R0,#+1
//  289         //����CAN����ͨ�ŵĲ�����
//  290         canx->CTRL1 |= CAN_CTRL1_RJW(2) 
//  291                  | CAN_CTRL1_PROPSEG(6)  
//  292                  | CAN_CTRL1_PSEG1(6) 
//  293                  | CAN_CTRL1_PSEG2(4)
//  294                  | CAN_CTRL1_PRESDIV(prescale); 
        LDR      R1,[R4, #+4]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSLS     R0,R0,#+24
        ORR      R0,R0,#0xB40000
        ORRS     R0,R0,#0x6
        ORRS     R0,R0,R1
        STR      R0,[R4, #+4]
//  295         break;
        B.N      ??LPLD_CAN_SetBaudRate_4
//  296       }
//  297     default:break;
//  298   }
//  299 }
??LPLD_CAN_SetBaudRate_2:
??LPLD_CAN_SetBaudRate_4:
        POP      {R0,R4,R5,PC}    ;; return
//  300 
//  301 /*
//  302  * LPLD_CAN_Deinit
//  303  * CAN����ʼ���������ڸú����н�ֹCANx������ʱ�ӣ���ֹģ���жϣ��ر�CANģ��
//  304  * 
//  305  * ����:
//  306  *    CAN_InitTypeDef--can_init_structure
//  307  *                     ���嶨���HW_CAN.h can_init_structure
//  308  * ���:
//  309  *    ��
//  310  *
//  311 */

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//  312 void LPLD_CAN_Deinit(CAN_InitTypeDef can_init_structure)
//  313 {
LPLD_CAN_Deinit:
        PUSH     {R4,LR}
//  314   CAN_Type *canx = can_init_structure.CAN_Canx;
        MOVS     R4,R0
//  315   
//  316   if(canx == CAN0)
        LDR.W    R0,??DataTable12_5  ;; 0x40024000
        CMP      R4,R0
        BNE.N    ??LPLD_CAN_Deinit_0
//  317   {
//  318     SIM->SCGC6 &= ~SIM_SCGC6_FLEXCAN0_MASK;
        LDR.W    R0,??DataTable12_6  ;; 0x4004803c
        LDR      R0,[R0, #+0]
        BICS     R0,R0,#0x10
        LDR.W    R1,??DataTable12_6  ;; 0x4004803c
        STR      R0,[R1, #+0]
//  319     disable_irq(CAN0_ORed_Message_buffer_IRQn);
        MOVS     R0,#+29
        BL       NVIC_DisableIRQ
        B.N      ??LPLD_CAN_Deinit_1
//  320   }
//  321   else
//  322   {
//  323     SIM->SCGC3 &= ~SIM_SCGC3_FLEXCAN1_MASK;
??LPLD_CAN_Deinit_0:
        LDR.W    R0,??DataTable12_7  ;; 0x40048030
        LDR      R0,[R0, #+0]
        BICS     R0,R0,#0x10
        LDR.W    R1,??DataTable12_7  ;; 0x40048030
        STR      R0,[R1, #+0]
//  324     enable_irq(CAN1_ORed_Message_buffer_IRQn);
        MOVS     R0,#+37
        BL       NVIC_EnableIRQ
//  325   }
//  326   //��ֹCAN����
//  327   canx->MCR |= CAN_MCR_MDIS_MASK;
??LPLD_CAN_Deinit_1:
        LDR      R0,[R4, #+0]
        ORRS     R0,R0,#0x80000000
        STR      R0,[R4, #+0]
//  328   canx->MCR |= CAN_MCR_HALT_MASK; 
        LDR      R0,[R4, #+0]
        ORRS     R0,R0,#0x10000000
        STR      R0,[R4, #+0]
//  329   canx->MCR |= CAN_MCR_FRZ_MASK;
        LDR      R0,[R4, #+0]
        ORRS     R0,R0,#0x40000000
        STR      R0,[R4, #+0]
//  330 }
        POP      {R4,PC}          ;; return
//  331 /*
//  332  * LPLD_CAN_EnableIrq
//  333  * ʹ��CANģ���ж�
//  334  * 
//  335  * ����:
//  336  *    CAN_InitTypeDef--can_init_structure
//  337  *                     ���嶨���HW_CAN.h can_init_structure
//  338  * ���:
//  339  *    ��
//  340  *
//  341 */

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//  342 void LPLD_CAN_EnableIrq(CAN_InitTypeDef can_init_structure)
//  343 {
LPLD_CAN_EnableIrq:
        PUSH     {R7,LR}
//  344   CAN_Type *canx = can_init_structure.CAN_Canx;
//  345     
//  346   if(canx == CAN0)
        LDR.W    R1,??DataTable12_5  ;; 0x40024000
        CMP      R0,R1
        BNE.N    ??LPLD_CAN_EnableIrq_0
//  347   {
//  348     enable_irq(CAN0_ORed_Message_buffer_IRQn);
        MOVS     R0,#+29
        BL       NVIC_EnableIRQ
        B.N      ??LPLD_CAN_EnableIrq_1
//  349   }
//  350   else if(canx == CAN1)
??LPLD_CAN_EnableIrq_0:
        LDR.W    R1,??DataTable12_21  ;; 0x400a4000
        CMP      R0,R1
        BNE.N    ??LPLD_CAN_EnableIrq_1
//  351   {
//  352     enable_irq(CAN1_ORed_Message_buffer_IRQn);
        MOVS     R0,#+37
        BL       NVIC_EnableIRQ
//  353   }
//  354 }
??LPLD_CAN_EnableIrq_1:
        POP      {R0,PC}          ;; return
//  355 
//  356 /*
//  357  * LPLD_CAN_DisableIrq
//  358  * ��ֹCANģ���ж�
//  359  * 
//  360  * ����:
//  361  *    CAN_InitTypeDef--can_init_structure
//  362  *                     ���嶨���HW_CAN.h can_init_structure
//  363  * ���:
//  364  *    ��
//  365  *
//  366 */

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//  367 void LPLD_CAN_DisableIrq(CAN_InitTypeDef can_init_structure)
//  368 {
LPLD_CAN_DisableIrq:
        PUSH     {R7,LR}
//  369   CAN_Type *canx = can_init_structure.CAN_Canx;
//  370     
//  371   if(canx == CAN0)
        LDR.W    R1,??DataTable12_5  ;; 0x40024000
        CMP      R0,R1
        BNE.N    ??LPLD_CAN_DisableIrq_0
//  372   {
//  373     disable_irq(CAN0_ORed_Message_buffer_IRQn);
        MOVS     R0,#+29
        BL       NVIC_DisableIRQ
        B.N      ??LPLD_CAN_DisableIrq_1
//  374   }
//  375   else if(canx == CAN1)
??LPLD_CAN_DisableIrq_0:
        LDR.W    R1,??DataTable12_21  ;; 0x400a4000
        CMP      R0,R1
        BNE.N    ??LPLD_CAN_DisableIrq_1
//  376   {
//  377     disable_irq(CAN1_ORed_Message_buffer_IRQn);
        MOVS     R0,#+37
        BL       NVIC_DisableIRQ
//  378   }
//  379 }
??LPLD_CAN_DisableIrq_1:
        POP      {R0,PC}          ;; return
//  380 
//  381 /*
//  382  * LPLD_CAN_InitMessageObject
//  383  *
//  384  * ����CAN Message buffer��Msg����ʽ��ͨ��CAN_MSGOBJ_InitTypeDef
//  385  * �ṹ���CAN Message buffer���г�ʼ����
//  386  * 
//  387  * ����:
//  388  *    CAN_MSGOBJ_InitTypeDef--can_msg_init_structure
//  389  *                            ���嶨���HW_CAN.h CAN_MSGOBJ_InitTypeDef
//  390  *    message_id
//  391  *      ����message_id��id���ȸ���CAN_MSGOBJ_InitTypeDef�Ĳ�������
//  392  *      id�ĳ��ȷ�Ϊ11λ��׼֡��29λ��չ֡����
//  393  *      CAN_MSGOBJ_ID_STD ��ʾ��׼֡����CAN_MSGOBJ_InitTypeDef������id����ΪCAN_MSGOBJ_ID_STD
//  394  *      ��ʱmessage_id�Ϊ11λ�����ܳ���0x0FFF��
//  395  *      CAN_MSGOBJ_ID_EXT ��ʾ��չ֡����CAN_MSGOBJ_InitTypeDef������id����ΪCAN_MSGOBJ_ID_EXT
//  396  *      ��ʱmessage_id�Ϊ29λ�����ܳ���0x1FFFFFFF��
//  397  * ���:
//  398  *    ��
//  399  *
//  400  */

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//  401 void LPLD_CAN_InitMessageObject(CAN_MSGOBJ_InitTypeDef can_msg_init_structure,uint32 message_id)	
//  402 {
LPLD_CAN_InitMessageObject:
        PUSH     {R0-R6,LR}
//  403   uint32    cs_temp;
//  404   uint8     msg_num_temp  = can_msg_init_structure.CAN_MsgNum;
        LDRB     R4,[SP, #+4]
//  405   CAN_Type *canx_ptr      = can_msg_init_structure.CAN_Canx;
        LDR      R5,[SP, #+0]
//  406   
//  407   CAN_ISR_CALLBACK isr_func = can_msg_init_structure.CAN_Isr;
        LDR      R6,[SP, #+12]
//  408   
//  409   cs_temp = CAN_MB_CS_RTR(can_msg_init_structure.CAN_MsgRTR) | \ 
//  410             CAN_MB_CS_SRR(can_msg_init_structure.CAN_MsgSRR) | \ 
//  411             CAN_MB_CS_IDE(can_msg_init_structure.CAN_MsgIdLength) |\ 
//  412             CAN_MB_CS_DLC(can_msg_init_structure.CAN_MsgDataLength);
        LDRB     R0,[SP, #+8]
        LSLS     R0,R0,#+20
        ANDS     R0,R0,#0x100000
        LDRB     R1,[SP, #+7]
        LSLS     R1,R1,#+22
        ANDS     R1,R1,#0x400000
        ORRS     R0,R1,R0
        LDRB     R1,[SP, #+5]
        LSLS     R1,R1,#+21
        ANDS     R1,R1,#0x200000
        ORRS     R0,R1,R0
        LDRB     R1,[SP, #+6]
        LSLS     R1,R1,#+16
        ANDS     R1,R1,#0xF0000
        ORRS     R0,R1,R0
//  413   //����CAN_MsgDirection����������Msg�Ƿ��͹��ܻ��ǽ��չ���
//  414   if(can_msg_init_structure.CAN_MsgDirection == CAN_MSGOBJ_DIR_TX)
        LDRB     R1,[SP, #+9]
        CMP      R1,#+2
        BNE.N    ??LPLD_CAN_InitMessageObject_0
//  415   {
//  416     cs_temp |= CAN_MB_CS_CODE(CAN_MSGOBJ_TX_INACTIVE);
        ORRS     R0,R0,#0x8000000
        B.N      ??LPLD_CAN_InitMessageObject_1
//  417   }
//  418   else if(can_msg_init_structure.CAN_MsgDirection == CAN_MSGOBJ_DIR_RX)
??LPLD_CAN_InitMessageObject_0:
        LDRB     R1,[SP, #+9]
        CMP      R1,#+1
        BNE.N    ??LPLD_CAN_InitMessageObject_1
//  419   {
//  420     cs_temp |= CAN_MB_CS_CODE(CAN_MSGOBJ_RX_EMPTY);
        ORRS     R0,R0,#0x4000000
//  421   }
//  422   //����Msg CODE�ĳ�ʼֵ
//  423   //����Ƿ���ģʽ���ó�ΪINACTIVE״̬
//  424   //����ǽ���ģʽ���ó�ΪEMPTY״̬
//  425   canx_ptr->MB[msg_num_temp].CS = cs_temp;
??LPLD_CAN_InitMessageObject_1:
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        ADDS     R1,R5,R4, LSL #+4
        STR      R0,[R1, #+128]
//  426   //����Msg ID
//  427   LPLD_CAN_SetMsgID(canx_ptr,msg_num_temp,message_id); 
        LDR      R2,[SP, #+32]
        MOVS     R1,R4
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R5
        BL       LPLD_CAN_SetMsgID
//  428   //
//  429   if(can_msg_init_structure.CAN_MsgInterrupt == TRUE)
        LDRB     R0,[SP, #+10]
        CMP      R0,#+1
        BNE.N    ??LPLD_CAN_InitMessageObject_2
//  430   {
//  431     LPLD_CAN_EnableMsgInterrupt(canx_ptr,msg_num_temp);//ʹ��Msg�ж� 
        MOVS     R1,R4
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R5
        BL       LPLD_CAN_EnableMsgInterrupt
//  432     //����Msg�жϻص�����
//  433     if(canx_ptr == CAN0)
        LDR.W    R0,??DataTable12_5  ;; 0x40024000
        CMP      R5,R0
        BNE.N    ??LPLD_CAN_InitMessageObject_3
//  434     {
//  435       if(isr_func != NULL)
        MOVS     R0,R6
        CMP      R0,#+0
        BEQ.N    ??LPLD_CAN_InitMessageObject_4
//  436       {
//  437         CAN0_ISR[msg_num_temp] = isr_func;
        LDR.W    R0,??DataTable12_22
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        STR      R6,[R0, R4, LSL #+2]
        B.N      ??LPLD_CAN_InitMessageObject_4
//  438       }
//  439     }
//  440     else
//  441     {
//  442       if(isr_func != NULL)
??LPLD_CAN_InitMessageObject_3:
        MOVS     R0,R6
        CMP      R0,#+0
        BEQ.N    ??LPLD_CAN_InitMessageObject_4
//  443       {
//  444         CAN1_ISR[msg_num_temp] = isr_func;
        LDR.W    R0,??DataTable12_23
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        STR      R6,[R0, R4, LSL #+2]
        B.N      ??LPLD_CAN_InitMessageObject_4
//  445       }
//  446     }
//  447   }
//  448   else
//  449   {
//  450     if(canx_ptr == CAN0)
??LPLD_CAN_InitMessageObject_2:
        LDR.W    R0,??DataTable12_5  ;; 0x40024000
        CMP      R5,R0
        BNE.N    ??LPLD_CAN_InitMessageObject_5
//  451     {
//  452         CAN0_ISR[msg_num_temp] = NULL; 
        MOVS     R0,#+0
        LDR.W    R1,??DataTable12_22
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        STR      R0,[R1, R4, LSL #+2]
        B.N      ??LPLD_CAN_InitMessageObject_4
//  453     }
//  454     else
//  455     {
//  456         CAN1_ISR[msg_num_temp] = NULL;
??LPLD_CAN_InitMessageObject_5:
        MOVS     R0,#+0
        LDR.W    R1,??DataTable12_23
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        STR      R0,[R1, R4, LSL #+2]
//  457     }
//  458   }
//  459 }
??LPLD_CAN_InitMessageObject_4:
        POP      {R0-R6,PC}       ;; return
//  460 
//  461 /*
//  462  * LPLD_CAN_TransmitMessage
//  463  * CAN ������Ϣ����
//  464  * ����:
//  465  *    CAN_MSGOBJ_InitTypeDef--can_msg_init_structure
//  466  *                            ���嶨���HW_CAN.h CAN_MSGOBJ_InitTypeDef
//  467  *    CAN_MessageFormat_TypeDef--*can_rx_msg
//  468  *                            ���嶨���HW_CAN.h CAN_MessageFormat_TypeDef
//  469  *    �ýṹ�����ڶ���CAN����Ϣ��ʽ����CAN������Ϣʱ�����CAN_MessageFormat_TypeDef����Ľṹ�������
//  470  *    ��ȡ��Ϣ���洢��MSG�У�MSG�Ὣ��Ϣ���͵�CAN�����ϡ�
//  471  *    �ýṹ���е���Ҫ����Ϊ ��ϢID��ʱ��������ݳ��ȡ��洢���ݵĻ�����
//  472  * ���:
//  473  *    1 �������
//  474  *    0 ����ʧ��
//  475  *
//  476  */

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//  477 uint8 LPLD_CAN_TransmitMessage(CAN_MSGOBJ_InitTypeDef can_msg_init_structure,\ 
//  478                                CAN_MessageFormat_TypeDef *can_tx_msg )
//  479 {
LPLD_CAN_TransmitMessage:
        PUSH     {R0-R8,LR}
//  480   uint8  request;
//  481   uint8  msg_num_temp  = can_msg_init_structure.CAN_MsgNum;
        LDRB     R4,[SP, #+4]
//  482   CAN_Type *canx_ptr   = can_msg_init_structure.CAN_Canx;
        LDR      R5,[SP, #+0]
        LDR      R0,[SP, #+40]
//  483   uint32  message_id = can_tx_msg->CAN_MsgID;  
        LDR      R6,[R0, #+0]
//  484   uint8  *in_data_buffer = can_tx_msg->CAN_MsgDataBuffer;
        ADDS     R7,R0,#+6
//  485   uint8   in_data_length = can_tx_msg->CAN_MsgDataLength;
        LDRB     R8,[R0, #+14]
//  486   //�������Msg�������е�CODE������CAN_MSGOBJ_TX_UNCONDITIONAL
//  487   if( LPLD_CAN_GetMsgCode(canx_ptr,msg_num_temp) != CAN_MSGOBJ_TX_UNCONDITIONAL)
        MOVS     R1,R4
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R5
        BL       LPLD_CAN_GetMsgCode
        CMP      R0,#+12
        BEQ.N    ??LPLD_CAN_TransmitMessage_0
//  488   {
//  489     //����Msg�������е�CODE��дINACTIVE����
//  490     canx_ptr->MB[msg_num_temp].CS |= CAN_MB_CS_CODE(CAN_MSGOBJ_TX_INACTIVE);
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        ADDS     R0,R5,R4, LSL #+4
        LDR      R0,[R0, #+128]
        ORRS     R0,R0,#0x8000000
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        ADDS     R1,R5,R4, LSL #+4
        STR      R0,[R1, #+128]
//  491     //����Msg��ID
//  492     LPLD_CAN_SetMsgID(canx_ptr,msg_num_temp,message_id);
        MOVS     R2,R6
        MOVS     R1,R4
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R5
        BL       LPLD_CAN_SetMsgID
//  493     //��Msg��д��Ҫ���͵�����
//  494     LPLD_CAN_WriteData(canx_ptr,msg_num_temp,in_data_length,in_data_buffer);
        MOVS     R3,R7
        MOV      R2,R8
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        MOVS     R1,R4
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R5
        BL       LPLD_CAN_WriteData
//  495     //����Msg�ĳ���
//  496     LPLD_CAN_SetMsgLength(canx_ptr,msg_num_temp,in_data_length);
        MOV      R2,R8
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        MOVS     R1,R4
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R5
        BL       LPLD_CAN_SetMsgLength
//  497     //����Msg�������е�CODE��дTX_UNCONDITIONAL����ȴ��������
//  498     canx_ptr->MB[msg_num_temp].CS |= CAN_MB_CS_CODE(CAN_MSGOBJ_TX_UNCONDITIONAL);
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        ADDS     R0,R5,R4, LSL #+4
        LDR      R0,[R0, #+128]
        ORRS     R0,R0,#0xC000000
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        ADDS     R1,R5,R4, LSL #+4
        STR      R0,[R1, #+128]
//  499     //������������жϣ�������Ϻ�ᴥ���ж�
//  500     request = 1;
        MOVS     R0,#+1
        B.N      ??LPLD_CAN_TransmitMessage_1
//  501   }
//  502   else
//  503   {
//  504     request = 0;
??LPLD_CAN_TransmitMessage_0:
        MOVS     R0,#+0
//  505   }
//  506   return (request);
??LPLD_CAN_TransmitMessage_1:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        ADD      SP,SP,#+16
        POP      {R4-R8,PC}       ;; return
//  507 }
//  508 
//  509 /*
//  510  * LPLD_CAN_ReceivedMessage
//  511  * CAN ������պ������˺���һ����CAN�жϺ����е���
//  512  * ����:
//  513  *    CAN_MSGOBJ_InitTypeDef--can_msg_init_structure
//  514  *                            ���嶨���HW_CAN.h CAN_MSGOBJ_InitTypeDef
//  515  *    CAN_MessageFormat_TypeDef--*can_rx_msg
//  516  *                            ���嶨���HW_CAN.h CAN_MessageFormat_TypeDef
//  517  *    �ýṹ�����ڶ���CAN����Ϣ��ʽ����CAN�ɹ�������Ϣ�Ժ󣬻��MSG�н���Ϣ�洢��
//  518  *    CAN_MessageFormat_TypeDef����Ľṹ������С�
//  519  *    �ýṹ���е���Ҫ����Ϊ ��ϢID��ʱ��������ݳ��ȡ��洢���ݵĻ�����
//  520  *    
//  521  *  �����
//  522  *    ��
//  523  */

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//  524 void LPLD_CAN_ReceivedMessage(CAN_MSGOBJ_InitTypeDef can_msg_init_structure,\ 
//  525                               CAN_MessageFormat_TypeDef *can_rx_msg)
//  526 {
LPLD_CAN_ReceivedMessage:
        PUSH     {R0-R7,LR}
        SUB      SP,SP,#+12
//  527   uint32 message_code,i;
//  528   volatile uint16 timer;
//  529   uint8 rx_data[CAN_DATA_MAX_BYTES];
//  530   uint8 data_length;
//  531   uint16 time_stamp;
//  532   uint32 msg_id;
//  533   CAN_Type *canx = can_msg_init_structure.CAN_Canx;
        LDR      R4,[SP, #+12]
//  534   uint8 msg_num = can_msg_init_structure.CAN_MsgNum;
        LDRB     R5,[SP, #+16]
//  535   //���Msg�������е�codeֵ
//  536   message_code = LPLD_CAN_GetMsgCode(canx,msg_num);
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
        BL       LPLD_CAN_GetMsgCode
//  537   
//  538   if ((message_code != CAN_MSGOBJ_RX_BUSY) && 
//  539      (message_code != CAN_MSGOBJ_RX_OVERRUN))
        CMP      R0,#+1
        BEQ.N    ??LPLD_CAN_ReceivedMessage_0
        CMP      R0,#+6
        BEQ.N    ??LPLD_CAN_ReceivedMessage_0
//  540   {
//  541     //��ȡID(��ѡ)
//  542     msg_id = LPLD_CAN_GetMsgID(canx, msg_num);
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
        BL       LPLD_CAN_GetMsgID
        MOVS     R6,R0
//  543    
//  544     //��ȡMsg�������������ݵĳ���
//  545     data_length    = (uint8_t)LPLD_CAN_GetMsgLength(canx, msg_num);
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
        BL       LPLD_CAN_GetMsgLength
        MOVS     R7,R0
//  546     //��ȡMsg��������������
//  547     LPLD_CAN_GetData(canx, msg_num,data_length,rx_data);
        ADD      R3,SP,#+4
        MOVS     R2,R7
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
        BL       LPLD_CAN_GetData
//  548     //���Msg������ʱ���
//  549     time_stamp = LPLD_CAN_GetMsgTimeStamp(canx, msg_num);
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
        BL       LPLD_CAN_GetMsgTimeStamp
//  550     
//  551     for(i = data_length; i < 8; i++)
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        MOVS     R1,R7
        B.N      ??LPLD_CAN_ReceivedMessage_1
//  552     {
//  553        rx_data[i] = 0;
??LPLD_CAN_ReceivedMessage_2:
        MOVS     R2,#+0
        ADD      R3,SP,#+4
        STRB     R2,[R1, R3]
//  554     }
        ADDS     R1,R1,#+1
??LPLD_CAN_ReceivedMessage_1:
        CMP      R1,#+8
        BCC.N    ??LPLD_CAN_ReceivedMessage_2
        LDR      R3,[SP, #+48]
//  555     //��MSG�е���Ϣ�洢��can_rx_msg��
//  556     can_rx_msg->CAN_MsgID = msg_id;
        STR      R6,[R3, #+0]
//  557     can_rx_msg->CAN_MsgDataLength = data_length;
        STRB     R7,[R3, #+14]
//  558     can_rx_msg->CAN_MsgTimeStamp  = time_stamp;
        STRH     R0,[R3, #+4]
//  559     memcpy(can_rx_msg->CAN_MsgDataBuffer,rx_data,data_length);
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        MOVS     R2,R7
        ADD      R1,SP,#+4
        ADDS     R0,R3,#+6
        BL       memcpy
//  560      
//  561     //��ȡ���ɼ�������������ǰMsg������
//  562     timer = LPLD_CAN_UnlockMsg(canx);
        MOVS     R0,R4
        BL       LPLD_CAN_UnlockMsg
        STRH     R0,[SP, #+0]
//  563     //���Msg���������жϱ�־λ
//  564     LPLD_CAN_Interrupt_ClearPending(canx,msg_num);
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
        BL       LPLD_CAN_Interrupt_ClearPending
//  565     //�ٴ�д�����codeΪempty״̬
//  566     LPLD_CAN_SetMsgCode(canx,msg_num,CAN_MSGOBJ_RX_EMPTY);
        MOVS     R2,#+4
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
        BL       LPLD_CAN_SetMsgCode
        B.N      ??LPLD_CAN_ReceivedMessage_3
//  567   }
//  568   else
//  569   {
//  570     //�����������
//  571     LPLD_CAN_Interrupt_ClearPending(canx,msg_num);
??LPLD_CAN_ReceivedMessage_0:
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
        BL       LPLD_CAN_Interrupt_ClearPending
//  572     //��ȡ���ɼ�������������ǰMsg������
//  573     timer = LPLD_CAN_UnlockMsg(canx);
        MOVS     R0,R4
        BL       LPLD_CAN_UnlockMsg
        STRH     R0,[SP, #+0]
//  574   }
//  575 }
??LPLD_CAN_ReceivedMessage_3:
        ADD      SP,SP,#+28
        POP      {R4-R7,PC}       ;; return
//  576 
//  577 /*
//  578  * LPLD_CAN_Interrupt_ClearPending
//  579  * ���Msg���жϱ�־λ
//  580  * ����:
//  581  *    CAN_Type *canx
//  582  *      |___ CAN0 --CAN0��ģ��
//  583  *      |___ CAN1 --CAN1��ģ��
//  584  *    msg_num ѡ��Ҫ����жϱ�־��Msg������
//  585  *      |___ MSG_NUM_0 --Msg������0
//  586  *      |___ MSG_NUM_1 --Msg������1
//  587  *      |___ MSG_NUM_2 --Msg������2
//  588  *      |___ MSG_NUM_3 --Msg������3
//  589  *      |___ MSG_NUM_4 --Msg������4
//  590  *      ...... 
//  591  *      |___ MSG_NUM_15 --Msg������15
//  592  * ���:
//  593  *   ��
//  594  */

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//  595 static void LPLD_CAN_Interrupt_ClearPending(CAN_Type *canx, uint8 msg_num)
//  596 {   
//  597   if(msg_num < 32)
LPLD_CAN_Interrupt_ClearPending:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        CMP      R1,#+32
        BGE.N    ??LPLD_CAN_Interrupt_ClearPending_0
//  598   {
//  599     canx->IFLAG1 |= (1<<msg_num); 
        LDR      R2,[R0, #+48]
        MOVS     R3,#+1
        LSLS     R1,R3,R1
        ORRS     R1,R1,R2
        STR      R1,[R0, #+48]
        B.N      ??LPLD_CAN_Interrupt_ClearPending_1
//  600   }
//  601   else
//  602   {
//  603     canx->IFLAG2 |= (1<<(msg_num-32));
??LPLD_CAN_Interrupt_ClearPending_0:
        LDR      R2,[R0, #+44]
        MOVS     R3,#+1
        SUBS     R1,R1,#+32
        LSLS     R1,R3,R1
        ORRS     R1,R1,R2
        STR      R1,[R0, #+44]
//  604   }
//  605 }
??LPLD_CAN_Interrupt_ClearPending_1:
        BX       LR               ;; return
//  606 
//  607 /*
//  608  * LPLD_CAN_Interrupt_GetFlag
//  609  * ���CAN ���жϱ�־λ
//  610  * ����:
//  611  *    CAN_Type *canx
//  612  *      |___ CAN0 --CAN0��ģ��
//  613  *      |___ CAN1 --CAN1��ģ��
//  614  *    msg_num ѡ��Ҫ����жϱ�־��Msg������
//  615  *      |___ MSG_NUM_0 --Msg������0
//  616  *      |___ MSG_NUM_1 --Msg������1
//  617  *      |___ MSG_NUM_2 --Msg������2
//  618  *      |___ MSG_NUM_3 --Msg������3
//  619  *      |___ MSG_NUM_4 --Msg������4
//  620  *      ...... 
//  621  *      |___ MSG_NUM_15 --Msg������15
//  622  * ���:
//  623  *    ������Ҫ Msg���������жϱ�־λ
//  624  */

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//  625 static uint8 LPLD_CAN_Interrupt_GetFlag(CAN_Type *canx, uint8 msg_num)
//  626 {   
//  627   uint8 status;
//  628   if(msg_num < 32)
LPLD_CAN_Interrupt_GetFlag:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        CMP      R1,#+32
        BGE.N    ??LPLD_CAN_Interrupt_GetFlag_0
//  629   {
//  630     status = (canx->IFLAG1 >> msg_num) & 0x01; 
        LDR      R0,[R0, #+48]
        LSRS     R0,R0,R1
        ANDS     R0,R0,#0x1
        B.N      ??LPLD_CAN_Interrupt_GetFlag_1
//  631   }
//  632   else
//  633   {
//  634     status = (canx->IFLAG2 >> (msg_num - 32) )& 0x01; 
??LPLD_CAN_Interrupt_GetFlag_0:
        LDR      R0,[R0, #+44]
        SUBS     R1,R1,#+32
        LSRS     R0,R0,R1
        ANDS     R0,R0,#0x1
//  635   }
//  636   return status;
??LPLD_CAN_Interrupt_GetFlag_1:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        BX       LR               ;; return
//  637 }
//  638 
//  639 /*
//  640  * LPLD_CAN_UnlockMsg
//  641  * �ú���ͨ����ȡ���ɼ�����������Msg
//  642  * ����:
//  643  *    canx--����CAN����ͨ��
//  644  *      |__CAN0 --CAN0��ģ��
//  645  *      |__CAN1 --CAN1��ģ��
//  646  * ���:
//  647  *    16λCAN���ɼ�������¼��ʱ���
//  648  */

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//  649 static uint16 LPLD_CAN_UnlockMsg(CAN_Type *canx)
//  650 {
//  651   return (canx->TIMER);
LPLD_CAN_UnlockMsg:
        LDR      R0,[R0, #+8]
        UXTH     R0,R0            ;; ZeroExt  R0,R0,#+16,#+16
        BX       LR               ;; return
//  652 }
//  653 
//  654 /*
//  655  * LPLD_CAN_EnableMsgInterrupt
//  656  * �ú�������ʹ��Msg�ж�
//  657  * 
//  658  * ����:
//  659  *    canx--����CAN����ͨ��
//  660  *      |__CAN0 --CAN0��ģ��
//  661  *      |__CAN1 --CAN1��ģ��
//  662  *    msg_num ѡ����Ҫʹ���жϵ�Msg������
//  663  *      |___ MSG_NUM_0 --Msg������0
//  664  *      |___ MSG_NUM_1 --Msg������1
//  665  *      |___ MSG_NUM_2 --Msg������2
//  666  *      |___ MSG_NUM_3 --Msg������3
//  667  *      |___ MSG_NUM_4 --Msg������4
//  668  *      ...... 
//  669  *      |___ MSG_NUM_15 --Msg������15
//  670  * ���:
//  671  *    ��
//  672  *
//  673  */

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//  674 static void LPLD_CAN_EnableMsgInterrupt(CAN_Type *canx, uint8 msg_num)
//  675 {
//  676   if(msg_num < 32)
LPLD_CAN_EnableMsgInterrupt:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        CMP      R1,#+32
        BGE.N    ??LPLD_CAN_EnableMsgInterrupt_0
//  677   {
//  678     canx->IMASK1 |= (1<<msg_num); 
        LDR      R2,[R0, #+40]
        MOVS     R3,#+1
        LSLS     R1,R3,R1
        ORRS     R1,R1,R2
        STR      R1,[R0, #+40]
        B.N      ??LPLD_CAN_EnableMsgInterrupt_1
//  679   }
//  680   else
//  681   {
//  682     canx->IMASK2 |= (1<<(msg_num-32));
??LPLD_CAN_EnableMsgInterrupt_0:
        LDR      R2,[R0, #+36]
        MOVS     R3,#+1
        SUBS     R1,R1,#+32
        LSLS     R1,R3,R1
        ORRS     R1,R1,R2
        STR      R1,[R0, #+36]
//  683   }
//  684 }
??LPLD_CAN_EnableMsgInterrupt_1:
        BX       LR               ;; return
//  685 
//  686 /*
//  687  * LPLD_CAN_SetMsgCode
//  688  * �ú�����������Msg��codeֵ
//  689  * 
//  690  * ����:
//  691  *    canx--����CAN����ͨ��
//  692  *      |__CAN0 --CAN0��ģ��
//  693  *      |__CAN1 --CAN1��ģ��
//  694  *    msg_num ѡ����Ҫ����CODE��Msg������
//  695  *      |___ MSG_NUM_0 --Msg������0
//  696  *      |___ MSG_NUM_1 --Msg������1
//  697  *      |___ MSG_NUM_2 --Msg������2
//  698  *      |___ MSG_NUM_3 --Msg������3
//  699  *      |___ MSG_NUM_4 --Msg������4
//  700  *      ...... 
//  701  *      |___ MSG_NUM_15 --Msg������15
//  702  *    code--��Ҫ���õ�codeֵ
//  703  * ���:
//  704  *    ��
//  705  *
//  706  */

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//  707 static void LPLD_CAN_SetMsgCode(CAN_Type *canx, uint8 msg_num, uint8 code)
//  708 {
//  709   //canx->MB[msg_num].CS &= (~CAN_MB_CS_CODE_MASK);
//  710   canx->MB[msg_num].CS |= CAN_MB_CS_CODE(code);
LPLD_CAN_SetMsgCode:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ADDS     R3,R0,R1, LSL #+4
        LDR      R3,[R3, #+128]
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        LSLS     R2,R2,#+24
        ANDS     R2,R2,#0xF000000
        ORRS     R2,R2,R3
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ADDS     R0,R0,R1, LSL #+4
        STR      R2,[R0, #+128]
//  711 }
        BX       LR               ;; return
//  712 
//  713 /*
//  714  * LPLD_CAN_GetMsgCode
//  715  * �ú������ڻ��Msg��������codeֵ
//  716  * 
//  717  * ����:
//  718  *    canx--����CAN����ͨ��
//  719  *      |__CAN0 --CAN0��ģ��
//  720  *      |__CAN1 --CAN1��ģ��
//  721  *    msg_num ѡ����Ҫ���CODE��Msg������
//  722  *      |___ MSG_NUM_0 --Msg������0
//  723  *      |___ MSG_NUM_1 --Msg������1
//  724  *      |___ MSG_NUM_2 --Msg������2
//  725  *      |___ MSG_NUM_3 --Msg������3
//  726  *      |___ MSG_NUM_4 --Msg������4
//  727  *      ...... 
//  728  *      |___ MSG_NUM_15 --Msg������15
//  729  * ���:
//  730  *     ������ӦMsg��������code
//  731  *
//  732  */

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//  733 static uint32 LPLD_CAN_GetMsgCode(CAN_Type *canx, uint8 msg_num)
//  734 {
//  735    return CAN_GET_MB_CS_CODE(canx->MB[msg_num].CS);
LPLD_CAN_GetMsgCode:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ADDS     R0,R0,R1, LSL #+4
        LDR      R0,[R0, #+128]
        UBFX     R0,R0,#+24,#+4
        BX       LR               ;; return
//  736 }
//  737 /*
//  738  * LPLD_CAN_GetMsgTimeStamp
//  739  * �ú������ڻ��Msg��������ʱ���
//  740  * 
//  741  * ����:
//  742  *    canx--����CAN����ͨ��
//  743  *      |__CAN0 --CAN0��ģ��
//  744  *      |__CAN1 --CAN1��ģ��
//  745  *    msg_num ѡ����Ҫ���ʱ�����Msg������
//  746  *      |___ MSG_NUM_0 --Msg������0
//  747  *      |___ MSG_NUM_1 --Msg������1
//  748  *      |___ MSG_NUM_2 --Msg������2
//  749  *      |___ MSG_NUM_3 --Msg������3
//  750  *      |___ MSG_NUM_4 --Msg������4
//  751  *      ...... 
//  752  *      |___ MSG_NUM_15 --Msg������15
//  753  * ���:
//  754  *     ������ӦMsg��������ʱ���
//  755  *
//  756  */

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//  757 static uint16 LPLD_CAN_GetMsgTimeStamp(CAN_Type *canx, uint8 msg_num)
//  758 {
//  759   return CAN_GET_MB_CS_TIMESTAMP(canx->MB[msg_num].CS);
LPLD_CAN_GetMsgTimeStamp:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ADDS     R0,R0,R1, LSL #+4
        LDR      R0,[R0, #+128]
        UXTH     R0,R0            ;; ZeroExt  R0,R0,#+16,#+16
        BX       LR               ;; return
//  760 }
//  761 
//  762 /*
//  763  * LPLD_CAN_SetMsgID
//  764  * �ú�����������Msg��������IDֵ
//  765  * 
//  766  * ����:
//  767  *    canx--����CAN����ͨ��
//  768  *      |__CAN0 --CAN0��ģ��
//  769  *      |__CAN1 --CAN1��ģ��
//  770  *    msg_num ѡ����Ҫ����ID��Msg������
//  771  *      |___ MSG_NUM_0 --Msg������0
//  772  *      |___ MSG_NUM_1 --Msg������1
//  773  *      |___ MSG_NUM_2 --Msg������2
//  774  *      |___ MSG_NUM_3 --Msg������3
//  775  *      |___ MSG_NUM_4 --Msg������4
//  776  *      ...... 
//  777  *      |___ MSG_NUM_15 --Msg������15
//  778  *   message_id
//  779  *      |__����Msg��������ID
//  780  * �����
//  781  *   ��
//  782  */

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//  783 static void LPLD_CAN_SetMsgID(CAN_Type *canx, uint8 msg_num,uint32 message_id)
//  784 {
//  785   uint32 id_temp;
//  786   if(CAN_GET_MB_CS_IDE(canx->MB[msg_num].CS) == CAN_MSGOBJ_ID_STD)
LPLD_CAN_SetMsgID:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ADDS     R3,R0,R1, LSL #+4
        LDR      R3,[R3, #+128]
        UBFX     R3,R3,#+21,#+1
        CMP      R3,#+0
        BNE.N    ??LPLD_CAN_SetMsgID_0
//  787   {
//  788     id_temp =  CAN_MB_ID_STD(message_id);
        LDR.W    R3,??DataTable12_24  ;; 0x1ffc0000
        ANDS     R2,R3,R2, LSL #+18
        B.N      ??LPLD_CAN_SetMsgID_1
//  789   }
//  790   else
//  791   {
//  792     id_temp =  CAN_MB_ID_EXT(message_id);
??LPLD_CAN_SetMsgID_0:
        LSLS     R2,R2,#+3        ;; ZeroExtS R2,R2,#+3,#+3
        LSRS     R2,R2,#+3
//  793   }
//  794   canx->MB[msg_num].ID = id_temp;
??LPLD_CAN_SetMsgID_1:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ADDS     R0,R0,R1, LSL #+4
        STR      R2,[R0, #+132]
//  795 }
        BX       LR               ;; return
//  796 
//  797 /*
//  798  * LPLD_CAN_GetMsgID
//  799  * �ú������ڻ��Msg��������IDֵ
//  800  * 
//  801  * ����:
//  802  *    canx--����CAN����ͨ��
//  803  *      |__CAN0 --CAN0��ģ��
//  804  *      |__CAN1 --CAN1��ģ��
//  805  *    msg_num ѡ����Ҫ���ID��Msg������
//  806  *      |___ MSG_NUM_0 --Msg������0
//  807  *      |___ MSG_NUM_1 --Msg������1
//  808  *      |___ MSG_NUM_2 --Msg������2
//  809  *      |___ MSG_NUM_3 --Msg������3
//  810  *      |___ MSG_NUM_4 --Msg������4
//  811  *      ...... 
//  812  *      |___ MSG_NUM_15 --Msg������15
//  813  * ���:
//  814  *     ������ӦMsg��������ID
//  815  *
//  816  */

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//  817 static uint32 LPLD_CAN_GetMsgID(CAN_Type *canx, uint8 msg_num)
//  818 {
//  819   uint32 id_temp;
//  820   if(CAN_GET_MB_CS_IDE(canx->MB[msg_num].CS) == CAN_MSGOBJ_ID_STD)
LPLD_CAN_GetMsgID:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ADDS     R2,R0,R1, LSL #+4
        LDR      R2,[R2, #+128]
        UBFX     R2,R2,#+21,#+1
        CMP      R2,#+0
        BNE.N    ??LPLD_CAN_GetMsgID_0
//  821   {
//  822     id_temp =  CAN_GET_MB_ID_STD(canx->MB[msg_num].ID);
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ADDS     R0,R0,R1, LSL #+4
        LDR      R0,[R0, #+132]
        UBFX     R0,R0,#+18,#+11
        B.N      ??LPLD_CAN_GetMsgID_1
//  823   }
//  824   else
//  825   {
//  826     id_temp =  CAN_GET_MB_ID_EXT(canx->MB[msg_num].ID);
??LPLD_CAN_GetMsgID_0:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ADDS     R0,R0,R1, LSL #+4
        LDR      R0,[R0, #+132]
        LSLS     R0,R0,#+3        ;; ZeroExtS R0,R0,#+3,#+3
        LSRS     R0,R0,#+3
//  827   }
//  828   return id_temp;
??LPLD_CAN_GetMsgID_1:
        BX       LR               ;; return
//  829 }
//  830 
//  831 /*
//  832  * LPLD_CAN_SetMsgLength
//  833  * �ú�����������Msg�����������ݳ���
//  834  * 
//  835  * ����:
//  836  *    canx--����CAN����ͨ��
//  837  *      |__CAN0 --CAN0��ģ��
//  838  *      |__CAN1 --CAN1��ģ��
//  839  *    msg_num ѡ����Ҫ�������ݳ��ȵ�Msg������
//  840  *      |___ MSG_NUM_0 --Msg������0
//  841  *      |___ MSG_NUM_1 --Msg������1
//  842  *      |___ MSG_NUM_2 --Msg������2
//  843  *      |___ MSG_NUM_3 --Msg������3
//  844  *      |___ MSG_NUM_4 --Msg������4
//  845  *      ...... 
//  846  *      |___ MSG_NUM_15 --Msg������15
//  847  *   in_length
//  848  *      |__���ݳ���
//  849  * ���:
//  850  *     ��
//  851  *
//  852  */

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//  853 static void LPLD_CAN_SetMsgLength(CAN_Type *canx, uint8 msg_num,uint8 in_length)
//  854 {
//  855   //canx->MB[msg_num].CS &= (~CAN_MB_CS_DLC_MASK);
//  856   canx->MB[msg_num].CS |= CAN_MB_CS_DLC(in_length);  
LPLD_CAN_SetMsgLength:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ADDS     R3,R0,R1, LSL #+4
        LDR      R3,[R3, #+128]
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        LSLS     R2,R2,#+16
        ANDS     R2,R2,#0xF0000
        ORRS     R2,R2,R3
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ADDS     R0,R0,R1, LSL #+4
        STR      R2,[R0, #+128]
//  857 }
        BX       LR               ;; return
//  858 
//  859 /*
//  860  * LPLD_CAN_GetMsgLength
//  861  * �ú������ڻ��Msg�����������ݳ���
//  862  * 
//  863  * ����:
//  864  *    canx--����CAN����ͨ��
//  865  *      |__CAN0 --CAN0��ģ��
//  866  *      |__CAN1 --CAN1��ģ��
//  867  *    msg_num ѡ����Ҫ������ݳ��ȵ�Msg������
//  868  *      |___ MSG_NUM_0 --Msg������0
//  869  *      |___ MSG_NUM_1 --Msg������1
//  870  *      |___ MSG_NUM_2 --Msg������2
//  871  *      |___ MSG_NUM_3 --Msg������3
//  872  *      |___ MSG_NUM_4 --Msg������4
//  873  *      ...... 
//  874  *      |___ MSG_NUM_15 --Msg������15
//  875  * ���:
//  876  *     ���ݵĳ���
//  877  *
//  878  */

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//  879 static uint8 LPLD_CAN_GetMsgLength(CAN_Type *canx, uint8 msg_num)
//  880 {
//  881   return CAN_GET_MB_CS_LENGTH(canx->MB[msg_num].CS);
LPLD_CAN_GetMsgLength:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ADDS     R0,R0,R1, LSL #+4
        LDR      R0,[R0, #+128]
        LSRS     R0,R0,#+16
        ANDS     R0,R0,#0xF
        BX       LR               ;; return
//  882 }
//  883 
//  884 /*
//  885  * LPLD_CAN_GetData
//  886  * �ú������ڻ��Msg�������е�����
//  887  * 
//  888  * ����:
//  889  *    canx--����CAN����ͨ��
//  890  *      |__CAN0 --CAN0��ģ��
//  891  *      |__CAN1 --CAN1��ģ��
//  892  *    msg_num ѡ����Ҫ������ݵ�Msg������
//  893  *      |___ MSG_NUM_0 --Msg������0
//  894  *      |___ MSG_NUM_1 --Msg������1
//  895  *      |___ MSG_NUM_2 --Msg������2
//  896  *      |___ MSG_NUM_3 --Msg������3
//  897  *      |___ MSG_NUM_4 --Msg������4
//  898  *      ...... 
//  899  *      |___ MSG_NUM_15 --Msg������15
//  900  *    in_length
//  901  *      |__������ݳ���
//  902  *    *in_buffer
//  903  *      |__������ݵĻ�����
//  904  * ���:
//  905  *    �� 
//  906  */

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//  907 static void LPLD_CAN_GetData(
//  908     CAN_Type *canx, 
//  909     uint8     msg_num, 
//  910     uint8     in_length, 
//  911     uint8    *in_buffer)
//  912 {
LPLD_CAN_GetData:
        PUSH     {R4-R7}
//  913   int i;
//  914   uint8  *pMBData;
//  915   uint8  *pRecieve  = in_buffer;
//  916   uint8  word_num   = (in_length - 1)/ 4; //Get the Message buffer word number
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        SUBS     R4,R2,#+1
        MOVS     R5,#+4
        SDIV     R4,R4,R5
//  917   uint8  rest_bytes = (in_length - 1)% 4; //Get the reset bytes of buffer
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        SUBS     R5,R2,#+1
        MOVS     R6,#+4
        SDIV     R2,R5,R6
        MLS      R2,R2,R6,R5
//  918   if(word_num > 0)
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        CMP      R4,#+0
        BEQ.N    ??LPLD_CAN_GetData_0
//  919   {  
//  920     pMBData = (uint8*)&canx->MB[msg_num].WORD0+3;
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ADDS     R5,R0,R1, LSL #+4
        ADDS     R6,R5,#+139
//  921     for(i = 0 ; i < 4 ; i++)   
        MOVS     R5,#+0
        B.N      ??LPLD_CAN_GetData_1
//  922     {
//  923       pRecieve[i] = *pMBData--;
??LPLD_CAN_GetData_2:
        LDRB     R7,[R6, #+0]
        STRB     R7,[R5, R3]
        SUBS     R6,R6,#+1
//  924     }
        ADDS     R5,R5,#+1
??LPLD_CAN_GetData_1:
        CMP      R5,#+4
        BLT.N    ??LPLD_CAN_GetData_2
//  925     pMBData = (uint8*)&canx->MB[msg_num].WORD1+3;
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ADDS     R0,R0,R1, LSL #+4
        ADDS     R6,R0,#+143
        B.N      ??LPLD_CAN_GetData_3
//  926   }
//  927   else
//  928   {
//  929     pMBData = (uint8*)&canx->MB[msg_num].WORD0+3;
??LPLD_CAN_GetData_0:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ADDS     R0,R0,R1, LSL #+4
        ADDS     R6,R0,#+139
//  930   }
//  931 
//  932   for(i = 0; i <= rest_bytes; i++)
??LPLD_CAN_GetData_3:
        MOVS     R5,#+0
        B.N      ??LPLD_CAN_GetData_4
//  933   {
//  934     pRecieve[i+(word_num<<2)] = *pMBData--;    
??LPLD_CAN_GetData_5:
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        ADDS     R0,R5,R4, LSL #+2
        LDRB     R1,[R6, #+0]
        STRB     R1,[R0, R3]
        SUBS     R6,R6,#+1
//  935   }
        ADDS     R5,R5,#+1
??LPLD_CAN_GetData_4:
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        CMP      R2,R5
        BGE.N    ??LPLD_CAN_GetData_5
//  936 }
        POP      {R4-R7}
        BX       LR               ;; return
//  937 
//  938 /*
//  939  * LPLD_CAN_WriteData
//  940  * �ú���������Msg�������е�д����
//  941  * 
//  942  * ����:
//  943  *    canx--����CAN����ͨ��
//  944  *      |__CAN0 --CAN0��ģ��
//  945  *      |__CAN1 --CAN1��ģ��
//  946  *    msg_num ѡ����Ҫд���ݵ�Msg������
//  947  *      |___ MSG_NUM_0 --Msg������0
//  948  *      |___ MSG_NUM_1 --Msg������1
//  949  *      |___ MSG_NUM_2 --Msg������2
//  950  *      |___ MSG_NUM_3 --Msg������3
//  951  *      |___ MSG_NUM_4 --Msg������4
//  952  *      ...... 
//  953  *      |___ MSG_NUM_15 --Msg������15
//  954  *    in_length
//  955  *      |__д�����ݳ���
//  956  *    *in_buffer
//  957  *      |__д�����ݵĻ�����
//  958  * ���:
//  959  *    �� 
//  960  */

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
//  961 static void LPLD_CAN_WriteData(
//  962     CAN_Type *canx, 
//  963     uint8     msg_num, 
//  964     uint8     in_length, 
//  965     uint8    *in_buffer )
//  966 {
LPLD_CAN_WriteData:
        PUSH     {R2-R7}
//  967   int i;
//  968   uint32 word[2] = {0};  
        ADD      R4,SP,#+0
        MOVS     R5,#+0
        MOVS     R6,#+0
        STM      R4!,{R5,R6}
        SUBS     R4,R4,#+8
//  969   uint8  word_num   = (in_length - 1)/ 4; //��� Msg ��Word ֵ
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        SUBS     R4,R2,#+1
        MOVS     R5,#+4
        SDIV     R4,R4,R5
//  970   uint8  rest_bytes = (in_length - 1)% 4; //���ʣ���ֽڵ�ֵ
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        SUBS     R5,R2,#+1
        MOVS     R6,#+4
        SDIV     R2,R5,R6
        MLS      R2,R2,R6,R5
//  971 
//  972   if(word_num < 1)
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        CMP      R4,#+0
        BNE.N    ??LPLD_CAN_WriteData_0
//  973   {
//  974     for (i = 0; i <= rest_bytes ; i++)
        MOVS     R5,#+0
??LPLD_CAN_WriteData_1:
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        CMP      R2,R5
        BLT.N    ??LPLD_CAN_WriteData_2
//  975     {
//  976       word[word_num] |=  (uint32)in_buffer[i] << (24-(i<<3));
        ADD      R6,SP,#+0
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        LDR      R6,[R6, R4, LSL #+2]
        LDRB     R7,[R5, R3]
        LSLS     R12,R5,#+3
        RSBS     R12,R12,#+24
        LSLS     R7,R7,R12
        ORRS     R6,R7,R6
        ADD      R7,SP,#+0
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        STR      R6,[R7, R4, LSL #+2]
//  977     }
        ADDS     R5,R5,#+1
        B.N      ??LPLD_CAN_WriteData_1
//  978   }
//  979   else
//  980   {
//  981     for (i = 0; i < 4; i++)
??LPLD_CAN_WriteData_0:
        MOVS     R5,#+0
        B.N      ??LPLD_CAN_WriteData_3
//  982     {
//  983       word[word_num-1] |= (uint32)in_buffer[i] << (24-(i<<3));
??LPLD_CAN_WriteData_4:
        ADD      R6,SP,#+0
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        ADDS     R6,R6,R4, LSL #+2
        LDR      R6,[R6, #-4]
        LDRB     R7,[R5, R3]
        LSLS     R12,R5,#+3
        RSBS     R12,R12,#+24
        LSLS     R7,R7,R12
        ORRS     R6,R7,R6
        ADD      R7,SP,#+0
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        ADDS     R7,R7,R4, LSL #+2
        STR      R6,[R7, #-4]
//  984     }
        ADDS     R5,R5,#+1
??LPLD_CAN_WriteData_3:
        CMP      R5,#+4
        BLT.N    ??LPLD_CAN_WriteData_4
//  985     for (i = 0; i <= rest_bytes ; i++)
        MOVS     R5,#+0
        B.N      ??LPLD_CAN_WriteData_5
//  986     {
//  987       word[word_num] |=  (uint32)in_buffer[i+4] << (24-(i<<3));
??LPLD_CAN_WriteData_6:
        ADD      R6,SP,#+0
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        LDR      R6,[R6, R4, LSL #+2]
        ADDS     R7,R5,R3
        LDRB     R7,[R7, #+4]
        LSLS     R12,R5,#+3
        RSBS     R12,R12,#+24
        LSLS     R7,R7,R12
        ORRS     R6,R7,R6
        ADD      R7,SP,#+0
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        STR      R6,[R7, R4, LSL #+2]
//  988     }
        ADDS     R5,R5,#+1
??LPLD_CAN_WriteData_5:
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        CMP      R2,R5
        BGE.N    ??LPLD_CAN_WriteData_6
//  989   }
//  990   canx->MB[msg_num].WORD0 = word[0];
??LPLD_CAN_WriteData_2:
        LDR      R2,[SP, #+0]
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ADDS     R3,R0,R1, LSL #+4
        STR      R2,[R3, #+136]
//  991   canx->MB[msg_num].WORD1 = word[1];
        LDR      R2,[SP, #+4]
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ADDS     R0,R0,R1, LSL #+4
        STR      R2,[R0, #+140]
//  992 }
        POP      {R0,R1,R4-R7}
        BX       LR               ;; return
//  993 
//  994 /*
//  995  * LPLD_CAN_Transmit_Interrupt
//  996  * �ú������ڴ���Msg�����������жϺ���
//  997  * 
//  998  * ����:
//  999  *    canx--����CAN����ͨ��
// 1000  *      |__CAN0 --CAN0��ģ��
// 1001  *      |__CAN1 --CAN1��ģ��
// 1002  *    msg_num ѡ����Ҫ�������жϵ�Msg������
// 1003  *      |___ MSG_NUM_0 --Msg������0
// 1004  *      |___ MSG_NUM_1 --Msg������1
// 1005  *      |___ MSG_NUM_2 --Msg������2
// 1006  *      |___ MSG_NUM_3 --Msg������3
// 1007  *      |___ MSG_NUM_4 --Msg������4
// 1008  *      ...... 
// 1009  *      |___ MSG_NUM_15 --Msg������15
// 1010  * ���:
// 1011  *    �� 
// 1012  */

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
// 1013 static void LPLD_CAN_Transmit_Interrupt(CAN_Type *canx, uint8 msg_num)
// 1014 {
LPLD_CAN_Transmit_Interrupt:
        PUSH     {R3-R5,LR}
        MOVS     R5,R0
        MOVS     R4,R1
// 1015   if(LPLD_CAN_Interrupt_GetFlag(canx,msg_num))
        MOVS     R1,R4
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R5
        BL       LPLD_CAN_Interrupt_GetFlag
        CMP      R0,#+0
        BEQ.N    ??LPLD_CAN_Transmit_Interrupt_0
// 1016   {
// 1017    LPLD_CAN_Interrupt_ClearPending(canx,msg_num);
        MOVS     R1,R4
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R5
        BL       LPLD_CAN_Interrupt_ClearPending
// 1018    if(canx == CAN0)
        LDR.N    R0,??DataTable12_5  ;; 0x40024000
        CMP      R5,R0
        BNE.N    ??LPLD_CAN_Transmit_Interrupt_1
// 1019    {
// 1020     if( CAN0_ISR[msg_num] != NULL)
        LDR.N    R0,??DataTable12_22
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        LDR      R0,[R0, R4, LSL #+2]
        CMP      R0,#+0
        BEQ.N    ??LPLD_CAN_Transmit_Interrupt_0
// 1021     {
// 1022       CAN0_ISR[msg_num]();
        LDR.N    R0,??DataTable12_22
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        LDR      R0,[R0, R4, LSL #+2]
        BLX      R0
// 1023     }
// 1024    }
// 1025    else if (canx == CAN1)
// 1026    {
// 1027     if( CAN1_ISR[msg_num] != NULL)
// 1028     {
// 1029       CAN1_ISR[msg_num]();
// 1030     }
// 1031    }
// 1032    else
// 1033    {
// 1034       return;
// 1035    }
// 1036   }
// 1037 }
??LPLD_CAN_Transmit_Interrupt_0:
??LPLD_CAN_Transmit_Interrupt_2:
        POP      {R0,R4,R5,PC}    ;; return
??LPLD_CAN_Transmit_Interrupt_1:
        LDR.N    R0,??DataTable12_21  ;; 0x400a4000
        CMP      R5,R0
        BNE.N    ??LPLD_CAN_Transmit_Interrupt_3
        LDR.N    R0,??DataTable12_23
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        LDR      R0,[R0, R4, LSL #+2]
        CMP      R0,#+0
        BEQ.N    ??LPLD_CAN_Transmit_Interrupt_0
        LDR.N    R0,??DataTable12_23
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        LDR      R0,[R0, R4, LSL #+2]
        BLX      R0
        B.N      ??LPLD_CAN_Transmit_Interrupt_0
??LPLD_CAN_Transmit_Interrupt_3:
        B.N      ??LPLD_CAN_Transmit_Interrupt_2
// 1038 
// 1039 /*
// 1040  * LPLD_CAN_Receive_Interrupt
// 1041  * �ú������ڴ���Msg�����������жϺ���
// 1042  * 
// 1043  * ����:
// 1044  *    canx--����CAN����ͨ��
// 1045  *      |__CAN0 --CAN0��ģ��
// 1046  *      |__CAN1 --CAN1��ģ��
// 1047  *    msg_num ѡ����Ҫ��������жϵ�Msg������
// 1048  *      |___ MSG_NUM_0 --Msg������0
// 1049  *      |___ MSG_NUM_1 --Msg������1
// 1050  *      |___ MSG_NUM_2 --Msg������2
// 1051  *      |___ MSG_NUM_3 --Msg������3
// 1052  *      |___ MSG_NUM_4 --Msg������4
// 1053  *      ...... 
// 1054  *      |___ MSG_NUM_15 --Msg������15
// 1055  * ���:
// 1056  *    �� 
// 1057  */

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
// 1058 static void LPLD_CAN_Receive_Interrupt(CAN_Type *canx, uint8 msg_num)
// 1059 {
LPLD_CAN_Receive_Interrupt:
        PUSH     {R3-R5,LR}
        MOVS     R4,R0
        MOVS     R5,R1
// 1060   if(LPLD_CAN_Interrupt_GetFlag(canx,msg_num))
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
        BL       LPLD_CAN_Interrupt_GetFlag
        CMP      R0,#+0
        BEQ.N    ??LPLD_CAN_Receive_Interrupt_0
// 1061   {
// 1062     //����������жϻص��������ڴ�ִ��
// 1063     if(canx == CAN0)
        LDR.N    R0,??DataTable12_5  ;; 0x40024000
        CMP      R4,R0
        BNE.N    ??LPLD_CAN_Receive_Interrupt_1
// 1064     {
// 1065       if( CAN0_ISR[msg_num] != NULL)
        LDR.N    R0,??DataTable12_22
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        LDR      R0,[R0, R5, LSL #+2]
        CMP      R0,#+0
        BEQ.N    ??LPLD_CAN_Receive_Interrupt_2
// 1066       {
// 1067           CAN0_ISR[msg_num]();
        LDR.N    R0,??DataTable12_22
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        LDR      R0,[R0, R5, LSL #+2]
        BLX      R0
// 1068       }
// 1069     }
// 1070     else if (canx == CAN1)
// 1071     {
// 1072       if( CAN1_ISR[msg_num] != NULL)
// 1073       {
// 1074           CAN1_ISR[msg_num]();
// 1075       }
// 1076     }
// 1077     else
// 1078     {
// 1079       return;
// 1080     }
// 1081     LPLD_CAN_Interrupt_ClearPending(canx,msg_num);
??LPLD_CAN_Receive_Interrupt_2:
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
        BL       LPLD_CAN_Interrupt_ClearPending
// 1082   }
// 1083 }
??LPLD_CAN_Receive_Interrupt_0:
??LPLD_CAN_Receive_Interrupt_3:
        POP      {R0,R4,R5,PC}    ;; return
??LPLD_CAN_Receive_Interrupt_1:
        LDR.N    R0,??DataTable12_21  ;; 0x400a4000
        CMP      R4,R0
        BNE.N    ??LPLD_CAN_Receive_Interrupt_4
        LDR.N    R0,??DataTable12_23
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        LDR      R0,[R0, R5, LSL #+2]
        CMP      R0,#+0
        BEQ.N    ??LPLD_CAN_Receive_Interrupt_2
        LDR.N    R0,??DataTable12_23
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        LDR      R0,[R0, R5, LSL #+2]
        BLX      R0
        B.N      ??LPLD_CAN_Receive_Interrupt_2
??LPLD_CAN_Receive_Interrupt_4:
        B.N      ??LPLD_CAN_Receive_Interrupt_3
// 1084 
// 1085 /*
// 1086  * LPLD_CAN_CAN0_Interrupt_Handler
// 1087  * CAN0�жϴ�����
// 1088  * 
// 1089  * ����:
// 1090  *    CAN_MSGOBJ_InitTypeDef *CAN_MSGOBJ_Ptr
// 1091  *                           ��ѯ���16��Msg�Ľṹ��
// 1092  * ���:
// 1093  *    �� 
// 1094  */

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
// 1095 static void LPLD_CAN_CAN0_Interrupt_Handler(CAN_MSGOBJ_InitTypeDef *CAN_MSGOBJ_Ptr)
// 1096 {
LPLD_CAN_CAN0_Interrupt_Handler:
        PUSH     {R3-R5,LR}
        MOVS     R4,R0
// 1097   uint8 msg_obj;
// 1098   uint8 direction;
// 1099   CAN_Type *canx_ptr;
// 1100 
// 1101   for (msg_obj = 0; msg_obj < MSG_MAX_NO; msg_obj++)
        MOVS     R5,#+0
        B.N      ??LPLD_CAN_CAN0_Interrupt_Handler_0
// 1102   {
// 1103     canx_ptr  = (CAN_MSGOBJ_Ptr + msg_obj)->CAN_Canx;
// 1104     direction = (CAN_MSGOBJ_Ptr + msg_obj)->CAN_MsgDirection;
// 1105     if(direction == CAN_MSGOBJ_DIR_TX)
// 1106     {
// 1107       LPLD_CAN_Transmit_Interrupt(canx_ptr,msg_obj);
// 1108     }
// 1109     else if(direction == CAN_MSGOBJ_DIR_RX)
??LPLD_CAN_CAN0_Interrupt_Handler_1:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        CMP      R1,#+1
        BNE.N    ??LPLD_CAN_CAN0_Interrupt_Handler_2
// 1110     {
// 1111       LPLD_CAN_Receive_Interrupt(canx_ptr,msg_obj);
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        BL       LPLD_CAN_Receive_Interrupt
// 1112     }
??LPLD_CAN_CAN0_Interrupt_Handler_2:
        ADDS     R5,R5,#+1
??LPLD_CAN_CAN0_Interrupt_Handler_0:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+16
        BGE.N    ??LPLD_CAN_CAN0_Interrupt_Handler_3
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        LSLS     R0,R5,#+4
        LDR      R0,[R0, R4]
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R1,R4,R5, LSL #+4
        LDRB     R1,[R1, #+9]
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        CMP      R1,#+2
        BNE.N    ??LPLD_CAN_CAN0_Interrupt_Handler_1
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        BL       LPLD_CAN_Transmit_Interrupt
        B.N      ??LPLD_CAN_CAN0_Interrupt_Handler_2
// 1113   }
// 1114 }
??LPLD_CAN_CAN0_Interrupt_Handler_3:
        POP      {R0,R4,R5,PC}    ;; return
// 1115 
// 1116 /*
// 1117  *  LPLD_CAN_CAN1_Interrupt_Handler
// 1118  *  CAN1�жϴ�����
// 1119  * 
// 1120  * ����:
// 1121  *    CAN_MSGOBJ_InitTypeDef *CAN_MSGOBJ_Ptr
// 1122  *                           ��ѯ���16��Msg�Ľṹ��
// 1123  * ���:
// 1124  *    �� 
// 1125  */

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
// 1126 static void LPLD_CAN_CAN1_Interrupt_Handler(CAN_MSGOBJ_InitTypeDef *CAN_MSGOBJ_Ptr)
// 1127 {
LPLD_CAN_CAN1_Interrupt_Handler:
        PUSH     {R3-R5,LR}
        MOVS     R4,R0
// 1128   uint8 msg_obj;
// 1129   uint8 direction;
// 1130   CAN_Type *canx_ptr;
// 1131 
// 1132   for (msg_obj = 0; msg_obj < MSG_MAX_NO; msg_obj++)
        MOVS     R5,#+0
        B.N      ??LPLD_CAN_CAN1_Interrupt_Handler_0
// 1133   {
// 1134     canx_ptr  = (CAN_MSGOBJ_Ptr + msg_obj)->CAN_Canx;
// 1135     direction = (CAN_MSGOBJ_Ptr + msg_obj)->CAN_MsgDirection;
// 1136 
// 1137     if(direction == CAN_MSGOBJ_DIR_TX)
// 1138     {
// 1139       LPLD_CAN_Transmit_Interrupt(canx_ptr,msg_obj);
// 1140     }
// 1141     else if(direction == CAN_MSGOBJ_DIR_RX)
??LPLD_CAN_CAN1_Interrupt_Handler_1:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        CMP      R1,#+1
        BNE.N    ??LPLD_CAN_CAN1_Interrupt_Handler_2
// 1142     {
// 1143       LPLD_CAN_Receive_Interrupt(canx_ptr,msg_obj);
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        BL       LPLD_CAN_Receive_Interrupt
// 1144     }
??LPLD_CAN_CAN1_Interrupt_Handler_2:
        ADDS     R5,R5,#+1
??LPLD_CAN_CAN1_Interrupt_Handler_0:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+16
        BGE.N    ??LPLD_CAN_CAN1_Interrupt_Handler_3
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        LSLS     R0,R5,#+4
        LDR      R0,[R0, R4]
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R1,R4,R5, LSL #+4
        LDRB     R1,[R1, #+9]
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        CMP      R1,#+2
        BNE.N    ??LPLD_CAN_CAN1_Interrupt_Handler_1
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        BL       LPLD_CAN_Transmit_Interrupt
        B.N      ??LPLD_CAN_CAN1_Interrupt_Handler_2
// 1145   }
// 1146 }
??LPLD_CAN_CAN1_Interrupt_Handler_3:
        POP      {R0,R4,R5,PC}    ;; return
// 1147 
// 1148 /*
// 1149  * CAN0�жϴ�����
// 1150  * �������ļ�startup_K60.s�е��ж����������
// 1151  * �û������޸ģ������Զ������Ӧͨ���жϺ���
// 1152  */

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
// 1153 void CAN0_MESS_IRQHandler(void)
// 1154 {
CAN0_MESS_IRQHandler:
        PUSH     {R7,LR}
// 1155 #if (UCOS_II > 0u)
// 1156   OS_CPU_SR  cpu_sr = 0u;
// 1157   OS_ENTER_CRITICAL(); //��֪ϵͳ��ʱ�Ѿ��������жϷ����Ӻ���
// 1158   OSIntEnter();
// 1159   OS_EXIT_CRITICAL();
// 1160 #endif
// 1161   LPLD_CAN_CAN0_Interrupt_Handler(flexcan_msgobj_param_t);
        LDR.N    R0,??DataTable12_25
        BL       LPLD_CAN_CAN0_Interrupt_Handler
// 1162 #if (UCOS_II > 0u)
// 1163   OSIntExit();          //��֪ϵͳ��ʱ�����뿪�жϷ����Ӻ���
// 1164 #endif
// 1165 }      
        POP      {R0,PC}          ;; return
// 1166 //29:  CAM 0 OR'ed Message buffer (0-15)
// 1167 

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
// 1168 void CAN0_BUS_OFF_IRQHandler(void){}   //30:  CAM 0 Bus Off
CAN0_BUS_OFF_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
// 1169 void CAN0_ERR_IRQHandler(void){}       //31:  CAM 0 Error
CAN0_ERR_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
// 1170 void CAN0_TW_IRQHandler(void){}        //32:  CAM 0 Transmit Warning
CAN0_TW_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
// 1171 void CAN0_RW_IRQHandler(void){}        //33:  CAM 0 Receive Warning
CAN0_RW_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
// 1172 void CAN0_WAKE_UP_IRQHandler(void){}   //34:  CAM 0 WakeUp
CAN0_WAKE_UP_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
// 1173 void CAN0_IMEU_IRQHandler(void){}      //35:  CAM 0 Individual Matching Elements Update (IMEU)
CAN0_IMEU_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
// 1174 void CAN0_LR_IRQHandler(void){}        //36:  CAM 0 Lost receive
CAN0_LR_IRQHandler:
        BX       LR               ;; return
// 1175 
// 1176 /*
// 1177  * CAN1�жϴ�����
// 1178  * �������ļ�startup_K60.s�е��ж����������
// 1179  * �û������޸ģ������Զ������Ӧͨ���жϺ���
// 1180  */

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
// 1181 void CAN1_MESS_IRQHandler(void)
// 1182 {
CAN1_MESS_IRQHandler:
        PUSH     {R7,LR}
// 1183 #if (UCOS_II > 0u)
// 1184   OS_CPU_SR  cpu_sr = 0u;
// 1185   OS_ENTER_CRITICAL(); //��֪ϵͳ��ʱ�Ѿ��������жϷ����Ӻ���
// 1186   OSIntEnter();
// 1187   OS_EXIT_CRITICAL();
// 1188 #endif
// 1189   LPLD_CAN_CAN1_Interrupt_Handler(flexcan_msgobj_param_t);
        LDR.N    R0,??DataTable12_25
        BL       LPLD_CAN_CAN1_Interrupt_Handler
// 1190 #if (UCOS_II > 0u)
// 1191   OSIntExit();          //��֪ϵͳ��ʱ�����뿪�жϷ����Ӻ���
// 1192 #endif
// 1193 }
        POP      {R0,PC}          ;; return

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12:
        DC32     0xe000e100

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_1:
        DC32     0xe000e180

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_2:
        DC32     ?_0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_3:
        DC32     0xf4241

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_4:
        DC32     0x40065000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_5:
        DC32     0x40024000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_6:
        DC32     0x4004803c

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_7:
        DC32     0x40048030

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_8:
        DC32     0x4004a048

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_9:
        DC32     0x40049030

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_10:
        DC32     0x4004a04c

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_11:
        DC32     0x40049034

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_12:
        DC32     0x4004b044

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_13:
        DC32     0x4004d060

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_14:
        DC32     0x4004b040

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_15:
        DC32     0x4004d064

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_16:
        DC32     0x186a0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_17:
        DC32     0x3d090

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_18:
        DC32     0x7a120

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_19:
        DC32     0xf4240

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_20:
        DC32     g_bus_clock

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_21:
        DC32     0x400a4000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_22:
        DC32     CAN0_ISR

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_23:
        DC32     CAN1_ISR

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_24:
        DC32     0x1ffc0000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_25:
        DC32     flexcan_msgobj_param_t
// 1194 //37:  CAM 1 OR'ed Message buffer (0-15)
// 1195 

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
// 1196 void CAN1_BUS_OFF_IRQHandler(void){}   //38:  CAM 1 Bus Off
CAN1_BUS_OFF_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
// 1197 void CAN1_ERR_IRQHandler(void){}       //39:  CAM 1 Error
CAN1_ERR_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
// 1198 void CAN1_TW_IRQHandler(void){}        //40:  CAM 1 Transmit Warning
CAN1_TW_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
// 1199 void CAN1_RW_IRQHandler(void){}        //41:  CAM 1 Receive Warning
CAN1_RW_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
// 1200 void CAN1_WAKE_UP_IRQHandler(void){}   //42:  CAM 1 WakeUp
CAN1_WAKE_UP_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
// 1201 void CAN1_IMEU_IRQHandler(void){}      //43:  CAM 1 Individual Matching Elements Update (IMEU)
CAN1_IMEU_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(1)
        THUMB
// 1202 void CAN1_LR_IRQHandler(void){}        //44:  CAM 1 Lost receive
CAN1_LR_IRQHandler:
        BX       LR               ;; return

        SECTION `.iar_vfe_header`:DATA:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
?_0:
        DATA
        DC8 "D:\\2019.IAR\\lib\\LPLD\\HW\\HW_CAN.c"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC32 0
        DC8 0, 0, 0, 0

        END
// 1203 
// 1204 
// 
//   384 bytes in section .bss
//    44 bytes in section .rodata
// 2 546 bytes in section .text
// 
// 2 546 bytes of CODE  memory
//    44 bytes of CONST memory
//   384 bytes of DATA  memory
//
//Errors: none
//Warnings: none
