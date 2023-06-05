#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"

int main(int argc, char *argv[])
{
    // Define as opções de filtro permitidas
    char *filterOptions = "begr";

    // getopt Obtém a opção de filtro selecionada a partir dos argumentos de linha de comando
    char selectedFilter = getopt(argc, argv, filterOptions);
    if (selectedFilter == '?') // retorna ? caso o argumento passado for inválido (não esteja em filterOptions )
    {
        fprintf(stderr, "Filtro inválido.\n");
        return 1;
    }

    // Verifica se foi selecionado apenas um filtro
    if (getopt(argc, argv, filterOptions) != -1) // verifica se ainda existem argumentos não processados
    {
        fprintf(stderr, "Apenas um filtro é permitido.\n");
        return 2;
    }

    // Verifica se a quantidade de argumentos está correta
    if (argc != optind + 2) // optind é uma variável global definida pela função getopt
    {
        fprintf(stderr, "Uso: filter [filtro] arquivo_entrada arquivo_saida\n");
        return 3;
    }

    // Obtém os nomes dos arquivos de entrada e saída
    char *inputFile = argv[optind];
    char *outputFile = argv[optind + 1];

    // Abre o arquivo de entrada
    FILE *inputPtr = fopen(inputFile, "r"); // read "modo leitura"
    if (!inputPtr)
    {
        fprintf(stderr, "Não foi possível abrir o arquivo %s.\n", inputFile);
        return 4;
    }

    // Abre o arquivo de saída
    FILE *outputPtr = fopen(outputFile, "w"); // write "modo escrever"
    if (!outputPtr)
    {
        fclose(inputPtr); // se o arquivo não existir ele cria então provavelmente nunca vai cair aqui
        fprintf(stderr, "Não foi possível criar o arquivo %s.\n", outputFile);
        return 5;
    }

    // Lê o cabeçalho do arquivo de entrada (BITMAPFILEHEADER)
    BITMAPFILEHEADER bitmapFileHeader;
    fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, inputPtr);

    /*
        O BITMAPFILEHEADER é o cabeçalho do arquivo BMP.
        Ele armazena informações gerais sobre o arquivo BMP, como o tipo de arquivo ("BM"),
        o tamanho do arquivo, o deslocamento para a área de dados da imagem e outros campos.
    */

    // Lê o cabeçalho do arquivo de entrada (BITMAPINFOHEADER)
    BITMAPINFOHEADER bitmapInfoHeader;
    fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, inputPtr);

    /*
        O BITMAPINFOHEADER é o cabeçalho do formato da imagem BMP.
        Ele contém informações sobre as propriedades da imagem, como as dimensões (largura e altura),
        o número de planos de cor, o número de bits por pixel, o tipo de compressão utilizado,
        o tamanho da imagem em bytes e outras informações relacionadas à resolução e à tabela de cores.
    */

    // A função fread é usada para ler um bloco de bytes do arquivo especificado e armazená-lo na memória.
    // O primeiro argumento é o endereço de memória onde os dados lidos serão armazenados.
    // O segundo argumento indica o tamanho, em bytes, que deve ser lido do arquivo.
    // O terceiro argumento é o número de elementos a serem lidos, neste caso, apenas um único elemento do tipo BITMAPFILEHEADER.
    // O último argumento é um ponteiro para o arquivo de entrada, que deve ser aberto previamente usando a função fopen.
    // Após a execução dessa linha, o cabeçalho do arquivo de entrada será armazenado na variável bitmapFileHeader.
    // O mesmo serve para BITMAPINFOHEADER

    // Verifica se o arquivo de entrada é um BMP de 24 bits não compactado
    if (bitmapFileHeader.bfType != 0x4d42 || bitmapFileHeader.bfOffBits != 54 ||
        bitmapInfoHeader.biSize != 40 || bitmapInfoHeader.biBitCount != 24 ||
        bitmapInfoHeader.biCompression != 0)
    {
        fclose(outputPtr);
        fclose(inputPtr);
        fprintf(stderr, "Formato de arquivo não suportado.\n");
        return 6;
    }

    // Armazenando largura e altura da imagem
    int imageHeight = abs(bitmapInfoHeader.biHeight);
    int imageWidth = bitmapInfoHeader.biWidth;

    // Aloca memória para a imagem
    // Declara um ponteiro chamado 'image' que aponta para um array bidimensional de 'RGBTRIPLE'
    // A função 'calloc' é usada para alocar memória para armazenar a imagem
    // Ela recebe o número de linhas (imageHeight) e o tamanho de cada linha (imageWidth * sizeof(RGBTRIPLE))
    // O resultado da alocação de memória é armazenado no ponteiro 'image'
    RGBTRIPLE(*image)
    [imageWidth] = calloc(imageHeight, imageWidth * sizeof(RGBTRIPLE));

    // Verifica se a alocação de memória foi bem-sucedida
    // Se 'image' for NULL, significa que não há memória suficiente disponível para alocar o array
    // Nesse caso, exibe uma mensagem de erro, fecha os arquivos de entrada e saída e retorna o código de erro 7
    if (!image)
    {
        fprintf(stderr, "Não há memória suficiente para armazenar a imagem.\n");
        fclose(outputPtr);
        fclose(inputPtr);
        return 7;
    }

    // Calcula o preenchimento necessário para as linhas de pixels
    // O preenchimento é usado em imagens BMP para garantir que cada linha tenha um tamanho múltiplo de 4 bytes
    // A fórmula utilizada aqui calcula a quantidade de bytes de preenchimento necessário
    // O valor resultante será 0, 1, 2 ou 3, dependendo do tamanho da linha em relação a múltiplos de 4
    int padding = (4 - (imageWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // Lê as linhas de pixels do arquivo de entrada
    // Este laço percorre cada linha da imagem
    for (int i = 0; i < imageHeight; i++)
    {
        // Lê uma linha de pixels e armazena no array de pixels da imagem
        // A função 'fread' é usada para ler uma quantidade especificada de elementos (neste caso, pixels) de um arquivo
        // O tamanho de cada pixel é determinado por 'sizeof(RGBTRIPLE)'
        // Os pixels lidos são armazenados na linha 'image[i]'
        // O número total de pixels a serem lidos é 'imageWidth'
        // O arquivo de entrada 'inputPtr' é usado como origem dos dados
        fread(image[i], sizeof(RGBTRIPLE), imageWidth, inputPtr);

        // Pula o preenchimento da linha
        // O preenchimento é pulado utilizando a função 'fseek'
        // A posição do arquivo é deslocada em 'padding' bytes a partir da posição atual (SEEK_CUR)
        // Isso move o indicador de posição do arquivo para a próxima linha
        fseek(inputPtr, padding, SEEK_CUR);
    }

    // Aplica o filtro selecionado na imagem
    switch (selectedFilter)
    {
    case 'b':
        blur(imageHeight, imageWidth, image);
        break;

    case 'e':
        edges(imageHeight, imageWidth, image);
        break;

    case 'g':
        grayscale(imageHeight, imageWidth, image);
        break;

    case 'r':
        reflect(imageHeight, imageWidth, image);
        break;
    }

    // Escreve o cabeçalho do arquivo de saída (BITMAPFILEHEADER)
    // A função 'fwrite' é usada para escrever uma quantidade especificada de bytes em um arquivo
    // O cabeçalho do arquivo bitmap (BITMAPFILEHEADER) é escrito no arquivo de saída (outputPtr)
    fwrite(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, outputPtr);

    // Escreve o cabeçalho do arquivo de saída (BITMAPINFOHEADER)
    // O cabeçalho de informações do bitmap (BITMAPINFOHEADER) é escrito no arquivo de saída (outputPtr)
    fwrite(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, outputPtr);

    // Escreve os pixels processados no arquivo de saída
    // Este laço percorre cada linha da imagem
    for (int i = 0; i < imageHeight; i++)
    {
        // Escreve uma linha de pixels no arquivo de saída
        // Os pixels da linha 'image[i]' são escritos no arquivo de saída (outputPtr)
        // O número total de pixels a serem escritos é 'imageWidth'
        // Cada pixel tem um tamanho de 'sizeof(RGBTRIPLE)' bytes
        fwrite(image[i], sizeof(RGBTRIPLE), imageWidth, outputPtr);

        // Adiciona o preenchimento ao final da linha
        // Este laço é usado para escrever os bytes de preenchimento após a linha de pixels
        // O valor de 'padding' indica a quantidade de bytes de preenchimento a serem escritos
        for (int k = 0; k < padding; k++)
        {
            // Escreve um byte de preenchimento (valor 0x00) no arquivo de saída (outputPtr)
            fputc(0x00, outputPtr);
        }
    }

    // Libera a memória alocada para a imagem
    free(image);

    // Fecha os arquivos
    fclose(inputPtr);
    fclose(outputPtr);
    return 0;
}