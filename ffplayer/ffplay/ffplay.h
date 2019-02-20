#ifndef FFMPEG_PLAY_PLAYER_H
#define FFMPEG_PLAY_PLAYER_H

#include <inttypes.h>

typedef struct MediaInfo {
	int64_t duration;
	char *artist;
	char *title;
	char *album;
	char *genre;
}MediaInfo;

int ffplay_init(char* filename, int width, int height);
int ffplay_play(void* hwnd);
void ffplay_stop(void);
void ffplay_pause(void);
void ffplay_av_log_set_callback(void *func);
int ffplay_get_stream_totaltime(void);
double ffplay_get_stream_curtime(void);
void ffplay_seek(double pos);
void ffplay_toggle_display(void);
void ffplay_toggle_update_volume(int sign, double step);
double ffplay_toggle_get_volume(void);
void ffplay_toggle_set_init_volume(int volume);
void ffplay_toggle_mute(void);
void ffplay_step_to_next_frame(void);
int64_t ffplay_get_meida_duration(void);
void ffplay_get_media_info(MediaInfo *mediainfo);
#endif
