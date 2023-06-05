// BMP-related data types based on Microsoft's own

#include <stdint.h>

/**
 * Common Data Types
 *
 * The data types in this section are essentially aliases for C/C++
 * primitive data types.
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/cc230309.aspx.
 * See http://en.wikipedia.org/wiki/Stdint.h for more on stdint.h.
 */

// Definição de tipos de dados com base nos tipos fornecidos pela Microsoft

// Aliases para tipos de dados comuns
typedef uint8_t BYTE;   // 1 byte
typedef uint32_t DWORD; // 4 bytes
typedef int32_t LONG;   // 4 bytes
typedef uint16_t WORD;  // 2 bytes

/**
 * BITMAPFILEHEADER
 *
 * The BITMAPFILEHEADER structure contains information about the type, size,
 * and layout of a file that contains a DIB [device-independent bitmap].
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/dd183374(VS.85).aspx.
 */

// Estrutura BITMAPFILEHEADER
typedef struct
{
    WORD bfType;      // Tipo do arquivo (deve ser "BM")
    DWORD bfSize;     // Tamanho do arquivo em bytes
    WORD bfReserved1; // Reservado (deve ser 0)
    WORD bfReserved2; // Reservado (deve ser 0)
    DWORD bfOffBits;  // Deslocamento para a área de dados da imagem
} __attribute__((__packed__))
BITMAPFILEHEADER;

/**
 * BITMAPINFOHEADER
 *
 * The BITMAPINFOHEADER structure contains information about the
 * dimensions and color format of a DIB [device-independent bitmap].
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/dd183376(VS.85).aspx.
 */

// Estrutura BITMAPINFOHEADER
typedef struct
{
    DWORD biSize;         // Tamanho da estrutura (deve ser 40)
    LONG biWidth;         // Largura da imagem em pixels
    LONG biHeight;        // Altura da imagem em pixels
    WORD biPlanes;        // Número de planos de cor (deve ser 1)
    WORD biBitCount;      // Número de bits por pixel
    DWORD biCompression;  // Tipo de compressão utilizado
    DWORD biSizeImage;    // Tamanho da imagem em bytes
    LONG biXPelsPerMeter; // Resolução horizontal em pixels por metro
    LONG biYPelsPerMeter; // Resolução vertical em pixels por metro
    DWORD biClrUsed;      // Número de cores na tabela de cores (0 = máximo)
    DWORD biClrImportant; // Número de cores importantes (0 = todas)
} __attribute__((__packed__))
BITMAPINFOHEADER;

/**
 * RGBTRIPLE
 *
 * This structure describes a color consisting of relative intensities of
 * red, green, and blue.
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/aa922590.aspx.
 */

// Estrutura RGBTRIPLE
typedef struct
{
    BYTE rgbtBlue;  // Intensidade de cor azul
    BYTE rgbtGreen; // Intensidade de cor verde
    BYTE rgbtRed;   // Intensidade de cor vermelha
} __attribute__((__packed__))
RGBTRIPLE;

// O atributo __attribute__((__packed__)) é usado para garantir que a estrutura seja compacta,
// sem espaços extras de preenchimento. Isso significa que os campos da estrutura serão
// armazenados em sequência, ocupando o menor espaço possível na memória.