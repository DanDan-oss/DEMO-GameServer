object=*.cpp *.cc
target=a.out
lib = -lzinx -lprotobuf -lpthread -lhiredis
cmd= -pthread -std=c++11 -g -Wall

zinxPath=/home/mohui/projects/ZinxIO/
staticObject=*.cpp *.cc /usr/local/lib/libprotobuf.a /usr/local/lib/libhiredis.a /usr/lib/libzinx.a

a.out:$(object)
	g++ $(cmd)  $^ -o ./bin/$@  $(lib)

game.out: $(staticObject)
	g++ -pthread -std=c++11 -g -Wall $^  -o ./bin/$@  -lpthread

clean:
	rm $(target) -f





