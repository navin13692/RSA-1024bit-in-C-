#include <stdlib.h> 
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "rsa.h"


bool rsa1024(uint64_t res[], uint64_t data[], uint64_t expo[],uint64_t key[])
{
    int32_t i,j,expo_len;
    uint64_t mod_data[18]={0},result[18]={0};
    uint64_t temp_expo=0;

    modbignum(mod_data,data,key,16);
    result[0] = 1;
    expo_len = bit_length(expo,16) /64;
    for(i=0;i<expo_len+1;i++)
    {
        temp_expo = expo[i];
        for(j=0;j<64;j++)
        {
            if(temp_expo & 0x1UL)
                modmult1024(result,result,mod_data,key);

            modmult1024(mod_data,mod_data,mod_data,key);
            temp_expo = temp_expo >> 1;
        }
    }
    for(i=0;i<16;i++)
        res[i]=result[i];


    return 1;
}




bool addbignum(uint64_t res[], uint64_t op1[], uint64_t op2[],uint32_t n)
{
	uint32_t i;
	uint64_t j,k,carry=0;
	for(i = 0; i<n; i++)
	{
		j = (op1[i] & 0xffffffff) + (op2[i] & 0xffffffff) + carry;
		
		k = ((op1[i]>>32) & 0xffffffff) + ((op2[i]>>32) & 0xffffffff) + ((j>>32) & 0xffffffff);
				
		carry = ((k>>32) & 0xffffffff);
		
		res[i] = ((k & 0xffffffff)<<32)  | (j & 0xffffffff);
	}
	res[i] = carry;
	return 0;
} 

bool multbignum(uint64_t res[], uint64_t op1[], uint32_t op2 ,uint32_t n)
{
	uint32_t i;
	uint64_t j,k,carry1=0,carry2=0;
	for(i = 0; i<n; i++)
	{
		j = (op1[i] & 0xffffffff) * (op2 & 0xffffffff);
		
		k = ((op1[i]>>32) & 0xffffffff) * (op2 & 0xffffffff);
		carry1 = ((k>>32) & 0xffffffff);
		k = (k & 0xffffffff) + ((j>>32) & 0xffffffff);
		j = (j & 0xffffffff) + carry2;
		k = k + ((j>>32) & 0xffffffff);
		carry2 = carry1 + ((k>>32) & 0xffffffff);
		
		res[i] = ((k & 0xffffffff)<<32)  | (j & 0xffffffff);
	}
	res[i] = carry2;
	return 0;
} 
bool modmult1024(uint64_t res[], uint64_t op1[], uint64_t op2[],uint64_t mod[]) //optimized
{
    int32_t i,j;
    uint64_t mult1[33]={0},mult2[33]={0},
            result[33]={0},xmod[33]={0};

    for(i=0;i<16;i++)
        xmod[i]=mod[i];

    for(i=0;i<16;i++)
    {
        for(j=0;j<33;j++)
        {
            mult1[j]=0;
            mult2[j]=0;
        }
        multbignum(mult1,op1,(op2[i]&0xffffffff),16);
        multbignum(mult2,op1,((op2[i]>>32)&0xffffffff),16);
        slnbignum(mult2,mult2,33,32);
        addbignum(mult2,mult2,mult1,32);

        slnbignum(mult2,mult2,33,64*i);

        addbignum(result,result,mult2,32);

    }
    modbignum(result,result,xmod,33);
    for(i=0;i<16;i++)
            res[i]=result[i];

    return 0;
}
/*
bool modmult1024(uint64_t res[], uint64_t op1[], uint64_t op2[],uint64_t mod[])
{
	int32_t i,j;
	uint64_t mult1[19]={0},mult2[19]={0},result[18]={0};

	for(i=0;i<16;i++)
	{
		multbignum(mult1,op1,(op2[i]&0xffffffff),16);

		multbignum(mult2,op1,((op2[i]>>32)&0xffffffff),16);
		slnbignum(mult2,mult2,17,32);
		addbignum(mult2,mult2,mult1,17);
		modbignum(mult2,mult2,mod,17);

		for(j=0;j<i;j++)
		{
		    slnbignum(mult2,mult2,17,64);
		    modbignum(mult2,mult2,mod,17);
		}
		
		addbignum(result,result,mult2,16);
		modbignum(result,result,mod,17);
		
	}
	for(i=0;i<16;i++)
	        res[i]=result[i];
	
	return 0;
}
*/
bool modbignum(uint64_t res[],uint64_t op1[], uint64_t op2[],uint32_t n)//optimized
{
    uint32_t i;
    int32_t len_op1,len_op2,len_dif;

    len_op1 = bit_length(op1,n);
    len_op2 = bit_length(op2,n);
    len_dif = len_op1 - len_op2;



    for(i=0;i<n;i++)
        res[i]=op1[i];

    if(len_dif < 0)
    {
        return 1;
    }

    if(len_dif == 0)
    {
        while(compare(res,op2,n)>=0)
        {
            subbignum(res,res,op2,n);
        }
        return 1;
    }

    slnbignum(op2,op2,n,len_dif);
    for(i=0;i<len_dif;i++)
    {
        srnbignum(op2,op2,n,1);
        while(compare(res,op2,n)>=0)
        {
            subbignum(res,res,op2,n);
        }
    }

    return 1;
}

