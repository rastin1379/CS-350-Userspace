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
#include <pthread.h>

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

struct Search
{
  struct Library *lib;
  char *word;
};

void * SingleThreadWordCount(void *args)
{
  struct Search *search = args;
  struct Library *lib = search->lib;
  char *word = search->word;

  size_t *wordCount = malloc(sizeof(size_t));
  *wordCount = 0;
  for (unsigned int i = 0; i < lib->numArticles; i++)
  {
    struct Article *art = lib->articles[i];
    for (unsigned int j = 0; j < art->numWords; j++)
    {
      // Get the length of the function.
      size_t len = strnlen(art->words[j], MAXWORDSIZE);
      if (!strncmp(art->words[j], word, len))
      {
        (*wordCount) += 1;
      }
    }
  }

  pthread_exit((void *)wordCount);
}

size_t MultithreadedWordCount( struct  Library * lib, char * word)
{
  size_t totalWordCount = 0;

  pthread_t *ptid = (pthread_t *)malloc(NUMTHREADS * sizeof(pthread_t));
  struct Article **art_ptr = lib->articles;
  long int art_size = lib->numArticles / NUMTHREADS;
  long int remains = lib->numArticles % NUMTHREADS;
  for (int i = 0; i < NUMTHREADS; i++)
  {
    struct Search *search = (struct Search *)malloc(sizeof(struct Search));
    struct Library *library = (struct Library *)malloc(sizeof(struct Library));
    int thread_lib_size = art_size + (i < remains ? 1 : 0);
    library->articles = art_ptr;
    art_ptr += thread_lib_size;
    library->numArticles = thread_lib_size;
    search->lib = library;
    search->word = word;
    pthread_create(&ptid[i], NULL, &SingleThreadWordCount, search);
  }

  for (int i = 0; i < NUMTHREADS; ++i) {
    size_t *wordCount;
    pthread_join(ptid[i], (void **)&wordCount);
    totalWordCount += *wordCount;
    free(wordCount);
  }

  // deletes

  return totalWordCount;
}
