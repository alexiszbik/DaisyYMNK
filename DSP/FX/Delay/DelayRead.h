//
//  DelayRead.hpp
//  AudioStuff
//
//  Created by Alexis ZBIK on 26/02/2019.
//  Copyright © 2019 Bleass. All rights reserved.
//

#ifndef DelayRead_h
#define DelayRead_h

#include "DelayWrite.h"
#include "Memory.h"

class DelayRead {
    
public :
    
    DelayRead() {
    }
    
    float x_sr;         /* samples per msec */
    
    void init(float sampleRate) {
        x_sr = sampleRate;
    }
    
    void process(float *times, float *out, DelayWrite *writerBlock, int n)
    {
        DelayWrite::t_delwritectl *ctl = &(writerBlock->x_cspace);
        
        int nsamps = ctl->c_n;
        float limit = nsamps - n;
        
        float *vp = ctl->c_vec;
        int phase = ctl->c_phase;
        
        float zerodel = 0;
        
        for (int i = 0 ; i < n; i++) {
            
            float delsamps = (x_sr * *times++ - zerodel);
            
            if (!(delsamps >= 1.00001f)) {    /* too small or NAN */
                delsamps = 1.00001f;
            }
            if (delsamps > limit) {       /* too big */
                delsamps = limit;
            }
            
            int idelsamps = (int)floor(delsamps);
            float frac = delsamps - (float)idelsamps;
            
            idelsamps = idelsamps + (64 - i);
            
            int samplePosition = (phase - idelsamps);
            
            if (samplePosition < XTRASAMPS) {
                samplePosition = samplePosition + nsamps;
            }
            
            float* read = vp + samplePosition;
            
            float d = read[-3];
            float c = read[-2];
            float b = read[-1];
            float a = read[0];
            float cminusb = c-b;
            
            out[i] = b + frac * (cminusb - 0.1666667f * (1.-frac) * ((d - a - 3.0f * cminusb) * frac + (d + 2.0f*a - 3.0f*b)));
        }
    }
};

#endif /* DelayRead_h */
