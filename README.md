# Village
Project for computer network

Uruchamianie:
./make, po zbudowaniu ./clt i ./server (w folderze server)

Obsługa klienta:

Trzy wiersze od góry wskazują na posiadaną ilość zasobów (drewno, jedzenie), poziom tartaku/stołówki/muru, oraz pozwalają na podniesienie ich poziomu i wskazują na jego cenę. Wyższy poziom -> większa produkcja zasobów w jednostce czasu albo w przypadku muru lepsza obrona.

W lewym dolnym rogu widać posiadaną przez gracza armię, może rekrutować wybraną liczbę jednostek w podanej cenie - pod warunkiem, że go na to stać.

W prawym dolnym rogu jest panel ataku. Pierwsze wciśnięcie przycisku "attack" wskazuje serwerowi nasz zamiar ataku, i w odpowiedzi dostajemy listę dostępnych celów. Wybieramy który chcemy, wpisujemy liczbę wojsk jakie chcemy na ten atak przeznaczyć, i wciskamy przycisk po raz kolejny. W tym momencie lista celów znika, ponieważ zanim wyślemy kolejny atak może okazać się, że jest już nieaktualna, i i tak musielibyśmy sprawdzić ją przed każdym atakiem. Jeśli atak się powiódł, po 5s otrzymamy o tym informację zwrotną oraz aktualizację naszego stanu armii i surowców. Przegrana walka pozostaje bez odpowiedzi. Minimalna liczba jednostek biorąca udział w ataku to 20, mniejsze ataki zostaną odrzucone.

Za każdy udany atak gracz otrzymuje punkt. Gra kończy się, gdy któryś z graczy zdobędzie 4 punkty - wówczas wszyscy otrzymują stosowny komunikat o wygranej bądź przegranej.
