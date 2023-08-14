#include "stdio.h"
#include "string.h"
#include "Util.h"

char isleapyear(int year){   //ham kiem tra nam nhuan
    if((year %4 ==0) && ((year %100 !=0) || (year % 400==0))) return 1;
    else return 0;
}

char truedate(int dd, int mm, int yy) {   //ham kiem tra ngay, thang nam nhap vao co hop le ko
    char dayofmonth[13] = {31, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if(yy>= 2000){
        if(mm>=1 && mm<=12){
            if(isleapyear(yy)) dayofmonth[2]++;
            if(dd>0 && dd<=(int)dayofmonth[mm]) { 
				return 1;
			}  else {
				return 0;
			}
        } else { 
			return 0;
		}
    } else {
		return 0;
	}
}

char truetime(int h, int m, int s) {
	if(h >= 0 && h < 24) {
		if(m >=0 && m < 60) {
			if(s >= 0 && s < 60) {
				return 1;
			} else return 0;
		} else return 0;
	} else return 0;
}

int getHour(char* dt) {
	char tmpT[2] = {0};

	for(char i = 0; i<2; i++) {
		tmpT[i] = dt[i];
	}
	return a_to_i(tmpT, 2);
}

int getMinute(char* dt) {
	char tmpT[2] = {0};
	for(char i = 0; i<2; i++) {
		tmpT[i] = dt[i+3];
	}
	return a_to_i(tmpT, 2);
}

int getSecond(char* dt) {
	char tmpT[2] = {0};
	for(char i = 0; i<2; i++) {
		tmpT[i] = dt[i+6];
	}
	return a_to_i(tmpT, 2);
}

char istruetime(char* dt) {
	for(char i = 0; i < 8; i++) {
		if(dt[i] == '_') {
			return 0;
		}
	}	
	return truetime(getHour(dt), getMinute(dt), getSecond(dt));
}

int getDay(char* dt) {
	char tmpT[2] = {0};
	for(char i = 0; i<2; i++) {
		tmpT[i] = dt[i];
	}
	return a_to_i(tmpT, 2);
}

int getMon(char* dt) {
	char tmpT[2] = {0};
	for(char i = 0; i<2; i++) {
		tmpT[i] = dt[i+3];
	}
	return a_to_i(tmpT, 2);
}

int getYear(char* dt) {
	char tmpT[4] = {0};
	for(char i = 0; i<4; i++) {
		tmpT[i] = dt[i+6];
	}
	return a_to_i(tmpT, 4);
}

char istruedate(char* dt) {
	for(char i = 0; i < 10; i++) {
		if(dt[i] == '_') {
			return 0;
		}
	}	
	return truedate(getDay(dt), getMon(dt), getYear(dt));
}
