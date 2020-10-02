# RCA_ProjectDette er en guide til hvordan man sætter op miljøet op, 
og begynder at programmere.

---------------Setup miljø------------------------
1. Åben denne mappe, mappe Gazebo
2. Højre click og tryk åben terminal
3. Tast " bash gazebo_server.sh smallworld.world "
(
    Denne kommando forbinder til program og aktivere funktioner
)

4. Åbent ny terminal i samme folder (I ubuntu, click +
i top venstre hjørne), og task " bash gazebo_client.sh "
(
    Denne kommando activere de visuelle elementer, men er faktisk
    ikke påkrævet for at få programmet til at virke.
)

---------------Skriv kode i program-----------------
1. Åben mappe Custom_Robot_Controller (Dette er vores projekt)
2. Højre klik og tryk åben terminal
3. Skriv " code . "
(
    Dette åbner mappen hvor vores kode skal være i visual studio code. 
    main og header filer skal være herude, CMakeLists.txt ligger i 
    CMake mappen.
)
4. Her kan coden til at styrer roboten skrives
5. Vær opmærksom på, at koden kører for første gang på din maskine, så slet
CMake filer undtagen CMakeLists.txt.
6. For at compile koden skal man bruge terminalen indbygget i vc-code.
Skriv først " cd CMake " , dette behøver man kun at gøre en gang.
Skriv " cmake . ", dette behøver man kun at gøre hver gang man ændre CMake (Så en gang)
Skriv " make " dette compiler koden.

7. For at kører det compilede program, skriv " ./robot_control "

--------------Push til GitHub-------------------------
1. Åben mappe RCA_Project
2. Højre klik og åben terminal
3. Skriv " git add . " (tilføjer fill til at kunne blive commited)
4. Skriv " git commit -m "text" " (Sender fil til at blive commited)
5. Skriv " git push " (Pusher alt hvad man har commited, man skal også udfylde kode)

--------------Pull fra Github---------------------------
I terminal "git pull "
Hvis konflikter lav " git add" og "git commig -m "something" " og prøv "git pull igen"