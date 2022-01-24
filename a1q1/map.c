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
	struct Article** articles;
	char* word;
	int start;
	int end;
};
void* WordCount(void *arguments)
{
	// extracting arguments
	struct arg_struct *args = (struct arg_struct*)arguments;	
	struct Article** articles = args -> articles;
	char* word = args -> word;
	int start = args -> start;
	int end = args -> end;

	// occurences of the target word
	int *word_count = (int*)malloc(sizeof(int));
	for (int i = start; i < end; i++)
	{
		struct Article* current_article = articles[i];
		int num_words = current_article -> numWords;
		for (int j = 0; j < num_words; j++)
		{
			if (strcmp( (current_article -> words)[j], word ) == 0 )
			{
				*word_count += 1;
			}
		}
	}


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
	int work_load = 0;
	// Occurences of the target word

	int *temp_word_count;
	int global_word_count = 0;

	// Threads arguments 
	pthread_t *threads;
	threads = (pthread_t*)malloc(sizeof(*threads) * NTHREADS);	
	struct arg_struct* args = (struct arg_struct*) malloc(sizeof(struct arg_struct) * NTHREADS);
	int start = 0;

	int end = 0;
	
	while (articles_consumed < num_articles)
	{
		if (num_articles - articles_consumed < NTHREADS)
		{
			work_load = 1;
		}
		else
		{
			work_load = num_articles / NTHREADS;
		}
		threads_active = 0;	
		// Dividing each thread amount of work = work_load
		while (articles_consumed < num_articles && threads_active < NTHREADS)
		{
			args[threads_active].word = word;
			args[threads_active].articles = articles;
			start = end; args[threads_active].start = start;
			end = start + work_load; args[threads_active].end = end;
			articles_consumed += work_load;

			error = pthread_create(&threads[threads_active], NULL, WordCount, (void*)&args[threads_active]);
			if (error != 0) {
				fprintf(stderr, "freqcount: %s\n", strerror(error));
				exit(1);
			}
			threads_active += 1;
		}
		// Joining and gathering results
		for (int i = 0; i < threads_active; i++)
		{
			pthread_join(threads[i], (void**)&temp_word_count);
			global_word_count += *temp_word_count;
		}
	}
	free(threads);	
	free(args);
	return global_word_count;
}


