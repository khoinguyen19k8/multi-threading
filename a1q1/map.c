/* map.c
 * ----------------------------------------------------------
 *  CS350
 *  Assignment 1
 *  Question 1
 *
 *  Purpose:  Gain experience with threads and basic 
 *  synchronization.
 *
 *  YOU MAY ADD WHATEVER YOU LIKE TO THIS FILE.
 *  YOU CANNOT CHANGE THE SIGNATURE OF MultithreadedWordCount.
 * ----------------------------------------------------------
 */
#include "data.h"

#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------
 * MultithreadedWordCount
 * --------------------------------------------------------------------
 * Takes a Library of articles containing words and a word.
 * Returns the total number of times that the word appears in the 
 * Library.
 *
 * For example, "There the thesis sits on the theatre bench.", contains
 * 2 occurences of the word "the".
 * --------------------------------------------------------------------
 */

#define NTHREADS 16

struct arg_struct 
{
	struct Article* article;
	char* word;
};
void* WordCount(void *arguments)
{
	struct arg_struct *args = (struct arg_struct*)arguments;	
	struct Article* article = args -> article;
	char* word = args -> word;
	int num_words = article -> numWords;
	int i;
	int *word_count = (int*)malloc(sizeof(int));
	for (i = 0; i < num_words; i++)
	{
		if (strcmp( (article -> words)[i], word ) == 0 )
		{
			*word_count += 1;
		}
	}
	printf("word count: %d\n", *word_count);
	pthread_exit((void*)word_count);
}
size_t MultithreadedWordCount( struct  Library * lib, char * word )
{
    /* XXX FILLMEIN */
 	int num_articles = lib -> numArticles;
    struct Article ** articles = lib -> articles;
	int articles_consumed = 0;
	int threads_active = 0;
	int error = 0;

	// occurences of the target word

	int *temp_word_count;
	int global_word_count = 0;

	// Threads arguments 
	pthread_t *threads;
	struct arg_struct args;
	threads = malloc(sizeof(*threads) * NTHREADS);	

	while (articles_consumed < num_articles)
	{
//		printf("articles consumed: %d\n", articles_consumed);
		threads_active = 0;
		while (threads_active < NTHREADS || articles_consumed < num_articles)
		{
			args.word = word;
			args.article = articles[articles_consumed];
			error = pthread_create(&threads[threads_active], NULL, WordCount,(void*)&args);
		if (error != 0) 
		{
			fprintf(stderr, "freqcount: %s\n", strerror(error));
			exit(1);
		}		
			threads_active += 1;
			articles_consumed += 1;
		}
		for (int j = 0; j < threads_active; j++)
		{
			pthread_join(threads[j], (void**)(&temp_word_count));
			//printf("temp_word_count: %d\n", *temp_word_count);
			global_word_count += *temp_word_count;
		}
	}
	return global_word_count;
}


