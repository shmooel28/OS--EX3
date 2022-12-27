.PHONY: all clean
all: crtFile clean
crtFile:
	gcc -o crtfile create_file.c
	./crtfile
	rm crtfile
	gcc Tcp.c -o tcp
	./tcp 
	rm tcp file2.txt
	gcc Unx_stream.c -o unx_s
	./unx_s
	rm unx_s
	gcc mmap.c -o mmap
	./mmap
	rm mmap file2.txt
	gcc pipe.c -o pipe
	./pipe
	rm pipe file2.txt
	gcc udp.c -o udp
	./udp
	rm udp file2.txt
	gcc -pthread thread.c -o thr
	./thr
	rm thr
clean:
	rm file1.txt file2.txt