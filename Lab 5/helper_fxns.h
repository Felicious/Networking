/* helper_fxns.h */

//function that prints the matrix


void pmatrix(int sz, int matrix[sz][sz])
{
	for(int row = 0; row < sz; row++)
	{
		printf("| %d %d %d %d |\n", matrix[row][0], matrix[row][1], matrix[row][2], matrix[row][3]);
	}
}