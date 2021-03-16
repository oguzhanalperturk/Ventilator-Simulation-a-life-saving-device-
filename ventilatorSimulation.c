#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"queue.h"
#include"queue.c"


int main(int argc,char *argv[]){
	
	struct data input;
	struct flagandqueue *fq;
	struct node temp;
	struct node *insertpatient;
	List Listpatients;
	int i=0,j=0,k=0;
	int endtime=0;
	int totalwait = 0;
	int patientcount = 0;
	struct node *iter;
	
	input = parseInput(argc,argv);
	
	Listpatients = createPatientList(input);

	fq = initialiseSimulator(input);

	struct node *arrivalpatient = Listpatients->head->next;
	struct node *currentpatient = (struct node*)malloc(input.noOfVentilators*sizeof(struct node));
	if(currentpatient == NULL){
		printf("Out of memory space!\n");}
	

	int *ventilators = (int *)malloc(input.noOfVentilators *sizeof(int));
	if(ventilators == NULL){
		printf("Out of memory space!\n");}
	
	for(k=0;k<input.noOfVentilators;k++){
		ventilators[k] = 0;
	}
	
	int clock = arrivalpatient->arrival_time;

	while(1){
		if(Listpatients->head->next != NULL){
			arrivalpatient = Listpatients->head->next;
		}

		if(arrivalpatient->arrival_time == clock){
			while(arrivalpatient->next != NULL && arrivalpatient->arrival_time == arrivalpatient->next->arrival_time){
		 		arrivalpatient = arrivalpatient->next;
				newPatient(Listpatients,fq->q);
		 		
		 	}

			newPatient(Listpatients,fq->q);
		}
	
		for(i=0;i<input.noOfVentilators;i++){
			if(fq->q->front->next != NULL){
				if(fq->flag[i] == 1){
			
					temp = servePatient(fq->q);
					
					strcpy(currentpatient[i].age_group,temp.age_group);
					currentpatient[i].arrival_time = temp.arrival_time;
					strcpy(currentpatient[i].gender,temp.gender);
					currentpatient[i].patient_type = temp.patient_type;
					currentpatient[i].service_time = temp.service_time;
					currentpatient[i].serv_start_time = temp.serv_start_time;
					currentpatient[i].ventID = temp.ventID;
					currentpatient[i].serv_start_time = clock;
					currentpatient[i].ventID = i+1;
		
					fq->flag[i] = 0;
				}
			}
		}
	
		for(i=0;i<input.noOfVentilators;i++){
			if(currentpatient[i].service_time != 0){
				if(clock == (currentpatient[i].serv_start_time + currentpatient[i].service_time)){
				
				fq->flag[i] = 1;
				endtime = clock;
				ventilators[currentpatient[i].ventID-1]++;
				patientcount++;
				insertpatient = (struct node*)malloc(sizeof(struct node));
				if(insertpatient == NULL){
					printf("Out of memory space!\n");}
					
				strcpy(insertpatient->age_group,currentpatient[i].age_group);
				insertpatient->arrival_time = currentpatient[i].arrival_time;
				strcpy(insertpatient->gender,currentpatient[i].gender);
				insertpatient->patient_type = currentpatient[i].patient_type;
				insertpatient->service_time = currentpatient[i].service_time;
				insertpatient->serv_start_time = currentpatient[i].serv_start_time;
				insertpatient->ventID = currentpatient[i].ventID;
				
				iter = Listpatients->head;
				while(iter->next != NULL){
					iter = iter->next;
				}
				insertpatient->next = iter->next;
				iter->next = insertpatient;
				Listpatients->tail = insertpatient;
				
				}
			}	
		}
		
		if(patientcount==input.noOfPatients){
			break;
		}
		
		clock++;
	}

	
	reportStatistics(Listpatients,input);

	free(Listpatients);
	free(currentpatient);
	free(ventilators);
	free(insertpatient);
	free(fq->q);
	free(fq->flag);
	free(fq);
	
	return 0;
}


