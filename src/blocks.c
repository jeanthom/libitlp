#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "itlp/types.h"
#include "memtools.h"
#include "blocks.h"

struct msdh* itlp_parse_msdh(char **msdh_string) {
  struct msdh *msdh_block;
  uint32_t msdh_header_length;

  /* Allocating memory for the msdh block */
  msdh_block = malloc(sizeof(struct msdh));
  if (!msdh_block)
    return NULL;

  /* Reading header */
  msdh_header_length = read_uint32t_offset(*msdh_string,4);
  msdh_block->type = read_uint32t_offset(*msdh_string,12);

  /* Skipping header section */
  *msdh_string += msdh_header_length;

  /* Parse msdh's subblock */
  switch (msdh_block->type) {
  case BLOCK_MLIH:
    msdh_block->subblock = (void*)itlp_parse_mlih(msdh_string);
    break;
  case BLOCK_MLTH_ALT:
  case BLOCK_MLTH:
    msdh_block->subblock = (void*)itlp_parse_mlth(msdh_string);
    break;
  case BLOCK_MFDH:
    msdh_block->subblock = (void*)itlp_parse_mfdh(msdh_string);
    break;
  case BLOCK_MLRH:
    msdh_block->subblock = (void*)itlp_parse_mlrh(msdh_string);
    break;
  case BLOCK_MLAH:
    msdh_block->subblock = (void*)itlp_parse_mlah(msdh_string);
    break;
  case BLOCK_MLSH:
    msdh_block->subblock = (void*)itlp_parse_mlsh(msdh_string);
    break;
  case BLOCK_MHGH:
    msdh_block->subblock = (void*)itlp_parse_mhgh(msdh_string);
    break;
  case BLOCK_STSH:
    msdh_block->subblock = (void*)itlp_parse_stsh(msdh_string);
    break;
  case BLOCK_MLPH_ALT:
  case BLOCK_MLPH:
    msdh_block->subblock = (void*)itlp_parse_mlph(msdh_string);
    break;
  case BLOCK_MLQH:
    msdh_block->subblock = (void*)itlp_parse_mlqh(msdh_string);
    break;
  case BLOCK_FILE:
    *msdh_string -= msdh_header_length;
    msdh_block->subblock = (void*)itlp_parse_file(msdh_string);
    break;
  default:
    return NULL;
    break;
  }
  
  return msdh_block;
}

struct mith* itlp_parse_mith(char **mith_string) {
  struct mith *mith_block;
  struct mhoh *current_mhoh;
  size_t header_length;
  unsigned int i, mhohs_number;

  /* Memory allocation for mith block */
  mith_block = malloc(sizeof(struct mith));
  if (!mith_block)
    return NULL;

  /* Reading various mith informations */
  header_length = read_uint32t_offset(*mith_string, 4);
  mhohs_number = read_uint32t_offset(*mith_string, 12);

  /* Reading mith metadata */
  mith_block->rating = read_uint8t_offset(*mith_string, 108);
  mith_block->id = read_uint32t_offset(*mith_string, 16);
  mith_block->playcount = read_uint32t_offset(*mith_string, 0x4C);
  mith_block->unchecked = read_uint8t_offset(*mith_string, 0x6E);
  mith_block->add_date = read_uint32t_offset(*mith_string, 0x78);
  mith_block->persistent_id = read_uint64t_offset(*mith_string, 0x80);

  /* Persistent ID at 0x80 */
  /* Track number at 0x10 */

  /* Skipping header */
  *mith_string += header_length;

  /* Memory allocation for mhoh block embedded in mith */
  mith_block->mhohs = malloc((mhohs_number+1)*sizeof(struct mhoh*));
  if (!mith_block->mhohs)
    return NULL;

  /* Parsing embedded mhoh in mith */
  for (i = 0; i < mhohs_number; i++) {
    current_mhoh = itlp_parse_mhoh(mith_string);
    if (!current_mhoh)
      return NULL;
    
    mith_block->mhohs[i] = current_mhoh;
  }

  /* NULL-ending the array */
  mith_block->mhohs[mhohs_number] = NULL;
  
