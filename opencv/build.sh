#!/bin/bash
# build.sh
# Copyright (c) 2019 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
# 
for dir in ./* 
do
  if [ -d $dir ];then
    if [ -e "$dir/Makefile" ];then
      cd $dir 
      make clean; make 
      cd ../
    fi 
  fi
done
