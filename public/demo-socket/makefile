all:demo-client-write demo-epoll-write

demo-client-write:demo-client-write.cpp
	g++ demo-client-write.cpp -g -o demo-client-write  -lm -lc
	mv -f demo-client-write ./bin/.
demo-epoll-write:demo-epoll-write.cpp
	g++ demo-epoll-write.cpp -g -o demo-epoll-write ../_public.cpp -lm -lc
	mv -f demo-epoll-write ./bin/.

clean:
	rm -rf ./bin/*
runc:
	./bin/demo-client-write 127.0.0.1 5008
runs:
	./bin/demo-epoll-write 5008

runt:
	sh ./tmp2.sh
