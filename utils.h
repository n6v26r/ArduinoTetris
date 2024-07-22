#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdbool.h>

#pragma once

#define EXITONERR 1
#define ENDL "\n"

enum Type{
  info = 0,
  warn = 1,
  err = 2
};

const char* colors[] = { "\033[34m", "\033[33m", "\033[31m"};
const char* prefix[] = {"[INFO] ", "[WARN] ", "[ERR] "};

/* @debug
* type:
* 0 -> info
* 1 -> warn
* 2 -> error
* 
*/
void(* resetFunc) (void) = 0;

void _exit(int code){
  Serial.print("EXITED WITH CODE:");
  Serial.println(code, DEC);
  resetFunc();
}

void debug(FILE* file, const enum Type type, const char* msg, ...);

void terminate(int code);

void waitFor(int input){
}

void _init(){
  Serial.begin(9600);
}

void debug(const char* msg){
  Serial.print(msg);
}

// custom assert function, uk how it works
void assert(bool expressionValue, const enum Type type, const char* msg){
  if(expressionValue) return;
  Serial.print(msg);
  if(type==err && EXITONERR) terminate(3);
}

// convert time from timeval to a simple double (seconds)
double getTime(){
  return (double)millis()/1000;
}

void terminate(int code){
  debug("Terminating...");
  printf("\33[?25h");
  system("stty echo");
  _exit(code);
}
