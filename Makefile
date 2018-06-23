CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g -std=c++11 -Wall -pedantic
LDFLAGS=-g
LDLIBS=

SRCS=assign2_part1.cpp assign2_part2.cpp assign2_part3.cpp assign2_part4.cpp assign2_part5.cpp assign2_part6.cpp assign2_part7.cpp assign2_part8.cpp instruction.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

part1: 
	$(CXX) $(CPPFLAGS) -o assign2_part1 assign2_part1.cpp instruction.cpp $(LDLIBS) 

part2: 
	$(CXX) $(CPPFLAGS) -o assign2_part2 assign2_part2.cpp instruction.cpp $(LDLIBS)

part3: 
	$(CXX) $(CPPFLAGS) -o assign2_part3 assign2_part3.cpp instruction.cpp $(LDLIBS)

part4: 
	$(CXX) $(CPPFLAGS) -o assign2_part4 assign2_part4.cpp instruction.cpp $(LDLIBS)

part5: 
	$(CXX) $(CPPFLAGS) -o assign2_part5 assign2_part5.cpp instruction.cpp $(LDLIBS)

part6: 
	$(CXX) $(CPPFLAGS) -o assign2_part6 assign2_part6.cpp instruction.cpp $(LDLIBS)

part7: 
	$(CXX) $(CPPFLAGS) -o assign2_part7 assign2_part7.cpp instruction.cpp $(LDLIBS)

part8: 
	$(CXX) $(CPPFLAGS) -o assign2_part8 assign2_part8.cpp instruction.cpp $(LDLIBS)
clean:
	$(RM) $(OBJS) *.hack
