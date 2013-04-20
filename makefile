# Author: Ashwin Ramesh
# SID: 311254012
# Assignment 1- A Simple Shell
# UOS: COMP3520 - Operating SYstem Internals

myshell: myshell.c utility.c myshell.h
		gcc -Wall myshell.c utility.c -o myshell
