# BLOCK I/O
--------------------------------------------------------------
### [과제1]
리눅스에서 blktrace를 실행하여 일정한 형식으로 I/O trace를 출력한다.

##### [과제1 확장]
--------------------------------------------------------------
### [과제2]
리눅스에서 C언어를 사용하여 read I/O를 발생하는 프로그램을 작성한다. <br>
<br> - 입력 : start LBA, size
<br> - 동작 : 입력받은 LBA와 size에 대해 target device(default: /dev/nvme0n1)에서 read I/O를 발생시킨다.

##### [과제2 확장]
--------------------------------------------------------------
### [과제3]
입력으로 주어지는 파일의 byte address가 block device의 LBA로 어떻게 매핑되어 있는지 확인하는 프로그램을 작성한다.
<br> 프로그램은 두 부분으로 구성된다. <br>
<br> - Part I: 입력으로 주어지는 파일을 512byte씩 읽는 프로그램. O_DIRECT로 open()한다.
<br> - Part II: 위 프로그램을 실행하면서 캡처한 blktrace의 기록을 parsing하는 프로그램을 작성한다. <br>
<br> 과제 동작의 검증은 hdparm --fibmap [file_path] 명령어를 통해 출력을 비교하였다. <br>
##### < 실행 사진 (hdparm과 비교, 오른쪽 사진은 4.2GB의 large file) >
<div>
<img src="./assignment3/images/screenshot.png" height="320px"></img>
<img src="./assignment3/images/large_file.png" height="320px"></img>
</div>

<b>tracing</b>
<br> 1. $ make
<br> 2. $ sudo ./trace.sh [block_device] [file_path] [i/o_size] </br>
<br> <b>cleaning</b>
<br> 1. $ make clean | make clean_all </br>
<br> hdparm과 비교하면 sector의 수에서 약간의 차이가 발생한 것을 확인 할 수 있는데, read I/O의 크기를 512 bytes로 설정했기 때문이다. 옵션으로 read I/O를 file system의 block size와 동일한 4096 bytes로 설정하면 hdparm과 동일한 결과를 얻을 수 있다.</br>
