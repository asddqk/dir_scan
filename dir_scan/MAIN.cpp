/*------------------------------------------------------------------------------
// main.cpp 
Хранит: main() — запуск программы

Что внутри:
- Проверка аргументов командной строки
- Открытие файлов
- Цикл: while(не конец файла) { nxl(); fprintlex(); }

Что делать: ИСПРАВИТЬ void main → int main!
//------------------------------------------------------------------------------*/

#include <string.h>
#include <stdio.h>

#include "scaner_ext.h"

//------------------------------------------------------------------------------

void fprintlex(FILE*);   // печать лексемы в выходной файл
bool scan_init(char*);   // инициализация сканера (открыть входной файл)
void scan_destroy(void); // закрыть входной файл

//------------------------------------------------------------------------------

FILE *outfil;

int main(int argc, char** argv)  //void → int
{
  if(argc != 3) {
    printf("\nIncorrect command format!\n");
    printf("\nYou must type: command sourced_file recived_file.\n");
    return 1;  //добавить return
  }

  outfil = fopen(argv[2], "w");
  line = 1; column = 0;

  if(!scan_init(argv[1])) return 1;  //добавить return
  
  while(lc != lexEof) 
  {
    nxl(); 
    fprintlex(outfil);
  }
  
  printf("\nEnd of scaning!\n");

  scan_destroy();
  fclose(outfil);
  
  return 0;  //добавить return 0
}