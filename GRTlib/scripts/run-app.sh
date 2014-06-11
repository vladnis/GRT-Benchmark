#!/bin/bash

PACKAGE_NAME=com.licenta.grt_benchmark;

ACTIVITY=MainActivity


adb shell am start -n $PACKAGE_NAME/$PACKAGE_NAME.$ACTIVITY
#adb logcat com.licenta.grt_benchmark:* *:S;
adb logcat | grep `adb shell ps | grep com.licenta.grt_benchmark | cut -c10-15`
