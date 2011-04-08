#ifndef __VINYLCONTROLPROXY_H__
#define __VINYLCONTROLPROXY_H__

#include "vinylcontrol.h"
#include "soundmanagerutil.h"

//#include "vinylcontrolscratchlib.h"

class VinylControlProxy : public VinylControl, public AudioDestination
{
    public:
        VinylControlProxy(ConfigObject<ConfigValue> *pConfig, const char *_group);
        ~VinylControlProxy();
        bool isEnabled();
        void AnalyseSamples(const short* samples, size_t size);
        void ToggleVinylControl(bool enable);
        void run();
        float getSpeed();
        virtual void receiveBuffer(AudioInput input, const short *pBuffer, unsigned int iNumFrames);
    protected:
        VinylControl* m_pVinylControl; //Pointer to active VinylControl object
};

#endif
