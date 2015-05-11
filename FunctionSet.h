void CreateBigPrime(mpz_t mpzPrime, int bits)
{
	int i, last_rand = 0;
	char *char_rand = new char[bits + 1];
	char_rand[0] = '1';
	char_rand[bits] = '\0';
	mpz_init(mpzPrime);

	do
	{
		for (i = 1; i<bits; i++)
		{
			last_rand = rand();
			char_rand[i] = '0' + (0x01 & last_rand);

			mpz_set_str(mpzPrime, char_rand, 2);
			mpz_nextprime(mpzPrime, mpzPrime);
		}
	} while (0 == mpz_probab_prime_p(mpzPrime, 20));
}
