# A primer on *.itl* format

## Introduction

The `.itl` format was invented by *Apple Inc.* for their flagship music player software *iTunes*. The `itl` file format is used as a database for storing all the information about the songs, apps, books, movies, etc. imported into the database, such as meta-data, playlists and much more.

Its format is proprietary and not well documented. Previous attempts at parsing its structure ([titl](https://github.com/josephw/titl) and [meta::cpan's Mac::iTunes::Library::Parse](http://metacpan.org/pod/Mac::iTunes::Library::Parse)) were far from covering the whole database format, and had a wrong understanding of the block structure used by the iTunes library.

The following informations were obtained by analysing itl files. **I do not know how iTunes' itl parser works, so there should be errors here and there in this document.**

## Header block (hdfm) and encryption/compression

All library files begin with an `hdfm` block. The `hdfm` block tells us the iTunes version used to produce this file, how big the file is, and how much of its data is encrypted (we'll talk about that later). All values in the `hdfm` block are big endian.

After the `hdfm` block, the remaining part of the file is compressed (with zlib) then encrypted (only the first 100kB) with AES128-ECB algorithm. All libraries are encrypted using the same 128bits key.

## msdh meta-blocks

Once the file has been decrypted and inflated, the root structure is composed of `msdh` blocks following each other. `msdh` blocks are generic meta-blocks that can hold multiple kinds of blocks (see the `struct msdh` to see which blocks are included by msdh blocks).

## `mith` and `mhoh`

Those two kinds of blocks are embedded in lots of blocks, thus they deserve their own section.

`mith` blocks (parsed as `struct mith` in [types.h](../includes/itlp/types.h)) describes "tracks" which are in your library. This could be songs, apps, books… The mith block by itself stores a little bit of information about the track, and further information about the track is provided by `mhoh` blocks embedded in the `mith` block.

`mhoh` blocks (parsed as `struct mhoh` in [types.h](../includes/itlp/types.h)) contains meta-data for multiple blocks : for instance tracks and playlists. `mhoh` blocks are generic, they only contains the type of metadata informations and its associated value.

## Playlists : `mlph`, `miph`, `mtph`

`mlph` blocks are embedded in `msdh` blocks. What they contains is a list of playlists (`miph`).

`miph` blocks describes a playlist. They contains both `mhoh` and `mtph` blocks. The `mhoh` blocks are informations about the playlist (playlist name, smart playlist criteria -if relevant-).

`mtph` blocks only points to tracks.
