# Zadanie 1 (20%)
Proszę napisać program przyjmujący jeden (nadawca lub data) lub trzy argumenty (\<adressEmail> <tytuł> <treść>):
- W przypadku wywołania z jednym argumentem uruchamiany jest (za pomocą popen()) program mail. Program użytkownika ma wypisywać listę e-maili posortowaną
alfabetycznie wg. adresów e-mail (argument nadawca) lub wg. daty otrzymania e-maili (argument data)
- Jeżeli program zostanie wywołany z trzema argumentami, to (za pomocą popen()) uruchamiany jest program mail i za jego pomocą wysyłany jest e-mail do
określonego nadawcy z określonym tematem i treścią
# Zadanie 2 (40%)
Napisz program, który liczy numerycznie wartość całki oznaczonej z funkcji 4/(x2+1) w przedziale od 0 do 1 metodą prostokątów (z definicji całki oznaczonej Riemanna).
Pierwszy parametr programu to szerokość każdego prostokąta, określająca dokładność obliczeń. Obliczenia należy rozdzielić na n procesów potomnych (n drugi parametr
wywołania programu), tak by każdy z procesów liczył inny fragment ustalonego wyżej przedziału. Każdy z procesów powinien wynik swojej części obliczeń przesłać przez
potok nienazwany do procesu macierzystego. Każdy proces potomny do komunikacji z procesem macierzystym powinien używać osobnego potoku. Proces macierzysty
powinien oczekiwać na wyniki uzyskane od wszystkich procesów potomnych po czym powinien dodać te wyniki cząstkowe i wyświetlić wynik na standardowym wyjściu.
W programie zmierz, wypisz na konsolę i zapisz do pliku z raportem czasy realizacji dla różnej liczby procesów potomnych oraz różnych dokładności obliczeń.
Dokładności obliczeń należy dobrać w ten sposób by obliczenia trwały co najmniej kilka sekund.
# Zadanie 3 (40%)
Napisz program, który liczy numerycznie wartość całki oznaczonej tak jak w zadaniu 2 ale obliczenia powinny zostać rozdzielone nie na kilka procesów a na kilka
programów. Parametry wywołania programu głównego są takie same jak w zadaniu 2. Program główny uruchamia n programów liczących odpowiednie przydzielone
przez program główny fragmenty całki. Po wykonaniu obliczeń każdy z programów liczących przesyła policzony wynik cząstkowy do programu głównego przez potok
nazwany, wspólny dla wszystkich programów. Podobnie jak w zadaniu 2, zmierz, wypisz na konsolę i zapisz do pliku z raportem czasy realizacji dla różnej liczby
programów liczących oraz różnych dokładności obliczeń. Dokładności obliczeń należy dobrać w ten sposób by obliczenia trwały co najmniej kilka sekund.