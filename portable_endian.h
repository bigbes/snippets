#ifndef __PORTABLE_ENDIAN_H__
#define __PORTABLE_ENDIAN_H__

#if defined(__linux__) || defined(__CYGWIN__)
#  include <endian.h>
#  define betoh16(x) be16toh(x)
#  define letoh16(x) le16toh(x)
#  define betoh32(x) be32toh(x)
#  define letoh32(x) le32toh(x)
#  define betoh64(x) be64toh(x)
#  define letoh64(x) le64toh(x)
#elif defined(__APPLE__)
#  include <libkern/OSByteOrder.h>
#  define htobe16(x) OSSwapHostToBigInt16(x)
#  define htole16(x) OSSwapHostToLittleInt16(x)
#  define betoh16(x) OSSwapBigToHostInt16(x)
#  define letoh16(x) OSSwapLittleToHostInt16(x)
#  define htobe32(x) OSSwapHostToBigInt32(x)
#  define htole32(x) OSSwapHostToLittleInt32(x)
#  define betoh32(x) OSSwapBigToHostInt32(x)
#  define letoh32(x) OSSwapLittleToHostInt32(x)
#  define htobe64(x) OSSwapHostToBigInt64(x)
#  define htole64(x) OSSwapHostToLittleInt64(x)
#  define betoh64(x) OSSwapBigToHostInt64(x)
#  define letoh64(x) OSSwapLittleToHostInt64(x)
#  define __BYTE_ORDER    BYTE_ORDER
#  define __BIG_ENDIAN    BIG_ENDIAN
#  define __LITTLE_ENDIAN LITTLE_ENDIAN
#  define __PDP_ENDIAN    PDP_ENDIAN
#elif defined(__OpenBSD__)
#  include <sys/endian.h>
#  define betoh16(x) be16toh(x)
#  define letoh16(x) le16toh(x)
#  define betoh32(x) be32toh(x)
#  define letoh32(x) le32toh(x)
#  define betoh64(x) be64toh(x)
#  define letoh64(x) le64toh(x)
#elif defined(__NetBSD__) || defined(__FreeBSD__) || defined(__DragonFly__)
#  include <sys/endian.h>
#elif (defined(_WIN16) || defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__))
#  include <winsock2.h>
#  include <sys/param.h>
#  if BYTE_ORDER == LITTLE_ENDIAN
#    define htobe16(x) htons(x)
#    define htole16(x) (x)
#    define betoh16(x) ntohs(x)
#    define letoh16(x) (x)
#    define htobe32(x) htonl(x)
#    define htole32(x) (x)
#    define betoh32(x) ntohl(x)
#    define letoh32(x) (x)
#    define htobe64(x) htonll(x)
#    define htole64(x) (x)
#    define betoh64(x) ntohll(x)
#    define letoh64(x) (x)
#  elif BYTE_ORDER == BIG_ENDIAN
#    define htobe16(x) (x)
#    define htole16(x) __builtin_bswap16(x)
#    define betoh16(x) (x)
#    define letoh16(x) __builtin_bswap16(x)
#    define htobe32(x) (x)
#    define htole32(x) __builtin_bswap32(x)
#    define betoh32(x) (x)
#    define letoh32(x) __builtin_bswap32(x)
#    define htobe64(x) (x)
#    define htole64(x) __builtin_bswap64(x)
#    define betoh64(x) (x)
#    define letoh64(x) __builtin_bswap64(x)
#  else
#    error byte order not supported
#  endif
#  define __BYTE_ORDER    BYTE_ORDER
#  define __BIG_ENDIAN    BIG_ENDIAN
#  define __LITTLE_ENDIAN LITTLE_ENDIAN
#  define __PDP_ENDIAN    PDP_ENDIAN
#else
#  error platform not supported
#endif

#endif  /* __PORTABLE_ENDIAN_H__ */
