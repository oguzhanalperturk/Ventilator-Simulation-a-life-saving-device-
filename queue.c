

struct data parseInput(int argc,char *argv[]){
	int i,a;
	struct data ret;
	
	if(argc == 1){
		printf("Please provide command line argumands\n ");

	}
	for(i=1;i<=argc;i++){
		a = atoi(argv[i]);
		
		if(i==1){
			ret.noOfPatients = a;
		}
		else if(i==2){
			ret.noOfVentilators = a;
		}
		else if(i==3){
			ret.maxArrivalTime = a;
		}
		else if(i==4){
			ret.maxServiceTime = a;
		}
	}
	
	return ret;
}


List createPatientList(struct data input){
	
	int i,random;
	srand(time(NULL));
	struct node *iter;
	List temp = (List)malloc(sizeof(List));
	if(temp == NULL){
		printf("Out of memory space!\n");
	}
	temp->head = (struct node*)malloc(sizeof(struct node));
	if(temp->head == NULL){
		printf("Out of memory space!\n");
	}
	temp->tail = temp->head;
	temp->head->next = NULL;
	temp->size = 0;
	for(i=0;i<input.noOfPatients;i++){
		struct node *insert = (struct node*)malloc(sizeof(struct node));
		if(insert == NULL){
		printf("Out of memory space!\n");
		}
		insert->next = NULL;
		insert->arrival_time = rand()%(input.maxArrivalTime)+1;
		insert->service_time = rand()%(input.maxServiceTime)+1;
		insert->serv_start_time = 0;
		insert->ventID = 0;
		random = rand()%2;
		
		if(random == 0){
			strcpy(insert->gender,"Male");
		}
		else if(random == 1){
			strcpy(insert->gender,"Female");
		}
		
		random = rand()%3;
		
		if(random == 0){
			strcpy(insert->age_group,"Young");
		}
		else if(random == 1){
			strcpy(insert->age_group,"Adult");
		}
		else if(random == 2){
			strcpy(insert->age_group,"Elderly");
		}
		
		random = rand()%3;
		
		if(random == 0){
			insert->patient_type = 'M';    /* Mild */
		}
		else if(random == 1){
			insert->patient_type = 'D';    /* Moderate */
		}
		else if(random == 2){
			insert->patient_type = 'S';    /* Severe */
		}
	
		
		if(temp->head->next == NULL){
			temp->head->next = insert;
			temp->tail = insert;
		}
		else{
			iter = temp->head;
			while(iter->next !=NULL && insert->arrival_time >= iter->next->arrival_time){
				iter = iter->next;
			}
			if(iter->next != NULL){
				insert->next = iter->next;
				iter->next = insert;
		
			}
			else{
				temp->tail->next = insert;
				temp->tail = insert;
			}
		}
		temp->size++;
	}

	return temp;
}


void MakeEmptyQueue(Queue temp){
	temp->size = 0;
	temp->front = (struct node*)malloc(sizeof(struct node));
	if(temp->front == NULL){
		printf("Out of memory space!\n");
	}
	temp->front->next = NULL;
	temp->rear = temp->front;
}


Queue CreateQueue(){
	
	Queue temp = (Queue)malloc(sizeof(Queue));
	if(temp==NULL){
		printf("Out of memory space!\n");
	}
	MakeEmptyQueue(temp);

	return temp;
}
	
	
struct flagandqueue *initialiseSimulator(struct data input){
	
	struct flagandqueue *fq = (struct flagandqueue*)malloc(sizeof(struct flagandqueue));
	if(fq == NULL){
		printf("Out of memory space!\n");
	}
	int i = 0;
	
	fq->flag = (int *)malloc(input.noOfVentilators*sizeof(int));
	if(fq->flag == NULL){
		printf("Out of memory space!\n");
	} 
	
	for(i=0;i<input.noOfVentilators;i++){
		fq->flag[i] = 1;
	}
	
	fq->q = CreateQueue();
	
	return fq;
	
}


