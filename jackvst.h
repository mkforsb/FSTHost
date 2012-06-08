#ifndef __jack_vst_h__
#define __jack_vst_h__

#include <sys/types.h>
#include <sys/time.h>
#include <jack/jack.h>
#include <jack/ringbuffer.h>
#include <jack/session.h>
#include <math.h>

#include "fst.h"

typedef struct _JackVST JackVST;

struct _JackVST {
    jack_client_t *client;
    FSTHandle*     handle;
    FST*           fst;
    short          numIns;
    short          numOuts;
    float**        ins;
    float**        outs;
    jack_port_t*   midi_inport;
    jack_port_t*   midi_outport;
    jack_port_t**  inports;
    jack_port_t**  outports;
    int            channel;
    bool           bypassed;
    short          with_editor;
    double         tempo;
    float          volume; /* wehere 0.0 mean silence */

    /* For VST/i support */
    int	   want_midi_in;
    struct VstMidiEvent* event_array;
    struct VstEvents*    events;

    int uuid;
    jack_session_event_t *session_event;

    /* For VST midi effects & synth source (like audio to midi VSTs) support */
    jack_ringbuffer_t* ringbuffer;
};

static inline float 
jvst_set_volume(JackVST* jvst, short volume) { jvst->volume = powf(volume / 63.0f, 2); }

static short
jvst_get_volume(JackVST* jvst)
{
	short ret = roundf(sqrtf(jvst->volume) * 63.0f);

	if (ret < 0) ret = 0;
	if (ret > 127) ret = 127;
	return ret;
}

#define MIDI_EVENT_MAX 1024

#endif /* __jack_vst_h__ */
