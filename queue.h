

struct node{
	char patient_type; 
	int arrival_time;
	int service_time;
	int serv_start_time;
	int ventID;
	char gender[20]; 
	char age_group[20]; 
	struct node *next;
};

struct data{
	int noOfPatients;
	int noOfVentilators;
	int maxArrivalTime;
	int maxServiceTime;
};

struct QueueRecord{
 	struct node *front;
 	struct node *rear;
 	int size;
};
typedef struct QueueRecord *Queue;

struct ListRecord{
	struct node *head;
	struct node *tail;
	int size;
};
typedef struct ListRecord *List;

struct flagandqueue{
	int *flag;
	Queue q;
};

struct data parseInput(int argc,char *argv[]);

List createPatientList(struct data input);

void MakeEmptyQueue(Queue temp);

Queue CreateQueue();

struct flagandqueue *initialiseSimulator(struct data input);

void newPatient(List l,Queue q);
	
struct node servePatient(Queue temp);

void reportStatistics(List l,struct data input);



