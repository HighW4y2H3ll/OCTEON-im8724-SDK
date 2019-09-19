#include <pthread.h>
#include <asm/unistd.h>
#include <stdio.h>

static int
gettid (void)
{
  return syscall (__NR_gettid);
}

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int available = 0;

/* Same condition variable.  */
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
int available2 = 0;

pthread_spinlock_t spin;
pthread_t thr, thr2;

__thread j = 0;
void * dealloc (void *a)
{
  int tid = gettid ();
  printf ("thread running: %d\n", tid);
  for (j = 0; j < 1; j++)
    {
      pthread_mutex_lock (&mutex);
      available++;
      printf ("thread signaling %d (%d)\n", available, tid);
      pthread_cond_broadcast (&cond);
      pthread_mutex_unlock (&mutex);

      pthread_mutex_lock (&mutex2);
      available2++;
      printf ("thread signaling on second %d (%d)\n", available2, tid);
      pthread_cond_broadcast (&cond);
      pthread_mutex_unlock (&mutex2);      
    }
}


main ()
{
  pthread_spin_lock (&spin);
  pthread_mutex_lock (&mutex);
  pthread_mutex_lock (&mutex2);

  pthread_create (&thr, NULL, dealloc, NULL);
  pthread_create (&thr2, NULL, dealloc, NULL);
  for (j = 0; j < 2; j++)
    {
      if (j)
	pthread_mutex_lock (&mutex);
      puts ("main waiting");
      while (!available)
	{
	  pthread_cond_wait (&cond, &mutex);
	  printf ("main signaled %d\n", available);
	}
      available--;
      pthread_mutex_unlock (&mutex);

      if (j)
	pthread_mutex_lock (&mutex2);
      puts ("main waiting for second");
      while (!available2)
	{
	  pthread_cond_wait (&cond, &mutex2);
	  printf ("main signaled for second %d\n", available2);
	}
      available2--;
      pthread_mutex_unlock (&mutex2);
    }
  pthread_spin_unlock (&spin);
}