  return mith_block;
}

struct mhoh* itlp_parse_mhoh(char **mhoh_string) {
  struct mhoh *block;
  size_t mhoh_value_size;
  uint32_t record_length;

  /* Allocating memory for mhoh structure */
  block = malloc(sizeof(struct mhoh));
  if (!block)
    return NULL;

  /* Reading header */
  record_length = read_uint32t_offset(*mhoh_string,8);

  /* Reading mhoh's type and value size */
  block->type = read_uint32t_offset(*mhoh_string,12);
  
  if (block->type == 0x1fb || block->type == 0x32 ||
      block->type == 0x2bc || block->type == 0x30 ||
      block->type == 0x36 || block->type == 0x13 ||
      block->type == 0x1F7 || block->type == 0x205 ||
      block->type == 0x192 || block->type == 0x1 ||
      block->type == 0x69 || block->type == 0x6C ||
      block->type == 0x66 || block->type == 0x65 ||
      block->type == 0x6D || block->type== 0x67 ||
      block->type == 0x320) {
    mhoh_value_size = record_length - read_uint32t_offset(*mhoh_string,4);
    *mhoh_string += read_uint32t_offset(*mhoh_string,4);
  } else {
    *mhoh_string += 28;
    mhoh_value_size = read_uint32t_offset(*mhoh_string,0);
    *mhoh_string += 12;
  }
  
  /* Allocating memory for mhoh value and copy it */
  block->value = malloc(mhoh_value_size);
  if (block->value == NULL)
    return NULL;
  
  block->length = mhoh_value_size;
  memcpy(block->value, *mhoh_string, mhoh_value_size);
  *mhoh_string += mhoh_value_size;

  return block;
}

struct mfdh* itlp_parse_mfdh(char **pos_buffer) {
  struct mfdh *block;
  size_t version_string_len, header_length;

  block = malloc(sizeof(struct mfdh));
  if (!block)
    return NULL;

  header_length = read_uint32t_offset(*pos_buffer, 4);

  version_string_len = read_uint8t_offset(*pos_buffer, 13);
  block->application_version = malloc(sizeof(char)*(version_string_len+1));
  memcpy(block->application_version, *pos_buffer+14, version_string_len);
  block->application_version[version_string_len] = 0;

  *pos_buffer += header_length;

  return block;
}

struct miph* itlp_parse_miph(char **pos_buffer) {
  struct miph *block;
  size_t header_size;
  uint32_t mhoh_number, mtph_number;
  unsigned int i;
  
  block = malloc(sizeof(struct miph));
  if (!block)
    return NULL;

  /* Reading header informations */
  header_size = read_uint32t_offset(*pos_buffer, 4);
  mhoh_number = read_uint32t_offset(*pos_buffer, 12);
  mtph_number = read_uint32t_offset(*pos_buffer, 16);
  block->id = read_uint32t_offset(*pos_buffer, 0xD40);

  /* Playlist persistent ID at 0x1B8, 64 bits */
  /* Distinguished kind at 0x23A (maybe less than uint32_t) */

  *pos_buffer += header_size;

  /* Allocating memory for mhoh blocks and mtph blocks */
  block->mhohs = malloc((mhoh_number+1)*sizeof(struct mhoh*));
  block->mtphs = malloc((mtph_number+1)*sizeof(struct mtph*));
  
  /* Ending arrays with NULL */
  block->mhohs[mhoh_number] = NULL;
  block->mtphs[mtph_number] = NULL;

  /* Parsing mhoh blocks */
  for (i = 0; i < mhoh_number; i++) {
    block->mhohs[i] = itlp_parse_mhoh(pos_buffer);
  }

  /* Parsing mtph blocks */
  for (i = 0; i < mtph_number; i++) {
    block->mtphs[i] = itlp_parse_mtph(pos_buffer);
  }

  return block;
}

struct mtph* itlp_parse_mtph(char **mtph_string) {
  struct mtph* block;
  size_t block_size;

  block = malloc(sizeof(struct mtph));
  if (!block)
    return NULL;

  /* Reading header informations */
  block_size = read_uint32t_offset(*mtph_string,4);
  
