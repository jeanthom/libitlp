/**
 * @file types.h
 * @brief All data types found in iTunes library format.
 * @author Jean THOMAS <itlp@jeanthomas.me>
 */

/**
 * @struct msdh
 * @brief 
 * 
 * Generic meta-block
 */
struct msdh {
  enum {
    BLOCK_MFDH = 0x10,
    BLOCK_MHGH = 0x0C, /**< Contains some settings related to the library */
    BLOCK_MLAH = 0x09,
    BLOCK_MLIH = 0x0B,
    BLOCK_MLTH = 0x01, /**< Master track list */
    BLOCK_MLTH_ALT = 0x0D,
    BLOCK_MLQH = 0x14,
    BLOCK_STSH = 0x17,
    BLOCK_MLPH = 0x02, /**< Playlist list */
    BLOCK_MLPH_ALT = 0x0E,
    BLOCK_MLSH = 0x15,
    BLOCK_FILE = 0x04,
    BLOCK_MLRH = 0x0F
  } type;
  void *subblock;
};

typedef enum {
  TRACK_TITLE = 0x02,
  ALBUM_TITLE = 0x03,
  ARTIST = 0x04,
  GENRE = 0x05,
  KIND = 0x06,
  FILE_TYPE = 0x06,
  COMMENTS = 0x08,
  LOCAL_PATH = 0x0B,
  COMPOSER = 0x0C,
  PLAYLIST_NAME = 0x64,
  SMART_CRITERIA = 0x65,
  SMART_INFO = 0x66,
  LIBRARY_NAME = 0x1FC
} mhoh_type;

/**
 * @struct mhoh
 * @brief Metadata structure
 *
 * mhoh structure hold metadata for multiple types of blocks,
 * for instance mith blocks (tracks). The mhoh structure is
 * generic, and its inner data structure is defined by the
 * mhoh's type.
 */
struct mhoh {
  int type;
  int length;
  char *value;
};

/**
 * @struct mtph
 * @brief Playlist item
 *
 * mtph structure is a playlist item, usually embedded in a miph block.
 */
struct mtph {
  uint32_t identifier; /**< Track identifier of the item */
};

struct miah {
  struct mhoh **mhohs;
};

struct mlah {
  struct miah **miahs;
};

/**
 * @struct mith
 * @brief Track
 * 
 * mith structure holds track information
 */
struct mith {
  uint64_t persistent_id;
  uint32_t id;
  uint32_t add_date;
  uint32_t start_time;
  uint32_t end_time;
  uint32_t playcount;
  uint8_t rating;
  uint8_t unchecked;
  char macos_file_type[4];
  unsigned int mhohs_number;
  struct mhoh **mhohs;
};

/**
 * @struct mhgh
 * @brief Library global settings
 *
 * mhgh holds settings for a library, such as the display size for
 * items. It also has mhoh subblocks for variable length informations
 * (library name for instance). In my test library, embedded mhohs have
 * 0x1F7, 0x1FC, 0x205 types
 */
struct mhgh {
  enum {
    SMALL = 0x01,
    MEDIUM = 0x02,
    LARGE = 0x03
  } list_size;
  struct mhoh **mhohs;
};

struct mfdh {
  char *application_version;
};

/**
 * @struct miph
 * @brief Playlist
 *
 * miph structure holds playlist content and attributes
 */
struct miph {
  uint64_t persistent_id; /**< Playlist persistent ID */
  uint32_t id; /**< Playlist ID */
  uint16_t distinguished_kind;
  struct mhoh **mhohs; /**< Playlist attributes */
  struct mtph **mtphs; /**< Playlist items */
};

/**
 * @struct mlth
 * @brief Master record of tracks
 *
 * Master record of all the tracks (songs, apps, movies, etc.) present in the library.
 */
struct mlth {
  struct mith **miths;
};

struct miih {
  struct mhoh **mhohs;
};

struct mlih {
  struct miih **miihs;
};

struct mprh {
  int todo;
};

struct mlrh {
  struct mprh **mprhs;
};

struct msph {
  struct mhoh **mhohs;
};

struct mlsh {
  struct msph **msphs;
};

/**
 * @struct stsh
 *
 * We don't know anything yet about stsh blocks. **adamish** on GitHub
 * Gists suggested that this could be some sort of signature.
 */
struct stsh {
  int todo;
};

/**
 * @struct miph
 * @brief Playlist list
 *
 * mlph blocks hold multiple playlists (miphs blocks).
 */
struct mlph {
  struct miph **miphs; /**< Playlist array */
};

struct miqh {
  struct mhoh **mhohs;
};

struct mlqh {
  struct miqh **miqhs;
  struct mhoh **mhohs;
};

/**
 * @struct file
 * @brief Absolute path of the Library
 *
 * This struct holds the absolute path of the library.
 * Because file path are also absolute paths in mith blocks
 * this could be really usefull to get relative paths
 */
struct file {
  char *string;
};
