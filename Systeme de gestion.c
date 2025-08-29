#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROOMS 100
#define MAX_RESERVATIONS 100
#define MAX_CLIENTS 100
#define MAX_SERVICES 100
#define MAX_HOTELS 20
#define MAX_CITIES 50

// Structures pour les chambres, réservations, clients, services et hôtels
typedef struct {
    int id;
    char type[50];
    char equipement[100];
    int num_chambre;
    int available;
    float price;
} Room;

typedef struct {
    int roomId;
    char clientName[50];
    char clientf_name[50];
    int numGuests;
    char checkInDate[15];
    char checkOutDate[15];
    char statu[20];
} Reservation;

typedef struct {
    char name[50];
    char f_name[50];
    char gender[10];
    char pay_meth[20];
    char email[50];
    char phone[15];
    char ID_ref[30];
    char cust_type[20];
    int isLoyal;
} Client;

typedef struct {
    int id;
    char name[50];
    char description[100];
    float price;
} Service;

typedef struct {
    int id;
    char name[50];
    char phone[15];
    char mail[50];
    char site[50];
    char emplacement[30];
    char category[10];
    Service services[MAX_SERVICES];
    int serviceCount;
} Hotel;

typedef struct {
    int id;
    char name[50];
    char country[50];
    /*int population;*/
} City;

// Tableaux pour stocker les données
Room rooms[MAX_ROOMS];
Reservation reservations[MAX_RESERVATIONS];
Client clients[MAX_CLIENTS];
Service services[MAX_SERVICES];
Hotel hotels[MAX_HOTELS];
City cities[MAX_CITIES];

int roomCount = 0;
int reservationCount = 0;
int clientCount = 0;
int serviceCount = 0;
int hotelCount = 0;
int nextRoomId = 1;
int cityCount = 0;
int nextCityId = 1;
char selectedCityName[50] = "";

Hotel *selectedHotel = NULL;

// Fonctions pour sauvegarder et charger les données dans des fichiers
void saveRooms(Room rooms[], int roomCount) {
    FILE *file = fopen("rooms.txt", "w");
    if (file == NULL) {
        printf("Erreur d'ouverture du fichier des chambres.\n");
        return;
    }
    for (int i = 0; i < roomCount; i++) {
        fprintf(file, "Identifiant:%d\n Numero de chambre:%d\n Type de chambre:%s\n Equipement:%s\n Disponibilite:%d\n Prix:%.2f\n\n",
                rooms[i].id, rooms[i].num_chambre, rooms[i].type, rooms[i].equipement, rooms[i].available, rooms[i].price);
    }
    fclose(file);
}

void loadRooms() {
    FILE *file = fopen("rooms.txt", "r");
    if (file == NULL) {
        printf("Aucune donnee de chambre trouvee.\n");
        return;
    }
    roomCount = 0;
    int maxId = 0;
    while (!feof(file) && roomCount < MAX_ROOMS) {
        int id, num_chambre, available;
        char type[50], equipement[100];
        float price;

        if (fscanf(file, "Identifiant:%d\n", &id) != 1) break;
        if (fscanf(file, "Numero de chambre:%d\n", &num_chambre) != 1) break;
        if (fscanf(file, "Type de chambre:%4s[^\n]\n", type) != 1) break;
        if (fscanf(file, "Equipement:%9s[^\n]\n", equipement) != 1) break;
        if (fscanf(file, "Disponibilite:%d\n", &available) != 1) break;
        if (fscanf(file, "Prix:%f\n\n", &price) != 1) break;

        rooms[roomCount].id = id;
        rooms[roomCount].num_chambre = num_chambre;
        strcpy(rooms[roomCount].type, type);
        strcpy(rooms[roomCount].equipement, equipement);
        rooms[roomCount].available = available;
        rooms[roomCount].price = price;
        roomCount++;

        if (id > maxId) {
            maxId = id;
        }
    }
    nextRoomId = maxId + 1; // Mettre à jour l'ID pour la prochaine chambre à ajouter
    fclose(file);
}


void saveReservations() {
    FILE *file = fopen("reservations.txt", "w");
    if (file == NULL) {
        printf("Erreur d'ouverture du fichier des reservations.\n");
        return;
    }
    for (int i = 0; i < reservationCount; i++) {
        fprintf(file, "Identifiant:%2d\n Nom du client:%s\n Prenom(s) du client:%s\n Nombre de personnes:%d\n Date d'arrivee:%s\n Date de depart:%s\n",
                reservations[i].roomId, reservations[i].clientName,reservations[i].clientf_name,
                reservations[i].numGuests, reservations[i].checkInDate,
                reservations[i].checkOutDate);
    }
    fclose(file);
}

void loadReservations() {
    FILE *file = fopen("reservations.txt", "r");
    if (file == NULL) {
        printf("Aucune donnee de reservation trouvee.\n");
        return;
    }
    reservationCount = 0;
    while (fscanf(file, "Identifiant:%2d\n Nom du client:%s\n Prenom(s) du client:%s\n Nombre de personnes:%d\n Date d'arrivee:%s\n Date de depart:%s\n",
                  &reservations[reservationCount].roomId,
                  reservations[reservationCount].clientName,
                  reservations[reservationCount].clientf_name,
                  &reservations[reservationCount].numGuests,
                  reservations[reservationCount].checkInDate,
                  reservations[reservationCount].checkOutDate) != EOF) {
        reservationCount++;
    }
    fclose(file);
}

