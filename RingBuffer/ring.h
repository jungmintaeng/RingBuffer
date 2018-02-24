#define BUF_SIZE 32
#define DATA_SIZE sizeof(int)
#define TIME_MEAN_FAST 1000
#define TIME_MEAN_SLOW 1500
typedef unsigned char byte;

class CustomRingbuffer {
public:
	byte* buffer;
	int dataStart, dataEnd;
	CustomRingbuffer();
};

CustomRingbuffer::CustomRingbuffer() {
	buffer = new byte[BUF_SIZE];
	dataStart = 0;
	dataEnd = 0;
}
