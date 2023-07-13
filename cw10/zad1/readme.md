# Zmodyfikuj zadanie 2 zestawu 6 w taki sposów aby komunikacja pomiędzy uczestmnikami czatu / klientami / klientami i serwerem realizoiwana była za pośrednictwem socketów.

Dla ustalenia uwagi

Serwer przyjmuje jako swoje argumenty:
- numer portu TCP/UDP (zależnie od zadania)
- ścieżkę gniazda UNIX

Wątek obsługujący sieć powinien obsługiwać gniazdo sieciowe i gniazdo lokalne jednocześnie, wykorzystując w tym celu funkcje do monitorowania wielu deskryptorów
(epoll/poll/select).\
Dodatkowo, osobny wątek powinien cyklicznie "pingować" zarejestrowanych klientów, aby zweryfikować że wciąż odpowiadają na żądania i jeśli nie - usuwać ich z listy
klientów.\
Można przyjąć, że ilość klientów zarejestrowanych na serwerze jest ograniczona do maksymalnie kilkunastu.

Klient przyjmuje jako swoje argumenty:
- swoją nazwę (string o z góry ograniczonej długości)
- sposób połączenia z serwerem (sieć lub komunikacja lokalna przez gniazda UNIX)
- adres serwera (adres IPv4 i numer portu lub ścieżkę do gniazda UNIX serwera)

Klient przy wyłączeniu Ctrl+C powinien wyrejestrować się z serwera.

Zadanie 1 (50%)\
Komunikacja klientów i serwera odbywa się z użyciem protokołu strumieniowego.\
Zadanie 2 (50%)\
Komunikacja klientów i serwera odbywa się z użyciem protokołu datagramowego.