/* Ciobanu Andrei-Bogdan 312CB */

#include "gates.h"

#include <stdio.h>
#include <assert.h>

/* Task 1 - Bit by Bit */

uint8_t get_bit(uint64_t nr, uint8_t i)
{
    assert(i <= 8 * sizeof nr);

    uint8_t res = -1;

    /* TODO
     *
     * "res" should be 1 if the bit is active, else 0
     */
    uint64_t bitmask = (uint64_t)1 << i;
    res = (nr & bitmask) ? 1 : 0;

    return res;
}


uint64_t flip_bit(uint64_t nr, uint8_t i)
{
    assert(i <= 8 * sizeof nr);

    uint64_t res = -1;

    /* TODO
     *
     * Return the "nr" with the ith bit flipped
     */
    uint64_t bitmask = (uint64_t)1 << i;
    res = nr ^ bitmask;

    return res;
}


uint64_t activate_bit(uint64_t nr, uint8_t i)
{
    assert(i <= 8 * sizeof nr);

    uint64_t res = 0xFF;

    /* TODO
     *
     * Return the "nr" with the ith bit "1"
     */
    uint64_t bitmask = (uint64_t)1 << i;
    res = nr | bitmask;

    return res;
}


uint64_t clear_bit(uint64_t nr, uint8_t i)
{
    assert(i <= 8 * sizeof nr);

    uint64_t res = -1;

    /* TODO
     *
     * Return the "nr" with the ith bit "0"
     */
    uint64_t bitmask = ~((uint64_t)1 << i);
    res = nr & bitmask;

    return res;
}


/* Task 2 - One Gate to Rule Them All */

uint8_t nand_gate(uint8_t a, uint8_t b)
{
    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);

    return !(a & b);
}


uint8_t and_gate(uint8_t a, uint8_t b)
{
    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);

    uint8_t res = -1;

    /* TODO - Use the nand gate to implement the and gate */
    res = nand_gate(nand_gate(a, b), nand_gate(a, b));

    return res;
}


uint8_t not_gate(uint8_t a)
{
    assert (a == 0 || a == 1);

    uint8_t res = -1;

    /* TODO - Use the nand gate to implement the not gate */
    res = nand_gate(a, a);

    return res;
}


uint8_t or_gate(uint8_t a, uint8_t b)
{
    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);

    uint8_t res = -1;

    /* TODO - Use the previously defined gates to implement the or gate */
    res = nand_gate(not_gate(a), not_gate(b));

    return res;
}


uint8_t xor_gate(uint8_t a, uint8_t b)
{
    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);

    uint8_t res = -1;

    /* TODO - Use the previously defined gates to implement the xor gate */
    uint8_t input1 = nand_gate(a, nand_gate(a, b));
    uint8_t input2 = nand_gate(b, nand_gate(a, b));
    res = nand_gate(input1, input2);

    return res;
}


/* Task 3 - Just Carry the Bit */

uint8_t full_adder(uint8_t a, uint8_t b, uint8_t c)
{
    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);
    assert (c == 0 || c == 1);

    uint8_t res = -1;

    /* TODO - implement the full_adder using the previous gates
     * Since the full_adder needs to provide 2 results, you should
     * encode the sum bit and the carry bit in one byte - you can encode
     * it in whatever way you like
     */

    /* sum = a ^ b ^ c */
    int8_t sum = 0;
    sum = xor_gate(a, xor_gate(b, c));

    /* carry = b*c + a*c + a*b */
    int8_t carry = 0;
    carry = or_gate(and_gate(b, c), and_gate(a, c));
    carry = or_gate(carry, and_gate(a, b));

    res = sum;
    if (carry) {
        res = activate_bit(res, 1);
    }

    return res;
}


uint64_t ripple_carry_adder(uint64_t a, uint64_t b)
{
    uint64_t res = 0;

    /* TODO
     * Use the full_adder to implement the ripple carry adder
     * If there is ANY overflow while adding "a" and "b" then the
     * result should be 0
     */
    uint8_t carry = 0;

    for (size_t i = 0; i < sizeof(uint64_t) * 8; ++i) {
        uint8_t bit_a = get_bit(a, i);
        uint8_t bit_b = get_bit(b, i);
        uint8_t bit_sum = full_adder(bit_a, bit_b, carry);

        if (get_bit(bit_sum, 0)) {
            res = activate_bit(res, i);
        }

        carry = get_bit(bit_sum, 1);
    }

    /* check for overflow */
    if (carry) {
        res = 0;
    }

    return res;
}
