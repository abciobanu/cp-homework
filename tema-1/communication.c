/* Ciobanu Andrei-Bogdan 312CB */

#include "communication.h"
#include "util_comm.h"

#include <stdio.h>


/* Task 1 - The Beginning */

void send_byte_message(void)
{
    /* TODO
     * Send the encoding of the characters: R, I, C, K
     */
    const char msg[] = "RICK";
    for (uint8_t i = 0; i < 4; ++i) {
        send_squanch(msg[i] - 'A' + 1);
    }
}


void recv_byte_message(void)
{
    /* TODO
     * Receive 5 encoded characters, decode them and print
     * them to the standard output (as characters)
     *
     * ATTENTION!: Use fprintf(stdout, ...)
     */
    for (uint8_t i = 0; i < 5; ++i) {
        uint8_t ch = recv_squanch();
        fprintf(stdout, "%c", ch + 'A' - 1);
    }
}


void comm_byte(void)
{
    /* TODO
     * Receive 10 encoded characters and send each character (the character is
     * still encoded) 2 times
     */
    for (uint8_t i = 0; i < 10; ++i) {
        uint8_t ch = recv_squanch();
        send_squanch(ch);
        send_squanch(ch);
    }
}


/* Task 2 - Waiting for the Message */

void send_message(void)
{
    /* TODO
     * Send the message: HELLOTHERE
     * - send the encoded length
     * - send each character encoded
     */
    const char msg[] = "HELLOTHERE";
    send_squanch(10 << 2);  /* send the encoded length */
    for (uint8_t i = 0; i < 10; ++i) {
        send_squanch(msg[i] - 'A' + 1);
    }
}


void recv_message(void)
{
    /* TODO
     * Receive a message:
     * - the first value is the encoded length
     * - length x encoded characters
     * - print each decoded character
     * 
     * ATTENTION!: Use fprintf(stdout, ...)
     */
    uint8_t length = recv_squanch();

    /* decode the length */
    length <<= 2;
    length >>= 4;

    fprintf(stdout, "%d", length);

    /* decode and print each character */
    for (uint8_t i = 0; i < length; ++i) {
        uint8_t ch = recv_squanch();
        fprintf(stdout, "%c", ch + 'A' - 1);
    }
}


void comm_message(void)
{
    /* TODO
     * Receive a message from Rick and do one of the following depending on the
     * last character from the message:
     * - 'P' - send back PICKLERICK
     * - anything else - send back VINDICATORS
     * You need to send the messages as you did at the previous tasks:
     * - encode the length and send it
     * - encode each character and send them
     */
    uint8_t recv_length = recv_squanch();

    /* decode the length */
    recv_length <<= 2;
    recv_length >>= 4;

    /* ignore all characters except the last one */
    for (uint8_t i = 0; i < recv_length - 1; ++i) {
        recv_squanch();
    }

    uint8_t ch = recv_squanch();    /* last character from the message */
    if (ch == ('P' - 'A' + 1)) {
        uint8_t send_length = 10;
        send_squanch(send_length << 2); /* send the encoded length */

        char msg[] = "PICKLERICK";
        for (uint8_t i = 0; i < send_length; ++i) {
            send_squanch(msg[i] - 'A' + 1);
        }
    } else {
        uint8_t send_length = 11;
        send_squanch(send_length << 2); /* send the encoded length */

        char msg[] = "VINDICATORS";
        for (uint8_t i = 0; i < send_length; ++i) {
            send_squanch(msg[i] - 'A' + 1);
        }
    }
}


/* Task 3 - In the Zone */

void send_squanch2(uint8_t c1, uint8_t c2)
{
    /* TODO
     * Steps:
     * - "merge" the character encoded in c1 and the character encoded in c2
     * - use send_squanch to send the newly formed byte
     */
    uint8_t msg = 0;
    for (uint8_t i = 0; i < sizeof(uint8_t) * 8; ++i) {
        uint8_t bit_c1 = c1;
        uint8_t bit_c2 = c2;

        /* delete unnecessary bits on the left and right */
        bit_c1 >>= i;
        bit_c1 <<= (sizeof(uint8_t) * 8 - 1);

        bit_c2 >>= i;
        bit_c2 <<= (sizeof(uint8_t) * 8 - 1);

        /* move the bits to the correct position */
        bit_c1 >>= (sizeof(uint8_t) * 8 - 1 - i * 2 - 1);
        bit_c2 >>= (sizeof(uint8_t) * 8 - 1 - i * 2);

        msg = msg | bit_c1 | bit_c2;    /* build the encoded byte */
    }

    send_squanch(msg);
}


uint8_t decode_squanch2(uint8_t c)
{
    /*
     * Decode the given byte:
     * - split the two characters as in the image from ocw.cs.pub.ro
     */

    uint8_t res = 0;

    /* TODO */
    for (uint8_t i = 0; i < sizeof(uint8_t) * 8; i += 2) {
            uint8_t bit_c1 = c;
            uint8_t bit_c2 = c;

            /* delete unnecessary bits on the left and right */
            bit_c1 >>= (i + 1);
            bit_c1 <<= (sizeof(uint8_t) * 8 - 1);

            bit_c2 >>= i;
            bit_c2 <<= (sizeof(uint8_t) * 8 - 1);

            /* move the bits to the correct position */
            bit_c1 >>= (sizeof(uint8_t) * 4 - 1 - i / 2);
            bit_c2 >>= (sizeof(uint8_t) * 8 - 1 - i / 2);

            res = res | bit_c1 | bit_c2;    /* build the decoded byte */
    }

    return res;
}
