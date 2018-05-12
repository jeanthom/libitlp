#include <stdlib.h>
#include <stdint.h>

#include "itlp/types.h"
#include "free.h"

void itlp_free(struct msdh **msdh_array) {
  int i;

  for (i = 0; msdh_array[i]; i++) {
    itlp_free_msdh(msdh_array[i]);
  }

  free(msdh_array);
}

void itlp_free_msdh(struct msdh *msdh_block) {
  switch (msdh_block->type) {
  case BLOCK_MFDH:
    itlp_free_mfdh((struct mfdh*)msdh_block->subblock);
    break;
  case BLOCK_MHGH:
    itlp_free_mhgh((struct mhgh*)msdh_block->subblock);
    break;
  case BLOCK_MLAH:
    itlp_free_mlah((struct mlah*)msdh_block->subblock);
    break;
  case BLOCK_MLIH:
    itlp_free_mlih((struct mlih*)msdh_block->subblock);
    break;
  case BLOCK_MLTH:
  case BLOCK_MLTH_ALT:
    itlp_free_mlth((struct mlth*)msdh_block->subblock);
    break;
  case BLOCK_MLQH:
    itlp_free_mlqh((struct mlqh*)msdh_block->subblock);
    break;
  case BLOCK_STSH:
    itlp_free_stsh((struct stsh*)msdh_block->subblock);
    break;
  case BLOCK_MLPH:
  case BLOCK_MLPH_ALT:
    itlp_free_mlph((struct mlph*)msdh_block->subblock);
    break;
  case BLOCK_MLSH:
    itlp_free_mlsh((struct mlsh*)msdh_block->subblock);
    break;
  case BLOCK_FILE:
    itlp_free_file((struct file*)msdh_block->subblock);
    break;
  case BLOCK_MLRH:
    itlp_free_mlrh((struct mlrh*)msdh_block->subblock);
    break;
  default:
    break;
  }
}

void itlp_free_mfdh(struct mfdh *mfdh_block) {
  free(mfdh_block->application_version);
  free(mfdh_block);
}

void itlp_free_mhgh(struct mhgh *mhgh_block) {
  int i;
  
  for (i = 0; mhgh_block->mhohs[i]; i++) {
    itlp_free_mhoh(mhgh_block->mhohs[i]);
  }

  free(mhgh_block->mhohs);
  free(mhgh_block);
}

void itlp_free_mhoh(struct mhoh *mhoh_block) {
  free(mhoh_block->value);
  free(mhoh_block);
}

void itlp_free_mlah(struct mlah *mlah_block) {
  int i;

  for (i = 0; mlah_block->miahs[i]; i++) {
    itlp_free_miah(mlah_block->miahs[i]);
  }

  free(mlah_block->miahs);
  free(mlah_block);
}

void itlp_free_miah(struct miah *miah_block) {
  int i;

  for (i = 0; miah_block->mhohs[i]; i++) {
    itlp_free_mhoh(miah_block->mhohs[i]);
  }

  free(miah_block->mhohs);
  free(miah_block);
}

void itlp_free_mlih(struct mlih *mlih_block) {
  int i;

  for (i = 0; mlih_block->miihs[i]; i++) {
    itlp_free_miih(mlih_block->miihs[i]);
  }

  free(mlih_block->miihs);
  free(mlih_block);
}

void itlp_free_miih(struct miih *miih_block) {
  int i;

  for (i = 0; miih_block->mhohs[i]; i++) {
    itlp_free_mhoh(miih_block->mhohs[i]);
  }

  free(miih_block->mhohs);
  free(miih_block);
}

void itlp_free_mlth(struct mlth *mlth_block) {
  int i;

  for (i = 0; mlth_block->miths[i]; i++) {
    itlp_free_mith(mlth_block->miths[i]);
  }

  free(mlth_block->miths);
  free(mlth_block);
}

void itlp_free_mith(struct mith *mith_block) {
  int i;

  for (i = 0; mith_block->mhohs[i]; i++) {
    itlp_free_mhoh(mith_block->mhohs[i]);
  }

  free(mith_block->mhohs);
  free(mith_block);
}

void itlp_free_mlqh(struct mlqh *mlqh_block) {
  int i;

  /* Freeing embedded miqhs */
  for (i = 0; mlqh_block->miqhs[i]; i++) {
    itlp_free_miqh(mlqh_block->miqhs[i]);
  }
  free(mlqh_block->miqhs);

  /* Freeing embedded mhohs */
  for (i = 0; mlqh_block->mhohs[i]; i++) {
    itlp_free_mhoh(mlqh_block->mhohs[i]);
  }
  free(mlqh_block->mhohs);

  free(mlqh_block);
}

void itlp_free_stsh(struct stsh *stsh_block) {
  free(stsh_block);
}

void itlp_free_mlph(struct mlph *mlph_block) {
  int i;

  /* Freeing embedded miphs */
  for (i = 0; mlph_block->miphs[i]; i++) {
    itlp_free_miph(mlph_block->miphs[i]);
  }
  free(mlph_block->miphs);

  free(mlph_block);
}

void itlp_free_mlsh(struct mlsh *mlsh_block) {
  int i;

  for (i = 0; mlsh_block->msphs[i]; i++) {
    itlp_free_msph(mlsh_block->msphs[i]);
  }
  free(mlsh_block->msphs);

  free(mlsh_block);
}

void itlp_free_file(struct file *file_block) {
  free(file_block->string);
  free(file_block);
}

void itlp_free_mlrh(struct mlrh *mlrh_block) {
  int i;

  for (i = 0; mlrh_block->mprhs[i]; i++) {
    itlp_free_mprh(mlrh_block->mprhs[i]);
  }
  free(mlrh_block->mprhs);

  free(mlrh_block);
}

void itlp_free_miqh(struct miqh *miqh_block) {
  int i;

  for (i = 0; miqh_block->mhohs[i]; i++) {
    itlp_free_mhoh(miqh_block->mhohs[i]);
  }
  free(miqh_block->mhohs);

  free(miqh_block);
}

void itlp_free_miph(struct miph *miph_block) {
  int i;

  for (i = 0; miph_block->mhohs[i]; i++) {
    itlp_free_mhoh(miph_block->mhohs[i]);
  }
  free(miph_block->mhohs);

  for (i = 0; miph_block->mtphs[i]; i++) {
    itlp_free_mtph(miph_block->mtphs[i]);
  }
  free(miph_block->mtphs);

  free(miph_block);
}

void itlp_free_msph(struct msph *msph_block) {
  int i;

  for (i = 0; msph_block->mhohs[i]; i++) {
    itlp_free_mhoh(msph_block->mhohs[i]);
  }
  free(msph_block->mhohs);

  free(msph_block);
}

void itlp_free_mprh(struct mprh *mprh_block) {
  free(mprh_block);
}

void itlp_free_mtph(struct mtph *mtph_block) {
  free(mtph_block);
}
