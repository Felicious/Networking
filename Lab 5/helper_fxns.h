/* helper_fxns.h */

void pmatrix(int matrix[], int sz)
{
	for(int i = 0; i < sz; i++)
	{
		if(((i%4) == 4) || ((i%4) == 0))
			printf("| %d ", matrix[i]);
		else if((i%4)== 3)
			printf("%d |\n", matrix[i]);
		else printf("%d ");
	}
	
}