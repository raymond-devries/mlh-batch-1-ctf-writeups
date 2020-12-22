## The Misc.
```
Silent Night, Holy Night, All is calm all is well. 
Well, jingle bells are all around the town and Karan is
reminiscing the time he spent with Santa last year.
Going through images and reviews he left around town,
he wants you to visit and have a glimpse of his fun times.
The ticket that leads you to the place is within the
registration email. 
Category: Misc.
```
> [Link](https://www.youtube.com/watch?v=Cp4RRAEgpeU)

The "...ticket that leads you to the place is within the
registration email" is a pretty blatant hint, I can just search for the email using filters.

![](https://dev-to-uploads.s3.amazonaws.com/i/4jjhuvz74r5ahsrd25sc.png)

Once you click on reply to the email and hover over the email ID, you should get a dynamically generated hovercard, which contains the UID of the user. Then, you need to make an educated guess about where to go next but the fact that we have a video of the Santa Village and you needed the ***Gmail*** address should tell you where to go next - of course, Google Maps. In fact, you could've just looked up the reviews of the village in the course of the investigation (as a lot of people did) and even found Karan's reviews (which also a lot of people did).

The approach was to basically access the user's map contributions by going to `https://www.google.com/maps/contrib/<user-id>` and finding the reviews (well, not exactly).

But as has been said, it's a *miscellaneous* challenge, not an *OSINT* one.
The second part is a steganography challenge and it's not immediately clear sadly. But hey, sometimes you gotta try everything. The flag was actually (very well) hidden in one of the images (out of two) uploaded with the reviews (out of two). I was eventually able to extract the flag with PCA with relative ease, just took a while because it can be *literally* anywhere.

![](https://dev-to-uploads.s3.amazonaws.com/i/otfc7jh3jq0p2r5htr62.png)

And there, we find the flag - `mlh{M3rry_Chr1stmas}` 🆙
