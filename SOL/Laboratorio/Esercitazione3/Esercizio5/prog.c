extern int printf(const char *format, ...);
extern int getopt(int argc, char * const argv[], const char *optstring);
extern char* optarg;
extern char optopt;

int main(int argc, char *argv[])
{
	int opt;
	while ((opt = getopt(argc,argv, ":n:m:h")) != -1)
	{
		switch(opt)
		{
			case 'n':
			case 'm':printf("l'argomento di '-%c' e' '%s'\n",opt,optarg); break;
			case 'h':printf("'-h' non ha argomenti\n"); break;
			case ':':printf("'-%c' richiede un argomento\n", optopt); break;
			case '?':printf("'-%c' non e' riconosciuta\n", optopt);
			default:;
		}
	}
  return 0;
}