void saveClients() {
    FILE *file = fopen("clients.txt", "w");
    if (file == NULL) {
        printf("Erreur d'ouverture du fichier des clients.\n");
        return;
    }
    for (int i = 0; i < clientCount; i++) {
        fprintf(file, "Nom:%s\n Prenom(s):%s\n Genre:%s\n Mode de payement:%s\n Email:%s\n Telephone:%s\n Identifiant de la piece:%s\n Type de cleint:%s\n Fidelite:%d\n",
                clients[i].name, clients[i].f_name, clients[i].gender, clients[i].pay_meth, clients[i].email,
                clients[i].phone, clients[i].ID_ref, clients[i].cust_type, clients[i].isLoyal);
    }
    fclose(file);
}

void loadClients() {
    FILE *file = fopen("clients.txt", "r");
    if (file == NULL) {
        printf("Aucune donnee de client trouvee.\n");
        return;
    }
    clientCount = 0;
    while (fscanf(file, "Nom:%s\n Prenom(s):%s\n Genre:%s\n Mode de payement:%s\n Email:%s\n Telephone:%s\n Identifiant de la piece:%s\n Type de cleint:%s\n Fidelite:%d\n",
                  clients[clientCount].name,clients[clientCount].f_name, clients[clientCount].gender, clients[clientCount].pay_meth, clients[clientCount].email,
                clients[clientCount].phone, clients[clientCount].ID_ref, clients[clientCount].cust_type, &clients[clientCount].isLoyal) != EOF) {
        clientCount++;
    }
    fclose(file);
}

void saveServices() {
    FILE *file = fopen("services.txt", "w");
    if (file == NULL) {
        printf("Erreur d'ouverture du fichier des services.\n");
        return;
    }
    for (int i = 0; i < serviceCount; i++) {
        fprintf(file, "Identifiant:%2d\n Nom:%s\n Description:%s\n Prix:%.2f\n",
                services[i].id, services[i].name, services[i].description, services[i].price);
    }
    fclose(file);
}

void loadServices() {
    FILE *file = fopen("services.txt", "r");
    if (file == NULL) {
        printf("Aucune donnee de service trouvee.\n");
        return;
    }
    serviceCount = 0;
    while (fscanf(file,"Identifiant:%d\n Nom:%s\n Description:%s\n Prix:%.2f\n",
                  &services[serviceCount].id, services[serviceCount].name,
                  services[serviceCount].description, &services[serviceCount].price) != EOF) {
        serviceCount++;
    }
    fclose(file);
}

void saveHotels() {
    FILE *file = fopen("hotels.txt", "w");
    if (file == NULL) {
        printf("Erreur d'ouverture du fichier des hotels.\n");
        return;
    }
    for (int i = 0; i < hotelCount; i++) {
        fprintf(file, "Identifiant:%d\n Nom:%s\n Telephone:%s\n Email:%s\n Emplacement:%s\n Site Web:%s\n Categorie:%s\n Service:%d\n",
                hotels[i].id, hotels[i].name, hotels[i].phone, hotels[i].mail, hotels[i].emplacement, hotels[i].site, hotels[i].category, hotels[i].serviceCount);
        for (int j = 0; j < hotels[i].serviceCount; j++) {
            fprintf(file, "%d %s %s %.2f\n",
                    hotels[i].services[j].id, hotels[i].services[j].name,
                    hotels[i].services[j].description, hotels[i].services[j].price);
        }
    }
    fclose(file);
}

void loadHotels() {
    FILE *file = fopen("hotels.txt", "r");
    if (file == NULL) {
        printf("Aucune donnee d'hotel trouvee.\n");
        return;
    }
    hotelCount = 0;
    while (fscanf(file, "Identifiant:%d\n Nom:%s\n Telephone:%s\n Email:%s\n Emplacement:%s\n Site Web:%s\n Categorie:%s\n Service:%d\n",
                  &hotels[hotelCount].id,hotels[hotelCount].name, hotels[hotelCount].phone, hotels[hotelCount].mail, hotels[hotelCount].emplacement, hotels[hotelCount].site,
                 hotels[hotelCount].category,
                  &hotels[hotelCount].serviceCount) != EOF) {
        for (int j = 0; j < hotels[hotelCount].serviceCount; j++) {
            fscanf(file, "%d %s %s %f",
                   &hotels[hotelCount].services[j].id, hotels[hotelCount].services[j].name,
                   hotels[hotelCount].services[j].description, &hotels[hotelCount].services[j].price);
        }
        hotelCount++;
    }
    fclose(file);
}

void saveCities() {
    FILE *file = fopen("cities.txt", "w");
    if (file == NULL) {
        printf("Erreur d'ouverture du fichier des villes.\n");
        return;
    }
    for (int i = 0; i < cityCount; i++) {
        fprintf(file, "Identifiant:%2d\n Nom:%s\n Pays:%s\n",
                cities[i].id, cities[i].name, cities[i].country);
    }
    fclose(file);
}

