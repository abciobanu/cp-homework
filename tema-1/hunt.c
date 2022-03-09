/* Ciobanu Andrei-Bogdan 312CB */

#include "hunt.h"
#include <stdio.h>

/* Task 1 - Axii */

uint16_t find_spell(uint64_t memory)
{
    /*
     * The spell is 16 bits and they are in the memory exactly after 5
     * consecutive bits of 1:
     *
     *            high                                             low
     *                [][][x]...[x][x][x][1][1][1][1][1][]...[][][]
     *                    |----16bits---|
     *                          Spell
     *
     * There is only 1 group of 5 consecutive bits of 1 and the other groups
     * have less than consecutive bits of 1
     * Steps:
     * - find 5 consecutive bits of 1
     * - the NEXT 16 bits == spell
     */

    uint16_t res = -1;

    /* TODO */
    uint8_t consecutive_bits = 0;
    while (memory) {
        if (memory % 2) {
            ++consecutive_bits;
        } else if (consecutive_bits) {
            consecutive_bits = 0;
        }

        if (consecutive_bits == 5) {
            memory >>= 1;
            break;
        }

        memory >>= 1;
    }

    res = (uint16_t)memory;

    return res;
}


uint16_t find_key(uint64_t memory)
{
    /*
     * The key is 16 bits and they are in the memory exactly before 3
     * consecutive bits of 1:
     *
     *                high                                           low
     *                     [][][][1][1][1][x][x]...[x][x][]...[][][]
     *                                    |----16bits---|
     *                                           Key
     * Steps:
     * - find 3 consecutive bits of 1
     * - the PREVIOUS 16 bits == key
     */

    uint16_t res = -1;

    /* TODO */
    uint64_t memory_copy = memory;
    uint8_t consecutive_bits = 0;
    uint8_t key_position = 0;
    while (memory_copy) {
        if (memory_copy % 2) {
            ++consecutive_bits;
        } else if (consecutive_bits) {
            consecutive_bits = 0;
        }

        if (consecutive_bits == 3) {
            key_position -= 18; /* 16 (the key) + 2 (consecutive bits of 1) */
            break;
        }

        ++key_position;
        memory_copy >>= 1;
    }

    memory >>= key_position;
    res = (uint16_t)memory;

    return res;
}


uint16_t decrypt_spell(uint16_t spell, uint16_t key)
{
    /*
     * Find the spell knowing that
     * spell_encrypted = spell_plaintext ^ key
     */

    uint16_t res = -1;

    /* TODO */
    res = spell ^ key;

    return res;
}


/* Task 2 - Sword Play */

uint32_t choose_sword(uint16_t enemy)
{
    /*
     * Help Geralt construct a sword that will give him an advantage in battle.
     * The steps we will follow:
     * - choose the corect sword
     * - brand the sword with the correct runes such that the equation
     * for that specific enemy type is true
     *
     * How does the sword look (Sword template):
     *  [][][][] [0][0][0]...[0][0][0] [][][][][][][][][][][][][][][][]
     *  -------- --------------------- --------------------------------
     *     Type    Does not matter      The "Value" such that the equation
     *                               from 2 holds depending on the enemy type
     *    4 bits      12 bits                     16 bits
     *
     * 1. Type of the sword:
     * - 1001 - Silver Sword - for monsters
     * - 0110 - Steel Sword - for humans
     *
     *  If the number of active bits from variable "enemy" is even == Monster
     *  else == Human
     *
     * 2.
     * Monster Equation:
     *    Value ^ (Enemy & (1 - Enemy)) = 0
     *
     * Human Equation:
     *    Value + Enemy = 0
     */

    uint32_t res = -1;

    /* TODO */
    uint16_t enemy_copy = enemy;
    uint8_t active_bits = 0;
    while (enemy_copy) {
        if (enemy_copy % 2) {
            ++active_bits;
        }

        enemy_copy >>= 1;
    }

    if (active_bits % 2) {
        res = 6;    /* 0110 - Steel Sword */
        res <<= (sizeof(uint32_t) * 8 - 4);

        uint16_t value = ~enemy + 1;
        res |= value;
    } else {
        res = 9;    /* 1001 - Silver Sword */
        res <<= (sizeof(uint32_t) * 8 - 4);

        uint16_t value = enemy & (1 - enemy);
        res |= value;
    }

    return res;
}


