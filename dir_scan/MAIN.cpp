/*------------------------------------------------------------------------------
// main.cpp 
Хранит: main() — запуск программы

Что внутри:
- Проверка аргументов командной строки
- Открытие файлов
- Цикл: while(не конец файла) { nxl(); fprintlex(); }

Что делать: НЕ ТРОГАТЬ (почти никогда)*/

//------------------------------------------------------------------------------

#include     <string.h>
#include     <stdio.h>

#include     "scaner_ext.h"

//------------------------------------------------------------------------------

void fprintlex(FILE*);   // �������� ����� ����������� ������ �������
bool scan_init(char*);   // ��������� ��������� ������� (����� ������ �����)
void scan_destroy(void); // ���������� ������ �������

//------------------------------------------------------------------------------

FILE  *outfil;

void main(int argc, char** argv)
{
  if(argc != 3) {
    printf("\nIncorrect command format!\n");
    printf("\nYou must type: command sourced_file recived_file.\n");
    return;
  }

  outfil = fopen(argv[2], "w");
  line = 1; column = 0;

  if(!scan_init(argv[1])) return;
  
  while(lc != lexEof) 
  {
    nxl(); fprintlex(outfil);
  }
  
  printf("\nEnd of scaning!\n");

  scan_destroy();
  fclose(outfil);
} 

//------------------------------------------------------------------------------

