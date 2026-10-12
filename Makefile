#Emilio Montelongo Luna
#11317069
#rjq760
#CMPT 332-Fall-2026

# Ahmet Guldadov
# Student Number: 11369772
# NSID: six402
# CMPT 332-Fall-2026


CC = gcc
CFLAGS = -std=gnu90 -Wall -Wextra -pedantic -g

# PHASE 2 CROSS-COMP PLAN
# For Phase 2, we must compile s-chat for x86_64, ppc, and arm architectures.
# doing this by creating specific targets (s-chat-ppc, s-chat-arm)
# or by passing an ARCH variable. (lok at Makefile example in student/cmpt332
# We will change the RTT_LIB and PTH_LIB paths to point to the correct 
# subdirectories
# (e.g., /student/cmpt332/rtt/lib/Linuxppc or 
# /student/cmpt332/pthreads/lib/Linuxarmv7l).
# Will ask TA some questions on this.

CPPFLAGS = -I. -I/student/cmpt332/pthreads/ -I/student/cmpt332/rtt/include/

# Library paths for Linux x86_64
RTT_LIB = -L/student/cmpt332/rtt/lib/Linuxx86_64
PTH_LIB = -L/student/cmpt332/pthreads/lib/Linuxx86_64

all: libMonitor.a reader_writer_test s-chat liblist.a

# Part B: Monitor Library
libMonitor.a: Monitor_linux.o
	ar rcs libMonitor.a Monitor_linux.o

Monitor_linux.o: Monitor.c Monitor.h list.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c Monitor.c -o Monitor_linux.o

# Part B: Reader/Writer Test

reader_writer_test: readers_writers_linux.o reader_writer_monitor_linux.o \
       	libMonitor.a liblist.a
	$(CC) $(CFLAGS) -o reader_writer_test\
	       	readers_writers_linux.o \
		reader_writer_monitor_linux.o \
		-L. -lMonitor -llist\
		$(PTH_LIB) -lpthreads\
		$(RTT_LIB) -lRtt\
		-ltirpc
		

reader_writer_monitor_linux.o: reader-writer-monitor.c \
	reader-writer-monitor.h Monitor.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c reader-writer-monitor.c\
	       	-o reader_writer_monitor_linux.o

readers_writers_linux.o: reader-writer.c reader-writer-monitor.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c reader-writer.c -o \
		readers_writers_linux.o

# Part B & C: List Library (From A1)
liblist.a: list_adders_linux.o list_movers_linux.o list_removers_linux.o
	ar rcs liblist.a list_adders_linux.o list_movers_linux.o \
	 list_removers_linux.o

list_adders_linux.o: list_adders.c list.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c list_adders.c -o list_adders_linux.o

list_movers_linux.o: list_movers.c list.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c list_movers.c -o list_movers_linux.o

list_removers_linux.o: list_removers.c list.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c list_removers.c -o list_removers_linux.o


# Part C: S-chat
s-chat: s-chat_linux.o liblist.a
	$(CC) $(CFLAGS) -o s-chat s-chat_linux.o -L. -llist $(PTH_LIB)\
	       	-lpthreads

s-chat_linux.o: s-chat.c list.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c s-chat.c -o s-chat_linux.o

clean:
	rm -f *_linux.o reader_writer_test s-chat liblist.a libMonitor.a


.PHONY: all clean
