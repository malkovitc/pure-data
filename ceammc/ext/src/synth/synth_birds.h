/* ------------------------------------------------------------
author: "Pierre Cochard"
name: "synth.birds"
Code generated with Faust 2.15.0 (https://faust.grame.fr)
Compilation options: cpp, -scal -ftz 0
------------------------------------------------------------ */

#ifndef  __synth_birds_H__
#define  __synth_birds_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __dsp__
#define __dsp__

#include <string>
#include <vector>

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

class UI;
struct Meta;

/**
 * DSP memory manager.
 */

struct dsp_memory_manager {
    
    virtual ~dsp_memory_manager() {}
    
    virtual void* allocate(size_t size) = 0;
    virtual void destroy(void* ptr) = 0;
    
};

/**
* Signal processor definition.
*/

class dsp {

    public:

        dsp() {}
        virtual ~dsp() {}

        /* Return instance number of audio inputs */
        virtual int getNumInputs() = 0;
    
        /* Return instance number of audio outputs */
        virtual int getNumOutputs() = 0;
    
        /**
         * Trigger the ui_interface parameter with instance specific calls
         * to 'addBtton', 'addVerticalSlider'... in order to build the UI.
         *
         * @param ui_interface - the user interface builder
         */
        virtual void buildUserInterface(UI* ui_interface) = 0;
    
        /* Returns the sample rate currently used by the instance */
        virtual int getSampleRate() = 0;
    
        /**
         * Global init, calls the following methods:
         * - static class 'classInit': static tables initialization
         * - 'instanceInit': constants and instance state initialization
         *
         * @param samplingRate - the sampling rate in Hertz
         */
        virtual void init(int samplingRate) = 0;

        /**
         * Init instance state
         *
         * @param samplingRate - the sampling rate in Hertz
         */
        virtual void instanceInit(int samplingRate) = 0;

        /**
         * Init instance constant state
         *
         * @param samplingRate - the sampling rate in Hertz
         */
        virtual void instanceConstants(int samplingRate) = 0;
    
        /* Init default control parameters values */
        virtual void instanceResetUserInterface() = 0;
    
        /* Init instance state (delay lines...) */
        virtual void instanceClear() = 0;
 
        /**
         * Return a clone of the instance.
         *
         * @return a copy of the instance on success, otherwise a null pointer.
         */
        virtual dsp* clone() = 0;
    
        /**
         * Trigger the Meta* parameter with instance specific calls to 'declare' (key, value) metadata.
         *
         * @param m - the Meta* meta user
         */
        virtual void metadata(Meta* m) = 0;
    
        /**
         * DSP instance computation, to be called with successive in/out audio buffers.
         *
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         *
         */
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) = 0;
    
        /**
         * DSP instance computation: alternative method to be used by subclasses.
         *
         * @param date_usec - the timestamp in microsec given by audio driver.
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         *
         */
        virtual void compute(double /*date_usec*/, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }
       
};

/**
 * Generic DSP decorator.
 */

class decorator_dsp : public dsp {

    protected:

        dsp* fDSP;

    public:

        decorator_dsp(dsp* dsp = 0):fDSP(dsp) {}
        virtual ~decorator_dsp() { delete fDSP; }

        virtual int getNumInputs() { return fDSP->getNumInputs(); }
        virtual int getNumOutputs() { return fDSP->getNumOutputs(); }
        virtual void buildUserInterface(UI* ui_interface) { fDSP->buildUserInterface(ui_interface); }
        virtual int getSampleRate() { return fDSP->getSampleRate(); }
        virtual void init(int samplingRate) { fDSP->init(samplingRate); }
        virtual void instanceInit(int samplingRate) { fDSP->instanceInit(samplingRate); }
        virtual void instanceConstants(int samplingRate) { fDSP->instanceConstants(samplingRate); }
        virtual void instanceResetUserInterface() { fDSP->instanceResetUserInterface(); }
        virtual void instanceClear() { fDSP->instanceClear(); }
        virtual decorator_dsp* clone() { return new decorator_dsp(fDSP->clone()); }
        virtual void metadata(Meta* m) { fDSP->metadata(m); }
        // Beware: subclasses usually have to overload the two 'compute' methods
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(count, inputs, outputs); }
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(date_usec, count, inputs, outputs); }
    
};

/**
 * DSP factory class.
 */

class dsp_factory {
    
    protected:
    
        // So that to force sub-classes to use deleteDSPFactory(dsp_factory* factory);
        virtual ~dsp_factory() {}
    
    public:
    
        virtual std::string getName() = 0;
        virtual std::string getSHAKey() = 0;
        virtual std::string getDSPCode() = 0;
        virtual std::string getCompileOptions() = 0;
        virtual std::vector<std::string> getLibraryList() = 0;
        virtual std::vector<std::string> getIncludePathnames() = 0;
    
