//g++ -std=c++11  robustmain.cpp -pthread

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <random>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include <sched.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "seeding_msg.h"

/*
[1] sendtoaddress
[2] getbalance(*)
[3] generate
[4] getnewaddress*
[5] getmempoolinfo*
[6] getchaintips*
[7] settxfee
[8] getbestchaintps
*/
char *CMD[8];
int expect = 0;

#include <thread>
int *balance = NULL;//(*)

int spam = 0;
int cnt = 0;

void setshellcmdoutput(FILE **fd, int op, ...){
	char command[512];
	va_list args;
	va_start(args, CMD[op]);
	vsnprintf(command, 512, CMD[op], args);
	va_end(args);

	//printf("= %s =\n", command);

	if(fd != NULL)
		//printf("=%s=\n", command);
		*fd = popen(command, "r");
	else
		system(command);
	
	return;
}
/////////////////////////////////////////////////////////////////////////


void node_monitor(int num_node){
	FILE *fp, *fq;
        char tmp[10], tmq[10];
        int txnum = -1, tip = -1;
        struct timespec start, check;
        clock_gettime(CLOCK_REALTIME, &start);
        fprintf(stderr, "time1 [%lld.%.9ld]\n", (long long)start.tv_sec, start.tv_nsec);


//      setshellcmdoutput(NULL, 0, -1, address[]);

	int i;
        while( /*cnt < spam*/ 1 ){
                setshellcmdoutput(&fp, 4, -1);
                fgets(tmp, 10, fp); tmp[strlen(tmp)-1] = '\0';
                fflush(fp); pclose(fp);
                txnum = atoi(tmp);

		setshellcmdoutput(&fq, 5, -1);
		fgets(tmq, 10, fq); tmq[strlen(tmq)-1] = '\0';
		fflush(fq); pclose(fq);
		tip = atoi(tmq);
		
                fprintf(stderr, "tx in mempool: %d | tip: %d\n", txnum, tip);
		usleep(1000);
/*
		for(i = 0; i < num_node; i++){
			//balance
		}
*/

        }
        clock_gettime(CLOCK_REALTIME, &check);
        fprintf(stderr, "time2 [%lld.%.9ld]\n", (long long)check.tv_sec, check.tv_nsec);
}
//////////////////////////////////////////////////////////////

char dnsholder[256];
int port;
int rpcport;
char selfaddress[256];
int findaddress(int sockfd, MSG *buf_send, void **buf_recv)
{
    send(sockfd, buf_send, sizeof(MSG), 0);
    int res = recv(sockfd, *buf_recv, sizeof(MSG), 0);
    close(sockfd);
    
    return res;
}
struct in_addr getdnsholderip(char *dnsholder){
    
    struct addrinfo aiHint;
    memset(&aiHint, 0, sizeof(struct addrinfo));
    aiHint.ai_socktype = SOCK_STREAM;
    aiHint.ai_protocol = IPPROTO_TCP;
    aiHint.ai_family = AF_UNSPEC;
    aiHint.ai_flags = AI_ADDRCONFIG;

    struct addrinfo *aiRes = NULL;
    if( getaddrinfo(dnsholder, NULL, &aiHint, &aiRes) ){
        printf("getaddrinfo failed\n");
        exit(0);
    }//if dns server is IPv4/6 address, this sends out a packet for DNS query AAAA over IPv4/6

    struct addrinfo *aiTrav = aiRes;
    struct sockaddr *res = NULL;
    while( aiTrav != NULL ){
        res = aiTrav->ai_addr;
        aiTrav = aiTrav->ai_next;
    } freeaddrinfo(aiRes);
    
    if(!res){
        printf("getaddrinfo not failed but get nothing\n");
        exit(0);
    }

    return ((struct sockaddr_in *)res)->sin_addr;
}

double checkbalance()
{
    FILE *fbl; char temp[32];
    setshellcmdoutput(&fbl, 1, rpcport);
    fgets(temp, 32, fbl); temp[strlen(temp)-1] = '\0'; fflush(fbl);pclose(fbl);
    return atof(temp);
}

