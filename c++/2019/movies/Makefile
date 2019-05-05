all:	movies


movies:	main.cpp OmdbApi.o CurlRest.o
	g++ main.cpp OmdbApi.o CurlRest.o -lcurl -ljsoncpp -I include -o movies

OmdbApi.o:	src/OmdbApi.cpp include/OmdbApi.h
	g++ -c src/OmdbApi.cpp -I include

CurlRest.o:	src/CurlRest.cpp include/CurlRest.h
	g++ -c src/CurlRest.cpp -I include

clean:
	rm -f movies OmdbApi.o CurlRest.o