/* Task 3 - The Witcher Trials */

uint32_t trial_of_the_grasses(uint16_t cocktail)
{
    /*
     * To become a witcher one must survive a cocktail of different
     * herbs. The body should generate the correct antibodies to
     * neutralize the given cocktail.
     *
     *
     * The antibodies need to respect the following properties:
     *   (antibodies_high & cocktail) ^ (antibodies_low | cocktail) = 0
     *   antibodies_low & antibodies_high = 0
     *   antibodies_low | antibodies_high = cocktail
     *
     * Where:
     *  [][][]...[][][] | [][][]...[][][]
     *  ---------------   ---------------
     *  antibodies_high    antibodies_low
     *      16 bits           16 bits
     *      -------------------------
     *              antibodies
     */

    uint32_t res = -1;

    /* TODO */
    res = cocktail << sizeof(uint32_t) * 4;

    return res;
}


uint8_t trial_of_forrest_eyes(uint64_t map)
{
    /*
     * For the next trail, the candidate is tied up and blindfolded.
     * They are also taken to a forest and they should return until
     * the next morning.
     *
     * The candidate knows that there are 4 possibilities for the forest:
     * Brokilon, Hindar, Wolven Glade and Caed Dhu.
     *
     * The candidate also knows those forests by heart, but first you
     * need to identify the forest.
     *
     * The characteristics for 3 of the forests are as follows:
     * - Brokilon - has ONLY groups of 4 trees (or a forest without any tree)
     *   Ex: a group of 4 trees: "...00111100..."
     * - Hindar - has ONLY 2 trees in the MIDDLE
     * - Wolven Glade - FULL of trees
     *
     * The "map" variable encodes the following information:
     * - 1 tree
     * - 0 patch of grass
     *
     * You should put in the "res" variable the index for the identified
     * forrest.
     * - Brokilon - index 0
     * - Hindar - index 1
     * - Wolven Glade - index 2
     * - Caed Dhu - index 3
     */

    /* TODO */
    if (~map == 0) {
        return 2;   /* Wolven Glade */
    }

    uint64_t hindar_mask = (uint64_t)3 << (sizeof(uint64_t) * 4 - 1);
    if (map == hindar_mask) {
        return 1;   /* Hindar */
    }

    /* distinguish between Brokilon and Caed Dhu */
    uint8_t group_size = 0;
    while (map) {
        if (map % 2) {
            ++group_size;
        } else if (group_size == 4) {
            group_size = 0;
        } else if (group_size) {
            return 3;   /* Caed Dhu */
        }

        map >>= 1;
    }

    return 0;   /* Brokilon */
}


uint8_t trial_of_the_dreams(uint32_t map)
{
    /*
     * For this final trial, the candidates go into a trance and when
     * they woke up they would be a fully fledged witcher.
     *
     * For this task, you need to help the witcher find the number
     * of steps that would take the candidate from the dream realm to
     * the real world.
     *
     * The "map" variable would have only 2 bits of 1:
     * - candidate location
     * - portal location
     *
     * You should put in the "res" variable the distance from the
     * candidate position to the portal.
     *
     *  Ex:
     *  [0][0][0][0][0][0]....[1][0][0][0][0][1][0]....[0][0][0]
     *
     *  res = 5
     */

    uint8_t res = 0;

    /* TODO */
    while (map) {
        if (map % 2 && res == 0) {
            ++res;
        } else if (map % 2) {
            break;
        } else if (res > 0) {
            ++res;
        }

        map >>= 1;
    }

    return res;
}
