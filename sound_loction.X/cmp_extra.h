#ifndef __CMP_EXTRA_H    /* Guard against multiple inclusion */
#define __CMP_EXTRA_H

#define CMP3Open(config)  (CM3CON = (config)&0xFFFF, CMSTATCLR = _CMSTAT_SIDL_MASK, CMSTATSET = ((config)>>29))

#define CMP_POS_INPUT_C3IN_POS  (0)
#define CMP3_NEG_INPUT_IVREF    (3 << _CM3CON_CCH_POSITION)

#define mCMP3ClearIntFlag()                     (IFS1CLR = _IFS1_CMP3IF_MASK)
#define mCMP3SetIntPriority(priority)           ( IPC7CLR = _IPC7_CMP3IP_MASK, IPC7SET = ((priority) << _IPC7_CMP3IP_POSITION))
#define mCMP3SetIntSubPriority(subPriority)     ( IPC7CLR = _IPC7_CMP3IS_MASK, IPC7SET = ((subPriority) << _IPC7_CMP3IS_POSITION))
#define mCMP3IntEnable(enable)                  (IEC1CLR = _IEC1_CMP3IE_MASK, IEC1SET = ((enable) << _IEC1_CMP3IE_POSITION))

#define CMP3ConfigInt(config)  ( mCMP3ClearIntFlag(), mCMP3SetIntPriority((config)&7), mCMP3SetIntSubPriority(((config) >> 4)&3), mCMP3IntEnable((config) >> 15) )

void cmp_init();

#endif /* __CMP_EXTRA_H */
