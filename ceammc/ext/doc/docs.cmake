set(DOC_FILES
    an.onset~
    an.pitchtrack~
    array.bpm
    array.copy
    array.do
    array.each
    array.fill
    array.hist
    array.mean
    array.minmax
    array.p2s
    array.resample
    array.rms
    array.s2p
    array.set
    array.stddev
    array.stretch
    array.sum
    array.sum2
    array.variance
    array.vplay
    array.window
    canvas.current
    canvas.dir
    canvas.name
    canvas.path
    canvas.top
    chaos.gbman0
    chaos.gbman0~
    click~
    conv.amp2dbfs
    conv.bpm2hz
    conv.bpm2ms
    conv.bpm2sec
    conv.car2pol
    conv.cc2amp
    conv.dbfs2amp
    conv.lin2curve
    conv.lin2exp
    conv.lin2lin
    conv.lin2lin~
    conv.list2props
    conv.midi2freq
    conv.phase2rad
    conv.phase2rad~
    conv.pitch2midi
    conv.pol2car
    conv.rad2phase
    conv.rad2phase~
    conv.samp2ms
    conv.samp2sec
    conv.sec2samp
    conv.sec2str
    conv.str2sec
    data.fifo
    data.float
    data.dict
    data.int
    data.list
    data.mlist
    data.set
    data.set2list
    dict.contains
    dict.each
    dict.from_list
    dict.get
    dict.keys
    dict.pass
    dict.reject
    dict.size
    dict.to_list
    dict.values
    dyn.comp2~
    dyn.comp~
    dyn.gate2~
    dyn.gate~
    dyn.limit2~
    dyn.limit~
    dyn.softclip~
    envelope
    env.mix
    env.tshift
    env.tscale
    env.vscale
    env.concat
    env.adsr~
    env.asr~
    env.ar~
    env.follow~
    env.smooth~
    env2array
    env2vline
    vline2env
    expand_env
    flow.append
    flow.change
    flow.count
    flow.gate
    flow.greater
    flow.group
    flow.less
    flow.less_eq
    flow.demultiplex
    flow.demultiplex~
    flow.demultiplex2~
    flow.interval
    flow.match
    flow.multiplex
    flow.multiplex~
    flow.multiplex2~
    flow.once
    flow.pack
    flow.pass
    flow.pass_if
    flow.reject
    flow.reject_if
    flow.route
    flow.speedlim
    flow.split
    flow.sync
    flow.sync_pack
    flow.tee~
    flt.biquad~
    flt.bpf12~
    flt.bpf24~
    flt.c_bpf~
    flt.c_highshelf~
    flt.c_hpf~
    flt.c_lowshelf~
    flt.c_lpf~
    flt.c_notch~
    flt.c_peak~
    flt.dcblock2~
    flt.dcblock~
    flt.eq10~
    flt.eq_peak~
    flt.eq_peak_cq~
    flt.highshelf~
    flt.hpf12~
    flt.hpf24~
    flt.lowshelf~
    flt.lpf12~
    flt.lpf24~
    flt.median
    flt.moog_vcf~
    flt.notch~
    flt.resonbp~
    fluid~
    function
    function.call
    fx.bitdown~
    fx.chorus~
    fx.distortion~
    fx.distortion1~
    fx.distortion2~
    fx.distortion3~
    fx.drive~
    fx.drone_box~
    fx.echo~
    fx.flanger~
    fx.freeverb2~
    fx.freeverb~
    fx.freqshift~
    fx.granulator~
    fx.greyhole~
    fx.looper~
    fx.pitchshift~
    fx.sdelay~
    fx.vocoder~
    fx.wahwah~
    fx.zita_rev1~
    gain~
    global.float
    global.dict
    global.int
    global.list
    global.mlist
    global.set
    hoa.2d.decoder~
    hoa.2d.encoder~
    hoa.2d.map~
    hoa.2d.optim~
    hoa.2d.projector~
    hoa.2d.recomposer~
    hoa.2d.rotate~
    hoa.2d.wider~
    hoa.in
    hoa.in~
    hoa.out
    hoa.out~
    hoa.process~
    hoa.scope~
    hoa.@process
    hw.apple_smc
    hw.apple_sms
    hw.arduino
    hw.cpu_temp
    hw.display
    hw.kbd_light
    is_any
    is_bang
    is_data
    is_dict
    is_even
    is_file
    is_float
    is_list
    is_odd
    is_pointer
    is_symbol
    lfo.+pulse~
    lfo.+saw~
    lfo.+square~
    lfo.+tri~
    lfo.impulse~
    lfo.saw~
    lfo.square~
    lfo.tri~
    lfo.pulse~
    list.^at
    list.^contains
    list.^search
    list.all_of
    list.any_of
    list.append
    list.apply_to
    list.at
    list.choice
    list.contains
    list.count
    list.count_if
    list.delta
    list.distribution
    list.do
    list.each
    list.enumerate
    list.equal
    list.first
    list.gen
    list.histogram
    list.insert
    list.integrator
    list.last
    list.length
    list.max
    list.mean
    list.min
    list.none_of
    list.normalize
    list.pass_if
    list.prepend
    list.product
    list.range
    list.reduce
    list.remove
    list.remove_if
    list.repack
    list.repeat
    list.resample
    list.resize
    list.reverse
    list.rldecode
    list.rlencode
    list.rotate
    list.route
    list.search
    list.separate
    list.set
    list.seq
    list.shift
    list.shuffle
    list.slice
    list.sort
    list.sort_with
    list.split
    list.stretch
    list.sum
    list.unique
    list.unpack
    list.unzip
    list.walk
    list.zip
    live.capture~
    local.float
    local.dict
    local.int
    local.list
    local.mlist
    local.set
    math.abs
    math.abs~
    math.acos
    math.acosh
    math.and
    math.approx
    math.asin
    math.asinh
    math.atan
    math.atanh
    math.cbrt
    math.cdiv~
    math.ceil
    math.cmul~
    math.cos
    math.cosh
    math.div
    math.e
    math.exp
    math.exp2
    math.expr
    math.floor
    math.gcd
    math.inf
    math.lcm
    math.log
    math.log10
    math.log2
    math.mul
    math.nan
    math.neg
    math.or
    math.pi
    math.pi~
    math.polyeval
    math.reciprocal
    math.round
    math.round~
    math.sign
    math.sin
    math.sinh
    math.sqrt
    math.squared
    math.sync_add
    math.sync_and
    math.sync_div
    math.sync_eq
    math.sync_ge
    math.sync_gt
    math.sync_le
    math.sync_lt
    math.sync_mod
    math.sync_mul
    math.sync_ne
    math.sync_or
    math.sync_sub
    math.sync_xor
    math.tan
    math.tanh
    math.trunc
    matrix~
    metro.pattern
    metro.random
    metro.seq
    midi.ctl2str
    midi.file
    midi.track
    midi.event2ctl
    midi.event2note
    midi.event2prg
    midi.prg2str
    midi.key2str
    mix~
    mlist.flatten
    modplug~
    msg
    msg.after
    msg.onload
    net.host2ip
    noise.crackle~
    noise.pink~
    noise.white~
    noise.lfreq~
    noise.lfreq0~
    obj.props
    osc.blit~
    osc.impulse~
    osc.pulse~
    osc.saw~
    osc.saw4~
    osc.sin~
    osc.sinfb~
    osc.square~
    osc.tri~
    pan.cos~
    pan.lin~
    pan.linsig~
    pan.spread~
    pan.sqrt~
    patch.args
    patch.props
    path.basename
    path.dirname
    path.exists
    path.is_dir
    path.lsdir
    preset.float
    preset.list
    preset.storage
    preset.symbol
    proto.firmata
    proto.sp.alpaca
    prop
    prop.declare
    prop.get
    prop.get~
    prop.set
    radio
    random.float
    random.gauss
    random.int
    random.linear
    random.pw_lin
    random.pw_const
    random.discrete
    replace
    set.contains
    set.diff
    set.equal
    set.intersect
    set.size
    set.symdiff
    set.union
    snd.file
    spat.pan4~
    spat.pan8~
    speech.flite
    string
    string.contains
    string.ends_with
    string.equal
    string.format
    string.join
    string.length
    string.match
    string.remove
    string.replace
    string.split
    string.starts_with
    string.substr
    string2symbol
    symbol.equal
    symbol.length
    symbol.num_compare
    synth.bee3~
    synth.birds~
    synth.church_bell~
    synth.dubdub~
    synth.eguitar~
    synth.fgrain~
    synth.ks~
    synth.marimba~
    synth.rhodey~
    synth.risset_arp~
    synth.risset_tone~
    synth.shakers~
    synth.wurley~
    system.colorpanel
    system.cursor
    system.getenv
    system.hostname
    system.memsize
    system.memused
    system.screen_size
    system.shell
    tl.bang
    tl.cue
    tl.timeline
    tl.toggle
    tl.transport
    ui.aview
    ui.bang
    ui.colorpanel
    ui.display
    ui.dsp~
    ui.env
    ui.gain~
    ui.gain2~
    ui.icon
    ui.incdec
    ui.keyboard
    ui.knob
    ui.matrix
    ui.menu
    ui.mouse_filter
    ui.mouse_route
    ui.number
    ui.label
    ui.link
    ui.number~
    ui.polar
    ui.preset
    ui.scope~
    ui.slider
    ui.slider2d
    ui.sliders
    ui.radio
    ui.rslider
    ui.tab
    ui.spectroscope~
    ui.toggle
    ui.meter~
    window
    xfade~
    xfade2~
    xdac~
)
