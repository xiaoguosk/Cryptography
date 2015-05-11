#include <stdio.h>
#include <iostream>
#include <ctime>
#include "gmp.h"
#include "FunctionSet.h"

using namespace std;

int main()
{
	cout << "密钥生成" << endl;
	//生成两个大素数
	mpz_t p,pt, q,qt;
	mpz_init2(p, 512);
	mpz_init2(pt, 512);
	mpz_init2(q, 512);
	mpz_init2(qt, 512);
	srand((unsigned)time(NULL));
	CreateBigPrime(p, 512);
	CreateBigPrime(q, 512);
	mpz_sub_ui(pt, p, 1);
	mpz_sub_ui(qt, q, 1);
	gmp_printf("p=%Zd\n", p); cout << endl;
	gmp_printf("q=%Zd\n", q); cout << endl;
	//计算两个大素数的积
	mpz_t N;
	mpz_t Nt;
	mpz_init(N);
	mpz_init(Nt);
	mpz_mul(N, p, q);
	mpz_mul(Nt, pt, qt);
	gmp_printf("N=%Zd\n", N); cout << endl;
	//生成e
	mpz_t e,r;
	mpz_init2(e, 512);
	mpz_init(r);	
	do
	{
		CreateBigPrime(e, 512);
		mpz_gcd(r, Nt, e);
	} while (mpz_cmpabs_ui(r,1) != 0);	
	gmp_printf("e=%Zd\n", e); cout << endl;
	//生成d
	mpz_t d;
	mpz_init(d);
	mpz_invert(d, e, Nt);
	gmp_printf("d=%Zd\n", d); cout << endl;

	//生成明文
	cout << "产生随机明文" << endl;
	gmp_randstate_t grt;
	__gmp_randinit_default(grt);
	mpz_t m,c;
	mpz_init(m);
	mpz_init(c);
	mpz_urandomm(m, grt, N);
	gmp_printf("m=%Zd\n", m); cout << endl;
	cout << "加密" << endl;
	//enc;
	mpz_powm(c, m, e, N);
	gmp_printf("c=%Zd\n", c); cout << endl;
	//dec
	cout << "解密" << endl;
	mpz_powm(m, c, d, N);
	gmp_printf("m=%Zd\n", m); cout << endl;

	getchar();
	return 0;
}
