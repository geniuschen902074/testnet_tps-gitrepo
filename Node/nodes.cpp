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

/*
[1] sendtoaddress
[2] getbalance(*)
[3] generate
[4] getnewaddress*
[5] getmempoolinfo*
[6] getchaintips*
[7] settxfee
*/
char *CMD[7];
int expect = 0;

#include <thread>
long *randtable = NULL;
char **address = NULL;
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

void node_supply(int num_node){
	int target = num_node * 1000;
	double tmp = 0;
	FILE *fp;
	char temp[32];

	while(tmp < target){
		setshellcmdoutput(NULL, 2, -1, 1);

		setshellcmdoutput(&fp, 1, -1);
		fgets(temp, 32, fp); temp[strlen(temp)-1] = '\0';
		fflush(fp); pclose(fp);
		tmp = atof(temp); printf("supplying: %f\n", tmp);
	} setshellcmdoutput(NULL, 2, -1, 1);

	

	int i;
	for(i = 0; i < num_node; i++)
		setshellcmdoutput(NULL, 0, -1, address[i], 1000.000000);
	setshellcmdoutput(NULL, 2, -1, 1);
	for(i = 0; i < num_node; i++){
		setshellcmdoutput(&fp, 1, i);
                fgets(temp, 32, fp); temp[strlen(temp)-1] = '\0';
		fflush(fp); pclose(fp);
                tmp = atof(temp); printf("%d: %f\n", i, tmp);
	}

}

void node_action(int pos, int num_node, double lumbda,int order){
	/* run script to send money[1, parameter: pos, address[(pos+1)%num_node], ammount=0.0000055(ammount can't be 0 & tx fee is inevitable)] */
	/* TODO when to stop? */
	struct timespec tv;
	FILE *fbl; char temp[32]; double tmp; /*int budg;*/

	int scale = ((int)(lumbda*(1000000/order)))/* /10 */;
	double bound = (double)(scale/100+1) + (scale*0.1);//maxtxfee (BTC)

	int index = pos-1;
	int sendee = (pos+1) % num_node;
	int i;
	//int cnt = 0;
	while( 1 ){
		setshellcmdoutput(&fbl, 1, pos);
                fgets(temp, 32, fbl); temp[strlen(temp)-1] = '\0'; fflush(fbl);pclose(fbl);
                tmp = atof(temp); //fprintf(stderr, "node %d: %d tx, %f bal\n", pos, cnt, tmp);
		/* budg = (int)tmp; */
		if(/*budg*/ tmp <= bound) continue;

		for(/*i = budg*(scale/100); i > scale;*/i = scale; i > 0; --i){
			
			index = (index+1) % num_node;
			usleep( randtable[index] );

			//setshellcmdoutput(NULL, 6, pos, 0.00); //! zero means floating feerate. On the other hand, use -paytxfee option for running core for present case
			setshellcmdoutput(NULL, 0, pos, address[sendee], 0.01); //fprintf(stderr, "node %d %f %d\n" , pos, tmp, i);fflush(stderr);
			
		}
		fprintf(stderr, "[%d] %d tx done.\n", pos, scale);

//		index = (index+1) % num_node;

		/*
		tv.tv_sec = randtable[index] / 1000000000;
		tv.tv_nsec = randtable[index] % 1000000000;
		if(nanosleep( &tv, NULL ) < 0) printf("why can't you sleep?\n");
		*/
//		usleep( randtable[index] );

//		setshellcmdoutput(NULL, 0, pos, address[sendee], 1.0);
		//++cnt; fprintf(stderr, "node %d: %d tx\n", pos, cnt);

	}

}
void node_miner(int pos){
	/* (constantly check balance[2, parameter: pos] of,) mine block[3, parameter: num=1] (to supply[1], and maintain balance of each node) */
	/* probing #tx in mempool[5] */
/*
	FILE *fp;
	char tmp[10];
	int txnum = -1;
*/

	
//	setshellcmdoutput(NULL, 0, -1, address[]);


	//int b = 0;
	while( 1 /*b < spam*/){
/*
		setshellcmdoutput(&fp, 4, pos);
                fgets(tmp, 10, fp); tmp[strlen(tmp)-1] = '\0';
		fflush(fp); pclose(fp);
                txnum = atoi(tmp);
*/

//		if(txnum >= 60){
			setshellcmdoutput(NULL, 2, pos, 1);
			//++b; fprintf(stderr, "node %d: %d blk\n", pos, b);
//		}
	}

}

