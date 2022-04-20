#include <iostream>
//#include <fstream>

int main()
{
    //-------------CZYTANIE Z PLIKU----------------
    FILE* fin;
    fin = fopen("wiadomosc-do-zaszyfrowania.txt", "r"); //otwieramy plik do odczytu
    const int size = 12; //długość szyfrowanej wiadomości
    int* M = (int*)calloc(size, sizeof(int));
    int* p = M; //pomocniczy wskaźnik
    if (M == NULL)
    {
        perror("Allocation error!");
        return 1;
    }
    for (int i = 0; i < size; i++)
    {
        fscanf(fin, "%d ", p++); //kolejne bity w pliku oddzielone są spacjami
    }
    //-----------------------------------------------------
    const int nKeyLength = 40;
    int K[nKeyLength] = { 0 }; //40-bitowy klucz
    for (int i = 0; i < nKeyLength; i++) //zainicjowanie klucza na przemian zerami i jedynkami
    {
        K[i] = i % 2;
    }
    int S[256] = { 0 };
    //------------------------------------------------------
    for (int i = 0; i < 256; i++)
    {
        S[i] = i;
    }
    int j = 0;
    int temp = 0;
    for (int i = 0; i < 256; i++)
    {
        j = (j + S[i] + K[i % nKeyLength]) % 256;
        temp = S[i];
        S[i] = S[j];
        S[j] = temp; //zamiana
    } //etap 1: permutacja 256-elementowa S wszystkich możliwych stanów, inicjalizacja na podst.klucza
    //------------------------------------------------------
    int i = 0;
    j = 0;
    for(int l = 0; l < size; l++) //tu warunek jak długo generujemy pseudolosowe wartosci, tzn. długość zaszyfrowanej wiadomości
    {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        temp = S[i];
        S[i] = S[j];
        S[j] = temp; //zamiana
        M[l] ^= S[(S[i] + S[j]) % 256];
    }
    //------------------ZAPIS DO PLIKU--------------------------
    if ((fin = fopen("zaszyfrowana_wiadomosc.txt", "w")) == NULL) 
    {
        printf("Cannot open file to write!\n");
        return 4;
    }
    for(int i = 0; i < size; i++) fprintf(fin, "%d ", M[i]); //przy zapisie oddzielamy bity spacjami
    fclose(fin);
    return 0;
}


