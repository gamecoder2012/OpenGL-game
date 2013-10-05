CC = g++
CFLAGS = -Wall
PROG = game

#SRCS = cse251_sampleCode.cpp
SRCS = file.cpp
LIBS = -lglut -lGL -lGLU

all: $(PROG)

$(PROG):	$(SRCS)
	$(CC) -w $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

clean:
	rm -f $(PROG)
