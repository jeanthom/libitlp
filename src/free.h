/**
 * @file free.h
 */

/**
 * @fn void itlp_free_msdh(struct msdh *msdh_block)
 * @param msdh_block msdh block structure to be freed
 * @internal
 */
void itlp_free_msdh(struct msdh *msdh_block);

/**
 * @fn void itlp_free_mfdh(struct mfdh *mfdh_block)
 * @param mfdh_block mfdh block structure to be freed
 * @internal
 */
void itlp_free_mfdh(struct mfdh *mfdh_block);

void itlp_free_mhgh(struct mhgh *mhgh_block);
void itlp_free_mhoh(struct mhoh *mhoh_block);
void itlp_free_mlah(struct mlah *mlah_block);
void itlp_free_miah(struct miah *miah_block);
void itlp_free_mlih(struct mlih *mlih_block);
void itlp_free_miih(struct miih *miih_block);
void itlp_free_mlth(struct mlth *mlth_block);
void itlp_free_mith(struct mith *mith_block);
void itlp_free_mlqh(struct mlqh *mlqh_block);
void itlp_free_stsh(struct stsh *stsh_block);
void itlp_free_mlph(struct mlph *mlph_block);
void itlp_free_mlsh(struct mlsh *mlsh_block);
void itlp_free_file(struct file *file_block);
void itlp_free_mlrh(struct mlrh *mlrh_block);
void itlp_free_miqh(struct miqh *miqh_block);
void itlp_free_miph(struct miph *miph_block);
void itlp_free_msph(struct msph *msph_block);
void itlp_free_mprh(struct mprh *mprh_block);
void itlp_free_mtph(struct mtph *mtph_block);

