# benjaesq@me.com

SRCS_CXX=$(wildcard *.cpp)
OBJS_CXX=$(SRCS_CXX:.cpp=.o)
CXX=g++
CXXFLAGS=-g -O0 -Wall -std=c++11
