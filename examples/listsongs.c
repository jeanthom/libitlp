/**
 * @file listsongs.c
 * @brief This programs lists all the songs found in an iTunes Library.
 * @author Jean THOMAS <itlp@jeanthomas.me>
 * 
 * One of libitlp's example program, this program lists all songs found
 * in an iTunes Library.
 */
#include <itlp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  struct msdh** msdh_array = NULL;
  struct mlth* mlth = NULL;
  unsigned int i, j;

  if (argc != 2) {
    printf("Usage : listSongs Library.itl\n");
    return EXIT_SUCCESS;
  }

  /* Parsing the library */
  msdh_array = itlp_open_library(argv[1]);
  if (!msdh_array) {
    fprintf(stderr, "Library parsing has failed !\n");
    return EXIT_FAILURE;
  }

  /* Looking for mlth block (track listing) */
  for (i = 0; msdh_array[i]; i++) {
    if (msdh_array[i]->type == BLOCK_MLTH) {
      mlth = msdh_array[i]->subblock;
    }
  }
  if (!mlth) {
    fprintf(stderr, "mlth block not found.\n");
    return EXIT_FAILURE;
  }

  /* Iterating through tracks */ 
  for (i = 0; mlth->miths[i]; i++) {
    /*
     * mith blocks contains one or multiple mhoh blocks.
     * mhoh blocks contains metadatas about tracks or playlists
     */
    for (j = 0; mlth->miths[i]->mhohs[j]; j++) {
      if (mlth->miths[i]->mhohs[j]->type == TRACK_TITLE) {
	printf("%.*s\n", mlth->miths[i]->mhohs[j]->length,
	       mlth->miths[i]->mhohs[j]->value);
      }
    }

    printf("\tIdentifier:0x%04x - Add date : %u - Unchecked : %d\n",
	   mlth->miths[i]->id,
	   mlth->miths[i]->add_date,
	   mlth->miths[i]->unchecked);
  }

  itlp_free(msdh_array);
  
  return EXIT_SUCCESS;
}
