all: umap
umap: main.cpp bank_um.h history_LL.h  md5.cpp
	g++ main.cpp -c -std=c++11 -O2 -D UMAP
	g++ main.o md5.cpp -o final_project -std=c++11 -O2

map: main.cpp bank.h history.h md5.cpp
	g++ main.cpp -c -std=c++11 -O2 -D MAP
	g++ main.o md5.cpp -o final_project -std=c++11 -O2

bonus: main.cpp md5.cpp bonus.cpp bank_um.h history_LL.h bonus.h
	g++ -std=c++11 -O2 -DBONUS -DUMAP  main.cpp -c 
	g++ -std=c++11 -O2 main.o md5.cpp -o final_project_BUM 