void loadCities() {
    FILE *file = fopen("cities.txt", "r");
    if (file == NULL) {
        printf("Aucune donnee de ville trouvee.\n");
        return;
    }
    cityCount = 0;
    int maxId = 0;
    while (!feof(file) && cityCount < MAX_CITIES) {
        int id;
        char name[50], country[50];
        if (fscanf(file, "Identifiant:%2d\n Nom:%4s[^\n]\n Pays:%4s[^\n]\n",
                   &id, name, country) != 3) {
            break;
        }
        cities[cityCount].id = id;
        strcpy(cities[cityCount].name, name);
        strcpy(cities[cityCount].country, country);
        cityCount++;
        if (id > maxId) {
            maxId = id;
        }
    }
    nextCityId = maxId + 1; // Mettre à jour l'ID pour la prochaine ville à ajouter
    fclose(file);
}


// Fonctions de gestion des chambres
void addRoom() {
    if (selectedHotel == NULL) {
        printf("Aucun hôtel sélectionné.\n");
        return;
    }

    if (roomCount >= MAX_ROOMS) {
        printf("La capacite maximale des chambres est atteinte.\n");
        return;
    }

    int num_chambre;
    char type[50];
    char equipement[100];
    float price;

    printf("Ajouter une nouvelle chambre:\n");

    // Génération automatique de l'ID
    int id = nextRoomId++;
    printf("ID: %d\n", id);

    // Saisie et vérification du numéro de chambre
    printf("Numero de chambre: ");
    if (scanf("%d", &num_chambre) != 1) {
        printf("Erreur de saisie. Veuillez entrer un nombre valide pour le numero de chambre.\n");
        while (getchar() != '\n'); // Vider le buffer
        return;
    }
    while (getchar() != '\n'); // Vider le buffer après scanf

    // Saisie et gestion des espaces pour le type
    printf("Type: ");
    if (fgets(type, sizeof(type), stdin) == NULL) {
        printf("Erreur de saisie. Veuillez entrer un type valide.\n");
        return;
    }
    type[strcspn(type, "\n")] = 0; // Enlever le retour à la ligne

    // Saisie et gestion des espaces pour l'équipement
    printf("Equipement: ");
    if (fgets(equipement, sizeof(equipement), stdin) == NULL) {                                                                 
        printf("Erreur de saisie. Veuillez entrer des équipements valides.\n");
        return;
    }
    equipement[strcspn(equipement, "\n")] = 0; // Enlever le retour à la ligne

    // Saisie et vérification du prix
    printf("Prix: ");
    if (scanf("%f", &price) != 1) {
        printf("Erreur de saisie. Veuillez entrer un prix valide.\n");
        while (getchar() != '\n'); // Vider le buffer
        return;
    }
    while (getchar() != '\n'); // Vider le buffer après scanf

    // Ajouter la chambre
    rooms[roomCount].id = id;
    rooms[roomCount].num_chambre = num_chambre;
    strcpy(rooms[roomCount].type, type);
    strcpy(rooms[roomCount].equipement, equipement);
    rooms[roomCount].price = price;
    rooms[roomCount].available = 1;
    roomCount++;
    printf("Chambre ajoutee avec succes.\n");
    saveRooms(rooms, roomCount);
}

