#!/bin/bash

# Pull GRT test result from mobile device

OUTPUT_FILENAME=com.licenta.grt-benchmark.out

adb pull /sdcard/$OUTPUT_FILENAME