void matching(unsigned long long int randseed, unsigned int order)
{
    int sockfd;
    struct sockaddr_in dest;
    MSG *buf_send = new MSG();
    void *buf_recv = malloc(1 * sizeof(MSG));
    buf_send->op = REQUEST_BTCADDR;
    //sockfd = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&dest, sizeof(dest));
    dest.sin_family = PF_INET;
    dest.sin_port = htons(5566);
    /******************/
    //dest.sin_addr.s_addr = inet_addr(dnsseed);
    dest.sin_addr = getdnsholderip(dnsholder);
    /******************/
    //connect(sockfd, (struct sockaddr*)&dest, sizeof(dest));

    std::mt19937 mt(randseed);
    std::exponential_distribution<double> exp(0.1);

    do{
        double r = exp(mt);
        long interval = (long)(r * (double)order);
        /* wait */
        long j = interval; //printf("%ld\n", j);
        while((j--) > 0) ;

        /* request address to send */
        sockfd = socket(PF_INET, SOCK_STREAM, 0);
        connect(sockfd, (struct sockaddr*)&dest, sizeof(dest));
        findaddress(sockfd, buf_send, &buf_recv);
        char *addr = ((MSG *)buf_recv)->message; printf("btcaddr %s\n", addr);
        
        /* TODO fill in 4th parameter of sendMoney below according to balance */
        double balance = checkbalance();
        /* send the money */
        //TODO setshellcmdoutput(NULL, 0, rpcport, addr, 0.01);//paytxfee when running core?

    } while(1);
}

void startnode(unsigned long long int randseed, unsigned int order, char *datadir)
{

    int sockfd;
    struct sockaddr_in dest;
    MSG *buf_send = new MSG();
    buf_send->op = REGISTER_IP;
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&dest, sizeof(dest));
    dest.sin_family = PF_INET;
    dest.sin_port = htons(5566);
    /******************/
    //dest.sin_addr.s_addr = inet_addr(dnsseed);
    dest.sin_addr = getdnsholderip(dnsholder); printf("%s\n", inet_ntoa(dest.sin_addr));
    /******************/

    /* run core */
    char command[280];
    snprintf(command, 280, "./startnoderob.sh %s %d %d", datadir, port, rpcport);//mintxfee, minrelaytxfee
    system(command);
    sleep(3);
    /* get BTC address */
    FILE *fd;
    setshellcmdoutput(&fd, 3, rpcport, 1);
    fgets(selfaddress, 256, fd); selfaddress[strlen(selfaddress)-1] = '\0';//[pop_back]
    fflush(fd); pclose(fd); printf("%s\n", selfaddress);
    /* report BTC address */
    buf_send->op = REGISTER_BTCADDR;
    strcpy(buf_send->message, selfaddress); printf("%s\n", buf_send->message);
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    connect(sockfd, (struct sockaddr*)&dest, sizeof(dest));
    send(sockfd, buf_send, sizeof(MSG), 0);
    close(sockfd);

    delete buf_send;
    
    /* fork miner process */
    /* hashMerkleRoot(txns choice),hashPrevBlock(topic choice) */
    int pid = fork();
    if(pid == 0){
        /* [when] balance, run */
        /* TODO mine the block */
        
        exit(0);
    }

    /* starting matching */
    matching(randseed, order);

}/* eventually... no thread are used */

