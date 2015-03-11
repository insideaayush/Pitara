server=localhost 5005
port=5005

server:
		cd ./Src/Server; g++ -std=c++11 -w -o server server.cpp -L/usr/lib -lssl -lcrypto -luuid; ./server $(port); 
client:
		cd ./Src/Client/QtApp; qmake login_screen.pro -o MyMakeFile; make -w -f MyMakeFile; ./login_screen $(server) 
doc:
		cd ./Doc; pdflatex COP290_Ass2_Doc.tex;
DEBUG:

clean:
	cd ./Doc; \rm -rf *.out *~ *.pdf *.aux *.log;
	cd ./Src/Server; \rm -rf *.out *~ *.pdf *.aux *.log;rm server;
	cd ./Src/Client/QtApp; make -f MyMakeFile clean; rm login_screen;