#ifndef FFMPEG_PLAY_PLAYER_H
#define FFMPEG_PLAY_PLAYER_H

void init_ffplay(char *filename, void* hwnd, int width, int height);
void stop_ffplay(void);

#endif
