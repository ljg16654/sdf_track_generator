#!/bin/sh

OUTPUT_PATH=path.txt
OUTPUT_SDF=custom_track.sdf
BASE_SDF_FILE=model_single_cone.sdf

test -e $BASE_SDF_FILE || exit
mkdir -p build
make -k
python3 gen_pose.py
[ -e $OUTPUT_PATH ] && rm $OUTPUT_PATH
[ -e $OUTPUT_SDF ] && rm $OUTPUT_SDF
echo "<?xml version='1.0'?>" > $OUTPUT_SDF
build/gen_track $OUTPUT_PATH
build/gen_sdf $OUTPUT_PATH $BASE_SDF_FILE >> $OUTPUT_SDF