        virtual dsp* createDSPInstance() = 0;
    
        virtual void setMemoryManager(dsp_memory_manager* manager) = 0;
        virtual dsp_memory_manager* getMemoryManager() = 0;
    
};

/**
 * On Intel set FZ (Flush to Zero) and DAZ (Denormals Are Zero)
 * flags to avoid costly denormals.
 */

#ifdef __SSE__
    #include <xmmintrin.h>
    #ifdef __SSE2__
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8040)
    #else
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8000)
    #endif
#else
    #define AVOIDDENORMALS
#endif

#endif
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __UI_H__
#define __UI_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

/*******************************************************************************
 * UI : Faust DSP User Interface
 * User Interface as expected by the buildUserInterface() method of a DSP.
 * This abstract class contains only the method that the Faust compiler can
 * generate to describe a DSP user interface.
 ******************************************************************************/

struct Soundfile;

class UI
{

    public:

        UI() {}

        virtual ~UI() {}

        // -- widget's layouts

        virtual void openTabBox(const char* label) = 0;
        virtual void openHorizontalBox(const char* label) = 0;
        virtual void openVerticalBox(const char* label) = 0;
        virtual void closeBox() = 0;

        // -- active widgets

        virtual void addButton(const char* label, FAUSTFLOAT* zone) = 0;
        virtual void addCheckButton(const char* label, FAUSTFLOAT* zone) = 0;
        virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;
        virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;
        virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;

        // -- passive widgets

        virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) = 0;
        virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) = 0;
    
        // -- soundfiles
    
        virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) = 0;

        // -- metadata declarations

        virtual void declare(FAUSTFLOAT*, const char*, const char*) {}
};

#endif
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __meta__
#define __meta__

struct Meta
{
    virtual void declare(const char* key, const char* value) = 0;
    virtual ~Meta() {};
};

#endif
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/
 
#ifndef __misc__
#define __misc__

#include <algorithm>
#include <map>
#include <cstdlib>
#include <string.h>


using std::max;
using std::min;

struct XXXX_Meta : std::map<const char*, const char*>
{
    void declare(const char* key, const char* value) { (*this)[key] = value; }
};

struct MY_Meta : Meta, std::map<const char*, const char*>
{
    void declare(const char* key, const char* value) { (*this)[key] = value; }
};

static int lsr(int x, int n) { return int(((unsigned int)x) >> n); }

static int int2pow2(int x) { int r = 0; while ((1<<r) < x) r++; return r; }

static long lopt(char* argv[], const char* name, long def)
{
	int	i;
    for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return std::atoi(argv[i+1]);
	return def;
}

static bool isopt(char* argv[], const char* name)
{
	int	i;
	for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return true;
	return false;
}

static const char* lopts(char* argv[], const char* name, const char* def)
{
	int	i;
	for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return argv[i+1];
	return def;
}

#endif


#include "ceammc_faust.h"

using namespace ceammc::faust;

#ifdef FAUST_MACRO
// clang-format off
// clang-format on
#endif

#define sym(name) xsym(name)
#define xsym(name) #name

// clang-format off
#ifndef FAUST_MACRO
struct synth_birds : public dsp {
};
#endif
// clang-format on

#ifdef FAUST_MACRO
// clang-format off
#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <math.h>


class synth_birdsSIG0 {
	
  private:
	
	int iRec14[2];
	
  public:
	
	int getNumInputssynth_birdsSIG0() {
		return 0;
		
	}
	int getNumOutputssynth_birdsSIG0() {
		return 1;
		
	}
	int getInputRatesynth_birdsSIG0(int channel) {
		int rate;
		switch (channel) {
			default: {
				rate = -1;
				break;
			}
			
		}
		return rate;
		
	}
	int getOutputRatesynth_birdsSIG0(int channel) {
		int rate;
		switch (channel) {
			case 0: {
				rate = 0;
				break;
			}
			default: {
				rate = -1;
				break;
			}
			
		}
		return rate;
		
	}
	
	void instanceInitsynth_birdsSIG0(int samplingFreq) {
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			iRec14[l16] = 0;
			
		}
		
	}
	
	void fillsynth_birdsSIG0(int count, float* output) {
		for (int i = 0; (i < count); i = (i + 1)) {
			iRec14[0] = (iRec14[1] + 1);
			output[i] = std::sin((9.58738019e-05f * float((iRec14[0] + -1))));
			iRec14[1] = iRec14[0];
			
		}
		
	}
};

synth_birdsSIG0* newsynth_birdsSIG0() { return (synth_birdsSIG0*)new synth_birdsSIG0(); }
void deletesynth_birdsSIG0(synth_birdsSIG0* dsp) { delete dsp; }

static float ftbl0synth_birdsSIG0[65537];
static float synth_birds_faustpower2_f(float value) {
	return (value * value);
	
}
static float synth_birds_faustpower3_f(float value) {
	return ((value * value) * value);
	
}

