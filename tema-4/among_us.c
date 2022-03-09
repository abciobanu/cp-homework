/* Ciobanu Andrei-Bogdan 312CB */

#include "utils.h"

/* numarul maxim de octeti pe care un int ii poate ocupa ca sir de caractere */
#define MAX_LEN_STR_INT ((8 * sizeof(char) * sizeof(int) - 1) / 3 + 2)

void* (* Abilities[4])(void* x) = {
	RotateMatrix, DecodeString, InvertArray, KillPlayer
};

char *fromEnumtoString(PlayerRoles playerRole)
{
	char *str = calloc(MAX_LEN_STR_ATR, sizeof(char));
	switch(playerRole)
	{
		case Rotator:
			str = strcpy(str, "Rotator");
			break;
		case Decoder:
			str = strcpy(str, "Decoder");
			break;
		case Invertor:
			str = strcpy(str, "Invertor");
			break;
		case Impostor:
			str = strcpy(str, "Impostor");
			break;
	}
	return str;
}

// Task 1
void *RotateMatrix(void *input)
{
	int size = *(int *)input;

	int *matrix = malloc(size * size * sizeof(int));
	if (matrix == NULL) {
		return NULL;
	}

	/* populare matrice sub forma deja rotita */
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			int padding = 10;
			while (i + 1 >= padding) {
				padding *= 10;
			}

			matrix[i * size + j] = (size - j) * padding + i + 1;
		}
	}

	char *output = malloc(size * size * MAX_LEN_STR_INT * sizeof(char));
	if (output == NULL) {
		free(matrix);

		return NULL;
	}
	output[0] = '\0';

	/* formare sir de caractere pentru output */
	int offset = 0;
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			offset += sprintf(output + offset, "%d ", matrix[i * size + j]);
		}

		/* '\n' la sfarsit de rand - exceptie face ultimul rand */
		if (i < size - 1) {
			output[offset] = '\n';
			++offset;
		}
	}

	free(matrix);

	return (void *)output;
}

// Task 2
void *DecodeString(void *input)
{
	char *input_copy = strdup((char *)input);
	if (input_copy == NULL) { 
		return NULL;
	}

	int sum = 0;

	/* calculare suma */
	char *number = strtok(input_copy, "_");
	while (number) {
		sum += atoi(number);
		number = strtok(NULL, "_");
	}

	free(input_copy);

	char *output = malloc(MAX_LEN_STR_INT);
	if (output == NULL) {
		return NULL;
	}
	output[0] = '\0';

	/* suma ca sir de caractere */
	sprintf(output, "%d", sum);

	return (void *)output;
}

//Task 3
void *InvertArray(void *input)
{
	int *array = (int *)input;

	char *output = malloc(array[0] * MAX_LEN_STR_INT);
	if (output == NULL) {
		return NULL;
	}
	output[0] = '\0';

	int offset = 0;

	if (array[0] % 2 == 0) {
		/* lungimea vectorului ramas este numar par */
		for (int i = 1; i < array[0]; i += 2) {
			offset += sprintf(output + offset, "%d %d ", array[i + 1],
								array[i]);
		}
	} else {
		/* lungimea vectorului ramas este numar impar */
		for (int i = array[0]; i >= 1; --i) {
			offset += sprintf(output + offset, "%d ", array[i]);
		}
	}

	output[offset] = '\0';

	return (void *)output;
}

//Task 4
Player *allocPlayer()
{
	Player *new_player = malloc(sizeof(Player));
	if (new_player == NULL) {
		return NULL;
	}

	new_player->name = malloc(MAX_LEN_STR_ATR * sizeof(char));
	if (new_player->name == NULL) {
		free(new_player);

		return NULL;
	}

	new_player->color = malloc(MAX_LEN_STR_ATR * sizeof(char));
	if (new_player->color == NULL) {
		free(new_player->name);
		free(new_player);

		return NULL;
	}

	new_player->hat = malloc(MAX_LEN_STR_ATR * sizeof(char));
	if (new_player->hat == NULL) {
		free(new_player->name);
		free(new_player->color);
		free(new_player);

		return NULL;
	}

	new_player->alive = 1;
	new_player->indexOfLocation = 0;

	return new_player;
}