  block->identifier = read_uint32t_offset(*mtph_string,24);

  *mtph_string += block_size;
  
  return block;
}

struct mlth* itlp_parse_mlth(char **mlth_string) {
  struct mlth* block;
  size_t header_length;
  uint32_t mith_number;
  unsigned int i;

  /* Parsing mlth header */
  header_length = read_uint32t_offset(*mlth_string, 4);
  mith_number = read_uint32t_offset(*mlth_string,8);
  *mlth_string += header_length;

  /* Allocating memory */
  block = malloc(sizeof(struct mlth));
  if (!block)
    return NULL;
  block->miths = malloc((mith_number+1)*sizeof(struct mith*));

  /* Parsing mith blocks */
  for (i = 0; i < mith_number; i++) {
    block->miths[i] = itlp_parse_mith(mlth_string);
  }

  /* Ending array with NULL */
  block->miths[i] = NULL;
  
  return block;
}

struct mhgh* itlp_parse_mhgh(char **mhgh_string) {
  struct mhgh* block;
  size_t header_length;
  uint32_t mhoh_number;
  unsigned int i;

  /* Allocating memory */
  block = malloc(sizeof(struct mhgh));
  if (!block)
    return NULL;
  
  /* Reading header informations */
  header_length = read_uint32t_offset(*mhgh_string, 4);
  mhoh_number = read_uint32t_offset(*mhgh_string, 8);
  block->list_size = read_uint8t_offset(*mhgh_string, 0x37);
  *mhgh_string += header_length;
  
  block->mhohs = malloc((mhoh_number+1)*sizeof(struct mhoh*));

  /* Parsing mhoh blocks */
  for (i = 0; i < mhoh_number; i++) {
    block->mhohs[i] = itlp_parse_mhoh(mhgh_string);
  }

  /* Ending array with NULL */
  block->mhohs[mhoh_number] = NULL;
  
  return block;
}

/* We don't know yet what is the point of stsh.
 * adamish (github.com/adamish) suggested on a gist that this
 * could be some sort of a signature
 */
struct stsh* itlp_parse_stsh(char **stsh_string) {
  struct stsh* block;
  size_t header_length;

  block = malloc(sizeof(struct stsh));
  if (!block)
    return NULL;

  header_length = read_uint32t_offset(*stsh_string, 4);
  *stsh_string += header_length;
  
  return block;
}

struct mlah* itlp_parse_mlah(char **mlah_string) {
  struct mlah* block;
  size_t header_length;
  uint32_t miah_number;
  unsigned int i;
  
  block = malloc(sizeof(struct mlah));
  if (!block)
    return NULL;

  /* Read mlah header */
  header_length = read_uint32t_offset(*mlah_string, 4);
  miah_number = read_uint32t_offset(*mlah_string, 8);
  *mlah_string += header_length;

  /* Allocating memory for miah blocks */
  block->miahs = malloc((miah_number+1)*sizeof(struct miah*));
  if (!block->miahs)
    return NULL;

  /* Ending array with NULL */
  block->miahs[miah_number] = NULL;

  /* Parsing miah blocks */
  for (i = 0; i < miah_number; i++) {
    block->miahs[i] = itlp_parse_miah(mlah_string);
    if (!block->miahs[i])
      return NULL;
  }

  return block;
}

struct miah* itlp_parse_miah(char **miah_string) {
  struct miah* block;
  size_t header_length;
  uint32_t mhoh_number;
  unsigned int i;

  block = malloc(sizeof(struct miah));
  if (!block)
    return NULL;

  /* Read miah header */
  header_length = read_uint32t_offset(*miah_string, 4);
  mhoh_number = read_uint32t_offset(*miah_string, 12);
  *miah_string += header_length;

  /* Allocating memory for mhoh blocks */
  block->mhohs = malloc((mhoh_number+1)*sizeof(struct mhoh*));
  if (!block->mhohs)
    return NULL;

  /* Ending array with NULL */
  block->mhohs[mhoh_number] = NULL;
  