void newPatient(List l,Queue q){
	
	if(l->head->next != NULL){
		struct node *insert = (struct node*)malloc(sizeof(struct node));
		if(insert == NULL){
		printf("Out of memory space!\n");
		}
		insert->next = NULL;
		struct node *del;
		struct node *iter;
		
		del = l->head->next;
		
		strcpy(insert->age_group,del->age_group);
		insert->arrival_time = del->arrival_time;
		strcpy(insert->gender,del->gender);
		insert->patient_type = del->patient_type;
		insert->service_time = del->service_time;
		insert->serv_start_time = del->serv_start_time;
		insert->ventID = del->ventID;
	
		l->head->next = l->head->next->next;
		free(del);
		
		
		if(q->front->next == NULL){
			q->front->next = insert;
			q->rear = insert;
		}
		
		else{
			iter = q->front;
			if(insert->patient_type == 'M'){
				q->rear->next = insert;
				q->rear = insert;
			}
			else if(insert->patient_type == 'D'){
				while(iter->next !=NULL && iter->next->patient_type != 'M'){
					iter = iter->next;
				}
				if(iter->next == NULL){
					insert->next = iter->next;
					iter->next = insert;
					q->rear = insert;
				}
				else{
					insert->next = iter->next;
					iter->next = insert;
				}
				
			}
			else if(insert->patient_type == 'S'){
				while(iter->next !=NULL && iter->next->patient_type != 'D' && iter->next->patient_type != 'M'){
					iter = iter->next;
				}
				if(iter->next == NULL){
					insert->next = iter->next;
					iter->next = insert;
					q->rear = insert;
				}
				else{
					insert->next = iter->next;
					iter->next = insert;
				}
			}
		}
	
		q->size++;
		l->size--;
	}
}
	
	
struct node servePatient(Queue temp){
	
	struct node ret;
	
	if(temp->size != 0){
		struct node *del;
		del = temp->front->next;

		strcpy(ret.age_group,del->age_group);
		ret.arrival_time = del->arrival_time;
		strcpy(ret.gender,del->gender);
		ret.patient_type = del->patient_type;
		ret.service_time = del->service_time;
		ret.serv_start_time = del->serv_start_time;
		ret.ventID = del->ventID;
	
		temp->front->next = temp->front->next->next;
		free(del);
		temp->size--;
	}
	
	return ret;
}

	
void reportStatistics(List l,struct data input){
	
	int waitingtime = 0,maxwait =0;
	int male = 0,female = 0;
	int young =0,adult = 0,elderly=0; 
	int severe = 0,moderate = 0,mild = 0;
	int i;
	float avg,totalwait = 0;
	struct node *iter;
	int *v = (int *)malloc(input.noOfVentilators*sizeof(int));
	for(i =0;i<input.noOfPatients;i++){
		v[i]= 0;
	}
	for(iter = l->head->next;iter!= NULL;iter=iter->next){
		if(iter->patient_type == 'S'){
			severe++;
		}
		else if(iter->patient_type == 'D'){
			moderate++;
		}
		else if(iter->patient_type == 'M'){
			mild++;
		}
		if(strcmp("Male",iter->gender) == 0){
			male++;
		}
		else if(strcmp("Female",iter->gender) == 0){
			female++;
		}
		if(strcmp("Young",iter->age_group) == 0){
			young++;
		}
		else if(strcmp("Adult",iter->age_group) == 0){
			adult++;
		}
		else if(strcmp("Elderly",iter->age_group) == 0){
			elderly++;
		}
		waitingtime = iter->serv_start_time - iter->arrival_time;
		if(waitingtime >= maxwait){
			maxwait = waitingtime;
		}
		totalwait += waitingtime;
		v[(iter->ventID)-1]++;
	}
	avg = totalwait / input.noOfPatients;
	
	printf("****************Report*****************\n");
	printf("The number of ventilators: %d\n",input.noOfVentilators);
	printf("The number of patients: %d\n",input.noOfPatients);
	printf("Number of patients for each patient type:\n");
	printf("Severe: %d\n",severe);
	printf("Moderate: %d\n",moderate);
	printf("Mild: %d\n",mild);
	printf("Number of patients for each ventilator:\n");
	
	for(i=0;i<input.noOfVentilators;i++){
		printf("Ventilator %d: %d \n",i+1,v[i]);
	}
	printf("Completion time: %d\n",l->tail->serv_start_time + l->tail->service_time);
	printf("Average time spent in the queue: %.2f\n",avg);
	printf("Maximum waiting time: %d\n",maxwait);
	if(male > female){
		printf("Most gender usage: Male\n");
	}
	else if(female > male){
		printf("Most gender usage: Female\n");
	}
	else if(female==male){
		printf("Most gender usage: Equal\n");
	}
	if(young > adult && young > elderly){
		printf("Most age usage: Young\n");
	}
	else if(adult > young && adult > elderly){
		printf("Most age usage: Adult\n");
	}
	else if(elderly > adult && elderly > young){
		printf("Most age usage: Elderly\n");
	}
	else if(elderly == adult && adult == young){
		printf("Equal amount of Young, Elderly and Adult\n");
	}	
}
