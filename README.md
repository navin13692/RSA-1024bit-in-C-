# RSA 1024bit in C for Embedded Systems

Simple RSA 1024bit library to encrypt and decrypt data using private public key. 
Best part of it, it does not have any dependancies otherthen basic standard libraries that Embedded system environments have. 
Specially made for Embedded systems.

Here 1024bits are devided in array of 16 word with 64bit length.
Care must be taken as 0th element of all variables are considered as least significant.
Although for 1024bit it need only 16 64bit-word, all variables must be decleard with 18 words and must be initialized to 0.
These extra 3 words are used for processing as no function calls malloc(), which is more suitable for embedded systems.

<i>Algorythem used for multiplication and modular division are elementry and need to be updated for faster performance, even though performance now is up to acceptable level and depends highly on machine. Please let me know if you have any suggestions</i>

Auther:<br>
Navin Maheshwari<br>
<i>navin13692@gmail.com</i>
