# CryptoToy
Simple Android App to demonstrate low-level encryption calls to `libcrypto_1_0_2.so`.

The App doesn't use `EVP_*` functions, instead it uses direct calls to `AES_cbc_encrypt()` and `AES_decrypt()`.

Expected output for `adb logcat | grep FOO`, after both buttons were pressed:
```
OpenSSL 1.0.2 22 Jan 2015 (ABI: armeabi-v7a/NEON)
AES_cbc_encrypt()
Original : 01 02 03 04 01 02 03 04 01 02 03 04 01 02 03 04
Encrypted: 3b c0 da 12 01 a1 bb 97 0c a2 64 b9 f1 62 70 6d
Decrypted: 01 02 03 04 01 02 03 04 01 02 03 04 01 02 03 04
AES_decrypt()
Original : 01 02 03 04 01 02 03 04 01 02 03 04 01 02 03 04
Encrypted: 3b c0 da 12 01 a1 bb 97 0c a2 64 b9 f1 62 70 6d
Decrypted: 01 02 03 04 01 02 03 04 01 02 03 04 01 02 03 04
```
