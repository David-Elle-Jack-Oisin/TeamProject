default :
	g++ -Wall -c main.cpp -o build/main.o -I ./include/ 
	g++ build/main.o -o game.exe -L ./lib/ -lGameNetworkingSockets -lraylib  -lm -pthread -ldl

	g++ -Wall -c src/Mains/mainServer.cpp -o build/server.o -I ./include/ 
	g++ ./build/server.o -o server.exe -L ./lib/ -lGameNetworkingSockets -lraylib -lm -pthread -ldl
	./game.exe

test :
	g++ -Wall -c src/test/testMain.cpp -o build/testmain.o -I ./include/ 
	g++ build/testmain.o -o test.exe -L ./lib/ -lGameNetworkingSockets -lraylib -lgtest  -lm -pthread -ldl
	./test.exe