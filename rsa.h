/*
 * rsa.h
 *
 *  Created on: 09-May-2017
 *      Author: navin
 */

#ifndef RSA_H_
#define RSA_H_

bool addbignum(uint64_t res[], uint64_t op1[], uint64_t op2[],uint32_t n);
bool subbignum(uint64_t res[], uint64_t op1[], uint64_t op2[],uint32_t n);
bool modbignum(uint64_t res[],uint64_t op1[], uint64_t op2[],uint32_t n);
bool modnum(uint64_t res[],uint64_t op1[], uint64_t op2[],uint32_t n);
bool modmult1024(uint64_t res[], uint64_t op1[], uint64_t op2[],uint64_t mod[]);
bool rsa1024(uint64_t res[], uint64_t data[], uint64_t expo[],uint64_t key[]);
bool multbignum(uint64_t res[], uint64_t op1[], uint32_t op2 ,uint32_t n);
uint32_t bit_length(uint64_t op[],uint32_t n);
int32_t compare(uint64_t op1[], uint64_t op2[],uint32_t n);
bool slnbignum(uint64_t res[], uint64_t op[],uint32_t len, uint32_t n);//shift left by n
bool srnbignum(uint64_t res[], uint64_t op[],uint32_t len, uint32_t n);



#endif /* RSA_H_ */