int main(int argc, char *argv[]){
    int i = 0;
    for(i = 0; i < 8; i++)
        CMD[i] = new char[32];
    strcpy(CMD[0], "./sendmoney.sh %d %s %.7f");
    strcpy(CMD[1], "./checkbalance.sh %d");//atof
    strcpy(CMD[2], "./mineblock.sh %d %d");//
    strcpy(CMD[3], "./setaddress.sh %d %d");//(ascii string)
    strcpy(CMD[4], "./txInmempool.sh %d");//atoi
    strcpy(CMD[5], "./getchaintip.sh %d");//atoi (longest height -> #branch)
    strcpy(CMD[6], "./settxfee.sh %d %d %.7f");
    istrcpy(CMD[7], "./getbestchaintps.sh %d %d");//science notation ascii string
    //in newer bitcoin, gettxoutsetinfo
    //in newer bitcoin, getblockchaininfo


    //std::thread monitor( node_monitor , num_node);//master node aside from the num_node nodes
    //setshellcmdoutput(NULL, 2, i, 1);


    FILE *f = fopen("setup", "r");
    int res;
    char seedname[256];
    unsigned long long int randseed;
    unsigned int order;
    double txfee;
    char datadir[256];
    int fPNR, fPAMDB, nPTTS, nPTS;
    int seednum;
    res = fscanf(f, "%s", dnsholder);
    res = fscanf(f, "%s", seedname);
    res = fscanf(f, "%llu", &randseed);
    res = fscanf(f, "%u", &order);
    res = fscanf(f, "%lf", &txfee);
    res = fscanf(f, "%s", datadir);
    res = fscanf(f, "%d", &fPNR);
    res = fscanf(f, "%d", &fPAMDB);
    res = fscanf(f, "%d", &nPTTS);
    res = fscanf(f, "%d", &nPTS);
    res = fscanf(f, "%d", &seednum);
    res = fscanf(f, "%d", &port);
    res = fscanf(f, "%d", &rpcport);
    fclose(f);
    
    /* report ip */
    int sockfd;
    struct sockaddr_in dest;
    MSG *buf_send = new MSG();
    void *buf_recv = malloc(1 * sizeof(MSG));
    buf_send->op = REGISTER_IP;
    sprintf(buf_send->message, "%d", port);
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&dest, sizeof(dest));
    dest.sin_family = PF_INET;
    dest.sin_port = htons(5566);
    /******************/
    //dest.sin_addr.s_addr = inet_addr(dnsseed);
    dest.sin_addr = getdnsholderip(dnsholder); printf("%s\n", inet_ntoa(dest.sin_addr));
    /******************/
    connect(sockfd, (struct sockaddr*)&dest, sizeof(dest));
    send(sockfd, buf_send, sizeof(MSG), 0);
    close(sockfd);
    sleep(3);
    /* request ip */
    buf_send->op = REQUEST_IP;
    strcpy(buf_send->message, "");
 

    char confpath[270];
    strcpy(confpath, datadir);
    strcpy(&confpath[strlen(confpath)], "/bitcoin.conf");
    printf("%s\n", confpath);
    f = fopen(confpath, "w"); 
    fprintf(f, "rpcuser=test\n");
    fprintf(f, "rpcpassword=test\n");
    fprintf(f, "dnsholder=%s\n", dnsholder);
    fprintf(f, "seedname=%s\n", seedname);
    fprintf(f, "paytxfee=%.7f\n", txfee);
    //fprintf(f, "datadir=%s\n", datadir);
    fprintf(f, "fPNR=%d\n", fPNR);
    fprintf(f, "fPAMDB=%d\n", fPAMDB);
    fprintf(f, "nPTTS=%d\n", nPTTS);
    fprintf(f, "nPTS=%d\n", nPTS);
    int ipcnt = 0;
    do{ fprintf(f, "\n");
        sleep(1);
        int res;

        sockfd = socket(PF_INET, SOCK_STREAM, 0);
        connect(sockfd, (struct sockaddr*)&dest, sizeof(dest));
        if((res = findaddress(sockfd, buf_send, &buf_recv)) > 0){
            fprintf(f, "seednode=%s", ((MSG *)buf_recv)->message); printf("res %d cnt %d %s\n", res, ipcnt, ((MSG *)buf_recv)->message);
            ++ipcnt;
        }
    }while( ipcnt < seednum );
    fclose(f);

    startnode(randseed, order, datadir);

	
    return 0;	
}
