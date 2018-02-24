#include <thread>
#include <iostream>
#include <random>
#include "ring.h"

using namespace std;

/*
 * function
 */
void generate(CustomRingbuffer* mBuf);
void process(CustomRingbuffer* mBuf);
byte* intToByte(int value);
int byteToInt(byte* value);

/*
 * global variable	--> 가우시안 분포로 난수를 발생시킴
 */
default_random_engine randomEngine(time(NULL));
normal_distribution<> fastDist(TIME_MEAN_FAST, 250);
normal_distribution<> slowDist(TIME_MEAN_SLOW, 250);

int main() {
	clock_t begin, end;
	begin = clock();
	cout << "[Software Engineering Assignment 2012726006 신정민]\n" << endl;
	cout << "\t\t끝내기 : q" << endl;
	CustomRingbuffer* mBuf = new CustomRingbuffer();
	
	thread generator(&generate, mBuf);
	thread processor(&process, mBuf);
	generator.join();
	processor.join();
	end = clock();
	cout << "수행시간 : " << end - begin << endl;
	generator.~thread();
	processor.~thread();
	while (getchar());
	exit(0);
}

byte* intToByte(int value) {
	byte* target = new byte[4];
	target[3] = (value & 0xff);
	target[2] = ((value >> 8) & 0xff);
	target[1] = ((value >> 16) & 0xff);
	target[0] = ((value >> 24) & 0xff);
	return target;
}

int byteToInt(byte* value) {
	return value[3] & 0xff | (value[2] & 0xff) << 8 | (value[1] & 0xff) << 16 | (value[0] & 0xff) << 24;
}

void generate(CustomRingbuffer* mBuf) {
	int generateNumber = 0;
	byte* integerData = new byte[4];
	while (1) {
		integerData = intToByte(generateNumber);
		memcpy(mBuf->buffer + (mBuf->dataEnd % BUF_SIZE), integerData, DATA_SIZE);
		printf("++발생 : (%d)\n", generateNumber++);
		mBuf->dataEnd += DATA_SIZE;
		std::this_thread::sleep_for(std::chrono::milliseconds((long)fastDist(randomEngine)));
		if (generateNumber == 100)
			return;
	}
}

void process(CustomRingbuffer* mBuf) {
	byte* integerData = new byte[DATA_SIZE];
	int data;
	while (1) {
		if (mBuf->dataStart >= mBuf->dataEnd)
			continue;
		memcpy(integerData, &mBuf->buffer[mBuf->dataStart % BUF_SIZE], DATA_SIZE);
		data = byteToInt(integerData);
		printf("--소모 : \t[%d]\n", data);
		mBuf->dataStart += DATA_SIZE;
		if (data >= 99) {
			return;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds((long)slowDist(randomEngine)));
	}
}