//Task 4
Game *allocGame()
{
	Game *new_game = malloc(sizeof(Game));
	if (new_game == NULL) {
		return NULL;
	}

	new_game->name = malloc(MAX_LEN_STR_ATR * sizeof(char));
	if (new_game->name == NULL) {
		free(new_game);

		return NULL;
	}

	return new_game;
}

//Task 5
Player *ReadPlayer(FILE *inputFile)
{
	Player *player = allocPlayer();
	if (player == NULL) {
		return NULL;
	}

	fscanf(inputFile, "%s", player->name);
	fscanf(inputFile, "%s", player->color);
	fscanf(inputFile, "%s", player->hat);
	fscanf(inputFile, "%d", &player->numberOfLocations);

	player->locations = malloc(player->numberOfLocations * sizeof(Location));
	if (player->locations == NULL) {
		free(player->name);
		free(player->color);
		free(player->hat);
		free(player);

		return NULL;
	}

	/* zona de memorie pentru un sir de caractere de forma "(x,y)" */
	char *coordinates = malloc(2 * MAX_LEN_STR_INT);
	if (coordinates == NULL) {
		free(player->name);
		free(player->color);
		free(player->hat);
		free(player->locations);
		free(player);

		return NULL;
	}

	for (int i = 0; i < player->numberOfLocations; ++i) {
		fscanf(inputFile, "%s", coordinates); /* pereche de coordonate */

		char *coord = strtok(coordinates, "(,)");
		player->locations[i].x = atoi(coord);

		coord = strtok(NULL, "(,)");
		player->locations[i].y = atoi(coord);
	}

	char *role = malloc(MAX_LEN_STR_ATR * sizeof(char));
	if (role == NULL) {
		free(player->name);
		free(player->color);
		free(player->hat);
		free(player->locations);
		free(player);

		return NULL;
	}

	fscanf(inputFile, "%s", role);

	if (strcmp(role, "Rotator") == 0) {
		player->playerRole = Rotator;
		player->ability = Abilities[Rotator];
	} else if (strcmp(role, "Decoder") == 0) {
		player->playerRole = Decoder;
		player->ability = Abilities[Decoder];
	} else if (strcmp(role, "Invertor") == 0) {
		player->playerRole = Invertor;
		player->ability = Abilities[Invertor];
	} else if (strcmp(role, "Impostor") == 0) {
		player->playerRole = Impostor;
		player->ability = Abilities[Impostor];
	}

	free(coordinates);
	free(role);

	return player;
}

// Task 5
Game *ReadGame(FILE *inputFile)
{
	Game *game = allocGame();
	if (game == NULL) {
		return NULL;
	}

	fscanf(inputFile, "%s", game->name);
	fscanf(inputFile, "%d", &game->killRange);
	fscanf(inputFile, "%d", &game->numberOfCrewmates);

	game->crewmates = malloc(game->numberOfCrewmates * sizeof(Player *));
	if (game->crewmates == NULL) {
		free(game->name);
		free(game);

		return NULL;
	}

	for (int i = 0; i < game->numberOfCrewmates; ++i) {
		game->crewmates[i] = ReadPlayer(inputFile);
		if (game->crewmates[i] == NULL) {
			free(game->name);
			free(game->crewmates);
			free(game);

			return NULL;
		}
	}

	game->impostor = ReadPlayer(inputFile);
	if (game->impostor == NULL) {
		free(game->name);
		free(game->crewmates);
		free(game);

		return NULL;
	}

	return game;
}

// Task 6
void WritePlayer(Player *player, FILE *outputFile)
{
	char *role = fromEnumtoString(player->playerRole);
	fprintf(outputFile, "Player %s with color %s, hat %s and role %s has "
			"entered the game.\n", player->name, player->color, player->hat,
			role);

	free(role);

	fprintf(outputFile, "Player's locations:");
	for (int i = 0; i < player->numberOfLocations; ++i) {
		fprintf(outputFile, " (%d,%d)", player->locations[i].x,
				player->locations[i].y);
	}
	fprintf(outputFile, "\n");
}

