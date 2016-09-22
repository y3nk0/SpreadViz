#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <igraph.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

int nextBool(float probability);
long int count_occur(long int a[],long int num_elements, int value);

char* ReadFile(char *filename)
{
   char *buffer = NULL;
   int string_size,read_size;
   FILE *handler = fopen(filename,"r");

   if (handler)
   {
       //seek the last byte of the file
       fseek(handler,0,SEEK_END);
       //offset from the first to the last byte, or in other words, filesize
       string_size = ftell (handler);
       //go back to the start of the file
       rewind(handler);

       //allocate a string that can hold it all
       buffer = (char*) malloc (sizeof(char) * (string_size + 1) );
       //read it all in one operation
       read_size = fread(buffer,sizeof(char),string_size,handler);
       //fread doesnt set it so put a \0 in the last position
       //and buffer is now officialy a string
       buffer[string_size] = '\0';

       if (string_size != read_size) {
           //something went wrong, throw away the memory and set
           //the buffer to NULL
           free(buffer);
           buffer = NULL;
        }
    }

    return buffer;
}


char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}


int main(void){
     
	
	//seed for rand
	time_t t;
	srand ((unsigned)time(&t)); 
	
    char buftxt[20000];	
    char buftxt2[20000];
	FILE *ifile;
    igraph_t graph;
    
    ifile=fopen("Undirected_GCC_Epinions_new.txt", "r");
	
	igraph_read_graph_edgelist(&graph, ifile, 0, IGRAPH_UNDIRECTED);
	
	fclose(ifile);
	
	int nodes,edges;
    nodes=igraph_vcount(&graph);
	edges=igraph_ecount(&graph);
	
	printf("nodes are %d\n",nodes);
	printf("edges are %d\n",edges);
	
	float p_beta=0.01;
	float p_gamma=0.8;
	int res_beta,res_gamma;
	int p;
	
	res_beta=nextBool(p_beta);
	res_gamma=nextBool(p_gamma);
	
	printf ("probabilities b and g are %d,%d\n",res_beta,res_gamma);
	
	
	//S=0 , I=1
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
		
		
	printf ("test those 3: %li,%li,%li\n",(long int)VECTOR (status)[1],(long int)VECTOR (time_of_infection)[1],(long int)VECTOR (i_list)[1]);
	
		
	//int listing[44]={803,742, 743, 1280, 718, 206, 328, 208, 826, 746, 218, 797, 1294, 1314, 804, 748, 203, 1342, 721, 463, 123, 781, 1326, 759, 780, 1273, 107, 741, 763, 1288, 618, 795, 751, 769, 5109, 79, 128, 94, 115, 738, 112, 754, 118, 739};
	//int listing[2]={803,742};

	////////////// read array  of nodes from file to array "listing" ///////////////////


	char *string = ReadFile("higgs-social_network_kcore.txt");

    char** tokens;

    //remove first "["

    char* string_chopped = string + 1;

    int len = strlen(string_chopped);

    //remove larst "]"

    string_chopped[len-1] = '\0';


    //count node set

    int n_nodes=0;


    if (string_chopped) {


     tokens = str_split(string_chopped, ','); 

     if (tokens)
    {
        int i;
        for (i = 0; *(tokens + i); i++)
        {
            printf("node=[%s]\n", *(tokens + i));
            //free(*(tokens + i));

            n_nodes++;
        }
        printf("\n");
        //free(tokens);
    }


    if (tokens)

    {

      int listing[n_nodes];
      int i;

      for ( i = 0; i < n_nodes; i++ ) {

        listing[i]= atoi( *(tokens + i) );
        //printf("%d\n", listing[i]);
      }

    }


    }


	////////////////////////////////////////////////////////////////////////////////////

	int len;
    len=sizeof(listing)/sizeof(listing[0]);
	
	printf ("length is %d\n",len); 
	
	int n_iterations=1;
	int i,j;
	long int k,l;
	long int total_count=0;
	long int current_count=0;
	float current_perc;
	FILE *snfile;
	FILE *snfile2;
	
	long int snode,this;
	
	//to stop iterations after 2 times current count is 0
	int reg=0;
	int once=0;
	
	
	//to stop iterations manually for SIS
	//int stop=30;
	
	////////////////////////
	int cur_neighb;
	
	
	
	
	for (snode=0; snode<len; snode++){
		
		snprintf(buftxt,sizeof(buftxt),"/cygdrive/c/Users/server/Desktop/Maria/SIR_test/EmailEnron/i_to_s_08/common_p001/sir_results_node%d.txt",listing[snode]);
	    snfile=fopen(buftxt, "w");
		
		snprintf(buftxt2,sizeof(buftxt2),"/cygdrive/c/Users/server/Desktop/Maria/SIR_test/EmailEnron/i_to_s_08/common_p001/sir_spec_results_node%d.txt",listing[snode]);
	    snfile2=fopen(buftxt2, "w");
		
		
		for (p=0; p<nodes;p++){
    	
    		VECTOR(status)[p]=0;
			VECTOR(time_of_infection)[p]=100;
			VECTOR (i_list)[p]=0;
			
        }
		
		
		
		this=listing[snode];
		printf("%ld\n",this);
    	
		
		VECTOR (status)[this]=1;
		VECTOR (time_of_infection)[this]=0;
		total_count=1;
		
		//array keeping neighbors of nodes
     	igraph_vector_t neis;
    	igraph_vector_init(&neis, 0);
    	long int m;
		
		
		
		for (i= 1; i <= n_iterations; i++ ) {
	
			
			
			fprintf(snfile,"node %d iteration %d\n",listing[snode],i);
			
			fprintf(snfile,"0 1 1 %f\n",((float)1/(float)nodes));
			
			fprintf(snfile2,"node %d iteration %d\n",listing[snode],i);
			
			fprintf(snfile2,"0 [%d]\n",listing[snode]);
			
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
				
				//current_count=count_occur(VECTOR(time_of_infection),nodes,j);
				total_count+=current_count;
				current_perc=(float)total_count/(float)nodes;
				
				fprintf(snfile2,"]\n");
				fprintf(snfile,"%d %ld %ld %f\n",j,current_count,total_count,current_perc);
				
				
				
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
		    
			fprintf(snfile,"last timestep %d\n",j-1);
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
		
		
		
		fclose(snfile);
	    
    }
	
	////////////////////
	
	
	
	
	
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