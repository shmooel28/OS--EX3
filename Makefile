.PHONY: all clean run
all: crtFile
run: 
	./crtfile
	rm crtfile
	./tcp 
	rm tcp file2.txt
	./unx_s
	rm unx_s file2.txt
	./unx_d
	rm unx_d file2.txt
	./mmap
	rm mmap file2.txt
	./pipe
	rm pipe file2.txt
	./udp
	rm udp file2.txt
	./thr
	rm thr file1.txt file2.txt
crtFile:
	gcc -o crtfile create_file.c
	gcc Tcp.c -o tcp
	gcc Unx_stream.c -o unx_s
	gcc Unx_data.c -o unx_d
	gcc mmap.c -o mmap
	gcc pipe.c -o pipe
	gcc udp.c -o udp
	gcc -pthread thread.c -o thr

clean:
	rm file1.txt file2.txt