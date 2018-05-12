/**
 * @file blocks.h
 * @brief Block parsing functions
 * 
 * This file provides all the functions needed to parse all the
 * blocks present in a `.itl` file.
 */

struct msdh* itlp_parse_msdh(char **msdh_string);
struct mith* itlp_parse_mith(char **mith_string);
struct mhoh* itlp_parse_mhoh(char **mhoh_string);
struct mfdh* itlp_parse_mfdh(char **pos_buffer);
struct miph* itlp_parse_miph(char **pos_buffer);
struct mlth* itlp_parse_mlth(char **mlth_string);
struct mhgh* itlp_parse_mhgh(char **mhgh_string);
struct mlah* itlp_parse_mlah(char **mlah_string);
struct miah* itlp_parse_miah(char **miah_string);
struct mlih* itlp_parse_mlih(char **mlih_string);
struct mlrh* itlp_parse_mlrh(char **mlrh_string);
struct mlsh* itlp_parse_mlsh(char **mlsh_string);
struct stsh* itlp_parse_stsh(char **stsh_string);
struct msph* itlp_parse_msph(char **msph_string);
struct mlph* itlp_parse_mlph(char **mlph_string);
struct mtph* itlp_parse_mtph(char **mtph_string);
struct miih* itlp_parse_miih(char **miih_string);
struct mprh* itlp_parse_mprh(char **mprh_string);
struct mlqh* itlp_parse_mlqh(char **mlqh_string);
struct miqh* itlp_parse_miqh(char **miqh_string);
struct file* itlp_parse_file(char **file_string);
