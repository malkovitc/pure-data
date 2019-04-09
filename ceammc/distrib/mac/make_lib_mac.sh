#!/bin/sh

if [ $# -ne 4 ]
then
    echo "Usage: $0 SRCDIR BINDIR OUTDIR VERSION"
fi

SRCDIR="$1"
BINDIR="$2"
VERSION="$4"
OUTDIR="$3/ceammc"
SYSVER=$(sw_vers | grep ProductVersion | cut -f2 | cut -f1,2 -d.)
OUTFILE="ceammc-${VERSION}-macosx-${SYSVER}-pd-@PD_TEXT_VERSION_SHORT@.tar.gz"
DYLIBBUNDLER="@DYLIBBUNDLER@"


function skip_ext {
    #skip experimental extensions
    exp=$(echo $1 | grep -v '/exp' | grep -v '/tl' | grep -v '/test')
    if [ -z $exp ]
    then
        return 1
    else
        return 0
    fi
}

echo "Making CEAMMC library from build directory: $BINDIR"
mkdir -p "${OUTDIR}"
rm -f "${OUTDIR}/*"

echo "Copying libraries to ${OUTDIR} ..."
find "${BINDIR}" -name *.dylib -print0 | while read -r -d '' file
do
    cp "$file" "${OUTDIR}"
    echo "+ Lib:  $(basename $file)"
done

find_ext() {
    find "$1" -name "$2\\.d_fat" \
        -o -name "$2\\.d_amd64" \
        -o -name "$2\\.pd_darwin" \
        -o -name "$2\\.d_i386"
}


echo "Copying extension files to ${OUTDIR} ..."
find_ext ${BINDIR} "*" | while read file
do
    ext_name=$(basename $file)
    skip_ext $file
    if [ $? -eq 1 ]
    then
        echo "- Skip: '$ext_name'"
        continue
    fi

    cp "$file" "${OUTDIR}/${ext_name}"
    echo "+ Copy: '$ext_name'"
    ${DYLIBBUNDLER} -x ${OUTDIR}/$ext_name -b -d ${OUTDIR} -p @loader_path/ -of
done

echo "Copying [system.serial] extension files to ${OUTDIR} ..."
find_ext "${BINDIR}/../extra/comport" "*" | while read file
do
    ext_name=$(basename $file)
    skip_ext $file
    if [ $? -eq 1 ]
    then
        echo "- Skip: '$ext_name'"
        continue
    fi

    cp "$file" "${OUTDIR}/${ext_name}"
    echo "+ Copy: '$ext_name'"
    ${DYLIBBUNDLER} -x ${OUTDIR}/$ext_name -b -d ${OUTDIR} -p @loader_path/ -of
done

ceammc_lib=$(find_ext "${BINDIR}" ceammc)
cp $ceammc_lib "${OUTDIR}"
${DYLIBBUNDLER} -x ${OUTDIR}/$(basename $ceammc_lib) -b -d ${OUTDIR} -p @loader_path/ -of

echo "Copying help files to ${OUTDIR} ..."
find "${SRCDIR}/ext/doc" -name *-help\\.pd | while read file
do
    help=$(basename $file)
    cat "$file" |
        sed 's/ceammc\/ceammc-help\.pd/ceammc-help.pd/' |
        sed 's/\.\.\/index-help\.pd/index-help.pd/' > "${OUTDIR}/${help}"
    echo "+ Help: '$help'"
done

echo "Copying wrapper help files to ${OUTDIR} ..."
find "${SRCDIR}/ext/class-wrapper/modules" -name *-help\\.pd | while read file
do
    help=$(basename $file)
    cat "$file" |
        sed 's/ceammc\/ceammc-help\.pd/ceammc-help.pd/' |
        sed 's/\.\.\/index-help\.pd/index-help.pd/' > "${OUTDIR}/${help}"
    echo "+ Help: '$help'"
done

echo "Copying STK rawwaves files to ${OUTDIR}/stk ..."
mkdir -p "${OUTDIR}/stk"
find "${SRCDIR}/extra/stk/stk/rawwaves" -name *\\.raw | while read file
do
    help=$(basename $file)
    cp "$file" "${OUTDIR}/stk"
    echo "+ RAW: '$help'"
done

echo "Copying CEAMMC wav examples to ${OUTDIR} ..."
find "${SRCDIR}/ext/doc" -name *\\.wav | while read file
do
    help=$(basename $file)
    cp "$file" ${OUTDIR}
    echo "+ WAV: '$help'"
done

echo "Copying SF2 fonts to ${OUTDIR}/sf2 ..."
cp -R "${SRCDIR}/extra/fluidsynth/fluidsynth/sf2" ${OUTDIR}

echo "+ Copying abstractions:"
for abs in ${SRCDIR}/ext/abstractions/*.pd
do
    echo "    $(basename $abs)"
    cp "${abs}" "${OUTDIR}"
done

echo "+ Copying misc files:"
echo "    stargazing.mod"
cp "${SRCDIR}/ext/doc/stargazing.mod" "${OUTDIR}"
echo "    prs.txt"
cp "${SRCDIR}/ext/doc/prs.txt" "${OUTDIR}"
echo "    soundtouch~"
soundtouch_ext=$(find_ext "${BINDIR}/../extra/SoundTouch/pd" "soundtouch~")
cp "$soundtouch_ext" "${OUTDIR}"
${DYLIBBUNDLER} -x ${OUTDIR}/$(basename $soundtouch_ext) -b -d ${OUTDIR} -p @loader_path/ -of
echo "    soundtouch~-help.pd"
cp "${BINDIR}/../extra/SoundTouch/pd/soundtouch~-help.pd" "${OUTDIR}"
echo "    soundtouch-help.pd"
cp "${BINDIR}/../extra/SoundTouch/pd/soundtouch-help.pd" "${OUTDIR}"

echo "+ Fix soundtouch link in index-help.pd..."
sed -i "" 's/ceammc\/soundtouch-help\.pd/soundtouch-help.pd/' "${OUTDIR}/index-help.pd"

cd "$3"
tar cfvz "${OUTFILE}" $(basename $OUTDIR)
mv "${OUTFILE}" ..
