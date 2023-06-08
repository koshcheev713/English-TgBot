# English-TgBot
### Telegramm BOT on C++ that will help you remember new words.

## Usage Linux
Install libreri: English-TgBot/lib/tgbot-cpp/

English-TgBot/bot/
    
Compiling

    clang++ bot.cpp -o tgbot -I/usr/local/include -lTgBot -lboost_system -lssl -lcrypto -lpthread

Run
    
    ./tgbot

Bot's commands

* /new hello - add new word (hello)
* /tr привет - add translate of word
* /list - Show all words
* /last - Show last words
* /rand - Get word to translate
* /last_rand - Get word to translate from last

Lib sourse: https://github.com/reo7sp/tgbot-cpp 

All words save to "EnglishWords.txt" and "RussiaWords.txt" and next run read data from files.
