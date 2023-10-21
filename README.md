# English-TgBot
### Telegramm BOT on C++ that will help you remember new words.

## Usage Linux
Install libreri: English-TgBot/lib/tgbot-cpp/

English-TgBot/bot/
    
Compiling

    clang++ bot.cpp -o tgbot -I/usr/local/include -lTgBot -lboost_system -lssl -lcrypto -lpthread

Run
    
    ./tgbot

restartBotScript - Bush script, wich restarts bot if it crash
Using:
	chmod +x restartBotScript
	./restartBotScript
 
Bot's commands:

* /new  - Add new word (use with /tr)
	/new word
	/tr translate of word
* /tr   - Add translate of word
	/tr translate of word
* /list - Show all words
* /last - Show last words
* /rand - Get word to translate (use with /tr)
	/rand
	Bot: word
	/tr translate
* /last_rand - Get word to translate from last (use with /tr)
	/last_rand
	Bot: word
	/tr translate
* /how    - Get translate of currend word
* /delete - Delte  word from list
	/delete word
	/tr translate of deleted word 

Lib sourse: https://github.com/reo7sp/tgbot-cpp 

All words save to "EnglishWords.txt" and "RussiaWords.txt" and next run reads data from files.
