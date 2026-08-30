all:demo-client-write demo-client-read demo-epoll-write demo-select demo-poll

demo-client-write:demo-client-write.cpp
	g++ demo-client-write.cpp -g -o demo-client-write  -lm -lc
	mv -f demo-client-write ./bin/.
demo-client-read:demo-client-read.cpp
	g++ demo-client-read.cpp -g -o demo-client-read  -lm -lc
	mv -f demo-client-read ./bin/.
demo-epoll-write:demo-epoll-write.cpp
	g++ demo-epoll-write.cpp -g -o demo-epoll-write ../_public.cpp -lm -lc
	mv -f demo-epoll-write ./bin/.
demo-select:demo-select.cpp
	g++ demo-select.cpp -g -o demo-select ../_public.cpp -lm -lc
	mv -f demo-select ./bin/.
demo-poll:demo-poll.cpp
	g++ demo-poll.cpp -g -o demo-poll ../_public.cpp -lm -lc
	mv -f demo-poll ./bin/.

clean:
	rm -rf ./bin/*
runc:
	./bin/demo-client-write 127.0.0.1 5008
runs:
	./bin/demo-epoll-write 5008
runc1:
	./bin/demo-client-read 127.0.0.1 5058
runs1:
	./bin/demo-select 5008

runt:
	sh ./tmp2.sh
