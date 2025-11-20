#ifndef __BUZZER_H
#define __BUZZER_H

/* 音符频率定义 */
#define P	0
#define L1	262
#define L1_	277
#define L2	294
#define L2_	311
#define L3	330
#define L4	349
#define L4_	370
#define L5	392
#define L5_	415
#define L6	440
#define L6_	466
#define L7	494
#define M1	523
#define M1_	554
#define M2	587
#define M2_	622
#define M3	659
#define M4	698
#define M4_	740
#define M5	784
#define M5_	831
#define M6	880
#define M6_	932
#define M7	988
#define H1	1046
#define H1_	1109
#define H2	1175
#define H2_	1245
#define H3	1318
#define H4	1397
#define H4_	1480
#define H5	1568
#define H5_	1661
#define H6	1760
#define H6_	1865
#define H7	1976

void Buzzer_PlayNote(uint16_t freq);
void Music_Update(void);
void Music_Start(void);
void Music_Pause(void);
void Music_Resume(void);
void Music_Stop(void);


#endif