// Task 6
void WriteGame(Game *game, FILE *outputFile)
{
	fprintf(outputFile, "Game %s has just started!\n", game->name);

	fprintf(outputFile, "\tGame options:\n");
	fprintf(outputFile, "Kill Range: %d\n", game->killRange);
	fprintf(outputFile, "Number of crewmates: %d\n", game->numberOfCrewmates);
	fprintf(outputFile, "\n");

	fprintf(outputFile, "\tCrewmates:\n");
	for (int i = 0; i < game->numberOfCrewmates; ++i) {
		WritePlayer(game->crewmates[i], outputFile);
	}

	fprintf(outputFile, "\n\tImpostor:\n");
	WritePlayer(game->impostor, outputFile);
}

/* returneaza distanta Manhattan dintre doi jucatori */
int get_distance(Player *p1, Player *p2)
{
	int p1_x = p1->locations[p1->indexOfLocation].x;
	int p1_y = p1->locations[p1->indexOfLocation].y;
	int p2_x = p2->locations[p2->indexOfLocation].x;
	int p2_y = p2->locations[p2->indexOfLocation].y;

	return abs(p1_x - p2_x) + abs(p1_y - p2_y);
}

//Task 7
void *KillPlayer(void *input)
{
	Game *game = (Game *)input;

	int min_distance = game->killRange + 1;
	Player *next_victim = NULL;

	for (int i = 0; i < game->numberOfCrewmates; ++i) {
		if (game->crewmates[i]->alive == 1) {
			int distance = get_distance(game->crewmates[i], game->impostor);
			if (distance <= game->killRange && distance <= min_distance) {
				/* s-a gasit o (mai buna) posibila viitoare victima */
				next_victim = game->crewmates[i];
				min_distance = distance;
			}
		}
	}

	char *output = malloc(MAX_LEN_STR_OUT * sizeof(char));
	if (output == NULL) {
		return NULL;
	}

	if (next_victim == NULL) {
		sprintf(output, "Impostor %s couldn't kill anybody.",
				game->impostor->name);
	} else {
		next_victim->alive = 0;

		sprintf(output, "Impostor %s has just killed crewmate %s from "
				"distance %d.", game->impostor->name, next_victim->name,
				min_distance);
	}

	return (void *)output;
}

// Task 8
void CalcuateNextCycleOfGame(Game *game, FILE *outputFile, void **inputMatrix)
{
	for (int i = 0; i < game->numberOfCrewmates; ++i) {
		Player *crewmate = game->crewmates[i];

		if (crewmate->alive == 0) {
			fprintf(outputFile, "Crewmate %s is dead.\n", crewmate->name);
		} else {
			/* actualizeaza indicele de locatie - miscare circulara */
			crewmate->indexOfLocation = (crewmate->indexOfLocation + 1) %
										crewmate->numberOfLocations;

			fprintf(outputFile, "Crewmate %s went to location (%d,%d).\n",
					crewmate->name,
					crewmate->locations[crewmate->indexOfLocation].x,
					crewmate->locations[crewmate->indexOfLocation].y);

			fprintf(outputFile, "Crewmate %s's output:\n", crewmate->name);

			/* sirul de caractere returnat de abilitate */
			char *ability_output = crewmate->ability(inputMatrix[i]);

			fprintf(outputFile, "%s\n", ability_output);
			free(ability_output);
		}
	}

	/* actualizeaza indicele de locatie - miscare circulara */
	game->impostor->indexOfLocation = (game->impostor->indexOfLocation + 1) %
										game->impostor->numberOfLocations;

	fprintf(outputFile, "Impostor %s went to location (%d,%d).\n",
			game->impostor->name,
			game->impostor->locations[game->impostor->indexOfLocation].x,
			game->impostor->locations[game->impostor->indexOfLocation].y);

	fprintf(outputFile, "Impostor %s's output:\n", game->impostor->name);

	void *ability_input = inputMatrix[game->numberOfCrewmates];
	char *ability_output = game->impostor->ability(ability_input);

	fprintf(outputFile, "%s\n", ability_output);
	free(ability_output);
}

// Task 9
void FreePlayer(Player *player)
{
	free(player->name);
	free(player->color);
	free(player->hat);
	free(player->locations);

	free(player);
}

// Task 9
void FreeGame(Game *game)
{
	free(game->name);

	for (int i = 0; i < game->numberOfCrewmates; ++i) {
		FreePlayer(game->crewmates[i]);
	}
	free(game->crewmates);

	FreePlayer(game->impostor);

	free(game);
}