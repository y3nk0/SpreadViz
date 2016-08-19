#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <igraph.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

int nextBool(float probability);
long int count_occur(long int a[],long int num_elements, int value);


int main(int argc,char *argv[]){    
	
	//seed for rand
	time_t t;
	srand ((unsigned)time(&t)); 
	
    char buftxt[20000];	
    char buftxt2[20000];
	FILE *ifile;
    igraph_t graph;
    
    char* dataset_dir=argv[4];
    ifile=fopen(dataset_dir , "r");
	
	igraph_read_graph_edgelist(&graph, ifile, 0, IGRAPH_UNDIRECTED);
	
	fclose(ifile);
	
	int nodes,edges;
    nodes=igraph_vcount(&graph);
	edges=igraph_ecount(&graph);
	


	float p_beta=atof( argv[1] );
	float p_gamma=atof( argv[2] );

	int res_beta,res_gamma;
	int p;
	
	res_beta=nextBool(p_beta);
	res_gamma=nextBool(p_gamma);

	
	
	//S=0 , I=1 , R=2
	igraph_vector_t status,time_of_infection,i_list;
	igraph_vector_init (&status,nodes);
	igraph_vector_init (&time_of_infection,nodes);
	igraph_vector_init (&i_list,nodes);
	
	//array to save which nodes are being infected at each step
	//igraph_vector_init (&step_i_list,nodes);
	
	
	
	for (p=0; p<nodes;p++){
    	
    		VECTOR(status)[p]=0;
			VECTOR(time_of_infection)[p]=100;
			VECTOR (i_list)[p]=0;
			
        }
		
		
	
	int n_iterations=1;
	int i,j;
	long int k,l;
	long int total_count=0;
	long int current_count=0;
	float current_perc;
	FILE *snfile2;
	
	long int snode,this;
	
	//to stop iterations after 2 times current count is 0
	int reg=0;
	int once=0;
	
	
	//to stop iterations manually for SIS
	//int stop=30;
	
	////////////////////////
	int cur_neighb;
	
	
	int node=atoi( argv[3]);
	
		
	char* output_file_dir=argv[5];
	snprintf(buftxt2,sizeof(buftxt2),"%s/sir_spec_results_node%d.txt",output_file_dir,node);
    snfile2=fopen(buftxt2, "w");
	
	
	for (p=0; p<nodes;p++){
	
		VECTOR(status)[p]=0;
		VECTOR(time_of_infection)[p]=100;
		VECTOR (i_list)[p]=0;
		
    }
	
	
	
	VECTOR (status)[node]=1;
	VECTOR (time_of_infection)[node]=0;
	total_count=1;
	
	//array keeping neighbors of nodes
 	igraph_vector_t neis;
	igraph_vector_init(&neis, 0);
	long int m;
	
	
	
	for (i= 1; i <= n_iterations; i++ ) {

		
		fprintf(snfile2,"node %d iteration %d\n",node,i);
		
		fprintf(snfile2,"0 [%d]\n",node);
		
		j=1;
		
		//for (j=1; j<=time_steps; j++){
		do{	
			
			fprintf(snfile2,"%d [",j);
		
			for (k=0; k<nodes; k++){
		
				
				if (VECTOR(status)[k]==1 && VECTOR (i_list)[k]==0){
				
					igraph_neighbors(&graph,&neis,k,IGRAPH_ALL);
					
				
					for (m=0; m<igraph_vector_size(&neis); m++){
						
						cur_neighb=(long int)VECTOR(neis)[m];
						if (VECTOR (status)[cur_neighb]==0){
						
						
					       res_beta=nextBool(p_beta);
							
						
							if (res_beta==1) {
						
								VECTOR (status)[cur_neighb]=1;
								VECTOR (i_list)[cur_neighb]=1;
								VECTOR (time_of_infection)[cur_neighb]=j;
								current_count++;
								fprintf(snfile2,"%d,",(int)VECTOR(neis)[m]);
							}
						
						
						}
				
					}
					
					
					res_gamma=nextBool(p_gamma);
					
					if (res_gamma==1) {
						
						VECTOR(status)[k]=2;
					
					}
					
				
				}
				
				
				

			}
			

			
			fprintf(snfile2,"]\n");
			
			
			
			for (l=0; l<nodes;l++){
	
				VECTOR (i_list)[l]=0;
			}
			
			
			
			 if (current_count==0 && once==0){
			
				once=1;
		
			}else if (current_count==0 && once==1){
			
				reg=1;
		
				}
			
			j++;
			current_count=0; 
		
	    }while (reg==0);
	    

		fprintf(snfile2,"last timestep %d\n",j-1);
		
		
		for (p=0; p<nodes;p++){
	
			VECTOR(status)[p]=0;
			VECTOR(time_of_infection)[p]=100;
			VECTOR (i_list)[p]=0;
		
		}
		
		VECTOR(status)[this]=1;
	    VECTOR (time_of_infection)[this]=0;
		total_count=1;
		reg=0;
		once=0;
		
    }	

    
	
	
		
	for (p=0; p<nodes;p++){
	
		VECTOR(status)[p]=0;
		VECTOR(time_of_infection)[p]=100;
		VECTOR (i_list)[p]=0;
		
		
    }
	
	
	
	fclose(snfile2);
	    

	
	
	igraph_destroy(&graph);
	
	return 0;
	}


int nextBool(float given_probability){

		int result;
		float result_probability;
		
    	
        result_probability = rand() % 100;
        result_probability=result_probability/100;
		
		if ( result_probability <=given_probability){
    		result=1;
    	}else {
    		result=0;
    	}
    	
    	
    	return result;
    	
	}
	
	
long int count_occur(long int a[],long int num_elements, int value)
{
   long int i, count=0;
   for (i=0; i<num_elements; i++)
   {
	 if (a[i] == value)
	 {
		++count; 
	 }
   }
   return(count);
}


