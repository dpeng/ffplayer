#ifndef FFMPEG_PLAY_PLAYER_H
#define FFMPEG_PLAY_PLAYER_H

#include <inttypes.h>
int ffplay_init(char *filename, void* hwnd, int width, int height);
void ffplay_stop(void);
void ffplay_pause(void);
void ffplay_av_log_set_callback(void *func);
int ffplay_get_stream_totaltime(void);
double ffplay_get_stream_curtime(void);
void ffplay_seek(double pos);

#endif
