#ifndef __MUSIC_H
#define __MUSIC_H

typedef struct mus_event_s
{
	unsigned char param;
	int time;		// milliseconds since song was started
	int order;		// position in pattern sequence
	int pattern;	// pattern no.
	int row;		// row (in pattern, not total)
} mus_event;

int mus_play(const char *path);
int mus_stop(void);

// returns next event or NULL if queue is empty
mus_event* mus_dequeue(void);

#endif
