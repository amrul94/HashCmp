Изменения 24.03.2021:
1. Удалил 128-битные хеш-функции;
2. Разделил хеши на 2 группы 32 и 64 битные хеши
3. Добавил Pearson Hash, Paul Hsieh's SuperFastHash, Rolling Hash (BuzHash)

Изменения 06.04.2021:
1. Добавил SDBM hash, t1ha hash (заработало со второй попытки), MetroHash

Изменения 08.04.2021:
1. Добавил Fast-Hash, Bernstein's hash djb2, FNV-1a hash, Spooky hash, Jenkins hash (one_at_a_time, lookup3)
2. Включил хеш-функции CityHash в namespace city, так как бл конфликто типов uint128