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
5. For at compile koden skal man bruge terminalen indbygget i vc-code.
Skriv først " cd CMake " , dette behøver man kun at gøre en gang.
Skriv " cmake . ", dette behøver man kun at gøre hver gang man ændre CMake (Så en gang)

5. For at compile eller test kode, gå ind i build mappen via vscode terminalen "cd build",
og skriv " ./robot_control ".
