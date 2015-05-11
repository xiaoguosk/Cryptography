#include <stdio.h>
#include <iostream>
#include <ctime>
#include "gmp.h"
#include "FunctionSet.h"

using namespace std;

#define SECURITY_PARAMETER 1024

int main()
{
	/* 生成公共参数	*/
	cout << "密钥生成" << endl;
	mpz_t p,g;
	gmp_randstate_t grt;
	__gmp_randinit_default(grt);
	mpz_init2(p, SECURITY_PARAMETER);
	mpz_init(g);
	CreateBigPrime(p, SECURITY_PARAMETER);
	do
	{
		mpz_urandomm(g, grt, p);
	} while (!(mpz_cmp_ui(g, 1) > 0));	
	gmp_printf("p=%Zd\n", p); cout << endl;
	gmp_printf("g=%Zd\n", g); cout << endl;

	/*生成密钥*/
	mpz_t sk, pk;
	mpz_init(sk);
	mpz_init(pk);
	do
	{
		mpz_urandomm(sk, grt, p);
	} while (!(mpz_cmp_ui(sk, 1) > 0));
	mpz_powm(pk, g, sk, p);
	gmp_printf("sk=%Zd\n", sk); cout << endl;
	gmp_printf("pk=%Zd\n", pk); cout << endl;

	/* 加密 */
	cout << "加密" << endl;
	int m = 1024;
	cout << "m=" << m << endl;
	mpz_t c1, c2, r;
	mpz_init(c1);
	mpz_init(c2);
	mpz_init(r);
	do
	{
		mpz_urandomm(r, grt, p);
	} while (!(mpz_cmp_ui(r, 1) > 0));
	mpz_powm(c1, g, r, p);
	mpz_powm(c2, pk, r, p);
	mpz_mul_ui(c2, c2, m);
	mpz_mod(c2, c2, p);
	gmp_printf("c1=%Zd\n", c1);
	gmp_printf("c2=%Zd\n", c2); cout << endl;

	//解密
	cout << "解密" << endl << endl;
	mpz_t plain;
	mpz_init(plain);
	mpz_powm(plain, c1, sk, p);
	mpz_invert(plain, plain, p);
	mpz_mul(plain, plain, c2);
	mpz_mod(plain, plain, p);
	gmp_printf("m=%Zd\n", plain); cout << endl;



	system("pause");
	return 0;
}
