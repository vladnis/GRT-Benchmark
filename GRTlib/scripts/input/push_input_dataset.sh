#!/bin/bash

# Push input dataset for Gesture Recognition Toolkit to mobile device

#File from "dataset_folder" that will be pushed to mobile device
dataset_filename=DTWTrainingData.txt

dataset_folder=./datasets

filename_on_device=com.licenta.grt-benchmark.in

# Push file to device
cp $dataset_folder/$dataset_filename $filename_on_device
adb push $filename_on_device /sdcard/
rm -f $filename_on_device