void removeRoom() {
    printf("Entrez l'ID de la chambre a supprimer: ");
    int roomId;
    scanf("%d", &roomId);

    int found = 0;
    for (int i = 0; i < roomCount; i++) {
        if (rooms[i].id == roomId) {
            rooms[i] = rooms[roomCount - 1];
            roomCount--;
            saveRooms(rooms, roomCount);
            printf("Chambre supprimee avec succes.\n");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Chambre non trouvee.\n");
    }
}

void displayAvailableRooms() {
    printf("Liste des chambres disponibles:\n");
    printf("------------------------------------------------------------------------------------\n");
    printf("| %5s | %15s | %20s | %15s | %8s |\n", "ID", "Numéro de chambre", "Equipement", "Type", "Prix");
    printf("------------------------------------------------------------------------------------\n");

    int found = 0; // Variable pour vérifier s'il y a des chambres disponibles
    for (int i = 0; i < roomCount; i++) {
        if (rooms[i].available) {
            printf("| %5d | %15d | %20s | %15s | %8.2f |\n",
                   rooms[i].id, rooms[i].num_chambre, rooms[i].equipement, rooms[i].type, rooms[i].price);
            found = 1;
        }
    }

    if (!found) {
        printf("Aucune chambre disponible.\n");
    }
    printf("-----------------------------------------------------------------------\n");
}


// Fonctions de gestion des réservations
void reserveRoom() {
    if (selectedHotel == NULL) {
        printf("Aucun hotel sélectionne.\n");
        return;
    }

    if (reservationCount >= MAX_RESERVATIONS) {
        printf("La capacite maximale des reservations est atteinte.\n");
        return;
    }

    // Rechercher automatiquement une chambre disponible
    int roomFound = 0;
    int roomId = -1;
    for (int i = 0; i < roomCount; i++) {
        if (rooms[i].available) {
            roomFound = 1;
            roomId = rooms[i].id;
            rooms[i].available = 0;
            break;
        }
    }

    if (roomFound) {
        reservations[reservationCount].roomId = roomId;

        // Demander les informations de la réservation
        printf("Nom du client: ");
        fgets(reservations[reservationCount].clientName, sizeof(reservations[reservationCount].clientName), stdin);
        reservations[reservationCount].clientName[strcspn(reservations[reservationCount].clientName, "\n")] = 0; // Supprimer le caractère de nouvelle ligne

        printf("Prenom(s) du client: ");
        fgets(reservations[reservationCount].clientf_name, sizeof(reservations[reservationCount].clientf_name), stdin);
        reservations[reservationCount].clientf_name[strcspn(reservations[reservationCount].clientf_name, "\n")] = 0; // Supprimer le caractère de nouvelle ligne

        printf("Nombre de personnes: ");
        scanf("%d", &reservations[reservationCount].numGuests);
        while (getchar() != '\n'); // Vider le buffer après lecture de l'entier

        printf("Date d'arrivee (JJ/MM/AAAA): ");
        fgets(reservations[reservationCount].checkInDate, sizeof(reservations[reservationCount].checkInDate), stdin);
        reservations[reservationCount].checkInDate[strcspn(reservations[reservationCount].checkInDate, "\n")] = 0; // Supprimer le caractère de nouvelle ligne

        printf("Date de depart (JJ/MM/AAAA): ");
        fgets(reservations[reservationCount].checkOutDate, sizeof(reservations[reservationCount].checkOutDate), stdin);
        reservations[reservationCount].checkOutDate[strcspn(reservations[reservationCount].checkOutDate, "\n")] = 0; // Supprimer le caractère de nouvelle ligne

        reservationCount++;
        printf("Reservation ajoutee avec succes pour la chambre ID %d.\n", roomId);

        saveRooms(rooms, roomCount);
        saveReservations();
    } else {
        printf("Aucune chambre disponible.\n");
    }
}

void modifyReservation() {
    char clientName[MAX_CLIENTS];
    printf("Entrez le nom du client pour modifier la reservation: ");
    fgets(clientName, sizeof(clientName), stdin);
    clientName[strcspn(clientName, "\n")] = '\0'; // Supprimer le '\n' ajouté par fgets

    int found = 0;
    for (int i = 0; i < reservationCount; i++) {
        if (strcmp(reservations[i].clientName, clientName) == 0) {
            found = 1;
            printf("Modification de la reservation pour le client %s :\n", clientName);

            printf("Nouveau nombre de personnes: ");
            scanf("%d", &reservations[i].numGuests);
            while (getchar() != '\n'); // Vider le buffer après lecture de l'entier

            printf("Nouvelle date d'arrivee (JJ/MM/AAAA): ");
            fgets(reservations[i].checkInDate, sizeof(reservations[i].checkInDate), stdin);
            reservations[i].checkInDate[strcspn(reservations[i].checkInDate, "\n")] = '\0'; // Supprimer le '\n' ajouté par fgets

            printf("Nouvelle date de depart (JJ/MM/AAAA): ");
            fgets(reservations[i].checkOutDate, sizeof(reservations[i].checkOutDate), stdin);
            reservations[i].checkOutDate[strcspn(reservations[i].checkOutDate, "\n")] = '\0'; // Supprimer le '\n' ajouté par fgets

            printf("Reservation modifiee avec succes.\n");
            break;
        }
    }

    if (!found) {
        printf("Reservation pour %s non trouvee.\n", clientName);
    }

    // Sauvegarder les réservations après chaque modification
    saveReservations();
}

void removeReservation() {
    char clientName[100]; // Taille suffisamment grande pour les noms
    char clientf_name[100];

    printf("Entrez le nom du client pour lequel supprimer la reservation: ");
    // Lire le nom complet du client (avec espaces)
    fgets(clientName, sizeof(clientName), stdin);
    clientName[strcspn(clientName, "\n")] = 0; // Supprimer le caractère de nouvelle ligne

    printf("Entrez le prenom(s) du client pour lequel supprimer la reservation: ");
    // Lire le prénom(s) complet du client (avec espaces)
    fgets(clientf_name, sizeof(clientf_name), stdin);
    clientf_name[strcspn(clientf_name, "\n")] = 0; // Supprimer le caractère de nouvelle ligne

    int found = 0;
    for (int i = 0; i < reservationCount; i++) {
        // Comparer les noms et prénoms en utilisant strcmp pour la comparaison sécurisée des chaînes
        if (strcmp(reservations[i].clientName, clientName) == 0 &&
            strcmp(reservations[i].clientf_name, clientf_name) == 0) {
            // Suppression de la réservation en déplaçant la dernière réservation à sa place
            reservations[i] = reservations[reservationCount - 1];
            reservationCount--;

            // Marquer la chambre comme disponible
            int roomId = reservations[i].roomId;
            for (int j = 0; j < roomCount; j++) {
                if (rooms[j].id == roomId) {
                    rooms[j].available = 1;
                    break;
                }
            }

            // Sauvegarder les chambres et les réservations mises à jour
            saveRooms(rooms, roomCount);
            saveReservations();
            printf("Reservation pour %s %s supprimee avec succes.\n", clientName, clientf_name);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Reservation pour %s %s non trouvee.\n", clientName, clientf_name);
    }
}


/*void addReservation() {
    if (reservationCount < MAX_RESERVATIONS) {
        printf("Entrez l'identifiant de la chambre: ");
        scanf("%d", &reservations[reservationCount].roomId);
        getchar(); // Capturer le retour à la ligne

        printf("Entrez le nom du client: ");
        fgets(reservations[reservationCount].clientName, sizeof(reservations[reservationCount].clientName), stdin);
        reservations[reservationCount].clientName[strcspn(reservations[reservationCount].clientName, "\n")] = 0; // Supprimer le caractère de nouvelle ligne

        printf("Entrez le prenom(s) du client: ");
        fgets(reservations[reservationCount].clientf_name, sizeof(reservations[reservationCount].clientf_name), stdin);
        reservations[reservationCount].clientf_name[strcspn(reservations[reservationCount].clientf_name, "\n")] = 0; // Supprimer le caractère de nouvelle ligne

        printf("Entrez le nombre de personnes: ");
        scanf("%d", &reservations[reservationCount].numGuests);
        getchar(); // Capturer le retour à la ligne

        printf("Entrez la date d'arrivee (JJ/MM/AAAA): ");
        fgets(reservations[reservationCount].checkInDate, sizeof(reservations[reservationCount].checkInDate), stdin);
        reservations[reservationCount].checkInDate[strcspn(reservations[reservationCount].checkInDate, "\n")] = 0; // Supprimer le caractère de nouvelle ligne

        printf("Entrez la date de depart (JJ/MM/AAAA): ");
        fgets(reservations[reservationCount].checkOutDate, sizeof(reservations[reservationCount].checkOutDate), stdin);
        reservations[reservationCount].checkOutDate[strcspn(reservations[reservationCount].checkOutDate, "\n")] = 0; // Supprimer le caractère de nouvelle ligne

        reservationCount++;
        printf("Reservation ajoutee avec succes.\n");
    } else {
        printf("Nombre maximum de reservations atteint.\n");
    }
}*/



void displayReservations() {
    printf("Reservations:\n");
    for (int i = 0; i < reservationCount; i++) {
        printf("ID_Chambre: %d, Client: %s %s, Nombre d'invite: %d, Date d'arrivee: %s, Date d'arrivee: %s\n",
               reservations[i].roomId, reservations[i].clientName, reservations[i].clientf_name,
               reservations[i].numGuests, reservations[i].checkInDate,
               reservations[i].checkOutDate);
    }
}



// Fonctions de gestion des clients
void addClient() {
    if (clientCount >= MAX_CLIENTS) {
        printf("Capacite maximale de client atteinte.\n");
        return;
    }

    printf("Ajoutez un nouveau client:\n");
    
    printf("Nom: ");
    fgets(clients[clientCount].name, sizeof(clients[clientCount].name), stdin);
    clients[clientCount].name[strcspn(clients[clientCount].name, "\n")] = '\0';

    printf("Prenom(s): ");
    fgets(clients[clientCount].f_name, sizeof(clients[clientCount].f_name), stdin);
    clients[clientCount].f_name[strcspn(clients[clientCount].f_name, "\n")] = '\0';

    printf("Genre: ");
    fgets(clients[clientCount].gender, sizeof(clients[clientCount].gender), stdin);
    clients[clientCount].gender[strcspn(clients[clientCount].gender, "\n")] = '\0';

    printf("Email: ");
    fgets(clients[clientCount].email, sizeof(clients[clientCount].email), stdin);
    clients[clientCount].email[strcspn(clients[clientCount].email, "\n")] = '\0';

    printf("Telephone: ");
    fgets(clients[clientCount].phone, sizeof(clients[clientCount].phone), stdin);
    clients[clientCount].phone[strcspn(clients[clientCount].phone, "\n")] = '\0';

    printf("Numero de reference de la piece d'identite: ");
    fgets(clients[clientCount].ID_ref, sizeof(clients[clientCount].ID_ref), stdin);
    clients[clientCount].ID_ref[strcspn(clients[clientCount].ID_ref, "\n")] = '\0';

    printf("Mode de payement: ");
    fgets(clients[clientCount].pay_meth, sizeof(clients[clientCount].pay_meth), stdin);
    clients[clientCount].pay_meth[strcspn(clients[clientCount].pay_meth, "\n")] = '\0';

    printf("Est-il fidele?(1 pour oui, 0 pour non): ");
    scanf("%d", &clients[clientCount].isLoyal);
    while (getchar() != '\n'); // Clear buffer after reading integer

    printf("Type de client: ");
    fgets(clients[clientCount].cust_type, sizeof(clients[clientCount].cust_type), stdin);
    clients[clientCount].cust_type[strcspn(clients[clientCount].cust_type, "\n")] = '\0';

    clientCount++;
    printf("Client ajoute avec succes.\n");
    saveClients();
}

void removeClient() {
    printf("Entrer le nom du client a supprimer: ");
    char clientName[100];
    fgets(clientName, sizeof(clientName), stdin);
    clientName[strcspn(clientName, "\n")] = '\0';

    int found = 0;
    for (int i = 0; i < clientCount; i++) {
        if (strcmp(clients[i].name, clientName) == 0) {
            clients[i] = clients[clientCount - 1];
            clientCount--;
            saveClients();
            printf("Client supprime avec succes.\n");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Client non trouve.\n");
    }
}

void displayClients() {
    printf("Clients:\n");
    for (int i = 0; i < clientCount; i++) {
        printf("Nom: %s, Prenom(s): %s, Genre: %s, Email: %s, Téléphone: %s, Reference de la piece d'identite: %s, Mode de paiement: %s, Fidèle: %d, Type de client: %s\n",
               clients[i].name, clients[i].f_name, clients[i].gender, clients[i].email, clients[i].phone, clients[i].ID_ref, clients[i].pay_meth, clients[i].isLoyal, clients[i].cust_type);
    }
}


// Fonctions de gestion des services
void addService() {
    if (serviceCount >= MAX_SERVICES) {
        printf("La capacite maximale des services est atteinte.\n");
        return;
    }

    services[serviceCount].id = serviceCount + 1; 

    printf("Ajouter un nouveau service:\n");
    
    printf("Nom: ");
    fgets(services[serviceCount].name, sizeof(services[serviceCount].name), stdin);
    services[serviceCount].name[strcspn(services[serviceCount].name, "\n")] = '\0'; 

    printf("Description: ");
    fgets(services[serviceCount].description, sizeof(services[serviceCount].description), stdin);
    services[serviceCount].description[strcspn(services[serviceCount].description, "\n")] = '\0'; 

    printf("Prix: ");
    scanf("%f", &services[serviceCount].price);
    while (getchar() != '\n'); 

    serviceCount++;
    printf("Service ajoute avec succes.\n");
    saveServices();
}

void removeService() {
    printf("Entrez le nom du service a supprimer: ");
    char serviceName[100];
    fgets(serviceName, sizeof(serviceName), stdin);
    serviceName[strcspn(serviceName, "\n")] = '\0';

    int found = 0;
    for (int i = 0; i < serviceCount; i++) {
        if (strcmp(services[i].name, serviceName) == 0) {
            
            services[i] = services[serviceCount - 1];
            serviceCount--;
            saveServices();
            printf("Service \"%s\" supprime avec succes.\n", serviceName);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Service \"%s\" non trouve.\n", serviceName);
    }
}

void displayServices() {
    printf("Services:\n");
    for (int i = 0; i < serviceCount; i++) {
        printf("ID: %d, Nom: %s, Description: %s, Prix: %.2f\n",
               services[i].id, services[i].name, services[i].description, services[i].price);
    }
}

// Fonctions de gestion des hôtels
void addHotel() {
    if (hotelCount >= MAX_HOTELS) {
        printf("La capacite maximale des hotels est atteinte.\n");
        return;
    }

    printf("Ajouter un nouvel hotel:\n");

   
    static int nextId = 1;
    hotels[hotelCount].id = nextId++;

    printf("Nom: ");
    getchar(); 
    fgets(hotels[hotelCount].name, sizeof(hotels[hotelCount].name), stdin);
    hotels[hotelCount].name[strcspn(hotels[hotelCount].name, "\n")] = '\0'; 

    printf("Telephone: ");
    fgets(hotels[hotelCount].phone, sizeof(hotels[hotelCount].phone), stdin);
    hotels[hotelCount].phone[strcspn(hotels[hotelCount].phone, "\n")] = '\0'; 
    printf("Email: ");
    fgets(hotels[hotelCount].mail, sizeof(hotels[hotelCount].mail), stdin);
    hotels[hotelCount].mail[strcspn(hotels[hotelCount].mail, "\n")] = '\0'; 

    printf("Emplacement: ");
    fgets(hotels[hotelCount].emplacement, sizeof(hotels[hotelCount].emplacement), stdin);
    hotels[hotelCount].emplacement[strcspn(hotels[hotelCount].emplacement, "\n")] = '\0'; 
    printf("Site web: ");
    fgets(hotels[hotelCount].site, sizeof(hotels[hotelCount].site), stdin);
    hotels[hotelCount].site[strcspn(hotels[hotelCount].site, "\n")] = '\0'; 
    printf("Categorie (ex: 3 etoiles): ");
    fgets(hotels[hotelCount].category, sizeof(hotels[hotelCount].category), stdin);
    hotels[hotelCount].category[strcspn(hotels[hotelCount].category, "\n")] = '\0'; 

    hotels[hotelCount].serviceCount = 0;
    hotelCount++;

    printf("Hôtel ajoute avec succes.\n");
    saveHotels();
}

void removeHotel(const char* name) {
    int found = 0;
    for (int i = 0; i < hotelCount; i++) {
        if (strcmp(hotels[i].name, name) == 0) {
            found = 1;
            for (int j = i; j < hotelCount - 1; j++) {
                hotels[j] = hotels[j + 1];
            }
            hotelCount--;
            printf("Hotel supprime avec succes.\n");
            break;
        }
    }
    if (!found) {
        printf("Hotel non trouve.\n");
    }
}

void selectHotel() {
    printf("Liste des hotels disponibles:\n");
    displayHotels();

    char hotelName[50];
    printf("Entrez le nom de l'hotel a selectionner: ");
    scanf("%s", hotelName);

    for (int i = 0; i < hotelCount; i++) {
        if (strcmp(hotels[i].name, hotelName) == 0) {
            selectedHotel = &hotels[i];
            printf("Hotel '%s' selectionne.\n", selectedHotel->name);
            return;
        }
    }
    printf("Hotel non trouve.\n");
}


void displayHotels() {
    printf("Hotels:\n");
    for (int i = 0; i < hotelCount; i++) {
        printf("ID: %2d, Nom: %s, Telephone: %s, Email: %s, Emplacement: %s, Site web: %s, Categorie: %s\n",
               hotels[i].id, hotels[i].name, hotels[i].phone, hotels[i].mail, hotels[i].emplacement, hotels[i].site, hotels[i].category);
    }
}

// Associer des services à un hôtel
void associateServiceToHotel() {
    char hotelIdStr[10]; 
    char serviceIdStr[10]; 

    printf("ID de l'hotel: ");
    fgets(hotelIdStr, sizeof(hotelIdStr), stdin);
    hotelIdStr[strcspn(hotelIdStr, "\n")] = '\0'; 

    printf("ID du service: ");
    fgets(serviceIdStr, sizeof(serviceIdStr), stdin);
    serviceIdStr[strcspn(serviceIdStr, "\n")] = '\0'; 

    int hotelId = atoi(hotelIdStr); 
    int serviceId = atoi(serviceIdStr); 

    int hotelFound = 0;
    for (int i = 0; i < hotelCount; i++) {
        if (hotels[i].id == hotelId) {
            hotelFound = 1;
            if (hotels[i].serviceCount >= MAX_SERVICES) {
                printf("La capacite maximale des services pour cet hotel est atteinte.\n");
                return;
            }
            int serviceFound = 0;
            for (int j = 0; j < serviceCount; j++) {
                if (services[j].id == serviceId) {
                    serviceFound = 1;
                    hotels[i].services[hotels[i].serviceCount] = services[j];
                    hotels[i].serviceCount++;
                    printf("Service associe avec succes.\n");
                    saveHotels();
                    return;
                }
            }
            if (!serviceFound) {
                printf("Service non trouve.\n");
                return;
            }
        }
    }
    if (!hotelFound) {
        printf("Hotel non trouve.\n");
    }
}

// Fonction pour éditer l'état des hôtels
void editHotelState() {
    int hotelId;
    printf("ID de l'hotel a editer: ");
    scanf("%d", &hotelId);

    for (int i = 0; i < hotelCount; i++) {
        if (hotels[i].id == hotelId) {
            printf("Modifier l'etat de l'hotel %s:\n", hotels[i].name);

            printf("Nom actuel: %s\n", hotels[i].name);
            printf("Nouveau nom: ");
            getchar(); 
            fgets(hotels[i].name, sizeof(hotels[i].name), stdin);
            hotels[i].name[strcspn(hotels[i].name, "\n")] = '\0'; 

            printf("Telephone actuel: %s\n", hotels[i].phone);
            printf("Nouveau telephone: ");
            fgets(hotels[i].phone, sizeof(hotels[i].phone), stdin);
            hotels[i].phone[strcspn(hotels[i].phone, "\n")] = '\0'; 

            printf("Email actuel: %s\n", hotels[i].mail);
            printf("Nouveau Email: ");
            fgets(hotels[i].mail, sizeof(hotels[i].mail), stdin);
            hotels[i].mail[strcspn(hotels[i].mail, "\n")] = '\0'; 

            printf("Emplacement actuel: %s\n", hotels[i].emplacement);
            printf("Nouvel emplacement: ");
            fgets(hotels[i].emplacement, sizeof(hotels[i].emplacement), stdin);
            hotels[i].emplacement[strcspn(hotels[i].emplacement, "\n")] = '\0'; 

            printf("Site web actuel: %s\n", hotels[i].site);
            printf("Nouveau Site web: ");
            fgets(hotels[i].site, sizeof(hotels[i].site), stdin);
            hotels[i].site[strcspn(hotels[i].site, "\n")] = '\0'; 

            printf("Categorie actuelle: %s\n", hotels[i].category);
            printf("Nouvelle categorie: ");
            fgets(hotels[i].category, sizeof(hotels[i].category), stdin);
            hotels[i].category[strcspn(hotels[i].category, "\n")] = '\0'; 

            printf("L'hotel a ete mis a jour.\n");
            saveHotels();
            return;
        }
    }
    printf("Hotel non trouve.\n");
}

// Fonction pour ajouter une ville
void addCity() {
    if (cityCount >= MAX_CITIES) {
        printf("La capacite maximale des villes est atteinte.\n");
        return;
    }

    int id = nextCityId++;
    char name[50];
    char country[50];

    printf("Ajouter une nouvelle ville:\n");

    // Saisie et gestion des espaces pour le nom
    printf("Nom de la ville: ");
    if (fgets(name, sizeof(name), stdin) == NULL) {
        printf("Erreur de saisie. Veuillez entrer un nom valide.\n");
        return;
    }
    name[strcspn(name, "\n")] = 0; 

    // Saisie et gestion des espaces pour le pays
    printf("Pays: ");
    if (fgets(country, sizeof(country), stdin) == NULL) {
        printf("Erreur de saisie. Veuillez entrer un pays valide.\n");
        return;
    }
    country[strcspn(country, "\n")] = 0; 

    // Ajouter la ville
    cities[cityCount].id = id;
    strcpy(cities[cityCount].name, name);
    strcpy(cities[cityCount].country, country);
    cityCount++;
    printf("Ville ajoutée avec succes.\n");
    saveCities();
}

// Fonction pour afficher les villes
void displayCities() {
    if (cityCount == 0) {
        printf("Aucune ville enregistree.\n");
        return;
    }
    printf("Liste des villes:\n");
    for (int i = 0; i < cityCount; i++) {
        printf("Identifiant:%d\n Nom:%s\n Pays:%s\n", cities[i].id, cities[i].name, cities[i].country);
    }
}

// Fonction pour seclectionner une ville
void selectCityByName() {
    if (cityCount == 0) {
        printf("Aucune ville disponible pour selection.\n");
        return;
    }

    char cityName[50];
    printf("Entrez le nom de la ville que vous souhaitez sélectionner: ");
    if (fgets(cityName, sizeof(cityName), stdin) == NULL) {
        printf("Erreur de saisie. Veuillez entrer un nom valide.\n");
        return;
    }
    cityName[strcspn(cityName, "\n")] = 0;

    int found = 0;
    for (int i = 0; i < cityCount; i++) {
        if (strcmp(cities[i].name, cityName) == 0) {
            strcpy(selectedCityName, cityName);
            printf("Ville selectionnee: %s, %s\n", cities[i].name, cities[i].country);
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Aucune ville trouvee avec ce nom.\n");
    }
}

//Fonction pour supprimer une ville
void removeCityByName() {
    if (cityCount == 0) {
        printf("Aucune ville enregistree pour suppression.\n");
        return;
    }

    char cityName[50];
    printf("Entrez le nom de la ville à supprimer: ");
    if (fgets(cityName, sizeof(cityName), stdin) == NULL) {
        printf("Erreur de saisie. Veuillez entrer un nom valide.\n");
        return;
    }
    cityName[strcspn(cityName, "\n")] = 0;

    int found = 0;
    for (int i = 0; i < cityCount; i++) {
        if (strcmp(cities[i].name, cityName) == 0) {
            for (int j = i; j < cityCount - 1; j++) {
                cities[j] = cities[j + 1];
            }
            cityCount--;
            printf("Ville supprimee avec succes.\n");
            saveCities();
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Aucune ville trouvee avec ce nom.\n");
    }
}


// Menu principal
int main() {
    int choice;
    loadRooms();
    loadReservations();
    loadClients();
    loadServices();
    loadHotels();
    loadCities();
    

   do {
        printf("\n****************************************");
        printf("\n**********Systeme de gestion************\n");
        printf("****************************************\n");
        printf("********Gestion des villes**************\n");
        printf("*  1. Ajouter une ville                *\n");
        printf("*  2. Selectionner une ville           *\n");
        printf("*  3. Afficher les villes              *\n");
        printf("*  4. Supprimer une ville              *\n");
        printf("****************************************\n");
        printf("********Gestion des hotels**************\n");
        printf("*  5. Selectionner un hotel            *\n");
        printf("*  6. Ajouter un hotel                 *\n");
        printf("*  7. Afficher tous les hotels         *\n");
        printf("*  8. Modifier l'etat d'un hotel       *\n");
        printf("*  9. Supprimer un hotel               *\n");
        printf("****************************************\n");
        printf("********Gestion des reservations********\n");
        printf("* 10. Ajouter une reservation          *\n");
        printf("* 11. Afficher toutes les reservations *\n");
        printf("* 12. Modifier une reservation         *\n");
        printf("* 13. Supprimer une reservation        *\n");
        printf("****************************************\n");
        printf("********Gestion des chambres************\n");
        printf("* 14. Ajouter une chambre              *\n");
        printf("* 15. Afficher les chambres disponibles*\n");
        printf("* 16. Supprimer une chambre            *\n");
        printf("****************************************\n");
        printf("********Gestion des clients*************\n");
        printf("****************************************\n");
        printf("* 17. Ajouter un client                *\n");
        printf("* 18. Afficher tous les clients        *\n");
        printf("* 19. Supprimer un client              *\n");
        printf("****************************************\n");
        printf("********Gestion des services************\n");
        printf("* 20. Ajouter un service               *\n");
        printf("* 21. Afficher tous les services       *\n");
        printf("* 22. Associer un service a un hotel   *\n");
        printf("* 23. Supprimer un service             *\n");
        printf("****************************************\n");
        printf("*             24. Quitter              *\n");
        printf("****************************************\n");
        printf("Votre choix: ");
        scanf("%d", &choice);
        while (getchar() != '\n'); // Vider le buffer après scanf

        switch (choice) {
            case 1:
                addCity();
                 break;
            case 2:
                selectCityByName();
                break;
            case 3:
                displayCities();
                break;
            case 4:
                removeCityByName();
                break;
            case 5:
                // Vérification que la ville a été sélectionnée avant de permettre la sélection d'un hôtel
                if (strcmp(selectedCityName, "") == 0) {
                    printf("Veuillez d'abord sélectionner une ville par son nom.\n");
                } else {
                    selectHotel();// Fonction pour sélectionner un hôtel
                }
                break;
            case 6:
                addHotel();
                break;
            case 7:
                displayHotels();
                break;
            case 8:
                editHotelState();
                break;
            case 9:
                {
                    char hotelName[50];
                    printf("Entrez le nom de l'hotel a supprimer: ");
                    scanf("%s", hotelName);
                    removeHotel(hotelName);
                }
                break;
            case 10:
                reserveRoom();
                break;
            case 11:
                displayReservations();
                break;
            case 12:
                modifyReservation();
                break;
            case 13:
                removeReservation();
                break;
            case 14:
                addRoom();
                break;
            case 15:
                displayAvailableRooms();
                break;
            case 16:
                removeRoom();
                break;
            case 17:
                addClient();
                break;
            case 18:
                displayClients();
                break;
            case 19:
                removeClient();
                break;
            case 20:
                addService();
                break;
            case 21:
                displayServices();
                break;
            case 22:
                associateServiceToHotel();
                break;
            case 23:
                removeService();
                break;
           /* case 22:
                addCity();
                 break;*/
            case 24:
                printf("Quitter le programme.\n");
                break;
            default:
                printf("Choix invalide, veuillez reessayer.\n");
        }
    } while (choice != 24);

    return 0;
}

