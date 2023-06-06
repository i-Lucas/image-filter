#include <math.h>

#include "helpers.h"

// Converte a imagem para tons de cinza
void grayscale(int altura, int largura, RGBTRIPLE imagem[altura][largura])
{
    // Iterando sobre cada pixel na imagem
    for (int i = 0; i < altura; i++)
    {
        for (int j = 0; j < largura; j++)
        {
            // Calculando a média dos valores de RGB
            int media = (imagem[i][j].rgbtRed + imagem[i][j].rgbtGreen + imagem[i][j].rgbtBlue) / 3;

            // Atribuindo o valor médio a cada componente RGB
            imagem[i][j].rgbtRed = media;
            imagem[i][j].rgbtGreen = media;
            imagem[i][j].rgbtBlue = media;
        }
    }
}

// Reflete a imagem horizontalmente
void reflect(int altura, int largura, RGBTRIPLE imagem[altura][largura])
{
    // Iterando sobre cada linha na imagem
    for (int i = 0; i < altura; i++)
    {
        // Iterando sobre cada coluna até chegar ao meio
        for (int j = 0; j < largura / 2; j++)
        {
            // Trocando os pixels horizontalmente
            RGBTRIPLE temp = imagem[i][j]; // Armazena temporariamente o valor do pixel atual em 'temp' antes de realizar a troca
            imagem[i][j] = imagem[i][largura - 1 - j];
            imagem[i][largura - 1 - j] = temp;
        }
    }
}

// Aplica o efeito de desfoque na imagem
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Cria uma cópia da imagem original para armazenar os valores borrados
    RGBTRIPLE blurred_image[height][width];

    // Itera sobre cada pixel na imagem original
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int count = 0;
            int totalRed = 0, totalGreen = 0, totalBlue = 0;

            // Itera sobre os vizinhos do pixel atual
            for (int k = -1; k <= 1; k++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    int neighborRow = i + k;
                    int neighborCol = j + l;

                    // Verifica se o vizinho está dentro dos limites da imagem
                    if (neighborRow >= 0 && neighborRow < height && neighborCol >= 0 && neighborCol < width)
                    {
                        // Soma os valores de cada componente de cor do pixel vizinho
                        totalRed += image[neighborRow][neighborCol].rgbtRed;
                        totalGreen += image[neighborRow][neighborCol].rgbtGreen;
                        totalBlue += image[neighborRow][neighborCol].rgbtBlue;
                        count++;
                    }
                }
            }

            // Calcula os valores médios para cada componente de cor e atribui à imagem borrada
            blurred_image[i][j].rgbtRed = totalRed / count;
            blurred_image[i][j].rgbtGreen = totalGreen / count;
            blurred_image[i][j].rgbtBlue = totalBlue / count;
        }
    }

    // Copia a imagem borrada de volta para a imagem original
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = blurred_image[i][j];
        }
    }
}

// Detectar bordas na imagem
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Criar uma cópia da imagem original para armazenar os valores das bordas
    RGBTRIPLE edged_image[height][width];

    // Definir as matrizes de convolução Sobel
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Iterar sobre cada pixel na imagem original
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int Gx_red = 0, Gx_green = 0, Gx_blue = 0;
            int Gy_red = 0, Gy_green = 0, Gy_blue = 0;

            // Aplicar o operador Sobel para calcular os gradientes nas direções horizontal e vertical
            for (int k = -1; k <= 1; k++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    int neighborRow = i + k;
                    int neighborCol = j + l;

                    // Verificar se o vizinho está dentro dos limites da imagem
                    if (neighborRow >= 0 && neighborRow < height && neighborCol >= 0 && neighborCol < width)
                    {
                        // Calcular os gradientes nas direções horizontal e vertical
                        Gx_red += image[neighborRow][neighborCol].rgbtRed * Gx[k + 1][l + 1];
                        Gx_green += image[neighborRow][neighborCol].rgbtGreen * Gx[k + 1][l + 1];
                        Gx_blue += image[neighborRow][neighborCol].rgbtBlue * Gx[k + 1][l + 1];

                        Gy_red += image[neighborRow][neighborCol].rgbtRed * Gy[k + 1][l + 1];
                        Gy_green += image[neighborRow][neighborCol].rgbtGreen * Gy[k + 1][l + 1];
                        Gy_blue += image[neighborRow][neighborCol].rgbtBlue * Gy[k + 1][l + 1];
                    }
                }
            }

            // Calcular o valor do gradiente final
            int red = round(sqrt(Gx_red * Gx_red + Gy_red * Gy_red));
            int green = round(sqrt(Gx_green * Gx_green + Gy_green * Gy_green));
            int blue = round(sqrt(Gx_blue * Gx_blue + Gy_blue * Gy_blue));

            // Limitar o valor do gradiente a 255 caso ultrapasse
            red = red > 255 ? 255 : red;
            green = green > 255 ? 255 : green;
            blue = blue > 255 ? 255 : blue;

            // Atribuir os valores do gradiente à imagem das bordas
            edged_image[i][j].rgbtRed = red;
            edged_image[i][j].rgbtGreen = green;
            edged_image[i][j].rgbtBlue = blue;
        }
    }

    // Copiar a imagem das bordas de volta para a imagem original
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = edged_image[i][j];
        }
    }
}

// Aplica o efeito de envelhecimento da imagem
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // Percorrendo cada linha da imagem
    for (int i = 0; i < height; i++)
    {
        // Percorrendo cada pixel da linha atual
        for (int j = 0; j < width; j++)
        {
            // Obtendo os valores originais dos componentes RGB do pixel atual
            unsigned char originalRed = image[i][j].rgbtRed;
            unsigned char originalGreen = image[i][j].rgbtGreen;
            unsigned char originalBlue = image[i][j].rgbtBlue;

            // Calculando os valores sepia para cada componente RGB
            unsigned char sepiaRed = (unsigned char)((0.393 * originalRed) + (0.769 * originalGreen) + (0.189 * originalBlue));
            unsigned char sepiaGreen = (unsigned char)((0.349 * originalRed) + (0.686 * originalGreen) + (0.168 * originalBlue));
            unsigned char sepiaBlue = (unsigned char)((0.272 * originalRed) + (0.534 * originalGreen) + (0.131 * originalBlue));

            // Verificando se os valores sepia excedem 255 e ajustando, se necessário
            image[i][j].rgbtRed = (sepiaRed > 255) ? 255 : sepiaRed;
            image[i][j].rgbtGreen = (sepiaGreen > 255) ? 255 : sepiaGreen;
            image[i][j].rgbtBlue = (sepiaBlue > 255) ? 255 : sepiaBlue;
        }
    }
}