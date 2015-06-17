#include <pbc.h>
#include <time.h>
#include <iostream>
#include <pbc_test.h>
#include <vector>
#include <string>
#include "CTimer.h"

//#include <chrono>

using namespace std;

int main(int argc, char **argv)
{
	CTimer ct;
	int n = 50;
	pairing_t pairing;
	pbc_demo_pairing_init(pairing, argc, argv);

	/* Setup */
	cout << "Setup:" << endl;
	element_ptr L[50];
	element_t s, P, P1, P2, upie, ui, sP2;
	element_init_Zr(s, pairing);
	element_init_G1(P, pairing);
	element_init_G1(P1, pairing);
	element_init_G1(P2, pairing);
	element_init_G1(upie, pairing);
	element_init_G1(ui, pairing);
	element_init_G1(sP2, pairing);

	ct.Start();
	element_random(s);
	element_random(P);
	element_mul_zn(P1, P, s);
	element_random(P2);
	element_random(upie);
	for (int i = 0; i<n; i++)
	{
		L[i] = new element_s[1];
		element_init_G1(L[i], pairing);
		element_random(L[i]);
	}
	element_mul_zn(sP2, P2, s);
	cout << ct.End()*1000 << "ms" << endl;
	element_printf("P=%B\n", P);
	element_printf("P1=%B\n", P1);
	element_printf("upie=%B\n", upie);
	element_printf("sP2=%B\n", sP2);
	
	/* Extract */
	cout << "Extract:" << endl;
	element_t tmp1, r, S1, S2;
	element_init_G1(tmp1, pairing);
	element_init_G1(S1, pairing);
	element_init_G1(S2, pairing);
	element_init_Zr(r, pairing);
	element_set0(tmp1);
	cout << "input a binary string of identity which is less than 50 bits:" << endl;
	string id;
	cin >> id;
	ct.Start();
	for (size_t i = 0; i < id.length(); i++)
	{
		if ('1' == id[i])
		{
			element_add(tmp1, tmp1, L[i]);
		}
	}
	element_add(tmp1, tmp1, upie);
	element_random(r);
	element_mul_zn(tmp1, tmp1, r);
	element_add(S1, tmp1, sP2);
	element_mul_zn(S2, P, r);
	cout << ct.End() * 1000 << "ms" << endl;
	element_printf("S1=%B\n", S1);
	element_printf("S2=%B\n", S2);


	/* Encrypt: */
	cout << "Encrypt:" << endl;
	element_t m,t,C1,C2,C3;
	element_init_GT(m, pairing);
	element_init_GT(C1, pairing);
	element_init_G1(C2, pairing);
	element_init_G2(C3, pairing);
	element_init_Zr(t, pairing);
	element_random(m);
	element_printf("m=%B\n", m);
	ct.Start();
	element_random(t);
	pairing_apply(C1, P1, P2, pairing);
	element_pow_zn(C1, C1, t);
	element_mul(C1, C1, m);
	element_mul_zn(C2, P, t);
	element_set0(tmp1);
	for (size_t i = 0; i < id.length(); i++)
	{
		if ('1' == id[i])
		{
			element_add(tmp1, tmp1, L[i]);
		}
	}
	element_add(tmp1, tmp1, upie);
	element_mul_zn(C3, tmp1, t);
	cout << ct.End() * 1000 << "ms" << endl;
	element_printf("C1=%B\n", C1);
	element_printf("C2=%B\n", C2);
	element_printf("C3=%B\n", C3);

	/* Decryption */
	cout << "Decrypte:" << endl;
	ct.Start();
	element_t tmp2, tmp3;
	element_init_GT(tmp2, pairing);
	element_init_GT(tmp3, pairing);
	pairing_apply(tmp2, S2, C3, pairing);
	pairing_apply(tmp3, S1, C2, pairing);
	element_invert(tmp3, tmp3);
	element_mul(tmp2, tmp2, tmp3);
	element_mul(m, C1, tmp2);
	cout << ct.End() * 1000 << "ms" << endl;
	element_printf("m=%B\n", m);

	system("pause");
	return 0;
}
