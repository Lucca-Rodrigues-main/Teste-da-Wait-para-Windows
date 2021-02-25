#define _CRT_SECURE_NO_WARNINGS
//https://docs.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-waitforsingleobject

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int Pausa(void); // Aguarda algo do teclado, sem ler do teclado

int main()
{
    printf("Aguardando algo do teclado...");
    
    int x = Pausa();

    if(!x) {
        printf("\n\nTudo ocorreu bem!\nO retorno de WaitForSingleObject() foi WAIT_OBJECT_0.\n");
    } else {
        printf("\n\nAlgo de errado ocorreu!\nO retorno da WaitForSingleObject() nao foi WAIT_OBJECT_0.\n");
    }

    return 0;
}

int Pausa(void) {
    POINT posicao;
    int x, y;

    // Obtendo a posição atual do cursor
    GetCursorPos(&posicao);
    x = posicao.x;
    y = posicao.y;

    while (1) {
        // Par flush-wait
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
        if (WaitForSingleObject(GetStdHandle(STD_INPUT_HANDLE), INFINITE) != WAIT_OBJECT_0) {
            /* Esperamos que o retorno de WaitForSingleObject() seja
            WAIT_OBJECT_0, indicando que o estado do objeto especificado
            foi sinalizado, e caso o retorno não seja este, Pausa()
            retorna 1 */
            return 1;
        }
        if (GetCursorPos(&posicao) && posicao.x == x && posicao.y == y) {
            /* Ignoramos a possibilidade de uma alteração na posição do cursor
            fazer parte do objeto em questão, então caso a mesma não se altere
            e WaitForSingleObject() retorne WAIT_OBJECT_0, sai do loop */
            break;
        }
        /* Se a posição do cursor se alterar desde a última chamada de
        GetCursorPos(), então significa que a mesma fez WaitForSingleObject()
        retornar, logo, o loop persiste */
        x = posicao.x;
        y = posicao.y;
    }
    return 0;
}