/*
bool modbignum(uint64_t res[],uint64_t op1[], uint64_t op2[],uint32_t n)
{
    uint32_t i;
	int32_t len_op1,len_op2,len_dif;
	
	len_op1 = bit_length(op1,n);
	len_op2 = bit_length(op2,n);
	len_dif = len_op1 - len_op2;
	
	for(i=0;i<n;i++)
		res[i]=op1[i];
	
	if(len_dif < 0)
	{		
		return 1;
	}
	if(len_dif == 0)
	{
		modnum(res,res,op2,n);
		return 1;
	}
	
	slnbignum(op2,op2,n,len_dif);
	for(i=0;i<len_dif;i++)
	{
		srnbignum(op2,op2,n,1);
		modnum(res,res,op2,n);
	}
	return 1;
}
*/
/****************************************************************
 * bool modnum(uint64_t res[],uint64_t op1[], uint64_t op2[],uint32_t n)
 * res = op1 % op2
 * n is bit length/64
 * res must have extra 64 bits to avoid errors 
 ****************************************************************/
bool modnum(uint64_t res[],uint64_t op1[], uint64_t op2[],uint32_t n)
{
	uint32_t i;
	bool result=0;
	for(i=0;i<n;i++)
		res[i]=op1[i];
		
	while(!result)
	{
		result = subbignum(res,res,op2,n);
	}
	
	addbignum(res,res,op2,n);
	res[n]=0;
			
	return 0;
}
/****************************************************************
* int32_t compare(uint64_t op1[], uint64_t op2[],uint32_t n)
* returns 1 if op1>op2
* 		 -1 if op1<op2
* 		  0 if op1=op2
*****************************************************************/
int32_t compare(uint64_t op1[], uint64_t op2[],uint32_t n)
{
	for( ; n>0; n--)
	{
		if(op1[n-1]>op2[n-1])
		{
			return 1;
		}
		else if(op1[n-1]<op2[n-1])
		{
			return -1;
		}
	}
			
	return 0;
}

/****************************************************************
 * bool subbignum(uint64_t res[], uint64_t op1[], uint64_t op2[],uint32_t n)
 * subtracts op2 from op1
 * returns 0 if op1>=op2
 * 		   1 if op1<op2
 * result is not valid if return value is 1 (or is in 2's compliment :P)
 * **************************************************************/
bool subbignum(uint64_t res[], uint64_t op1[], uint64_t op2[],uint32_t n)
{
	bool carry=0;
	uint32_t i;
	for(i=0;i<n;i++)
	{
		if(carry)
		{
			if(op1[i]!=0)
				carry=0;
			op1[i]--;		
		}
		if(op1[i]<op2[i])
			carry = 1;
			
		res[i]= op1[i] - op2[i];
	}	
	return carry;
}
bool slnbignum(uint64_t res[], uint64_t op[],uint32_t len, uint32_t n)//shift left by n
{
    uint32_t i,x,y;
    uint64_t j,k,carry = 0;
    x = n / 64;
    y = n % 64;

    for(i=len; i - x >0; i--)
    {
        res[i-1] = op[i - 1 - x];
    }
    for(;i>0;i--)
    {
        res[i-1] = 0;
    }
    for(i=0;i<len;i++)
    {
        j = res[i];
        k=0;
        for(x=0;x<y;x++)
        {
            if(j & 0x8000000000000000)
            {
                k = (k<<1) | 1;
            }
            else
            {
                k = (k<<1);
            }
            j = j <<1;
        }
        res[i] = j | carry;
        carry = k;
    }
    return 1;
}
bool srnbignum(uint64_t res[], uint64_t op[],uint32_t len, uint32_t n)//shift right by n
{
    uint32_t i,x,y;
    uint64_t j,k,carry = 0;
    x = n / 64;
    y = n % 64;

    for(i=0; i + x < len; i++)
    {
        res[i] = op[i + x];
    }
    for(;i<len;i++)
    {
        res[i] = 0;
    }
    for(i=len;i>0;i--)
    {
        j = res[i-1];
        k=0;
        for(x=0;x<y;x++)
        {
            if(j & 0x0000000000000001)
            {
                k = (k>>1) | 0x8000000000000000;
            }
            else
            {
                k = (k>>1);
            }
            j = j >>1;
        }
        res[i-1] = j | carry;
        carry = k;
    }
    return 1;

}
/****************************************************************
 * uint32_t bit_length(uint64_t op[],uint32_t n)
 * returns position of MSB present
 *
 *
 ****************************************************************/
uint32_t bit_length(uint64_t op[],uint32_t n)
{
    uint32_t len=0;
    uint32_t i;
    uint64_t unit = 1;
    for( ;n>0;n--)
    {
        if(op[n-1]==0)
            continue;
        for(i=64;i>0;i--)
        {
            if(op[n-1] & (unit<<(i-1)))
            {
                len = (64*(n-1)) + i;
                break;
            }

        }
        if(len)
            break;
    }
    return len;
}
/*
bool setbitbignum(uint64_t op[],uint32_t n,uint32_t bit)//sets n'th bit
{
    uint32_t q,r;
    uint64_t unit=0x1;
    q = bit / 64;
    r = bit % 64;
    if(q>=n)
        return 0;
    op[q] |= (unit<<r);
    return 1;
}
*/