  /* Parsing mhoh blocks */
  for (i = 0; i < mhoh_number; i++) {
    block->mhohs[i] = itlp_parse_mhoh(miah_string);
    if (!block->mhohs[i])
      return NULL;
  }
  
  return block;
}

struct mlih* itlp_parse_mlih(char **mlih_string) {
  struct mlih *block;
  size_t header_length;
  uint32_t miih_number;
  unsigned int i;

  /* Reading header informations */
  header_length = read_uint32t_offset(*mlih_string, 4);
  miih_number = read_uint32t_offset(*mlih_string,8);
  *mlih_string += header_length;

  /* Allocating memory */
  block = malloc(sizeof(struct mlih));
  if (!block)
    return NULL;
  block->miihs = malloc((miih_number+1)*sizeof(struct miih*));

  /* Parsing miih blocks */
  for (i = 0; i < miih_number; i++) {
    block->miihs[i] = itlp_parse_miih(mlih_string);
  }

  /* Ending the array with NULL */
  block->miihs[miih_number] = NULL;

  return block;
}

struct miih* itlp_parse_miih(char **miih_string) {
  struct miih *block;
  size_t header_length;
  uint32_t mhoh_number;
  unsigned int i;

  /* Reading header informations */
  header_length = read_uint32t_offset(*miih_string, 4);
  mhoh_number = read_uint32t_offset(*miih_string, 12);
  *miih_string += header_length;

  /* Allocating memory */
  block = malloc(sizeof(struct miih));
  if (!block)
    return NULL;
  block->mhohs = malloc((mhoh_number+1)*sizeof(struct mhoh*));

  /* Parsing mhoh blocks */
  for (i = 0; i < mhoh_number; i++) {
    block->mhohs[i] = itlp_parse_mhoh(miih_string);
  }

  /* Ending array with NULL */
  block->mhohs[mhoh_number] = NULL;

  return block;
}

struct mlrh* itlp_parse_mlrh(char **mlrh_string) {
  struct mlrh *block;
  size_t header_length;
  uint32_t mprh_number;
  unsigned int i;

  /* Reading header informations */
  header_length = read_uint32t_offset(*mlrh_string, 4);
  mprh_number   = read_uint32t_offset(*mlrh_string, 8);
  *mlrh_string += header_length;

  /* Allocating memory */
  block = malloc(sizeof(struct mlrh));
  if (!block)
    return NULL;
  block->mprhs = malloc((mprh_number+1)*sizeof(struct mprh*));

  /* Parsing mprh blocks */
  for (i = 0; i < mprh_number; i++) {
    block->mprhs[i] = itlp_parse_mprh(mlrh_string);
  }

  /* Ending array with NULL */
  block->mprhs[mprh_number] = NULL;

  return block;
}

struct mlsh* itlp_parse_mlsh(char **mlsh_string) {
  struct mlsh *block;
  size_t header_length;
  uint32_t msph_number;
  unsigned int i;

  /* Reading header informations */
  header_length = read_uint32t_offset(*mlsh_string, 4);
  msph_number = read_uint32t_offset(*mlsh_string, 8);

  /* Allocating memory */
  block = malloc(sizeof(struct mlsh));
  if (!block)
    return NULL;
  block->msphs = malloc((msph_number+1)*sizeof(struct msph*));

  *mlsh_string += header_length;

  /* Parsing msph blocks */
  for (i = 0; i < msph_number; i++) {
    block->msphs[i] = itlp_parse_msph(mlsh_string);
  }

  /* Ending array with NULL */
  block->msphs[msph_number] = NULL;

  return block;
}

struct msph* itlp_parse_msph(char **msph_string) {
  struct msph *block;
  size_t header_length;
  uint32_t mhoh_number;
  unsigned int i;

  /* Reading header informations */
  header_length = read_uint32t_offset(*msph_string, 4);
  mhoh_number = read_uint32t_offset(*msph_string, 12);
  *msph_string += header_length;

  /* Allocating memory */
  block = malloc(sizeof(struct msph));
  if (!block)
    return NULL;
  block->mhohs = malloc((mhoh_number+1)*sizeof(struct mhoh*));

