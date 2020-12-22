## The theory
```
Cryptography is beautiful, isn't it? You just have to use it right.
```
[secret-flag.png.crypt](https://github.com/raymond-devries/mlh-batch-1-ctf-writeups/blob/main/Crypto/AESthetics/files/secret-flag.png.crypt) [encrypt.c](https://github.com/raymond-devries/mlh-batch-1-ctf-writeups/blob/main/Crypto/AESthetics/files/encrypt.c)

This challenge was on a different level, not gonna lie. I mean, to veteran CTF-ers, this would be medium (?) at best, but it was an absolute wreck for our team, as all of us were beginners. But the best part, we eventually solved it, right as the CTF was ending, tying in points but 4 minutes later than another team. Quite dramatic, yes.

I had a serious love-hate relationship with this relationship but I probably learnt the *most* from this one. The first step to solving this challenge was to take apart the source code with a fine toothpick and secondly, understand how PNGs work. Since I am how I am, I did neither and immediately dug into the different kinds of possible attacks, known-plaintext, chosen-plaintext, forbidden whatnot (well, it was somewhat like a forbidden known-plaintext attack).

So, the first step! Research. I read up on how block ciphers work and more importantly, the counter mode - really what that means is that I read a [bunch of pages](https://en.wikipedia.org/wiki/Block_cipher_mode_of_operation#Counter_(CTR)) on Wikipedia. 🥱 But seriously, it was kind of worth it. In particular, I noticed the paragraph about known-plaintext attack but didn't understand anything because, yeah, cryptography, amirite.

But I read this and that, one-time pad attack, two-time pad, all of that (yes, I was basking in my ignorance). All of that until the first hint was released:

> the CTR implementation is buggy

Despite knowing this, I had never actually figured out what was the bug - because I assumed that the bug was actually the code itself, which itself was only buggy because it was written with OpenBSD libraries not available on standard Linux distros. Ugh. (And yeah, those took hours to fix and script).

Eventually, I got around to seeing the `inc_counter()` function in the script and had a very vague suspicion if it was actually written correctly because of they way it was written, I was very sure it was not changing the counter at all (that was not the actual problem apparently). The issue was with the `ctr_block_encrypt()` function parameter which receives a copy of the `struct` instead of a pointer to it:
```c
ctr_block_encrypt(struct ctr_state state, uint8_t *in, uint8_t *out)`
{
	...
}
```
So, I set to debugging with my favourite debugger, `fprintf`. I wrapped them around the function invocation, and lo and behold, no increment! The nonce was being reused. This opens us up to the world of attacks based on nonce-reuse. The part after this was actually more painful - figuring how to *guess* the plaintext to actually get the keystream - and then the decrypted file.

### PNGs say what?
I was quite sure that this would have to be the way to decrypt the file because we know how they all have a unique file signature. I retrieved the hex values from the W3C PNG [spec site](http://www.libpng.org/pub/png/spec/1.2/PNG-Rationale.html#R.PNG-file-signature) and got to work.
Using a crib dragging attack (a "crib" is known or guessed plaintext, making this a variant of a known-plaintext attack), I figured out the first half of the key, I got `5c 92 d5 10 d0 63 20 9d 00 00 00 00 00 00 00`. But I was truly stuck on the rest of the key, or so I thought - but I discovered that each PNG file at least has two other *critical* chunks - IHDR and IEND.
Using the IEND (+ CRC) bytes, I used crib dragging to get a bit more out of the key, `5c 92 d5 10 d0 63 20 9d 34 cf 24 95 00 00 00 00` - finally, all that was left was to fit in IHDR into an appropriate block allowing us to guess the whole key.

![](https://dev-to-uploads.s3.amazonaws.com/i/pieypzhde97knuepirc8.png)

We then dump the decrypted bytes into a .txt file, clean up the file to meet PNG specs (and remove the counter bytes), and open it to retrieve the flag!

![](https://dev-to-uploads.s3.amazonaws.com/i/677str27kwpazi19zg69.png)

And we can finally get the flag with our own eyes - `mlh{n0nc3_m34ns_numb3r_u53d_ONCE}`. Truly a thrilling end to this journey (I remember screaming after getting this flag). 🏁
