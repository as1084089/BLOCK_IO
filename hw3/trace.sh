#!/bin/bash

if [ $# -ne 3 ]; then
  echo "usage: ./trach.sh [BLOCK_DEVICE] [FILE_PATH] [I/O_SIZE]"
  exit
elif [ ! -b $1 ] || [ ! -r $2 ]; then
  echo "wrong parameters."
  exit
fi

if [ ! ${EUID} = 0 ]; then
  echo "run as root permission."
  exit
fi

echo " ========================    I/O tracing start    ========================"
./command.sh $1 &
sleep 1
if [ ! -n "`pgrep blktrace`" ]; then
  echo "I/O tracing failed."
  exit
fi
echo " ========================   I/O generating start  ========================"
./io_generator $2 $3
echo " ======================== I/O generating finished ========================"
sleep 1
kill -15 `pgrep blktrace` 1> /dev/null
echo " ========================   I/O tracing finished  ========================"
echo ""
sleep 1
./parser trace_output $3
