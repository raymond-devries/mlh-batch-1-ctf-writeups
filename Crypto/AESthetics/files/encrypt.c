#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <openssl/aes.h>

struct ctr_state {
	uint8_t key_bytes[16];
	uint8_t counter[16];
};

/* helper to print hex strings */
void
print_hex(uint8_t *buf, size_t len)
{
	for (size_t i = 0; i < len; i++) {
		fprintf(stderr, "%02x", buf[i]);
	}
	fprintf(stderr, "\n");
}

/* helper to increment the 128-bit counter */
void
inc_counter(uint8_t *counter)
{
	uint8_t *pos = counter + 15;
	while (pos > counter) {
		if (++(*pos) != 0x00) {
			return;
		}
		pos--;
	}
	(*pos)++;
}

/* block-level counter mode encryption.
 * takes the ctr mode state, a 128-bit block to encrypt, and a pointer
 * to a 128-bit output buffer.
 */
void
ctr_block_encrypt(struct ctr_state state, uint8_t *in, uint8_t *out)
{
	AES_KEY key;
	uint8_t pad[16];

	/* encrypt the counter with AES */
	AES_set_encrypt_key(state.key_bytes, 128, &key);
	AES_encrypt(state.counter, pad, &key);

	/* xor the plaintext with the pad */
	for (int i = 0; i < 16; i++) {
		out[i] = in[i] ^ pad[i];
	}

	/* increment the counter */
	inc_counter(state.counter);
}

int
main(int argc, char **argv)
{
	struct ctr_state state;
	uint8_t input_buffer[16];
	uint8_t output_buffer[16];
	ssize_t bytes_read;
	size_t pos = 0;

	/* generate a random key */
	arc4random_buf(state.key_bytes, 16); /* sorry, linux users! */
	fprintf(stderr, "your super secure random key: ");
	print_hex(state.key_bytes, 16);

	/* start the counter at a random value; write it out as the
	 * first 16 bytes of the output file (the counter is not
	 * secret, only the key is!)
	 */
	arc4random_buf(state.counter, 16);
	write(1, state.counter, 16);

	/* initial zeroing of input buffer */
	memset(input_buffer, 0x00, 16);

	/* read a byte at a type and encrypt 16-byte blocks */
	while ((bytes_read = read(0, input_buffer+pos, 1)) > 0) {
		if (pos < 15) {
			pos++;
			continue;
		} else {
			ctr_block_encrypt(state, input_buffer, output_buffer);
			write(1, output_buffer, 16);
			memset(input_buffer, 0x00, 16);
			pos = 0;
		}
	}

	/* we've reached EOF; encrypt any leftover bytes */
	if (pos != 0) {
		ctr_block_encrypt(state, input_buffer, output_buffer);
		write(1, output_buffer, pos);
	}

	return 0;
}
