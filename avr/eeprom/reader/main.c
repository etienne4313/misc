#include <stdlib.h>
#include <stdio.h>

int checksum[] ={0xf,0xe,0xd,0xc,0xb,0xa,9,8,7,6,5,4,3,2,1,0};

int lookup[] ={15,93,223,143,159,207,79,31,191,239,111,63,47,127,255,175};

#define MIN(a,b) (((a)<(b))?(a):(b))

/*
 * ./reader 108100 dd
 * PROG 1a6 [68] X = 11
 * ==> 1a6c
 * ./reader 1a6c
 * Value = 0x1a6c
 * output = 108079
 * CHK = e593
 */
int main(int argc, char*argv[])
{
	int value,output,tmp;
	unsigned short prog, prog1;
	char *arg = argv[1];
	int x, min=1024, best, delta;

	if(argc == 2){
		value =  strtol(arg, NULL, 16);
		fprintf(stderr,"Value = 0x%x\n",value);

		output = (((value&0xf000)>>12) * 65536) + (((value&0x0f00)>>8) * 4096) + (((value&0x00f0)>>4) * 256);
		output = output + lookup[value&0xf];


		fprintf(stderr,"output = %d\n",output);
		fprintf(stderr,"CHK = %x%x%x%x\n",checksum[((value&0xf000)>>12)], checksum[((value&0x0f00)>>8)], checksum[((value&0x00f0)>>4)],
			checksum[((value&0x000f))]);
		return 0;
	}
	value =  strtol(arg, NULL, 10);

	tmp = (value/65536);
	prog = tmp;
//	fprintf(stderr,"%x ", tmp);
	value = value - (tmp*65536);

	tmp = (value/4096);
	prog = (prog<<4) | tmp;
	//fprintf(stderr,"%x ", tmp);
	value = value - (tmp*4096);

	tmp = (value/256);
	prog = (prog<<4) | tmp;
	//fprintf(stderr,"%x ", tmp);
	value = value - (tmp*256);

	fprintf(stderr,"PROG %x ", prog);
	prog1 = prog;

	tmp = value;
	prog = (prog<<4) | tmp;
	fprintf(stderr,"[%d] ", tmp); //LOOKUP table closet possible

		for(x=0; x<16; x++){
			if(tmp >= lookup[x])
				delta = tmp - lookup[x];
			else
				delta = lookup[x] - tmp;
			if(MIN(min, delta) < min){
				min = delta;
				best = x;
			}
		}
		fprintf(stderr,"X = %d\n",best);

	fprintf(stderr,"==> %x%x\n",prog1,best+1);

/*
 * ./reader 129600 dummy
 * 1 f a [64] EX 64 ~= 63 POSITION 12 ==>0xc
 *
 * ==> 0x1fac
 * ./reader 1fac
 * Value = 0x1fac
 * output = 129583
 * CHK = e053
 *
 */
	return 0;
}
