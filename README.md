# libitlp

itlp (aka. **itl Parser/iTunes Library Parser**) is a parser written in C for `.itl` files (iTunes database) produced by *Apple iTunes*. Its goal is to provide more accurate parsing than other third-party software available on internet (such as [titl](https://github.com/josephw/titl) or [meta::cpan's Mac::iTunes::Library::Parse](http://metacpan.org/pod/Mac::iTunes::Library::Parse)), but it is also lower level than those libraries : libitlp provides direct access to the structure of the itl file, not an edulcorated representation which would be easier to work with.

I had this code lying around for months, so I decided to publish it.

## What to know

 * libitlp is not portable : it should works okay on Linux & Mac, but don't expect it to work on big-endian system
 * read-only
 * the parsing is far from being accurate and misses a ton of things (like album artwork)

## Documentation

Some documentation about libitlp is avaiable in the [doc/](doc/) folder. Some example programs using libitlp are available in [examples/](examples/) folder to better understand how libitlp should be used.

All the functions in libitlp were documented with Doxygen comments.

## Dependencies

libitlp relies on the following software :

 * OpenSSL-devel (may not be necessary on macOS, see the note in "Compilation/Installation")
 * Zlib-devel
 * CMake (>= 2.8)

### Debian

The required dependencies can be installed from the usual Debian repositories using apt-get :

```
apt-get install cmake libssl-dev zlib1g-dev build-essential
```

## Decryption key

A decryption key is needed in order to decrypt `iTunes Library.itl` file. I'm not sure whether it is legal or not to republish it on my GitHub repository, so bring your own (**and please do not attempt to publish it here**).

## Compilation/Installation

libitlp can be compiled using the following commands :

```
mkdir build && cd build
cmake -DDECRYPT_KEY="ABCDEF1234567890" ..
make
sudo make install
```

The default parameters for cmake compiles libitlp as a shared library, but it can also be built as static :

```
mkdir build && cd build
cmake -DDECRYPT_KEY="ABCDEF1234567890" -DBUILD_SHARED_LIBS=OFF ..
make
```

The Doxygen documentation can be built using the `doxygen` tool in libitlp's root folder. libitlp's specifies that the documentation should be built in `build/` folder.

**Notes on compiling on macOS :** As of macOS ≥ 10.7, OpenSSL library has been deprecated in favor of Apple's CommonCrypto library. If you're trying to compile for older versions of macOS, you'll need to use the following cmake command to force using OpenSSL :

```
cmake -DDECRYPT_KEY="ABCDEF1234567890" -DFORCE_OPENSSL=ON
```

## Legal

Apple and iTunes are trademarks of Apple Inc., registered in the U.S. and other countries. libiltp is not affiliated with Apple Inc. in any way.
