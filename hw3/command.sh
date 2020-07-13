blktrace -a issue -d /dev/nvme0n1 -o - | blkparse -f "%p %d %S %n\n" -i - -o output.txt