  /* Parsing mhoh blocks */
  for (i = 0; i < mhoh_number; i++) {
    block->mhohs[i] = itlp_parse_mhoh(msph_string);
  }

  /* Ending array with NULL */
  block->mhohs[mhoh_number] = NULL;

  return block;
}

struct mlph* itlp_parse_mlph(char **mlph_string) {
  struct mlph *block;
  size_t header_length;
  uint32_t miph_number;
  unsigned int i;

  /* Reading header informations */
  header_length = read_uint32t_offset(*mlph_string, 4);
  miph_number = read_uint32t_offset(*mlph_string, 8);
  
  *mlph_string += header_length;

  /* Allocating memory */
  block = malloc(sizeof(struct mlph));
  if (!block)
    return NULL;
  block->miphs = malloc((miph_number+1)*sizeof(struct miph*));

  /* Parsing miph blocks */
  for (i = 0; i < miph_number; i++) {
    block->miphs[i] = itlp_parse_miph(mlph_string);
  }

  /* Ending array with NULL */
  block->miphs[miph_number] = NULL;

  return block;
}

struct mprh* itlp_parse_mprh(char **mprh_string) {
  struct mprh *block;
  size_t header_length;

  /* Allocating memory */
  block = malloc(sizeof(struct mprh));
  if (!block)
    return NULL;

  /* Reading header informaations */
  header_length = read_uint32t_offset(*mprh_string, 4);
  *mprh_string += header_length;

  return block;
}

struct mlqh* itlp_parse_mlqh(char **mlqh_string) {
  struct mlqh *block;
  size_t header_length;
  uint32_t miqh_number, mhoh_number;
  unsigned int i;

  /* Reading header informations */
  header_length = read_uint32t_offset(*mlqh_string, 4);
  mhoh_number = read_uint32t_offset(*mlqh_string, 12);
  miqh_number = read_uint32t_offset(*mlqh_string, 16);
  *mlqh_string += header_length;

  /* Allocating memory */
  block = malloc(sizeof(struct mlqh));
  if (!block)
    return NULL;
  block->mhohs = malloc((mhoh_number+1)*sizeof(struct mhoh*));
  block->miqhs = malloc((miqh_number+1)*sizeof(struct miqh*));

  /* Reading mhoh and miqh blocks */
  for (i = 0; i < mhoh_number; i++) {
    block->mhohs[i] = itlp_parse_mhoh(mlqh_string);
  }
  for (i = 0; i < miqh_number; i++) {
    block->miqhs[i] = itlp_parse_miqh(mlqh_string);
  }

  /* Ending array with NULL */
  block->mhohs[mhoh_number] = NULL;
  block->miqhs[miqh_number] = NULL;

  return block;
}

struct file* itlp_parse_file(char **file_string) {
  struct file *block;
  size_t header_length, string_length;

  /* Parsing header informations */
  header_length = read_uint32t_offset(*file_string, 4);
  string_length = read_uint32t_offset(*file_string, 8) - header_length;;

  *file_string += header_length;

  /* Allocating memory */
  block = malloc(sizeof(struct file));
  block->string = malloc((string_length+1)*sizeof(char));

  /* Parsing block */
  memcpy(block->string, *file_string, string_length);
  block->string[string_length] = 0;

  *file_string += string_length;
  
  return block;
}

struct miqh* itlp_parse_miqh(char **miqh_string) {
  struct miqh *block;
  size_t header_length;
  uint32_t mhoh_number;
  unsigned int i;

  /* Reading header informations */
  header_length = read_uint32t_offset(*miqh_string, 4);
  mhoh_number = read_uint32t_offset(*miqh_string, 12);
  *miqh_string += header_length;

  /* Allocating memory */
  block = malloc(sizeof(struct miqh));
  if (!block)
    return NULL;
  block->mhohs = malloc((mhoh_number+1)*sizeof(struct mhoh*));

  /* Parsing mhoh blocks */
  for (i = 0; i < mhoh_number; i++) {
    block->mhohs[i] = itlp_parse_mhoh(miqh_string);
  }

  /* Ending array with NULL */
  block->mhohs[mhoh_number] = NULL;

  return block;
}
