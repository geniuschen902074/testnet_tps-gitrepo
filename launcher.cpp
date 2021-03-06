#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]){

    if(argc != 15){
        printf("parameter not enough\n");
        return 0;
    }

    char *dnsholder = argv[1];
    char *seedname = argv[2];

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    unsigned long long int randseed = ts.tv_nsec;

    unsigned int order = atoi(argv[3]);

    double txfee = atof(argv[4]);
    char *datadir = argv[5];
    int fPNR = atoi(argv[6]);
    int fPAMDB = atoi(argv[7]);
    int nPTTS = atoi(argv[8]);
    int nPTS = atoi(argv[9]);
    char *BITS = argv[10];

    int seednum = atoi(argv[11]);
    char *ip = argv[12];
    int port = atoi(argv[13]);
    int rpcport = atoi(argv[14]);

    //FILE *fp = fopen("setup", "w");
    //fprintf(fp, "%s\n%s\n%llu\n%u\n%.7f\n%s\n%d\n%d\n%d\n%d\n%d", dnsholder, seedname, randseed, order, txfee, datadir, fPNR, fPAMDB, nPTTS, nPTS, seednum);
    //fclose(fp);
    printf("%s\n%s\n%llu\n%u\n%.7f\n%s\n%d\n%d\n%d\n%d\n%d\n%s\n%d\n%d\n%s", dnsholder, seedname, randseed, order, txfee, datadir, fPNR, fPAMDB, nPTTS, nPTS, seednum, ip, port, rpcport, BITS);

    return 0;
}