void node_monitor(int num_node){
	FILE *fp, *fq;
        char tmp[10], tmq[10];
        int txnum = -1, tip = -1;
        struct timespec start, check;
        clock_gettime(CLOCK_REALTIME, &start);
        fprintf(stderr, "time1 [%lld.%.9ld]\n", (long long)start.tv_sec, start.tv_nsec);


//      setshellcmdoutput(NULL, 0, -1, address[]);

	int i;
        while( cnt < spam /*1*/){
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
int main(int argc, char *argv[]){
	int i = 0;
	if(argc != 6)
		return 0;
	for(i = 0; i < 7; i++)
		CMD[i] = new char[32];
	strcpy(CMD[0], "./sendmoney.sh %d %s %.7f");
	strcpy(CMD[1], "./checkbalance.sh %d");//pos
	strcpy(CMD[2], "./mineblock.sh %d %d");//pos
	strcpy(CMD[3], "./setaddress.sh %d");//num_node
	strcpy(CMD[4], "./txInmempool.sh %d");//pos
	strcpy(CMD[5], "./getchaintip.sh %d");
	strcpy(CMD[6], "./settxfee.sh %d %.7f");
	spam = atoi(argv[4]); // in terms of CLOCK_REALTIME or #BLOCK
	expect = atoi(argv[5]); /* expected [tx] digesting capacity from any node's respect view */
	
	/* support 1M tx-p-S test */
	/* within one Second, for example */
	int num_node = atoi(argv[1]);
	double lumbda = atof(argv[2]);  /* the parameter to boost to the limit */
	int order = atoi(argv[3]); //with respect to usleep(). 1000000/order time instant(t) per one Second
	/* totally (lumbda*t)*num_node tx generated("happen") per Second */

	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	std::mt19937 mt(ts.tv_nsec);
	std::exponential_distribution<double> exp(lumbda);
	//while(1) printf("%f\n", exp(mt));
	randtable = new long[num_node] /*malloc(sizeof(long) * num_node)*/;
	for(i = 0; i < num_node; i++){
		double r = exp(mt);
		fprintf(stderr, "tv #%d %f\n", i, r);
		randtable[i] = (long)(r * (double)order);
	}


	char command[64];
	snprintf(command, 64, "./startnoderob.sh %d 0.001", num_node);//mintxfee, minrelaytxfee
	system(command);
	sleep(3);

	address = new char*[num_node];
	/* run script to get new address[4, parameter: num_node], maintaining address */
	//snprintf(command, 64, "./getaddress.sh %d", num_node);
	//FILE *fd = popen(command, "r");
	FILE *fd;
	setshellcmdoutput(&fd, 3, num_node);
	char addr[256];
	for(i = 0; i < num_node; i++){
		fgets(addr, 256, fd); addr[strlen(addr)-1] = '\0';//[pop_back]
		address[i] = new char[256];
		strcpy(address[i], addr); //printf("%s\n", address[i]);
	} fflush(fd); pclose(fd);

	balance = new int[num_node];
	for(i = 0; i < num_node; i++)
		balance[i] = 0;


	/* prepared each node with budget */
//	std::thread feed( node_supply, num_node );
//	feed.join();
//	snprintf(command, 64, "./startmining.sh %d", num_node);
//	system(command);
	//return 0;
	

	struct sched_param param;
	param.sched_priority = sched_get_priority_max(SCHED_FIFO);//same as SCHED_RR
	sched_setscheduler(getpid(), SCHED_FIFO, &param);

	std::thread monitor( node_monitor , num_node);//master node aside from the num_node nodes
	printf("parent pid %d\n", getpid());
	int *childtable = new int[num_node];
	for(i = 0; i < num_node; i++){

		int pid = fork();	
		sched_setscheduler(pid, SCHED_FIFO, &param);
		if( pid == 0 ){
			
			FILE *fbl; char temp[32]; double tmp;
			/*
			FILE *ftx; char temp[32]; int tmp;
			*/

			//mine blocks until killed by parent
			/*setshellcmdoutput(NULL, 2, i, 100+(int)(lumbda*spam*0.11/50.0)+1 );*/
		//	int period = order;
			while(1){
				/*
				setshellcmdoutput(NULL, 2, i, // /expect/ tx to mine - TODO FORMED TO BLOCK // );
				usleep(  (int)(((double)(expect*order))/lumbda) ); // TODO WHAT IS OUR EXPECTATION - note our ceiling  //
				*/
				//It is said that we can expect not too much, but actually our mining ability usually doesn't allow us to expect that much, either.

			//	int c;
			//	for(c = 0; c < spam; c++){ //c is for child himself
				int I;
				for(I = 0; I < num_node; I++){
					unsigned long long int j;
					for(j = 0; j < randtable[(i+I)%num_node]; j++)  ;
		//			for(j = 1; j <= (unsigned long long int)period; j++){ //ooxxsleep() doesn't fit well
						/* according to balance */
						/*TODO too many IO causing this process to be blocked. CAN WE AVOID CHECKING?
						* maybe checking can be moved out to a thread within main process, or another sibing process, FINALLY UTILIZING THE balance VARIABLE.
						* but balance should be shared BETWEEN PROCESSES
						*/
						setshellcmdoutput(&fbl, 1, i);
						fgets(temp, 32, fbl); temp[strlen(temp)-1] = '\0'; fflush(fbl);pclose(fbl);
						tmp = atof(temp);
						if( tmp <= ((0.01+0.001)*(double)num_node) /*(lumbda*spam*(0.01+0.1))*/ )
							setshellcmdoutput(NULL, 2, i, 1);
						/*
						* according to #tx@pool
						setshellcmdoutput(&ftx, 4, i);
                                                fgets(temp, 32, ftx); temp[strlen(temp)-1] = '\0'; fflush(ftx);pclose(ftx);
                                                tmp = atoi(temp);
                                                if( tmp >= expect  )
                                                        setshellcmdoutput(NULL, 2, i, 1);
						else
							sched_yield();
						*/
						
		//			}
				}
				sched_yield();

			//	return 0;
			} //end while

		} //child process
		else if( pid > 0 ){ //the parent
			printf("%dth child pid %d\n", i, pid);
			childtable[i] = pid;
			sched_yield();
		}

	}
//	std::thread *minertable = new std::thread[num_node];
//	for(i = 0; i < num_node; i++)
//		minertable[i] = std::thread( node_miner, i);
	/* Let me and my miner children CNT like this */
	int *index = new int[num_node];
	long *pend = new long[num_node];
	unsigned long long int j;
	FILE *fbl; char bltemp[32]; double bltmp;
	for(i = 0; i < num_node; i++){ //initialization
		index[i] = i;
		pend[i] = randtable[index[i]];
stuck:
		setshellcmdoutput(&fbl, 1, i);
		fgets(bltemp, 32, fbl); bltemp[strlen(bltemp)-1] = '\0'; fflush(fbl);pclose(fbl);
		bltmp = atof(bltemp);
		if( bltmp <= ((0.01+0.001)*(double)num_node)) goto stuck;
	}
        int period = order;
        for(; cnt < spam; cnt++){
                for(j = 0; j <= (unsigned long long int)period; j++){

			for(i = 0; i < num_node; i++)
				if((pend[i]--) == 0){
					setshellcmdoutput(NULL, 0, i, address[(i+1)%num_node], 0.01);
					index[i] = ( index[i]+1 ) % num_node;
					pend[i] = randtable[index[i]];
				}

		}

		sched_yield();
        } //note that my children started earlier
//	std::thread *threadtable = new std::thread[num_node];
//	for(i = 0; i < num_node; i++)
//		threadtable[i] = std::thread( node_action, i, num_node, lumbda,order); //these threads starts already


	/* join and release threads and related spaces in reverse order */
//	for(i = 0; i < num_node; i++)
//		threadtable[i].join();
//	for(i = 0; i < num_node; i++)
//		minertable[i].join();
	/* waitpid() is not enough cuz they mine to infinity. kill(childtable[i], SIGKILL) them at some point 
           since children are just in a while, signal seems to be sent to parent and children
        */
	int status;
        for(i = 0; i < num_node; i++)
		kill(childtable[i], SIGKILL); //waitpid(childtable[i], &status, 0);
	monitor.join();


	snprintf(command, 64, "./stopnode.sh %d", num_node);
	system(command);

	//

	
	return 0;	
}
