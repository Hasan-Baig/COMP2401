all: plannerTester bigEnvironment
	gcc -o plannerTester plannerTester.o display.o pathPlanner.o -lX11
	gcc -o bigEnvironment bigEnvironment.o display.o pathPlanner.o -lX11

plannerTester: plannerTester.c display.c display.h obstacles.h
	gcc -c plannerTester.c display.c pathPlanner.c obstacles.h

bigEnvironment: bigEnvironment.c display.c display.h obstacles.h
	gcc -c bigEnvironment.c display.c pathPlanner.c obstacles.h

clean:
	rm -f *.o
