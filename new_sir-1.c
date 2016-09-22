#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <igraph.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <string.h>

int nextBool(float probability);
long int count_occur(long int a[],long int num_elements, int value);
void replaceCharacter(char *replace, char *with, char *str);

int main(){

	int content_length;
	char * s_content;
	int i;
	int c;
	char * s_text;

	printf("%s%c%c\n", "Content-Type:text/html;charset=iso-8859-1",13,10);
	printf(" <html><title>hello</title><body> \n");

	content_length = atoi(getenv("CONTENT_LENGTH"))+10000;
	if(content_length == 0){
		return 0;
	}

	printf("<BR>content_length: %d", content_length);
	printf("<BR>");

	s_content = (char *) malloc(sizeof(char) * content_length);
	if(s_content == NULL){
		return 0;
	}

	for ( i = 0 ; i < content_length && (c = getchar()) != EOF ; i++ ) {
		s_content[i] = c;
	}

	i=0;
	// }else{
	// 	i=1;
	// }

  printf ("String is now: %s\n", s_content);
	FILE *ifile;
	ifile = fopen("data/string.txt", "w");
	fputs(s_content, ifile);
  fclose(ifile);

	char *ps;

	int s;
	ps = strtok(s_content, "------");
	// char *array1[30];

	int counter_s;

	const char *PATTERN1 = "filename=\"";
  const char *PATTERN2 = "\"";

  char *target = NULL;
  char *start, *end;

	int s_node;
	double beta,gamma;
	char *in_file;
	double d1, d2, d3;

	while (ps != NULL){
		ps = strtok(NULL, "------");
		// array1[i] = ps;
		// printf("%s",ps);
		if(i==1){
			// printf("%s\n\n",strstr(ps, "filename"));
			// printf("%s",ps);

			if ( (start = strstr( ps, PATTERN1 ))){
	        start += strlen( PATTERN1 );

	        if ( (end = strstr( start, PATTERN2 ))){
	            target = ( char * )malloc( end - start + 1 );
	            memcpy( target, start, end - start );
	            target[end - start] = '\0';
	        }
	    }
			// sscanf(ps, "%*[^\"]%*c%[^\"]%*c%*[^\n]%*c", filename);
		}else if(i==2){
			in_file = strndup(ps+34, strlen(ps));
		}else if(i==11){
			sscanf(ps,"%*[^0-9]%d",&s_node);

		}else if(i==17){

			sscanf(ps,"%*[^0-9]%lf",&beta);

			// printf("%lf\n",beta

		}else if(i==20){
			// printf("%f\n",gamma );
			sscanf(ps,"%*[^0-9]%lf",&gamma);

		}
		i++;
	}

	printf("<p>");
	printf("%d,%s,%d,%lf,%lf",i,target,s_node,beta,gamma);
	printf("</p>");

	// if(ferror(stdin)){
	// 	printf("<BR>ERROR");
	// }else{
	// 	printf("<BR>NO ERROR");
	// }

	free(s_content);

	//seed for rand
	time_t t;
	srand ((unsigned)time(&t));

	char buftxt[20000];
	char buftxt2[20000];

	igraph_t graph;
	int ch;
	// char* dataset_dir= target ;

  // ifile = fmemopen(in_file, strlen (in_file), "r");
  // while ((ch = fgetc (ifile)) != EOF){
  //   // fwrite(ifile,ch);
  //   printf ("%c\n", ch);
	// }
  // fclose (ifile);
	char *kl;
	char *ks;
 	char * start_n,* end_n;
	int counter = 0;

	// len = strlen(in_file);
	replaceCharacter(",", " ", in_file);
	// ifile = fopen("/Applications/XAMPP/xamppfiles/htdocs/icdm2016_demo/data/edgelist.txt", "w");
	if (strstr(target, "txt") != NULL) {
		ifile = fopen("data/edgelist.txt", "w");
		fputs(in_file, ifile);
  	fclose(ifile);
	}

	//
	// kl = strtok(in_file, "\n");
	// while (kl != NULL){
	// 	kl = strtok(NULL, "\n");
	// 	// printf("%s-",kl);
	// 	char *rest = kl;
	// 	start_n = strtok_r(rest, ",", &rest);
	// 	end_n = strtok_r(rest, ",", &rest);
	//
	// 	printf("%s-%s\n", start_n,end_n);
	//
	// 	// printf("%s",kl);
	// 	// start_n = strchr(kl, ',');
	//
	// 	igraph_add_edge(&graph,start_n,end_n);
	// }
	// printf("%s",kl);

	// ifile = fopen("/Applications/XAMPP/xamppfiles/htdocs/icdm2016_demo/data/edgelist.txt", "r");
	ifile = fopen("data/edgelist.txt", "r");
	// while ((ch = fgetc (ifile)) != EOF){
  //   // fwrite(ifile,ch);
  //   printf ("%c", ch);
	// }
	igraph_read_graph_edgelist(&graph, ifile, 0, IGRAPH_UNDIRECTED);
	fclose(ifile);

	int nodes,edges;
	nodes=igraph_vcount(&graph);

	// printf("<p>%d</p>",nodes );
	edges=igraph_ecount(&graph);

	float p_beta= beta ;
	float p_gamma= gamma ;

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
	int j;
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

	int cur_neighb;
	int node= s_node;

	// char* output_file_dir=argv[5];
	// snprintf(buftxt2,sizeof(buftxt2),"/Applications/XAMPP/xamppfiles/htdocs/icdm2016_demo/data/sir_spec_results_node%d.txt",node);
	snprintf(buftxt2,sizeof(buftxt2),"data/sir_spec_results_node%d.txt",node);
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

	printf(" </body></html>");

	// printf (redirect_page_format, getenv("HTTP_REFERER"));

	return 1;
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


long int count_occur(long int a[],long int num_elements, int value){
	long int i, count=0;
	for (i=0; i<num_elements; i++){
		if (a[i] == value){
			++count;
		}
	}
	return(count);
}

void replaceCharacter(char *replace, char *with, char *str){
	char *pch;
  do{
      pch = strstr (str, replace);
      if(pch != NULL)
      strncpy (pch, with, 1);
  }
  while(pch != NULL);
}
