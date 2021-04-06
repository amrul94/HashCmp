"Наивная" заготовка для дипломной работы "Сравнение хэш-функций (hash functions) в задаче организации быстрого доступа к данным"

В данной работе я пытаюсь сравнить разные некриптографические хеш-функции.

Работа состоит из следующих папов и файлов:
- папка "csv_parser" включает в себя CSV-парсер, который я взял по адресу: https://github.com/ben-strasser/fast-cpp-csv-parser 
- папка "hashes" включает в себя различные семейства хеш-функций:
    - CityHash, который взял по адресу: https://github.com/google/cityhash
    - FarmHash, который взял по адресу: https://github.com/google/farmhash
    - xxHash, который взял по адресу: https://github.com/Cyan4973/xxHash
    - MurMurHash, который взял по адресу: https://github.com/aappleby/smhasher
    - Pearson Hash, который взял по адресу: https://github.com/Logan007/pearson
    - Paul Hsieh's SuperFastHash, который взял по адресу: http://www.azillionmonkeys.com/qed/hash.html
    - Rolling Hash (BuzHash), который взял по адресу: https://github.com/lemire/rollinghashcpp
- папка "Data" включает в себя отчет проведенного теста ("report.txt")
- "hash_wrappers.h" и "hash_wrappers.cpp" включают в себя обертки над хеш-функциями для удобного использования с std::unordered_map или c любой другой хеш-таблицей, которая принимает хеш-функцию в виде шаблонного параметра
- "log_duration.h" включает в себя класс и пару макросов для удобного подсчета времени выполнения интересующего блока кода
- "london_crime_tests.h" и "london_crime_tests.cpp" включают в себя "наивный" тест хеш-функций. В данных файлах осуществляется парсинг файла "LondonCrime.csv" (для использования скачайте по ссылке: https://drive.google.com/file/d/1Z60w6MKuMNImQ0-JGTEefp03d2FdG-Vr/view?usp=sharing и разместите в папке Data) с загрузкой в std::unordered_map с различными хеш-функциями и попробовал протестировать полученные хеш-таблицы на предмет коллизий
- "random_world_tests.h" и "random_world_tests.cpp" включают в себя тест хеш-функций с случаныйм набором строк заданной длины 
- "main.cpp" собственно запускает тесты

Остальные файлы и папки созданы IDE и особого описания на мой взгляд не трубуют.
