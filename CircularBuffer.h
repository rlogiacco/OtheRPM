/*
 * CircularBuffer.h
 *
 *  Created on: 04 dic 2016
 *      Author: rlogiacco
 */

#ifndef CIRCULARBUFFER_H_
#define CIRCULARBUFFER_H_

#include <inttypes.h>

template<typename T, uint16_t size>
class CircularBuffer {
public:
	enum {
		EMPTY = 0, HALF = size / 2, FULL = size,
	};

	CircularBuffer() : wp_(buf_), rp_(buf_), tail_(buf_ + size), used_(0) { }
	~CircularBuffer() { }

	void push(T value) {
		*wp_++ = value;
		if (used_ < size) used_++;
		if (wp_ == tail_) wp_ = buf_;
	}

	T pop() {
		T result = *rp_++;
		if (used_ > 0) used_--;
		if (rp_ == tail_) rp_ = buf_;
		return result;
	}

	int count() const {
		return used_;
	}

	T* values() {
		return buf_;
	}

private:
	T buf_[size];
	T *wp_;
	T *rp_;
	T *tail_;
	uint16_t used_;
};

#endif /* CIRCULARBUFFER_H_ */
