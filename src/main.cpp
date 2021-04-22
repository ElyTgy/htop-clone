#include "ncurses_display.h"
#include <iostream>
#include "processor.h"
#include "format.h"

//TODO after submission:
//display all CPUs
//store starttime value of each process so that it doesn't have to be calculated every time
//write PIDS function
//sort by ram usage / cpu usage

int main() {
  System system;
  NCursesDisplay::Display(system);
}