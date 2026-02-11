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
    {"vector", kwVECTOR},
    {"of", kwOF},
    {"integer", kwINTEGER},
    {"real", kwREAL},
    {"goto", kwGOTO},
    {"read", kwREAD},
    {"write", kwWRITE},
    {"skip", kwSKIP},
    {"space", kwSPACE},
    {"tab", kwTAB},
    {"if", kwIF},
    {"then", kwTHEN},
    {"else", kwELSE},
    {"while", kwWHILE},
    {"do", kwDO},
    {"begin", lexBST},
    {"end",   lexEST},
    {"mod",   lexMOD}

};

int rw_size = sizeof(rwtab) / sizeof(find_string);