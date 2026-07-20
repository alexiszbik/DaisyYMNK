
// This is a modified version of delwrite && delread4 from pure data

#ifndef DelayWrite_h
#define DelayWrite_h

#define XTRASAMPS 4
#define SAMPBLK 4
#define DEFDELVS 64

#include <cstring>
#include "BufferMath.h"
#include "SdramPool.h"

class DelayWrite  {
    public :
    
    typedef struct delwritectl
    {
        int c_n;
        float* c_vec;
        int c_phase;
        
        ~delwritectl() {
        }
    } t_delwritectl;
    
    float x_sampleRate;
    float x_deltime;  /* delay in msec (added by Mathieu Bouchard) */
    t_delwritectl x_cspace;
    int x_sortno;   /* DSP sort number at which this was last put on chain */
    int x_rsortno;  /* DSP sort # for first delread or write in chain */
    int x_vecsize;  /* vector size for delread~ to use */
    float x_f;
    
    unsigned long sizeOfArray = 0;
    
    void *resizebytes(void * /*old*/, size_t /*oldsize*/, size_t newsize)
    {
        if (newsize < 1) newsize = 1;
        void *ret = sdram_alloc(newsize);
        sizeOfArray = newsize / sizeof(float);
        return ret;
    }
    
    void sigdelwrite_updatesr(float sr) /* added by Mathieu Bouchard */
    {
        int nsamps = x_deltime * sr * (float)(0.001f);
        if (nsamps < 1) nsamps = 1;
        nsamps += ((- nsamps) & (SAMPBLK - 1));
        nsamps += DEFDELVS;
        if (x_cspace.c_n != nsamps)
        {
            x_cspace.c_vec = (float *)resizebytes(x_cspace.c_vec,
                                                 (x_cspace.c_n + XTRASAMPS) * sizeof(float),
                                                 (nsamps + XTRASAMPS) * sizeof(float));
            x_cspace.c_n = nsamps;
            x_cspace.c_phase = XTRASAMPS;
        }
    }
    
    /* routine to check that all delwrites/delreads/vds have same vecsize */
    void sigdelwrite_checkvecsize(int vecsize)
    {
        if (x_rsortno != 0)
        {
            x_vecsize = vecsize;
            x_rsortno = 0;
        }
        /*
         LATER this should really check sample rate and blocking, once that is
         supported.  Probably we don't actually care about vecsize.
         For now just suppress this check. */
        /*
    #if 0
        else if (vecsize != x_vecsize)
            pd_error(x, "delread/delwrite/vd vector size mismatch");
    #endif
         */
    }
    
    void init(float msec, float sampleRate)
    {
        x_sampleRate = sampleRate;
        x_deltime = msec;
        x_cspace.c_n = 0;
        x_cspace.c_vec = (float *)sdram_alloc(XTRASAMPS * sizeof(float));
        x_vecsize = 0;
        x_f = 0;
        
        x_sortno = 0;
        sigdelwrite_checkvecsize(sampleRate * 1); //number of size - 1 sec in 44100
        sigdelwrite_updatesr(sampleRate); //check sample rate
    }
    
    void clear()
    {
        BufferMath::clear((float*)x_cspace.c_vec, sizeOfArray);
    }
    
    void process(float* in, int n)
    {
        t_delwritectl *c = &x_cspace;
        
        int phase = c->c_phase;
        int nsamps = c->c_n;
        
        float *vp = c->c_vec;
        float *bp = vp + phase;
        float *ep = vp + (nsamps + XTRASAMPS);
        
        // on copie 64 par 64 ... quand on arrive à la fin ... et on copie les 4 premiers dans les 4 derniers ...
        // c'est une espèce de ring buffer avec 4 sample de décallage
    
        phase += n;
        //ME
        if (phase > nsamps) {
            
            int remainSamplesCount = nsamps - c->c_phase;
            memcpy(bp, in, (remainSamplesCount + XTRASAMPS)*sizeof(float));
            
            int samplesToStoreInBegin = phase - nsamps;
            memcpy(vp, in + remainSamplesCount, samplesToStoreInBegin*sizeof(float));
            
            phase = samplesToStoreInBegin;
            
            vp[0] = ep[-4];
            vp[1] = ep[-3];
            vp[2] = ep[-2];
            vp[3] = ep[-1];
            
        } else {
            memcpy(bp, in, n*sizeof(float));
        }
        
        //END
        c->c_phase = phase;
    }
    
};


#endif /* DelayWrite_h */


