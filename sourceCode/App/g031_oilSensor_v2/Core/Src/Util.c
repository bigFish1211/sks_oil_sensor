#include <stdio.h>
#include <string.h>

void i_to_a(unsigned long n, char *s, int b) {
	char tmp_val[15] = {0};
	static char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
	int i=0, sign;

	if ((sign = n) < 0)
		n = -n;
	do {
		tmp_val[i++] = digits[n % b];
	} while ((n /= b) > 0);

	if (sign < 0)
		tmp_val[i++] = '-';

	//revert data
	int idx=0;
	for(int j=i-1; j>=0; j--) {
		s[idx] = tmp_val[j];
		idx++;
	}
	s[idx] = '\0';
}

void append_number(char* dest, unsigned long n) {
	char val[15];
	i_to_a(n,val,10);
	strcat(dest, val);
}

static char val[15];
void append_hexa(char* dest, unsigned long n) {
	i_to_a(n,val,16);
	if(n<16) {
		strcat(dest, "0");
	}
	strcat(dest, val);
}

void left_pad(unsigned long n, char* s, int size) {
	char tmp_val[15] = {0};
	char val[15];
	i_to_a(n,val,10);
	int pad = 0;

	unsigned long tmp = n;
	do {
		pad++;
	} while ((tmp /= 10) > 0);

	pad = size - pad;

	int i=0;
	for(i=0; i<pad; i++) {
		tmp_val[i] = '0';
	}
	tmp_val[i] = '\0';
	strcat(tmp_val, val);

	int idx=0;
	for(int i=0; i<6; i++) {
		s[idx] = tmp_val[i];
		idx++;
	}
	s[idx] = '\0';
}

/* atoi:  convert s to integer */
int a_to_i(char s[], unsigned char size)
{
	int i, n;

    n = 0;

    for (i = 0; i < size; ++i) {
		if(s[i] >= '0' && s[i] <= '9') {
        	n = 10 * n + (s[i] - '0');
		}
	}
    return n;
}

char toHex(unsigned char i) {
	if(i <= 9) {
		return '0' + i;
	} else {
		return 'a' - 10 + i;
	}
}

char isNumber(char val) {
	if(val >= '0' && val <= '9') {
		return 1;
	}
	return 0;
}

char charToNumber(char val) {
	if(val >= '0' && val <= '9') {
		return val - '0';
	}
	return 0;
}

char hexToi(char c) {
	char arr[17] = "0123456789abcdef\0"; //hex string
	for(int i=0; i<16; i++) {
		if(arr[i] == c) {
			return i;
		}
	}
	return 0;
}

void charToHexString(char* input, char* output, int size) {

}

char charToHex(const unsigned char c) {
	return (c/16 * 10) + (c%16);
}

unsigned long convertToSecond(char tm_hour, char tm_min, char tm_sec) {
	return (unsigned long)(((unsigned long)tm_hour * 3600) + ((unsigned long)tm_min * 60) + (unsigned long)tm_sec);
}

unsigned long convertToDate(int tm_day, int tm_mon, int tm_year) {
	return (unsigned long)((tm_year  * 10000l) + (tm_mon * 100) + (int)tm_day);
}
