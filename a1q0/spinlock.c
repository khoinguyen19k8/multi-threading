#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>

#define ROUNDS (100)
#define ITERATIONS (64)
#define BIGPRIME (4093082899)
#define TIMEOUT (10)

uint8_t lock = 0;

void
compute()
{
	int r, i;

	for (i = 0; i < ROUNDS; i++) {
		/*
		 * Break condition triggers with a probability  1 / 2^32,
		 * it's there so the computation of r isn't optimized out. The
		 * rand() call actually modifies libc state, so it doesn't
		 * really need that, but we may change the computation later.
		 */
		r = rand();
		if (r % BIGPRIME == 0)
			break;
	}
}

void
spinlock_lock()
{
    /* XXX FILLMEIN */
	while (__atomic_test_and_set(&lock, __ATOMIC_ACQ_REL) == true )
	{}
}

void
spinlock_unlock()
{
    /* XXX FILLMEIN */
	__atomic_clear((bool *)&lock, __ATOMIC_ACQ_REL);
}

void *
workload(void *ctx)
{
	int *counter = (int *) ctx;
	int iters;

	for (iters = 0; iters < ITERATIONS; iters++) {
		compute();

		spinlock_lock();
		*counter = *counter + 1;
		spinlock_unlock();

	}

	pthread_exit(NULL);
}

void
usage(void)
{
	printf("Usage: ./spinlock <nthreads>\n");
	exit(1);
}

int
main(int argc, char **argv)
{
	pthread_t *threads;
	long nthreads;
	int error;
	int counter;
	int i;

	if (argc != 2)
		usage();

	nthreads = strtol(argv[1], NULL, 10);
	if (nthreads == 0)
		usage();

	threads = malloc(sizeof(*threads) * nthreads);
	if (threads == NULL) {
		perror("malloc");
		exit(1);
	}

	srand(time(NULL));
	counter = 0;

	alarm(TIMEOUT);

	for (i = 0; i < nthreads; i++) {
		error = pthread_create(&threads[i], NULL, workload, (void *)&counter);
		if (error != 0) {
			fprintf(stderr, "freqcount: %s\n", strerror(error));
			exit(1);
		}

	}

	for (i = 0; i < nthreads; i++)
		pthread_join(threads[i], NULL);

	assert(counter == (nthreads * ITERATIONS));

	free(threads);

//	printf("The counter value is:%d\n", counter);
	return (0);
}
