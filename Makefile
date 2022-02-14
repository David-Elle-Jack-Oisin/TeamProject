default:
	g++ -Wall -c main.cpp -o build/main.o -I ./include/ 
	g++ build/main.o -o test.exe -L ./lib/ -lGameNetworkingSockets -lraylib  -lm -pthread -ldl

	g++ -Wall -c src/network/server.cpp -o build/server.o -I ./include/ 
	g++ ./build/server.o -o server.exe -L ./lib/ -lGameNetworkingSockets -lraylib -lm -pthread -ldl
	./test.exe