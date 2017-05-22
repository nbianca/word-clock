#ifndef _BUZZER_H_
#define _BUZZER_H_

/**
 * Gets buzzer's status.
 *
 * @return 1 - emits sound; 0 - does not emit.
 */
int get_buzzer();

/**
 * Sets buzzer's status.
 *
 * @param status 1 - emits sound; 0 - does not emit.
 */
void set_buzzer(int status);

#endif // _BUZZER_H_
