blktrace -a issue -d $1 -o - | blkparse -f "%p %d %S %n\n" -i - -o ../outputs/trace_output
