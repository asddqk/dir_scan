/* 
Хранит: ЛОКАЛЬНЫЕ данные только для SCANER.cpp

Что внутри:
- struct find_string — пара {строка, тип лексемы}
- rwtab[] — ТАБЛИЦА КЛЮЧЕВЫХ СЛОВ!
  * Например: {"begin", kwBegin}, {"end", kwEnd}

ДОБАВЛЯТЬ новые ключевые слова в rwtab[]
*/
struct find_string {
    char* wrd;   
    lc_type lc;
};


struct find_string rwtab[] = {
    {"abort", kwAbort},
    {"begin", kwBegin},
    {"case",  kwCase},
    {"end",   kwEnd},
    {"float", kwFloat},
    {"goto",  kwGoto},
    {"int",   kwInt},
    {"loop",  kwLoop},
    {"read",  kwRead},
    {"skip",  kwSkip},
    {"space", kwSpace},
    {"tab",   kwTab},
    {"var",   kwVar},
    {"write", kwWrite}
};

int rw_size = sizeof(rwtab) / sizeof(find_string);