#include "bmp.h"

// Converte a imagem para tons de cinza
void grayscale(int height, int width, RGBTRIPLE image[height][width]);

// Reflete a imagem horizontalmente
void reflect(int height, int width, RGBTRIPLE image[height][width]);

// Detectar bordas na imagem
void edges(int height, int width, RGBTRIPLE image[height][width]);

// Aplica o efeito de desfoque na imagem
void blur(int height, int width, RGBTRIPLE image[height][width]);