#ifndef __VIR_BUZZER_H__
#define __VIR_BUZZER_H__

/*********************************************************************************************************
  Ãù½ÐÄ£Ê½
*********************************************************************************************************/

typedef enum
{
	VIR_BUZZER_SHORT = 1,
    VIR_BUZZER_TWO_SHORT,
    VIR_BUZZER_LONG
}TweetMode;

void vir_buzzer_init(void);
void vir_buzzer_tweet(TweetMode mode);

#endif
