blktrace -a complete -a issue -d /dev/sda -o - | blkparse -f "%M %m %d %a %S %n %T.%t\n" -i - | grep "8 0 R"
