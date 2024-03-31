server: src/server.cpp
	g++ src/server.cpp -std=c++17 -o build/server

client: src/client.cpp
	g++ src/client.cpp -std=c++17 -o build/client