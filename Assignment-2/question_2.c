#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>


void main()
{
  /*new file name*/	
	char *file_name = "question_2.txt";

	FILE *file_ptr = NULL;
	
	/*Part 1 - Print to standard out an interesting string using printf*/
	printf("\nHomework 2 part 1 completed\n");
	
  /*part 2,3-providing required permissions to the file created*/
	int result = chmod("question_2.txt", 0777);
  /*conditional check for successful permission changed*/
	if (result == 0)
	{
		printf("\nSuccessful in permission change\n");
	}
	else printf("Not successful in changing permission\n");
	
	/*Part 4-create file and open it in write mode*/
	 file_ptr = fopen(file_name, "w");
	 if (file_ptr == NULL)
    {
        printf("File does not exists \n");
    }
	
	/*part 5-Write a character to the file*/
	char write_value = 'h';
	result = fwrite(&write_value, 1, sizeof(write_value), file_ptr);
	if (result != sizeof(write_value))
	{
		printf("Error in writing a character \n");
	}
	else printf("Write successful \n");
	
	/*Part 6-Closing file after successfully writing the character*/
	result = fclose(file_ptr);
	if (result != 0)
	{
		printf("Error in closing the file\n");
	}
	else printf("File is successfully closed \n");
	
	/*Part 7- open file and append data in file*/
	file_ptr = fopen(file_name, "a");
	if (file_ptr == NULL)
    {
        printf("Error in opening the file in append mode \n");
    }
	
	/*Part 8-Dynamically allocate an array of memory*/
	char *ip_str = malloc(sizeof(char*)*100);
	
	/*Part 9-Read and input string from the command line and write to the string*/
	fgets(ip_str,100,stdin);
  printf("input string value is: %s\n",ip_str);	
	/*part 10-Write the string to the file*/
	result = fwrite(ip_str, 1, strlen(ip_str), file_ptr);
	if (result == 0)
	{
		printf("Error in writing string \n");
	}
	else printf("Write successful \n");
	
	/*part 11-Flush file output*/
	fflush(file_ptr);
	
	/*Part 12-Close the file*/
  fclose(file_ptr);
	
	/*Part 13-open the file*/
	file_ptr = fopen(file_name, "r");
	
	/*Part 14-Read a single character*/
	char read_value= getc(file_ptr);
	if (read_value != EOF)
	{
		printf("Character value is = %c \n", read_value);
	}
	else printf("Couldn't read the character \n");
	
	/*Part 15-Read a string of characters*/
  
	ip_str=fgets(ip_str,100, file_ptr);
	if (ip_str != NULL)
	{
		printf("String read is: %s\n",ip_str);
	}
	else printf("\nError while reading file\n");
	
	/*Part 16-Close the file*/
	fclose(file_ptr);
	
	/*Part 17-Free the memory*/
	free(file_ptr);
	
}
