#include "mod_analyze.h"
#include "an_pitchtrack.h"

extern void setup_an_onset_tilde();

void ceammc_analyze_setup()
{
    setup_an_pitchtrack();
    setup_an_onset_tilde();
}
