default: all

main3: main3.cpp
	g++ -std=c++0x main3.cpp -o main3

main4: main4.cpp
	g++ -std=c++0x main4.cpp -o main4

all: main3.cpp main4.cpp
	g++ -std=c++0x main3.cpp -o main3
	g++ -std=c++0x main4.cpp -o main4

clean:
	rm main3
	rm main4
