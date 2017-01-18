

// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _Smoother_H_
#define _Smoother_H_
#include "Arduino.h"

//end of add your includes here


class Smoother{
	public:
	  Smoother(int size);
	  void add(float num);
	  float getSmoothed();
	  boolean exceedVariance(int variance);
	private:
	  int _size;
	  float *_arr;
	  int _curr_index;
	  float _sum;
};




//Do not add code below this line
#endif /* _Smoother_H_ */
