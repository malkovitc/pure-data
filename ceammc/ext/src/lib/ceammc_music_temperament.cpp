#include "ceammc_music_temperament.h"
#include "ceammc_convert.h"

using namespace ceammc;
using namespace ceammc::music;

static const char* NAMES[] = {
    "Equal Tempered, Perfect Octave",
    "Equal Tempered, Perfect Fourth",
    "Equal Tempered, Perfect Fifth",
    "Equal Tempered, Streched (1.0 Cent)",
    "Equal Tempered, Streched (1.25 Cent)",
    "Equal Tempered, Streched (1.5 Cent)",
    "Just Tempered (Schugk)",
    "Just Tempered (Barbour)",
    "Naturally harmonious (Thirds)",
    "Naturally harmonious",
    "Pythagorean",
    "Pythagorean (Perfect)",
    "Pythagorean (Fifth chain Cb - E)",
    "Pythagorean (Fifth chain Db - F#)",
    "Pythagorean (Fifth chain Ebb - G)",
    "Pythagorean (Fifth chain Fb - A)",
    "Pythagorean (Fifth chain F - A#)",
    "Pythagorean (Fifth chain Gb - B)",
    "Pythagorean (Fifth chain Ab - C#)",
    "Pythagorean (Fifth chain Bb - D#)",
    "Meantone",
    "Meantone # (-1/4)",
    "Meantone b (-1/4)",
    "Meantone (-1/4) (LargeThird)",
    "Meantone (Small third)",
    "Meantone (Homogeneous)",
    "Meantone (Homogeneous third)",
    "Meantone (Homogeneous gradated)",
    "Comma - Temperament (1/7)",
    "Comma - Temperament (1/8)",
    "Comma - Temperament (1/9)",
    "Comma - Temperament (2/9)",
    "Comma - Temperament (1/10)",
    "Comma - Temperament (3/11)",
    "Pythagorei comma (3-Split)",
    "Pythagorei comma (4-Split)",
    "Pythagorei comma (5-Split)",
    "Pythagorei comma (6-Split)",
    "Pythagorei comma (6 & 12-Split)",
    "Syntonic comma (2-Split)",
    "Syntonic comma (4-Split)",
    "Syntonic comma (5-Split)",
    "Diatonic (Chromatic addition)",
    "Organ of Freiberg (Silbermann-Orgel, 1985)",
    "Organ of Fribourg (Manderscheidt-Orgel, 1640)",
    "Organ of Hamburg (Schnitger-Orgel, 1993)",
    "Organ of Maihingen (Baumeister-Orgel, 1737)",
    "Organ of Muri (Evangelien-Orgel)",
    "Organ of Niederbobritzsch (Göthel-Orgel)",
    "Organ of Weingarten (Gabler-Orgel, 1750)",
    "Organ of Weingarten (Gabler-Orgel, 1983)",
    "Agricola (Martin, 1539, 1543, 1545)",
    "Ammerbach (1571)",
    "Ammerbach (1583, Interpretation 1)",
    "Ammerbach (1583, Interpretation 2)",
    "Bach (Billeter, Well-Tempered)",
    "Bach (Kelletats, 1966)",
    "Bach (Kellner, Well-Tempered)",
    "Bach (Kellner, 1977)",
    "Bach (Klais)",
    "Barnes (1971)",
    "Barnes (1977)",
    "Bendeler (Fractions)",
    "Bendeler III",
    "Bermudo (1555)",
    "Bossart I",
    "Bossart II",
    "Bossart III",
    "Bruder (1829)",
    "Ganassi (1543)",
    "Goebel (1967)",
    "Grammateus (1518)",
    "Kelletat (1960)",
    "Kelletat (1966)",
    "Kellner",
    "Kircher",
    "Kirnberger I (1766)",
    "Kirnberger II (1771)",
    "Kirnberger II (1776)",
    "Kirnberger III (1779)",
    "Lambert (1774)",
    "Lublin (1540)",
    "Malcolm (Alexander, 1721)",
    "Marpurg I",
    "Marpurg II",
    "Marpurg III",
    "Marpurg IV",
    "Marpurg V",
    "Marpurg VI",
    "Marpurg VII",
    "Marpurg VIII",
    "Marpurg IX",
    "Marpurg X",
    "Marpurg XI",
    "Marpurg XII",
    "Meister",
    "Mersenne (Fractions)",
    "Mersenne (Marin, 1636)",
    "Neidhardt (1724, Grosse Stadt)",
    "Neidhardt (1732, Dorf)",
    "Neidthardt I",
    "Neidthardt II",
    "Neidthardt III",
    "Rameau",
    "Rameau (-1/4)",
    "Ramis de Pareia (1482)",
    "Reinhard (Andreas, 1604)",
    "Rossi (-1/5)",
    "Rossi (-2/9)",
    "Salinas (-1/3)",
    "Schlick (Barbour)",
    "Schlick (Dupont)",
    "Schlick (Lange)",
    "Schlick (Ratte)",
    "Schlick (Schugk)",
    "Schlick (Tessmer)",
    "Schlick (Vogel)",
    "Schneegass I (1590)",
    "Schneegass II (1590)",
    "Schneegass III (1590)",
    "Silbermann (-1/6)",
    "Silbermann (1/6)",
    "Sorge",
    "Stanhope (1801)",
    "Trost (Johann Caspar, 1677)",
    "Valotti (1754)",
    "Van Zwolle",
    "Veroli (Ordinaire)",
    "Werckmeister I",
    "Werckmeister II",
    "Werckmeister III",
    "Werckmeister IV",
    "Wiegleb",
    "Wiegleb (1790)",
    "Young I",
    "Young II",
    "Zarlino (-2/7)"
};

