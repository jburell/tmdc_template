#include <windows.h>
#include "fmod.h"
#include "music.h"

// 10 events should be plenty for one frame
#define MUS_MAXEVENTS 10

mus_event mus_queue[MUS_MAXEVENTS];
int mus_head=0, mus_tail=0, mus_numevents=0;

FMUSIC_MODULE *mus_mod = NULL;

// returns next event or NULL if queue is empty
mus_event* mus_dequeue()
{
	mus_event *r;

	if(mus_numevents){
		r = &mus_queue[mus_head];
		mus_head = (mus_head + 1) % MUS_MAXEVENTS;
		mus_numevents--;
		return(r);
	}
	return(NULL);
}

// success=1, failure=0
int mus_enqueue(mus_event *e)
{
	if(mus_numevents < MUS_MAXEVENTS){
		CopyMemory(&mus_queue[mus_tail], e, sizeof(mus_event));
		mus_tail = (mus_tail + 1) % MUS_MAXEVENTS;
		mus_numevents++;
		return(1);
	}
	return(0);
}

void F_CALLBACKAPI mus_synccallback(FMUSIC_MODULE *mod, unsigned char param)
{
	static mus_event event;

	event.param = param;
	event.time = FMUSIC_GetTime(mod);
	event.order = FMUSIC_GetOrder(mod);
	event.pattern = FMUSIC_GetPattern(mod);
	event.row = FMUSIC_GetRow(mod);
	mus_enqueue(&event);
}

// success=1, failure=0
int mus_stop()
{
	int r=0;

	if(mus_mod){
		r = FMUSIC_FreeSong(mus_mod);
		mus_mod = NULL;
	}
	return(r);
}

// success=1, failure=0
int mus_play(const char *path)
{
	static int initialized=0;
	int r;

	if(!initialized){
		// FSOUND_INIT_DONTLATENCYADJUST so we get correct row numbers in the callback
		if(!FSOUND_Init(44100, 32, FSOUND_INIT_DONTLATENCYADJUST)) return(0);
		initialized = 1;
	}

	if(mus_mod){
		FMUSIC_FreeSong(mus_mod);
		mus_mod = NULL;
	}

	if(!(mus_mod = FMUSIC_LoadSong(path))) return(0);	

	r  = FMUSIC_SetZxxCallback(mus_mod, mus_synccallback);
	r &= FMUSIC_PlaySong(mus_mod);

	
	// Just before credits
	//FMUSIC_SetOrder(mus_mod,9);

	if(!r){
		mus_stop();
		return(0);
	}
	return(1);
}
