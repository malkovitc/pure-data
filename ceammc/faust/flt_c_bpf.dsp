declare ui "disable";

msp = library("maxmsp.lib");
ui = library("ceammc_ui.lib");

process(in) = msp.filtercoeff(ui.freq(1000), 0, ui.q(0.1)).BPF;