static const double DEVIATIONS[][12] = {
    { 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000 },
    { 3.519, 3.128, 2.737, 2.346, 1.955, 1.564, 1.173, 0.782, 0.391, 0.000, -0.391, -0.782 },
    { -2.514, -2.235, -1.956, -1.677, -1.397, -1.118, -0.838, -0.559, -0.280, 0.000, 0.279, 0.558 },
    { -0.750, -0.667, -0.583, -0.500, -0.417, -0.333, -0.250, -0.167, -0.083, 0.000, 0.083, 0.167 },
    { -0.938, -0.834, -0.730, -0.625, -0.521, -0.417, -0.313, -0.209, -0.105, 0.000, 0.104, 0.208 },
    { -1.125, -1.000, -0.875, -0.750, -0.625, -0.500, -0.375, -0.250, -0.125, 0.000, 0.125, 0.250 },
    { 15.641, -13.686, 19.551, 31.282, 1.955, 13.686, 5.864, 17.596, 29.327, 0.000, 33.237, 3.910 },
    { 15.641, -13.686, 19.551, 31.282, 1.955, 13.686, 5.864, 17.596, -11.732, 0.000, 33.237, 3.910 },
    { 15.641, 27.372, 19.551, 31.282, 1.955, 13.686, 5.864, 17.596, 29.327, 0.000, 11.731, 3.910 },
    { 15.641, 27.372, 19.551, 31.282, 1.956, 13.686, 5.864, 17.596, 29.327, 0.000, 33.237, 3.910 },
    { -5.865, 7.820, -1.955, -11.730, 1.955, -7.820, 5.865, -3.910, 9.775, 0.000, -9.775, 3.910 },
    { 15.640, 7.820, 19.550, 11.730, 1.955, 13.685, 5.865, 17.595, 9.775, 0.000, 11.730, 3.910 },
    { -5.865, -15.640, -1.955, -11.730, 1.955, -7.820, -17.595, -3.910, -13.685, 0.000, -9.775, -19.550 },
    { -5.865, -15.640, -1.955, -11.730, 1.955, -7.820, 5.865, -3.910, -13.685, 0.000, -9.775, 3.910 },
    { 17.595, 7.820, -1.955, 11.730, 1.955, 15.640, 5.865, 19.550, 9.775, 0.000, 13.685, 3.910 },
    { -5.865, -15.640, -1.955, -11.730, -21.505, -7.820, -17.595, -3.910, -13.685, 0.000, -9.775, -19.550 },
    { -5.865, 7.820, -1.955, 11.730, 1.955, -7.820, 5.865, -3.910, 9.775, 0.000, 13.685, 3.910 },
    { -5.865, -15.640, -1.955, -11.730, 1.955, -7.820, -17.595, -3.910, -13.685, 0.000, -9.775, 3.910 },
    { -5.865, 7.820, -1.955, -11.730, 1.955, -7.820, 5.865, -3.910, -13.685, 0.000, -9.775, 3.910 },
    { -5.865, 7.820, -1.955, 11.730, 1.955, -7.820, 5.865, -3.910, 9.775, 0.000, -9.775, 3.910 },
    { 8.798, -9.775, 2.933, 15.640, -2.932, 11.731, -7.819, 5.865, -10.752, 0.000, 13.686, -5.864 },
    { 10.265, -13.686, 3.422, -20.529, -3.421, 13.686, -10.265, 6.843, -17.108, 0.000, -23.951, -6.843 },
    { 10.265, 27.373, 3.422, 20.530, -3.421, 13.686, 30.794, 6.843, 23.951, 0.000, 17.108, -6.843 },
    { 10.265, -13.686, 3.422, 20.530, -3.421, 13.686, -10.265, 6.843, -17.108, 0.000, 17.108, -6.843 },
    { 15.642, -20.856, 5.214, 31.284, -5.214, 20.856, -15.642, 10.428, -26.070, 0.000, 26.070, -10.428 },
    { 7.038, -9.384, 2.346, 14.076, -2.346, 9.384, -7.038, 4.692, -11.730, 0.000, 11.730, -4.692 },
    { 12.570, -16.760, 4.190, 25.140, -4.190, 16.760, -12.570, 8.380, -20.950, 0.000, 20.950, -8.380 },
    { 4.887, -6.516, 1.629, 9.775, -1.629, 6.516, -4.889, 3.258, -8.145, 0.000, 8.145, -3.258 },
    { 3.351, -4.468, 1.117, 6.702, -1.117, 4.468, -3.351, 2.234, -5.585, 0.000, 5.585, -2.234 },
    { 2.199, -2.932, 0.733, 4.398, -0.733, 2.932, -2.199, 1.466, -3.665, 0.000, 3.665, -1.466 },
    { 1.305, -1.740, 0.435, 2.610, -0.435, 1.740, -1.305, 0.870, -2.175, 0.000, 2.175, -0.870 },
    { 8.472, -11.296, 2.824, 16.944, -2.824, 11.296, -8.472, 5.648, -14.120, 0.000, 14.120, -5.648 },
    { 0.588, -0.784, 0.196, 1.176, -0.196, 0.784, -0.588, 0.392, -0.980, 0.000, 0.980, -0.392 },
    { 11.730, -15.640, 3.910, 23.460, -3.910, 15.640, -11.730, 7.820, -19.550, 0.000, 19.550, -7.820 },
    { 9.775, 0.000, -1.955, 3.910, 1.955, 7.820, -1.955, 3.910, 1.955, 0.000, 5.865, 3.910 },
    { 5.865, 1.955, -1.955, 0.000, 1.955, 3.910, 0.000, 1.955, 3.910, 0.000, 1.955, -1.955 },
    { 8.211, -1.564, 2.737, 2.346, -2.737, 6.256, -3.519, 5.474, 0.391, 0.000, 4.301, -5.474 },
    { 5.865, -3.910, 1.955, 0.000, -1.955, 3.910, -5.865, 3.910, -1.955, 0.000, 1.955, -3.910 },
    { 5.865, -3.910, 1.955, 7.820, -1.955, 7.820, -3.910, 3.910, -3.910, 0.000, 7.820, -3.910 },
    { 15.641, -13.686, 8.798, 31.282, 1.955, 13.686, -4.888, 17.596, -11.731, 0.000, 22.484, 3.910 },
    { 10.264, -3.910, 3.421, 4.399, -3.422, 8.309, -10.265, 6.842, 2.444, 0.000, 6.354, -6.844 },
    { 7.038, 1.173, 2.346, 3.519, -2.346, 5.865, 0.000, 4.692, 2.346, 0.000, 4.692, -1.173 },
    { 15.631, -13.696, 19.541, 31.272, 1.945, 13.676, 5.855, 17.586, -11.741, 0.000, 11.721, 3.900 },
    { 3.910, -5.865, 0.000, 1.955, -1.955, 3.910, -5.865, 1.955, -5.865, 0.000, 3.910, -3.910 },
    { 7.820, -0.978, 2.933, 14.662, -4.887, 8.798, -2.933, 5.865, -10.752, 0.000, 12.708, -4.887 },
    { 7.038, -5.083, 2.346, 3.128, -2.346, 9.384, -7.038, 4.692, -3.128, 0.000, 7.429, -4.692 },
    { 3.910, -10.752, 2.933, 9.775, -8.797, 4.888, -12.707, 5.865, -14.662, 0.000, -1.955, -9.775 },
    { 8.798, -10.752, 3.911, 14.663, -2.932, 10.753, -8.797, 7.821, -11.730, 0.000, 9.775, -5.864 },
    { 0.000, -1.955, 2.444, -1.955, -0.978, 3.421, 0.489, -0.977, -1.955, 0.000, 1.466, -5.865 },
    { 6.399, -8.532, 2.133, 12.798, -2.133, 8.532, -6.399, 4.266, -10.665, 0.000, 10.665, -4.266 },
    { 4.888, 0.000, 0.978, 6.354, -0.977, 3.910, -3.910, 2.933, 2.933, 0.000, 6.354, -3.909 },
    { -5.865, -13.686, -1.955, -9.776, 1.955, -7.820, -15.641, -3.910, -11.731, 0.000, -9.775, 3.910 },
    { 5.865, -7.820, 3.910, 8.798, -1.955, 3.910, -3.910, 7.820, -9.775, 0.000, 4.888, -1.955 },
    { 6.135, -4.180, 4.045, 6.270, 1.955, 4.180, -0.135, 8.090, -2.225, 0.000, 8.225, 3.910 },
    { 6.135, -8.180, 4.045, 9.270, -2.045, 4.180, -4.135, 8.090, -10.225, 0.000, 5.225, -2.090 },
    { 4.888, -2.932, 4.888, 0.978, -4.887, 4.888, -4.887, 4.888, -0.977, 0.000, 2.933, -4.887 },
    { 9.124, -0.651, 4.563, 3.259, -4.563, 7.169, -2.606, 9.126, 1.304, 0.000, 5.214, -4.561 },
    { 9.774, -0.001, 3.258, 3.909, -3.258, 7.819, -1.956, 6.516, 1.954, 0.000, 5.864, -1.303 },
    { 8.211, -1.564, 2.737, 2.346, -2.737, 6.256, -3.519, 5.474, 0.391, 0.000, 4.301, -0.782 },
    { 7.491, -1.953, 3.741, 1.852, -4.878, 5.651, -3.815, 7.512, -0.020, 0.000, 3.770, -5.700 },
    { 2.933, -0.977, 0.978, 0.000, -0.977, 3.911, -2.932, 1.956, 0.978, 0.000, 1.956, -1.955 },
    { 5.865, 0.000, 1.955, 3.910, -1.955, 7.820, -1.955, 3.910, 1.955, 0.000, 5.865, 0.000 },
    { 10.456, 0.681, -1.955, 4.591, 1.955, 8.501, -1.274, 3.229, 2.636, 0.000, 6.546, 3.910 },
    { 5.865, 1.955, -1.955, 0.000, 1.955, 3.910, 0.000, 1.955, 3.910, 0.000, 1.955, -1.955 },
    { -1.955, -1.955, -1.955, -7.820, -1.955, -3.910, -3.910, 0.000, 0.000, 0.000, -5.865, 0.000 },
    { 5.865, -3.910, 3.910, 14.663, -3.910, 9.775, -5.865, 4.888, 0.000, 0.000, 13.685, -4.888 },
    { 5.865, 0.000, 0.978, 14.663, 0.000, 9.775, -1.955, 4.888, 1.955, 0.000, 10.753, 1.955 },
    { 5.865, -0.978, 3.910, 11.730, -3.910, 9.775, -2.933, 4.888, 2.932, 0.000, 10.753, -4.888 },
    { 2.933, -1.955, 5.866, 0.000, -5.865, 1.955, -3.422, 4.399, -0.977, 0.000, 0.978, -4.887 },
    { 15.641, 4.442, -1.955, -3.001, 1.955, 13.686, 12.641, 17.596, 6.397, 0.000, -1.046, 3.910 },
    { -0.169, 0.109, 0.208, 0.128, 0.118, -0.140, 0.026, 0.020, -0.019, 0.000, -0.051, -0.117 },
    { -5.865, -3.910, -1.955, 0.000, 1.955, -7.820, -5.865, -3.910, -1.955, 0.000, -9.775, 3.910 },
    { 7.820, -1.955, 3.910, 1.955, -4.887, 5.865, -3.910, 7.820, 0.000, 0.000, 3.910, -5.865 },
    { 7.820, -1.955, 3.910, 1.955, -3.910, 5.865, -3.910, 7.820, 0.000, 0.000, 3.910, -5.865 },
    { 8.211, -1.564, 2.737, 2.346, -2.737, 6.256, -3.519, 5.474, 0.391, 0.000, 4.301, -0.782 },
    { 15.640, -13.685, 19.550, -9.775, 1.955, 13.685, 5.865, 17.595, -11.730, 0.000, -7.820, 3.910 },
    { 15.640, 5.865, 19.550, 9.775, 1.955, 13.685, 5.865, 17.595, 7.820, 0.000, 11.730, 3.910 },
    { 4.888, -4.887, 8.798, -0.977, -8.798, 2.933, -4.888, 6.843, -2.932, 0.000, 0.978, -6.843 },
    { 5.865, -3.910, 9.775, 0.000, -9.775, 3.910, -5.865, 7.820, -1.955, 0.000, 1.955, -7.820 },
    { 10.264, 0.489, 3.421, 4.399, -3.422, 8.309, 0.488, 6.842, 2.444, 0.000, 6.354, -1.467 },
    { 4.190, -2.234, 1.397, 1.676, -1.396, 5.586, -4.189, 2.793, -0.279, 0.000, 3.631, -2.793 },
    { 1.090, -13.686, -1.955, 2.180, 1.955, 6.090, 5.865, 3.045, -11.731, 0.000, 4.135, 3.910 },
    { 15.641, 20.596, 19.551, 13.154, 1.955, 13.686, 18.641, 17.596, 11.199, 0.000, 4.955, 3.910 },
    { 15.640, 5.865, 19.550, 9.775, 1.955, 13.685, 5.865, 17.595, 7.820, 0.000, 11.730, 3.910 },
    { -5.865, 3.910, -1.955, 7.820, 1.955, -7.820, 1.955, -3.910, 5.865, 0.000, 9.775, 0.000 },
    { -5.865, -9.775, -1.955, -5.865, 1.955, -7.820, -11.730, -3.910, -7.820, 0.000, -9.775, -13.685 },
    { -5.865, -7.820, -1.955, -11.730, 1.955, -7.820, -9.775, -3.910, -5.865, 0.000, -9.775, -11.730 },
    { -5.865, -1.955, -1.955, 1.955, 1.955, -7.820, -3.910, -3.910, 0.000, 0.000, 3.910, -5.865 },
    { -5.865, -3.910, -1.955, 0.000, 1.955, -7.820, -5.865, -3.910, -1.955, 0.000, 1.955, 3.910 },
    { 1.955, 0.000, -1.955, -3.910, 1.955, 0.000, -1.955, -3.910, 1.955, 0.000, -1.955, -3.910 },
    { 0.000, 1.955, -1.955, 0.000, 1.955, -1.955, 0.000, -3.910, -1.955, 0.000, 1.955, -1.955 },
    { -5.865, -3.910, -1.955, 0.000, 1.955, -1.955, 0.000, -3.910, -1.955, 0.000, 1.955, 3.910 },
    { 1.955, 0.000, 1.955, 0.000, 1.955, 0.000, 1.955, 0.000, 1.955, 0.000, 1.955, 0.000 },
    { -5.865, 0.000, -1.955, -3.910, 1.955, -7.820, 1.955, -3.910, -1.955, 0.000, -5.865, 3.910 },
    { -7.820, 3.910, -1.955, -11.730, -1.955, -7.820, 1.955, -3.910, 5.865, 0.000, -9.775, 0.000 },
    { 4.888, 16.618, 3.422, 20.528, -3.421, 2.933, 14.663, 6.843, 18.573, 0.000, 0.978, -6.842 },
    { 15.640, -13.685, 19.550, 31.280, 1.955, 13.685, -15.640, 17.595, 29.325, 0.000, 11.730, 3.910 },
    { 5.131, 1.710, -1.711, -5.132, -8.553, -6.843, -5.132, -3.422, -1.711, 0.000, 1.710, 3.421 },
    { 5.865, 1.955, 1.955, 3.910, 0.000, 3.910, 1.955, 1.955, 1.955, 0.000, 3.910, 1.955 },
    { 5.865, 0.000, 3.910, 1.955, -3.910, 3.910, -1.955, 5.865, 0.000, 0.000, 3.910, -1.955 },
    { 5.865, 0.000, 1.955, 1.955, -1.955, 3.910, -1.955, 3.910, 1.955, 0.000, 1.955, -1.955 },
    { 5.865, 1.955, 1.955, 3.910, 0.000, 5.865, 1.955, 3.910, 1.955, 0.000, 5.865, 1.955 },
    { 5.865, 1.955, 1.955, 3.910, 0.000, 3.910, 1.955, 3.910, 1.955, 0.000, 3.910, 1.955 },
    { 11.730, -3.910, 3.910, 0.000, -3.910, 15.640, -5.865, 7.820, -1.955, 0.000, 7.820, -7.820 },
    { 10.265, -2.933, 3.422, -4.561, -3.421, 13.686, -4.888, 6.843, -0.978, 0.000, 4.562, -6.843 },
    { 15.641, 7.820, -1.955, 9.776, 1.955, 13.686, 5.865, 17.596, 7.821, 0.000, 11.731, 3.910 },
    { 15.641, 14.596, 19.551, 8.352, 1.955, 13.686, 12.641, 17.596, 6.397, 0.000, -1.046, 3.910 },
    { 7.039, -9.385, 2.346, 14.078, -2.346, 9.385, -7.039, 4.693, -11.731, 0.000, 11.731, -4.693 },
    { 8.473, -11.296, 2.824, 16.945, -2.824, 11.297, -8.472, 5.648, -14.121, 0.000, 14.121, -5.648 },
    { 15.642, -20.854, 5.214, 31.283, -5.213, 20.856, -15.641, 10.428, -26.068, 0.000, 26.069, -10.427 },
    { 5.865, -3.910, 1.955, 7.820, -1.955, 7.820, -3.910, 3.910, 1.955, 0.000, 7.820, -3.910 },
    { 10.266, -13.688, 3.422, 20.532, -3.422, 13.688, -10.266, 6.844, 3.422, 0.000, 17.110, -6.844 },
    { 6.231, -8.308, 2.077, 12.462, -2.077, 8.308, -6.231, 4.154, 6.475, 0.000, 10.385, -4.154 },
    { 5.865, -5.865, 1.955, 9.775, -1.955, 7.820, -3.910, 3.910, 5.865, 0.000, 7.820, -3.910 },
    { 8.211, -10.948, 2.737, 16.422, -2.737, 10.948, -8.211, 5.474, 19.159, 0.000, 13.685, -5.474 },
    { 7.331, -4.888, 2.444, 9.775, -2.444, 9.775, -4.888, 4.887, 5.865, 0.000, 9.775, -4.888 },
    { 8.211, -6.256, 2.737, 2.346, -2.737, 10.948, -8.211, 5.474, -4.301, 0.000, 8.993, -5.474 },
    { 10.340, -13.786, 3.447, 20.680, -3.446, 13.787, -10.340, 6.893, -17.233, 0.000, 17.233, -6.893 },
    { 10.341, -10.655, 3.447, 20.682, -0.314, 13.788, -7.208, 6.894, -14.102, 0.000, 17.235, -3.761 },
    { 10.198, -9.020, 4.270, 18.306, -1.449, 11.105, -8.576, 6.158, -14.463, 0.000, 15.656, -4.319 },
    { 4.889, -6.517, 1.630, 9.777, -1.629, 6.518, -4.888, 3.259, -8.146, 0.000, 8.147, -3.258 },
    { 5.865, -7.820, 1.955, 11.730, -1.955, 7.820, -5.865, 3.910, -9.775, 0.000, 9.775, -3.910 },
    { 5.865, 1.955, 1.955, 3.910, 0.000, 3.910, 1.955, 3.910, 3.910, 0.000, 3.910, 1.955 },
    { 9.775, 0.000, 5.865, 3.910, -5.865, 7.820, -1.955, 11.730, 1.955, 0.000, 5.865, -3.910 },
    { -3.421, -6.842, 3.422, -6.842, -3.421, 0.000, -3.421, -6.842, -10.263, 0.000, 3.421, -13.685 },
    { 5.865, 0.000, 1.955, 3.910, -1.955, 7.820, -1.955, 3.910, 1.955, 0.000, 5.865, -3.910 },
    { -5.865, -15.640, -1.955, -11.730, 1.955, -7.820, -17.595, -3.910, -13.685, 0.000, -9.775, 3.910 },
    { 10.266, -8.799, 3.422, -1.464, -3.422, 8.311, -8.799, 6.844, -6.844, 0.000, 4.401, -6.844 },
    { 11.730, 1.955, 3.910, 5.865, 1.955, 9.775, 0.000, 7.820, 3.910, 0.000, 7.820, 3.910 },
    { 9.775, -7.820, 5.865, 3.910, 1.955, 7.820, -1.955, 3.910, -5.865, 0.000, 13.685, -3.910 },
    { 0.000, -3.910, 3.910, 0.000, -3.910, 3.910, 0.000, 1.955, -7.820, 0.000, 1.955, -1.955 },
    { 7.539, -2.236, -5.307, 5.026, 1.955, 5.584, 2.513, 6.090, -0.281, 0.000, 6.981, 3.910 },
    { 5.865, -1.955, 1.955, 1.955, 0.000, 5.865, -3.910, 3.910, 0.000, 0.000, 3.910, -1.955 },
    { 8.798, 0.000, 2.933, 3.911, 0.000, 7.820, -1.955, 5.865, 1.956, 0.000, 5.865, -0.977 },
    { 5.865, 0.000, 1.955, 3.910, -1.955, 5.865, -1.955, 3.910, 1.955, 0.000, 5.865, -1.955 },
    { 5.865, -3.910, 1.955, 0.000, -1.955, 3.910, -5.865, 3.910, -1.955, 0.000, 1.955, -3.910 },
    { 12.569, -16.759, 4.190, 25.138, -4.190, 16.758, -12.569, 8.379, -20.949, 0.000, 20.948, -8.380 }
};

template <typename T, std::size_t N>
constexpr std::size_t countof(T const (&)[N]) noexcept
{
    return N;
}

static_assert(countof(NAMES) == countof(DEVIATIONS), "temperation table error");
static_assert(countof(NAMES) == size_t(TemperamentType::__TOTAL), "invalid temperament name table size");

namespace ceammc {
namespace music {
    std::string to_string(const TemperamentType& t)
    {
        if (t >= TemperamentType::__TOTAL)
            return "";

        return NAMES[size_t(t)];
    }

    double to_freq(size_t p, const TemperamentType& t, double a_base)
    {
        if (t >= TemperamentType::__TOTAL)
            return 0;

        auto cents = DEVIATIONS[size_t(t)][p % 12] * 0.01;

        return convert::midi2freq(p + cents, a_base);
    }
}
}
