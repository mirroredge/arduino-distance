/*
 * Smoother.cpp
 *
 *  Created on: Aug 25, 2015
 *      Author: mike
 */
#include "Smoother.h"

Smoother::Smoother(int size){
	_size = size;
	_arr = new float[size];
	_curr_index = 0;
	_sum = 0;

	for(int i = 0; i < _size; i++){
		_arr[i] = 0;
	}
}

void Smoother::add(float num){
	_sum -= _arr[_curr_index];
  num *= 10;
	_arr[_curr_index] = num;
	_sum += num;
	_curr_index++;
	if(_curr_index >= _size){
		_curr_index = 0;
	}
}

float Smoother::getSmoothed(){
	return ((_sum/10.0)/_size);
}

boolean Smoother::exceedVariance(int variance){
	int avg = Smoother::getSmoothed();
	for(int i = 0; i < _size; i++){
		if(_arr[i] > avg + variance || _arr[i] < avg - variance){
			return false;
		}
	}
	return true;
}

