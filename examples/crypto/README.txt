Readme for "crypto" example

This example performs various cryptographic operations on the sample 
plain text message "Original plaintext message". It performs the following
operations:

1)  Calculate the MD5 of the message
2)  Compare the MD5 with the known correct value
3)  Calculate the SHA1 of the message
4)  Compare the SHA1 with the known correct value
5)  Encrypt the message with 3DES without block chaining.
6)  Decrypt the message, comparing it with the original.
7)  Encrypt the message with 3DES with CBC.
8)  Decrypt the message, comparing it with the original.
9)  Encrypt the message with AES without block chaining.
10) Decrypt the message, comparing it with the original.
11) Encrypt the message with AES with CBC.
12) Decrypt the message, comparing it with the original.


Running
-------

Build and run the sample:

	$ make clean
	$ make
	$ make run


Sample Output
-------------
...

PP0:~CONSOLE-> Plaintext: "Original plaintext message"
PP0:~CONSOLE-> MD5:      1517fa5c261777f967f2c7c034a13fd4
PP0:~CONSOLE-> SHA1:     4964868fb28c4fdb08afa18c462e4c2c53b9c0ca
PP0:~CONSOLE-> 3des:     3fa40e8a984d48153fa40e8a984d4815
PP0:~CONSOLE-> 3des cbc: e5c7cdde872bf27cbc3e81b5aaa5190c
PP0:~CONSOLE-> aes:      f8a4ba8e3feab9b1d63e0d1f67b00808f8a4ba8e3feab9b1d63e0d1f67b00808
PP0:~CONSOLE-> aes cbc:  e5a82dea368354dcb2982f24ed13bc170b4256f2222e06362e54096ace81568f

...

---
