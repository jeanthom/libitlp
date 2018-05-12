#include <itlp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  struct msdh** msdh_array = NULL;
  struct mlph* mlph = NULL;
  struct miph* miph = NULL;
  struct mhgh* mhgh = NULL;
  int i, j, k;

  if (argc != 2) {
    printf("Usage : %s /path/to/iTunes\\ Library.itl\n", argv[0]);
    return EXIT_SUCCESS;
  }

  msdh_array = itlp_open_library(argv[1]);
  if (!msdh_array) {
    fprintf(stderr, "Library parsing has failed !\n");
    return EXIT_FAILURE;
  }

  /* Finding the mlph block in the msdh array */
  for (i = 0; msdh_array[i]; i++) {
    if (msdh_array[i]->type == BLOCK_MLPH) {
      mlph = (struct mlph*)(msdh_array[i]->subblock);
    } else if (msdh_array[i]->type == BLOCK_MHGH) {
      mhgh = (struct mhgh*)(msdh_array[i]->subblock);
    }
  }

  /* Iterating through all playlists */
  for (i = 0; mlph->miphs[i]; i++) {
    miph = mlph->miphs[i];

    for (j = 0; miph->mhohs[j]; j++) {
      if (miph->mhohs[j]->type == PLAYLIST_NAME) {
	printf("%.*s\n", miph->mhohs[j]->length,
	       miph->mhohs[j]->value);
      }
    }
    for (j = 0; miph->mhohs[j]; j++) {
      if (miph->mhohs[j]->type != PLAYLIST_NAME) {
	printf("\tType:%d, length:%d\n", miph->mhohs[j]->type,
	       miph->mhohs[j]->length);
	for (k = 0; k < miph->mhohs[j]->length; k++) {
	  printf("%02x", (unsigned char)(miph->mhohs[j]->value[k]));
	}
	printf("\n");
      }
    }

    for (j = 0; miph->mtphs[j]; j++) {
      printf("\tTrack id : %02x\n", miph->mtphs[j]->identifier);
    }
  }
  
  return EXIT_SUCCESS;
}
