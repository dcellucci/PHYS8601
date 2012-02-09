/*********************************************/
/* r1279.c                                   */
/*********************************************/


#define nab3 101280
#define	mask  (000000000000000000000000000000000001)

static int num;
static int ranvec[nab3];
static double ranvec2[nab3];

void rinitialize(int iseed);
void ranini(int iseed);
void random2(int number);
double ranf();
double randa(int number);

/******************************************************/
void rinitialize(int iseed)
{

	num = nab3 - 1280;
    ranini(iseed);
}

/******************************************************/
double ranf()
{
	double temp;

	temp = randa(num);
	num++;
	return temp;
}

/******************************************************/
void ranini(int iseed)
{
    int imod, i,j,k;
	int imax;
	double rmod, pmod, dmaxi;
	int tmask, jt;

	imax = 2147483647;
	dmaxi = 1.0/2147483647.0;
	rmod = (double)iseed;
	pmod = (double)imax;

	for(i=0;i<1000;i++) 
	{
		rmod = rmod * 16807.0;
		imod = (int)(rmod * dmaxi);
		rmod = rmod - pmod * imod;
	}


	for(i=0; i<1279;i++)
	{
		ranvec[i]=0;
		for(j=0;j<31;j++)
		{
			for(k=0;k<36;k++)
			{
				rmod = rmod * 16807.0;
				imod = (int)(rmod * dmaxi);
				rmod = rmod - pmod * imod;
			}
			rmod = rmod * 16807.0;
			imod = (int)(rmod * dmaxi);
			rmod = rmod - pmod * imod;
			if( rmod > ( 0.5 * pmod ) )
			{
				tmask = mask;
				jt=0;
				while(jt<j)
				{	
					tmask <<=1;
					jt++;
				}
				ranvec[i]|=tmask;
				
			}
		}
	}
	random2(1000);
}

/******************************************************/
void random2(int number)
{
	int i;
	for(i=0;i<number;i+=4)
	{
		ranvec[i+1279]=ranvec[i  ]^ranvec[i+216];
		ranvec[i+1280]=ranvec[i+1]^ranvec[i+217];
		ranvec[i+1281]=ranvec[i+2]^ranvec[i+218];
		ranvec[i+1282]=ranvec[i+3]^ranvec[i+219];
	}
	for(i=0;i<1276;i+=4)
	{
		ranvec[i  ] = ranvec[i  +number];
		ranvec[i+1] = ranvec[i+1+number];
		ranvec[i+2] = ranvec[i+2+number];
		ranvec[i+3] = ranvec[i+3+number];
	}
	ranvec[1276]=ranvec[1276+number];
	ranvec[1277]=ranvec[1277+number];
	ranvec[1278]=ranvec[1278+number];
	for(i=0;i<number;i++)
	{
		ranvec2[i]=(double)ranvec[i] * .0000000004656612875;
	}
}

/******************************************************/
double randa(int number)
{
	double temp;

	if(number >= (nab3-1280) )
	{
		random2(nab3-1280);
		temp = ranvec2[0];
		num = 2;
	}
	else
	{
		temp = ranvec2[number-1];
		num++;
	}
	return temp;
}

/******************************************************/
/*
int main(void)
{
	FILE *out1;
	int iseed, i;
	double temp1, temp2;

	iseed = 660522;
	out1 = fopen("out_c.prn", "w");
	
	printf("GENERATE RANDOM NUMBER WITH R1279 RANDOM GENERATOR\n");
	
	rinitialize(iseed);

	for(i=1;i<2001;i++)
	{
		temp1 = ranf();
		temp2 = ranf();
		printf("%9.7f  %9.7f \n", temp1, temp2);
		fprintf(out1, "%9.7f  %9.7f \n", temp1, temp2);
	}

	fclose(out1);
	return 0;
}
*/