#ifndef FAUSTCLASS 
#define FAUSTCLASS synth_birds
#endif
#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class synth_birds : public dsp {
	
 private:
	
	int fSamplingFreq;
	float fConst0;
	float fConst1;
	float fConst2;
	float fConst3;
	float fConst4;
	float fConst5;
	int iVec0[3];
	float fConst6;
	float fConst7;
	FAUSTFLOAT fHslider0;
	float fRec8[2];
	int iRec7[2];
	int iRec9[2];
	FAUSTFLOAT fHslider1;
	float fRec10[2];
	int iVec1[2];
	float fRec6[2];
	float fRec11[4];
	float fRec5[2];
	int iVec2[2];
	int iRec4[2];
	float fConst8;
	float fRec13[4];
	float fRec12[2];
	int iRec3[2];
	float fConst9;
	float fConst10;
	float fConst11;
	float fConst12;
	float fConst13;
	float fConst14;
	float fConst15;
	float fConst16;
	float fConst17;
	float fConst18;
	float fConst19;
	float fConst20;
	float fConst21;
	float fConst22;
	float fConst23;
	float fConst24;
	float fConst25;
	float fRec2[2];
	float fRec1[2];
	float fConst26;
	float fConst27;
	float fConst28;
	float fConst29;
	float fRec18[4];
	float fRec17[2];
	float fConst30;
	int iRec21[2];
	float fConst31;
	float fConst32;
	float fConst33;
	float fConst34;
	float fConst35;
	float fConst36;
	float fConst37;
	float fConst38;
	float fRec20[2];
	float fRec19[2];
	float fVec3[2];
	float fRec22[2];
	float fVec4[2];
	float fVec5[2];
	int IOTA;
	float fVec6[4096];
	float fRec16[2];
	float fConst39;
	int iRec25[2];
	float fConst40;
	float fConst41;
	float fConst42;
	float fConst43;
	float fRec24[2];
	float fRec23[2];
	float fConst44;
	int iRec28[2];
	float fConst45;
	float fConst46;
	float fRec30[4];
	float fRec29[2];
	float fConst47;
	float fConst48;
	float fConst49;
	float fConst50;
	float fConst51;
	float fConst52;
	float fConst53;
	float fConst54;
	float fConst55;
	float fConst56;
	float fConst57;
	float fConst58;
	float fConst59;
	float fConst60;
	float fConst61;
	float fConst62;
	float fConst63;
	float fConst64;
	float fConst65;
	float fConst66;
	float fRec27[2];
	float fRec26[2];
	float fRec15[2];
	float fVec7[2];
	float fConst67;
	float fRec0[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("author", "Pierre Cochard");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.0");
		m->declare("filename", "synth_birds");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/version", "0.0");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.1");
		m->declare("name", "synth.birds");
		m->declare("noises.lib/name", "Faust Noise Generator Library");
		m->declare("noises.lib/version", "0.0");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "0.0");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.0");
	}

	virtual int getNumInputs() {
		return 0;
		
	}
	virtual int getNumOutputs() {
		return 2;
		
	}
	virtual int getInputRate(int channel) {
		int rate;
		switch (channel) {
			default: {
				rate = -1;
				break;
			}
			
		}
		return rate;
		
	}
	virtual int getOutputRate(int channel) {
		int rate;
		switch (channel) {
			case 0: {
				rate = 1;
				break;
			}
			case 1: {
				rate = 1;
				break;
			}
			default: {
				rate = -1;
				break;
			}
			
		}
		return rate;
		
	}
	
	static void classInit(int samplingFreq) {
		synth_birdsSIG0* sig0 = newsynth_birdsSIG0();
		sig0->instanceInitsynth_birdsSIG0(samplingFreq);
		sig0->fillsynth_birdsSIG0(65537, ftbl0synth_birdsSIG0);
		deletesynth_birdsSIG0(sig0);
		
	}
	
	virtual void instanceConstants(int samplingFreq) {
		fSamplingFreq = samplingFreq;
		fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSamplingFreq)));
		fConst1 = (1.0f / std::tan((7853.98145f / fConst0)));
		fConst2 = (1.0f / (fConst1 + 1.0f));
		fConst3 = (1.0f / std::tan((9424.77832f / fConst0)));
		fConst4 = (1.0f / (fConst3 + 1.0f));
		fConst5 = (1.0f - fConst3);
		fConst6 = (0.00100000005f * fConst0);
		fConst7 = (60.0f * fConst0);
		fConst8 = (0.00251000002f * fConst0);
		fConst9 = (0.00218875497f * fConst0);
		fConst10 = (0.00208835304f * fConst0);
		fConst11 = (0.00182730902f * fConst0);
		fConst12 = (0.00174698804f * fConst0);
		fConst13 = (0.00152610498f * fConst0);
		fConst14 = (0.00134538196f * fConst0);
		fConst15 = (0.001064257f * fConst0);
		fConst16 = (0.000983936014f * fConst0);
		fConst17 = (0.000682730984f * fConst0);
		fConst18 = (3319.99805f / fConst0);
		fConst19 = (0.000582328998f * fConst0);
		fConst20 = (9959.96094f / fConst0);
		fConst21 = (0.000461846997f * fConst0);
		fConst22 = (7702.39551f / fConst0);
		fConst23 = (6.0241e-05f * fConst0);
		fConst24 = (2490.00269f / fConst0);
		fConst25 = (16599.9902f / fConst0);
		fConst26 = (1.0f / fConst0);
		fConst27 = (359.25119f / fConst0);
		fConst28 = (0.0416666679f * synth_birds_faustpower2_f(fConst0));
		fConst29 = (0.5f * fConst0);
		fConst30 = (0.00170000002f * fConst0);
		fConst31 = (0.001646586f * fConst0);
		fConst32 = (0.001305221f * fConst0);
		fConst33 = (2929.41577f / fConst0);
		fConst34 = (0.000865000009f * fConst0);
		fConst35 = (2271.58643f / fConst0);
		fConst36 = (0.000863454014f * fConst0);
		fConst37 = (646830.5f / fConst0);
		fConst38 = (567.48822f / fConst0);
		fConst39 = (0.000549999997f * fConst0);
		fConst40 = (0.000502068026f * fConst0);
		fConst41 = (0.000240964f * fConst0);
		fConst42 = (3829.8916f / fConst0);
		fConst43 = (2299.09863f / fConst0);
		fConst44 = (0.000943000021f * fConst0);
		fConst45 = (0.000829679018f * fConst0);
		fConst46 = (0.000943333027f * fConst0);
		fConst47 = (8798.63477f / fConst0);
		fConst48 = (0.000806947995f * fConst0);
		fConst49 = (43992.7852f / fConst0);
		fConst50 = (0.00071602402f * fConst0);
		fConst51 = (10998.1963f / fConst0);
		fConst52 = (0.000659196987f * fConst0);
		fConst53 = (17597.2695f / fConst0);
		fConst54 = (0.000613734999f * fConst0);
		fConst55 = (21996.3926f / fConst0);
		fConst56 = (0.000488715013f * fConst0);
		fConst57 = (7998.72021f / fConst0);
		fConst58 = (0.000431888009f * fConst0);
		fConst59 = (7637.21484f / fConst0);
		fConst60 = (0.000318233011f * fConst0);
		fConst61 = (8798.55664f / fConst0);
		fConst62 = (0.000193212996f * fConst0);
		fConst63 = (7.99872017f / fConst0);
		fConst64 = (0.000147750994f * fConst0);
		fConst65 = (21996.3926f / fConst0);
		fConst66 = (6768.14355f / fConst0);
		fConst67 = (1.0f - fConst1);
		
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(240.0f);
		fHslider1 = FAUSTFLOAT(50.0f);
		
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 3); l0 = (l0 + 1)) {
			iVec0[l0] = 0;
			
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec8[l1] = 0.0f;
			
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			iRec7[l2] = 0;
			
		}
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			iRec9[l3] = 0;
			
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec10[l4] = 0.0f;
			
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			iVec1[l5] = 0;
			
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec6[l6] = 0.0f;
			
		}
		for (int l7 = 0; (l7 < 4); l7 = (l7 + 1)) {
			fRec11[l7] = 0.0f;
			
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec5[l8] = 0.0f;
			
		}
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			iVec2[l9] = 0;
			
		}
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			iRec4[l10] = 0;
			
		}
		for (int l11 = 0; (l11 < 4); l11 = (l11 + 1)) {
			fRec13[l11] = 0.0f;
			
		}
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fRec12[l12] = 0.0f;
			
		}
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			iRec3[l13] = 0;
			
		}
		for (int l14 = 0; (l14 < 2); l14 = (l14 + 1)) {
			fRec2[l14] = 0.0f;
			
		}
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			fRec1[l15] = 0.0f;
			
		}
		for (int l17 = 0; (l17 < 4); l17 = (l17 + 1)) {
			fRec18[l17] = 0.0f;
			
		}
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			fRec17[l18] = 0.0f;
			
		}
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			iRec21[l19] = 0;
			
		}
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fRec20[l20] = 0.0f;
			
		}
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			fRec19[l21] = 0.0f;
			
		}
		for (int l22 = 0; (l22 < 2); l22 = (l22 + 1)) {
			fVec3[l22] = 0.0f;
			
		}
		for (int l23 = 0; (l23 < 2); l23 = (l23 + 1)) {
			fRec22[l23] = 0.0f;
			
		}
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fVec4[l24] = 0.0f;
			
		}
		for (int l25 = 0; (l25 < 2); l25 = (l25 + 1)) {
			fVec5[l25] = 0.0f;
			
		}
		IOTA = 0;
		for (int l26 = 0; (l26 < 4096); l26 = (l26 + 1)) {
			fVec6[l26] = 0.0f;
			
		}
		for (int l27 = 0; (l27 < 2); l27 = (l27 + 1)) {
			fRec16[l27] = 0.0f;
			
		}
		for (int l28 = 0; (l28 < 2); l28 = (l28 + 1)) {
			iRec25[l28] = 0;
			
		}
		for (int l29 = 0; (l29 < 2); l29 = (l29 + 1)) {
			fRec24[l29] = 0.0f;
			
		}
		for (int l30 = 0; (l30 < 2); l30 = (l30 + 1)) {
			fRec23[l30] = 0.0f;
			
		}
		for (int l31 = 0; (l31 < 2); l31 = (l31 + 1)) {
			iRec28[l31] = 0;
			
		}
		for (int l32 = 0; (l32 < 4); l32 = (l32 + 1)) {
			fRec30[l32] = 0.0f;
			
		}
		for (int l33 = 0; (l33 < 2); l33 = (l33 + 1)) {
			fRec29[l33] = 0.0f;
			
		}
		for (int l34 = 0; (l34 < 2); l34 = (l34 + 1)) {
			fRec27[l34] = 0.0f;
			
		}
		for (int l35 = 0; (l35 < 2); l35 = (l35 + 1)) {
			fRec26[l35] = 0.0f;
			
		}
		for (int l36 = 0; (l36 < 2); l36 = (l36 + 1)) {
			fRec15[l36] = 0.0f;
			
		}
		for (int l37 = 0; (l37 < 2); l37 = (l37 + 1)) {
			fVec7[l37] = 0.0f;
			
		}
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			fRec0[l38] = 0.0f;
			
		}
		
	}
	
	virtual void init(int samplingFreq) {
		classInit(samplingFreq);
		instanceInit(samplingFreq);
	}
	virtual void instanceInit(int samplingFreq) {
		instanceConstants(samplingFreq);
		instanceResetUserInterface();
		instanceClear();
	}
	
	virtual synth_birds* clone() {
		return new synth_birds();
	}
	virtual int getSampleRate() {
		return fSamplingFreq;
		
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openHorizontalBox("Bird");
		ui_interface->declare(&fHslider1, "acc", "0 1 -10 0 10");
		ui_interface->declare(&fHslider1, "style", "knob");
		ui_interface->declare(&fHslider1, "unit", "%");
		ui_interface->addHorizontalSlider("probability", &fHslider1, 50.0f, 25.0f, 100.0f, 1.0f);
		ui_interface->declare(&fHslider0, "acc", "0 1 -10 0 10");
		ui_interface->declare(&fHslider0, "style", "knob");
		ui_interface->addHorizontalSlider("speed", &fHslider0, 240.0f, 120.0f, 480.0f, 0.100000001f);
		ui_interface->closeBox();
		
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = (0.00100000005f * float(fHslider0));
		float fSlow1 = (9.99999975e-06f * float(fHslider1));
		for (int i = 0; (i < count); i = (i + 1)) {
			iVec0[0] = 1;
			fRec8[0] = (fSlow0 + (0.999000013f * fRec8[1]));
			iRec7[0] = ((iVec0[1] + iRec7[1]) % int((fConst7 / fRec8[0])));
			iRec9[0] = ((1103515245 * iRec9[1]) + 12345);
			fRec10[0] = (fSlow1 + (0.999000013f * fRec10[1]));
			int iTemp0 = ((iRec7[0] == 0) * (std::fabs((4.65661287e-10f * float(iRec9[0]))) <= fRec10[0]));
			iVec1[0] = iTemp0;
			fRec6[0] = ((fRec6[1] + float((float((iTemp0 - iVec1[1])) > 0.0f))) - (0.020833334f * float((fRec6[1] > 0.0f))));
			int iTemp1 = (fRec6[0] > 0.0f);
			fRec11[0] = std::fmod(float((int((2994.2312f * (fRec11[2] + fRec11[3]))) + 38125)), 2900.0f);
			fRec5[0] = (iTemp1?(fRec11[0] + 100.0f):fRec5[1]);
			int iTemp2 = int((fConst6 * std::fabs((fRec5[0] + -1.0f))));
			iVec2[0] = iTemp2;
			iRec4[0] = ((iTemp2 == iVec2[1]) * (iRec4[1] + 1));
			float fTemp3 = float(iRec4[0]);
			fRec13[0] = std::fmod(float((int((2994.2312f * (fRec13[2] + fRec13[3]))) + 38125)), 2000.0f);
			fRec12[0] = (iTemp1?fRec13[0]:fRec12[1]);
			float fTemp4 = std::fabs((fRec12[0] + -1.0f));
			iRec3[0] = ((fTemp3 < (fConst8 * fTemp4))?iRec4[0]:iRec3[1]);
			float fTemp5 = float(iRec3[0]);
			int iTemp6 = (fTemp5 < (fConst16 * fTemp4));
			float fTemp7 = (fConst17 * fTemp4);
			int iTemp8 = (fTemp5 < fTemp7);
			float fTemp9 = (fConst19 * fTemp4);
			int iTemp10 = (fTemp5 < fTemp9);
			float fTemp11 = (fConst21 * fTemp4);
			int iTemp12 = (fTemp5 < fTemp11);
			float fTemp13 = (fConst23 * fTemp4);
			int iTemp14 = (fTemp5 < fTemp13);
			fRec2[0] = ((0.999000013f * fRec2[1]) + (0.00100000005f * ((fTemp5 < (fConst9 * fTemp4))?((fTemp5 < (fConst10 * fTemp4))?((fTemp5 < (fConst11 * fTemp4))?((fTemp5 < (fConst12 * fTemp4))?((fTemp5 < (fConst13 * fTemp4))?((fTemp5 < (fConst14 * fTemp4))?((fTemp5 < (fConst15 * fTemp4))?(iTemp6?(iTemp8?(iTemp10?(iTemp12?(iTemp14?((iRec3[0] < 0)?0.0f:(iTemp14?(fConst25 * (fTemp5 / fTemp4)):1.0f)):(iTemp12?((fConst24 * ((0.0f - (fTemp5 - fTemp13)) / fTemp4)) + 1.0f):0.0f)):(iTemp10?(fConst22 * ((fTemp5 - fTemp11) / fTemp4)):0.927999973f)):(iTemp8?((fConst20 * ((0.0f - (0.428000003f * (fTemp5 - fTemp9))) / fTemp4)) + 0.927999973f):0.5f)):(iTemp6?((fConst18 * ((0.0f - (0.5f * (fTemp5 - fTemp7))) / fTemp4)) + 0.5f):0.0f)):0.0f):0.0f):0.0f):0.0f):0.0f):0.0f):0.0f):0.0f)));
			fRec1[0] = (0.0f - (fConst4 * ((fConst5 * fRec1[1]) - (fRec2[0] + fRec2[1]))));
			fRec18[0] = std::fmod(float((int((2994.2312f * (fRec18[2] + fRec18[3]))) + 38125)), 22.0f);
			fRec17[0] = (iTemp1?(fRec18[0] + 72.0f):fRec17[1]);
			float fTemp15 = std::pow(2.0f, (0.0833333358f * (std::fabs((fRec17[0] + -1.0f)) + -69.0f)));
			iRec21[0] = ((fTemp3 < (fConst30 * fTemp4))?iRec4[0]:iRec21[1]);
			float fTemp16 = float(iRec21[0]);
			int iTemp17 = (fTemp16 < (fConst31 * fTemp4));
			float fTemp18 = (fConst32 * fTemp4);
			int iTemp19 = (fTemp16 < fTemp18);
			float fTemp20 = (fConst34 * fTemp4);
			int iTemp21 = (fTemp16 < fTemp20);
			float fTemp22 = (fConst36 * fTemp4);
			int iTemp23 = (fTemp16 < fTemp22);
			fRec20[0] = ((0.999000013f * fRec20[1]) + (0.00100000005f * (iTemp17?(iTemp19?(iTemp21?(iTemp23?((iRec21[0] < 0)?0.0f:(iTemp23?(fConst38 * (fTemp16 / fTemp4)):0.49000001f)):(iTemp21?((fConst37 * ((0.0f - (0.49000001f * (fTemp16 - fTemp22))) / fTemp4)) + 0.49000001f):0.0f)):(iTemp19?(fConst35 * ((fTemp16 - fTemp20) / fTemp4)):1.0f)):(iTemp17?((fConst33 * ((0.0f - (fTemp16 - fTemp18)) / fTemp4)) + 1.0f):0.0f)):0.0f)));
			fRec19[0] = (0.0f - (fConst4 * ((fConst5 * fRec19[1]) - (fRec20[0] + fRec20[1]))));
			float fTemp24 = (fRec19[0] + 1.0f);
			float fTemp25 = std::max<float>((27.7199993f * (fTemp15 * fTemp24)), 23.4489498f);
			float fTemp26 = std::max<float>(0.0f, std::min<float>(2047.0f, (fConst29 / fTemp25)));
			float fTemp27 = std::floor(fTemp26);
			float fTemp28 = std::max<float>(20.0f, std::fabs(fTemp25));
			fVec3[0] = fTemp28;
			float fTemp29 = (fRec22[1] + (fConst26 * fVec3[1]));
			fRec22[0] = (fTemp29 - std::floor(fTemp29));
			float fTemp30 = (2.0f * fRec22[0]);
			float fTemp31 = synth_birds_faustpower3_f((fTemp30 + -1.0f));
			fVec4[0] = (fTemp31 + (1.0f - fTemp30));
			float fTemp32 = ((fTemp31 + (1.0f - (fTemp30 + fVec4[1]))) / fTemp28);
			fVec5[0] = fTemp32;
			float fTemp33 = ((float(iVec0[2]) * (fTemp32 - fVec5[1])) / fTemp28);
			fVec6[(IOTA & 4095)] = fTemp33;
			int iTemp34 = int(fTemp26);
			fRec16[0] = ((0.999000013f * fRec16[1]) - (fConst28 * ((((fTemp27 + (1.0f - fTemp26)) * fVec6[((IOTA - iTemp34) & 4095)]) - fTemp33) + (fVec6[((IOTA - (iTemp34 + 1)) & 4095)] * (fTemp26 - fTemp27)))));
			iRec25[0] = ((fTemp3 < (fConst39 * fTemp4))?iRec4[0]:iRec25[1]);
			float fTemp35 = float(iRec25[0]);
			int iTemp36 = (fTemp35 < (fConst40 * fTemp4));
			float fTemp37 = (fConst41 * fTemp4);
			int iTemp38 = (fTemp35 < fTemp37);
			fRec24[0] = ((0.00100000005f * (iTemp36?(iTemp38?((iRec25[0] < 0)?0.0f:(iTemp38?(fConst43 * (fTemp35 / fTemp4)):0.55400002f)):(iTemp36?((fConst42 * ((0.0f - (0.55400002f * (fTemp35 - fTemp37))) / fTemp4)) + 0.55400002f):0.0f)):0.0f)) + (0.999000013f * fRec24[1]));
			fRec23[0] = (fConst4 * ((fRec24[0] + fRec24[1]) - (fConst5 * fRec23[1])));
			iRec28[0] = ((fTemp3 < (fConst44 * fTemp4))?iRec4[0]:iRec28[1]);
			float fTemp39 = float(iRec28[0]);
			float fTemp40 = (fConst45 * fTemp4);
			int iTemp41 = (fTemp39 < fTemp40);
			fRec30[0] = std::fmod(float((int((2994.2312f * (fRec30[2] + fRec30[3]))) + 38125)), 1000.0f);
			fRec29[0] = (iTemp1?fRec30[0]:fRec29[1]);
			float fTemp42 = std::fabs((fRec29[0] + -1.0f));
			float fTemp43 = (0.00100000005f * fTemp42);
			float fTemp44 = (fConst48 * fTemp4);
			int iTemp45 = (fTemp39 < fTemp44);
			float fTemp46 = (fTemp43 + -1.0f);
			float fTemp47 = (fConst50 * fTemp4);
			int iTemp48 = (fTemp39 < fTemp47);
			float fTemp49 = (1.0f - fTemp43);
			float fTemp50 = (fConst52 * fTemp4);
			int iTemp51 = (fTemp39 < fTemp50);
			float fTemp52 = (fConst54 * fTemp4);
			int iTemp53 = (fTemp39 < fTemp52);
			float fTemp54 = (fConst56 * fTemp4);
			int iTemp55 = (fTemp39 < fTemp54);
			float fTemp56 = (fConst58 * fTemp4);
			int iTemp57 = (fTemp39 < fTemp56);
			float fTemp58 = (fConst60 * fTemp4);
			int iTemp59 = (fTemp39 < fTemp58);
			float fTemp60 = (fConst62 * fTemp4);
			int iTemp61 = (fTemp39 < fTemp60);
			float fTemp62 = (fConst64 * fTemp4);
			int iTemp63 = (fTemp39 < fTemp62);
			fRec27[0] = ((0.999000013f * fRec27[1]) + (0.00100000005f * (iTemp41?(iTemp45?(iTemp48?(iTemp51?(iTemp53?(iTemp55?(iTemp57?(iTemp59?(iTemp61?(iTemp63?((iRec28[0] < 0)?0.0f:(iTemp63?(fConst66 * (fTemp39 / fTemp4)):1.0f)):(iTemp61?((fConst65 * ((0.0f - (fTemp39 - fTemp62)) / fTemp4)) + 1.0f):0.0f)):(iTemp59?(fConst63 * ((fTemp42 * (fTemp39 - fTemp60)) / fTemp4)):fTemp43)):(iTemp57?(fTemp43 + (fConst61 * ((0.0f - (0.00100000005f * (fTemp42 * (fTemp39 - fTemp58)))) / fTemp4))):0.0f)):(iTemp55?(fConst59 * ((fTemp39 - fTemp56) / fTemp4)):0.433999985f)):(iTemp53?((fConst57 * (((fTemp39 - fTemp54) * (fTemp43 + -0.433999985f)) / fTemp4)) + 0.433999985f):fTemp43)):(iTemp51?(fTemp43 + (fConst55 * ((fTemp49 * (fTemp39 - fTemp52)) / fTemp4))):1.0f)):(iTemp48?((fConst53 * ((fTemp46 * (fTemp39 - fTemp50)) / fTemp4)) + 1.0f):fTemp43)):(iTemp45?(fTemp43 + (fConst51 * (((fTemp39 - fTemp47) * fTemp49) / fTemp4))):1.0f)):(iTemp41?((fConst49 * (((fTemp39 - fTemp44) * fTemp46) / fTemp4)) + 1.0f):fTemp43)):((fTemp39 < (fConst46 * fTemp4))?(fTemp43 + (fConst47 * ((0.0f - (0.00100000005f * (fTemp42 * (fTemp39 - fTemp40)))) / fTemp4))):0.0f))));
			fRec26[0] = (0.0f - (fConst4 * ((fConst5 * fRec26[1]) - (fRec27[0] + fRec27[1]))));
			float fTemp64 = ((fConst26 * ((((fConst27 * ((fRec16[0] * (fRec23[0] + 1.0f)) * fTemp24)) + 440.0f) * (fRec26[0] + 1.0f)) * fTemp15)) + fRec15[1]);
			fRec15[0] = (fTemp64 - std::floor(fTemp64));
			float fTemp65 = (65536.0f * fRec15[0]);
			int iTemp66 = int(fTemp65);
			float fTemp67 = ftbl0synth_birdsSIG0[iTemp66];
			float fTemp68 = (fRec1[0] * (fTemp67 + ((fTemp65 - std::floor(fTemp65)) * (ftbl0synth_birdsSIG0[(iTemp66 + 1)] - fTemp67))));
			fVec7[0] = fTemp68;
			fRec0[0] = (fConst2 * ((fTemp68 + fVec7[1]) - (fConst67 * fRec0[1])));
			float fTemp69 = (0.800000012f * fRec0[0]);
			output0[i] = FAUSTFLOAT(fTemp69);
			output1[i] = FAUSTFLOAT(fTemp69);
			iVec0[2] = iVec0[1];
			iVec0[1] = iVec0[0];
			fRec8[1] = fRec8[0];
			iRec7[1] = iRec7[0];
			iRec9[1] = iRec9[0];
			fRec10[1] = fRec10[0];
			iVec1[1] = iVec1[0];
			fRec6[1] = fRec6[0];
			for (int j0 = 3; (j0 > 0); j0 = (j0 - 1)) {
				fRec11[j0] = fRec11[(j0 - 1)];
				
			}
			fRec5[1] = fRec5[0];
			iVec2[1] = iVec2[0];
			iRec4[1] = iRec4[0];
			for (int j1 = 3; (j1 > 0); j1 = (j1 - 1)) {
				fRec13[j1] = fRec13[(j1 - 1)];
				
			}
			fRec12[1] = fRec12[0];
			iRec3[1] = iRec3[0];
			fRec2[1] = fRec2[0];
			fRec1[1] = fRec1[0];
			for (int j2 = 3; (j2 > 0); j2 = (j2 - 1)) {
				fRec18[j2] = fRec18[(j2 - 1)];
				
			}
			fRec17[1] = fRec17[0];
			iRec21[1] = iRec21[0];
			fRec20[1] = fRec20[0];
			fRec19[1] = fRec19[0];
			fVec3[1] = fVec3[0];
			fRec22[1] = fRec22[0];
			fVec4[1] = fVec4[0];
			fVec5[1] = fVec5[0];
			IOTA = (IOTA + 1);
			fRec16[1] = fRec16[0];
			iRec25[1] = iRec25[0];
			fRec24[1] = fRec24[0];
			fRec23[1] = fRec23[0];
			iRec28[1] = iRec28[0];
			for (int j3 = 3; (j3 > 0); j3 = (j3 - 1)) {
				fRec30[j3] = fRec30[(j3 - 1)];
				
			}
			fRec29[1] = fRec29[0];
			fRec27[1] = fRec27[0];
			fRec26[1] = fRec26[0];
			fRec15[1] = fRec15[0];
			fVec7[1] = fVec7[0];
			fRec0[1] = fRec0[0];
			
		}
		
	}

	
};
// clang-format on
#endif

    template <class T>
    struct _synth_birds_UI : public UI {
    static std::string name;
};

template <class T>
std::string _synth_birds_UI<T>::name(sym(synth_birds));

typedef _synth_birds_UI<synth_birds> synth_birds_UI;

class faust_synth_birds_tilde : public FaustExternal<synth_birds, synth_birds_UI> {
public:
    faust_synth_birds_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif