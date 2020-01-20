#!/bin/bash        
if [ -z "$1" ]; then 
  echo usage: $0 "mac or linux"
  exit
fi

for d in `ls projects`; 
do
  cd projects/${d}
  rm -rf nbproject
  mv nbproject.macosx nbproject.mac
  cp -R nbproject.${1} nbproject
  make
  cd ../..
done
