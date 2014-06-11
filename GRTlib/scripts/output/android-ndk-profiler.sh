#!/bin/bash

check_result ()
{
  if [ $? -eq 0 ]; then
    printf "\n========== Operation succeded! ==========\n\n"
  else
    printf "\n__________ Operation faild! Abording execution ... __________\n\n"
    exit
  fi
}

adb pull /sdcard/gmon.out
arm-linux-androideabi-gprof ../../../GRT-Benchmark/obj/local/armeabi/libgrt.so
